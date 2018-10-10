#pragma once
#include "TaskMgr_List.h"


typedef struct _MYWINDOWINFO
{
	TCHAR hWnd[MAX_PATH + 1];  //���ھ��
	TCHAR WindowName[MAX_PATH + 1];//���ڱ���
	TCHAR ClassName[MAX_PATH + 1];//��������
}MYWINDOWINFO, *PMYWINDOWINFO;


// WinDialog �Ի���
//�鿴�����б��Ӵ�����
class WinDialog : public CDialogEx
{
	DECLARE_DYNAMIC(WinDialog)

public:
	WinDialog(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~WinDialog();

	// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG4 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
	TaskMgr_List m_WindowList;//�����б�󶨿ؼ�

	void EnumWindow();		//��ʾ�����б�

	static BOOL CALLBACK EnumWindowProc(HWND hWnd, LPARAM lParam);			//ö�ٴ��ڻص�����
	afx_msg void OnRclickWindowList1(NMHDR *pNMHDR, LRESULT *pResult);		//�Ҽ������˵�
	afx_msg void OnPopMenu(UINT id);					//��Ӧ�Ҽ��˵�
};
