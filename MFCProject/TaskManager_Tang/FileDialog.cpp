// FileDialog.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "TaskManager_Tang.h"
#include "FileDialog.h"
#include "afxdialogex.h"

int count = 0;//��¼�ļ��б�ڼ���
// FileDialog �Ի���

IMPLEMENT_DYNAMIC(FileDialog, CDialogEx)

FileDialog::FileDialog(CWnd* pParent /*=NULL*/) : CDialogEx(IDD_DIALOG3, pParent) {}

FileDialog::~FileDialog() {}

void FileDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_FILELIST1, m_FileList);
	DDX_Control(pDX, IDC_MFCSHELLTREE2, m_shellTreeCtrl);
}


BEGIN_MESSAGE_MAP(FileDialog, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &FileDialog::OnBnClickedButton1)//�鿴�ļ���ť
	ON_NOTIFY(TVN_SELCHANGED, IDC_MFCSHELLTREE2, &FileDialog::OnTvnSelchangedMfcshelltree2)//ѡ�����б���ʱ����
END_MESSAGE_MAP()


// FileDialog ��Ϣ�������


void FileDialog::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_FileList.DeleteAllItems();
	count = 0;

	CString str;
	m_shellTreeCtrl.GetItemPath(str, m_shellTreeCtrl.GetSelectedItem());
	//MessageBox(NULL, str,NULL);
	FindingFile(str);
}

void FileDialog::FindingFile(CString filePath)
{
	CString m_StrFileName;		//�ļ���;
	CString m_StrType;			//����;
	CString m_StrPath;			//·��;
	CString m_StrSize;			//��С;
	CString m_StrCreatTime;		//����ʱ��;
	CString m_StrChangeTime;	//�޸�ʱ��;


	CFileFind finder;
	CString string;
	string = filePath + L"\\*.*";

	BOOL bwork = finder.FindFile(string);
	while (bwork)
	{
		bwork = finder.FindNextFile();

		if (finder.GetFileName() == _T(".") || finder.GetFileName() == _T(".."))
			continue;


		if ((finder.IsDots() == FALSE) && finder.IsDirectory())
		{
			//�����ļ��б�
			m_StrFileName = finder.GetFileName();
			m_FileList.InsertItem(count, m_StrFileName);

			m_FileList.SetItemText(count, 1, m_StrCreatTime);
			m_FileList.SetItemText(count, 2, m_StrChangeTime);
			m_FileList.SetItemText(count, 3, m_StrSize);

		}
		else
		{
			CString name = finder.GetFileName();
			m_StrPath = finder.GetFilePath();

			//��ȡ�ļ���Ϣ;	
			CFileStatus FileStatus;
			if (CFile::GetStatus(m_StrPath, FileStatus))
			{
				//m_StrFileName = FileStatus.m_szFullName;
				m_StrFileName = name;
				//����ʱ��
				m_StrCreatTime.Format(FileStatus.m_ctime.Format("%Y��%m��%d��  %H:%M:%S"));
				//�޸�ʱ��
				m_StrChangeTime.Format(FileStatus.m_mtime.Format("%Y��%m��%d��  %H:%M:%S"));

				//�ļ���С;
				if (FileStatus.m_size / 1024 < 1)
					m_StrSize.Format(L"%d Byte", FileStatus.m_size);
				else if (FileStatus.m_size / 1024 / 1024 < 1)
					m_StrSize.Format(L"%d KB", FileStatus.m_size / 1024);
				else if (FileStatus.m_size / 1024 / 1024 / 1024 < 1)
					m_StrSize.Format(L"%d MB", FileStatus.m_size / 1024 / 1024);
				else if (FileStatus.m_size / 1024 / 1024 / 1024 / 1024 < 1)
					m_StrSize.Format(L"%d GB", FileStatus.m_size / 1024 / 1024 / 1024);

				//��ȡ�ļ����ͣ���ʱ����
				SHFILEINFO FileInfo = {};
				SHGetFileInfo(m_StrPath, 0, &FileInfo, sizeof(SHFILEINFO), SHGFI_TYPENAME);
				m_StrType = FileInfo.szTypeName;

				m_FileList.InsertItem(count, m_StrFileName);

				m_FileList.SetItemText(count, 1, m_StrCreatTime);
				m_FileList.SetItemText(count, 2, m_StrChangeTime);
				m_FileList.SetItemText(count, 3, m_StrSize);

			}
		}
	}
}


BOOL FileDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	DWORD dwOldStyle = m_FileList.GetExtendedStyle();
	m_FileList.SetExtendedStyle(dwOldStyle | LVS_EX_FULLROWSELECT);
	CRect rc;
	m_FileList.GetClientRect(rc);
	int nWidth = rc.Width();
	m_FileList.InsertColumn(0, L"�ļ���", 0, nWidth / 5);
	m_FileList.InsertColumn(1, L"����ʱ��", 0, nWidth / 3);
	m_FileList.InsertColumn(2, L"�޸�ʱ��", 0, nWidth / 3);
	m_FileList.InsertColumn(3, L"�ļ���С", 0, nWidth / 3);


	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}


void FileDialog::OnTvnSelchangedMfcshelltree2(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;
}