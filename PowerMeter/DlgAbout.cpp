// DlgAbout.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "PowerMeter.h"
#include "DlgAbout.h"
#include "afxdialogex.h"


// CDlgAbout �Ի���

IMPLEMENT_DYNAMIC(CDlgAbout, CDialog)

CDlgAbout::CDlgAbout(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_DIALOG_ABOUT, pParent)
{
	m_iCloseDelay = -1;
}

CDlgAbout::~CDlgAbout()
{
}

void CDlgAbout::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgAbout, CDialog)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CDlgAbout ��Ϣ�������


void CDlgAbout::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if (nIDEvent == IDEVENT_CLOSE)
	{
		KillTimer(nIDEvent);
		PostMessage(WM_CLOSE);
	}
	CDialog::OnTimer(nIDEvent);
}


BOOL CDlgAbout::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	if(m_iCloseDelay>0)  	SetTimer(IDEVENT_CLOSE, m_iCloseDelay, NULL);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}
