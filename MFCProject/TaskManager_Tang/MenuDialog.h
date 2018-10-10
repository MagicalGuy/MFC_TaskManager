#pragma once
#include "TaskMgr_List.h"


// MenuDialog �Ի���
//�߳�ģ�����Ϣ�б��Ӵ�����
class MenuDialog : public CDialogEx
{
	DECLARE_DYNAMIC(MenuDialog)

public:
	//MenuDialog(CWnd* pParent = NULL);   // ��׼���캯��
	MenuDialog(CWnd* pParent, int nMID, int nPID);   // ��׼���캯��);
	virtual ~MenuDialog();

	// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
	TaskMgr_List m_PopList;

	int m_nMenuID;	//�Ҽ��˵�ID;
	int m_nPID;		//����ID;
	bool m_bFlag;	//��ʾ�Ҽ��˵���־;

	void ShowModule(int nPid);	//ģ��;
	void ShowThread(int nPid);	//�߳�;
	void ShowHeap(int nPid);	//��;

	afx_msg void OnRclickPoplist1(NMHDR *pNMHDR, LRESULT *pResult);	//�Ҽ��˵�;
	afx_msg void OnMenuThread(UINT id);		//��Ӧ�Ҽ��˵�;


	static UINT  HeapThreadFunction(LPVOID lpParam);	//�鿴�ѵ��̻߳ص�����;

	HANDLE heapThread;//�鿴�ѵ��߳�

	static int getPid;//��̬��ȡ����ID
};
