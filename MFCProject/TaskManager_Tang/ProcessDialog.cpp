// ProcessDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "TaskManager_Tang.h"
#include "ProcessDialog.h"
#include "afxdialogex.h"
#include <TlHelp32.h>


HWND hwnd;

// ProcessDialog 对话框


IMPLEMENT_DYNAMIC(ProcessDialog, CDialogEx)

ProcessDialog::ProcessDialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG2, pParent) {}

ProcessDialog::~ProcessDialog() {}

void ProcessDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_Process_LIST1, m_List);
}


BEGIN_MESSAGE_MAP(ProcessDialog, CDialogEx)

	ON_COMMAND(ID_32791, &ProcessDialog::OnFresh)//刷新
	ON_COMMAND(ID_32782, &ProcessDialog::OnClose)//结束进程
	ON_CONTROL_RANGE(BN_CLICKED, ID_32779, ID_32781, &ProcessDialog::OnMenuClick)//挂起恢复结束线程
	ON_CONTROL_RANGE(BN_CLICKED, ID_32783, ID_32785, &ProcessDialog::OnMenuClick)//查看线程模块堆
	ON_NOTIFY(NM_RCLICK, IDC_Process_LIST1, &ProcessDialog::OnRclickProcessList1)//右击
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_Process_LIST1, &ProcessDialog::OnColumnclickProcessList1)//点击列名
	ON_WM_TIMER()
END_MESSAGE_MAP()


// ProcessDialog 消息处理程序

BOOL ProcessDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	//初始化列表;
	m_List.AddColumn(5, L"名称", 120, L"PID", 50, L"父进程PID", 80, L"线程数量", 80, L"路径", 180);
	//显示进程;
	ShowProcess();

	SetTimer(1, 10000, NULL);


	hwnd = this->m_hWnd;


	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

void ProcessDialog::ShowProcess()
{
	HANDLE hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
	if (hSnapShot == INVALID_HANDLE_VALUE)
	{
		MessageBox(L"HELP 函数调用失败！");
		return;
	}

	//初始化快照信息结构体;
	PROCESSENTRY32 pi = { sizeof(PROCESSENTRY32) };
	if (!Process32First(hSnapShot, &pi))
	{
		MessageBox(L"获取进程信息失败");
		return;
	}
	do
	{
		TCHAR *szBuf[5] = { new TCHAR[MAX_PATH], new TCHAR[6],  new TCHAR[6],  new TCHAR[6], new TCHAR[MAX_PATH], };
		szBuf[0] = pi.szExeFile;
		if (pi.th32ProcessID > 0 && pi.th32ParentProcessID > 0 && pi.cntThreads >= 0)
		{
			_stprintf_s(szBuf[1], 6, L"%d", pi.th32ProcessID);
			_stprintf_s(szBuf[2], 6, L"%d", pi.th32ParentProcessID);
			_stprintf_s(szBuf[3], 6, L"%d", pi.cntThreads);
		}
		else
		{
			szBuf[1] = L"  ";
			szBuf[2] = L"  ";
			szBuf[3] = L"  ";
		}

		//打开要获取路径的进程
		HANDLE hPro = OpenProcess(PROCESS_QUERY_INFORMATION
			| PROCESS_QUERY_LIMITED_INFORMATION,
			FALSE, pi.th32ProcessID);
		//获取进程完整路径;
		if (hPro)
		{
			DWORD dwSize = MAX_PATH;
			QueryFullProcessImageName(hPro, NULL, szBuf[4], &dwSize);
		}
		else
		{
			szBuf[4] = L"无权访问";
		}
		m_List.AddItem(5, szBuf[0], szBuf[1], szBuf[2], szBuf[3], szBuf[4]);
		CloseHandle(hPro);
	} while (Process32Next(hSnapShot, &pi));

	return;
}

void ProcessDialog::OnFresh()
{
	//刷新进程;
	m_List.DeleteAllItems();
	ShowProcess();
}

void ProcessDialog::OnClose()
{
	//获取Pid;
	int nSel = m_List.GetSelectionMark();
	if (nSel < 0)
	{
		MessageBox(L"请选中数据");
	}
	int PID = _wtoi(m_List.GetItemText(nSel, 1));
	//获取句柄;
	HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, PID);
	if (TerminateProcess(hProcess, 0))
	{
		MessageBox(L"结束成功!");
	}
	else
	{
		MessageBox(L"结束失败!");
	}
	CloseHandle(hProcess);
	m_List.DeleteAllItems();
	ShowProcess();
}



void ProcessDialog::OnColumnclickProcessList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	//获取点击列;
	int Sort_Colum = pNMLV->iSubItem;

	//0列4列字符串排序;其他整型排序;
	if (Sort_Colum == 0 || Sort_Colum == 4)
		m_List.SortItemsPlus(Sort_Colum, false);
	else
		m_List.SortItemsPlus(Sort_Colum);

	*pResult = 0;
}

void ProcessDialog::OnMenuClick(UINT id)
{
	//获取选中项;
	int nSel = m_List.GetSelectionMark();
	if (nSel < 0)
	{
		MessageBox(L"请选中数据");
	}
	int PID = _wtoi(m_List.GetItemText(nSel, 1));

	MenuDialog menuInfo(this, id, PID);
	menuInfo.DoModal();
}

void ProcessDialog::OnRclickProcessList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码

	//获取弹出菜单;
	CMenu* pMenu = new CMenu;
	pMenu->LoadMenuW(IDR_MENU2);

	CMenu* pSub = pMenu->GetSubMenu(0);

	//获取鼠标位置;
	CPoint point;
	GetCursorPos(&point);

	//弹出;
	pSub->TrackPopupMenu(TPM_LEFTALIGN, point.x, point.y, this, NULL);

	*pResult = 0;
}


void ProcessDialog::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	switch (nIDEvent)
	{
	case 1:case 2:case 3:
		m_List.DeleteAllItems();
		ShowProcess();
		break;
	default:
		break;
	}
	CDialogEx::OnTimer(nIDEvent);
}
