#define GEARINJECT_API

#include "GearInject.h"


BOOL g_bRunning = FALSE;
double g_dbSpeedMultiple = 1.0;

DWORD g_dwFirst_timeGetTime = 0;
DWORD g_dwFirst_GetTickCount = 0;
LARGE_INTEGER g_liFirst_QueryPerformanceCounter;



/************************************************************************/
/* ���ñ��ٳ��ֱ��ٱ��� 
/************************************************************************/
VOID SetSpeedMultiple(double dbSpeedMultiple)
{
	g_dbSpeedMultiple = dbSpeedMultiple;
}


/************************************************************************/
/* �������ٳ��ֽ��б���
/************************************************************************/
BOOL StartSpeedGear(double dbSpeedMultiple = 1.0)
{
	if(g_bRunning == FALSE)
	{
		g_dbSpeedMultiple = dbSpeedMultiple;

		g_dwFirst_timeGetTime = timeGetTime();
		g_dwFirst_GetTickCount = GetTickCount();
		QueryPerformanceCounter(&g_liFirst_QueryPerformanceCounter);


		InstallHook_SetTimer();
		InstallHook_Sleep();
		InstallHook_GetMessageTime();

		InstallHook_GetTickCount();
		InstallHook_QueryPerformanceCounter();

		InstallHook_timeSetEvent();
		InstallHook_timeGetTime();

		g_bRunning = TRUE;
	}

	return g_bRunning;
}


/************************************************************************/
/* ֹͣ���ٳ���
/************************************************************************/
VOID StopSpeedGear()
{
	if(g_bRunning == TRUE)
	{
		UnInstallHook_SetTimer();
		UnInstallHook_GetMessageTime();

		UnInstallHook_Sleep();
		UnInstallHook_GetTickCount();
		UnInstallHook_QueryPerformanceCounter();

		UnInstallHook_timeSetEvent();
		UnInstallHook_timeGetTime();

		g_dbSpeedMultiple = 1.0;

		g_bRunning = FALSE;
	}
}



/************************************************************************/
/* DLL ��ں���
/************************************************************************/
BOOL APIENTRY DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	switch (dwReason)
	{
	case DLL_PROCESS_ATTACH:
		{
			StartSpeedGear(1.0);

			break;
		}
	case DLL_PROCESS_DETACH:
		{
			StopSpeedGear();

			break;
		}
	case DLL_THREAD_ATTACH:
		{
			break;
		}
	case DLL_THREAD_DETACH:
		{
			break;
		}
	default:
		{
			break;
		}
	}

	return TRUE;
}




/************************************************************************/
/* SetTimer �� Hook ����
/************************************************************************/
UINT_PTR WINAPI Hooked_SetTimer(
	__in_opt HWND hWnd,
	__in UINT_PTR nIDEvent,
	__in UINT uElapse,
	__in_opt TIMERPROC lpTimerFunc
	)
{
	MNOutPutPrintT(TEXT("In Hooked_SetTimer."));

	UINT uMyElapse = (UINT)((double)uElapse / g_dbSpeedMultiple);

	return pOldAddr_SetTimer(hWnd, nIDEvent, uMyElapse, lpTimerFunc);
	//return pOldAddr_SetTimer(hWnd, nIDEvent, uElapse, lpTimerFunc);
}

/************************************************************************/
/* ��װ SetTimer Hook
/************************************************************************/
BOOL InstallHook_SetTimer()
{
	if(pOldAddr_SetTimer == NULL)
	{
		pOldAddr_SetTimer = (SETTIMER)GetProcAddress(GetModuleHandle(TEXT("User32.dll")), "SetTimer");
		if(pOldAddr_SetTimer != NULL)
		{
			MNOutPutPrintT(TEXT("In InstallHook_SetTimer."));

			return Mhook_SetHook((PVOID *)&pOldAddr_SetTimer, Hooked_SetTimer);
		}
		else
		{
			return FALSE;
		}
	}

	return TRUE;
}

/************************************************************************/
/* ж�� SetTimer Hook
/************************************************************************/
BOOL UnInstallHook_SetTimer()
{
	if(pOldAddr_SetTimer != NULL)
	{
		MNOutPutPrintT(TEXT("In UnInstallHook_SetTimer."));

		return Mhook_Unhook((PVOID *)&pOldAddr_SetTimer);
	}
	else
	{
		return TRUE;
	}
}




/************************************************************************/
/* GetMessageTime �� Hook ����
/************************************************************************/
LONG WINAPI Hooked_GetMessageTime(VOID)
{
	MNOutPutPrintT(TEXT("In Hooked_GetMessageTime."));

	return pOldAddr_GetMessageTime();
}

/************************************************************************/
/* ��װ GetMessageTime Hok
/************************************************************************/
BOOL InstallHook_GetMessageTime()
{
	if(pOldAddr_GetMessageTime == NULL)
	{
		pOldAddr_GetMessageTime = (GETMESSAGETIME)GetProcAddress(GetModuleHandle(TEXT("User32.dll")), "GetMessageTime");
		if(pOldAddr_GetMessageTime != NULL)
		{
			MNOutPutPrintT(TEXT("In InstallHook_GetMessageTime."));

			return Mhook_SetHook((PVOID *)&pOldAddr_GetMessageTime, Hooked_GetMessageTime);
		}
		else
		{
			return FALSE;
		}
	}

	return TRUE;
}

/************************************************************************/
/* ж�� GetMessageTime Hook
/************************************************************************/
BOOL UnInstallHook_GetMessageTime()
{
	if(pOldAddr_GetMessageTime != NULL)
	{
		MNOutPutPrintT(TEXT("In UnInstallHook_GetMessageTime."));

		return Mhook_Unhook((PVOID *)&pOldAddr_GetMessageTime);
	}
	else
	{
		return TRUE;
	}
}




/************************************************************************/
/* Sleep �� Hook ����
/************************************************************************/
VOID WINAPI Hooked_Sleep(
	__in DWORD dwMilliseconds
	)
{
	MNOutPutPrintT(TEXT("In Hooked_Sleep."));

	DWORD myInterval = (DWORD)((double)dwMilliseconds / g_dbSpeedMultiple);

	return pOldAddr_Sleep(myInterval);
	//return pOldAddr_Sleep(dwMilliseconds);
}

/************************************************************************/
/* ��װ Sleep Hook
/************************************************************************/
BOOL InstallHook_Sleep()
{
	if(pOldAddr_Sleep == NULL)
	{
		pOldAddr_Sleep = (SLEEP)GetProcAddress(GetModuleHandle(TEXT("Kernel32.dll")), "Sleep");
		if(pOldAddr_Sleep != NULL)
		{
			MNOutPutPrintT(TEXT("In InstallHook_Sleep."));

			return Mhook_SetHook((PVOID *)&pOldAddr_Sleep, Hooked_Sleep);
		}
		else
		{
			return FALSE;
		}
	}

	return TRUE;
}

/************************************************************************/
/* ж�� Sleep Hook
/************************************************************************/
BOOL UnInstallHook_Sleep()
{
	if(pOldAddr_Sleep != NULL)
	{
		MNOutPutPrintT(TEXT("In UnInstallHook_Sleep."));

		return Mhook_Unhook((PVOID *)&pOldAddr_Sleep);
	}
	else
	{
		return TRUE;
	}
}




/************************************************************************/
/* GetTickCount �� Hook ����
/************************************************************************/
DWORD WINAPI Hooked_GetTickCount(void)
{
	MNOutPutPrintT(TEXT("In Hooked_GetTickCount."));

	return DWORD(double(pOldAddr_GetTickCount() - g_dwFirst_GetTickCount) * g_dbSpeedMultiple);
	//return pOldAddr_GetTickCount();
}

/************************************************************************/
/* ��װ GetTickCount Hook
/************************************************************************/
BOOL InstallHook_GetTickCount()
{
	if(pOldAddr_GetTickCount == NULL)
	{
		pOldAddr_GetTickCount = (GETTICKCOUNT)GetProcAddress(GetModuleHandle(TEXT("Kernel32.dll")), "GetTickCount");
		if(pOldAddr_GetTickCount != NULL)
		{
			MNOutPutPrintT(TEXT("In InstallHook_GetTickCount."));

			return Mhook_SetHook((PVOID *)&pOldAddr_GetTickCount, Hooked_GetTickCount);
		}
		else
		{
			return FALSE;
		}
	}

	return TRUE;
}

/************************************************************************/
/* ж�� GetTickCount Hook
/************************************************************************/
BOOL UnInstallHook_GetTickCount()
{
	if(pOldAddr_GetTickCount != NULL)
	{
		MNOutPutPrintT(TEXT("In UnInstallHook_GetTickCount."));

		return Mhook_Unhook((PVOID *)&pOldAddr_GetTickCount);
	}
	else
	{
		return TRUE;
	}
}




/************************************************************************/
/* QueryPerformanceCounter �� Hook ����
/************************************************************************/
BOOL WINAPI Hooked_QueryPerformanceCounter(__out LARGE_INTEGER *lpPerformanceCount)
{
	MNOutPutPrintT(TEXT("In Hooked_QueryPerformanceCounter."));

	BOOL bSucceed = pOldAddr_QueryPerformanceCounter(lpPerformanceCount);
	if(bSucceed == TRUE)
	{
		lpPerformanceCount->QuadPart = LONGLONG(double(lpPerformanceCount->QuadPart - g_liFirst_QueryPerformanceCounter.QuadPart) * g_dbSpeedMultiple);
	}

	return bSucceed;
	//return pOldAddr_QueryPerformanceCounter(lpPerformanceCount);
}

/************************************************************************/
/* ��װ QueryPerformanceCounter Hook
/************************************************************************/
BOOL InstallHook_QueryPerformanceCounter()
{
	if(pOldAddr_QueryPerformanceCounter == NULL)
	{
		pOldAddr_QueryPerformanceCounter = (QUERYPERFORMANCECOUNTER)GetProcAddress(GetModuleHandle(TEXT("Kernel32.dll")), "QueryPerformanceCounter");
		if(pOldAddr_QueryPerformanceCounter != NULL)
		{
			MNOutPutPrintT(TEXT("In InstallHook_QueryPerformanceCounter."));

			return Mhook_SetHook((PVOID *)&pOldAddr_QueryPerformanceCounter, Hooked_QueryPerformanceCounter);
		}
		else
		{
			return FALSE;
		}
	}

	return TRUE;
}

/************************************************************************/
/* ж�� QueryPerformanceCounter Hook
/************************************************************************/
BOOL UnInstallHook_QueryPerformanceCounter()
{
	if(pOldAddr_QueryPerformanceCounter != NULL)
	{
		MNOutPutPrintT(TEXT("In UnInstallHook_QueryPerformanceCounter."));

		return Mhook_Unhook((PVOID *)&pOldAddr_QueryPerformanceCounter);
	}
	else
	{
		return TRUE;
	}
}





/************************************************************************/
/* timeSetEvent �� Hook ����
/************************************************************************/
MMRESULT Hooked_timeSetEvent(
	__in UINT uDelay, 
	__in UINT uResolution,
	__in LPTIMECALLBACK fptc, 
	__in DWORD_PTR dwUser,
	__in UINT fuEvent
	)
{
	MNOutPutPrintT(TEXT("In Hooked_timeSetEvent."));

	UINT uMyDelay = (UINT)((double)uDelay / g_dbSpeedMultiple);
	return pOldAddr_timeSetEvent(uMyDelay, uResolution, fptc, dwUser, fuEvent);
}

/************************************************************************/
/* ��װ timeSetEvent Hook
/************************************************************************/
BOOL InstallHook_timeSetEvent()
{
	if(pOldAddr_timeSetEvent == NULL)
	{
		pOldAddr_timeSetEvent = (TIMESETEVENT)GetProcAddress(GetModuleHandle(TEXT("Winmm.dll")), "timeSetEvent");
		if(pOldAddr_timeSetEvent != NULL)
		{
			MNOutPutPrintT(TEXT("In InstallHook_timeSetEvent."));

			return Mhook_SetHook((PVOID *)&pOldAddr_timeSetEvent, Hooked_timeSetEvent);
		}
		else
		{
			return FALSE;
		}
	}

	return TRUE;
}

/************************************************************************/
/* ж�� timeSetEvent Hook
/************************************************************************/
BOOL UnInstallHook_timeSetEvent()
{
	if(pOldAddr_timeSetEvent != NULL)
	{
		MNOutPutPrintT(TEXT("In UnInstallHook_timeSetEvent."));

		return Mhook_Unhook((PVOID *)&pOldAddr_timeSetEvent);
	}
	else
	{
		return TRUE;
	}
}




/************************************************************************/
/* timeGetTime �� Hook ����
/************************************************************************/
DWORD WINAPI Hooked_timeGetTime(void)
{
	MNOutPutPrintT(TEXT("In Hooked_timeGetTime."));

	return DWORD(double(pOldAddr_timeGetTime() - g_dwFirst_GetTickCount) * g_dbSpeedMultiple);
	//return pOldAddr_timeGetTime();
}

/************************************************************************/
/* ��װ timeGetTime Hook
/************************************************************************/
BOOL InstallHook_timeGetTime()
{
	if(pOldAddr_timeGetTime == NULL)
	{
		pOldAddr_timeGetTime = (TIMEGETTIME)GetProcAddress(GetModuleHandle(TEXT("Winmm.dll")), "timeGetTime");
		if(pOldAddr_timeGetTime != NULL)
		{
			MNOutPutPrintT(TEXT("In InstallHook_timeGetTime."));

			return Mhook_SetHook((PVOID *)&pOldAddr_timeGetTime, Hooked_timeGetTime);
		}
		else
		{
			return FALSE;
		}
	}

	return TRUE;
}

/************************************************************************/
/* ж�� timeGetTime Hook
/************************************************************************/
BOOL UnInstallHook_timeGetTime()
{
	if(pOldAddr_timeGetTime != NULL)
	{
		MNOutPutPrintT(TEXT("In UnInstallHook_timeGetTime."));

		return Mhook_Unhook((PVOID *)&pOldAddr_timeGetTime);
	}
	else
	{
		return TRUE;
	}
}
