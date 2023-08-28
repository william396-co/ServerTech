/*
 * ArcEmu MMORPG Server
 * Copyright (C) 2008-2012 <http://www.ArcEmu.org/>
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

#ifndef ARCEMU_PLATFORM_H
#define ARCEMU_PLATFORM_H

#ifdef WIN32
#pragma warning(disable:4996)
#define _CRT_SECURE_NO_DEPRECATE 1
#define _CRT_SECURE_COPP_OVERLOAD_STANDARD_NAMES 1
#pragma warning(disable:4251)		// dll-interface bullshit
#endif


#include <cstdlib>
#include <cstdio>

#include <cstdarg>
#include <ctime>
#include <cmath>
#include <cerrno>

#if defined( __WIN32__ ) || defined( WIN32 ) || defined( _WIN32 )
#  define WIN32_LEAN_AND_MEAN
//#  define _WIN32_WINNT 0x0500
#  define NOMINMAX
#  include <windows.h>
#  undef NOMINMAX
#else
#  include <cstring>
#  define MAX_PATH 1024
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
#  define ARCEMU_PLATFORM PLATFORM_WIN32
#elif defined( __APPLE_CC__ )
#  define ARCEMU_PLATFORM PLATFORM_APPLE
#else
#  define ARCEMU_PLATFORM PLATFORM_UNIX
#endif

#define COMPILER_MICROSOFT 0
#define COMPILER_GNU	   1
#define COMPILER_BORLAND   2

#ifdef _MSC_VER
#  define ARCEMU_COMPILER COMPILER_MICROSOFT
#elif defined( __BORLANDC__ )
#  define ARCEMU_COMPILER COMPILER_BORLAND
#elif defined( __GNUC__ )
#  define ARCEMU_COMPILER COMPILER_GNU
#else
#  pragma error "FATAL ERROR: Unknown compiler."
#endif

#if ARCEMU_PLATFORM == PLATFORM_UNIX || ARCEMU_PLATFORM == PLATFORM_APPLE
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

#if ARCEMU_PLATFORM == PLATFORM_WIN32
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


#if ARCEMU_PLATFORM == PLATFORM_WIN32
#define STRCASECMP stricmp
#else
#define STRCASECMP strcasecmp
#endif

#if ARCEMU_PLATFORM == PLATFORM_WIN32
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
#include <thread>
#include <atomic>
#include <cassert>
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

#ifndef HAS_CXX0X
#if ARCEMU_COMPILER == COMPILER_MICROSOFT && _MSC_VER > 1600
#define HAS_CXX0X
#endif
#endif

/* Use correct types for x64 platforms, too */
#if ARCEMU_COMPILER != COMPILER_GNU
typedef signed __int64 int64;
typedef signed __int32 int32;
typedef signed __int16 int16;
typedef signed __int8 int8;

typedef unsigned __int64 uint64;
typedef unsigned __int32 uint32;
typedef unsigned __int16 uint16;
typedef unsigned __int8 uint8;
#else

typedef int64_t int64;
typedef int32_t int32;
typedef int16_t int16;
typedef int8_t int8;
typedef uint64_t uint64;
typedef uint32_t uint32;
typedef uint16_t uint16;
typedef uint8_t uint8;
//typedef uint32_t DWORD;

#endif

/*
Scripting system exports/imports
*/

#ifdef WIN32
#ifndef SCRIPTLIB
#define SERVER_DECL __declspec(dllexport)
#define SCRIPT_DECL __declspec(dllimport)
#else
#define SERVER_DECL __declspec(dllimport)
#define SCRIPT_DECL __declspec(dllexport)
#endif
#define DECL_LOCAL
#elif defined __GNUC__ && __GNUC__ >= 4
#define SERVER_DECL __attribute__((visibility ("default")))
#define SCRIPT_DECL __attribute__((visibility ("default")))
#define DECL_LOCAL __attribute__((visibility ("hidden")))
#else
#define SERVER_DECL
#define SCRIPT_DECL
#define DECL_LOCAL
#endif


#if ARCEMU_COMPILER == COMPILER_MICROSOFT

#define I64FMT "%016I64X"
#define I64FMTD "%I64u"
#define SI64FMTD "%I64d"
#define snprintf _snprintf
#define atoll __atoi64

#else

#define stricmp strcasecmp
#define strnicmp strncasecmp
#define I64FMT "%016lX"
#define I64FMTD "%llu"
#define SI64FMTD "%lld"

#endif

#endif
