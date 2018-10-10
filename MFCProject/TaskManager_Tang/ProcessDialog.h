#pragma once
#include "TaskMgr_list.h"
#include "MenuDialog.h"

// ProcessDialog �Ի���
//�߳��б��Ӵ�����
extern HWND hwnd;

class ProcessDialog : public CDialogEx
{
	DECLARE_DYNAMIC(ProcessDialog)

public:
	ProcessDialog(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~ProcessDialog();

	// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG2 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:

	void ShowProcess();		//��ʾ�����б�

	afx_msg void OnFresh();					//ˢ�½����б�
	afx_msg void OnClose();					//��������
	afx_msg void OnColumnclickProcessList1(NMHDR *pNMHDR, LRESULT *pResult);//�����������
	afx_msg void OnMenuClick(UINT id);									//��Ӧ�Ҽ��˵�
	afx_msg void OnRclickProcessList1(NMHDR *pNMHDR, LRESULT *pResult);//��������Ҽ��˵�

	TaskMgr_List m_List;  //�����б�ؼ�
	afx_msg void OnTimer(UINT_PTR nIDEvent);//��ʱ��ˢ�½����б�
};
