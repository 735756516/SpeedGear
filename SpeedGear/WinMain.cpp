#include "WinMain.h"

HWND g_hMainWnd = NULL;


/************************************************************************/
/* 程序入口点
/************************************************************************/
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	/* 单实例程序 */
	HANDLE hMutex = CreateMutex(NULL, FALSE, SPEEDGEAR_MUTEX);
	if(ERROR_ALREADY_EXISTS == GetLastError())
	{
		return FALSE;
	}

	return CreateMainWindow(hInstance);
}


/************************************************************************/
/* 创建主窗口
/************************************************************************/
int CreateMainWindow(HINSTANCE hInstance)
{
	// 1. 设计窗口类
	WNDCLASS wndClass;
	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = 0;
	wndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);

	/* 设置窗口图标为资源文件中的图标 */
	//wndClass.hIcon = LoadIcon(NULL, IDI_ERROR);
	wndClass.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APPLICATION_ICON));
	wndClass.hInstance = hInstance;
	wndClass.lpfnWndProc = MainWndMsgProc;
	wndClass.lpszClassName = MainWndClassName;
	wndClass.lpszMenuName = MainWndAppName;
	wndClass.style = CS_HREDRAW | CS_VREDRAW;

	// 2. 注册窗口类
	RegisterClass(&wndClass);

	/* 获取当前屏幕的宽度和高度 */
	int nWidth = GetSystemMetrics(SM_CXSCREEN);
	int nHeight = GetSystemMetrics(SM_CYSCREEN);

	// 3. 创建窗口 - 屏幕居中显示，无最大化窗口按钮，不允许改变窗口大小
	g_hMainWnd = CreateWindow(
		MainWndClassName, 
		MainWndWindowName, 
		WS_OVERLAPPEDWINDOW & (~WS_THICKFRAME) & (~WS_MAXIMIZEBOX), 
		(nWidth - MAINWND_WINDOW_WIDTH) / 2, 
		(nHeight - MAINWND_WINDOW_HEIGHT) / 2, 
		MAINWND_WINDOW_WIDTH, 
		MAINWND_WINDOW_HEIGHT, 
		NULL, 
		NULL, 
		hInstance, 
		NULL
		);

	// 4. 显示窗口
	ShowWindow(g_hMainWnd, SW_SHOWNORMAL);

	// 5. 刷新窗口
	UpdateWindow(g_hMainWnd);

	// 6. 消息循环
	MSG msg;
	BOOL bReturn;
	while((bReturn = GetMessage(&msg, g_hMainWnd, 0, 0)) != 0)
	{
		if(bReturn == -1)
		{
			break;;
		}
		else
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return msg.wParam;
}



/************************************************************************/
/* 主窗口过程处理函数
/************************************************************************/
LRESULT CALLBACK MainWndMsgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
	case WM_POWERBROADCAST:
		{
			SYSTEMTIME st;
			GetSystemTime(&st);

			switch(wParam)
			{
			/* Power status has changed. */
			case PBT_APMPOWERSTATUSCHANGE:
				{
					TCHAR szTime[MAX_PATH] = { 0 };
					_stprintf(szTime, TEXT("PBT_APMPOWERSTATUSCHANGE %d-%d-%d %d:%d:%d"), st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);

					MessageBox(hWnd, szTime, TEXT("1"), MB_OK);

					break;
				}
			/* Operation is resuming automatically from a low-power state. */
			/* This message is sent every time the system resumes. */
			case PBT_APMRESUMEAUTOMATIC:
				{
					TCHAR szTime[MAX_PATH] = { 0 };
					_stprintf(szTime, TEXT("PBT_APMRESUMEAUTOMATIC %d-%d-%d %d:%d:%d"), st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);

					MessageBox(hWnd, szTime, TEXT("2"), MB_OK);

					break;
				}
			/* Operation is resuming from a low-power state. */
			/* This message is sent after PBT_APMRESUMEAUTOMATIC if the resume is triggered by user input, such as pressing a key. */
			case PBT_APMRESUMESUSPEND:
				{
					TCHAR szTime[MAX_PATH] = { 0 };
					_stprintf(szTime, TEXT("PBT_APMRESUMESUSPEND %d-%d-%d %d:%d:%d"), st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);

					MessageBox(hWnd, szTime, TEXT("3"), MB_OK);

					break;
				}
			/* System is suspending operation. */
			case PBT_APMSUSPEND:
				{
					TCHAR szTime[MAX_PATH] = { 0 };
					_stprintf(szTime, TEXT("PBT_APMSUSPEND %d-%d-%d %d:%d:%d"), st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);

					MessageBox(hWnd, szTime, TEXT("4"), MB_OK);

					break;
				}
			/* A power setting change event has been received. */
			case PBT_POWERSETTINGCHANGE:
				{
					TCHAR szTime[MAX_PATH] = { 0 };
					_stprintf(szTime, TEXT("PBT_POWERSETTINGCHANGE %d-%d-%d %d:%d:%d"), st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);

					MessageBox(hWnd, szTime, TEXT("5"), MB_OK);

					break;
				}
			/* Battery power is low. In Windows Server 2008 and Windows Vista, use PBT_APMPOWERSTATUSCHANGE instead. */
			case PBT_APMBATTERYLOW:
				{
					TCHAR szTime[MAX_PATH] = { 0 };
					_stprintf(szTime, TEXT("PBT_APMBATTERYLOW %d-%d-%d %d:%d:%d"), st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);

					MessageBox(hWnd, szTime, TEXT("6"), MB_OK);

					break;
				}
			/* OEM-defined event occurred. 
			/* In Windows Server 2008 and Windows Vista, this event is not available because these operating systems support only ACPI; 
			/* APM BIOS events are not supported. */
			case PBT_APMOEMEVENT:
				{
					TCHAR szTime[MAX_PATH] = { 0 };
					_stprintf(szTime, TEXT("PBT_APMOEMEVENT %d-%d-%d %d:%d:%d"), st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);

					MessageBox(hWnd, szTime, TEXT("7"), MB_OK);

					break;
				}
			/* Request for permission to suspend. In Windows Server 2008 and Windows Vista, use the SetThreadExecutionState function instead. */
			case PBT_APMQUERYSUSPEND:
				{
					TCHAR szTime[MAX_PATH] = { 0 };
					_stprintf(szTime, TEXT("PBT_APMQUERYSUSPEND %d-%d-%d %d:%d:%d"), st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);

					MessageBox(hWnd, szTime, TEXT("8"), MB_OK);

					break;
				}
			/* Suspension request denied. In Windows Server 2008 and Windows Vista, use SetThreadExecutionState instead. */
			case PBT_APMQUERYSUSPENDFAILED:
				{
					TCHAR szTime[MAX_PATH] = { 0 };
					_stprintf(szTime, TEXT("PBT_APMQUERYSUSPENDFAILED %d-%d-%d %d:%d:%d"), st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);

					MessageBox(hWnd, szTime, TEXT("9"), MB_OK);

					break;
				}
			/* Operation resuming after critical suspension. In Windows Server 2008 and Windows Vista, use PBT_APMRESUMEAUTOMATIC instead. */
			case PBT_APMRESUMECRITICAL:
				{
					TCHAR szTime[MAX_PATH] = { 0 };
					_stprintf(szTime, TEXT("PBT_APMRESUMECRITICAL %d-%d-%d %d:%d:%d"), st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);

					MessageBox(hWnd, szTime, TEXT("10"), MB_OK);

					break;
				}
			default:
				{
					break;
				}
			}
			return 0;
			break;
		}
	case WM_COMMAND:
		{
			if (HIWORD(wParam) == BN_CLICKED) {
				//SendMessage(hWndSpeedMultipleComboBox, CB_SHOWDROPDOWN, (WPARAM) TRUE, 0);
			}

			if ( HIWORD(wParam) == CBN_SELCHANGE) {               
				LRESULT  sel = SendMessage(hWndSpeedMultipleComboBox, CB_GETCURSEL, 0, 0);
				//SetWindowText(hwndStatic, items[sel]);
				//SetFocus(hwnd);
			}

			break;
		}
	case WM_CREATE:
		{
			/* 为窗口添加扩展风格 WS_EX_LAYERED，从而为实现窗口透明做准备 */
			LONG lExStyle = GetWindowLong(hWnd, GWL_EXSTYLE);
			lExStyle = lExStyle | WS_EX_LAYERED;
			SetWindowLong(hWnd, GWL_EXSTYLE, lExStyle);

			/* 实现窗口透明 - 设置窗口透明度为 220 */
			SetLayeredWindowAttributes(hWnd, 0, WND_TRANSPARENCY, LWA_ALPHA);


			InitSpeedMultipleComboBox(((LPCREATESTRUCT)lParam)->hInstance, hWnd);

			/* 初始化 Tracker Bar */
			InitSpeedMultipleTrackBar(((LPCREATESTRUCT)lParam)->hInstance, hWnd);

			/* 初始化按钮 */
			InitButton(((LPCREATESTRUCT)lParam)->hInstance, hWnd);


			/* 初始化速度未改变的进程列表框 */
			InitNotSpeedMultipleListView(((LPCREATESTRUCT)lParam)->hInstance, hWnd);

			/* 初始化速度已改变的进程列表框 */
			InitHasSpeedMultipleListView(((LPCREATESTRUCT)lParam)->hInstance, hWnd);

			/* 绑定数据到速度未改变的进程列表框 */
			BindData2NotSpeedMultipleListView(((LPCREATESTRUCT)lParam)->hInstance);

			break;
		}
	case WM_CTLCOLORSTATIC:
		{
			/* 使得静态文本框使用透明背景色 */
			return (DWORD)GetStockObject(NULL_BRUSH);
		}
		/*case WM_COMMAND:
		{
		switch (wParam)
		{
		case ID_BUTTON_INSERT_PROCESS:
		{
		DestroyWindow(hWnd);

		break;
		}
		case ID_BUTTON_REMOVE_PROCESS:
		{
		break;
		}
		case ID_BUTTON_REFRESH_PROCESS:
		{
		break;
		}
		default:
		{
		break;
		}
		}
		break;
		}*/
	case WM_CLOSE:
		{
			DestroyWindow(hWnd);

			break;
		}
	case WM_DESTROY:
		{
			PostQuitMessage(0);

			break;
		}
	default:
		{
			break;
		}
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}
