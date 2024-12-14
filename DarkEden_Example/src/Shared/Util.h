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

#ifndef _UTIL_H
#define _UTIL_H

#include "Common.h"
#include <stdio.h>
#include "SysInfo.hpp"
#pragma execution_character_set("utf-8")

extern "C" void __cdecl __acrt_report_runtime_error(wchar_t const* const message);

///////////////////////////////////////////////////////////////////////////////
// String Functions ///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
std::vector<std::string> StrSplit(const std::string &src, const std::string &sep);
std::vector<std::string> StrSplitEx(const std::string& src, const std::string& sep);//可以包含空格在内
std::vector<uint32> StrSplitToInt(const std::string &src, const std::string &sep);
std::string replace_all(const std::string& str, const std::string& old_value, const std::string& new_value);

bool CheckSeverID(uint32 SID, const std::vector<uint32> aServerIds);



// This HAS to be called outside the threads __try / __except block!
time_t convTimePeriod ( uint32 dLength, char dType);

inline uint32 secsToTimeBitFields(time_t secs)
{
    tm* lt = localtime(&secs);
    return (lt->tm_year - 100) << 24 | lt->tm_mon  << 20 | (lt->tm_mday - 1) << 14 | lt->tm_wday << 11 | lt->tm_hour << 6 | lt->tm_min;
}


extern SERVER_DECL void split(const std::string& src, const std::string& delim, std::vector<std::string>& dest);
extern SERVER_DECL void split(const std::string& src, const std::string& delim, std::vector<uint32>& dest);
// 此版本会忽略分隔符之间空白的内容 一般并列的内容的时候可以忽略 比如uin列表 都是 uin
extern SERVER_DECL void split_ign(const std::string & src, const std::string & delim, std::vector<std::string>& dest);
extern SERVER_DECL void split_ign(const std::string & src, const std::string & delim, std::vector<uint32>& dest);

extern SERVER_DECL const char* _StringToUTF8(const char*pASCIIBuf);
extern SERVER_DECL const char* _StringToANSI(const char*pUtf8Buf);
extern SERVER_DECL bool _IsStringUTF8(const char *str);
void GetDateFromTimestamp(uint64 timestamp, uint16 &aYear, uint8 &amonth, uint8 &date);
extern SERVER_DECL void GetNowMothData(uint16 &aYear, uint8 &amonth, uint8 &date);
extern SERVER_DECL void GetNowDate365(uint16 &date);
void GetNowDate365Real(uint16 &date);
extern SERVER_DECL void GetNowWeek365_old(uint16 & date);
extern SERVER_DECL void GetNowWeek365(uint16 &date);
void GetNowWeek365_old(uint16 & date, uint16 weekday);
void GetNowWeek365(uint16 &date, uint16 weekday);
uint16 GetNowWeek365From(uint16 weekday);
extern SERVER_DECL uint16 GetNowDate365();
extern SERVER_DECL uint16 GetNowDate365(time_t t);
extern SERVER_DECL uint16 GetNowWeek365();

extern SERVER_DECL void GetNowYear(uint16 &date);
extern SERVER_DECL time_t GetToday0Time(); // 得到今天0点的时间;
extern SERVER_DECL time_t GetToday5Time(uint32 t); // 得到传入时间最后5点的时间;
extern SERVER_DECL uint32 GetServerIdFromUid(uint32 Uid);

extern SERVER_DECL bool IsLeapYear(uint16 year);
extern SERVER_DECL uint16 GetTotalDay(uint16 year);

extern SERVER_DECL void GetCurrentHour(uint32 &hour, uint32 &min, uint32 &sec);
extern SERVER_DECL void GetCurrentHour(uint32&wday, uint32 &hour, uint32 &min, uint32 &sec);
extern SERVER_DECL uint32 GetTimByStr(const std::string atimestr, uint32 &startH, uint32 &startS, uint32 &endH, uint32 &endS);
extern SERVER_DECL uint32 GetCurrentSecond();
extern SERVER_DECL uint32 GetYearMonDay();
extern SERVER_DECL uint32 GetYearMonDay(time_t t);
extern SERVER_DECL std::string GetFormatDateStr(time_t t);
extern SERVER_DECL std::string GetFormatDateStrEx(time_t t);
extern SERVER_DECL std::string GetFormatDayStr(time_t t);
extern SERVER_DECL time_t DiffDayByTime(time_t t, int32 day);
extern SERVER_DECL std::string WeekDayToStr(uint32 weekday);
extern SERVER_DECL bool SameDay(time_t t1, time_t t2);
extern SERVER_DECL uint32 GetYesterday(uint32 today = 0);


extern std::string TimeConvertString(time_t t);
extern time_t StringConvertTime(const std::string& timeStamp);
extern time_t StringConvertTimeEx(const std::string& timeStamp);
extern std::string string_toLower(const std::string& src);
extern std::string string_toUpper(const std::string& src);
extern void RaiseException(const char* fmt, ...);
extern std::string FormatString(const char* fmt, ...);
extern std::string int32tostr(int32 val);
extern std::string uint32tostr(uint32 val);
extern std::string int64tostr(int64 val);
extern std::string uint64tostr(uint64 val);
extern int32 strtoint32(const std::string& val);
extern uint64 strtouint64(const std::string& val);
extern bool isNum(const std::string& val);
extern uint32 strtouint32(const std::string& val);

/* 默认拷贝ssize大小，并且不超过dsize大小 */
extern errno_t safe_memcpy(void* dest, size_t dsize, const void* src, size_t ssize, size_t cnt = RSIZE_MAX);

extern bool IsLegal(int year, int mon, int day, int hour, int minu, int sec);
extern time_t uint64ToTime(uint64 t);
extern uint64 TimeToUint64(time_t time);
extern bool validateTime(uint64 t);

extern uint8 ToHex(uint8 x);
extern uint8 FromHex(uint8 x);
std::string UrlEncode(const std::string& str);
std::string UrlDecode(const std::string& str);

extern void UTF_8ToUnicode(wchar_t* pOut, char *pText);
extern void UnicodeToUTF_8(char* pOut, wchar_t* pText);
extern bool ForceDirectory(const std::string &path);
extern std::string ExtractFilePath(const std::string path);

extern std::string ExpandFilename(const std::string& filename);
extern bool GetRandList(uint32 min, uint32 max, uint32 count, std::vector<uint32>& tar);
extern int32 GetFileSize(FILE* fp);
// 获取目录下的所有文件名;
extern void GetDirectoryFiles(const std::string& path, std::vector<std::string>& files, const std::string slash = "/");
extern time_t GetTimeStamp(uint32 week, uint32 hour, uint32 min, uint32 sec);
extern time_t GetTimeStamp(time_t t, uint32 week, uint32 hour, uint32 min, uint32 sec);
// 获取指定时间戳
extern time_t GetAppointTimeStamp(time_t t, uint32 hour, uint32 min, uint32 sec);

extern uint32 RandBetween1(uint32 st, uint32 end); //[st, end);
extern uint32 RandBetween2(uint32 st, uint32 end); //(st, end];

extern std::string GetCurrExeName();
extern std::string hexdump(const unsigned char *buf, const int num);
extern std::wstring Utf8ToUnicode(const char* utf8string);
extern size_t UTF8_GetCharCount(const char* utf8string);
extern std::string ConvertUtf8ToGBK(const std::string& strUtf8);
extern size_t UTF8_GetCharSize(const char* utf8string);
extern bool isSameWeek(time_t t1, time_t t2);
extern bool isNextWeek(time_t now, time_t nextweek);
extern void ShowErrorMsgBox(const std::string& title,const std::string& content);
extern uint32 Uint32Division(uint32 Divisor, uint32 beDiv); // uint32 除法
extern float FloatDivison(float Divisor, float beDiv); // float 除法

#define FloatEQU(a, b) (fabs((a)-(b)) < FLT_EPSILON)
class IDCreator
{
public:
	IDCreator() {};
	~IDCreator() {};
	uint64 GenerateNextID(uint32 serverId = 0)
	{
		char buffer[512];
		++Index;
		if (Index > 999)  Index = 1;
		time_t t = time(NULL);
		tm* local = localtime(&t);
		
		snprintf(buffer, sizeof(buffer), "%2u%2u%2u%2u%2u%2u%.3u%.4u", 
			local->tm_year % 100,
			local->tm_mon,
			local->tm_mday,
			local->tm_hour,
			local->tm_min,
			local->tm_sec,
			Index,
			serverId%10000);
  
		uint64 id = _strtoui64(buffer, 0, 0);
		return id;
	}
private:
	uint32 Index = 0;
};

class IDCreator_1
{
public:
	IDCreator_1() {};
	~IDCreator_1() {};
	uint64 GenerateNextID()
	{
		char buffer[512];
		++Index;
		if (Index > 9999)  Index = 1;
		time_t t = time(NULL);
		tm* local = localtime(&t);

		snprintf(buffer, sizeof(buffer), "%.2u%.2u%.2u%.2u%.2u%.2u%.4u",
			(local->tm_year + 1900) % 100,
			local->tm_mon+1,
			local->tm_mday,
			local->tm_hour,
			local->tm_min,
			local->tm_sec,
			Index);

		uint64 id = _strtoui64(buffer, 0, 0);
		return id;
	}
private:
	uint32 Index = 0;
};

std::string CompileTime();

class VersionInfo
{
public:
	VersionInfo() {};
	~VersionInfo() {};
	static std::string GeteVersionInfo(uint32 serverId = 0)
	{
		std::string compileTime = CompileTime();
		time_t t = time(NULL);
		tm* local = localtime(&t);
		char buffer[1024];
		const char* version= "9.04 v1.0正式版本";
		snprintf(buffer, sizeof(buffer), "version:%s compiletime:(%s) serverid:%u",
			version,
			compileTime.c_str(),
			serverId );
		return buffer;
	}
private:

};

extern bool CheckProcessSingle(std::string mutexName);

// 在一个容器里随机选择一个成员, 要求容器是可以随机访问的容器, 例如vector
template <class TContainer>
bool RandomGet(const TContainer &container, typename TContainer::value_type &out_value)
{
	if (container.empty())
		return false;

	size_t count = container.size();
	size_t rand_index = rand() % count;
	out_value = container[rand_index];
	return true;
}
#endif