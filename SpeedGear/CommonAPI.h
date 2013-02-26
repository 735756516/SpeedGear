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



/* ���Ϊ Debug ģʽ����ӡ��Ϣ */
#ifdef _DEBUG
#define MNOUTPUTPRINTA_YES
#define MNOUTPUTPRINTW_YES
#endif


/* ��ӡ ANSI �ַ��� */
#ifdef MNOUTPUTPRINTA_YES
#define MNOutPutPrintA	OutputDebugStringA
#else
#define MNOutPutPrintA	//
#endif

/* ��ӡ UNICODE �ַ��� */
#ifdef MNOUTPUTPRINTW_YES
#define MNOutPutPrintW	OutputDebugStringW
#else
#define MNOutPutPrintW	//
#endif

/* ���� UNICODE �ַ����� ANSI �ַ��� */
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


/* ��ӡ���������Ϣ���� %d, %s ���ַ�����Ϊ���� */
BOOL OutputErrorMessage(LPTSTR lpszMsg);


/* ��̬��������Ȩ�� */
BOOL EnableProcessTokenPrivilege(LPTSTR lpPrivilegeName, BOOL bEnable);


/* ö�����д��� */
BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam);


/* ˢ�µ�ǰ���л�������ڽ��̵Ľ�����Ϣ */
VOID RefreshAllWindow();


/* ��õ�ǰ���л�������ڽ��̵Ľ�����Ϣ */
DWORD GetAllWindowProcessInfo(ProcessInfo * processInfos, DWORD dwCount);


/* ����λͼ */
BOOL SaveBitmap(HBITMAP hBitmap, tstring strBmpName);


/* �� PE �ļ��н�����ͼ�꣬���� BITMAP ��� */
HBITMAP	GetBitmapFromPEFilePath(tstring strPEFilePath);


/* �� PE �ļ��н�����ͼ�꣬���� ICON ��� */
HICON GetIconFromPEFilePath(tstring strPEFilePath);


/* ��������ע�� DLL */
BOOL InjectDLL2Process(DWORD dwPID, PTCHAR szDllName);


#endif	// end of __COMMON_API_H__