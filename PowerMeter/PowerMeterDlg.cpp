// PowerMeterDlg.cpp : ʵ���ļ�
//

#pragma once
#include "stdafx.h"
#include <Windows.h>
#include "PowerMeter.h"
#include "PowerMeterDlg.h"
#include "PowerMeterLib.h"
#include "afxdialogex.h"
#include "ChartCtrl\ChartTitle.h"
#include "ChartCtrl\ChartLineSerie.h"
#include "ChartCtrl\ChartAxisLabel.h"
#include "SysParamDlg.h"
#include "EstimateLightPowerDlg.h"
#include "GPIB\ni4882.h"
#include <fstream>
#include <sstream>
#include<numeric>

//static void GPIBCleanup(int Dev, const char * ErrorMsg);

// CPowerMeterDlg �Ի���

void save(CPowerMeterDlg *pdlgMain) {
	CTime time = CTime::GetCurrentTime(); ///����CTime����
	CString m_strTime = time.Format("%Y_%m%d_%H%M");

	CString csFileName;

	// ��ȡ��ǰexe�ļ�·������path����
	char exeFullPath[MAX_PATH]; // Full path
	std::string strPath = "";
	GetModuleFileNameA(NULL, exeFullPath, MAX_PATH);
	strPath = (std::string)exeFullPath;    // Get full path of the file
	int pos = strPath.find_last_of('\\', strPath.length());
	std::string path = strPath.substr(0, pos) + "\\";  // Return the directory without the file name

												  //csFileName = cFileDialog.GetPathName();
	csFileName = path.c_str();
	csFileName.AppendFormat(m_strTime);
	// �ж��Ƿ��������ļ��У�û�еĻ����½�һ���ļ���
	if (!PathIsDirectory(csFileName))
	{
		::CreateDirectory(csFileName, 0);
	}
	csFileName.AppendFormat(L"\\");
	csFileName.AppendFormat(m_strTime);
	TChartString fileName;
	CRect crect;
	CString pic_path = csFileName;
	//int q = csFileName.Find('.');
	//w = csFileName.Left(q);
	// ����ͼƬ�Ĳ���
	pic_path.AppendFormat(L".png");
	fileName = pic_path;
	crect.SetRect(0, 0, 1920, 1080);                      //set the size of the image
	pdlgMain->m_ChartCtrl.SaveAsImage(fileName, crect, 32, GUID_NULL); //savethe image

	// ����csv�ļ��Ĳ���
	csFileName.AppendFormat(L".csv");
	std::ofstream outFile;
	outFile.open(csFileName, std::ios::out); // ��ģʽ��ʡ��  
	outFile << "���" << ',' << "��ѹֵ/mV" << ',' << "����ֵ/mA" << "," <<  "����" << std::endl;
	for (int i = 0; i < pdlgMain->xpAxisNums.size() - 1; i++)
	{
		outFile << pdlgMain->xpAxisNums[i] << ',' << pdlgMain->voltageNums[i] << ',' << pdlgMain->currentNums[i] << ',' << pdlgMain->dcNums[i] << std::endl;
	}
	outFile.close();
}

IMPLEMENT_DYNAMIC(CPowerMeterDlg, CDialogEx)
CPowerMeterDlg *pPSDlg = NULL;

CPowerMeterDlg::CPowerMeterDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_POWERMETER_DIALOG, pParent)
	, m_dbHeaterResHeat(0)
	, m_csResult(_T("��ӭʹ��"))
	, m_dbLightPowerEstimate(0)
	, m_dbLightPowerMeasue(0)
	, m_dbLightTemperature(0)
	, m_dbElectrHighTemperature(0)
	, m_dbElectrHighVoltage(0)
	, m_dbElectrHighCurrent(0)
	, m_dbElectrHighPower(0)
	, m_dbElectrLowTemperature(0)
	, m_dbElectrLowVoltage(0)
	, m_dbElectrLowCurrent(0)
	, m_dbElectrLowPower(0)
	, m_dbTemperature(0)
	, m_dbVoltage(0)
	, m_dbOutputCurrent(0)
	, m_bCheckElectricBridge(TRUE)
	, m_bCheckVoltage(TRUE)
	, m_bCheckOutputCurrent(TRUE)
{
	m_iSelfChecking = 0;//Idel
	m_bInMesuring = false;
}

CPowerMeterDlg::~CPowerMeterDlg()
{
	CString csValue;
	csValue.Format(_T("%lf"), m_dbLightPowerEstimate);
    WritePrivateProfileString(_T("Light"), _T("PowerEstimated"), csValue, _T(".\\PowerMeter.ini"));
}

void CPowerMeterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_ChartCtrl, m_ChartCtrl);
	DDX_Control(pDX, IDC_STATIC_LOGO, m_cStaticLogo);
	DDX_Text(pDX, IDC_STATIC_RESULT, m_csResult);
	DDX_Text(pDX, IDC_EDIT_LIGHT_POWER_ESTIMATE, m_dbLightPowerEstimate);
	DDX_Text(pDX, IDC_EDIT_LIGHT_POWER_MEASUR, m_dbLightPowerMeasue);
	DDX_Text(pDX, IDC_EDIT_LIGHT_TEMPERATURE, m_dbLightTemperature);
	DDX_Text(pDX, IDC_EDIT_ELECTR_HIGH_TEMPERATURE, m_dbElectrHighTemperature);
	DDX_Text(pDX, IDC_EDIT_ELECTR_HIGH_VOLTAGE, m_dbElectrHighVoltage);
	DDX_Text(pDX, IDC_EDIT_ELECTR_HIGH_CURRENT, m_dbElectrHighCurrent);
	DDX_Text(pDX, IDC_EDIT_ELECTR_HIGH_POWER, m_dbElectrHighPower);
	DDX_Text(pDX, IDC_EDIT_ELECTR_LOW_TEMPERATURE, m_dbElectrLowTemperature);
	DDX_Text(pDX, IDC_EDIT_ELECTR_LOW_VOLTAGE, m_dbElectrLowVoltage);
	DDX_Text(pDX, IDC_EDIT_ELECTR_LOW_CURRENT, m_dbElectrLowCurrent);
	DDX_Text(pDX, IDC_EDIT_ELECTR_LOW_POWER, m_dbElectrLowPower);
	DDX_Text(pDX, IDC_STATIC_TEMPERATURE, m_dbTemperature);
	DDX_Text(pDX, IDC_STATIC_VOLTAGE, m_dbVoltage);
	DDX_Text(pDX, IDC_STATIC_OUTPUT_CURRENT, m_dbOutputCurrent);
	DDX_Check(pDX, IDC_CHECK_TEMPRATURE, m_bCheckElectricBridge);
	DDX_Check(pDX, IDC_CHECK_VOLTAGE, m_bCheckVoltage);
	DDX_Check(pDX, IDC_CHECK_OUTPUT_CURRENT, m_bCheckOutputCurrent);
}


BEGIN_MESSAGE_MAP(CPowerMeterDlg, CDialogEx)
	ON_WM_TIMER()
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_CLOSE()
	ON_WM_CTLCOLOR()
	ON_WM_QUERYDRAGICON()
	ON_STN_CLICKED(IDC_STATIC_START, &CPowerMeterDlg::OnStnClickedStaticStart)
	ON_STN_CLICKED(IDC_STATIC_SELFCHECK, &CPowerMeterDlg::OnStnClickedStaticSelfcheck)
	ON_BN_CLICKED(IDC_CHECK_TEMPRATURE, &CPowerMeterDlg::OnBnClickedCheck)
	ON_BN_CLICKED(IDC_CHECK_VOLTAGE, &CPowerMeterDlg::OnBnClickedCheck)
	ON_BN_CLICKED(IDC_CHECK_OUTPUT_CURRENT, &CPowerMeterDlg::OnBnClickedCheck)
	ON_MESSAGE(WM_THREAD, OnThreadMessage)
	//ON_STN_CLICKED(IDC_STATIC_CURRENTSOURCE_CONNECT, &CPowerMeterDlg::OnStnClickedStaticCurrentsourceConnect)
	//ON_STN_CLICKED(IDC_STATIC_MULTIMETER_CONNECT, &CPowerMeterDlg::OnStnClickedStaticMultimeterConnect)
	//ON_STN_CLICKED(IDC_STATIC_GPIB488_CONNECT, &CPowerMeterDlg::OnStnClickedStaticGpib488Connect)
	//ON_STN_CLICKED(IDC_STATIC_GPIB488_CONNECT2, &CPowerMeterDlg::OnStnClickedStaticGpib488Connect2)
	ON_BN_CLICKED(IDC_BUTTON2, &CPowerMeterDlg::OnBnClickedButton2)
END_MESSAGE_MAP()


// CPowerMeterDlg ��Ϣ�������
BOOL CPowerMeterDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	m_pcSysMenu = GetSystemMenu(FALSE);
	m_pcSysMenu->InsertMenu(0, MF_BYPOSITION, MENUID_SYSPARAM, _T("ϵͳ����"));
	SetWindowText(_T("���·������ϵͳ"));

	m_hBrushOn = CreateSolidBrush(RGB(0, 0xff, 0xff));  //������޸Ļ�ˢ
	m_hBrushRed = CreateSolidBrush(RGB(255, 0, 0));
	m_hBrushGreen = CreateSolidBrush(RGB(0, 180, 0));
	m_hBrushBlue = CreateSolidBrush(RGB(80, 80, 255));
	m_hBrush[LineSerie_Temperature] = CreateSolidBrush(ColorTemperature);
	m_hBrush[LineSerie_Voltage] = CreateSolidBrush(ColorVoltage);
	m_hBrush[LineSerie_OutputCurrent] = CreateSolidBrush(ColorOutputCurrent);
	m_iWorkItem = MENUID_DATACOLLECT;

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	m_hIcon=LoadIcon(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDR_MAINFRAME));
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��
	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	CChartAxis *pAxis = NULL;
	pAxis = m_ChartCtrl.CreateStandardAxis(CChartCtrl::BottomAxis);
	pAxis->SetAutomatic(true);
	pAxis = m_ChartCtrl.CreateStandardAxis(CChartCtrl::LeftAxis);
	pAxis->SetAutomatic(true);

	//m_pLineSerie_Temperature = m_ChartCtrl.CreateLineSerie();//���ŵ�����
	//m_pLineSerie_Temperature->SetColor(ColorTemperature);//���ŵ���ɫ
	//m_pLineSerie_Temperature->SetName(_T("���ŵ���"));//

	m_pLineSerie_Voltage = m_ChartCtrl.CreateLineSerie();//��ѹ������
	m_pLineSerie_Voltage->SetColor(ColorVoltage);//��ѹ����ɫ
	m_pLineSerie_Voltage->SetName(_T("��ص�ѹ"));//

	m_pLineSerie_OutputCurrent = m_ChartCtrl.CreateLineSerie();//�������������
	m_pLineSerie_OutputCurrent->SetColor(ColorOutputCurrent);//������ɫ��Ϊ��ɫ
	m_pLineSerie_OutputCurrent->SetName(_T("�������"));//���õ���������

	RECT rect;
	GetDlgItem(IDC_EDIT_LIGHT_POWER_MEASUR)->GetClientRect(&rect);
	m_cFontValue.CreateFont(rect.bottom,                  // nHeight
		0,                         // nWidth
		0,                         // nEscapement
		0,                         // nOrientation
		FW_BOLD,                   // nWeight
		FALSE,                     // bItalic
		FALSE,                     // bUnderline
		0,                         // cStrikeOut
		ANSI_CHARSET,              // nCharSet
		OUT_DEFAULT_PRECIS,        // nOutPrecision
		CLIP_DEFAULT_PRECIS,       // nClipPrecision
		CLEARTYPE_QUALITY,         // nQuality
		DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
		TEXT("Arial"));            // lpszFacename
	pPSDlg = this;

	((CStatic *)GetDlgItem(IDC_EDIT_LIGHT_POWER_MEASUR))->SetFont(&m_cFontValue);
	((CStatic *)GetDlgItem(IDC_STATIC_SELFCHECK))->SetFont(&m_cFontValue);
	((CStatic *)GetDlgItem(IDC_STATIC_START))->SetFont(&m_cFontValue);

	m_cFontResult.CreateFont(rect.bottom*2,                  // nHeight
		0,                         // nWidth
		0,                         // nEscapement
		0,                         // nOrientation
		FW_BOLD,                   // nWeight
		FALSE,                     // bItalic
		FALSE,                     // bUnderline
		0,                         // cStrikeOut
		ANSI_CHARSET,              // nCharSet
		OUT_DEFAULT_PRECIS,        // nOutPrecision
		CLIP_DEFAULT_PRECIS,       // nClipPrecision
		CLEARTYPE_QUALITY,         // nQuality
		DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
		TEXT("Arial"));            // lpszFacename
	((CStatic *)GetDlgItem(IDC_STATIC_RESULT))->SetFont(&m_cFontResult);


	GetConfigFromINI();
	CImage cImg;
	CDC cDCImgLogo, *pcDC = m_cStaticLogo.GetDC();
	cImg.LoadFromResource(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_BITMAP_LOGO));
	m_cStaticLogo.GetClientRect(&rect);
	m_cImgLogo.Create(rect.right, rect.bottom, 24);
	cDCImgLogo.CreateCompatibleDC(pcDC);
	cDCImgLogo.SelectObject(m_cImgLogo);
	cImg.Draw(cDCImgLogo, 0, 0, rect.right, rect.bottom, 0, 0, cImg.GetWidth(), cImg.GetHeight());
	m_cStaticLogo.SetBitmap(m_cImgLogo);
	UpdateData(FALSE);
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CPowerMeterDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	switch (nID)
	{
	case MENUID_SYSPARAM://������Ҫ�����Լ�
		CSysParamDlg sysDlg;

		sysDlg.m_csComPortCurrentSource = m_csComPortCurrentSource;
		sysDlg.m_csComPortPDA = m_csComPortPDA;
		sysDlg.m_csComMultimeter = m_csComMultimeter;
		sysDlg.m_csComPortShutter = m_csComPortShutter;
		sysDlg.m_csComPortExtCtrl = m_csComPortExtCtrl;
		sysDlg.m_dbHeaterStability = m_dbHeaterStability;
		sysDlg.m_dbHeaterStableTime = m_dbHeaterStableTime;
		sysDlg.m_dbHeaterResCheck = m_dbHeaterResCheck;
		sysDlg.m_dbHeaterResHeat = m_dbHeaterResHeat;
		sysDlg.m_dbHeaterPowerStep = m_dbHeaterPowerStep;
		sysDlg.m_dbPidStability = m_dbPidStability;
		sysDlg.m_dbPidStableTime = m_dbPidStableTime;
		sysDlg.m_dbPidParamA = m_dbPidParamA;
		sysDlg.m_dbPidParamB = m_dbPidParamB;
		sysDlg.m_dbPidParamC = m_dbPidParamC;
		if (IDOK == sysDlg.DoModal())
		{
			if (sysDlg.m_bModified)
			{
				m_csComPortCurrentSource = sysDlg.m_csComPortCurrentSource;
				m_csComPortPDA = sysDlg.m_csComPortPDA;
				m_csComMultimeter = sysDlg.m_csComMultimeter;
				m_csComPortShutter = sysDlg.m_csComPortShutter;
				m_csComPortExtCtrl = sysDlg.m_csComPortExtCtrl;
				m_dbHeaterStability = sysDlg.m_dbHeaterStability;
				m_dbHeaterStableTime = sysDlg.m_dbHeaterStableTime;
				m_dbHeaterResCheck = sysDlg.m_dbHeaterResCheck;
				m_dbHeaterResHeat = sysDlg.m_dbHeaterResHeat;
				m_dbHeaterPowerStep = sysDlg.m_dbHeaterPowerStep;
				m_dbPidStability = sysDlg.m_dbPidStability;
				m_dbPidStableTime = sysDlg.m_dbPidStableTime;
				m_dbPidParamA = sysDlg.m_dbPidParamA;
				m_dbPidParamB = sysDlg.m_dbPidParamB;
				m_dbPidParamC = sysDlg.m_dbPidParamC;
				BackupINI();
				SaveConfigToINI();
			}

		}
		UpdateData(FALSE);
		break;
	}
	CDialogEx::OnSysCommand(nID, lParam);
	//TRACE("%d %d\n", nID, lParam);
}


// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�
void CPowerMeterDlg::OnPaint()
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
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CPowerMeterDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


HBRUSH CPowerMeterDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	//pDC->SetBkMode(TRANSPARENT);
	int  iCtrlID = pWnd->GetDlgCtrlID();

	switch (iCtrlID)
	{
		case IDC_STATIC_START:
			if (m_bInMesuring)
			{
				hbr = m_hBrushOn;
				pDC->SetBkMode(TRANSPARENT);
				pDC->SetTextColor(RGB(255, 0, 0));
			}
			break;
		case IDC_STATIC_SELFCHECK:
			if (m_iSelfChecking == 1) //in selfchecking
			{
				hbr = m_hBrushOn;
				pDC->SetBkMode(TRANSPARENT);
				pDC->SetTextColor(RGB(255, 0, 0));
			}
			else if (m_iSelfChecking < 0) //selfcheckin fault
			{
				hbr = m_hBrushRed;
				pDC->SetBkMode(TRANSPARENT);
				pDC->SetTextColor(RGB(0, 0, 0));
			}
			else if (m_iSelfChecking >1) //selfcheckin success
			{
				hbr = m_hBrushGreen;
				pDC->SetBkMode(TRANSPARENT);
				pDC->SetTextColor(RGB(255, 0, 0));
			}
			break;
		case IDC_STATIC_RESULT:
			pDC->SetBkMode(TRANSPARENT);
			pDC->SetTextColor(RGB(0, 0, 255));
			break;			
		case IDC_CHECK_TEMPRATURE:
		case IDC_STATIC_TEMPERATURE:
			if (m_bCheckElectricBridge)
			{
				hbr = m_hBrush[LineSerie_Temperature];
				pDC->SetBkMode(TRANSPARENT);
				pDC->SetTextColor(RGB(0, 0, 0));
			}
			break;
		case IDC_CHECK_VOLTAGE:
		case IDC_STATIC_VOLTAGE:
			if (m_bCheckVoltage)
			{
				hbr = m_hBrush[LineSerie_Voltage];
				pDC->SetBkMode(TRANSPARENT);
				pDC->SetTextColor(RGB(0, 0, 0));
			}
			break;
		case IDC_CHECK_OUTPUT_CURRENT:
		case IDC_STATIC_OUTPUT_CURRENT:
			if (m_bCheckOutputCurrent)
			{
				hbr = m_hBrush[LineSerie_OutputCurrent];
				pDC->SetBkMode(TRANSPARENT);
				pDC->SetTextColor(RGB(0, 0, 0));
			}
			break;

	}
	return hbr;
}

void CPowerMeterDlg::OnStnClickedStaticStart()//��ʼ����
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	HANDLE  hThread;
	CString cStr;
	CEstimateLightPowerDlg cEstLPDlg;
	if (m_bInMesuring)
	{
		//GetDlgItem(IDC_STATIC_STATUS)->SetWindowTextW(L"ֹͣ����!");
		m_bInMesuring = false;
	}
	else
	{

		//m_pLineSerie_Temperature->ClearSerie();
		m_pLineSerie_Voltage->ClearSerie();
		m_pLineSerie_OutputCurrent->ClearSerie();

		cEstLPDlg.m_dbLightPowerEstimated = m_dbLightPowerEstimate;
		if (cEstLPDlg.DoModal() == IDOK)
		{
			// �������⹦��Ԥ��ֵ���Դ�ֵ��Ϊ��������㡣
			m_dbLightPowerEstimate = cEstLPDlg.m_dbLightPowerEstimated;
			m_bInMesuring = true;
			hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)MeasureThread, this, 0, NULL);
			if (hThread != NULL)
			{
				CloseHandle(hThread);
				GetDlgItem(IDC_STATIC_START)->SetWindowTextW(L"ֹͣ����");
				GetDlgItem(IDC_STATIC_START)->RedrawWindow();
			}
			else
			{
				m_bInMesuring = false;
			}
		}
		UpdateData(FALSE);
	}


/*
	if (m_bInMesuring)
	{
		GetDlgItem(IDC_STATIC_STATUS)->SetWindowTextW(L"ֹͣ����!");
		m_bInMesuring = false;
		KillTimer(0);
		GetDlgItem(IDC_STATIC_START)->SetWindowTextW(L"��ʼ����");
		return;
	}
	else
	{
		GetDlgItem(IDC_STATIC_STATUS)->SetWindowTextW(L"��ʼ����!");
		GetDlgItem(IDC_STATIC_START)->SetWindowTextW(L"ֹͣ����");
		UpdateData(TRUE);
		m_fRayTempSum = m_fParamA + m_fParamB + m_fParamC;

		m_pLineSerie_OutputCurrent->ClearSerie();
		m_pLineSerie_Voltage->ClearSerie();
		m_pLineSerie_Temperature->ClearSerie();
		ZeroMemory(&m_X, sizeof(double)*m_c_arrayLength);
		ZeroMemory(&m_HightSpeedChartArray, sizeof(double)*m_c_arrayLength);
		ZeroMemory(&AmpOutput, sizeof(double)*m_c_arrayLength);
		ZeroMemory(&TempMeasure, sizeof(double)*m_c_arrayLength);
		ZeroMemory(&Voltage, sizeof(double)*m_c_arrayLength);
		ZeroMemory(&Amp, sizeof(double)*m_c_arrayLength);
		ZeroMemory(&Electricbridge, sizeof(double)*m_c_arrayLength);
		ZeroMemory(&PowerHeater, sizeof(double)*m_c_arrayLength);
		ZeroMemory(&VoltageStray, sizeof(double)*m_c_arrayLength);
		ZeroMemory(&PowerStray, sizeof(double)*m_c_arrayLength);
		m_count = 0;
		SetTimer(0, 40, NULL);

		UpdateData(FALSE);
		m_bInMesuring = true;
	}*/
	GetDlgItem(IDC_STATIC_START)->RedrawWindow();
	return;
}

void CPowerMeterDlg::OnBnClickedCheck()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData();

	if (m_bCheckElectricBridge)
	{
		//m_pLineSerie_Temperature->SetVisible(TRUE);
	}
	else
	{
		//m_pLineSerie_Temperature->SetVisible(FALSE);
	}

	if (m_bCheckVoltage)
	{
		m_pLineSerie_Voltage->SetVisible(TRUE);
	}
	else
	{
		m_pLineSerie_Voltage->SetVisible(FALSE);
	}
	if (m_bCheckOutputCurrent)
	{
		m_pLineSerie_OutputCurrent->SetVisible(TRUE);
	}
	else
	{
		m_pLineSerie_OutputCurrent->SetVisible(FALSE);
	}	
}

void CPowerMeterDlg::OnStnClickedStaticSelfcheck()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (m_iSelfChecking == 1)
	{
		m_iSelfChecking = 0; //idel
		GetDlgItem(IDC_STATIC_SELFCHECK)->SetWindowTextW(L"ֹͣ�Լ�");
	}
	else
	{
		m_iSelfChecking = 1;	//in selfchecking
		HANDLE  hThread;
		hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)SelfCheckThread, this, 0, NULL);
		if (hThread != NULL)
		{
			CloseHandle(hThread);
			GetDlgItem(IDC_STATIC_SELFCHECK)->SetWindowTextW(L"��ʼ�Լ�");
			GetDlgItem(IDC_STATIC_START)->EnableWindow(FALSE);
		}
		else m_iSelfChecking = 0;

	}
	GetDlgItem(IDC_STATIC_SELFCHECK)->RedrawWindow();
}

void CPowerMeterDlg::GetConfigFromINI()
{
	DWORD dwBufferSize = 1024, dwRet;
	CString csConfigIniFile = _T(".\\PowerMeter.ini"), csSubIniFile;
	TCHAR ptValue[256];

	dwRet = GetPrivateProfileString(_T("Light"), _T("PowerEstimated"), _T("10"), ptValue, sizeof(ptValue), csConfigIniFile);
	m_dbLightPowerEstimate = _ttof(ptValue);

	// ����ԴCOM��
	dwRet = GetPrivateProfileString(_T("ComPort"), _T("CurrentSource"), _T("COM1"), ptValue, sizeof(ptValue), csConfigIniFile);	
	m_csComPortCurrentSource = ptValue;
	// ������COM��
	dwRet = GetPrivateProfileString(_T("ComPort"), _T("PDA"), _T("COM1"), ptValue, sizeof(ptValue), csConfigIniFile);
	m_csComPortPDA = ptValue;
	// ���ñ�COM��
	dwRet = GetPrivateProfileString(_T("ComPort"), _T("Multimeter"), _T("COM1"), ptValue, sizeof(ptValue), csConfigIniFile);
	m_csComMultimeter = ptValue;
	// ����COM��
	dwRet = GetPrivateProfileString(_T("ComPort"), _T("Shutter"), _T("COM1"), ptValue, sizeof(ptValue), csConfigIniFile);
	m_csComPortShutter = ptValue;
	// ��������COM��
	dwRet = GetPrivateProfileString(_T("ComPort"), _T("ExtCtrl"), _T("COM1"), ptValue, sizeof(ptValue), csConfigIniFile);
	m_csComPortExtCtrl = ptValue;





	dwRet = GetPrivateProfileString(_T("Heater"), _T("Stability"), _T("0.00001"), ptValue, sizeof(ptValue), csConfigIniFile);
	m_dbHeaterStability = _ttof(ptValue);
	dwRet = GetPrivateProfileString(_T("Heater"), _T("StableTime"), _T("10"), ptValue, sizeof(ptValue), csConfigIniFile);
	m_dbHeaterStableTime = _ttof(ptValue);
	dwRet = GetPrivateProfileString(_T("Heater"), _T("ResCheck"), _T("100"), ptValue, sizeof(ptValue), csConfigIniFile);
	m_dbHeaterResCheck = _ttof(ptValue);
	dwRet = GetPrivateProfileString(_T("Heater"), _T("ResHeat"), _T("100"), ptValue, sizeof(ptValue), csConfigIniFile);
	m_dbHeaterResHeat = _ttof(ptValue);
	dwRet = GetPrivateProfileString(_T("Heater"), _T("PowerStep"), _T("0.0001"), ptValue, sizeof(ptValue), csConfigIniFile);
	m_dbHeaterPowerStep = _ttof(ptValue);


	dwRet = GetPrivateProfileString(_T("PID"), _T("Stability"), _T("0.00001"), ptValue, sizeof(ptValue), csConfigIniFile);
	m_dbPidStability = _ttof(ptValue);
	dwRet = GetPrivateProfileString(_T("PID"), _T("StableTime"), _T("10"), ptValue, sizeof(ptValue), csConfigIniFile);
	m_dbPidStableTime = _ttof(ptValue);
	dwRet = GetPrivateProfileString(_T("PID"), _T("ParamA"), _T("1"), ptValue, sizeof(ptValue), csConfigIniFile);
	m_dbPidParamA = _ttof(ptValue);
	dwRet = GetPrivateProfileString(_T("PID"), _T("ParamB"), _T("1"), ptValue, sizeof(ptValue), csConfigIniFile);
	m_dbPidParamB = _ttof(ptValue);
	dwRet = GetPrivateProfileString(_T("PID"), _T("ParamC"), _T("1"), ptValue, sizeof(ptValue), csConfigIniFile);
	m_dbPidParamC = _ttof(ptValue);

}

void CPowerMeterDlg::BackupINI()
{
	CFileFind cFileFind;
	if (cFileFind.FindFile(_T(".\\*.ini")))
	{
		CString csDir, csFileOrg, csFileDst;
		SYSTEMTIME sysTimeLocal;
		GetLocalTime(&sysTimeLocal);
		csDir.Format(_T(".\\PowerMeterINI_%04d%02d%02d-%02d%02d%02d.%03d\\"), sysTimeLocal.wYear, sysTimeLocal.wMonth, sysTimeLocal.wDay,
			sysTimeLocal.wHour, sysTimeLocal.wMinute, sysTimeLocal.wMinute, sysTimeLocal.wMilliseconds);
		if (CreateDirectory(csDir, NULL))
		{

			while (cFileFind.FindNextFile())
			{
				csFileOrg = cFileFind.GetFilePath();
				csFileDst = csDir + cFileFind.GetFileName();
				MoveFile(csFileOrg, csFileDst);
			}
		}
	}
}

void CPowerMeterDlg::SaveConfigToINI()
{
	DWORD dwBufferSize = 1024, dwRet;
	CString csValue, csStrKey,csConfigIniFile = _T(".\\PowerMeter.ini"), csSubIniFile;

	dwRet = WritePrivateProfileString(_T("ComPort"), _T("CurrentSource"), m_csComPortCurrentSource, csConfigIniFile);
	dwRet = WritePrivateProfileString(_T("ComPort"), _T("PDA"), m_csComPortPDA, csConfigIniFile);
	dwRet = WritePrivateProfileString(_T("ComPort"), _T("Multimeter"), m_csComMultimeter, csConfigIniFile);
	dwRet = WritePrivateProfileString(_T("ComPort"), _T("Shutter"), m_csComPortShutter, csConfigIniFile);
	dwRet = WritePrivateProfileString(_T("ComPort"), _T("ExtCtrl"), m_csComPortExtCtrl, csConfigIniFile);

	csValue.Format(_T("%lf"), m_dbHeaterStability);
	dwRet = WritePrivateProfileString(_T("Heater"), _T("Stability"), csValue, csConfigIniFile);
	csValue.Format(_T("%lf"), m_dbHeaterStableTime);
	dwRet = WritePrivateProfileString(_T("Heater"), _T("StableTime"), csValue, csConfigIniFile);
	csValue.Format(_T("%lf"), m_dbHeaterResCheck);
	dwRet = WritePrivateProfileString(_T("Heater"), _T("ResCheck"), csValue, csConfigIniFile);
	csValue.Format(_T("%lf"), m_dbHeaterResHeat);
	dwRet = WritePrivateProfileString(_T("Heater"), _T("ResHeat"), csValue, csConfigIniFile);
	csValue.Format(_T("%lf"), m_dbHeaterPowerStep);
	dwRet = WritePrivateProfileString(_T("Heater"), _T("PowerStep"), csValue, csConfigIniFile);
	csValue.Format(_T("%lf"), m_dbPidStability);
	dwRet = WritePrivateProfileString(_T("PID"), _T("Stability"), csValue, csConfigIniFile);
	csValue.Format(_T("%lf"), m_dbPidStableTime);
	dwRet = WritePrivateProfileString(_T("PID"), _T("StableTime"), csValue, csConfigIniFile);
	csValue.Format(_T("%lf"), m_dbPidParamA);
	dwRet = WritePrivateProfileString(_T("PID"), _T("ParamA"), csValue, csConfigIniFile);
	csValue.Format(_T("%lf"), m_dbPidParamB);
	dwRet = WritePrivateProfileString(_T("PID"), _T("ParamB"), csValue, csConfigIniFile);
	csValue.Format(_T("%lf"), m_dbPidParamC);
	dwRet = WritePrivateProfileString(_T("PID"), _T("ParamC"), csValue, csConfigIniFile);
	
}



LRESULT CPowerMeterDlg::OnThreadMessage(WPARAM wParam, LPARAM lParam)
{
	CString   csValue;
	switch (wParam)
	{
	case THREAD_SELFCHECK_STATUS:
		switch (lParam)
		{
		default:
		case THREAD_STATE_SUCCESS:
			TRACE("�Լ�����");
			GetDlgItem(IDC_STATIC_STATUS)->SetWindowText(_T("�Լ�����"));
			GetDlgItem(IDC_STATIC_RESULT)->SetWindowText(_T("�Լ�����"));
			m_iSelfChecking = 2;
			break;
		case THREAD_STATE_ERROR:
			TRACE("�Լ��쳣");
			GetDlgItem(IDC_STATIC_STATUS)->SetWindowText(_T("�Լ��쳣"));
			GetDlgItem(IDC_STATIC_RESULT)->SetWindowText(_T("�Լ��쳣"));
			m_iSelfChecking = -1;
			break;
		case THREAD_STATE_TERMINATED:
			TRACE("�Լ���ֹ");
			GetDlgItem(IDC_STATIC_STATUS)->SetWindowText(_T("�Լ���ֹ"));
			GetDlgItem(IDC_STATIC_RESULT)->SetWindowText(_T("�Լ���ֹ"));
			m_iSelfChecking = 0;
			break;
		}
		GetDlgItem(IDC_STATIC_START)->EnableWindow(TRUE);
		GetDlgItem(IDC_STATIC_SELFCHECK)->SetWindowTextW(L"��ʼ�Լ�");
		GetDlgItem(IDC_STATIC_SELFCHECK)->RedrawWindow();
		break;
	case THREAD_MEASURE_STATUS:
		switch (lParam)
		{
		case THREAD_STATE_SUCCESS:
			GetDlgItem(IDC_STATIC_STATUS)->SetWindowText(_T("�������"));
			GetDlgItem(IDC_STATIC_RESULT)->SetWindowText(_T("�������"));
			if ((m_dbElectrHighTemperature == m_dbElectrLowTemperature))
			{
				m_dbLightPowerMeasue = (m_dbElectrLowPower + m_dbElectrHighPower) / 2;
			}
			else
			{
				m_dbLightPowerMeasue = m_dbElectrLowPower +
					(m_dbElectrHighPower - m_dbElectrLowPower)*(m_dbLightTemperature - m_dbElectrLowTemperature) / (m_dbElectrHighTemperature - m_dbElectrLowTemperature);
			}
			m_dbLightPowerMeasue= floor(m_dbLightPowerMeasue*1000.0 + 0.5) / 1000.0;
			//������������Э������
			break;
		default:
		case THREAD_STATE_ERROR:
		case THREAD_STATE_TERMINATED:
			GetDlgItem(IDC_STATIC_STATUS)->SetWindowText(_T("������ֹ"));
			GetDlgItem(IDC_STATIC_RESULT)->SetWindowText(_T("������ֹ"));
			break;
		}

		m_bInMesuring = false;
		GetDlgItem(IDC_STATIC_START)->SetWindowTextW(L"��ʼ����");
		GetDlgItem(IDC_STATIC_START)->RedrawWindow();
		break;

	case THREAD_MEASURE_DATA:
		MeasureData MeasureData = *PMeasureData(lParam);
		if (xpAxisNums.size() == 10000) {
			xpAxisNums.pop_front();
			voltageNums.pop_front();
			currentNums.pop_front();
		}
		if (dcNums.size() == 1000) {
			dcNums.pop_front();
		}
		xpAxisNums.emplace_back(++xAxis);
		if ((int)xAxis % 2000 == 0) save(this);
		//// �¶Ȳ���ֵ�����ɹ�
		//if (MeasureData.bFlag & (1 << LineSerie_Temperature))
		//{
		//	m_pLineSerie_Temperature->AddPoint(MeasureData.dwIdex, MeasureData.dbTemperature);
		//	m_dbTemperature = floor(MeasureData.dbTemperature *1000.0 + 0.5)/1000.0; // ����ȡ��
		//}
		// ��ѹֵ�����ɹ�
		if (MeasureData.bFlag & (1 << LineSerie_Voltage))
		{
			double y = MeasureData.dbVoltage;
			m_pLineSerie_Voltage->AddPoint(xAxis, y);
			//m_pLineSerie_Voltage->AddPoint(MeasureData.dwIdex, MeasureData.dbVoltage);
			m_dbVoltage = floor(MeasureData.dbVoltage *1000.0 + 0.5) / 1000.0;
			voltageNums.emplace_back(y);
		}
		if (voltageNums.size() > 0) {
			// �������1000��������ȡ0
			int minPos = min(0, voltageNums.size() - 1000);
			double sum = 0, avg = 0, DCsum = 0;
			// �������1000��ֵ���ۼ�ֵ
			sum = std::accumulate(voltageNums.begin() + minPos, voltageNums.end(), 0.0);
			// ʵ��ȡ�˶��ٸ���������
			int size = voltageNums.size() - minPos;
			//int cnt = 0;
			//for (int i = voltageNums.size() - 1; i >= minPos; i--) {
			//	sum += dcNums[i];
			//	cnt++;
			//}
			avg = sum / max(size, 1); // ��ֹ����0
			for (int i = voltageNums.size() - 1; i >= minPos; i--) {
				DCsum += pow(voltageNums[i] - avg, 2);
			}
			double DC = pow((DCsum / max(size, 1)), 0.5);
			dcNums.emplace_back(DC);
		}

		// ����ֵ�����ɹ�
		if (MeasureData.bFlag & (1 << LineSerie_OutputCurrent))
		{
			double y = MeasureData.dbOutputCurrent;
			m_pLineSerie_OutputCurrent->AddPoint(xAxis, y);
			//m_pLineSerie_OutputCurrent->AddPoint(MeasureData.dwIdex, MeasureData.dbOutputCurrent);
			m_dbOutputCurrent= floor(MeasureData.dbOutputCurrent *1000.0 +0.5) / 1000.0;
			currentNums.emplace_back(y);
		}
		switch (MeasureData.bState)
		{
		default:
		case 0:
			break;
		case 1:
			m_dbLightTemperature= floor(MeasureData.dbTemperature *1000.0 + 0.5) / 1000.0;
			break;
		case 2:
			m_dbElectrLowTemperature = floor(MeasureData.dbTemperature *1000.0 + 0.5) / 1000.0;
			m_dbElectrLowVoltage = floor(MeasureData.dbVoltage *1000.0 + 0.5) / 1000.0;
			m_dbElectrLowCurrent = floor(MeasureData.dbVoltage/m_dbHeaterResCheck *1000.0 + 0.5) / 1000.0;
			m_dbElectrLowPower = floor(m_dbElectrLowCurrent*m_dbElectrLowCurrent / m_dbHeaterResHeat *1000.0 + 0.5) / 1000.0;
			break;
		case 3:
			m_dbElectrHighTemperature = floor(MeasureData.dbTemperature *1000.0 + 0.5) / 1000.0;//��Ҫ����
			m_dbElectrHighVoltage = floor(MeasureData.dbVoltage *1000.0 + 0.5) / 1000.0;
			m_dbElectrHighCurrent = floor(MeasureData.dbVoltage / m_dbHeaterResCheck *1000.0 + 0.5) / 1000.0;
			m_dbElectrHighPower = floor(m_dbElectrHighCurrent*m_dbElectrHighCurrent / m_dbHeaterResHeat *1000.0 + 0.5) / 1000.0;
			break;
		}
		break;
	}
	UpdateData(FALSE);
	return 0;
}

void CPowerMeterDlg::OnBnClickedButton2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	save(this);
}
