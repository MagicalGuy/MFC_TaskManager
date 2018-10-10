#pragma once


// TaskMgr_Tab
//TAB控件封装类
class TaskMgr_Tab : public CTabCtrl
{
	DECLARE_DYNAMIC(TaskMgr_Tab)

public:
	TaskMgr_Tab();
	virtual ~TaskMgr_Tab();

protected:
	DECLARE_MESSAGE_MAP()
	virtual void PreSubclassWindow();
public:
	afx_msg void OnNMClick(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTcnSelchange(NMHDR *pNMHDR, LRESULT *pResult);


	void MyInserItem(DWORD dwCount, ...);	//插入选项	
	void MyInsertChild(DWORD dwCount, ...);	//插入子对话框
	DWORD m_dwCount;						//保存子对话框个数;
	CDialogEx* m_pWnd[5];					//对话框指针数组 子窗口数组;

};


