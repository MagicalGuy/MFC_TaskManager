#pragma once
#include "TaskMgr_List.h"
#include "afxshelltreectrl.h"


// FileDialog 对话框
//文件查看子窗口类
class FileDialog : public CDialogEx
{
	DECLARE_DYNAMIC(FileDialog)

public:
	FileDialog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~FileDialog();

	// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG3 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	TaskMgr_List m_FileList;
	afx_msg void OnBnClickedButton1();//单击查看按钮
	CMFCShellTreeCtrl m_shellTreeCtrl;//树列表控件
	void FindingFile(CString fileName);//单层遍历文件
	virtual BOOL OnInitDialog();
	afx_msg void OnTvnSelchangedMfcshelltree2(NMHDR *pNMHDR, LRESULT *pResult);
};
