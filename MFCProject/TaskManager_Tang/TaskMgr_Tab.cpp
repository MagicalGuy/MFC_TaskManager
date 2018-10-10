// TaskMgr_Tab.cpp : ʵ���ļ�
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

// TaskMgr_Tab ��Ϣ�������


void TaskMgr_Tab::PreSubclassWindow()
{
	// TODO: �ڴ����ר�ô����/����û���

	CTabCtrl::PreSubclassWindow();
}


void TaskMgr_Tab::OnNMClick(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;
}

//ѡ��TAB���
void TaskMgr_Tab::OnTcnSelchange(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;
	//������ǰѡ�ѡ����;
	DWORD nSel = GetCurSel();
	//��������;
	for (size_t i = 0; i < m_dwCount; i++)
	{
		m_pWnd[i]->ShowWindow(SW_HIDE);
	}
	//�������;
	if (nSel >= m_dwCount)
	{
		return;
	}

	//�ƶ����ں�TAB�ͻ���һ����;
	CRect rt;
	GetClientRect(&rt);
	rt.DeflateRect(0, 25, 0, 0);		//TOPֵ��С;
	m_pWnd[nSel]->MoveWindow(&rt, 1);
	m_pWnd[nSel]->ShowWindow(SW_SHOW);
}

//����ѡ��
void TaskMgr_Tab::MyInserItem(DWORD dwCount, ...)
{
	//������;
	va_list va;
	va_start(va, dwCount);
	for (DWORD i = 0; i < dwCount; i++)
	{
		TCHAR *p = va_arg(va, TCHAR*);
		InsertItem(i, p);
	}
	va_end(va);
}


//�����ӶԻ���
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

	//�ƶ����ںͿͻ���һ����;
	CRect rt;
	GetClientRect(&rt);
	rt.DeflateRect(0, 25, 0, 0);
	m_pWnd[0]->MoveWindow(&rt, 1);
	m_pWnd[0]->ShowWindow(SW_SHOW);
}
