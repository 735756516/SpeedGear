#include "SpeedGear.h"


/************************************************************************/
/* ��ʼ����ť
/************************************************************************/
BOOL InitButton(HINSTANCE hInstance, HWND hWndParent)
{
	/* ���뵽�Ѽ����б�ť */
	hBtnInsertProcess = CreateWindow(TEXT("button"), TEXT("Insert"), 
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 
		ID_BUTTON_INSERT_PROCESS_X, ID_BUTTON_INSERT_PROCESS_Y, 
		ID_BUTTON_INSERT_PROCESS_WIDTH, ID_BUTTON_INSERT_PROCESS_HEIGHT, 
		hWndParent, (HMENU)ID_BUTTON_INSERT_PROCESS, hInstance, NULL);

	/* ���Ѽ����б��Ƴ���ť */
	hBtnRemoveProcess = CreateWindow(TEXT("button"), TEXT("Remove"), 
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 
		ID_BUTTON_REMOVE_PROCESS_X, ID_BUTTON_REMOVE_PROCESS_Y, 
		ID_BUTTON_REMOVE_PROCESS_WIDTH, ID_BUTTON_REMOVE_PROCESS_HEIGHT, 
		hWndParent, (HMENU)ID_BUTTON_REMOVE_PROCESS, hInstance, NULL);

	/* ˢ�°�ť */
	hBtnRefreshProcess = CreateWindow(TEXT("button"), TEXT("Refresh"), 
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 
		ID_BUTTON_REFRESH_PROCESS_X, ID_BUTTON_REFRESH_PROCESS_Y, 
		ID_BUTTON_REFRESH_PROCESS_WIDTH, ID_BUTTON_REFRESH_PROCESS_HEIGHT, 
		hWndParent, (HMENU)ID_BUTTON_REFRESH_PROCESS, hInstance, NULL);

	return TRUE;
}




/************************************************************************/
/* ��ʼ����Ͽ�
/************************************************************************/
BOOL InitSpeedMultipleComboBox(HINSTANCE hInstance, HWND hWndParent)
{
	hWndSpeedMultipleComboBox = CreateWindow(
		WC_COMBOBOX, NULL, 
		WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST,
		ID_SpeedMultiple_ComboBox_X, ID_SpeedMultiple_ComboBox_Y, 
		ID_SpeedMultiple_ComboBox_WIDTH, ID_SpeedMultiple_ComboBox_HEIGHT,
		hWndParent, (HMENU)ID_SpeedMultiple_ComboBox, hInstance, NULL); 

	for(int i = 0; i < 256; i++)
	{
		TCHAR szItem[16] = { 0 };
		_stprintf(szItem, TEXT("%d"), i);
		SendMessage(hWndSpeedMultipleComboBox, CB_ADDSTRING, 0, (LPARAM)szItem);
	}

	return TRUE;
}




/************************************************************************/
/* ��ʼ�� Tracker Bar
/************************************************************************/
BOOL InitSpeedMultipleTrackBar(HINSTANCE hInstance, HWND hWndParent)
{
	hWndSpeedMultipleTrackerBar = CreateWindow(TRACKBAR_CLASS, TEXT("Trackbar Control"), 
		WS_CHILD | WS_VISIBLE | TBS_AUTOTICKS | TBS_HORZ | TBS_FIXEDLENGTH | TBS_ENABLESELRANGE, 
		ID_SpeedMultipleTrackerBar_X, ID_SpeedMultipleTrackerBar_Y, 
		ID_SpeedMultipleTrackerBar_WIDTH, ID_SpeedMultipleTrackerBar_HEIGHT, 
		hWndParent, (HMENU)ID_SpeedMultipleTrackerBar, hInstance, NULL);


	/* ������Сֵ�����ֵ */
	SendMessage(hWndSpeedMultipleTrackerBar, TBM_SETRANGE, (WPARAM)TRUE, (LPARAM)MAKELONG(0, 10));

	/* ÿ���ƶ��Ĵ�С */
	SendMessage(hWndSpeedMultipleTrackerBar, TBM_SETPAGESIZE, 0, (LPARAM)1);

	//SendMessage(hWndSpeedMultipleTrackerBar, TBM_SETSEL, (WPARAM)FALSE, (LPARAM)MAKELONG(2, 4));

	SendMessage(hWndSpeedMultipleTrackerBar, TBM_SETPOS, (WPARAM)TRUE, (LPARAM)2);

	/* �滻�� Tracker Bar ��ԭ�д��ڹ��� */
	pOldSpeedMultipleTrackBarMsgProc = (WNDPROC)GetWindowLong(hWndSpeedMultipleTrackerBar, GWL_WNDPROC);
	SetWindowLong(hWndSpeedMultipleTrackerBar, GWL_WNDPROC, (LONG)SpeedMultipleTrackBarMsgProc);
	
	return TRUE;
}


/************************************************************************/
/* Tracker Bar �Ĵ��ڹ���
/************************************************************************/
LRESULT APIENTRY SpeedMultipleTrackBarMsgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;

	switch(uMsg)
	{
	case WM_PAINT:
		{
			//HDC hDC = BeginPaint(hWnd, &ps);

			//SetBkMode(hDC, TRANSPARENT) ;

			//EndPaint(hWnd, &ps);

			break;
			//double value;                         //a double
			//char *array = new char[50];           //a character pointer
			//value = SendMessage(hWndSpeedMultipleTrackerBar, TBM_GETPOS, 0, 0);   //get the pos of the
			////trackbar
			//gcvt(value, 50, array);               //store the double in the character
			////SetWindowText(edit_box, array);       //set the edit box to the char
			//delete [] array;                      //delete the character pointer
		}
	case WM_HSCROLL:
		{
			LRESULT res = 0;
			res = SendMessage(hWndSpeedMultipleTrackerBar, TBM_GETPOS, 0, 0);

			break;
		}
	default:
		{
			break;
		}
	}

	return CallWindowProc(pOldSpeedMultipleTrackBarMsgProc, hWnd, uMsg, wParam, lParam);
}




/************************************************************************/
/* ��ʼ���ٶ�δ�ı�Ľ����б��
/************************************************************************/
BOOL InitNotSpeedMultipleListView(HINSTANCE hInstance, HWND hWndParent)
{
	hWndNotSpeedMultipleListView = CreateWindowEx(NULL, WC_LISTVIEW, NULL, 
		LVS_REPORT | WS_CHILD | WS_VISIBLE | WS_BORDER, 
		ID_NotSpeedMultiple_ListView_X, ID_NotSpeedMultiple_ListView_Y, 
		ID_NotSpeedMultiple_ListView_WIDTH, ID_NotSpeedMultiple_ListView_HEIGHT, 
		hWndParent, (HMENU)ID_NotSpeedMultiple_ListView, hInstance, NULL);

	SendMessage(hWndNotSpeedMultipleListView, LVM_SETTEXTCOLOR, 0, (LPARAM)(RGB(0, 1, 255)));

	/* �� Icon */
	LV_COLUMN lvNotSpeedMultipleIconColumn;
	/* �� PID */
	LV_COLUMN lvNotSpeedMultiplePIDColumn;
	/* �� Name */
	LV_COLUMN lvNotSpeedMultipleNameColumn;
	/* �� Path */
	LV_COLUMN lvNotSpeedMultiplePathColumn;


	/* ���һ�У�������Ϊ 0 */
	lvNotSpeedMultipleIconColumn.cx = 20;
	lvNotSpeedMultipleIconColumn.iSubItem = 0;
	lvNotSpeedMultipleIconColumn.pszText = TEXT("");
	lvNotSpeedMultipleIconColumn.mask = LVCF_TEXT | LVCF_WIDTH | LVIF_IMAGE;
	ListView_InsertColumn(hWndNotSpeedMultipleListView, 0, &lvNotSpeedMultipleIconColumn);

	/* ���һ�У�������Ϊ 1 */
	lvNotSpeedMultiplePIDColumn.cx = 50;
	lvNotSpeedMultiplePIDColumn.iSubItem = 1;
	lvNotSpeedMultiplePIDColumn.pszText = TEXT("PID");
	lvNotSpeedMultiplePIDColumn.fmt = LVCFMT_RIGHT;
	lvNotSpeedMultiplePIDColumn.mask = LVCF_TEXT | LVCF_WIDTH;
	ListView_InsertColumn(hWndNotSpeedMultipleListView, 1, &lvNotSpeedMultiplePIDColumn);

	/* ���һ�У�������Ϊ 2 */
	lvNotSpeedMultipleNameColumn.mask = LVCF_TEXT | LVCF_WIDTH;
	lvNotSpeedMultipleNameColumn.pszText = TEXT("Name");
	lvNotSpeedMultipleNameColumn.iSubItem = 2;
	lvNotSpeedMultipleNameColumn.cx = 120;
	ListView_InsertColumn(hWndNotSpeedMultipleListView, 2, &lvNotSpeedMultipleNameColumn);

	/* ���һ�У�������Ϊ 3 */
	lvNotSpeedMultiplePathColumn.mask = LVCF_TEXT | LVCF_WIDTH;
	lvNotSpeedMultiplePathColumn.pszText = TEXT("Path");
	lvNotSpeedMultiplePathColumn.iSubItem = 3;
	lvNotSpeedMultiplePathColumn.cx = 560;
	ListView_InsertColumn(hWndNotSpeedMultipleListView, 3, &lvNotSpeedMultiplePathColumn);

	/* �����ʾ�����ߣ�����ѡȡ���Ƿ����� Cell ����ʾͼƬ���и�ѡ��(LVS_EX_CHECKBOXES)��ʾ�Ƿ�ѡ�� */
	ListView_SetExtendedListViewStyle(hWndNotSpeedMultipleListView, LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT | LVS_EX_SUBITEMIMAGES); 

	return TRUE;
}


/************************************************************************/
/* ��ʼ���ٶ��Ѹı�Ľ����б��
/************************************************************************/
BOOL InitHasSpeedMultipleListView(HINSTANCE hInstance, HWND hWndParent)
{
	hWndHasSpeedMultipleListView = CreateWindowEx(NULL, WC_LISTVIEW, NULL, 
		LVS_REPORT | WS_CHILD | WS_VISIBLE, 
		ID_HasSpeedMultiple_ListView_X, ID_HasSpeedMultiple_ListView_Y, 
		ID_HasSpeedMultiple_ListView_WIDTH, ID_HasSpeedMultiple_ListView_HEIGHT, 
		hWndParent, (HMENU)ID_HasSpeedMultiple_ListView, hInstance, NULL);

	SendMessage(hWndHasSpeedMultipleListView, LVM_SETTEXTCOLOR, 0, (LPARAM)(RGB(0, 1, 255)));

	/* �� Icon */
	LV_COLUMN lvHasSpeedMultipleIconColumn;
	/* �� PID */
	LV_COLUMN lvHasSpeedMultiplePIDColumn;
	/* �� Name */
	LV_COLUMN lvHasSpeedMultipleNameColumn;
	/* �� Path */
	LV_COLUMN lvHasSpeedMultiplePathColumn;


	/* ���һ�У�������Ϊ 0 */
	lvHasSpeedMultipleIconColumn.cx = 20;
	lvHasSpeedMultipleIconColumn.iSubItem = 0;
	lvHasSpeedMultipleIconColumn.pszText = TEXT("");
	lvHasSpeedMultipleIconColumn.mask = LVCF_TEXT | LVCF_WIDTH | LVIF_IMAGE;
	ListView_InsertColumn(hWndHasSpeedMultipleListView, 0, &lvHasSpeedMultipleIconColumn);

	/* ���һ�У�������Ϊ 1 */
	lvHasSpeedMultiplePIDColumn.cx = 50;
	lvHasSpeedMultiplePIDColumn.iSubItem = 1;
	lvHasSpeedMultiplePIDColumn.pszText = TEXT("PID");
	lvHasSpeedMultiplePIDColumn.fmt = LVCFMT_RIGHT;
	lvHasSpeedMultiplePIDColumn.mask = LVCF_TEXT | LVCF_WIDTH;
	ListView_InsertColumn(hWndHasSpeedMultipleListView, 1, &lvHasSpeedMultiplePIDColumn);

	/* ���һ�У�������Ϊ 2 */
	lvHasSpeedMultipleNameColumn.mask = LVCF_TEXT | LVCF_WIDTH;
	lvHasSpeedMultipleNameColumn.pszText = TEXT("Name");
	lvHasSpeedMultipleNameColumn.iSubItem = 2;
	lvHasSpeedMultipleNameColumn.cx = 120;
	ListView_InsertColumn(hWndHasSpeedMultipleListView, 2, &lvHasSpeedMultipleNameColumn);

	/* ���һ�У�������Ϊ 3 */
	lvHasSpeedMultiplePathColumn.mask = LVCF_TEXT | LVCF_WIDTH;
	lvHasSpeedMultiplePathColumn.pszText = TEXT("Path");
	lvHasSpeedMultiplePathColumn.iSubItem = 3;
	lvHasSpeedMultiplePathColumn.cx = 560;
	ListView_InsertColumn(hWndHasSpeedMultipleListView, 3, &lvHasSpeedMultiplePathColumn);

	/* �����ʾ�����ߣ�����ѡȡ���Ƿ����� Cell ����ʾͼƬ���и�ѡ��(LVS_EX_CHECKBOXES)��ʾ�Ƿ�ѡ�� */
	ListView_SetExtendedListViewStyle(hWndHasSpeedMultipleListView, LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT | LVS_EX_SUBITEMIMAGES); 

	return TRUE;
}


/************************************************************************/
/* �����ݵ��ٶ�δ�ı�Ľ��̶Ի���
/************************************************************************/
BOOL BindData2NotSpeedMultipleListView(HINSTANCE hInstance)
{
	ProcessInfo processInfos[MAX_PATH] = { 0 };
	DWORD dwCount = GetAllWindowProcessInfo(processInfos, MAX_PATH);

	/* ��ͼƬ�б�� ListView �ؼ���*/
	ImageList_RemoveAll(hNotSpeedMultipleIconList);
	hNotSpeedMultipleIconList = ImageList_Create(16, 16, ILC_COLOR32, 3, 1); 
	ListView_SetImageList(hWndNotSpeedMultipleListView, hNotSpeedMultipleIconList, LVSIL_SMALL); 

	/* ImageList ������Ϊ 0 ��ͼ��ΪĬ��ͼ�� */
	ImageList_AddIcon(hNotSpeedMultipleIconList, LoadIcon(hInstance, MAKEINTRESOURCE(IDI_DEFAULT_APP_ICON)));

	for(int i = 0; i < dwCount; i++)
	{
		/* ���һ�У��к�Ϊ i */
		LV_ITEM lvRowItem;
		lvRowItem.iItem = i;
		lvRowItem.iSubItem = 0;
		lvRowItem.pszText = NULL;
		lvRowItem.mask = LVIF_TEXT;
		lvRowItem.cchTextMax = MAX_PATH;
		ListView_InsertItem(hWndNotSpeedMultipleListView, &lvRowItem);


		/* ��ӵ�Ԫ�񵽵� i �е� 0 �� - ������ʾͼ�� */
		LV_ITEM lvIconCellItem;
		HICON hIcon = GetIconFromPEFilePath(processInfos[i].strProcessPath);
		if(hIcon != NULL)
		{
			ImageList_AddIcon(hNotSpeedMultipleIconList, hIcon);
			DestroyIcon(hIcon);
			lvIconCellItem.iImage = ImageList_GetImageCount(hNotSpeedMultipleIconList) - 1;
		}
		else
		{
			lvIconCellItem.iImage = 0;
		}
		lvIconCellItem.iItem = i;
		lvIconCellItem.iSubItem = 0;
		lvIconCellItem.mask = TVIF_IMAGE;
		lvIconCellItem.cchTextMax = MAX_PATH;
		ListView_SetItem(hWndNotSpeedMultipleListView, &lvIconCellItem); 


		/* �� DWORD ����ת��Ϊ TCHAR �ַ��� */
		TCHAR chProcessID[MAX_PATH] = { 0 };
		_stprintf(chProcessID, TEXT("%#05d"), processInfos[i].dwProcessID);

		/* ��ӵ�Ԫ�񵽵� i �е� 1 �� - ������ʾ PID */
		LV_ITEM lvPIDCellItem;
		lvPIDCellItem.iItem = i;
		lvPIDCellItem.iSubItem = 1;
		lvPIDCellItem.pszText = chProcessID;
		lvPIDCellItem.mask = TVIF_TEXT;
		lvPIDCellItem.cchTextMax = MAX_PATH;
		ListView_SetItem(hWndNotSpeedMultipleListView, &lvPIDCellItem); 

		/* ��ӵ�Ԫ�񵽵� i �е� 2 �� - ������ʾ�������� */
		LV_ITEM lvNameCellItem;
		lvNameCellItem.iItem = i;
		lvNameCellItem.iSubItem = 2;
		lvNameCellItem.pszText = (LPTSTR)processInfos[i].strProcessName.c_str();
		lvNameCellItem.mask = TVIF_TEXT;
		lvNameCellItem.cchTextMax = MAX_PATH;
		ListView_SetItem(hWndNotSpeedMultipleListView, &lvNameCellItem); 

		/* ��ӵ�Ԫ�񵽵� i �е� 3 �� - ������ʾ����·�� */
		LV_ITEM lvPathCellItem;
		lvPathCellItem.iItem = i;
		lvPathCellItem.iSubItem = 3;
		lvPathCellItem.pszText = (LPTSTR)processInfos[i].strProcessPath.c_str();
		lvPathCellItem.mask = TVIF_TEXT;
		lvPathCellItem.cchTextMax = MAX_PATH;
		ListView_SetItem(hWndNotSpeedMultipleListView, &lvPathCellItem); 
	}

	return TRUE;
}


/************************************************************************/
/* �����ݵ��ٶ��Ѹı�ĶԻ���
/************************************************************************/
BOOL BindData2HasSpeedMultipleListView(HINSTANCE hInstance)
{
	return TRUE;
}

