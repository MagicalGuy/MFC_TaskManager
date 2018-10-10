
// TaskManager_TangDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "TaskManager_Tang.h"
#include "TaskManager_TangDlg.h"
#include"ProcessDialog.h"
#include "FileDialog.h"
#include"WinDialog.h"
#include "CleanDialog.h"
#include "afxdialogex.h"
#include <windows.h>

//导入电源管理库;
#include <powrprof.h>
#pragma  comment(lib,"powrprof.lib")

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CTaskManager_TangDlg 对话框

CTaskManager_TangDlg::CTaskManager_TangDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_TASKMANAGER_TANG_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDI_ICON1);
}

void CTaskManager_TangDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB1, m_tab);
	DDX_Control(pDX, IDC_CPU_TEXT, m_cpuText);
	DDX_Control(pDX, IDC_CPU_NUM, m_cpuNum);
	DDX_Control(pDX, IDC_MEM_TEXT, m_memText);
	DDX_Control(pDX, IDC_MEM_NUM, m_memNum);
}

BEGIN_MESSAGE_MAP(CTaskManager_TangDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_SIZE()
	ON_CONTROL_RANGE(BN_CLICKED, ID_32772, ID_32778, &CTaskManager_TangDlg::OnMenuTop)	//主菜单消息处理;
	ON_CONTROL_RANGE(BN_CLICKED, ID_32794, ID_32795, &CTaskManager_TangDlg::OnMenuTop)  ////主菜单消息处理;
	ON_WM_CLOSE()
	
	ON_WM_SIZING()
END_MESSAGE_MAP()



// CTaskManager_TangDlg 消息处理程序

BOOL CTaskManager_TangDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

//	ShowWindow(SW_MAXIMIZE);

	// TODO: 在此添加额外的初始化代码
	CMenu menu;
	menu.LoadMenu(IDR_MENU1);  //加载菜单栏
	SetMenu(&menu);

	//初始化选项卡;
	m_tab.MyInserItem(4, L"进程", L"文件", L"窗口", L"清理");
	//插入窗口;
	m_tab.MyInsertChild(4,
		new ProcessDialog, IDD_DIALOG2,
		new FileDialog, IDD_DIALOG3,
		new WinDialog, IDD_DIALOG4,
		new CleanDialog, IDD_DIALOG5
	);

	//注册快捷键;
	::RegisterHotKey(GetSafeHwnd(), 0x1234, MOD_CONTROL, '7');
	::RegisterHotKey(GetSafeHwnd(), ID_32772, MOD_CONTROL, '1');
	::RegisterHotKey(GetSafeHwnd(), ID_32773, MOD_CONTROL, '2');
	::RegisterHotKey(GetSafeHwnd(), ID_32774, MOD_CONTROL, '3');
	::RegisterHotKey(GetSafeHwnd(), ID_32775, MOD_CONTROL, '4');
	::RegisterHotKey(GetSafeHwnd(), ID_32794, MOD_CONTROL, '5');
	::RegisterHotKey(GetSafeHwnd(), ID_32795, MOD_CONTROL, '6');


	m_bClose = true;

	//创建线程刷新CPU使用率和内存占用率;
	hThread = AfxBeginThread(MyThreadFunction, this);
	
	//设置定时器;
	SetTimer(0, 15000, NULL);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}


// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CTaskManager_TangDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}


	ReSize();

}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CTaskManager_TangDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CTaskManager_TangDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);
	if (nType != SIZE_MINIMIZED)  //判断窗口是不是最小化了，因为窗口最小化之后 ，窗口的长和宽会变成0，当前一次变化的时就会出现除以0的错误操作
	{
		ReSize();
	}

	// TODO: 在此处添加消息处理程序代码
}

void CTaskManager_TangDlg::OnMenuTop(UINT id)
{

	//获取关机重启权限;
	HANDLE hToken = NULL;
	HANDLE hProcess = GetCurrentProcess();//伪句柄
	OpenProcessToken(hProcess, TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken);
	TOKEN_PRIVILEGES tp = { 0 };
	LookupPrivilegeValue(0, SE_SHUTDOWN_NAME, &tp.Privileges[0].Luid);
	tp.PrivilegeCount = 1;
	tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
	//调用函数提升权限
	AdjustTokenPrivileges(hToken, FALSE, &tp, sizeof(tp), NULL, NULL);

	int Flag = 0;

	switch (id)
	{
		//休眠
	case ID_32775:
		Flag = MessageBox(L"确定要休眠吗？\r\n点击【确定】休眠\r\n点击【取消】继续", L"休眠", MB_OKCANCEL);
		if (Flag == IDCANCEL)
		{
			break;
		}
		else
		{
			SetSuspendState(TRUE, FALSE, FALSE);
		}
		break;
		//注销
	case ID_32774:

		Flag = MessageBox(L"确定要注销吗？\r\n点击【确定】注销\r\n点击【取消】继续", L"注销", MB_OKCANCEL);
		if (Flag == IDCANCEL)
		{
			break;
		}
		else
		{
			ExitWindowsEx(EWX_LOGOFF | EWX_FORCE, SHTDN_REASON_MAJOR_OTHER);
		}
		break;


		//关机
	case ID_32772:
		Flag = MessageBox(L"确定要关机吗？\r\n点击【确定】关机\r\n点击【取消】继续", L"关机", MB_OKCANCEL);
		if (Flag == IDCANCEL)
		{
			break;
		}
		else
		{
			ExitWindowsEx(EWX_POWEROFF | EWX_FORCE, SHTDN_REASON_MAJOR_OTHER);
		}
		break;
	case ID_32773:
		//重启提示;
		Flag = MessageBox(L"确定要重启吗？\r\n点击【确定】重启\r\n点击【取消】继续", L"重启", MB_OKCANCEL);
		if (Flag == IDCANCEL)
		{
			break;
		}
		else
		{
			ExitWindowsEx(EWX_REBOOT | EWX_FORCE, SHTDN_REASON_MAJOR_OTHER);
		}
		break;
		//锁屏
	case ID_32795:
		Flag = MessageBox(L"确定要锁屏吗？\r\n点击【确定】锁屏\r\n点击【取消】继续", L"锁屏", MB_OKCANCEL);
		if (Flag == IDCANCEL)
		{
			break;
		}
		else
		{
			LockWorkStation();
		}
		break;
		//睡眠
	case ID_32794:
		Flag = MessageBox(L"确定要睡眠吗？\r\n点击【确定】睡眠\r\n点击【取消】继续", L"睡眠", MB_OKCANCEL);
		if (Flag == IDCANCEL)
		{
			break;
		}
		else
		{
			SetSuspendState(FALSE, FALSE, FALSE);
		}
		break;
		//60S刷新时间间隔
	case ID_32776:
	{
		::KillTimer(hwnd, 1);
		::KillTimer(hwnd, 2);
		::SetTimer(hwnd, 3, 60000, NULL);
	}
	break;
	//30S
	case ID_32777:
	{
		::KillTimer(hwnd, 1);
		::KillTimer(hwnd, 3);
		::SetTimer(hwnd, 2, 30000, NULL);
	}
	break;
	//15S
	case ID_32778:
	{
		::KillTimer(hwnd, 2);
		::KillTimer(hwnd, 3);
		::SetTimer(hwnd, 1, 15000, NULL);
	}
	break;
	}

}

void CTaskManager_TangDlg::ReSize()
{

	float fsp[2];
	POINT Newp; //获取现在对话框的大小  
	CRect recta;
	GetClientRect(&recta);     //取客户区大小    
	Newp.x = recta.right - recta.left;
	Newp.y = recta.bottom - recta.top;
	fsp[0] = (float)Newp.x / old.x;
	fsp[1] = (float)Newp.y / old.y;
	CRect Rect;
	int woc;
	CPoint OldTLPoint, TLPoint; //左上角  
	CPoint OldBRPoint, BRPoint; //右下角  
	HWND  hwndChild = ::GetWindow(m_hWnd, GW_CHILD);  //列出所有控件    
	while (hwndChild) {
		woc = ::GetDlgCtrlID(hwndChild);//取得ID  
		GetDlgItem(woc)->GetWindowRect(Rect);
		ScreenToClient(Rect);
		OldTLPoint = Rect.TopLeft();
		TLPoint.x = long(OldTLPoint.x*fsp[0]);
		TLPoint.y = long(OldTLPoint.y*fsp[1]);
		OldBRPoint = Rect.BottomRight();
		BRPoint.x = long(OldBRPoint.x *fsp[0]);
		BRPoint.y = long(OldBRPoint.y *fsp[1]);
		Rect.SetRect(TLPoint, BRPoint);
		GetDlgItem(woc)->MoveWindow(Rect, TRUE);
		hwndChild = ::GetWindow(hwndChild, GW_HWNDNEXT);
	}
	old = Newp;

}


BOOL CTaskManager_TangDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类

	//响应快捷键;
	if ((pMsg->message == WM_HOTKEY) && (pMsg->wParam == 0x1234))
	{
		if (IsWindowVisible() == TRUE)
		{
			ShowWindow(SW_HIDE);
		}
		else
		{
			ShowWindow(SW_SHOW);
		}
	}
	//响应各个ID
	else if ((pMsg->message == WM_HOTKEY) && ((pMsg->wParam == ID_32775) ||
		(pMsg->wParam == ID_32774) || (pMsg->wParam == ID_32772) ||
		(pMsg->wParam == ID_32773) || (pMsg->wParam == ID_32794) || (pMsg->wParam == ID_32795))) {
		OnMenuTop(pMsg->wParam);
	}

	return CDialogEx::PreTranslateMessage(pMsg);

}



UINT CTaskManager_TangDlg::MyThreadFunction(LPVOID lpParam)
{
	CTaskManager_TangDlg*  TangDlg = (CTaskManager_TangDlg*)lpParam;

	CString Buff1;
	CString Buff2;

	MEMORYSTATUS memStatus;
	memStatus.dwLength = sizeof(MEMORYSTATUS);

	while (TangDlg->m_bClose)
	{
		//CPU利用率
		Buff1.Format(L"利用率 %d %%", GetCpuUsage());
		TangDlg->m_cpuNum.SetWindowText(Buff1);

		//内存占用率
		GlobalMemoryStatus(&memStatus);
		Buff2.Format(L"占用率 %d %%", (int)memStatus.dwMemoryLoad);
		TangDlg->m_memNum.SetWindowText(Buff2);
		Sleep(500);
	}

	return 0;
}


void CTaskManager_TangDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	//关闭线程;
	m_bClose = false;

	CDialogEx::OnClose();
}


double CTaskManager_TangDlg::FILETIME2Double(const _FILETIME& fileTime) {
	return double(fileTime.dwHighDateTime*4.294967296e9) + double(fileTime.dwLowDateTime);
}
//计算CPU占用率
int CTaskManager_TangDlg::GetCpuUsage() {
	//空闲时间   内核时间  用户时间
	_FILETIME idleTime, kernelTime, userTime;
	//获取时间
	GetSystemTimes(&idleTime, &kernelTime, &userTime);
	HANDLE hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	//等待1000毫秒使用内核对象会更精确
	WaitForSingleObject(hEvent, 1000);
	//获取新的时间
	_FILETIME newIdleTime, newKernelTime, newUserTime;
	GetSystemTimes(&newIdleTime, &newKernelTime, &newUserTime);
	//将各个时间转换
	double dOldIdleTime = FILETIME2Double(idleTime);
	double dNewIdleTime = FILETIME2Double(newIdleTime);
	double dOldKernelTime = FILETIME2Double(kernelTime);
	double dNewKernelTime = FILETIME2Double(newKernelTime);
	double dOldUserTime = FILETIME2Double(userTime);
	double dNewUserTime = FILETIME2Double(newUserTime);
	//计算出使用率
	return int(100.0 - (dNewIdleTime - dOldIdleTime) / (dNewKernelTime - dOldKernelTime + dNewUserTime - dOldUserTime)*100.0);
}


void CTaskManager_TangDlg::OnSizing(UINT fwSide, LPRECT pRect)
{
	CDialogEx::OnSizing(fwSide, pRect);
	// TODO: 在此处添加消息处理程序代码
	ReSize();
}
