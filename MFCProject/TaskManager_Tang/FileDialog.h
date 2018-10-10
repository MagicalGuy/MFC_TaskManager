#pragma once
#include "TaskMgr_List.h"
#include "afxshelltreectrl.h"


// FileDialog �Ի���
//�ļ��鿴�Ӵ�����
class FileDialog : public CDialogEx
{
	DECLARE_DYNAMIC(FileDialog)

public:
	FileDialog(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~FileDialog();

	// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG3 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	TaskMgr_List m_FileList;
	afx_msg void OnBnClickedButton1();//�����鿴��ť
	CMFCShellTreeCtrl m_shellTreeCtrl;//���б�ؼ�
	void FindingFile(CString fileName);//��������ļ�
	virtual BOOL OnInitDialog();
	afx_msg void OnTvnSelchangedMfcshelltree2(NMHDR *pNMHDR, LRESULT *pResult);
};
