// CleanDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "TaskManager_Tang.h"
#include "CleanDialog.h"
#include "afxdialogex.h"


// CleanDialog 对话框

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


// CleanDialog 消息处理程序


void CleanDialog::OnDropFiles(HDROP hDropInfo)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值


	UpdateData(TRUE);
	TCHAR FileName[MAX_PATH];

	DragQueryFile(hDropInfo, 0, FileName, 300); //取得文件名		

	m_shellTreeCtrl1.SelectPath(FileName);
	m_DirName = FileName;

	UpdateData(FALSE);

	//MessageBox(FileName);
	CDialogEx::OnDropFiles(hDropInfo);
}


//删除操作
void CleanDialog::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码

	//获取路径;
	UpdateData(TRUE);

	//获取树列表的路径
	CString str;
	m_shellTreeCtrl1.GetItemPath(str, m_shellTreeCtrl1.GetSelectedItem());


	CFileFind finder;
	CString string;
	string = str + L"\\*.*";
	BOOL bwork = finder.FindFile(str);
	finder.FindNextFile();
	//获取文件名或者文件夹名
	CString str1 = finder.GetFileName();


	if (m_DirName.IsEmpty() && (str1.Compare(L"Desktop") == 0)) {
		MessageBox(L"请选择要删除的文件");
		//return;
	}
	else {

		CStringA string;

		string.Format("【确定】确定删除\n【取消】取消删除\n");

		//弹窗;
		int mBoxID = MessageBoxA(NULL, string, "删除操作,请谨慎！", MB_OKCANCEL);
		if (mBoxID == IDOK) {

			if (m_DirName.IsEmpty() && (str1.Compare(L"Desktop") != 0)) {
				DeleFile(str);
				MessageBoxA(NULL, "删除完成", "删除操作", 0);
			}

			if (!m_DirName.IsEmpty() && (str1.Compare(L"Desktop") != 0) || !m_DirName.IsEmpty() && (str1.Compare(L"Desktop") == 0)) {

				if (DeleFile(m_DirName)) {
					MessageBox(L"清理完成");
				}
				else {
					MessageBox(L"清理失败");
				}

			}

		}
		//取消
		else if (mBoxID == IDCANCEL) {}

	}
}


BOOL CleanDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	//允许拖曳文件;
	ChangeWindowMessageFilterEx(this->m_hWnd, WM_DROPFILES, MSGFLT_ALLOW, NULL);
	ChangeWindowMessageFilterEx(this->m_hWnd, 0x0049, MSGFLT_ALLOW, NULL);		// 0x0049 == WM_COPYGLOBALDATA

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

bool CleanDialog::DeleFile(CString fileName)
{
	if (fileName.IsEmpty()) {
		MessageBox(L"路径有误");
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
				//删除文件夹及其内容
				DeleDirectory(finder.GetFilePath());
			}
			else

				DeleFile(finder.GetFilePath());//递归
		}
		else
		{
			CString name = finder.GetFileName();
			LPWSTR File = PathFindExtension(name);//获取后缀名
			if (wcscmp(File, L".db") == 0 || wcscmp(File, L".vs") == 0)
			{
				//删除文件
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

	//首先删除文件及子文件夹 
	CFileFind   ff;

	BOOL bFound = ff.FindFile(strDir + _T("\\*"), 0);
	while (bFound) {
		bFound = ff.FindNextFile();
		if (ff.GetFileName() == _T(".") || ff.GetFileName() == _T(".."))
			continue;
		//去掉文件(夹)只读等属性 
		SetFileAttributes(ff.GetFilePath(), FILE_ATTRIBUTE_NORMAL);
		if (ff.IsDirectory()) {
			//递归删除子文件夹 
			DeleDirectory(ff.GetFilePath());
			RemoveDirectory(ff.GetFilePath());
		}
		else {
			DeleteFile(ff.GetFilePath());   //删除文件 
		}
	}
	ff.Close();
	//然后删除该文件夹 
	RemoveDirectory(strDir);
}
