// WinDialog.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "TaskManager_Tang.h"
#include "WinDialog.h"
#include "afxdialogex.h"
#include "vector"
using namespace std;

//������Ϣ����;
vector<MYWINDOWINFO> vecWindowInfo;
// WinDialog �Ի���

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


// WinDialog ��Ϣ�������


void WinDialog::EnumWindow()
{
	//ö�ٴ���;
	::EnumWindows(EnumWindowProc, 0);
	//���봰��;
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
	//�����ṹ��;
	MYWINDOWINFO* WindowInfo = new MYWINDOWINFO;
	//��ʽ�����;
	_stprintf_s(WindowInfo->hWnd, L"%p", hWnd);
	//��ȡ���ڱ���;����;
	::GetWindowTextW(hWnd, WindowInfo->WindowName, MAX_PATH);
	::GetClassNameW(hWnd, WindowInfo->ClassName, MAX_PATH);

	//����ɼ���������;	�Ҵ����б���;
	if (::GetWindowLong(hWnd, GWL_STYLE) & WS_VISIBLE && WindowInfo->WindowName[0])
	{
		vecWindowInfo.push_back(*WindowInfo);
	}
	return TRUE;
}


void WinDialog::OnRclickWindowList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������


	//��ȡ�����˵�;
	CMenu* pMenu = new CMenu;
	pMenu->LoadMenuW(IDR_MENU2);

	CMenu* pSub = pMenu->GetSubMenu(1);

	//��ȡ���λ��;
	CPoint point;
	GetCursorPos(&point);

	//����;
	pSub->TrackPopupMenu(TPM_LEFTALIGN, point.x, point.y, this, NULL);


	*pResult = 0;
}

void WinDialog::OnPopMenu(UINT id)
{
	int nSel = m_WindowList.GetSelectionMark();

	DWORD dwTemp;
	swscanf_s(m_WindowList.GetItemText(nSel, 0), L"%x", &dwTemp);

	//�жϲ˵�ID;
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

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	//������;
	m_WindowList.AddColumn(3, L"���ھ��", 100, L"���ڱ���", 200, L"��������", 210);

	//���봰��;
	EnumWindow();

	//�����˵�;

	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}

