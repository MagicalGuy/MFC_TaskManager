#pragma once
#include "TaskMgr_List.h"


typedef struct _MYWINDOWINFO
{
	TCHAR hWnd[MAX_PATH + 1];  //窗口句柄
	TCHAR WindowName[MAX_PATH + 1];//窗口标题
	TCHAR ClassName[MAX_PATH + 1];//窗口类名
}MYWINDOWINFO, *PMYWINDOWINFO;


// WinDialog 对话框
//查看窗口列表子窗口类
class WinDialog : public CDialogEx
{
	DECLARE_DYNAMIC(WinDialog)

public:
	WinDialog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~WinDialog();

	// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG4 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
	TaskMgr_List m_WindowList;//窗口列表绑定控件

	void EnumWindow();		//显示窗口列表

	static BOOL CALLBACK EnumWindowProc(HWND hWnd, LPARAM lParam);			//枚举窗口回调函数
	afx_msg void OnRclickWindowList1(NMHDR *pNMHDR, LRESULT *pResult);		//右键弹出菜单
	afx_msg void OnPopMenu(UINT id);					//响应右键菜单
};
