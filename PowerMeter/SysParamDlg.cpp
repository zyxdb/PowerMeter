// SysParamDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "PowerMeter.h"
#include "SysParamDlg.h"
#include "afxdialogex.h"
#include "MD5/md5file.h"
#include <string>

using namespace std;
// CSysParamDlg 对话框

IMPLEMENT_DYNAMIC(CSysParamDlg, CDialogEx)

CSysParamDlg::CSysParamDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_SYSPARAM_DIALOG, pParent)
	, m_dbHeaterResHeat(100)
	, m_dbHeaterStability(0.00001)
	, m_dbHeaterStableTime(10)
	, m_dbHeaterResCheck(100)
	, m_dbHeaterPowerStep(0.0001)
	, m_dbPidStability(0.00001)
	, m_dbPidStableTime(10)
	, m_dbPidParamA(1)
	, m_dbPidParamB(1)
	, m_dbPidParamC(1)
{

}

CSysParamDlg::~CSysParamDlg()
{
}

void CSysParamDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_RES_HEATER, m_dbHeaterResHeat);
	DDX_Control(pDX, IDC_COMBO_CURRENTSOURCE_CONNECT, m_cComboCurrentSource);
	DDX_Control(pDX, IDC_COMBO_MULTIMETER_CONNECT, m_cComboMultimeter);
	DDX_Control(pDX, IDC_COMBO_PDA, m_cComboPDA);
	DDX_Control(pDX, IDC_COMBO_SHUTTER, m_cComboShutter);
	DDX_Text(pDX, IDC_EDIT_HEATER_STABLITY, m_dbHeaterStability);
	DDX_Text(pDX, IDC_EDIT_HEATER_STABLE_TIME, m_dbHeaterStableTime);
	DDX_Text(pDX, IDC_EDIT_RES_CHECK, m_dbHeaterResCheck);
	DDX_Text(pDX, IDC_EDIT_POWER_STEP, m_dbHeaterPowerStep);
	DDX_Text(pDX, IDC_EDIT_PID_STABILITY, m_dbPidStability);
	DDX_Text(pDX, IDC_EDIT_PID_STABLE_TIME, m_dbPidStableTime);
	DDX_Text(pDX, IDC_EDIT_PID_PARAMA, m_dbPidParamA);
	DDX_Text(pDX, IDC_EDIT_PID_PARAMB, m_dbPidParamB);
	DDX_Text(pDX, IDC_EDIT_PID_PARAMC, m_dbPidParamC);
	DDX_Control(pDX, IDC_COMBO_EXTCTRL, m_cComboExtCtrl);
}


BEGIN_MESSAGE_MAP(CSysParamDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CSysParamDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CSysParamDlg 消息处理程序


void CSysParamDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	//向D盘写入程序数据库连接ini文件信息，默认设置如下
	//CFileFind finder;   //查找是否存在ini文件，若不存在，则生成一个新的默认设置的ini文件，这样就保证了我们更改后的设置每次都可用
	//BOOL ifFind = finder.FindFile(_T(".//config.ini"));
	//if (!ifFind)
	//{
	//}
	/*
	str.Format(_T("%f"), m_fSampleFreq);
	::WritePrivateProfileStringW(_T("System Parameters"), _T("SampleFreq"), str, _T(".//config.ini"));
	str.Format(_T("%f"), m_fPowerSpan);
	::WritePrivateProfileStringW(_T("System Parameters"), _T("PowerSpan"), str, _T(".//config.ini"));
	str.Format(_T("%f"), m_dbHeaterResHeat);
	::WritePrivateProfileStringW(_T("System Parameters"), _T("ResHeater"), str, _T(".//config.ini"));
	str.Format(_T("%f"), m_fRes);
	::WritePrivateProfileStringW(_T("System Parameters"), _T("Res"), str, _T(".//config.ini"));
	str.Format(_T("%f"), m_fStability);
	::WritePrivateProfileStringW(_T("System Parameters"), _T("Stability"), str, _T(".//config.ini"));
	str.Format(_T("%f"), m_fDataLong);
	::WritePrivateProfileStringW(_T("System Parameters"), _T("DataLong"), str, _T(".//config.ini"));
	str.Format(_T("%f"), m_fParamA);
	::WritePrivateProfileStringW(_T("System Parameters"), _T("ParamA"), str, _T(".//config.ini"));
	str.Format(_T("%f"), m_fParamB);
	::WritePrivateProfileStringW(_T("System Parameters"), _T("ParamB"), str, _T(".//config.ini"));
	str.Format(_T("%f"), m_fParamC);
	::WritePrivateProfileStringW(_T("System Parameters"), _T("ParamC"), str, _T(".//config.ini"));

	int num = m_cComboCurrentSource.GetCurSel();
	str.Format(_T("%d"), num);
	::WritePrivateProfileStringW(_T("System Parameters"), _T("ComboCurren"), str, _T(".//config.ini"));
	num = m_cComboMultimeter.GetCurSel();
	str.Format(_T("%d"), num);
	::WritePrivateProfileStringW(_T("System Parameters"), _T("ComboMultimeter"), str, _T(".//config.ini"));
	num = m_cComboPDA.GetCurSel();
	str.Format(_T("%d"), num);
	::WritePrivateProfileStringW(_T("System Parameters"), _T("ComboPDA"), str, _T(".//config.ini"));
	num = m_cComboShutter.GetCurSel();
	str.Format(_T("%d"), num);
	::WritePrivateProfileStringW(_T("System Parameters"), _T("ComboShutter"), str, _T(".//config.ini"));

	string FileMD5 = getFileMD5(".//config.ini");
	str = FileMD5.c_str();
	::WritePrivateProfileStringW(_T("MD5"), _T("MD5"), str, _T(".//MD5.ini"));
	*/
	m_cComboCurrentSource.GetWindowText(m_csComPortCurrentSource);
	m_cComboMultimeter.GetWindowText(m_csComMultimeter);
	m_cComboPDA.GetWindowText(m_csComPortPDA);
	m_cComboShutter.GetWindowText(m_csComPortShutter);
	m_cComboExtCtrl.GetWindowText(m_csComPortExtCtrl);
	CDialogEx::OnOK();
}


BOOL CSysParamDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	CString cStr;
	int i;
	for (i = 1; i <= 16; i++)
	{
		cStr.Format(_T("COM%d"), i);
		 m_cComboCurrentSource.AddString(cStr);
		 m_cComboMultimeter.AddString(cStr);
		 m_cComboPDA.AddString(cStr);
		 m_cComboShutter.AddString(cStr);
		 m_cComboExtCtrl.AddString(cStr);
	}

	i = m_cComboCurrentSource.FindString(-1, m_csComPortCurrentSource);
	m_cComboCurrentSource.SetCurSel(i);

	i = m_cComboMultimeter.FindString(-1, m_csComMultimeter);
	m_cComboMultimeter.SetCurSel(i);

	i = m_cComboPDA.FindString(-1, m_csComPortPDA);
	m_cComboPDA.SetCurSel(i);

	i = m_cComboShutter.FindString(-1, m_csComPortShutter);
	m_cComboShutter.SetCurSel(i);

	i = m_cComboExtCtrl.FindString(-1, m_csComPortExtCtrl);
	m_cComboExtCtrl.SetCurSel(i);

	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

