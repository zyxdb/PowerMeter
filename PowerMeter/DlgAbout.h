#pragma once


// CDlgAbout �Ի���
#define IDEVENT_CLOSE WM_USER+100
class CDlgAbout : public CDialog
{
	DECLARE_DYNAMIC(CDlgAbout)

public:
	CDlgAbout(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgAbout();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_ABOUT };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	int m_iCloseDelay;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	virtual BOOL OnInitDialog();
};
