#pragma once

// TaskMgr_List
//�б��װ��
class TaskMgr_List : public CListCtrl
{
	DECLARE_DYNAMIC(TaskMgr_List)

public:
	TaskMgr_List();
	virtual ~TaskMgr_List();


	void AddColumn(DWORD dwCount, ...);					//��������;
	void AddItem(DWORD dwCount, ...);					//������Ԫ��;
	void SortItemsPlus(int nColum, bool Isint = true);	//����;


protected:
	DECLARE_MESSAGE_MAP()
	//����;
	static bool Isint;		//�Ƿ�int������,Ĭ����
	static bool method;		//������
	static int CALLBACK ListCompare(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);	//����ص�����
};

//����ṹ��;
typedef struct  _DATA
{
	int SubItem;
	TaskMgr_List* pList;
}DATA;




