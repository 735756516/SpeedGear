#ifndef __GEAR_INJECT_H__
#define __GEAR_INJECT_H__

#include <Windows.h>
#include <MMSystem.h>
#include "mhook-lib/mhook.h"

#pragma comment(lib, "Winmm.lib")



#ifdef GEARINJECT_API
#define GearInject_API	extern "C" __declspec(dllexport)
#else
#define GearInject_API extern "C" __declspec(dllimport)
#endif	//end GEARINJECT_API



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




/* ���ٳ�����Ҫ�� Hook ���� API ��ʵ��
/************************************************************************
SetTimer
GetMessageTime

Sleep
GetTickCount
QueryPerformanceCounter

timeSetEvent
timeGetTime
************************************************************************/



/* ���ñ��ٳ��ֱ��ٱ��� */
GearInject_API VOID SetSpeedMultiple(double dbSpeedMultiple);

/* �������ٳ��ֽ��б��� */
GearInject_API BOOL StartSpeedGear(double dbSpeedMultiple);

/* ֹͣ���ٳ��� */
GearInject_API VOID StopSpeedGear();




/* Hook SetTimer */
typedef UINT_PTR (WINAPI *SETTIMER)(
	__in_opt HWND hWnd,
	__in UINT_PTR nIDEvent,
	__in UINT uElapse,
	__in_opt TIMERPROC lpTimerFunc
	);

SETTIMER pOldAddr_SetTimer = NULL;

UINT_PTR WINAPI Hooked_SetTimer(
	__in_opt HWND hWnd,
	__in UINT_PTR nIDEvent,
	__in UINT uElapse,
	__in_opt TIMERPROC lpTimerFunc
	);

BOOL InstallHook_SetTimer();

BOOL UnInstallHook_SetTimer();




/* Hook GetMessageTime */
typedef LONG (WINAPI *GETMESSAGETIME)(VOID);

GETMESSAGETIME pOldAddr_GetMessageTime = NULL;

LONG WINAPI Hooked_GetMessageTime(VOID);

BOOL InstallHook_GetMessageTime();

BOOL UnInstallHook_GetMessageTime();




/* Hook Sleep */
typedef VOID (WINAPI *SLEEP)(
	__in DWORD dwMilliseconds
	);

SLEEP pOldAddr_Sleep = NULL;

VOID WINAPI Hooked_Sleep(
	__in DWORD dwMilliseconds
	);

BOOL InstallHook_Sleep();

BOOL UnInstallHook_Sleep();




/* Hook GetTickCount */
typedef DWORD (WINAPI *GETTICKCOUNT)(void);

GETTICKCOUNT pOldAddr_GetTickCount = NULL;

DWORD WINAPI Hooked_GetTickCount(void);

BOOL InstallHook_GetTickCount();

BOOL UnInstallHook_GetTickCount();




/* Hook QueryPerformanceCounter */
typedef BOOL (WINAPI *QUERYPERFORMANCECOUNTER)(__out LARGE_INTEGER *lpPerformanceCount);

QUERYPERFORMANCECOUNTER pOldAddr_QueryPerformanceCounter = NULL;

BOOL WINAPI Hooked_QueryPerformanceCounter(__out LARGE_INTEGER *lpPerformanceCount);

BOOL InstallHook_QueryPerformanceCounter();

BOOL UnInstallHook_QueryPerformanceCounter();




/* Hook timeSetEvent */
typedef MMRESULT (WINAPI *TIMESETEVENT)(
	__in UINT uDelay, 
	__in UINT uResolution,
	__in LPTIMECALLBACK fptc, 
	__in DWORD_PTR dwUser,
	__in UINT fuEvent
	);

TIMESETEVENT pOldAddr_timeSetEvent = NULL;

MMRESULT Hooked_timeSetEvent(
	__in UINT uDelay, 
	__in UINT uResolution,
	__in LPTIMECALLBACK fptc, 
	__in DWORD_PTR dwUser,
	__in UINT fuEvent
	);

BOOL InstallHook_timeSetEvent();

BOOL UnInstallHook_timeSetEvent();




/* Hook timeGetTime */
typedef DWORD (WINAPI *TIMEGETTIME)(void);

TIMEGETTIME pOldAddr_timeGetTime = NULL;

DWORD WINAPI Hooked_timeGetTime(void);

BOOL InstallHook_timeGetTime();

BOOL UnInstallHook_timeGetTime();



#endif	// end of __GEAR_INJECT_H__