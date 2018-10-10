
// TaskManager_TangDlg.h : 头文件
//

#pragma once
#include "TaskMgr_Tab.h"
#include "afxwin.h"

//主窗口类
// CTaskManager_TangDlg 对话框
class CTaskManager_TangDlg : public CDialogEx
{
// 构造
public:
	CTaskManager_TangDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TASKMANAGER_TANG_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
//	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	POINT old;//旧坐标
	afx_msg void OnSize(UINT nType, int cx, int cy);//重画大小
	afx_msg void OnMenuTop(UINT id);//菜单选项响应
	void ReSize();//重新设置大小
	TaskMgr_Tab m_tab;//tab控件
	virtual BOOL PreTranslateMessage(MSG* pMsg);//响应注册的快捷键
	CStatic m_cpuText;//控件
	CStatic m_cpuNum;//cpu利用率控件变量
	CStatic m_memText;//控件
	CStatic m_memNum;//内存利用率控件变量


	static UINT  MyThreadFunction(LPVOID lpParam);	//获取cpu和内存的线程回调函数;

	HANDLE hThread;//线程
	bool m_bClose;//线程关闭标志

	afx_msg void OnClose();//关闭线程

	static double FILETIME2Double(const _FILETIME& fileTime);//FILETIME to Double
	static int GetCpuUsage();//获取cpu利用率

	afx_msg void OnSizing(UINT fwSide, LPRECT pRect);
};
