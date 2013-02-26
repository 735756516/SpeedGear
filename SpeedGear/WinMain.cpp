#include "WinMain.h"

HWND g_hMainWnd = NULL;


/************************************************************************/
/* ������ڵ�
/************************************************************************/
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	/* ��ʵ������ */
	HANDLE hMutex = CreateMutex(NULL, FALSE, SPEEDGEAR_MUTEX);
	if(ERROR_ALREADY_EXISTS == GetLastError())
	{
		return FALSE;
	}

	return CreateMainWindow(hInstance);
}


/************************************************************************/
/* ����������
/************************************************************************/
int CreateMainWindow(HINSTANCE hInstance)
{
	// 1. ��ƴ�����
	WNDCLASS wndClass;
	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = 0;
	wndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);

	/* ���ô���ͼ��Ϊ��Դ�ļ��е�ͼ�� */
	//wndClass.hIcon = LoadIcon(NULL, IDI_ERROR);
	wndClass.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APPLICATION_ICON));
	wndClass.hInstance = hInstance;
	wndClass.lpfnWndProc = MainWndMsgProc;
	wndClass.lpszClassName = MainWndClassName;
	wndClass.lpszMenuName = MainWndAppName;
	wndClass.style = CS_HREDRAW | CS_VREDRAW;

	// 2. ע�ᴰ����
	RegisterClass(&wndClass);

	/* ��ȡ��ǰ��Ļ�Ŀ�Ⱥ͸߶� */
	int nWidth = GetSystemMetrics(SM_CXSCREEN);
	int nHeight = GetSystemMetrics(SM_CYSCREEN);

	// 3. �������� - ��Ļ������ʾ������󻯴��ڰ�ť��������ı䴰�ڴ�С
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

	// 4. ��ʾ����
	ShowWindow(g_hMainWnd, SW_SHOWNORMAL);

	// 5. ˢ�´���
	UpdateWindow(g_hMainWnd);

	// 6. ��Ϣѭ��
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
/* �����ڹ��̴�����
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
			/* Ϊ���������չ��� WS_EX_LAYERED���Ӷ�Ϊʵ�ִ���͸����׼�� */
			LONG lExStyle = GetWindowLong(hWnd, GWL_EXSTYLE);
			lExStyle = lExStyle | WS_EX_LAYERED;
			SetWindowLong(hWnd, GWL_EXSTYLE, lExStyle);

			/* ʵ�ִ���͸�� - ���ô���͸����Ϊ 220 */
			SetLayeredWindowAttributes(hWnd, 0, WND_TRANSPARENCY, LWA_ALPHA);


			InitSpeedMultipleComboBox(((LPCREATESTRUCT)lParam)->hInstance, hWnd);

			/* ��ʼ�� Tracker Bar */
			InitSpeedMultipleTrackBar(((LPCREATESTRUCT)lParam)->hInstance, hWnd);

			/* ��ʼ����ť */
			InitButton(((LPCREATESTRUCT)lParam)->hInstance, hWnd);


			/* ��ʼ���ٶ�δ�ı�Ľ����б�� */
			InitNotSpeedMultipleListView(((LPCREATESTRUCT)lParam)->hInstance, hWnd);

			/* ��ʼ���ٶ��Ѹı�Ľ����б�� */
			InitHasSpeedMultipleListView(((LPCREATESTRUCT)lParam)->hInstance, hWnd);

			/* �����ݵ��ٶ�δ�ı�Ľ����б�� */
			BindData2NotSpeedMultipleListView(((LPCREATESTRUCT)lParam)->hInstance);

			break;
		}
	case WM_CTLCOLORSTATIC:
		{
			/* ʹ�þ�̬�ı���ʹ��͸������ɫ */
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
