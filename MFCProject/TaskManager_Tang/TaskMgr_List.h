#pragma once

// TaskMgr_List
//列表封装类
class TaskMgr_List : public CListCtrl
{
	DECLARE_DYNAMIC(TaskMgr_List)

public:
	TaskMgr_List();
	virtual ~TaskMgr_List();


	void AddColumn(DWORD dwCount, ...);					//插入列名;
	void AddItem(DWORD dwCount, ...);					//插入行元素;
	void SortItemsPlus(int nColum, bool Isint = true);	//排序;


protected:
	DECLARE_MESSAGE_MAP()
	//排序;
	static bool Isint;		//是否int型排序,默认是
	static bool method;		//排序方向
	static int CALLBACK ListCompare(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);	//排序回调函数
};

//排序结构体;
typedef struct  _DATA
{
	int SubItem;
	TaskMgr_List* pList;
}DATA;




