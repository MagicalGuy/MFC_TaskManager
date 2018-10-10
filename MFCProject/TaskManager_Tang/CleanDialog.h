#pragma once
#include "afxshelltreectrl.h"
#include "afxwin.h"


// CleanDialog �Ի���
//����vs���̴�����
class CleanDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CleanDialog)

public:
	CleanDialog(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CleanDialog();

	// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG5 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CString m_DirName;//�༭�����
	afx_msg void OnDropFiles(HDROP hDropInfo);//��ק�ļ�
	afx_msg void OnBnClickedButton2();//����ɾ����ť
	virtual BOOL OnInitDialog();
	CMFCShellTreeCtrl m_shellTreeCtrl1;//���б�ؼ�

	bool DeleFile(CString fileName);//ɾ���ļ�
	void DeleDirectory(CString strDir);//ɾ�������ļ��м�������

	CEdit m_CleanEdit;//�༭��ؼ�
};
