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

#ifndef WOWSERVER_COMMON_H
#define WOWSERVER_COMMON_H

 /* Define these if you are creating a repack */
 /*
 #define REPACK "Moocow's Repack"
 #define REPACK_AUTHOR "Trelorn"
 #define REPACK_WEBSITE "www.google.com"*/

#ifdef WIN32
#pragma warning(disable:4996)
#pragma warning(disable:4091)
#define _CRT_SECURE_NO_DEPRECATE 1
#define _CRT_SECURE_COPP_OVERLOAD_STANDARD_NAMES 1
#pragma warning(disable:4251)		// dll-interface bullshit
#endif

enum TimeVariables
{
	TIME_SECOND = 1,
	TIME_MINUTE = TIME_SECOND * 60,
	TIME_HOUR = TIME_MINUTE * 60,
	TIME_DAY = TIME_HOUR * 24,
	TIME_MONTH = TIME_DAY * 30,
	TIME_YEAR = TIME_MONTH * 12
};

enum MsTimeVariables : unsigned int
{
	MSTIME_500MS = 500,						//200ms
	MSTIME_SECOND = 1000,						// 1秒
	MSTIME_5SECONDS = MSTIME_SECOND * 5,		// 5秒
	MSTIME_6SECONDS = MSTIME_SECOND * 6,		// 6秒
	MSTIME_10SECONDS = MSTIME_SECOND * 10,		// 10秒
	MSTIME_15SECONDS = MSTIME_SECOND * 15,		// 15秒
	MSTIME_30SECONDS = MSTIME_SECOND * 30,		// 30秒
	MSTIME_MINUTE = MSTIME_SECOND * 60,		// 1分钟

	MSTIME_5MINUTES = MSTIME_MINUTE * 5,		// 10分钟
	MSTIME_10MINUTES = MSTIME_MINUTE * 10,		// 10分钟
	MSTIME_HOUR = MSTIME_MINUTE * 60,		// 1小时
	MSTIME_2HOUR = MSTIME_HOUR * 2,			// 2小时
	MSTIME_DAY = MSTIME_HOUR * 24			// 24小时
};

#ifdef WIN32
#define Cross_FORCEINLINE __forceinline
#else
#define Cross_FORCEINLINE inline
#endif
#define Cross_INLINE inline

#ifdef HAVE_CONFIG_H
//# include <config.h>
#endif

//#include "arcemuConfig.h"

#include <cstdlib>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <time.h>
#include <math.h>
#include <errno.h>

#if defined( __WIN32__ ) || defined( WIN32 ) || defined( _WIN32 )
#define WIN32_LEAN_AND_MEAN
//#define _WIN32_WINNT 0x0600
#define NOMINMAX
#include <windows.h>
#include <corecrt_io.h>
#else
#include <string.h>
#define MAX_PATH 1024
#endif

#ifdef min
#undef min
#endif

#ifdef max
#undef max
#endif

#ifdef CONFIG_USE_SELECT
#undef FD_SETSIZE
#define FD_SETSIZE 2048 
#endif

#if defined( __WIN32__ ) || defined( WIN32 ) || defined( _WIN32 )
#include <winsock2.h>
#include <ws2tcpip.h>
#else
#include <sys/time.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <signal.h>
#include <netdb.h>
#endif

// current platform and compiler
#define PLATFORM_WIN32 0
#define PLATFORM_UNIX  1
#define PLATFORM_APPLE 2

#define UNIX_FLAVOUR_LINUX 1
#define UNIX_FLAVOUR_BSD 2
#define UNIX_FLAVOUR_OTHER 3
#define UNIX_FLAVOUR_OSX 4

#if defined( __WIN32__ ) || defined( WIN32 ) || defined( _WIN32 )
#  define PLATFORM PLATFORM_WIN32
#elif defined( __APPLE_CC__ )
#  define PLATFORM PLATFORM_APPLE
#else
#  define PLATFORM PLATFORM_UNIX
#endif

#define COMPILER_MICROSOFT 0
#define COMPILER_GNU	   1
#define COMPILER_BORLAND   2

#ifdef _MSC_VER
#  define COMPILER COMPILER_MICROSOFT
#elif defined( __BORLANDC__ )
#  define COMPILER COMPILER_BORLAND
#elif defined( __GNUC__ )
#  define COMPILER COMPILER_GNU
#else
#  pragma error "FATAL ERROR: Unknown compiler."
#endif

#if PLATFORM == PLATFORM_UNIX || PLATFORM == PLATFORM_APPLE
#ifdef HAVE_DARWIN
#define PLATFORM_TEXT "MacOSX"
#define UNIX_FLAVOUR UNIX_FLAVOUR_OSX
#else
#ifdef USE_KQUEUE
#define PLATFORM_TEXT "FreeBSD"
#define UNIX_FLAVOUR UNIX_FLAVOUR_BSD
#else
#define PLATFORM_TEXT "Linux"
#define UNIX_FLAVOUR UNIX_FLAVOUR_LINUX
#endif
#endif
#endif

#if PLATFORM == PLATFORM_WIN32
#define PLATFORM_TEXT "Win32"
#endif

#ifdef _DEBUG
#define CONFIG "Debug"
#else
#define CONFIG "Release"
#endif

#ifdef X64
#define ARCH "X64"
#else
#define ARCH "X86"
#endif


#if PLATFORM == PLATFORM_WIN32
#define STRCASECMP stricmp
#else
#define STRCASECMP strcasecmp
#endif

#if PLATFORM == PLATFORM_WIN32
#define ASYNC_NET
#endif

#ifdef USE_EPOLL
#define CONFIG_USE_EPOLL
#endif
#ifdef USE_KQUEUE
#define CONFIG_USE_KQUEUE
#endif
#ifdef USE_SELECT
#define CONFIG_USE_SELECT
#endif
#ifdef USE_POLL
#define CONFIG_USE_POLL
#endif

#ifdef min
#undef min
#endif

#ifdef max
#undef max
#endif

#include <cstdlib>
#include <set>
#include <list>
#include <string>
#include <map>
#include <queue>
#include <sstream>
#include <algorithm>
#include <cstring>
#include <climits>
#include <cstdlib>
//#include <iostream>

#if defined ( __GNUC__ )
#	define LIKELY( _x ) \
		__builtin_expect( ( _x ), 1 )
#	define UNLIKELY( _x ) \
 		__builtin_expect( ( _x ), 0 )
#else
#	define LIKELY( _x ) \
		_x
#	define UNLIKELY( _x ) \
		_x
#endif

#if defined (__GNUC__)
#  define GCC_VERSION (__GNUC__ * 10000 \
					   + __GNUC_MINOR__ * 100 \
					   + __GNUC_PATCHLEVEL__)
#endif


#ifndef WIN32
#ifndef X64
#  if defined (__GNUC__)
#	if GCC_VERSION >= 30400
#         ifdef HAVE_DARWIN
#	      define __fastcall
#         else
#    	      define __fastcall __attribute__((__fastcall__))
#         endif
#	else
#	  define __fastcall __attribute__((__regparm__(3)))
#	endif
#  else
#	define __fastcall __attribute__((__fastcall__))
#  endif
#else
#define __fastcall  
#endif
#endif

#ifdef HAVE_STDCXX_0X
#include <unordered_map>
#include <unordered_set>
#elif COMPILER == COMPILER_GNU && __GNUC__ >= 3
#include <ext/hash_map>
#include <ext/hash_set>
#else
// #include <hash_map>
// #include <hash_set>
#endif



#ifdef _STLPORT_VERSION
#define HM_NAMESPACE std
using std::hash_map;
using std::hash_set;
#elif COMPILER == COMPILER_MICROSOFT && _MSC_VER >= 1300
#define HM_NAMESPACE stdext
// using stdext::hash_map;
// using stdext::hash_set;
#define ENABLE_SHITTY_STL_HACKS 1

// hacky stuff for vc++
#define snprintf _snprintf
#define vsnprintf _vsnprintf
//#define strlen lstrlen


// cebernic added it
#define __utf8(x) _StringToUTF8(x)
#define __ansi(x) _StringToANSI(x)
#define __isutf8(x) _IsStringUTF8(x)


#elif COMPILER == COMPILER_INTEL
#define HM_NAMESPACE std
using std::hash_map;
using std::hash_set;
#elif defined(HAVE_STDCXX_0X)
#define HM_NAMESPACE std
#define hash_map unordered_map
#define hash_set unordered_set
using std::unordered_map;
using std::unordered_set;
#elif COMPILER == COMPILER_GNU && __GNUC__ >= 3
#define HM_NAMESPACE __gnu_cxx
using __gnu_cxx::hash_map;
using __gnu_cxx::hash_set;

namespace __gnu_cxx
{
	template<> struct hash<unsigned long long>
	{
		size_t operator()(const unsigned long long& __x) const { return (size_t)__x; }
	};
	template<typename T> struct hash<T*>
	{
		size_t operator()(T* const& __x) const { return (size_t)__x; }
	};

};

#else
#define HM_NAMESPACE std
using std::hash_map;
#endif

#include "CommonTypes.h"
/*
Scripting system exports/imports
*/

#ifdef WIN32
// 	#ifndef SCRIPTLIB
// 		#define SERVER_DECL __declspec(dllexport)
// 		#define SCRIPT_DECL __declspec(dllimport)
// 	#else
// 		#define SERVER_DECL __declspec(dllimport)
// 		#define SCRIPT_DECL __declspec(dllexport)
// 	#endif
#define SERVER_DECL 
#define SCRIPT_DECL 
#else

#define SERVER_DECL 
#define SCRIPT_DECL 
#endif

// Include all threading files
#include <assert.h>
#include "Threading/Threading.h"

#include "MersenneTwister.h"

#if COMPILER == COMPILER_MICROSOFT

#define I64FMT "%016I64X"
#define I64FMTD "%I64u"
#define SI64FMTD "%I64d"
#define snprintf _snprintf
#define atoll __atoi64

#else

#define stricmp strcasecmp
#define strnicmp strncasecmp
#define I64FMT "%016llX"
#define I64FMTD "%llu"
#define SI64FMTD "%lld"

#endif

#define atol(a) strtoul( a, NULL, 10)

#define STRINGIZE(a) #a

// fix buggy MSVC's for variable scoping to be reliable =S


#if COMPILER == COMPILER_MICROSOFT && _MSC_VER >= 1400
#pragma float_control(push)
#pragma float_control(precise, on)
#endif

// fast int abs
static inline int int32abs(const int value)
{
	return (value ^ (value >> 31)) - (value >> 31);
}

// fast int abs and recast to unsigned
static inline uint32 int32abs2uint32(const int value)
{
	return (uint32)(value ^ (value >> 31)) - (value >> 31);
}

///// Fastest Method of float2int32
//static inline int float2int32(const float value)
//{
//#if !defined(X64) && COMPILER == COMPILER_MICROSOFT && !defined(USING_BIG_ENDIAN)
//	int i;
//	/*__asm {
//		fld value
//		frndint
//		fistp i
//	};*/
//	return i;
//#else
//	union { int asInt[2]; double asDouble; } n;
//	n.asDouble = value + 6755399441055744.0;
//
//	return n.asInt [0];
//#endif
//}
//
///// Fastest Method of long2int32
//static inline int long2int32(const double value)
//{
//#if !defined(X64) && COMPILER == COMPILER_MICROSOFT && !defined(USING_BIG_ENDIAN)
//	int i;
//	//__asm {
//	//	fld value
//	//	frndint
//	//	fistp i
//	//}
//	return i;
//#else
//  union { int asInt[2]; double asDouble; } n;
//  n.asDouble = value + 6755399441055744.0;
//
//  return n.asInt [0];
//#endif
//}

#if COMPILER == COMPILER_MICROSOFT && _MSC_VER >= 1400
#pragma float_control(pop)
#endif

#ifndef WIN32
#include <sys/timeb.h>
#endif

Cross_INLINE uint32 now()
{
#ifdef WIN32
	return GetTickCount();
#else
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000) + (tv.tv_usec / 1000);
#endif
}

#ifndef WIN32
#define FALSE   0
#define TRUE	1
#endif

#ifndef WIN32
#define Sleep(ms) usleep(1000*ms)
#endif

#include "Util.h"
struct WayPoint
{
	WayPoint()
	{
		o = 0.0f;
	}
	uint32 id;
	float x;
	float y;
	float z;
	float o;
	uint32 waittime; //ms
	uint32 flags;
	bool forwardemoteoneshot;
	uint32 forwardemoteid;
	bool backwardemoteoneshot;
	uint32 backwardemoteid;
	uint32 forwardskinid;
	uint32 backwardskinid;

};

struct spawn_timed_emotes
{
	uint8		type; //1 standstate, 2 emotestate, 3 emoteoneshot
	uint32		value; //get yar list elsewhere
	char* msg; //maybe we wish to say smething while changing emote state
	uint8		msg_type; //yell ? say ?
	uint8		msg_lang; //yell ? say ?
	uint32		expire_after; //going to nex faze in
};
typedef std::list<spawn_timed_emotes*> TimedEmoteList;

Cross_INLINE void reverse_array(uint8* pointer, size_t count)
{
	size_t x;
	uint8* temp = (uint8*)malloc(count);
	memcpy(temp, pointer, count);
	for (x = 0; x < count; ++x)
		pointer[x] = temp[count - x - 1];
	free(temp);
}

typedef std::vector<WayPoint*> WayPointMap;


int32 GetTimePeriodFromString(const char* str);
std::string ConvertTimeStampToString(uint32 timestamp);
std::string ConvertTimeStampToDataTime(uint32 timestamp);
uint32 DecimalToMask(uint32 dec);
//char* join3(const char *s1, const char *s2);
//char * AddtoChar(char *data, uint32 aint32);

/*
char* join3(const char *s1, const char *s2)
{
	char *result = (char *) malloc(strlen(s1) + strlen(s2) + 1);
	if (result == NULL) exit(1);
	strcpy(result, s1);
	strcat(result, s2);
	return result;
};

char * AddtoChar(char *data,uint32 aint32)
{
	char numstr[100];
	sprintf(numstr, "%d", aint32);
	return join3(data, numstr);
}

*/
//char * AddtoChar2( char *data, std::string str)
//{
//	return join3(data, (char *)str.c_str());
//}

Cross_INLINE void arcemu_TOLOWER(std::string& str)
{
	for (size_t i = 0; i < str.length(); ++i)
		str[i] = (char)tolower(str[i]);
}

Cross_INLINE void arcemu_TOUPPER(std::string& str)
{
	for (size_t i = 0; i < str.length(); ++i)
		str[i] = (char)toupper(str[i]);
}

// returns true if the ip hits the mask, otherwise false
inline static bool ParseCIDRBan(unsigned int IP, unsigned int Mask, unsigned int MaskBits)
{
	// CIDR bans are a compacted form of IP / Submask
	// So 192.168.1.0/255.255.255.0 would be 192.168.1.0/24
	// IP's in the 192.168l.1.x range would be hit, others not.
	unsigned char* source_ip = (unsigned char*)&IP;
	unsigned char* mask = (unsigned char*)&Mask;
	int full_bytes = MaskBits / 8;
	int leftover_bits = MaskBits % 8;
	//int byte;

	// sanity checks for the data first
	if (MaskBits > 32)
		return false;

	// this is the table for comparing leftover bits
	static const unsigned char leftover_bits_compare[9] = {
		0x00,			// 00000000
		0x80,			// 10000000
		0xC0,			// 11000000
		0xE0,			// 11100000
		0xF0,			// 11110000
		0xF8,			// 11111000
		0xFC,			// 11111100
		0xFE,			// 11111110
		0xFF,			// 11111111 - This one isn't used
	};

	// if we have any full bytes, compare them with memcpy
	if (full_bytes > 0)
	{
		if (memcmp(source_ip, mask, full_bytes) != 0)
			return false;
	}

	// compare the left over bits
	if (leftover_bits > 0)
	{
		if ((source_ip[full_bytes] & leftover_bits_compare[leftover_bits]) !=
			(mask[full_bytes] & leftover_bits_compare[leftover_bits]))
		{
			// one of the bits does not match
			return false;
		}
	}

	// all of the bits match that were testable
	return true;
}

inline static unsigned int MakeIP(const char* str)
{
	unsigned int bytes[4];
	unsigned int res;
	if (sscanf(str, "%u.%u.%u.%u", &bytes[0], &bytes[1], &bytes[2], &bytes[3]) != 4)
		return 0;

	res = bytes[0] | (bytes[1] << 8) | (bytes[2] << 16) | (bytes[3] << 24);
	return res;
}

/* 字符数组转换为字符串，遇到非0结尾，则取指定最大长度 (暂时这么实现，有更好的实现或者库提供的方法再更改)*/
inline std::string GetStringFromCharBuffer(char* buffer, uint32 maxLength)
{
	if ((!buffer) || (0 == maxLength))
	{
		return "";
	}

	if (buffer[maxLength - 1] == 0)
	{
		return std::string(buffer);
	}
	else
	{
		return std::string(buffer, maxLength);
	}
}


#include <cstdlib>
#include <tchar.h>
using namespace std;
static void  GetProgramCompileTime1(SYSTEMTIME& st)
{
	TCHAR szDate[20];
	ZeroMemory(szDate, sizeof(szDate));
	ZeroMemory(&st, sizeof(SYSTEMTIME));
	_stprintf(szDate, TEXT("%s"), __DATE__);

	st.wYear = (WORD)_tcstoul(szDate + 7, 0, 10);

	st.wDay = szDate[5] - TEXT('0');
	if (szDate[4] != TEXT(' '))
	{
		st.wDay += (szDate[4] - TEXT('0')) * 10;
	}

	switch (szDate[0])
	{
	case TEXT('A'):
		if (szDate[1] == TEXT('p'))
		{
			st.wMonth = 4;
		}
		else
		{
			st.wMonth = 8;
		}
		break;

	case TEXT('D'):
		st.wMonth = 12;
		break;

	case TEXT('F'):
		st.wMonth = 2;
		break;

	case TEXT('J'):
		if (szDate[1] == TEXT('a'))
		{
			st.wMonth = 1;
		}
		else
		{
			if (szDate[2] == TEXT('n'))
			{
				st.wMonth = 6;
			}
			else
			{
				st.wMonth = 7;
			}
		}
		break;

	case TEXT('M'):
		if (szDate[2] == TEXT('r'))
		{
			st.wMonth = 3;
		}
		else
		{
			st.wMonth = 5;
		}
		break;

	case TEXT('N'):
		st.wMonth = 11;
		break;

	case TEXT('O'):
		st.wMonth = 10;
		break;

	case TEXT('S'):
		st.wMonth = 9;
		break;
	}

	ZeroMemory(szDate, sizeof(szDate));
	_stprintf(szDate, TEXT("%s"), __TIME__);

	st.wHour = (szDate[0] - TEXT('0')) * 10 + (szDate[1] - TEXT('0'));
	st.wMinute = (szDate[3] - TEXT('0')) * 10 + (szDate[4] - TEXT('0'));
	st.wSecond = (szDate[6] - TEXT('0')) * 10 + (szDate[7] - TEXT('0'));
	return;
}

#endif
