#pragma once


// TaskMgr_Tab
//TAB�ؼ���װ��
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


	void MyInserItem(DWORD dwCount, ...);	//����ѡ��	
	void MyInsertChild(DWORD dwCount, ...);	//�����ӶԻ���
	DWORD m_dwCount;						//�����ӶԻ������;
	CDialogEx* m_pWnd[5];					//�Ի���ָ������ �Ӵ�������;

};


