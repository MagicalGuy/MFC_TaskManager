// CleanDialog.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "TaskManager_Tang.h"
#include "CleanDialog.h"
#include "afxdialogex.h"


// CleanDialog �Ի���

IMPLEMENT_DYNAMIC(CleanDialog, CDialogEx)

CleanDialog::CleanDialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG5, pParent)
	, m_DirName(_T("")) {}

CleanDialog::~CleanDialog() {}

void CleanDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_CLEAN, m_DirName);
	DDX_Control(pDX, IDC_MFCSHELLTREE1, m_shellTreeCtrl1);
	DDX_Control(pDX, IDC_EDIT_CLEAN, m_CleanEdit);
}


BEGIN_MESSAGE_MAP(CleanDialog, CDialogEx)
	ON_WM_DROPFILES()
	ON_BN_CLICKED(IDC_BUTTON2, &CleanDialog::OnBnClickedButton2)
END_MESSAGE_MAP()


// CleanDialog ��Ϣ�������


void CleanDialog::OnDropFiles(HDROP hDropInfo)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ


	UpdateData(TRUE);
	TCHAR FileName[MAX_PATH];

	DragQueryFile(hDropInfo, 0, FileName, 300); //ȡ���ļ���		

	m_shellTreeCtrl1.SelectPath(FileName);
	m_DirName = FileName;

	UpdateData(FALSE);

	//MessageBox(FileName);
	CDialogEx::OnDropFiles(hDropInfo);
}


//ɾ������
void CleanDialog::OnBnClickedButton2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	//��ȡ·��;
	UpdateData(TRUE);

	//��ȡ���б��·��
	CString str;
	m_shellTreeCtrl1.GetItemPath(str, m_shellTreeCtrl1.GetSelectedItem());


	CFileFind finder;
	CString string;
	string = str + L"\\*.*";
	BOOL bwork = finder.FindFile(str);
	finder.FindNextFile();
	//��ȡ�ļ��������ļ�����
	CString str1 = finder.GetFileName();


	if (m_DirName.IsEmpty() && (str1.Compare(L"Desktop") == 0)) {
		MessageBox(L"��ѡ��Ҫɾ�����ļ�");
		//return;
	}
	else {

		CStringA string;

		string.Format("��ȷ����ȷ��ɾ��\n��ȡ����ȡ��ɾ��\n");

		//����;
		int mBoxID = MessageBoxA(NULL, string, "ɾ������,�������", MB_OKCANCEL);
		if (mBoxID == IDOK) {

			if (m_DirName.IsEmpty() && (str1.Compare(L"Desktop") != 0)) {
				DeleFile(str);
				MessageBoxA(NULL, "ɾ�����", "ɾ������", 0);
			}

			if (!m_DirName.IsEmpty() && (str1.Compare(L"Desktop") != 0) || !m_DirName.IsEmpty() && (str1.Compare(L"Desktop") == 0)) {

				if (DeleFile(m_DirName)) {
					MessageBox(L"�������");
				}
				else {
					MessageBox(L"����ʧ��");
				}

			}

		}
		//ȡ��
		else if (mBoxID == IDCANCEL) {}

	}
}


BOOL CleanDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	//������ҷ�ļ�;
	ChangeWindowMessageFilterEx(this->m_hWnd, WM_DROPFILES, MSGFLT_ALLOW, NULL);
	ChangeWindowMessageFilterEx(this->m_hWnd, 0x0049, MSGFLT_ALLOW, NULL);		// 0x0049 == WM_COPYGLOBALDATA

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}

bool CleanDialog::DeleFile(CString fileName)
{
	if (fileName.IsEmpty()) {
		MessageBox(L"·������");
		return false;
	}


	CFileFind finder;
	CString string;
	string = fileName + L"\\*.*";

	BOOL bwork = finder.FindFile(string);
	while (bwork)
	{
		bwork = finder.FindNextFile();
		if ((finder.IsDots() == FALSE) && finder.IsDirectory())
		{
			CString name = finder.GetFileName();
			if (wcscmp(name, L".vs") == 0 || wcscmp(name, L"Debug") == 0 ||
				wcscmp(name, L"ipch") == 0 || wcscmp(name, L"x64") == 0 || wcscmp(name, L"Release") == 0)
			{
				//ɾ���ļ��м�������
				DeleDirectory(finder.GetFilePath());
			}
			else

				DeleFile(finder.GetFilePath());//�ݹ�
		}
		else
		{
			CString name = finder.GetFileName();
			LPWSTR File = PathFindExtension(name);//��ȡ��׺��
			if (wcscmp(File, L".db") == 0 || wcscmp(File, L".vs") == 0)
			{
				//ɾ���ļ�
				DeleteFile(finder.GetFilePath());
			}
		}
	}

	return true;
}

void CleanDialog::DeleDirectory(CString strDir)
{
	if (strDir.IsEmpty()) {
		RemoveDirectory(strDir);
		return;
	}

	//����ɾ���ļ������ļ��� 
	CFileFind   ff;

	BOOL bFound = ff.FindFile(strDir + _T("\\*"), 0);
	while (bFound) {
		bFound = ff.FindNextFile();
		if (ff.GetFileName() == _T(".") || ff.GetFileName() == _T(".."))
			continue;
		//ȥ���ļ�(��)ֻ�������� 
		SetFileAttributes(ff.GetFilePath(), FILE_ATTRIBUTE_NORMAL);
		if (ff.IsDirectory()) {
			//�ݹ�ɾ�����ļ��� 
			DeleDirectory(ff.GetFilePath());
			RemoveDirectory(ff.GetFilePath());
		}
		else {
			DeleteFile(ff.GetFilePath());   //ɾ���ļ� 
		}
	}
	ff.Close();
	//Ȼ��ɾ�����ļ��� 
	RemoveDirectory(strDir);
}
