#ifndef __SPEED_GEAR_H__
#define __SPEED_GEAR_H__


#include <Windows.h>
#include <CommCtrl.h>

#pragma comment(lib, "Comctl32.lib")

#include "CommonAPI.h"
#include "resource.h"


#define INJECT_DLLNAME								TEXT("GearInject.dll")



static HWND hBtnInsertProcess;
static HWND hBtnRemoveProcess;
static HWND hBtnRefreshProcess;

#define ID_BUTTON_INSERT_PROCESS					0x1001
#define ID_BUTTON_REMOVE_PROCESS					0x1002
#define ID_BUTTON_REFRESH_PROCESS					0x1003

#define ID_BUTTON_INSERT_PROCESS_X					105
#define ID_BUTTON_INSERT_PROCESS_Y					250
#define ID_BUTTON_INSERT_PROCESS_WIDTH				150
#define ID_BUTTON_INSERT_PROCESS_HEIGHT				48

#define ID_BUTTON_REMOVE_PROCESS_X					305
#define ID_BUTTON_REMOVE_PROCESS_Y					250
#define ID_BUTTON_REMOVE_PROCESS_WIDTH				150
#define ID_BUTTON_REMOVE_PROCESS_HEIGHT				48

#define ID_BUTTON_REFRESH_PROCESS_X					505
#define ID_BUTTON_REFRESH_PROCESS_Y					250
#define ID_BUTTON_REFRESH_PROCESS_WIDTH				150
#define ID_BUTTON_REFRESH_PROCESS_HEIGHT			48




static HWND hWndSpeedMultipleTrackerBar = NULL;
static WNDPROC pOldSpeedMultipleTrackBarMsgProc = NULL; 

#define ID_SpeedMultipleTrackerBar					0x1004
#define ID_SpeedMultipleTrackerBar_X				10
#define ID_SpeedMultipleTrackerBar_Y				10
#define ID_SpeedMultipleTrackerBar_WIDTH			200
#define ID_SpeedMultipleTrackerBar_HEIGHT			30




static HWND hWndSpeedMultipleComboBox = NULL;

#define ID_SpeedMultiple_ComboBox					0x1005

#define ID_SpeedMultiple_ComboBox_X					300
#define ID_SpeedMultiple_ComboBox_Y					10
#define ID_SpeedMultiple_ComboBox_WIDTH				200
#define ID_SpeedMultiple_ComboBox_HEIGHT			200




static HWND hWndNotSpeedMultipleListView = NULL;
static HWND hWndHasSpeedMultipleListView = NULL;

static HIMAGELIST hNotSpeedMultipleIconList = NULL;

#define ID_NotSpeedMultiple_ListView				0x1006

#define ID_NotSpeedMultiple_ListView_X				2
#define ID_NotSpeedMultiple_ListView_Y				78
#define ID_NotSpeedMultiple_ListView_WIDTH			750
#define ID_NotSpeedMultiple_ListView_HEIGHT			170


#define ID_HasSpeedMultiple_ListView				0x1007

#define ID_HasSpeedMultiple_ListView_X				2
#define ID_HasSpeedMultiple_ListView_Y				300
#define ID_HasSpeedMultiple_ListView_WIDTH			750
#define ID_HasSpeedMultiple_ListView_HEIGHT			170



/* ��ʼ����ť */
BOOL InitButton(HINSTANCE hInstance, HWND hWndParent);




/* ��ʼ����Ͽ� */
BOOL InitSpeedMultipleComboBox(HINSTANCE hInstance, HWND hWndParent);




/* ��ʼ�� Tracker Bar */
BOOL InitSpeedMultipleTrackBar(HINSTANCE hInstance, HWND hWndParent);

/* Tracker Bar �Ĵ��ڹ��� */
LRESULT APIENTRY SpeedMultipleTrackBarMsgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);




/* ��ʼ���ٶ�δ�ı�Ľ����б�� */
BOOL InitNotSpeedMultipleListView(HINSTANCE hInstance, HWND hWndParent);

/* ��ʼ���ٶ��Ѹı�Ľ����б�� */
BOOL InitHasSpeedMultipleListView(HINSTANCE hInstance, HWND hWndParent);

/* �����ݵ��ٶ�δ�ı�Ľ��̶Ի��� */
BOOL BindData2NotSpeedMultipleListView(HINSTANCE hInstance);

/* �����ݵ��ٶ��Ѹı�ĶԻ��� */
BOOL BindData2HasSpeedMultipleListView(HINSTANCE hInstance);



#endif	// end of __SPEED_GEAR_H__