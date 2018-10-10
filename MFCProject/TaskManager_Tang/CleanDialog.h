#pragma once
#include "afxshelltreectrl.h"
#include "afxwin.h"


// CleanDialog 对话框
//清理vs工程窗口类
class CleanDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CleanDialog)

public:
	CleanDialog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CleanDialog();

	// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG5 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_DirName;//编辑框变量
	afx_msg void OnDropFiles(HDROP hDropInfo);//拉拽文件
	afx_msg void OnBnClickedButton2();//单击删除按钮
	virtual BOOL OnInitDialog();
	CMFCShellTreeCtrl m_shellTreeCtrl1;//树列表控件

	bool DeleFile(CString fileName);//删除文件
	void DeleDirectory(CString strDir);//删除整个文件夹及其内容

	CEdit m_CleanEdit;//编辑框控件
};
