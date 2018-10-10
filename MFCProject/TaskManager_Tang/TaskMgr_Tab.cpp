// TaskMgr_Tab.cpp : 实现文件
//

#include "stdafx.h"
#include "TaskManager_Tang.h"
#include "TaskMgr_Tab.h"

// TaskMgr_Tab

IMPLEMENT_DYNAMIC(TaskMgr_Tab, CTabCtrl)

TaskMgr_Tab::TaskMgr_Tab() {}

TaskMgr_Tab::~TaskMgr_Tab() {}

BEGIN_MESSAGE_MAP(TaskMgr_Tab, CTabCtrl)
	ON_NOTIFY_REFLECT(NM_CLICK, &TaskMgr_Tab::OnNMClick)
	ON_NOTIFY_REFLECT(TCN_SELCHANGE, &TaskMgr_Tab::OnTcnSelchange)

END_MESSAGE_MAP()

// TaskMgr_Tab 消息处理程序


void TaskMgr_Tab::PreSubclassWindow()
{
	// TODO: 在此添加专用代码和/或调用基类

	CTabCtrl::PreSubclassWindow();
}


void TaskMgr_Tab::OnNMClick(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
}

//选择TAB项触发
void TaskMgr_Tab::OnTcnSelchange(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
	//检索当前选项卡选择项;
	DWORD nSel = GetCurSel();
	//隐藏所有;
	for (size_t i = 0; i < m_dwCount; i++)
	{
		m_pWnd[i]->ShowWindow(SW_HIDE);
	}
	//出错结束;
	if (nSel >= m_dwCount)
	{
		return;
	}

	//移动窗口和TAB客户区一样大;
	CRect rt;
	GetClientRect(&rt);
	rt.DeflateRect(0, 25, 0, 0);		//TOP值减小;
	m_pWnd[nSel]->MoveWindow(&rt, 1);
	m_pWnd[nSel]->ShowWindow(SW_SHOW);
}

//插入选项
void TaskMgr_Tab::MyInserItem(DWORD dwCount, ...)
{
	//变参添加;
	va_list va;
	va_start(va, dwCount);
	for (DWORD i = 0; i < dwCount; i++)
	{
		TCHAR *p = va_arg(va, TCHAR*);
		InsertItem(i, p);
	}
	va_end(va);
}


//插入子对话框
void TaskMgr_Tab::MyInsertChild(DWORD dwCount, ...)
{
	m_dwCount = dwCount;
	va_list va;
	va_start(va, dwCount);
	for (DWORD i = 0; i < dwCount; i++)
	{
		m_pWnd[i] = va_arg(va, CDialogEx*);
		UINT uID = va_arg(va, UINT);
		m_pWnd[i]->Create(uID, this);
	}
	va_end(va);

	//移动窗口和客户区一样大;
	CRect rt;
	GetClientRect(&rt);
	rt.DeflateRect(0, 25, 0, 0);
	m_pWnd[0]->MoveWindow(&rt, 1);
	m_pWnd[0]->ShowWindow(SW_SHOW);
}
