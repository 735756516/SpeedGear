#include "CommonAPI.h"


/************************************************************************/
/* �����Ž�����Ϣ������
/************************************************************************/
vector<ProcessInfo> vctProcessInfo;


/************************************************************************/
/* ��ӡ���������Ϣ���� %d, %s ���ַ�����Ϊ����
/************************************************************************/
BOOL OutputErrorMessage(LPTSTR lpszMsg)
{
	LPVOID lpszBufMsg;
	LPVOID lpszBufErrorMsg;
	DWORD dwError = GetLastError();

	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL, dwError, 
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&lpszBufErrorMsg, 0, NULL);

	lpszBufMsg = (LPVOID)LocalAlloc(LMEM_ZEROINIT, sizeof(TCHAR) * 256);
	StringCchPrintf((LPTSTR)lpszBufMsg, LocalSize(lpszBufMsg), lpszMsg, dwError, lpszBufErrorMsg);

	MNOutPutPrintT((LPTSTR)lpszBufMsg);

	LocalFree(lpszBufMsg);
	LocalFree(lpszBufErrorMsg);

	return FALSE;
}


/************************************************************************/
/* ��̬��������Ȩ��
/************************************************************************/
BOOL EnableProcessTokenPrivilege(LPTSTR lpPrivilegeName, BOOL bEnable)
{
	LUID luidTmp;
	HANDLE hToken;
	TOKEN_PRIVILEGES tkp;

	if(!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken))
	{	
		return FALSE;
	}

	if(!LookupPrivilegeValue(NULL, lpPrivilegeName, &luidTmp))
	{	
		CloseHandle(hToken);

		return FALSE;
	}

	tkp.PrivilegeCount = 1;
	tkp.Privileges[0].Luid = luidTmp;
	tkp.Privileges[0].Attributes = bEnable? SE_PRIVILEGE_ENABLED : 0;

	if(!AdjustTokenPrivileges(hToken, FALSE, &tkp, sizeof(tkp), NULL, NULL))
	{	
		CloseHandle(hToken);

		return FALSE;
	}

	CloseHandle(hToken);

	return TRUE;
}


/************************************************************************/
/* ö�����д���
/************************************************************************/
BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam)
{
	/* �Ƿ�Ϊ���ھ�� || �����Ƿ���� || �����Ƿ�ɼ��� */
	if (!hwnd || !IsWindowVisible(hwnd) || !IsWindowEnabled(hwnd))
	{
		return TRUE;
	}

	DWORD dwProcessId = 0;
	DWORD dwThreadId = GetWindowThreadProcessId(hwnd, &dwProcessId);
	HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, dwProcessId);
	if(NULL == hProcess)
	{
#ifdef _DEBUG
		OutputErrorMessage(_TEXT("EnumWindowsProc - OpenProcess Failed , Error Code Is %d , Error Message Is %s !"));
#endif
	}
	else
	{
		BOOL bExisted = FALSE;
		DWORD dwProcessID = GetProcessId(hProcess);

		for(int i = 0; i < vctProcessInfo.size(); i++)
		{
			if(vctProcessInfo.at(i).dwProcessID == dwProcessID)
			{
				bExisted = TRUE;
			}
		}

		if(bExisted == FALSE)
		{
			TCHAR chProcessExt[MAX_PATH] = { 0 };
			TCHAR chProcessName[MAX_PATH] = { 0 };
			TCHAR chProcessPath[MAX_PATH] = { 0 };
			TCHAR chProcessTitle[MAX_PATH] = { 0 };
			if(GetWindowText(hwnd, chProcessTitle, MAX_PATH) > 0)
			{
				if(GetModuleFileNameEx(hProcess, NULL, chProcessPath, MAX_PATH) > 0)
				{
					_tsplitpath(chProcessPath, NULL, NULL, chProcessName, chProcessExt);

					tstring strProcessTitle = chProcessTitle;
					tstring strProcessName = chProcessName;
					tstring strProcessPath = chProcessPath;

					ProcessInfo processInfo;
					processInfo.dwProcessID = dwProcessId;
					processInfo.strProcessName = strProcessName + chProcessExt;
					processInfo.strProcessPath = strProcessPath;
					processInfo.strProcessTitle = strProcessTitle;

					vctProcessInfo.push_back(processInfo);
				}
			}
		}

		CloseHandle(hProcess);
	}

	return TRUE;
}


/************************************************************************/
/* ˢ�µ�ǰ���л�������ڽ��̵Ľ�����Ϣ
/************************************************************************/
VOID RefreshAllWindow()
{
	vctProcessInfo.clear();

	/* ���õ�ǰ���̵� WindowStation\Desktop Ϊ WinSta0\Default */
	HWINSTA hStaSave = GetProcessWindowStation();
	LPCTSTR lpszStation = _TEXT("WinSta0");
	HWINSTA hStaUser = OpenWindowStation(lpszStation, 0, WINSTA_ALL_ACCESS);
	if (hStaUser == NULL)
	{
#ifdef _DEBUG
		OutputErrorMessage(_TEXT("RefreshAllWindow - OpenWindowStation Failed , Error Code Is %d , Error Message Is %s !"));
#endif

		return;
	}

	/* ���ý��̵� WindowStation */
	SetProcessWindowStation(hStaUser);

	HDESK hDeskSave = GetThreadDesktop(GetCurrentThreadId());
	LPCTSTR lpszDesk = _TEXT("default");
	HDESK hDeskUser = OpenDesktop(lpszDesk, 0, FALSE, DESKTOP_CREATEMENU | DESKTOP_CREATEWINDOW | DESKTOP_ENUMERATE | DESKTOP_HOOKCONTROL | DESKTOP_JOURNALPLAYBACK | DESKTOP_JOURNALRECORD | DESKTOP_READOBJECTS | DESKTOP_SWITCHDESKTOP | DESKTOP_WRITEOBJECTS);  
	if (hDeskUser == NULL)  
	{
#ifdef _DEBUG
		OutputErrorMessage(_TEXT("RefreshAllWindow - OpenDesktop Failed , Error Code Is %d , Error Message Is %s !"));
#endif

		CloseWindowStation(hStaUser);

		return;
	}

	/* �����̵߳� Desktop */
	SetThreadDesktop(hDeskUser);

	/* ö�� WinSta0\Default �µ����л���� */
	if (!EnumWindows(EnumWindowsProc, NULL))
	{
#ifdef _DEBUG
		OutputErrorMessage(_TEXT("RefreshAllWindow - EnumWindows Failed , Error Code Is %d , Error Message Is %s !"));
#endif
	}

	/* �ָ����̵� Window Station �� Desktop */
	SetThreadDesktop(hDeskSave);  
	SetProcessWindowStation(hStaSave);

	CloseDesktop(hDeskUser);  
	CloseWindowStation(hStaUser);
}


/************************************************************************/
/* ��õ�ǰ���л�������ڽ��̵Ľ�����Ϣ
/************************************************************************/
DWORD GetAllWindowProcessInfo(ProcessInfo * processInfos, DWORD dwCount)
{
	DWORD dwReturn = 0;

	RefreshAllWindow();

	if(NULL != processInfos)
	{
		for(int i = 0; i < vctProcessInfo.size() && i < dwCount; i++)
		{
			processInfos[i] = vctProcessInfo.at(i);
			dwReturn++;
		}
	}

	return dwReturn;
}


/************************************************************************/
/* ����λͼ
/************************************************************************/
BOOL SaveBitmap(HBITMAP hBitmap, tstring strBmpName)       
{       
	HDC hDC;
	int nBits;					/* ��ǰ�ֱ�����ÿ������ռ�ֽ��� */
	WORD wBitCount;				/* λͼ��ÿ������ռ�ֽ��� */

	/* ����λͼ�ļ�ÿ��������ռ�ֽ��� */
	hDC = CreateDC(TEXT("DISPLAY"), NULL, NULL, NULL);
	nBits = GetDeviceCaps(hDC, BITSPIXEL) * GetDeviceCaps(hDC, PLANES);
	DeleteDC(hDC);

	if(nBits <= 1)
	{
		wBitCount = 1;
	}
	else if(nBits <= 4)
	{
		wBitCount = 4;
	}
	else if(nBits <= 8)
	{
		wBitCount = 8;
	}
	else
	{
		wBitCount = 24;
	}


	/* λͼ���Խṹ */
	BITMAP bmp;
	GetObject(hBitmap, sizeof(BITMAP), (LPSTR)&bmp);

	/* λͼ��Ϣͷ�ṹ */
	BITMAPINFOHEADER bmpInfoHeader;
	bmpInfoHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmpInfoHeader.biWidth = bmp.bmWidth;
	bmpInfoHeader.biHeight = bmp.bmHeight;
	bmpInfoHeader.biPlanes = 1;
	bmpInfoHeader.biBitCount = wBitCount;
	bmpInfoHeader.biCompression = BI_RGB;
	bmpInfoHeader.biSizeImage = 0;
	bmpInfoHeader.biXPelsPerMeter = 0;
	bmpInfoHeader.biYPelsPerMeter = 0;
	bmpInfoHeader.biClrImportant = 0;
	bmpInfoHeader.biClrUsed = 0;


	/* �����ɫ���С */
	DWORD dwPaletteSize = 0;

	/* λͼ�������ֽڴ�С */
	DWORD dwBmBitsSize = ((bmp.bmWidth * wBitCount + 31) / 32) * 4 * bmp.bmHeight;


	/* λͼ���ݷ����ڴ� */
	HANDLE hDIB = GlobalAlloc(GHND, dwBmBitsSize + dwPaletteSize + sizeof(BITMAPINFOHEADER));

	/* ָ��λͼ��Ϣͷ�ṹ */
	LPBITMAPINFOHEADER lpBmpInfoHeader = (LPBITMAPINFOHEADER)GlobalLock(hDIB);
	*lpBmpInfoHeader = bmpInfoHeader;


	/* �����ɫ�� */
	HANDLE hOldPalette = NULL;
	HANDLE hPalette = GetStockObject(DEFAULT_PALETTE);
	if (hPalette)
	{
		hDC = GetDC(NULL);
		hOldPalette = SelectPalette(hDC, (HPALETTE)hPalette, FALSE);
		RealizePalette(hDC);
	}

	/* ��ȡ�õ�ɫ�����µ�����ֵ */           
	GetDIBits(hDC, hBitmap, 0, (UINT)bmp.bmHeight, (LPSTR)lpBmpInfoHeader+ sizeof(BITMAPINFOHEADER) + dwPaletteSize, (BITMAPINFO *)lpBmpInfoHeader, DIB_RGB_COLORS);

	/* �ָ���ɫ�� */
	if (hOldPalette)
	{
		SelectPalette(hDC, (HPALETTE)hOldPalette, TRUE);
		RealizePalette(hDC);
		ReleaseDC(NULL, hDC);
	}


	/* ����λͼ�ļ� */
	HANDLE hFile = CreateFile(strBmpName.c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		return FALSE;
	}


	/* λͼ�ļ�ͷ�ṹ */
	BITMAPFILEHEADER bmpFileHeader;
	/* λͼ�ļ���С */
	DWORD dwDIBSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + dwPaletteSize + dwBmBitsSize;
	bmpFileHeader.bfType = 0x4D42;
	bmpFileHeader.bfSize = dwDIBSize;
	bmpFileHeader.bfReserved1 = 0;
	bmpFileHeader.bfReserved2 = 0;
	bmpFileHeader.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER) + (DWORD)sizeof(BITMAPINFOHEADER) + dwPaletteSize;


	/* д���ļ��ֽ��� */
	DWORD dwWritten = 0;

	/* д��λͼ�ļ�ͷ */
	WriteFile(hFile, (LPSTR)&bmpFileHeader, sizeof(BITMAPFILEHEADER), &dwWritten, NULL);
	/* д��λͼ�ļ��������� */
	WriteFile(hFile, (LPSTR)lpBmpInfoHeader, dwDIBSize, &dwWritten, NULL);

	GlobalUnlock(hDIB);
	GlobalFree(hDIB);
	CloseHandle(hFile);

	return TRUE;
}


/************************************************************************/
/* �� PE �ļ��н�����ͼ�꣬���� BITMAP ���
/************************************************************************/
HBITMAP	GetBitmapFromPEFilePath(tstring strPEFilePath)
{
	HBITMAP hPEBmp = NULL;

	HICON hPEIcon = NULL;
	HICON hLargeIcon = NULL;
	HICON hSmallIcon = NULL;

	ExtractIconEx(strPEFilePath.c_str(), 0, &hLargeIcon, &hSmallIcon, 1);

	if(hLargeIcon != NULL)
	{
		hPEIcon = hLargeIcon;
	}
	else if(hSmallIcon != NULL)
	{
		hPEIcon = hSmallIcon;
	}
	else
	{
		hPEIcon = NULL;
	}

	if(hPEIcon != NULL)
	{
		ICONINFO iconInfo = { 0 };

		GetIconInfo(hLargeIcon, &iconInfo);
		hPEBmp = iconInfo.hbmColor;
	}

	if(hLargeIcon != NULL)
	{
		DestroyIcon(hLargeIcon);
	}
	if(hSmallIcon != NULL)
	{
		DestroyIcon(hSmallIcon);
	}

	return hPEBmp;
}


/************************************************************************/
/* �� PE �ļ��н�����ͼ�꣬���� ICON ���
/************************************************************************/
HICON GetIconFromPEFilePath(tstring strPEFilePath)
{
	HICON hPEIcon = NULL;
	HICON hLargeIcon = NULL;
	HICON hSmallIcon = NULL;

	ExtractIconEx(strPEFilePath.c_str(), 0, &hLargeIcon, &hSmallIcon, 1);

	if(hLargeIcon != NULL)
	{
		hPEIcon = hLargeIcon;

		if(hSmallIcon != NULL)
		{
			DestroyIcon(hSmallIcon);
		}
	}
	else if(hSmallIcon != NULL)
	{
		hPEIcon = hSmallIcon;

		if(hLargeIcon != NULL)
		{
			DestroyIcon(hLargeIcon);
		}
	}
	else
	{
		hPEIcon = NULL;
	}

	return hPEIcon;
}


///************************************************************************/
///* ��������ע�� DLL
///************************************************************************/
//BOOL InjectDLL2Process(DWORD dwPID, PCHAR szDllName)
//{
//	EnableProcessTokenPrivilege(SE_DEBUG_NAME, TRUE);
//
//	BOOL bResult = FALSE;
//	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPID);
//	if(NULL == hProcess)
//	{
//#ifdef _DEBUG
//		OutputErrorMessage(TEXT("InjectDLL2Process Failed , Error Code Is %d , Error Message Is %s !"));
//#else
//#endif
//
//		bResult = FALSE;
//	}
//	else
//	{
//		const DWORD THREAD_SIZE = 1024 * 4;
//
//		/* �� hProcess ������Ľ����ڲ����������ڴ����������ǽ�Ҫ������Զ���߳� */
//		PVOID pRemoteThread = VirtualAllocEx(hProcess, NULL, THREAD_SIZE, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
//		if(NULL == pRemoteThread)
//		{
//#ifdef _DEBUG
//			OutputErrorMessage(TEXT("InjectDLL2Process Failed , Error Code Is %d , Error Message Is %s !"));
//#else
//#endif
//
//			/* �رս��̾�� */
//			CloseHandle(hProcess);
//
//			bResult = FALSE;
//		}
//		else
//		{
//			/* �� hProcess �����з���������ڴ�����д�����ݣ�������Ҫ�ǽ������̶߳�д��ȥ */
//			if(WriteProcessMemory(hProcess, pRemoteThread, &RemoteThreadProc, THREAD_SIZE, 0) == FALSE)
//			{
//#ifdef _DEBUG
//				OutputErrorMessage(TEXT("InjectDLL2Process Failed , Error Code Is %d , Error Message Is %s !"));
//#else
//#endif
//
//				/* �ͷ� VirtualAllocEx ������ڴ� */
//				VirtualFreeEx(hProcess, pRemoteThread, 0, MEM_RELEASE);
//				CloseHandle(hProcess);
//
//				bResult = FALSE;
//			}
//			else
//			{
//				HMODULE hKernel32 = GetModuleHandle(TEXT("Kernel32.dll"));
//				if(NULL == hKernel32)
//				{
//#ifdef _DEBUG
//					OutputErrorMessage(TEXT("InjectDLL2Process Failed , Error Code Is %d , Error Message Is %s !"));
//#else
//#endif
//
//					/* �ͷ� VirtualAllocEx ������ڴ� */
//					VirtualFreeEx(hProcess, pRemoteThread, 0, MEM_RELEASE);
//					CloseHandle(hProcess);
//
//					bResult = FALSE;
//				}
//				else
//				{
//					RemoteThreadPara remotePara = { 0 };
//
//					/* �� Kernel32 API �ĵ�ַ���浽 remotePara �� */
//					remotePara.m_dwLoadLibraryAAddr = (DWORD)GetProcAddress(hKernel32, "LoadLibraryA");
//					remotePara.m_dwLoadLibraryWAddr = (DWORD)GetProcAddress(hKernel32, "LoadLibraryW");
//					remotePara.m_dwFreeLibraryAddr = (DWORD)GetProcAddress(hKernel32, "FreeLibrary");
//					remotePara.m_dwGetProcAddrAddr = (DWORD)GetProcAddress(hKernel32, "GetProcAddress");
//
//					/* ��ȡ����ǰĿ¼·�� */
//					CHAR szCurrDir[MAX_PATH] = { 0 };
//					GetCurrentDirectoryA(MAX_PATH, szCurrDir);
//
//					string strDllPath = szCurrDir;
//					strDllPath += szDllName;
//
//					/* �� DLL ��·�������ĸ��Ƶ� remotePara �� */
//					strcpy(remotePara.m_strDllPath, strDllPath.c_str());
//
//					/* �����������з��������ڴ�������Զ���߳�����Ҫ�Ĳ��� */
//					PVOID pRemotePara = VirtualAllocEx(hProcess, NULL, sizeof(RemoteThreadPara), MEM_COMMIT, PAGE_READWRITE);
//					if(NULL == pRemotePara)
//					{
//#ifdef _DEBUG
//						OutputErrorMessage(TEXT("InjectDLL2Process Failed , Error Code Is %d , Error Message Is %s !"));
//#else
//#endif
//						/* �ͷ� VirtualAllocEx ������ڴ� */
//						VirtualFreeEx(hProcess, pRemoteThread, 0, MEM_RELEASE);
//						CloseHandle(hProcess);
//
//						bResult = FALSE;
//					}
//					else
//					{
//						/* ��Զ���߳���Я���Ĳ���д�뵽����������������������ڴ� */
//						if(NULL == WriteProcessMemory(hProcess, pRemotePara, &remotePara, sizeof(RemotePara), 0))
//						{
//#ifdef _DEBUG
//							OutputErrorMessage(TEXT("InjectDLL2Process Failed , Error Code Is %d , Error Message Is %s !"));
//#else
//#endif
//							/* �ͷ� VirtualAllocEx ������ڴ� */
//							VirtualFreeEx(hProcess, pRemoteThread, 0, MEM_RELEASE);
//							VirtualFreeEx(hProcess, pRemotePara, 0, MEM_RELEASE);
//							CloseHandle(hProcess);
//
//							bResult = FALSE;
//						}
//						else
//						{
//							HANDLE hThread;
//							DWORD dwThreadId;
//
//							hThread = NULL;
//							dwThreadId = 0;
//
//							/* ��ʼ����Զ���߳� */
//							hThread = CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)pRemoteThread, pRemotePara, 0, &dwThreadId);
//							if(NULL == hThread)
//							{
//#ifdef _DEBUG
//								OutputErrorMessage(TEXT("InjectDLL2Process Failed , Error Code Is %d , Error Message Is %s !"));
//#else
//#endif
//								bResult = FALSE;
//							}
//							else
//							{
//								/* �ȴ�Զ���߳̽��� */
//								WaitForSingleObject(hThread, INFINITE);
//								CloseHandle(hThread);
//
//								bResult = TRUE;
//							}
//
//							/* ����ȵ�Զ���߳̽���������ͷ�������������������ڴ棬�����������̻�ֱ�ӱ��� */
//							/* �ͷ� VirtualAllocEx ������ڴ� */
//							VirtualFreeEx(hProcess, pRemoteThread, 0, MEM_RELEASE);
//							VirtualFreeEx(hProcess, pRemotePara, 0, MEM_RELEASE);
//
//							CloseHandle(hProcess);
//						}
//					}
//				}
//			}
//		}
//	}
//
//	EnableProcessTokenPrivilege(SE_DEBUG_NAME, FALSE);
//
//	return TRUE;
//}
