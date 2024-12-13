/*
 * ArcEmu MMORPG Server
 * Copyright (C) 2008-2010 <http://www.ArcEmu.org/>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

//#include "Config/ConfigEnv.h"
#include "Log.h"
//#include "CrashInspector.h"
#include <stdarg.h>

time_t UNIXTIME;

class InternalLogger {
private:
	uint32 m_Cursor = 0;
	static const uint32 StrBufSize = 128 * 1024;
	char m_Buffer[StrBufSize];
public:
	string getLogString() {
		if (m_Cursor > 0) {
			if (m_Cursor + 1 < StrBufSize) {
				m_Buffer[m_Cursor] = '\n';
				m_Cursor += 1;
				m_Buffer[m_Cursor] = '\0';
			}
			return string(m_Buffer);
		}
		else
			return "";
	};

	void writeTime() {
		time_t now = time(NULL);
		tm* lt = localtime(&now);
		if (m_Cursor < StrBufSize)
			m_Cursor += strftime(&m_Buffer[m_Cursor], StrBufSize - m_Cursor, "%F %T ", lt);
	};

	void writeLog(const char* log)
	{
		if (m_Cursor < StrBufSize)
			m_Cursor += snprintf(&m_Buffer[m_Cursor], StrBufSize - m_Cursor, "%s ", log);
	}

	void writeLog(const char* str, va_list args)
	{
		if (m_Cursor < StrBufSize)
			m_Cursor += vsnprintf((char*)&m_Buffer[m_Cursor], StrBufSize - m_Cursor, str, args);
	};
};



const static int TLogColors[TLogLevel::loglevel_max] =
{
	FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY,
	FOREGROUND_RED | FOREGROUND_INTENSITY,
	FOREGROUND_RED | FOREGROUND_INTENSITY,
	FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY,
	FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY,
	FOREGROUND_GREEN | FOREGROUND_INTENSITY,
};

const static char* TLogLevelNames[TLogLevel::loglevel_max] =
{
	"",
	"[Fatal]",
	"[Error]",
	"[Warning]",
	"[Debug]",
	"[Info ]"
};


static_assert(sizeof(TLogColors) / sizeof(TLogColors[0]) == TLogLevel::loglevel_max, "log colors size error");
void oLog::logLooping()
{
	m_Terminated = false;
	SetThreadName("oLog");
	while (!m_Terminated)
	{
		Sleep(2000);
		try {
			if (m_file && m_LogList.size() > 0)
				this->flushLog(this->m_file, this->m_LogList, "Log");
			if (m_Sqlfile && m_FileLogList.size() > 0)
				flushLog(m_Sqlfile, m_FileLogList, "SQL");
		}
		catch (...) {

		}
	}
}

void oLog::flushLog(FILE*& afile, vector<string>& aLogList, string name)
{
	int pos = 0;
	if (aLogList.size() > 0)
	{
		vector<string> list;
		std::unique_lock<std::mutex> locker(m_LogMutext);
		if (aLogList.size() > 0)
		{
			list.swap(aLogList);
		}
		locker.unlock();
		for (auto& str : list)
		{
			int strlen = str.size();
			/* 异常的日志, 单条日志不允许超过512k */
			if (strlen > LogBufferSize)
				continue;

			/* 缓冲区不够用*/
			if (strlen + pos > LogBufferSize)
			{
				writeToFile(afile, buffer, pos, name);
				pos = 0;
			}
			memmove(&buffer[pos], str.data(), strlen);
			pos += strlen;
		}

		/* 如果有数据就写入文件 */
		if (pos > 0)
		{
			writeToFile(afile, buffer, pos, name);
			pos = 0;
		};
		fflush(afile);
	}
}

static void RebasePath()
{
	TCHAR buffer[8192];
	if (::GetModuleFileName(nullptr, buffer, MAX_PATH) != 0) {
		std::string filename = buffer;
		std::string path = ExtractFilePath(filename);
		SetCurrentDirectory(LPCSTR(path.c_str()));
	}
}

string expandLogFilename(const string& logfileName)
{
	RebasePath();
	string relativeName = "log/" + logfileName;
	TCHAR buffer[4096] = {};
	if (::GetModuleFileName(nullptr, buffer, MAX_PATH) != 0) {
		string filename = buffer;
		string path = ExtractFilePath(filename);
		string fullname = path + relativeName;
		return fullname;
	}
	return ExpandFilename(relativeName);
}

bool isFileExists(const string& filename) {
	FILE* f = fopen(filename.c_str(), "r");
	bool existed = f != nullptr;
	if (existed)
		fclose(f);
	return existed;
}

string genLogFilename(string name)
{
	time_t now;
	struct tm* timeinfo = NULL;
	now = time(NULL);
	timeinfo = localtime(&now);
	char fname[200] = {};

	sprintf(fname, "%s%d-%d-%d_%02d%02d%02d.log",
		name.c_str(),
		timeinfo->tm_year + 1900,
		timeinfo->tm_mon + 1,
		timeinfo->tm_mday,
		timeinfo->tm_hour,
		timeinfo->tm_min,
		timeinfo->tm_sec);

	return fname;
}

void oLog::renewLogFile(FILE*& afile, string name)
{
	string logFileName = genLogFilename(name);
	string filename = expandLogFilename(logFileName);

	ForceDirectory(ExtractFilePath(filename));
	if (afile != nullptr)
		fclose(afile);

	afile = fopen(filename.c_str(), "ab+");

	if (afile == NULL)
		fprintf(stderr, "%s: Error opening '%s': %s\n", __FUNCTION__, filename.c_str(), strerror(errno));
}


void oLog::openLogFile(FILE*& afile, string name)
{
	if (afile == nullptr)
		renewLogFile(afile, name);
}

string oLog::logString(TLogLevel level, const char* source, const char* str, va_list args)
{
	InternalLogger logger;
	logger.writeTime();

	if (level > loglevel_always)
		logger.writeLog(TLogLevelNames[level]);

	bool hasSourceString = source && *source;

	if (hasSourceString)
		logger.writeLog(source);

	logger.writeLog(str, args);
	return logger.getLogString();
}

void oLog::writeToFile(FILE*& afile, const char* log, int size, string name)
{
	if (afile != nullptr)
	{
		fpos_t fileSize = 0;
		fgetpos(afile, &fileSize);
		if (fileSize >= m_logFileSizeLimited)
			renewLogFile(afile, name);
		fwrite(log, 1, size, afile);
	}
}

void oLog::outScreen(TLogLevel level, const string& text)
{
	int color = TLogColors[level];
	SetConsoleTextAttribute(stdout_handle, color);
	fprintf(stderr, "%s", text.c_str());
	SetConsoleTextAttribute(stdout_handle, log_color_white);
}

void oLog::outFile(TLogLevel level, const string& text)
{
	std::unique_lock<std::mutex> locker(m_LogMutext);
	if (m_LogList.size() > m_MaxLogCount)
		return;
	m_LogList.push_back(text);
}

void oLog::outLog(const string& logMessage, TLogLevel level, bool force)
{
	if (m_screenLogLevel >= level)
		outScreen(level, logMessage);

	if (m_fileLogLevel >= level)
		outFile(level, logMessage);
}

void oLog::SQLoutLog(const string& logMessage, TLogLevel level, bool force)
{
	std::unique_lock<std::mutex> locker(m_LogMutext);
	m_FileLogList.push_back(logMessage);
}

oLog::oLog()
{
	//	open();
}

void oLog::close()
{
	m_Terminated = true;
	if (m_LoggerThread) {
		m_LoggerThread->join();
		m_LoggerThread.reset();
		fclose(m_file);
		fclose(m_Sqlfile);
		m_file = nullptr;
		m_Sqlfile = nullptr;
	}
}

void oLog::open()
{
	if (!m_LoggerThread) {
		openLogFile(m_file, "Log");
		openLogFile(m_Sqlfile, "SQL");
		m_LoggerThread = make_shared<std::thread>(bind(&oLog::logLooping, this));
	}
}

oLog::~oLog()
{
	close();
}

void oLog::Init(int32 fileLogLevel, int32 screenLogLevel)
{
	m_NeedLog = fileLogLevel > 0 || screenLogLevel > 0;
	m_screenLogLevel = screenLogLevel;
	m_fileLogLevel = fileLogLevel;

	// get error handle
#ifdef WIN32
	stderr_handle = GetStdHandle(STD_ERROR_HANDLE);
	stdout_handle = GetStdHandle(STD_OUTPUT_HANDLE);
#endif
}

void oLog::outString(const char* source, const char* str, ...)
{
	va_list ap;
	va_start(ap, str);
	string logMessage = logString(TLogLevel::loglevel_always, source, str, ap);
	va_end(ap);
	outLog(logMessage, TLogLevel::loglevel_always);
}


void oLog::outError(const char* source, const char* str, ...)
{
	try
	{
		if (m_NeedLog) {
			va_list ap;
			va_start(ap, str);
			string logMessage = logString(TLogLevel::loglevel_error, source, str, ap);
			va_end(ap);
			outLog(logMessage, TLogLevel::loglevel_error);
		}
	}
	catch (...)
	{

	}
}

void oLog::outToFile(const char* source, const char* str, ...)
{
	va_list ap;
	va_start(ap, str);
	string logMessage = logString(TLogLevel::loglevel_error, source, str, ap);
	va_end(ap);
	outLog(logMessage, TLogLevel::loglevel_error);
	if (m_file)
	{
		vector<string> aLogList;
		aLogList.push_back(logMessage);
		this->flushLog(this->m_file, aLogList, "Log");
	}
}




void oLog::outFatal(const char* source, const char* str, ...)
{
	if (m_NeedLog) {
		va_list ap;
		va_start(ap, str);
		string logMessage = logString(TLogLevel::loglevel_fatal, source, str, ap);
		va_end(ap);
		outLog(logMessage, TLogLevel::loglevel_fatal);
	}
}

void oLog::outWarning(const char* source, const char* str, ...)
{
	va_list ap;
	va_start(ap, str);
	string logMessage = logString(TLogLevel::loglevel_warning, source, str, ap);
	va_end(ap);
	outLog(logMessage, TLogLevel::loglevel_warning);
}



void oLog::outInfo(const char* source, const char* str, ...)
{
	if (m_NeedLog) {
		va_list ap;
		va_start(ap, str);
		string logMessage = logString(TLogLevel::loglevel_info, source, str, ap);
		va_end(ap);
		outLog(logMessage, TLogLevel::loglevel_info);
	}
}

void oLog::outDebug(const char* source, const char* str, ...)
{
	try
	{
		if (m_NeedLog) {
			va_list ap;
			va_start(ap, str);
			string logMessage = logString(TLogLevel::loglevel_debug, source, str, ap);
			va_end(ap);
			outLog(logMessage, TLogLevel::loglevel_debug);
		}
	}
	catch (...)
	{

	}
}
void oLog::SQLOutDebug(const char* source, const char* str, ...)
{
	va_list ap;
	va_start(ap, str);
	string logMessage = logString(TLogLevel::loglevel_debug, source, str, ap);
	va_end(ap);
	SQLoutLog(logMessage, TLogLevel::loglevel_debug, false);
}

void oLog::logScreen(LogColors color, const char* source, const char* str, ...)
{
	va_list ap;
	va_start(ap, str);
	string logMessage = logString(TLogLevel::loglevel_debug, source, str, ap);
	va_end(ap);
	SetConsoleTextAttribute(stdout_handle, color);
	fprintf(stderr, "%s", logMessage.c_str());
	SetConsoleTextAttribute(stdout_handle, log_color_white);
}

void oLog::logScreenAndFile(LogColors color, const char* source, const char* str, ...)
{
	va_list ap;
	va_start(ap, str);
	string logMessage = logString(TLogLevel::loglevel_debug, source, str, ap);
	va_end(ap);
	SetConsoleTextAttribute(stdout_handle, color);
	fprintf(stderr, "%s", logMessage.c_str());
	SetConsoleTextAttribute(stdout_handle, log_color_white);
	outFile(TLogLevel::loglevel_info, logMessage);
}

