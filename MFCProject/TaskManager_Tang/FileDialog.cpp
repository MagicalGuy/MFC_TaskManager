// FileDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "TaskManager_Tang.h"
#include "FileDialog.h"
#include "afxdialogex.h"

int count = 0;//记录文件列表第几行
// FileDialog 对话框

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
	ON_BN_CLICKED(IDC_BUTTON1, &FileDialog::OnBnClickedButton1)//查看文件按钮
	ON_NOTIFY(TVN_SELCHANGED, IDC_MFCSHELLTREE2, &FileDialog::OnTvnSelchangedMfcshelltree2)//选择数列表，暂时不用
END_MESSAGE_MAP()


// FileDialog 消息处理程序


void FileDialog::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	m_FileList.DeleteAllItems();
	count = 0;

	CString str;
	m_shellTreeCtrl.GetItemPath(str, m_shellTreeCtrl.GetSelectedItem());
	//MessageBox(NULL, str,NULL);
	FindingFile(str);
}

void FileDialog::FindingFile(CString filePath)
{
	CString m_StrFileName;		//文件名;
	CString m_StrType;			//类型;
	CString m_StrPath;			//路径;
	CString m_StrSize;			//大小;
	CString m_StrCreatTime;		//创建时间;
	CString m_StrChangeTime;	//修改时间;


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
			//插入文件列表
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

			//获取文件信息;	
			CFileStatus FileStatus;
			if (CFile::GetStatus(m_StrPath, FileStatus))
			{
				//m_StrFileName = FileStatus.m_szFullName;
				m_StrFileName = name;
				//创建时间
				m_StrCreatTime.Format(FileStatus.m_ctime.Format("%Y年%m月%d日  %H:%M:%S"));
				//修改时间
				m_StrChangeTime.Format(FileStatus.m_mtime.Format("%Y年%m月%d日  %H:%M:%S"));

				//文件大小;
				if (FileStatus.m_size / 1024 < 1)
					m_StrSize.Format(L"%d Byte", FileStatus.m_size);
				else if (FileStatus.m_size / 1024 / 1024 < 1)
					m_StrSize.Format(L"%d KB", FileStatus.m_size / 1024);
				else if (FileStatus.m_size / 1024 / 1024 / 1024 < 1)
					m_StrSize.Format(L"%d MB", FileStatus.m_size / 1024 / 1024);
				else if (FileStatus.m_size / 1024 / 1024 / 1024 / 1024 < 1)
					m_StrSize.Format(L"%d GB", FileStatus.m_size / 1024 / 1024 / 1024);

				//获取文件类型，暂时不用
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

	// TODO:  在此添加额外的初始化

	DWORD dwOldStyle = m_FileList.GetExtendedStyle();
	m_FileList.SetExtendedStyle(dwOldStyle | LVS_EX_FULLROWSELECT);
	CRect rc;
	m_FileList.GetClientRect(rc);
	int nWidth = rc.Width();
	m_FileList.InsertColumn(0, L"文件名", 0, nWidth / 5);
	m_FileList.InsertColumn(1, L"创建时间", 0, nWidth / 3);
	m_FileList.InsertColumn(2, L"修改时间", 0, nWidth / 3);
	m_FileList.InsertColumn(3, L"文件大小", 0, nWidth / 3);


	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void FileDialog::OnTvnSelchangedMfcshelltree2(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
}