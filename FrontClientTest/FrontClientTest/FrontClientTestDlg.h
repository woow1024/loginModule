
// FrontClientTestDlg.h : ͷ�ļ�
//

#pragma once


// CFrontClientTestDlg �Ի���
class CFrontClientTestDlg : public CDialog
{
// ����
public:
	CFrontClientTestDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_FRONTCLIENTTEST_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnClose();
	afx_msg void OnBnClickedButtonLogin();
	afx_msg void OnBnClickedButtonLogin2();
	afx_msg LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	CString m_csBody;
};
