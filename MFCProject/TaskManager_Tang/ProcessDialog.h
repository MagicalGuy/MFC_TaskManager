#pragma once
#include "TaskMgr_list.h"
#include "MenuDialog.h"

// ProcessDialog 对话框
//线程列表子窗口类
extern HWND hwnd;

class ProcessDialog : public CDialogEx
{
	DECLARE_DYNAMIC(ProcessDialog)

public:
	ProcessDialog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~ProcessDialog();

	// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG2 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:

	void ShowProcess();		//显示进程列表

	afx_msg void OnFresh();					//刷新进程列表
	afx_msg void OnClose();					//结束进程
	afx_msg void OnColumnclickProcessList1(NMHDR *pNMHDR, LRESULT *pResult);//点击列名排序
	afx_msg void OnMenuClick(UINT id);									//响应右键菜单
	afx_msg void OnRclickProcessList1(NMHDR *pNMHDR, LRESULT *pResult);//弹出鼠标右键菜单

	TaskMgr_List m_List;  //进程列表控件
	afx_msg void OnTimer(UINT_PTR nIDEvent);//定时器刷新进程列表
};
