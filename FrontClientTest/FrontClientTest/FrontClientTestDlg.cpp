
// FrontClientTestDlg.cpp : 实现文件
//

#include "stdafx.h"
//#include "ManagerMarket.h"
#include "FrontClientTest.h"
#include "FrontClientTestDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CFrontClientTestDlg 对话框




CFrontClientTestDlg::CFrontClientTestDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFrontClientTestDlg::IDD, pParent)
	, m_csBody(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CFrontClientTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_Body, m_csBody);
}

BEGIN_MESSAGE_MAP(CFrontClientTestDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_LOGIN, &CFrontClientTestDlg::OnBnClickedButtonLogin)
	ON_BN_CLICKED(IDC_BUTTON_LOGIN2, &CFrontClientTestDlg::OnBnClickedButtonLogin2)
END_MESSAGE_MAP()


// CFrontClientTestDlg 消息处理程序

BOOL CFrontClientTestDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	GetDlgItem(IDC_EDIT_IP)->SetWindowText(_T("192.168.44.223"));
	GetDlgItem(IDC_EDIT_PORT)->SetWindowText(_T("4097"));
	GetDlgItem(IDC_EDIT_USERNAME)->SetWindowText(_T("18756596872"));
	GetDlgItem(IDC_EDIT_PWD)->SetWindowText(_T("123456"));
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CFrontClientTestDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CFrontClientTestDlg::OnPaint()
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
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CFrontClientTestDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CFrontClientTestDlg::OnClose()
{
	CManagerMarket::GetInstance()->Stop();
	CDialog::OnClose();
}

void CFrontClientTestDlg::OnBnClickedButtonLogin()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	CString csIp;
	CString csPort;
	GetDlgItem(IDC_EDIT_IP)->GetWindowText(csIp);
	GetDlgItem(IDC_EDIT_PORT)->GetWindowText(csPort);
	string sIp(csIp.GetBuffer());
	string sPort(csPort.GetBuffer());
	CManagerMarket::GetInstance()->SetNotifyWnd(this->GetSafeHwnd());
	CManagerMarket::GetInstance()->Start();
	if(CManagerMarket::GetInstance()->ConnetToTS(sIp.c_str(),atoi(sPort.c_str())))
	{
		m_csBody += _T("服务器连接成功!\r\n"); 
	}
	else
	{
		m_csBody += _T("服务器连接失败!\r\n"); 
	}
	csIp.ReleaseBuffer();
	csPort.ReleaseBuffer();
	UpdateData(FALSE);
}

void CFrontClientTestDlg::OnBnClickedButtonLogin2()
{
	// TODO: 在此添加控件通知处理程序代码
	CString csName;
	CString csPwd;
	GetDlgItem(IDC_EDIT_USERNAME)->GetWindowText(csName);
	GetDlgItem(IDC_EDIT_PWD)->GetWindowText(csPwd);

	//用户名 base64 加密
	CString strEncryptUserName = GW_Encrypt::BASE64Encrypt(csName, csName.GetLength());

	//密码 MD5加密
	CString strEncryptPWD = csPwd;
	GW_Encrypt::MD5Encrypt(csPwd, strEncryptPWD, csPwd.GetLength());
	//截取18位，然后进行 base64加密
	CString strOrgMD5PWD = strEncryptPWD;
	csPwd = strEncryptPWD.Left(18);
	strEncryptPWD = GW_Encrypt::BASE64Encrypt(csPwd, csPwd.GetLength());

	string sIp(strEncryptUserName.GetBuffer());
	string sPort(strEncryptPWD.GetBuffer());

	CManagerMarket::GetInstance()->Login(sIp.c_str(),sPort.c_str());
	csName.ReleaseBuffer();
	csPwd.ReleaseBuffer();
}

LRESULT CFrontClientTestDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	int wpara = wParam;
	switch(message)
	{
	case GWTS_CMD_LOGIN:
		{ 
			UpdateData(TRUE);
			CString csTr = (char*)lParam;
			if (wParam == 1)
			{
				m_csBody += _T("发送信息：")+csTr+_T("\r\n");
			}
			else
			{
				m_csBody += _T("接收信息：")+csTr+_T("\r\n");
			}
			UpdateData(FALSE);
			break;
		}
	case 10000:
		{
			UpdateData(TRUE);
			CString csMsg;
			csMsg.Format("%d",wParam);
			m_csBody += _T("总消耗时间：")+csMsg+_T("\r\n");
			UpdateData(FALSE);
			break;
		}
	default:
		break;
	}
	return CDialog::WindowProc(message,wParam,lParam);
}
