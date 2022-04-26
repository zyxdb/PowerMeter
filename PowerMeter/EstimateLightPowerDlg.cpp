// EstimateLightPowerDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "PowerMeter.h"
#include "EstimateLightPowerDlg.h"
#include "afxdialogex.h"


// CEstimateLightPowerDlg 对话框

IMPLEMENT_DYNAMIC(CEstimateLightPowerDlg, CDialogEx)

CEstimateLightPowerDlg::CEstimateLightPowerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_POWER_ESTIMATED, pParent)
	, m_dbLightPowerEstimated(0)
{

}

CEstimateLightPowerDlg::~CEstimateLightPowerDlg()
{
}

void CEstimateLightPowerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_POWER_ESTIMATED, m_dbLightPowerEstimated);
}


BEGIN_MESSAGE_MAP(CEstimateLightPowerDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CEstimateLightPowerDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CEstimateLightPowerDlg 消息处理程序


void CEstimateLightPowerDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();
	CDialogEx::OnOK();
}
