
// TaskManager_TangDlg.cpp : ʵ���ļ�
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

//�����Դ�����;
#include <powrprof.h>
#pragma  comment(lib,"powrprof.lib")

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CTaskManager_TangDlg �Ի���

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
	ON_CONTROL_RANGE(BN_CLICKED, ID_32772, ID_32778, &CTaskManager_TangDlg::OnMenuTop)	//���˵���Ϣ����;
	ON_CONTROL_RANGE(BN_CLICKED, ID_32794, ID_32795, &CTaskManager_TangDlg::OnMenuTop)  ////���˵���Ϣ����;
	ON_WM_CLOSE()
	
	ON_WM_SIZING()
END_MESSAGE_MAP()



// CTaskManager_TangDlg ��Ϣ�������

BOOL CTaskManager_TangDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

//	ShowWindow(SW_MAXIMIZE);

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	CMenu menu;
	menu.LoadMenu(IDR_MENU1);  //���ز˵���
	SetMenu(&menu);

	//��ʼ��ѡ�;
	m_tab.MyInserItem(4, L"����", L"�ļ�", L"����", L"����");
	//���봰��;
	m_tab.MyInsertChild(4,
		new ProcessDialog, IDD_DIALOG2,
		new FileDialog, IDD_DIALOG3,
		new WinDialog, IDD_DIALOG4,
		new CleanDialog, IDD_DIALOG5
	);

	//ע���ݼ�;
	::RegisterHotKey(GetSafeHwnd(), 0x1234, MOD_CONTROL, '7');
	::RegisterHotKey(GetSafeHwnd(), ID_32772, MOD_CONTROL, '1');
	::RegisterHotKey(GetSafeHwnd(), ID_32773, MOD_CONTROL, '2');
	::RegisterHotKey(GetSafeHwnd(), ID_32774, MOD_CONTROL, '3');
	::RegisterHotKey(GetSafeHwnd(), ID_32775, MOD_CONTROL, '4');
	::RegisterHotKey(GetSafeHwnd(), ID_32794, MOD_CONTROL, '5');
	::RegisterHotKey(GetSafeHwnd(), ID_32795, MOD_CONTROL, '6');


	m_bClose = true;

	//�����߳�ˢ��CPUʹ���ʺ��ڴ�ռ����;
	hThread = AfxBeginThread(MyThreadFunction, this);
	
	//���ö�ʱ��;
	SetTimer(0, 15000, NULL);

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}


// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CTaskManager_TangDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}


	ReSize();

}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CTaskManager_TangDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CTaskManager_TangDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);
	if (nType != SIZE_MINIMIZED)  //�жϴ����ǲ�����С���ˣ���Ϊ������С��֮�� �����ڵĳ��Ϳ����0����ǰһ�α仯��ʱ�ͻ���ֳ���0�Ĵ������
	{
		ReSize();
	}

	// TODO: �ڴ˴������Ϣ����������
}

void CTaskManager_TangDlg::OnMenuTop(UINT id)
{

	//��ȡ�ػ�����Ȩ��;
	HANDLE hToken = NULL;
	HANDLE hProcess = GetCurrentProcess();//α���
	OpenProcessToken(hProcess, TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken);
	TOKEN_PRIVILEGES tp = { 0 };
	LookupPrivilegeValue(0, SE_SHUTDOWN_NAME, &tp.Privileges[0].Luid);
	tp.PrivilegeCount = 1;
	tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
	//���ú�������Ȩ��
	AdjustTokenPrivileges(hToken, FALSE, &tp, sizeof(tp), NULL, NULL);

	int Flag = 0;

	switch (id)
	{
		//����
	case ID_32775:
		Flag = MessageBox(L"ȷ��Ҫ������\r\n�����ȷ��������\r\n�����ȡ��������", L"����", MB_OKCANCEL);
		if (Flag == IDCANCEL)
		{
			break;
		}
		else
		{
			SetSuspendState(TRUE, FALSE, FALSE);
		}
		break;
		//ע��
	case ID_32774:

		Flag = MessageBox(L"ȷ��Ҫע����\r\n�����ȷ����ע��\r\n�����ȡ��������", L"ע��", MB_OKCANCEL);
		if (Flag == IDCANCEL)
		{
			break;
		}
		else
		{
			ExitWindowsEx(EWX_LOGOFF | EWX_FORCE, SHTDN_REASON_MAJOR_OTHER);
		}
		break;


		//�ػ�
	case ID_32772:
		Flag = MessageBox(L"ȷ��Ҫ�ػ���\r\n�����ȷ�����ػ�\r\n�����ȡ��������", L"�ػ�", MB_OKCANCEL);
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
		//������ʾ;
		Flag = MessageBox(L"ȷ��Ҫ������\r\n�����ȷ��������\r\n�����ȡ��������", L"����", MB_OKCANCEL);
		if (Flag == IDCANCEL)
		{
			break;
		}
		else
		{
			ExitWindowsEx(EWX_REBOOT | EWX_FORCE, SHTDN_REASON_MAJOR_OTHER);
		}
		break;
		//����
	case ID_32795:
		Flag = MessageBox(L"ȷ��Ҫ������\r\n�����ȷ��������\r\n�����ȡ��������", L"����", MB_OKCANCEL);
		if (Flag == IDCANCEL)
		{
			break;
		}
		else
		{
			LockWorkStation();
		}
		break;
		//˯��
	case ID_32794:
		Flag = MessageBox(L"ȷ��Ҫ˯����\r\n�����ȷ����˯��\r\n�����ȡ��������", L"˯��", MB_OKCANCEL);
		if (Flag == IDCANCEL)
		{
			break;
		}
		else
		{
			SetSuspendState(FALSE, FALSE, FALSE);
		}
		break;
		//60Sˢ��ʱ����
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
	POINT Newp; //��ȡ���ڶԻ���Ĵ�С  
	CRect recta;
	GetClientRect(&recta);     //ȡ�ͻ�����С    
	Newp.x = recta.right - recta.left;
	Newp.y = recta.bottom - recta.top;
	fsp[0] = (float)Newp.x / old.x;
	fsp[1] = (float)Newp.y / old.y;
	CRect Rect;
	int woc;
	CPoint OldTLPoint, TLPoint; //���Ͻ�  
	CPoint OldBRPoint, BRPoint; //���½�  
	HWND  hwndChild = ::GetWindow(m_hWnd, GW_CHILD);  //�г����пؼ�    
	while (hwndChild) {
		woc = ::GetDlgCtrlID(hwndChild);//ȡ��ID  
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
	// TODO: �ڴ����ר�ô����/����û���

	//��Ӧ��ݼ�;
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
	//��Ӧ����ID
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
		//CPU������
		Buff1.Format(L"������ %d %%", GetCpuUsage());
		TangDlg->m_cpuNum.SetWindowText(Buff1);

		//�ڴ�ռ����
		GlobalMemoryStatus(&memStatus);
		Buff2.Format(L"ռ���� %d %%", (int)memStatus.dwMemoryLoad);
		TangDlg->m_memNum.SetWindowText(Buff2);
		Sleep(500);
	}

	return 0;
}


void CTaskManager_TangDlg::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	//�ر��߳�;
	m_bClose = false;

	CDialogEx::OnClose();
}


double CTaskManager_TangDlg::FILETIME2Double(const _FILETIME& fileTime) {
	return double(fileTime.dwHighDateTime*4.294967296e9) + double(fileTime.dwLowDateTime);
}
//����CPUռ����
int CTaskManager_TangDlg::GetCpuUsage() {
	//����ʱ��   �ں�ʱ��  �û�ʱ��
	_FILETIME idleTime, kernelTime, userTime;
	//��ȡʱ��
	GetSystemTimes(&idleTime, &kernelTime, &userTime);
	HANDLE hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	//�ȴ�1000����ʹ���ں˶�������ȷ
	WaitForSingleObject(hEvent, 1000);
	//��ȡ�µ�ʱ��
	_FILETIME newIdleTime, newKernelTime, newUserTime;
	GetSystemTimes(&newIdleTime, &newKernelTime, &newUserTime);
	//������ʱ��ת��
	double dOldIdleTime = FILETIME2Double(idleTime);
	double dNewIdleTime = FILETIME2Double(newIdleTime);
	double dOldKernelTime = FILETIME2Double(kernelTime);
	double dNewKernelTime = FILETIME2Double(newKernelTime);
	double dOldUserTime = FILETIME2Double(userTime);
	double dNewUserTime = FILETIME2Double(newUserTime);
	//�����ʹ����
	return int(100.0 - (dNewIdleTime - dOldIdleTime) / (dNewKernelTime - dOldKernelTime + dNewUserTime - dOldUserTime)*100.0);
}


void CTaskManager_TangDlg::OnSizing(UINT fwSide, LPRECT pRect)
{
	CDialogEx::OnSizing(fwSide, pRect);
	// TODO: �ڴ˴������Ϣ����������
	ReSize();
}
