// MenuDialog.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "TaskManager_Tang.h"
#include "MenuDialog.h"
#include "afxdialogex.h"
#include <TlHelp32.h>

// MenuDialog �Ի���

int MenuDialog::getPid = 0;

IMPLEMENT_DYNAMIC(MenuDialog, CDialogEx)

//MenuDialog::MenuDialog(CWnd* pParent /*=NULL*/)
//	: CDialogEx(IDD_DIALOG1, pParent){}

MenuDialog::MenuDialog(CWnd* pParent, int nMID, int nPID) : CDialogEx(IDD_DIALOG1, pParent)
{
	this->m_nMenuID = nMID;
	this->m_nPID = nPID;
	m_bFlag = false;
}



MenuDialog::~MenuDialog()
{
}

void MenuDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_POPLIST1, m_PopList);
}



BEGIN_MESSAGE_MAP(MenuDialog, CDialogEx)

	ON_NOTIFY(NM_RCLICK, IDC_POPLIST1, &MenuDialog::OnRclickPoplist1)
	ON_CONTROL_RANGE(BN_CLICKED, ID_32779, ID_32781, &MenuDialog::OnMenuThread)	//����˵���;

END_MESSAGE_MAP()


// MenuDialog ��Ϣ�������



BOOL MenuDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	getPid = m_nPID;
	switch (m_nMenuID)
	{
	case ID_32783:
		m_bFlag = true;
		ShowThread(m_nPID);
		break;
	case ID_32784:
		ShowModule(m_nPID);
		break;
	case ID_32785:
		//ShowHeap(m_nPID);
		heapThread = AfxBeginThread(HeapThreadFunction, this);
		break;
	default:
		break;
	}

	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}

void MenuDialog::ShowModule(int nPid)
{
	HANDLE hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, nPid);
	if (hSnapShot == INVALID_HANDLE_VALUE)
	{
		MessageBox(L"�򿪽�����Ϣʧ�ܣ�");
		return;
	}
	MODULEENTRY32 me = { sizeof(MODULEENTRY32) };
	if (!Module32First(hSnapShot, &me))
	{
		MessageBox(L"��ȡģ����Ϣʧ�ܣ�");
		return;
	}
	m_PopList.AddColumn(3, L"ģ������", 100, L"ģ���С", 100, L"ģ��·��", 290);
	do
	{
		TCHAR* SizeBuf = new TCHAR[11];
		_stprintf_s(SizeBuf, 10, L"%d", me.modBaseSize);
		m_PopList.AddItem(3, me.szModule, SizeBuf, me.szExePath);
	} while (Module32Next(hSnapShot, &me));

	return;
}

void MenuDialog::ShowThread(int nPid)
{
	//��������;
	HANDLE hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, NULL);

	//���������Ϣ�ṹ��;
	THREADENTRY32 ThreadInfo = { sizeof(THREADENTRY32) };

	//��������;
	if (Thread32First(hSnapShot, &ThreadInfo))
	{
		m_PopList.AddColumn(3, L"�߳�ID", 160, L"���ȼ�", 160, L"״̬", 170);
		do {
			//��ʾ�����������߳�;
			if (ThreadInfo.th32OwnerProcessID == m_nPID)
			{
				CString PID, Levele, State;
				//�߳�ID
				PID.Format(L"%d", ThreadInfo.th32ThreadID);
				//���ȼ�
				Levele.Format(L"%d", ThreadInfo.tpBasePri);

				//��ȡ�߳̾��;
				//DWORD dwState = 0;
				HANDLE hThread = OpenThread(THREAD_SUSPEND_RESUME, FALSE, ThreadInfo.th32ThreadID);

				//�ж��߳�״̬;
				//GetExitCodeThread(hThread, &dwState);
				//����ķ�����ȡSTILL_ACTIVE�жϲ��ˣ���Ϊ��������ж���STILL_ACTIVE

				int suspendCount = -1;//�������������ͣ������  �����0��������
				suspendCount = SuspendThread(hThread);
				ResumeThread(hThread);

				if (suspendCount == 0 || suspendCount == -1)
				{
					State = L"������";
				}
				else
				{
					State = L"����";
				}
				CloseHandle(hThread);
				m_PopList.AddItem(3, PID, Levele, State);
			}

		} while (Thread32Next(hSnapShot, &ThreadInfo));
	}
	else
	{
		MessageBox(L"��ȡ�߳���Ϣʧ�ܣ�");
	}
}

void MenuDialog::ShowHeap(int nPid)
{
	//��ȡ����;
	HANDLE hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPHEAPLIST, nPid);
	if (!hSnapShot)
	{
		MessageBox(L"��ȡ��Ϣʧ��");
		return;
	}

	//����Ϣ;
	HEAPLIST32 HeapListInfo = { 0 };
	HeapListInfo.dwSize = sizeof(HEAPLIST32);

	//��ȡ��һ������Ϣ;
	if (Heap32ListFirst(hSnapShot, &HeapListInfo))
	{
		m_PopList.AddColumn(4, L"��ID", 120, L"��С", 120, L"��ʼ��ַ", 120, L"״̬", 130);
		do
		{
			//�ѿ���Ϣ;
			HEAPENTRY32 HeapInfo = { 0 };
			HeapInfo.dwSize = sizeof(HEAPENTRY32);

			//��ȡ�ѿ���Ϣ;
			if (Heap32First(&HeapInfo, nPid, HeapListInfo.th32HeapID))
			{
				do
				{
					CString HID, BlockSize, Address, Flags;
					HID.Format(L"%d", HeapInfo.th32HeapID);
					BlockSize.Format(L"%d", HeapInfo.dwBlockSize);
					Address.Format(L"0x%x", HeapInfo.dwAddress);
					if (HeapInfo.dwFlags == LF32_FIXED)
					{
						Flags = L"��ʹ��";
					}
					else if (HeapInfo.dwFlags == LF32_MOVEABLE)
					{
						Flags = L"���ƶ�";
					}
					else
					{
						Flags = L"����";
					}
					m_PopList.AddItem(4, HID, BlockSize, Address, Flags);
				} while (Heap32Next(&HeapInfo));
			}
		} while (Heap32ListNext(hSnapShot, &HeapListInfo));
	}
	else
	{
		MessageBox(L"��ȡ��Ϣʧ��");
	}
}

void MenuDialog::OnRclickPoplist1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	//���߳��б����;
	if (!m_bFlag)
	{
		return;
	}

	//�߳��б�����ʾ�˵�;

	//��ȡ�����˵�;
	CMenu* pMenuThread = new CMenu;
	pMenuThread->LoadMenuW(IDR_MENU2);

	CMenu* pSubThread = pMenuThread->GetSubMenu(0);

	//��ȡ���λ��;
	CPoint point;
	GetCursorPos(&point);

	//����;
	pSubThread->TrackPopupMenu(TPM_LEFTALIGN, point.x, point.y, this, NULL);

	*pResult = 0;
}

void MenuDialog::OnMenuThread(UINT id)
{
	int nSel = m_PopList.GetSelectionMark();
	if (nSel < 0)
	{
		MessageBox(L"��ѡ������");
	}
	int PID = _wtoi(m_PopList.GetItemText(nSel, 0));

	HANDLE hThread = OpenThread(THREAD_ALL_ACCESS, FALSE, PID);

	switch (id)
	{
	case  ID_32779:
		if (SuspendThread(hThread) == -1)
		{
			MessageBox(L"�����߳�ʧ�ܣ�");
		}
		else
		{
			MessageBox(L"����ɹ������������1��");
			m_PopList.DeleteAllItems();
			ShowThread(m_nPID);
		}
		break;
	case ID_32780:
		if (ResumeThread(hThread) == -1)
		{
			MessageBox(L"�ָ��߳�ʧ�ܣ�");
		}
		else
		{
			MessageBox(L"�ָ��ɹ������������1");
			m_PopList.DeleteAllItems();
			ShowThread(m_nPID);
		}
		break;
	case ID_32781:
		if (!TerminateThread(hThread, 0))
		{
			MessageBox(L"�����߳�ʧ�ܣ�");
		}
		else
		{
			MessageBox(L"�����̳߳ɹ���");
			m_PopList.DeleteAllItems();
			ShowThread(m_nPID);
		}
		break;
	default:
		break;
	}
}

UINT MenuDialog::HeapThreadFunction(LPVOID lpParam)
{
	MenuDialog*  menuDia = (MenuDialog*)lpParam;
	menuDia->ShowHeap(getPid);
	AfxEndThread(0);
	return 0;
}
