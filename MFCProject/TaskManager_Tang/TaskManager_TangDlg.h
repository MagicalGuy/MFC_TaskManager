
// TaskManager_TangDlg.h : ͷ�ļ�
//

#pragma once
#include "TaskMgr_Tab.h"
#include "afxwin.h"

//��������
// CTaskManager_TangDlg �Ի���
class CTaskManager_TangDlg : public CDialogEx
{
// ����
public:
	CTaskManager_TangDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TASKMANAGER_TANG_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
//	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	POINT old;//������
	afx_msg void OnSize(UINT nType, int cx, int cy);//�ػ���С
	afx_msg void OnMenuTop(UINT id);//�˵�ѡ����Ӧ
	void ReSize();//�������ô�С
	TaskMgr_Tab m_tab;//tab�ؼ�
	virtual BOOL PreTranslateMessage(MSG* pMsg);//��Ӧע��Ŀ�ݼ�
	CStatic m_cpuText;//�ؼ�
	CStatic m_cpuNum;//cpu�����ʿؼ�����
	CStatic m_memText;//�ؼ�
	CStatic m_memNum;//�ڴ������ʿؼ�����


	static UINT  MyThreadFunction(LPVOID lpParam);	//��ȡcpu���ڴ���̻߳ص�����;

	HANDLE hThread;//�߳�
	bool m_bClose;//�̹߳رձ�־

	afx_msg void OnClose();//�ر��߳�

	static double FILETIME2Double(const _FILETIME& fileTime);//FILETIME to Double
	static int GetCpuUsage();//��ȡcpu������

	afx_msg void OnSizing(UINT fwSide, LPRECT pRect);
};
