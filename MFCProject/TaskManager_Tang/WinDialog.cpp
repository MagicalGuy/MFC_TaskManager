// WinDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "TaskManager_Tang.h"
#include "WinDialog.h"
#include "afxdialogex.h"
#include "vector"
using namespace std;

//窗口信息容器;
vector<MYWINDOWINFO> vecWindowInfo;
// WinDialog 对话框

IMPLEMENT_DYNAMIC(WinDialog, CDialogEx)

WinDialog::WinDialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG4, pParent)
{

}

WinDialog::~WinDialog()
{
}

void WinDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_WINDOW_LIST1, m_WindowList);
}


BEGIN_MESSAGE_MAP(WinDialog, CDialogEx)
	ON_NOTIFY(NM_RCLICK, IDC_WINDOW_LIST1, &WinDialog::OnRclickWindowList1)
	ON_CONTROL_RANGE(BN_CLICKED, ID_32792, ID_32793, &WinDialog::OnPopMenu)

	//	ON_WM_RBUTTONDBLCLK()
END_MESSAGE_MAP()


// WinDialog 消息处理程序


void WinDialog::EnumWindow()
{
	//枚举窗口;
	::EnumWindows(EnumWindowProc, 0);
	//插入窗口;
	for (size_t i = 0; i < vecWindowInfo.size(); i++)
	{
		m_WindowList.AddItem(3,
			vecWindowInfo[i].hWnd,
			vecWindowInfo[i].WindowName,
			vecWindowInfo[i].ClassName);
	}
}

BOOL CALLBACK WinDialog::EnumWindowProc(HWND hWnd, LPARAM lParam)
{
	//建立结构体;
	MYWINDOWINFO* WindowInfo = new MYWINDOWINFO;
	//格式化句柄;
	_stprintf_s(WindowInfo->hWnd, L"%p", hWnd);
	//获取窗口标题;类名;
	::GetWindowTextW(hWnd, WindowInfo->WindowName, MAX_PATH);
	::GetClassNameW(hWnd, WindowInfo->ClassName, MAX_PATH);

	//窗体可见存入容器;	且窗口有标题;
	if (::GetWindowLong(hWnd, GWL_STYLE) & WS_VISIBLE && WindowInfo->WindowName[0])
	{
		vecWindowInfo.push_back(*WindowInfo);
	}
	return TRUE;
}


void WinDialog::OnRclickWindowList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码


	//获取弹出菜单;
	CMenu* pMenu = new CMenu;
	pMenu->LoadMenuW(IDR_MENU2);

	CMenu* pSub = pMenu->GetSubMenu(1);

	//获取鼠标位置;
	CPoint point;
	GetCursorPos(&point);

	//弹出;
	pSub->TrackPopupMenu(TPM_LEFTALIGN, point.x, point.y, this, NULL);


	*pResult = 0;
}

void WinDialog::OnPopMenu(UINT id)
{
	int nSel = m_WindowList.GetSelectionMark();

	DWORD dwTemp;
	swscanf_s(m_WindowList.GetItemText(nSel, 0), L"%x", &dwTemp);

	//判断菜单ID;
	switch (id)
	{
	case ID_32792:
		m_WindowList.DeleteAllItems();
		vecWindowInfo.clear();
		EnumWindow();
		break;
	case ID_32793:
		::PostMessage((HWND)dwTemp, WM_SYSCOMMAND, SC_CLOSE, 0);
		break;
	default:
		break;
	}
}

BOOL WinDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	//插入列;
	m_WindowList.AddColumn(3, L"窗口句柄", 100, L"窗口标题", 200, L"窗口类名", 210);

	//插入窗口;
	EnumWindow();

	//弹出菜单;

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

