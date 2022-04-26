// EstimateLightPowerDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "PowerMeter.h"
#include "EstimateLightPowerDlg.h"
#include "afxdialogex.h"


// CEstimateLightPowerDlg �Ի���

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


// CEstimateLightPowerDlg ��Ϣ�������


void CEstimateLightPowerDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData();
	CDialogEx::OnOK();
}
