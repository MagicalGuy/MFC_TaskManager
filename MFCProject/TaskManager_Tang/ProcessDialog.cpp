// ProcessDialog.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "TaskManager_Tang.h"
#include "ProcessDialog.h"
#include "afxdialogex.h"
#include <TlHelp32.h>


HWND hwnd;

// ProcessDialog �Ի���


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

	ON_COMMAND(ID_32791, &ProcessDialog::OnFresh)//ˢ��
	ON_COMMAND(ID_32782, &ProcessDialog::OnClose)//��������
	ON_CONTROL_RANGE(BN_CLICKED, ID_32779, ID_32781, &ProcessDialog::OnMenuClick)//����ָ������߳�
	ON_CONTROL_RANGE(BN_CLICKED, ID_32783, ID_32785, &ProcessDialog::OnMenuClick)//�鿴�߳�ģ���
	ON_NOTIFY(NM_RCLICK, IDC_Process_LIST1, &ProcessDialog::OnRclickProcessList1)//�һ�
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_Process_LIST1, &ProcessDialog::OnColumnclickProcessList1)//�������
	ON_WM_TIMER()
END_MESSAGE_MAP()


// ProcessDialog ��Ϣ�������

BOOL ProcessDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	//��ʼ���б�;
	m_List.AddColumn(5, L"����", 120, L"PID", 50, L"������PID", 80, L"�߳�����", 80, L"·��", 180);
	//��ʾ����;
	ShowProcess();

	SetTimer(1, 10000, NULL);


	hwnd = this->m_hWnd;


	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}

void ProcessDialog::ShowProcess()
{
	HANDLE hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
	if (hSnapShot == INVALID_HANDLE_VALUE)
	{
		MessageBox(L"HELP ��������ʧ�ܣ�");
		return;
	}

	//��ʼ��������Ϣ�ṹ��;
	PROCESSENTRY32 pi = { sizeof(PROCESSENTRY32) };
	if (!Process32First(hSnapShot, &pi))
	{
		MessageBox(L"��ȡ������Ϣʧ��");
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

		//��Ҫ��ȡ·���Ľ���
		HANDLE hPro = OpenProcess(PROCESS_QUERY_INFORMATION
			| PROCESS_QUERY_LIMITED_INFORMATION,
			FALSE, pi.th32ProcessID);
		//��ȡ��������·��;
		if (hPro)
		{
			DWORD dwSize = MAX_PATH;
			QueryFullProcessImageName(hPro, NULL, szBuf[4], &dwSize);
		}
		else
		{
			szBuf[4] = L"��Ȩ����";
		}
		m_List.AddItem(5, szBuf[0], szBuf[1], szBuf[2], szBuf[3], szBuf[4]);
		CloseHandle(hPro);
	} while (Process32Next(hSnapShot, &pi));

	return;
}

void ProcessDialog::OnFresh()
{
	//ˢ�½���;
	m_List.DeleteAllItems();
	ShowProcess();
}

void ProcessDialog::OnClose()
{
	//��ȡPid;
	int nSel = m_List.GetSelectionMark();
	if (nSel < 0)
	{
		MessageBox(L"��ѡ������");
	}
	int PID = _wtoi(m_List.GetItemText(nSel, 1));
	//��ȡ���;
	HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, PID);
	if (TerminateProcess(hProcess, 0))
	{
		MessageBox(L"�����ɹ�!");
	}
	else
	{
		MessageBox(L"����ʧ��!");
	}
	CloseHandle(hProcess);
	m_List.DeleteAllItems();
	ShowProcess();
}



void ProcessDialog::OnColumnclickProcessList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//��ȡ�����;
	int Sort_Colum = pNMLV->iSubItem;

	//0��4���ַ�������;������������;
	if (Sort_Colum == 0 || Sort_Colum == 4)
		m_List.SortItemsPlus(Sort_Colum, false);
	else
		m_List.SortItemsPlus(Sort_Colum);

	*pResult = 0;
}

void ProcessDialog::OnMenuClick(UINT id)
{
	//��ȡѡ����;
	int nSel = m_List.GetSelectionMark();
	if (nSel < 0)
	{
		MessageBox(L"��ѡ������");
	}
	int PID = _wtoi(m_List.GetItemText(nSel, 1));

	MenuDialog menuInfo(this, id, PID);
	menuInfo.DoModal();
}

void ProcessDialog::OnRclickProcessList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	//��ȡ�����˵�;
	CMenu* pMenu = new CMenu;
	pMenu->LoadMenuW(IDR_MENU2);

	CMenu* pSub = pMenu->GetSubMenu(0);

	//��ȡ���λ��;
	CPoint point;
	GetCursorPos(&point);

	//����;
	pSub->TrackPopupMenu(TPM_LEFTALIGN, point.x, point.y, this, NULL);

	*pResult = 0;
}


void ProcessDialog::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

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
