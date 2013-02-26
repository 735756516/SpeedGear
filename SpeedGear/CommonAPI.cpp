#include "CommonAPI.h"


/************************************************************************/
/* 定义存放进程信息的容器
/************************************************************************/
vector<ProcessInfo> vctProcessInfo;


/************************************************************************/
/* 打印错误调试信息，以 %d, %s 的字符串作为参数
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
/* 动态提升进程权限
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
/* 枚举所有窗口
/************************************************************************/
BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam)
{
	/* 是否为窗口句柄 || 窗口是否可视 || 窗口是否可激活 */
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
/* 刷新当前所有活动窗口所在进程的进程信息
/************************************************************************/
VOID RefreshAllWindow()
{
	vctProcessInfo.clear();

	/* 设置当前进程的 WindowStation\Desktop 为 WinSta0\Default */
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

	/* 设置进程的 WindowStation */
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

	/* 设置线程的 Desktop */
	SetThreadDesktop(hDeskUser);

	/* 枚举 WinSta0\Default 下的所有活动窗口 */
	if (!EnumWindows(EnumWindowsProc, NULL))
	{
#ifdef _DEBUG
		OutputErrorMessage(_TEXT("RefreshAllWindow - EnumWindows Failed , Error Code Is %d , Error Message Is %s !"));
#endif
	}

	/* 恢复进程的 Window Station 和 Desktop */
	SetThreadDesktop(hDeskSave);  
	SetProcessWindowStation(hStaSave);

	CloseDesktop(hDeskUser);  
	CloseWindowStation(hStaUser);
}


/************************************************************************/
/* 获得当前所有活动窗口所在进程的进程信息
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
/* 保存位图
/************************************************************************/
BOOL SaveBitmap(HBITMAP hBitmap, tstring strBmpName)       
{       
	HDC hDC;
	int nBits;					/* 当前分辨率下每象素所占字节数 */
	WORD wBitCount;				/* 位图中每象素所占字节数 */

	/* 计算位图文件每个像素所占字节数 */
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


	/* 位图属性结构 */
	BITMAP bmp;
	GetObject(hBitmap, sizeof(BITMAP), (LPSTR)&bmp);

	/* 位图信息头结构 */
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


	/* 定义调色板大小 */
	DWORD dwPaletteSize = 0;

	/* 位图中像素字节大小 */
	DWORD dwBmBitsSize = ((bmp.bmWidth * wBitCount + 31) / 32) * 4 * bmp.bmHeight;


	/* 位图内容分配内存 */
	HANDLE hDIB = GlobalAlloc(GHND, dwBmBitsSize + dwPaletteSize + sizeof(BITMAPINFOHEADER));

	/* 指向位图信息头结构 */
	LPBITMAPINFOHEADER lpBmpInfoHeader = (LPBITMAPINFOHEADER)GlobalLock(hDIB);
	*lpBmpInfoHeader = bmpInfoHeader;


	/* 处理调色板 */
	HANDLE hOldPalette = NULL;
	HANDLE hPalette = GetStockObject(DEFAULT_PALETTE);
	if (hPalette)
	{
		hDC = GetDC(NULL);
		hOldPalette = SelectPalette(hDC, (HPALETTE)hPalette, FALSE);
		RealizePalette(hDC);
	}

	/* 获取该调色板下新的像素值 */           
	GetDIBits(hDC, hBitmap, 0, (UINT)bmp.bmHeight, (LPSTR)lpBmpInfoHeader+ sizeof(BITMAPINFOHEADER) + dwPaletteSize, (BITMAPINFO *)lpBmpInfoHeader, DIB_RGB_COLORS);

	/* 恢复调色板 */
	if (hOldPalette)
	{
		SelectPalette(hDC, (HPALETTE)hOldPalette, TRUE);
		RealizePalette(hDC);
		ReleaseDC(NULL, hDC);
	}


	/* 创建位图文件 */
	HANDLE hFile = CreateFile(strBmpName.c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		return FALSE;
	}


	/* 位图文件头结构 */
	BITMAPFILEHEADER bmpFileHeader;
	/* 位图文件大小 */
	DWORD dwDIBSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + dwPaletteSize + dwBmBitsSize;
	bmpFileHeader.bfType = 0x4D42;
	bmpFileHeader.bfSize = dwDIBSize;
	bmpFileHeader.bfReserved1 = 0;
	bmpFileHeader.bfReserved2 = 0;
	bmpFileHeader.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER) + (DWORD)sizeof(BITMAPINFOHEADER) + dwPaletteSize;


	/* 写入文件字节数 */
	DWORD dwWritten = 0;

	/* 写入位图文件头 */
	WriteFile(hFile, (LPSTR)&bmpFileHeader, sizeof(BITMAPFILEHEADER), &dwWritten, NULL);
	/* 写入位图文件其余内容 */
	WriteFile(hFile, (LPSTR)lpBmpInfoHeader, dwDIBSize, &dwWritten, NULL);

	GlobalUnlock(hDIB);
	GlobalFree(hDIB);
	CloseHandle(hFile);

	return TRUE;
}


/************************************************************************/
/* 从 PE 文件中解析出图标，返回 BITMAP 句柄
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
/* 从 PE 文件中解析出图标，返回 ICON 句柄
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
///* 往进程中注入 DLL
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
//		/* 在 hProcess 所代表的进程内部分配虚拟内存来容纳我们将要创建的远程线程 */
//		PVOID pRemoteThread = VirtualAllocEx(hProcess, NULL, THREAD_SIZE, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
//		if(NULL == pRemoteThread)
//		{
//#ifdef _DEBUG
//			OutputErrorMessage(TEXT("InjectDLL2Process Failed , Error Code Is %d , Error Message Is %s !"));
//#else
//#endif
//
//			/* 关闭进程句柄 */
//			CloseHandle(hProcess);
//
//			bResult = FALSE;
//		}
//		else
//		{
//			/* 往 hProcess 进程中分配的虚拟内存里面写入数据，这里主要是将整个线程都写进去 */
//			if(WriteProcessMemory(hProcess, pRemoteThread, &RemoteThreadProc, THREAD_SIZE, 0) == FALSE)
//			{
//#ifdef _DEBUG
//				OutputErrorMessage(TEXT("InjectDLL2Process Failed , Error Code Is %d , Error Message Is %s !"));
//#else
//#endif
//
//				/* 释放 VirtualAllocEx 分配的内存 */
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
//					/* 释放 VirtualAllocEx 分配的内存 */
//					VirtualFreeEx(hProcess, pRemoteThread, 0, MEM_RELEASE);
//					CloseHandle(hProcess);
//
//					bResult = FALSE;
//				}
//				else
//				{
//					RemoteThreadPara remotePara = { 0 };
//
//					/* 将 Kernel32 API 的地址保存到 remotePara 中 */
//					remotePara.m_dwLoadLibraryAAddr = (DWORD)GetProcAddress(hKernel32, "LoadLibraryA");
//					remotePara.m_dwLoadLibraryWAddr = (DWORD)GetProcAddress(hKernel32, "LoadLibraryW");
//					remotePara.m_dwFreeLibraryAddr = (DWORD)GetProcAddress(hKernel32, "FreeLibrary");
//					remotePara.m_dwGetProcAddrAddr = (DWORD)GetProcAddress(hKernel32, "GetProcAddress");
//
//					/* 获取到当前目录路径 */
//					CHAR szCurrDir[MAX_PATH] = { 0 };
//					GetCurrentDirectoryA(MAX_PATH, szCurrDir);
//
//					string strDllPath = szCurrDir;
//					strDllPath += szDllName;
//
//					/* 将 DLL 的路径完整的复制到 remotePara 中 */
//					strcpy(remotePara.m_strDllPath, strDllPath.c_str());
//
//					/* 在宿主进程中分配虚拟内存来容纳远程线程所需要的参数 */
//					PVOID pRemotePara = VirtualAllocEx(hProcess, NULL, sizeof(RemoteThreadPara), MEM_COMMIT, PAGE_READWRITE);
//					if(NULL == pRemotePara)
//					{
//#ifdef _DEBUG
//						OutputErrorMessage(TEXT("InjectDLL2Process Failed , Error Code Is %d , Error Message Is %s !"));
//#else
//#endif
//						/* 释放 VirtualAllocEx 分配的内存 */
//						VirtualFreeEx(hProcess, pRemoteThread, 0, MEM_RELEASE);
//						CloseHandle(hProcess);
//
//						bResult = FALSE;
//					}
//					else
//					{
//						/* 将远程线程所携带的参数写入到宿主进程中所分配的虚拟内存 */
//						if(NULL == WriteProcessMemory(hProcess, pRemotePara, &remotePara, sizeof(RemotePara), 0))
//						{
//#ifdef _DEBUG
//							OutputErrorMessage(TEXT("InjectDLL2Process Failed , Error Code Is %d , Error Message Is %s !"));
//#else
//#endif
//							/* 释放 VirtualAllocEx 分配的内存 */
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
//							/* 开始创建远程线程 */
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
//								/* 等待远程线程结束 */
//								WaitForSingleObject(hThread, INFINITE);
//								CloseHandle(hThread);
//
//								bResult = TRUE;
//							}
//
//							/* 必须等到远程线程结束后才能释放宿主进程中所分配的内存，否则宿主进程会直接崩溃 */
//							/* 释放 VirtualAllocEx 分配的内存 */
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
