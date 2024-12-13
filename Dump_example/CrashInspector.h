#pragma once
#include <windows.h>
#include <DbgHelp.h>
#pragma comment(lib, "dbghelp.lib")

#include <string>

enum DumpMode {
	dump_mode_mini = 1,
	dump_mode_simple = 2,
	dump_mode_full = 4
};

enum DumpReason {
	// ǿ�Ʋ�����dump
	dump_reason_force,
	// ��⵽���̹߳���ʱ������dump
	dump_reason_hang,
	// ��ϵͳ��׽���쳣
	dump_reason_catched
};

// �Զ���DumpExceptionInfo, ���ڲ����쳣Dump�ļ�
struct DumpExceptionInfo
{
	DumpMode dumpMode;
	DumpReason dumpReason;  // dump ������ԭ��
	std::string  exceptionMessage;
};

// �ص���������������
typedef void(*ExceptionCallBack)(DumpExceptionInfo*);

const DWORD DumpExceptionCode = 0xE06D7364;
const DWORD CPPExceptionCode = 0xE06D7363;
const DWORD SetThreadNameExcetpionCode = 0x406D1388;


class CrashInspector
{
public:
	static void Install();
	static void Uninstall();
	static void SetDumpMode(DumpMode dmpMode);
	static void EnableAutoDump();
	static int GetExceptionCount();
	static void ResetExceptionCount();
	static void SetExceptionCallback(ExceptionCallBack callback);
};


extern void RaiseDumpException(DumpMode dumpMode,const char* message);
extern void RaiseHangDumpException(DumpMode dumpMode);

extern void SetThreadNameByThreadID(DWORD thread_id,const char* thread_name);
extern void SetThreadName(const char* fmt, ...);