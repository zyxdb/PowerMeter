// DlgAbout.cpp : 实现文件
//

#include "stdafx.h"
#include "PowerMeter.h"
#include "DlgAbout.h"
#include "afxdialogex.h"


// CDlgAbout 对话框

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


// CDlgAbout 消息处理程序


void CDlgAbout::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
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

	// TODO:  在此添加额外的初始化
	if(m_iCloseDelay>0)  	SetTimer(IDEVENT_CLOSE, m_iCloseDelay, NULL);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}
