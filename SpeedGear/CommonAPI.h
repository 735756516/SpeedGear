#ifndef __COMMON_API_H__
#define __COMMON_API_H__


#include <Windows.h>
#include <Shlwapi.h>
#include <Psapi.h >

#pragma comment(lib, "shlwapi.lib")
#pragma comment(lib, "Psapi.lib")

#include <tchar.h>
#include <string>
#include <strsafe.h>

using std::string;
using std::wstring;

#ifdef UNICODE
#define tstring wstring
#else
#define tstring string
#endif

#include <vector>

using std::vector;



/* 如果为 Debug 模式则会打印信息 */
#ifdef _DEBUG
#define MNOUTPUTPRINTA_YES
#define MNOUTPUTPRINTW_YES
#endif


/* 打印 ANSI 字符串 */
#ifdef MNOUTPUTPRINTA_YES
#define MNOutPutPrintA	OutputDebugStringA
#else
#define MNOutPutPrintA	//
#endif

/* 打印 UNICODE 字符串 */
#ifdef MNOUTPUTPRINTW_YES
#define MNOutPutPrintW	OutputDebugStringW
#else
#define MNOutPutPrintW	//
#endif

/* 兼容 UNICODE 字符串和 ANSI 字符串 */
#ifdef _UNICODE
#define MNOutPutPrintT	MNOutPutPrintW
#else
#define MNOutPutPrintT	MNOutPutPrintA
#endif


typedef struct _PROCESSINFO_
{
	DWORD dwProcessID;
	tstring strProcessName;
	tstring strProcessPath;
	tstring strProcessTitle;

}ProcessInfo, * PProcessInfo;


typedef struct _REMOTE_THREAD_PARAMETER
{
	CHAR m_strDllPath[MAX_PATH];
	DWORD m_dwLoadLibraryAAddr;
	DWORD m_dwLoadLibraryWAddr;
	DWORD m_dwFreeLibraryAddr;
	DWORD m_dwGetProcAddrAddr;

}RemoteThreadPara, * PRemoteThreadPara;


/* 打印错误调试信息，以 %d, %s 的字符串作为参数 */
BOOL OutputErrorMessage(LPTSTR lpszMsg);


/* 动态提升进程权限 */
BOOL EnableProcessTokenPrivilege(LPTSTR lpPrivilegeName, BOOL bEnable);


/* 枚举所有窗口 */
BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam);


/* 刷新当前所有活动窗口所在进程的进程信息 */
VOID RefreshAllWindow();


/* 获得当前所有活动窗口所在进程的进程信息 */
DWORD GetAllWindowProcessInfo(ProcessInfo * processInfos, DWORD dwCount);


/* 保存位图 */
BOOL SaveBitmap(HBITMAP hBitmap, tstring strBmpName);


/* 从 PE 文件中解析出图标，返回 BITMAP 句柄 */
HBITMAP	GetBitmapFromPEFilePath(tstring strPEFilePath);


/* 从 PE 文件中解析出图标，返回 ICON 句柄 */
HICON GetIconFromPEFilePath(tstring strPEFilePath);


/* 往进程中注入 DLL */
BOOL InjectDLL2Process(DWORD dwPID, PTCHAR szDllName);


#endif	// end of __COMMON_API_H__