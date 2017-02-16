
// FrontClientTestDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
//#include "ManagerMarket.h"
#include "FrontClientTest.h"
#include "FrontClientTestDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// CFrontClientTestDlg �Ի���




CFrontClientTestDlg::CFrontClientTestDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFrontClientTestDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CFrontClientTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CFrontClientTestDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_LOGIN, &CFrontClientTestDlg::OnBnClickedButtonLogin)
	ON_BN_CLICKED(IDC_BUTTON_LOGIN2, &CFrontClientTestDlg::OnBnClickedButtonLogin2)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CFrontClientTestDlg ��Ϣ�������

BOOL CFrontClientTestDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	GetDlgItem(IDC_EDIT_IP)->SetWindowText(_T("192.168.44.223"));
	GetDlgItem(IDC_EDIT_PORT)->SetWindowText(_T("4096"));
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CFrontClientTestDlg::OnPaint()
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
		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString csIp;
	CString csPort;
	CString csTotal;
	GetDlgItem(IDC_EDIT_IP)->GetWindowText(csIp);
	GetDlgItem(IDC_EDIT_PORT)->GetWindowText(csPort);
	GetDlgItem(IDC_EDIT_TotalNum)->GetWindowText(csTotal);
	string sIp(csIp.GetBuffer());
	string sPort(csPort.GetBuffer());
	string sTotal(csTotal.GetBuffer());
	CManagerMarket::GetInstance()->Start();
	CManagerMarket::GetInstance()->ConnetToTS(sIp.c_str(),atoi(sPort.c_str()),atoi(sTotal.c_str()));
	csIp.ReleaseBuffer();
	csPort.ReleaseBuffer();
}

void CFrontClientTestDlg::OnBnClickedButtonLogin2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString csName;
	CString csPwd;
	GetDlgItem(IDC_EDIT_USERNAME)->GetWindowText(csName);
	GetDlgItem(IDC_EDIT_PWD)->GetWindowText(csPwd);
	string sIp(csName.GetBuffer());
	string sPort(csPwd.GetBuffer());
	CManagerMarket::GetInstance()->Login(sIp.c_str(),sPort.c_str());
	csName.ReleaseBuffer();
	csPwd.ReleaseBuffer();
}
