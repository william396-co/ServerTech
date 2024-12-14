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
#pragma once

#include "Singleton.h"
#include "CommonType.h"
#include <thread>
#include <mutex>
#include <string>
#include <vector>
#include <functional>
#include <memory>

#ifdef _WIN32
#include <Windows.h>
#endif // _WIN32


using std::string;
using std::vector;
using std::thread;
using std::mutex;
using std::shared_ptr;

#define LogBufferSize 1024 * 1024
//#define LogFileMaxSize 1024 * 1024 * 128;
#define LogFileMaxSize 1024 * 1024 * 128;


extern time_t UNIXTIME;		/* update this every loop to avoid the time() syscall! */
//extern SERVER_DECL tm g_localTime;

#ifdef WIN32
#define TRED FOREGROUND_RED | FOREGROUND_INTENSITY
#define TGREEN FOREGROUND_GREEN | FOREGROUND_INTENSITY
#define TYELLOW FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY
#define TNORMAL FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE
#define TWHITE TNORMAL | FOREGROUND_INTENSITY
#define TBLUE FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY
#else
#define TRED 1
#define TGREEN 2
#define TYELLOW 3
#define TNORMAL 4
#define TWHITE 5
#define TBLUE 6
#endif

enum TLogLevel {
	loglevel_always,
	loglevel_fatal,
	loglevel_error,
	loglevel_warning,
	loglevel_debug,
	loglevel_info,
	loglevel_max
};


enum LogColors {
	log_color_white = FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY,
	log_color_red = FOREGROUND_RED,
	log_color_green = FOREGROUND_GREEN,
	log_color_blue = FOREGROUND_BLUE,
	log_color_yellow = FOREGROUND_RED | FOREGROUND_GREEN,
	log_color_purple = FOREGROUND_RED | FOREGROUND_BLUE,
	log_color_cyan = FOREGROUND_GREEN | FOREGROUND_BLUE,
	log_color_white_intensity = FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY,
	log_color_red_intensity = FOREGROUND_RED | FOREGROUND_INTENSITY,
	log_color_green_intensity = FOREGROUND_GREEN | FOREGROUND_INTENSITY,
	log_color_blue_intensity = FOREGROUND_BLUE | FOREGROUND_INTENSITY,
	log_color_yellow_intensity = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY,
	log_color_purple_intensity = FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY,
	log_color_cyan_intensity = FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY
};

class oLog : public Singleton< oLog > {
private:
#ifdef WIN32
	HANDLE stdout_handle, stderr_handle;
#endif
	int32 m_logFileSizeLimited = LogFileMaxSize;
	int32 m_fileLogLevel = 0;
	int32 m_screenLogLevel = 0;
	uint32 m_MaxLogCount = 100 * 10000;
	FILE* m_file = nullptr;
	FILE* m_Sqlfile = nullptr;
	string m_logFileName;
	bool m_NeedLog = false;
	bool m_Terminated = false;
	shared_ptr<std::thread> m_LoggerThread;
	std::mutex m_LogMutext;
	vector<string> m_LogList;
	vector<string> m_FileLogList;
	char buffer[LogBufferSize];
	void logLooping();
	void flushLog(FILE*& afile, vector<string>& aLogList, string name = "");
	void renewLogFile(FILE*& afile, string name = "");
	//void createLogFile(const string &filename);
	string logString(TLogLevel level, const char* source, const char* str, va_list args);
	void writeToFile(FILE*& afile, const char* log, int size, string name = "");
	void outScreen(TLogLevel level, const string& text);
	void outFile(TLogLevel level, const string& text);
	void outLog(const string& logMessage, TLogLevel level, bool force = false);
	void openLogFile(FILE*& afile, string name = "");
	void SQLoutLog(const string& logMessage, TLogLevel level, bool force);
public:
	oLog();
	void close();
	void open();
	virtual ~oLog();
	void outToFile(const char* source, const char* str, ...);
	void outString(const char* source, const char* str, ...);
	void outError(const char* source, const char* str, ...);
	void outFatal(const char* source, const char* str, ...);
	void outWarning(const char* source, const char* str, ...);
	void outInfo(const char* source, const char* str, ...);
	void outDebug(const char* source, const char* str, ...);
	void SQLOutDebug(const char* source, const char* str, ...);
	void logScreen(LogColors color, const char* source, const char* str, ...);
	void logScreenAndFile(LogColors color, const char* source, const char* str, ...);
	void Init(int32 fileLogLevel, int32 screenLogLevel);
	void setupLogFileMaxSize(int32 size) {
		if (size > 0)
			m_logFileSizeLimited = size;
		else
			m_logFileSizeLimited = LogFileMaxSize;
	}
};


