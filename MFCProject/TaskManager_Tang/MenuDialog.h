#pragma once
#include "TaskMgr_List.h"


// MenuDialog 对话框
//线程模块堆信息列表子窗口类
class MenuDialog : public CDialogEx
{
	DECLARE_DYNAMIC(MenuDialog)

public:
	//MenuDialog(CWnd* pParent = NULL);   // 标准构造函数
	MenuDialog(CWnd* pParent, int nMID, int nPID);   // 标准构造函数);
	virtual ~MenuDialog();

	// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
	TaskMgr_List m_PopList;

	int m_nMenuID;	//右键菜单ID;
	int m_nPID;		//进程ID;
	bool m_bFlag;	//显示右键菜单标志;

	void ShowModule(int nPid);	//模块;
	void ShowThread(int nPid);	//线程;
	void ShowHeap(int nPid);	//堆;

	afx_msg void OnRclickPoplist1(NMHDR *pNMHDR, LRESULT *pResult);	//右键菜单;
	afx_msg void OnMenuThread(UINT id);		//响应右键菜单;


	static UINT  HeapThreadFunction(LPVOID lpParam);	//查看堆的线程回调函数;

	HANDLE heapThread;//查看堆的线程

	static int getPid;//静态获取进程ID
};
