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

#include "Common.h"
#include "Log.h"
#include <stdio.h>

using namespace std;

///分割字符串
void split(const string& src, const string& delim, vector<string>& dest)
{
	int pos1 = 0, pos2 = 0;
	while (true)
	{
		pos2 = src.find_first_of(delim, pos1);
		if (pos2 == -1)
		{
			dest.push_back(src.substr(pos1));
			break;
		}
		else
		{
			dest.push_back(src.substr(pos1, pos2 - pos1));
		}
		pos1 = pos2 + 1;
	}
}

///分割字符串
void split(const string& src, const string& delim, vector<uint32>& dest)
{
	int pos1 = 0, pos2 = 0;
	int len = src.length();

	while (true && pos1<len)
	{
		pos2 = src.find_first_of(delim, pos1);
		if (pos2 == -1)
		{
			dest.push_back(strtoul(src.substr(pos1).c_str(), NULL, 10));
			break;
		}
		else if (pos2>pos1)
		{
			dest.push_back(strtoul(src.substr(pos1, pos2 - pos1).c_str(), NULL, 10));
		}
		pos1 = pos2 + 1;
	}
}

///分割字符串
void split_ign(const string& src, const string& delim, vector<string>& dest)
{
	int pos1 = 0, pos2 = 0;
	while (true)
	{
		pos1 = src.find_first_not_of(delim, pos2);
		if (pos1 == -1) break;

		pos2 = src.find_first_of(delim, pos1);
		if (pos2 == -1)
		{
			dest.push_back(src.substr(pos1));
			break;
		}
		else
		{
			dest.push_back(src.substr(pos1, pos2 - pos1));
		}
	}
}


///分割字符串
void split_ign(const string & src, const string & delim, vector<uint32>& dest)
{
	int pos1 = 0, pos2 = 0;
	while (true)
	{
		pos1 = src.find_first_not_of(delim, pos2);
		if (pos1 == -1) break;

		pos2 = src.find_first_of(delim, pos1);
		if (pos2 == -1)
		{
			dest.push_back(strtoul(src.substr(pos1).c_str(), NULL, 10));
			break;
		}
		else
		{
			dest.push_back(strtoul(src.substr(pos1, pos2 - pos1).c_str(), NULL, 10));
		}
	}
}


///////////////////////////

bool CheckSeverID(uint32 SID, const std::vector<uint32> aServerIds)
{
	bool isFind = false;

	for (uint32 i = 0; i<aServerIds.size(); i++)
	{
		//服务器编号里有0 或者pc模式都让登录
		if (aServerIds[i] == 0 || SID==0)
			isFind = true;
		if (aServerIds[i] == SID)
			isFind = true;
	}
	return isFind;
}

vector<string> StrSplit(const string &src, const string &sep)
{
	vector<string> r;
	string s;
	for (string::const_iterator i = src.begin(); i != src.end(); i++) {
		if (sep.find(*i) != string::npos) {
			if (s.length()) r.push_back(s);
			s = "";
		} else {
			s += *i;
		}
	}
	if (s.length()) r.push_back(s);
	return r;
}

vector<string> StrSplitEx(const std::string& src, const std::string& sep)
{
	vector<string> r;
	string s;
	for (string::const_iterator i = src.begin(); i != src.end(); i++) {
		if (sep.find(*i) != string::npos) {
			if (s.length()) r.push_back(s);
			else if (s.length() == 0)
				r.push_back("");
			s = "";
		}
		else {
			s += *i;
		}
	}
	if (s.length()) r.push_back(s);
	return r;
}

vector<uint32> StrSplitToInt(const string &src, const string &sep)
{
	vector<uint32> r;
	string s;
	for (string::const_iterator i = src.begin(); i != src.end(); i++) 
	{
		if (sep.find(*i) != string::npos) 
		{
			if (s.length())
				r.push_back(strtoul(s.c_str(), NULL, 10));
			s = "";
		}
		else 
		{
			s += *i;
		}
	}
	if (s.length()) 
		r.push_back(strtoul(s.c_str(), NULL, 10));
	return r;
}


std::string replace_all(const std::string& str, const std::string& old_value, const std::string& new_value)
{
	std::string src = str;
	//会出现死循环
	std::string::size_type  apos = new_value.find(old_value);
	if (apos != std::string::npos)
		return src;
	while (true) {
		std::string::size_type  pos = src.find(old_value);
		if (pos != std::string::npos)
			src.replace(pos, old_value.length(), new_value);
		else   break;
	}
	return src;
}

errno_t safe_memcpy(void* dest, size_t dsize,const void* src, size_t ssize, size_t cnt)
{
	// What was the point of this call?
	if (cnt == 0)
	{
		sLog.outInfo("safe_memcpy ", "cnt is zero");
		return 0;
	}

	if (dest == NULL || src == NULL)
	{
		sLog.outInfo("safe_memcpy ", "dest or src is NULL");
		return -1;
	}

	if (dsize == 0 || ssize == 0)
	{
#ifdef _WIN64
		sLog.outInfo("safe_memcpy ", "dsize(%llu) or ssize(%llu) is Zero", dsize, ssize);
#else
		sLog.outInfo("safe_memcpy ", "dsize(%lu) or ssize(%lu) is Zero", dsize, ssize);
#endif // _WIN64
		return -1;
	}
	
	if (dsize > RSIZE_MAX || ssize > RSIZE_MAX || cnt > RSIZE_MAX)
	{
#ifdef _WIN64
		sLog.outInfo("safe_memcpy ", "dsize or ssize or cnt is over %llu", RSIZE_MAX);
#else
		sLog.outInfo("safe_memcpy ", "dsize or ssize or cnt is over %lu", RSIZE_MAX);
#endif // #ifdef _WIN64
		return -1;
	}

	size_t cc = min(min(dsize, ssize), cnt);
	//memmove(dest, src, cc);
	memcpy(dest, src, cc);

	if (cc != cnt && cnt != RSIZE_MAX)
	{
#ifdef _WIN64
		sLog.outInfo("safe_memcpy ", "cc:%llu, cnt:%llu", cc, cnt);
#else
		sLog.outInfo("safe_memcpy ", "cc:%lu, cnt:%lu", cc, cnt);
#endif
		return 1;
	}

	return 0;
}


time_t convTimePeriod ( uint32 dLength, char dType )
{
	time_t rawtime = 0;
	if (dLength == 0)
		return rawtime;
	struct tm * ti = localtime( &rawtime );
	switch(dType)
	{
		case 'h':		// hours
			ti->tm_hour += dLength;
			break;
		case 'd':		// days
			ti->tm_mday += dLength;
			break;
		case 'w':		// weeks
			ti->tm_mday += 7 * dLength;
			break;
		case 'm':		// months
			ti->tm_mon += dLength;
			break;
		case 'y':		// years
			// are leap years considered ? do we care ?
			ti->tm_year += dLength;
			break;
		default:		// minutes
			ti->tm_min += dLength;
			break;
	}
	return mktime(ti);
}
int32 GetTimePeriodFromString(const char * str)
{
	uint32 time_to_ban = 0;
	char * p = (char*)str;
	uint32 multiplier;
	string number_temp;
	uint32 multipliee;
	number_temp.reserve(10);
	while(*p != 0)
	{
		// always starts with a number.
		if(!isdigit(*p))
			break;
		number_temp.clear();
		while(isdigit(*p) && *p != 0)
		{
			number_temp += *p;
			++p;
		}
		// try to find a letter
		if(*p != 0)
		{
		// check the type
			switch(tolower(*p))
			{
			case 'y':
				multiplier = TIME_YEAR;		// eek!
				break;
			case 'm':
				multiplier = TIME_MONTH;
				break;
			case 'd':
				multiplier = TIME_DAY;
				break;
			case 'h':
				multiplier = TIME_HOUR;
				break;
			default:
				return -1;
				break;
			}
			++p;
		}
		else
			multiplier = TIME_MINUTE; // Defaults to MINUTES, if no letter is given
		multipliee = strtouint32(number_temp.c_str());
		time_to_ban += (multiplier * multipliee);
	}
	return time_to_ban;
}

const char * szDayNames[7] = {
	"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"
};

const char * szMonthNames[12] = {
	"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"
};

void MakeIntString(char * buf, int num)
{
	if(num<10)
	{
		buf[0] = '0';
		//itoa(num, &buf[1], 10);
		sprintf(&buf[1], "%u", num);
	}
	else
	{
		//itoa(num,buf,10);
		sprintf(buf,"%u",num);
	}
}

void MakeIntStringNoZero(char * buf, int num)
{
	//itoa(num,buf,10);
	sprintf(buf,"%u",num);
}

string ConvertTimeStampToString(uint32 timestamp)
{
	int seconds = (int)timestamp;
	int mins= 0;
	int hours= 0;
	int days= 0;
	int months= 0;
	int years= 0;
	if(seconds >= 60)
	{
		mins = seconds / 60;
		if(mins)
		{
			seconds -= mins*60;
			if(mins >= 60)
			{
				hours = mins / 60;
				if(hours)
				{
					mins -= hours*60;
					if(hours >= 24)
					{
						days = hours/24;
						if(days)
						{
							hours -= days*24;
							if(days >= 30)
							{
								months = days / 30;
								if(months)
								{
									days -= months*30;
									if(months >= 12)
									{
										years = months / 12;
										if(years)
										{
											months -= years*12;
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}

	char szTempBuf[100] = {};
	string szResult;

	if(years) {
		MakeIntStringNoZero(szTempBuf, years);
		szResult += szTempBuf;
		szResult += " years, ";
	}

	if(months) {
		MakeIntStringNoZero(szTempBuf, months);
		szResult += szTempBuf;
		szResult += " months, ";
	}

	if(days) {
		MakeIntStringNoZero(szTempBuf, days);
		szResult += szTempBuf;
		szResult += " days, ";
	}

	if(hours) {
		MakeIntStringNoZero(szTempBuf, hours);
		szResult += szTempBuf;
		szResult += " hours, ";
	}

	if(mins) {
		MakeIntStringNoZero(szTempBuf, mins);
		szResult += szTempBuf;
		szResult += " minutes, ";
	}

	if(seconds) {
		MakeIntStringNoZero(szTempBuf, seconds);
		szResult += szTempBuf;
		szResult += " seconds";
	}

	return szResult;
}

string ConvertTimeStampToDataTime(uint32 timestamp)
{
	char szTempBuf[100] = {};
	time_t t = (time_t)timestamp;
	tm * pTM = localtime(&t);

	string szResult;
	szResult += szDayNames[pTM->tm_wday];
	szResult += ", ";

	MakeIntString(szTempBuf, pTM->tm_mday);
	szResult += szTempBuf;
	szResult += " ";

	szResult += szMonthNames[pTM->tm_mon];
	szResult += " ";

	MakeIntString(szTempBuf, pTM->tm_year+1900);
	szResult += szTempBuf;
	szResult += ", ";
	MakeIntString(szTempBuf, pTM->tm_hour);
	szResult += szTempBuf;
	szResult += ":";
	MakeIntString(szTempBuf, pTM->tm_min);
	szResult += szTempBuf;
	szResult += ":";
	MakeIntString(szTempBuf, pTM->tm_sec);
	szResult += szTempBuf;

	return szResult;
}

uint32 DecimalToMask(uint32 dec)
{
	return ( (uint32)1 << (dec - 1) );
}

void GetDateFromTimestamp(uint64 timestamp, uint16 &aYear, uint8 &amonth, uint8 &date)
{
	time_t now = timestamp;
	struct tm * timeinfo = localtime(&now);
	aYear = timeinfo->tm_year;
	amonth = timeinfo->tm_mon + 1;
	date = timeinfo->tm_mday;
}

void GetNowMothData(uint16 &aYear, uint8 &amonth, uint8 &date)
{	
  time_t now = time(NULL);
  struct tm * timeinfo= localtime(&now);
  aYear = timeinfo->tm_year + 1900;
  amonth = timeinfo->tm_mon+1;
  date = timeinfo->tm_mday;
}

void GetNowDate365(uint16 &date)
{
	time_t now = time(NULL) - (5 * 60 * 60);
	struct tm * timeinfo = localtime(&now);
	date = timeinfo->tm_yday+1;
}

void GetNowDate365Real(uint16 &date)
{
	time_t now = time(NULL);
	struct tm * timeinfo = localtime(&now);
	date = timeinfo->tm_yday;
}

void GetNowWeek365_old(uint16 &date)
{
	uint16 datea;
	GetNowDate365(datea);	
	//datea = ((int16)datea +1) < 0 ? 0 : datea ;
	date = (datea) / 7 + 1;
}

void GetNowWeek365(uint16 &date)
{
	int nweek = 0;
	int wd = 0;
	int yd = 0;

	time_t now = time(NULL) - (5 * 60 * 60);
	struct tm * timeinfo = localtime(&now);
	wd = timeinfo->tm_wday;
	yd = timeinfo->tm_yday;

	yd = yd + 1;
	//计算本年1月1日是周几
	int dan = 0;
	int  a1 = wd - yd % 7;
	//计算一年的1号是礼拜几开始
	if (a1 >= 0) dan = a1;
	if (a1 < 0) dan = 7 + a1;

	int  lastweek = (yd  + dan) / 7+1;
	date = lastweek;
}

//从第weekday开始，划分第几周,weekday范围[0,6],比如周三分界，weekday=3
void GetNowWeek365_old(uint16 &date,uint16 weekday)
{
	uint16 datea;
	GetNowDate365(datea);
	//sLog.outError("GetNowWeek365:", "---------------------- %u", datea);
	if (datea>=weekday)
	{
		date = (datea - weekday) / 7 + 1;//在第一年的第一周过后，则此时间是从周weekday开始分下一周
	}
	else
	{
		date = datea / 7 + 1;//在第一年的第一周，datea可能小于weekday
	}
}

//从第weekday开始，划分第几周,weekday范围[0,6],比如周三分界，weekday=3
void GetNowWeek365(uint16 &date, uint16 weekday)
{
	int nweek = 0;
	int wd = 0;
	int yd = 0;

	time_t now = time(NULL) - (5 * 60 * 60);
	struct tm * timeinfo = localtime(&now);
	wd = timeinfo->tm_wday;
	yd = timeinfo->tm_yday;

	//计算本年1月1日是周几
	int dan = 0;
	int  a1 = wd - yd % 7;
	//计算一年的1号是礼拜几开始
	if (a1 >= 0) dan = a1;
	if (a1 < 0) dan = 7 + a1;

	int  lastweek = (yd - weekday + dan) / 7 + 1;
	date = lastweek;
}

uint16 GetNowDate365()
{
	uint16 datea;
	GetNowDate365(datea);
	return datea;
}

uint16 GetNowDate365(time_t t)
{
	time_t now = t - (5 * 60 * 60);
	struct tm * timeinfo = localtime(&now);
	return  timeinfo->tm_yday + 1;
}

uint16 GetNowWeek365()
{
	uint16 week;
	GetNowWeek365(week);
	return week;
}

//从周几开始算作第一周,weekday=1,代表从周一开始分周的界限
uint16 GetNowWeek365From(uint16 weekday)
{
	uint16 week;
	GetNowWeek365(week,weekday);
	return week;
}

void GetNowYear(uint16 &date)
{
	time_t now = time(NULL);
	struct tm * timeinfo = localtime(&now);
	date = timeinfo->tm_year;
}

time_t GetToday0Time()
{
	time_t now = time(NULL);
	struct tm * timeinfo = localtime(&now);
	return now - timeinfo->tm_hour * 3600 - timeinfo->tm_min * 60 - timeinfo->tm_sec;
}

time_t GetToday5Time(uint32 t)
{
	time_t now = t;
	struct tm * timeinfo = localtime(&now);
	if (timeinfo->tm_hour >= 0 && timeinfo->tm_hour <= 5)
	{
		return now - timeinfo->tm_hour * 3600 - timeinfo->tm_min * 60 - timeinfo->tm_sec + 5 * 3600;
	}
	return now - timeinfo->tm_hour * 3600 - timeinfo->tm_min * 60 - timeinfo->tm_sec + 29 * 3600;
}

bool IsLeapYear(uint16 year)
{
	/* 4 years is  leap year, 100 years is not leap year, 400 years leap again */
	if ((year % 4 == 0 && year % 100 != 0) || year % 400 == 0)
		return true;
	return false;
}

uint16 GetTotalDay(uint16 year)
{
	if (IsLeapYear(year))
		return 366;
	return 365;
}

uint32 GetServerIdFromUid(uint32 Uid)
{
	return Uid % 10000;
}

uint32 GetCurrentSecond()
{
	time_t t = time(NULL);
	tm* currentTime = localtime(&t);	
	return ((currentTime->tm_hour * 60 * 60) + (currentTime->tm_min * 60) + currentTime->tm_sec);
}

uint32 GetYearMonDay()
{
	time_t t = time(NULL);
	tm* currentTime = localtime(&t);
	return ((currentTime->tm_year * 10000) + (currentTime->tm_mon * 100) + currentTime->tm_mday);
}

uint32 GetYearMonDay(time_t t)
{
	tm* time = localtime(&t);
	return ((time->tm_year * 10000) + (time->tm_mon * 100) + time->tm_mday);
}

std::string TimeConvertString(time_t t)
{
	char szBuffer[256] = {};
	tm _tm = {};
	localtime_s(&_tm, &t);
	strftime(szBuffer, 256, "%Y-%m-%d %H:%M:%S", &_tm);
	return szBuffer;
}

time_t StringConvertTime(const std::string& timeStamp)
{
	struct tm tm;
	memset(&tm, 0, sizeof(tm));
	sscanf(timeStamp.c_str(), "%d/%d/%d %d:%d:%d",
		&tm.tm_mon, &tm.tm_mday, &tm.tm_year,
		&tm.tm_hour, &tm.tm_min, &tm.tm_sec);

	tm.tm_year -= 1900;
	tm.tm_mon--;
	return mktime(&tm);
}

time_t StringConvertTimeEx(const std::string& timeStamp)
{
	struct tm tm;
	memset(&tm, 0, sizeof(tm));
	sscanf(timeStamp.c_str(), "%d-%d-%d %d:%d:%d",
		&tm.tm_year, &tm.tm_mon, &tm.tm_mday,
		&tm.tm_hour, &tm.tm_min, &tm.tm_sec);

	tm.tm_year -= 1900;
	tm.tm_mon--;
	return mktime(&tm);
}

std::string GetFormatDateStr(time_t t)
{
	tm *tm_ = localtime(&t);
	char dateStr[256] = { };
	sprintf_s(dateStr, "%04d-%02d-%02d %02d:%02d:%02d", (tm_->tm_year + 1900), (tm_->tm_mon + 1), (tm_->tm_mday), (tm_->tm_hour), (tm_->tm_min), (tm_->tm_sec));
	return std::string(dateStr);
}


std::string GetFormatDayStr(time_t t)
{
	tm* tm_ = localtime(&t);
	char dateStr[256];
	sprintf_s(dateStr, "%04d-%02d-%02d", (tm_->tm_year + 1900), (tm_->tm_mon + 1), (tm_->tm_mday));
	return std::string(dateStr);
}

time_t DiffDayByTime(time_t t, int32 day)
{
	tm* tm_ = localtime(&t);
	tm_->tm_mday += day;
	return mktime(tm_);
}

bool SameDay(time_t t1, time_t t2)
{
	tm tm1 = *localtime(&t1);
	tm tm2 = *localtime(&t2);
	return tm1.tm_mday == tm2.tm_mday && tm1.tm_year == tm2.tm_year && tm1.tm_mon == tm2.tm_mon;
}

uint32 GetYesterday(uint32 today /* = 0 */)
{
	if (today == 0)
		today = GetNowDate365();
	uint32 yesterday = today - 1;
	if (today == 1) //新年第一天 需要处理跨年问题
	{
		uint16 year = 0;
		uint8 month, day;
		GetNowMothData(year, month, day);
		if (IsLeapYear(year - 1))
			yesterday = 366;
		else
			yesterday = 365;
	}
	return yesterday;
}

std::string GetFormatDateStrEx(time_t t)
{
	tm *tm_ = localtime(&t);
	char dateStr[256] = { };
	sprintf_s(dateStr, "%04d-%02d-%02d %02d:%02d", (tm_->tm_year + 1900), (tm_->tm_mon + 1), (tm_->tm_mday), (tm_->tm_hour), (tm_->tm_min));
	return std::string(dateStr);
}
std::string WeekDayToStr(uint32 weekday) {
	switch (weekday)
	{
	case 0:
		return "周日";
	case 1:
		return "周一";
	case 2:
		return "周二";
	case 3:
		return "周三";
	case 4:
		return "周四";
	case 5:
		return "周五";
	case 6:
		return "周六";
	default:
		return "未知";
	}
}
void GetCurrentHour(uint32 &hour, uint32 &min, uint32 &sec)
{
	time_t t = time(NULL);
	tm* currentTime = localtime(&t);
	hour = currentTime->tm_hour;
	min = currentTime->tm_min;
	sec = currentTime->tm_sec;
}
void GetCurrentHour(uint32&wday, uint32 &hour, uint32 &min, uint32 &sec) {
	time_t t = time(NULL);
	tm* currentTime = localtime(&t);
	hour = currentTime->tm_hour;
	min = currentTime->tm_min;
	sec = currentTime->tm_sec;
	wday = currentTime->tm_wday;
}

uint32 GetTimByStr(const std::string atimestr, uint32 &startH, uint32 &startS, uint32 &endH, uint32 &endS)
{
	uint32 astime;
	sscanf(atimestr.c_str(), "%d:%d:%d-%d:%d:%d", &startH, &startS, &astime, &endH, &endS, &astime);
	return 0;
}

#ifdef WIN32
static char _StringConversionStorage[2048];
#endif

// win32 only
// cebernic added it
// for multilanguage supports
// --------------------------------------------------------------------------------------------------
SERVER_DECL const char* _StringToUTF8(const char*   pASCIIBuf)
{ 
#ifdef WIN32
	DWORD     UniCodeLen=MultiByteToWideChar(CP_ACP,   0,   pASCIIBuf,   -1,   0,   0); 
  std::vector <wchar_t>   vWCH(UniCodeLen); 
  MultiByteToWideChar(CP_ACP,   0,   pASCIIBuf,   -1,   &vWCH[0],   UniCodeLen); 
  DWORD   dwUtf8Len=WideCharToMultiByte(CP_UTF8,   0,   &vWCH[0],   UniCodeLen   ,   NULL,   NULL,   NULL,   NULL   ); 
	ASSERT( dwUtf8Len+1 < 2048 );
	memset(_StringConversionStorage,0,(sizeof(char)*dwUtf8Len)+1);
  WideCharToMultiByte(CP_UTF8,   0,   &vWCH[0],   UniCodeLen   ,   _StringConversionStorage,   dwUtf8Len,   NULL,   NULL   ); 
	return &_StringConversionStorage[0];
#else
	return &pASCIIBuf[0];
#endif
} 

#ifndef WIN32
/*
//mbstowcs()函数和wcstombs()函数，选自CRT库。平台无关，但需设定local。不设定Local,中文会乱码
bool UnicodeToAnsi(wchar_t* lpString, char* szAnsi)//(in,out)
{
	setlocale(LC_ALL,"zh_CN.UTF-8");
	setlocale(LC_ALL,"zh_CN.GB18030");
	size_t nLen = wcslen(lpString) * 2, nReturnLen;//wcslen <string.h> 
	char* lpszBuf = (char*)malloc(nLen);//malloc <stdlib.h> and <malloc.h> 
	nReturnLen = wcstombs(szAnsi, lpString, nLen);//wcstombs <stdlib.h> 
	if (nReturnLen<0)
	{
		free(lpszBuf);
		return false;
	}
	strcpy(szAnsi, lpszBuf);
	free(lpszBuf);
	return true;
}
*/
#endif

SERVER_DECL const char* _StringToANSI(const char*   pUtf8Buf)
{ 
#ifdef WIN32
	DWORD   UniCodeLen=MultiByteToWideChar(CP_UTF8,   0,   pUtf8Buf,   -1,   NULL,0   ); 
  std::vector <wchar_t>   vWCH(UniCodeLen); 
  MultiByteToWideChar(CP_UTF8,   0,   pUtf8Buf,   -1,   &vWCH[0]   ,   UniCodeLen   ); 
  DWORD   dwASCIILen=WideCharToMultiByte(CP_ACP,   0,   &vWCH[0],   UniCodeLen   ,   NULL   ,NULL   ,   NULL,   NULL   ); 
	ASSERT( dwASCIILen+1 < 2048 );
	memset(_StringConversionStorage,0,(sizeof(char)*dwASCIILen)+1);
  WideCharToMultiByte(CP_ACP,   0,   &vWCH[0],   UniCodeLen   ,   _StringConversionStorage,   dwASCIILen,   NULL,   NULL   ); 
	return &_StringConversionStorage[0];
#else
	char wcstr[1000];
	UnicodeToAnsi((wchar_t*)pUtf8Buf,wcstr);
	return pUtf8Buf;
#endif
} 



SERVER_DECL bool _IsStringUTF8(const char *str)
{
    int   i;
    unsigned char cOctets;  // octets to go in this UTF-8 encoded character
    unsigned char chr;
    bool  bAllAscii= TRUE;
    long iLen = (long)strlen(str);
 
    cOctets= 0;
    for( i= 0; i <iLen; i++ ) {
 
     chr = (unsigned char)str[i];
 
     if( (chr & 0x80) != 0 ) bAllAscii= FALSE;
 
     if( cOctets == 0 ) {
        if( chr>= 0x80 )  {
            do  {
                chr <<= 1;
                cOctets++;
            }
            while( (chr & 0x80) != 0 );
            
            cOctets--;                        
            if( cOctets == 0 ) return FALSE;  
        }
     }
     else  {
        if( (chr & 0xC0) != 0x80 ) 
            return FALSE;
 
        cOctets--;                       
     }
    }
    if( cOctets> 0 ) 
     return FALSE;
    if( bAllAscii ) 
     return FALSE;
    return TRUE;

 } 

std::string string_toLower(const std::string& src)
{
	std::string rtn;
	//if (src.size() > 0)
	//{
	//	rtn.resize(src.size());
	//	transform(begin(src), end(src), begin(rtn), tolower);
	//}
	//arcemu_TOLOWER(const_cast<string&>(src));
	rtn = src;
	arcemu_TOLOWER(rtn);
	return rtn;
}

std::string string_toUpper(const std::string& src)
{
	std::string rtn;
	//if (src.size() > 0)
	//{
	//	rtn.resize(src.size());
	//	transform(begin(src), end(src), begin(rtn), toupper);
	//}
	//arcemu_TOUPPER(const_cast<string&>(src));
	rtn = src;
	arcemu_TOUPPER(rtn);
	return rtn;
}

extern void RaiseException(const char* fmt, ...)
{
	va_list vp;
	char buffer[4096] = {};
	va_start(vp, fmt);
	vsnprintf(buffer, sizeof(buffer), fmt, vp);
	va_end(vp);
	 
	logic_error ex(buffer);
	throw std::exception(ex);
}

extern std::string FormatString(const char* fmt, ...)
{
	
	char buffer[8192] = {};
	va_list vp;
	va_start(vp, fmt);
	vsnprintf(buffer, sizeof(buffer),fmt, vp);
	va_end(vp);
	return std::string(buffer);
}

std::string int32tostr(int32 val)
{
	char buf[32] = { 0 };
	snprintf(buf, sizeof(buf), "%d", val);
	return std::string(buf);
}

std::string uint32tostr(uint32 val)
{
	char buf[32] = { 0 };
	snprintf(buf, sizeof(buf), "%u", val);
	return std::string(buf);
}

std::string int64tostr(int64 val)
{
	char buf[64] = { 0 };
	snprintf(buf, sizeof(buf), "%I64d", val);
	return std::string(buf);
}

std::string uint64tostr(uint64 val)
{
	char buf[64] = { 0 };
	sprintf_s(buf, 64, "%llu", val);
	return std::string(buf);
}

int32 strtoint32(const std::string& val)
{
	if (isNum(val))
		return atoi(val.c_str());
	return 0;
}

uint32 strtouint32(const std::string& val)
{
	if (isNum(val))
		return strtoul(val.c_str(), NULL, 10);
	return 0;
}

bool isNum(const std::string& val)
{
	stringstream sin(val);
	double d;
	char c;
	if (!(sin >> d))
		return false;
	if (sin >> c)
		return false;
	return true;
}

uint64 strtouint64(const std::string& val)
{
	/* 由于其它lib里面有_strtoull()函数的定义，
	   跟系统的strtoull()重名，故使用该方法*/

	if (isNum(val)) {
		uint64 Ret;
		sscanf(val.c_str(), "%I64u", &Ret);
		return Ret;
		//return strtoull(val.c_str(), NULL, 10);
	}
	return 0;
}

uint8 ToHex(uint8 x)
{
	return  x > 9 ? x + 55 : x + 48;
}

//uint8 FromHex(uint8 x)
//{
//	uint8 y;
//	if (x >= 'A' && x <= 'Z') y = x - 'A' + 10;
//	else if (x >= 'a' && x <= 'z') y = x - 'a' + 10;
//	else if (x >= '0' && x <= '9') y = x - '0';
//	else assert(0);
//	return y;
//}

uint64 TimeToUint64(time_t time)
{
	struct tm * timeinfo = localtime(&time);
	return uint64((timeinfo->tm_year + 1900) * 10000000000
		+ (timeinfo->tm_mon + 1) * 100000000
		+ timeinfo->tm_mday * 1000000
		+ timeinfo->tm_hour * 10000
		+ timeinfo->tm_min * 100
		+ timeinfo->tm_sec);
}

bool IsLegal(int year, int mon, int day, int hour, int minu, int sec)
{
	//大：1 3 5 7 8 10 12
	//小：4 6 9 11
	//平：2

	if (year < 0 || mon <= 0 || mon > 12 || day <= 0 || day > 31 || hour < 0 || hour > 23 || minu < 0 || minu > 59 || sec < 0 || sec > 60)
		return false;

	if (1 == mon || 3 == mon || 5 == mon || 7 == mon || 8 == mon || 10 == mon || 12 == mon)
		return true;
	if (IsLeapYear(year)) 
	{
		if (2 == mon && (28 == day || 30 == day || 31 == day))
			return false;
	}
	else {
		if (2 == mon && (29 == day || 30 == day || 31 == day))
			return false;
	}
	return true;
}

bool validateTime(uint64 t)
{
	string st = to_string(t);
	if (st.empty() || st.size() > 14)
		return false;

	int year = (int)(t / 10000000000);
	t -= year * 10000000000;
	year -= 1900;

	int mon = (int)(t / 100000000);
	t -= mon * 100000000;

	int day = (int)(t / 1000000);
	t -= day * 1000000;

	int hour = (int)(t / 10000);
	t -= hour * 10000;

	int minu = (int)(t / 100);
	t -= minu * 100;

	int sec = (int)t;

	if (!IsLegal(year, mon, day, hour, minu, sec))
		return false;
	return true;
}

time_t uint64ToTime(uint64 t)
{
	// 20170101103000
	tm tTime = { 0 };
	tTime.tm_year = (int)(t / 10000000000);
	t -= tTime.tm_year * 10000000000;
	tTime.tm_year -= 1900;
	tTime.tm_mon = (int)(t / 100000000);
	t -= tTime.tm_mon * 100000000;
	tTime.tm_mon--;

	tTime.tm_mday = (int)(t / 1000000);
	t -= tTime.tm_mday * 1000000;

	tTime.tm_hour = (int)(t / 10000);
	t -= tTime.tm_hour * 10000;
	tTime.tm_min = (int)(t / 100);
	t -= tTime.tm_min * 100;
	tTime.tm_sec = (int)t;
	return mktime(&tTime);
}

std::string UrlEncode(const std::string& str)
{
	std::string strTemp = "";
	size_t length = str.length();
	for (size_t i = 0; i < length; i++)
	{
		if (isalnum((unsigned char)str[i]) ||
			(str[i] == '-') ||
			(str[i] == '_') ||
			(str[i] == '.') ||
			(str[i] == '~'))
			strTemp += str[i];
		else if (str[i] == ' ')
			strTemp += "+";
		else
		{
			strTemp += '%';
			strTemp += ToHex((unsigned char)str[i] >> 4);
			strTemp += ToHex((unsigned char)str[i] % 16);
		}
	}
	return strTemp;
}

std::string UrlDecode(const std::string& str)
{
	std::string strTemp = "";
	size_t length = str.length();
	for (size_t i = 0; i < length; i++)
	{
		if (str[i] == '+') strTemp += ' ';
		else if (str[i] == '%')
		{
			assert(i + 2 < length);
			unsigned char high = FromHex((unsigned char)str[++i]);
			unsigned char low = FromHex((unsigned char)str[++i]);
			strTemp += high * 16 + low;
		}
		else strTemp += str[i];
	}
	return strTemp;
}

void UTF_8ToUnicode(wchar_t* pOut, char *pText)
{
	char* uchar = (char *)pOut;
	uchar[1] = ((pText[0] & 0x0F) << 4) + ((pText[1] >> 2) & 0x0F);
	uchar[0] = ((pText[1] & 0x03) << 6) + (pText[2] & 0x3F);
	return;
}

void UnicodeToUTF_8(char* pOut, wchar_t* pText)
{
	// 注意 WCHAR高低字的顺序,低字节在前，高字节在后
	char* pchar = (char *)pText;

	pOut[0] = (0xE0 | ((pchar[1] & 0xF0) >> 4));
	pOut[1] = (0x80 | ((pchar[1] & 0x0F) << 2)) + ((pchar[0] & 0xC0) >> 6);
	pOut[2] = (0x80 | (pchar[0] & 0x3F));

	return;
}


bool IsPathDelimiter(const std::string& path, int index) {

	if ((index >= 0) && (index < (int)path.size())) {
		char symbol = path[index];
		return (symbol == '\\') || (symbol == '/');;
	}
	return false;
}

std::string ExcludePathDelimiter(const std::string& path) {
	std::string s = path;
	if (IsPathDelimiter(path, path.size() - 1))
		s.resize(s.size() - 1);
	return s;

}

bool ExistDirectory(const std::string& path) {

	WIN32_FIND_DATA findData;
	findData.dwFileAttributes = 0;
	HANDLE hFind = FindFirstFile(path.c_str(), &findData);
	if (hFind != INVALID_HANDLE_VALUE) {
		FindClose(hFind);
		return (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0; 
	}
	return false;
}

bool CreateDir(std::string& dir) {
	return CreateDirectory(dir.c_str(), nullptr) == TRUE;
}

std::string ExtractFilePath(const std::string path)
{
	size_t posWinDelimiter = path.find_last_of('\\');
	size_t posLinuxDelimiter = path.find_last_of('/');
	size_t posDriverDelimiter = path.find_last_of(':');
	size_t pos = -1;
	if (posWinDelimiter != string::npos) {
		pos = posWinDelimiter;
	}

	if (posLinuxDelimiter != string::npos) {
		pos = max(posLinuxDelimiter, pos);
	}

	if (posDriverDelimiter != string::npos) {
		pos = max(posDriverDelimiter, pos);
	}

	if (pos == -1)
		return path;
	else 
		return path.substr(0, pos + 1);	
}

bool ForceDirectory(const std::string &path) {
	std::string dir = path;
	dir = ExcludePathDelimiter(dir);

	if (ExistDirectory(dir))
		return true;

	std::string parentPath = ExtractFilePath(dir);
	if ((dir.size() < 3) || (parentPath == dir))
		return CreateDir(dir);
	else
		return ForceDirectory(parentPath) && CreateDir(dir);

}

string IncludePathDelimiter(const std::string &path) {
	if (!IsPathDelimiter(path, path.size() - 1)) {
		return path + '\\';
	}
	return path;
}

string ExpandFilename(const std::string& filename) {
	char buff[4096];
	std::string result;
	int count = GetCurrentDirectory(sizeof(buff), buff);
	if (count > 0)
		result = IncludePathDelimiter(buff) + filename;
	else
		result = filename;
	return result;
}

bool GetRandList(uint32 min, uint32 max, uint32 count, std::vector<uint32>& tar)
{
	// 当前范围没有足够的可随机数;
	if (max - min + 1 < count)
		return false;

	std::vector<uint32> src;
	for (uint32 i = min; i <= max; i++)
		src.push_back(i);
	uint32 size = src.size();
	if (size == 0)
		return false;
	for (uint32 i = 0; i < count; i++)
	{
		uint32 index = rand() % size;
		tar.push_back(src[index]);
		size--;
		std::swap(src[index], src[size]);
	}
	return true;
}

int32 GetFileSize(FILE* fp)
{
	int32 curpos = 0, length = 0;
	curpos = ftell(fp);
	fseek(fp, 0L, SEEK_END);
	length = ftell(fp);
	fseek(fp, curpos, SEEK_SET);
	return length;
}

void GetDirectoryFiles(const std::string& path, std::vector<std::string>& files, const std::string slash)
{
	// 文件句柄;
	long   hFile = 0;
	// 文件信息; 
	struct _finddata_t fileinfo;

	std::string checkpath = path + slash;
	if ((hFile = _findfirst((checkpath + "*").c_str(), &fileinfo)) != -1)
	{
		do
		{
			// 如果是目录,迭代之;
			// 如果不是,加入列表; 
			if ((fileinfo.attrib & _A_SUBDIR))
			{
				if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)
					GetDirectoryFiles(path + slash + fileinfo.name, files);
			}
			else
				files.push_back(checkpath + fileinfo.name);
		} while (_findnext(hFile, &fileinfo) == 0);
		_findclose(hFile);
	}
}

time_t GetTimeStamp(uint32 week, uint32 hour, uint32 min, uint32 sec)
{
	time_t now = time(NULL);
	tm t0 = { 0 };
	localtime_s(&t0, &now);
	tm t1 = t0;
	t1.tm_hour = hour;
	t1.tm_min = min;
	t1.tm_sec = sec;
	time_t startTime = mktime(&t1);

	uint32 wk = t0.tm_wday == 0 ? 7 : t0.tm_wday;

	if (wk < week)
		startTime += (week - wk) * 60 * 60 * 24;
	else if (wk > week)
		startTime += (7 - wk + week) * 60 * 60 * 24;

	return startTime;
}
time_t GetTimeStamp(time_t t, uint32 week, uint32 hour, uint32 min, uint32 sec)
{
	tm t0 = { 0 };
	localtime_s(&t0, &t);
	tm t1 = t0;
	t1.tm_hour = hour;
	t1.tm_min = min;
	t1.tm_sec = sec;
	time_t startTime = mktime(&t1);

	uint32 wk = t0.tm_wday == 0 ? 7 : t0.tm_wday;

	if (wk < week)
		startTime += (week - wk) * 60 * 60 * 24;
	else if (wk > week)
		startTime += (7 - wk + week) * 60 * 60 * 24;

	return startTime;
}
time_t GetAppointTimeStamp(time_t t, uint32 hour, uint32 min, uint32 sec)
{
	tm t0 = { 0 };
	localtime_s(&t0, &t);
	t0.tm_hour = hour;
	t0.tm_min = min;
	t0.tm_sec = sec;
	return mktime(&t0);
}

uint32 RandBetween1(uint32 st, uint32 end)
{
	if (end <= st)
		return end;

	return st + rand() % (end-st);
}

uint32 RandBetween2(uint32 st, uint32 end)
{
	return RandBetween1(st,end)+1;
}

std::string GetCurrExeName()
{
	char szFileFullPath[MAX_PATH] = {};
	::GetModuleFileNameA(NULL, szFileFullPath, MAX_PATH);//获取文件路径  

	std::string exeName = szFileFullPath;
	std::string::size_type pos = exeName.find_last_of('\\');
	pos++;
	exeName = exeName.substr(pos);
	return exeName;
}


std::string hexdump(const unsigned char *buf, const int num)
{
	char szBuffer[20480] = {};
	int tlen = 20480;
	uint32 len = 0;
	if (num <= 0) return szBuffer;

	sprintf_s(szBuffer + len, tlen - len, "\n");
	for (int i = 0; i < num; i++)
	{
		sprintf_s(szBuffer + len, tlen - len, "%02X ", buf[i]);
		len = strlen(szBuffer);
		if ((i + 1) % 8 == 0) {
			sprintf_s(szBuffer + len, tlen - len, "\n");
			len = strlen(szBuffer);
		}
	}
	sprintf_s(szBuffer + len, tlen - len, "\n");
	return szBuffer;
}


// 将utf8转换到wstring
std::wstring Utf8ToUnicode(const char* utf8string)
{
	wstring unicodeString;
	DWORD  len = MultiByteToWideChar(CP_UTF8, 0, utf8string, -1, NULL, 0);
	unicodeString.resize(len);
	MultiByteToWideChar(CP_UTF8, 0, utf8string, -1, (LPWSTR)unicodeString.data(), len);
	return unicodeString;
}

size_t UTF8_GetCharCount(const char* utf8string) 
{	
	DWORD  len = MultiByteToWideChar(CP_UTF8, 0, utf8string, -1, NULL, 0);
	return len > 0 ? len - 1 : len;
}

std::string ConvertUtf8ToGBK(const std::string& strUtf8)
{
	std::string gbkString;
	int len = MultiByteToWideChar(CP_UTF8, 0, (LPCTSTR)strUtf8.c_str(), -1, NULL, 0);
	WCHAR *wszGBK = new WCHAR[len + 1];
	memset(wszGBK, 0, len * 2 + 2);
	MultiByteToWideChar(CP_UTF8, 0, (LPCTSTR)strUtf8.c_str(), -1, wszGBK, len);

	len = WideCharToMultiByte(CP_ACP, 0, wszGBK, -1, NULL, 0, NULL, NULL);
	gbkString.resize(len);	
	WideCharToMultiByte(CP_ACP, 0, wszGBK, -1, (LPSTR)gbkString.data(), len, NULL, NULL);
	delete[] wszGBK;
	return gbkString;		
	
}

extern size_t UTF8_GetCharSize(const char* utf8string)
{
	DWORD  len = MultiByteToWideChar(CP_UTF8, 0, utf8string, -1, NULL, 0);
	wstring unicode; 
	unicode.resize(len);
	MultiByteToWideChar(CP_UTF8, 0, utf8string, -1, (LPWSTR)unicode.data(), len);

	len = WideCharToMultiByte(CP_ACP, 0, (LPWSTR)unicode.data(), -1, NULL, 0, NULL, NULL);
	return 	len > 0 ? len - 1 : len;
	//string localString;
	//localString.resize(len);
	//len = WideCharToMultiByte(CP_ACP, 0, (LPWSTR)unicode.data(), -1, (LPSTR)localString.data(), len, NULL, NULL);

}

extern void ShowErrorMsgBox(const std::string& title, const std::string& content)
{
	INT_PTR nRes;
	nRes = MessageBox(NULL, content.c_str(), _T(title.c_str()), MB_OKCANCEL | MB_ICONERROR);
}

extern uint32 Uint32Division(uint32 Divisor, uint32 beDiv)
{
	if (beDiv == 0) {
		return 0;
	}
	return Divisor / beDiv;
}

extern float FloatDivison(float Divisor, float beDiv)
{
	if (beDiv < 0.001f) {
		return 0.0f;
	}
	return Divisor / beDiv;
}

extern bool CheckProcessSingle(std::string processName)
{
	string mutexName = "Global\\" + processName;
	HANDLE handle = CreateMutex(NULL, TRUE, mutexName.c_str());
	if ((GetLastError() == ERROR_ALREADY_EXISTS) || (NULL == handle))
	{
		sLog.outToFile("proc_single error", "已存在%s!!!!!",processName.c_str());
		return false;
	}
	return true;
}
extern bool isSameWeek(time_t t1, time_t t2) {
	time_t big, smal;
	if (t1 >= t2) {
		big = t1;
		smal = t2;
	}
	else {
		big = t2;
		smal = t1;
	}
	struct tm  bigtm = { 0 };
	localtime_s(&bigtm, &big);
	struct tm  smaltm = { 0 };
	localtime_s(&smaltm, &smal);
	int days = bigtm.tm_yday - smaltm.tm_yday;
	bigtm.tm_wday = bigtm.tm_wday == 0 ?  7: bigtm.tm_wday;
	smaltm.tm_wday = smaltm.tm_wday == 0 ? 7 : smaltm.tm_wday;
	int weeks = bigtm.tm_wday - smaltm.tm_wday;
	if (abs(days) < 7 && weeks >= 0) {
		return true;
	}
	return false;

}
extern bool isNextWeek(time_t now, time_t nextweek) {
	if (nextweek <= now)
		return false;
	time_t thisweekend = GetTimeStamp(now, 0, 23, 59, 59);
	time_t nextweekend = thisweekend + 7 * 60 * 60 * 24;
	return nextweek > thisweekend&&nextweek <= nextweekend;
}
std::string CompileTime()
{
	SYSTEMTIME st;
	Arcemu::SysInfo::GetProgramCompileTime(st);
	//sLog.outError("shared:compiletime", "mon:%u d:%u h:%u m:%u s:%u ms:%u ", st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
	GetProgramCompileTime1(st);
	//sLog.outError("gameworld:compiletime", "mon:%u d:%u h:%u m:%u s:%u ms:%u ", st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
	char ss[1024];
	sprintf(ss, "%u/%u/%u %u:%u:%u ms:%u ", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
	return ss;
}