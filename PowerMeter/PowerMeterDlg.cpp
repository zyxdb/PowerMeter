// PowerMeterDlg.cpp : 实现文件
//

#include "stdafx.h"
#include<Windows.h>
#include "PowerMeter.h"
#include "PowerMeterDlg.h"
#include "PowerMeterLib.h"
#include "afxdialogex.h"
#include "Commons.h"
#include "ChartCtrl\ChartCtrl.h"
#include "ChartCtrl\ChartTitle.h"
#include "ChartCtrl\ChartLineSerie.h"
#include "ChartCtrl\ChartAxisLabel.h"
#include "SysParamDlg.h"
#include "EstimateLightPowerDlg.h"
#include "GPIB\ni4882.h"

//static void GPIBCleanup(int Dev, const char * ErrorMsg);

// CPowerMeterDlg 对话框


IMPLEMENT_DYNAMIC(CPowerMeterDlg, CDialogEx)
CPowerMeterDlg *pPSDlg = NULL;

CPowerMeterDlg::CPowerMeterDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_POWERMETER_DIALOG, pParent)
	, m_dbHeaterResHeat(0)
	, m_csResult(_T("欢迎使用"))
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
	ON_STN_CLICKED(IDC_STATIC_CURRENTSOURCE_CONNECT, &CPowerMeterDlg::OnStnClickedStaticCurrentsourceConnect)
	ON_STN_CLICKED(IDC_STATIC_MULTIMETER_CONNECT, &CPowerMeterDlg::OnStnClickedStaticMultimeterConnect)
	ON_STN_CLICKED(IDC_STATIC_GPIB488_CONNECT, &CPowerMeterDlg::OnStnClickedStaticGpib488Connect)
	ON_STN_CLICKED(IDC_STATIC_GPIB488_CONNECT2, &CPowerMeterDlg::OnStnClickedStaticGpib488Connect2)
END_MESSAGE_MAP()


// CPowerMeterDlg 消息处理程序
BOOL CPowerMeterDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	m_pcSysMenu = GetSystemMenu(FALSE);
	m_pcSysMenu->InsertMenu(0, MF_BYPOSITION, MENUID_SYSPARAM, _T("系统参数"));
	SetWindowText(_T("低温辐射测量系统"));

	m_hBrushOn = CreateSolidBrush(RGB(0, 0xff, 0xff));  //你可以修改画刷
	m_hBrushRed = CreateSolidBrush(RGB(255, 0, 0));
	m_hBrushGreen = CreateSolidBrush(RGB(0, 180, 0));
	m_hBrushBlue = CreateSolidBrush(RGB(80, 80, 255));
	m_hBrush[LineSerie_Temperature] = CreateSolidBrush(ColorTemperature);
	m_hBrush[LineSerie_Voltage] = CreateSolidBrush(ColorVoltage);
	m_hBrush[LineSerie_OutputCurrent] = CreateSolidBrush(ColorOutputCurrent);
	m_iWorkItem = MENUID_DATACOLLECT;

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	m_hIcon=LoadIcon(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDR_MAINFRAME));
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标
	// TODO: 在此添加额外的初始化代码

	CChartAxis *pAxis = NULL;
	pAxis = m_ChartCtrl.CreateStandardAxis(CChartCtrl::BottomAxis);
	pAxis->SetAutomatic(true);
	pAxis = m_ChartCtrl.CreateStandardAxis(CChartCtrl::LeftAxis);
	pAxis->SetAutomatic(true);

	
	m_pLineSerie_Temperature = m_ChartCtrl.CreateLineSerie();//电桥的曲线
	m_pLineSerie_Temperature->SetColor(ColorTemperature);//电桥的颜色
	m_pLineSerie_Temperature->SetName(_T("电桥电阻"));//

	m_pLineSerie_Voltage = m_ChartCtrl.CreateLineSerie();//电压的曲线
	m_pLineSerie_Voltage->SetColor(ColorVoltage);//电压的颜色
	m_pLineSerie_Voltage->SetName(_T("监控电压"));//

	m_pLineSerie_OutputCurrent = m_ChartCtrl.CreateLineSerie();//输出电流的曲线
	m_pLineSerie_OutputCurrent->SetColor(ColorOutputCurrent);//电流颜色设为蓝色
	m_pLineSerie_OutputCurrent->SetName(_T("输出电流"));//设置电流曲线名


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
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CPowerMeterDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	switch (nID)
	{
	case MENUID_SYSPARAM://别忘了要加上自检
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


// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。
void CPowerMeterDlg::OnPaint()
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
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
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

void CPowerMeterDlg::OnStnClickedStaticStart()//开始测量
{
	// TODO: 在此添加控件通知处理程序代码
	HANDLE  hThread;
	CString cStr;
	CEstimateLightPowerDlg cEstLPDlg;
	if (m_bInMesuring)
	{
		//GetDlgItem(IDC_STATIC_STATUS)->SetWindowTextW(L"停止测量!");
		m_bInMesuring = false;
	}
	else
	{

		m_pLineSerie_Temperature->ClearSerie();
		m_pLineSerie_Voltage->ClearSerie();
		m_pLineSerie_OutputCurrent->ClearSerie();

		cEstLPDlg.m_dbLightPowerEstimated = m_dbLightPowerEstimate;
		if (cEstLPDlg.DoModal() == IDOK)
		{
			m_dbLightPowerEstimate = cEstLPDlg.m_dbLightPowerEstimated;
			m_bInMesuring = true;
			hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)MeasureThread, this, 0, NULL);
			if (hThread != NULL)
			{
				CloseHandle(hThread);
				GetDlgItem(IDC_STATIC_START)->SetWindowTextW(L"停止测量");
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
		GetDlgItem(IDC_STATIC_STATUS)->SetWindowTextW(L"停止测量!");
		m_bInMesuring = false;
		KillTimer(0);
		GetDlgItem(IDC_STATIC_START)->SetWindowTextW(L"开始测量");
		return;
	}
	else
	{
		GetDlgItem(IDC_STATIC_STATUS)->SetWindowTextW(L"开始测量!");
		GetDlgItem(IDC_STATIC_START)->SetWindowTextW(L"停止测量");
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


void CPowerMeterDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (0 == nIDEvent)
	{
		//读取电压表新数值
		USES_CONVERSION;//字符串转换宏
		DWORD a1;
		bool SEND;
		double Volt=0;
		char pcCommand[20] = { '$','$', 'R', 'E','A','D','V','L','T','#','#' };
		SEND = m_cComPortMultimeter.SendData(pcCommand, 11);
		if (SEND) {
			char* rec_f = new char[50];
			memset(rec_f, 0, 50);
			LONG XY = m_cComPortMultimeter.RecData(rec_f, 50, &a1);
			if (XY)
			{
				CString ff;
				ff = A2T(rec_f);
				int posC, posV,posHEAD;
				posHEAD = ff.Find('$');
				posC = ff.Find('C')+2;
				posV = ff.Find('#')-2;
				CString temp;
				temp = ff.Mid(posHEAD);
				temp = ff.Mid(posC,posV-posC);
				Volt = _ttof(temp);
				temp = temp + L"V";
				//GetDlgItem(IDC_STATIC_STATUS)->SetWindowTextW((LPCTSTR)temp);
				GetDlgItem(IDC_STATIC_VOLTAGE_MEASURE)->SetWindowTextW(temp);//测量电压
			}
		}

	}
	CDialogEx::OnTimer(nIDEvent);
}


void CPowerMeterDlg::OnStnClickedStaticCurrentsourceConnect()//连接精密电流源
{
	// TODO: 在此添加控件通知处理程序代码
	CString str, pcCommName;
	char pcCommand[20] = { 'A','T', 'V' , ',','S','E','T',',','A','O','N',',','0','0','0','0','0','0','0',';' };
	if (m_iSelfChecking)
	{
		//MakeLaserControllCommand((PBYTE)pcCommand, 0xA3, 0x000000);//disable current 
		//m_cComPortLaserController.SendData(pcCommand,4);
		//MakeLaserControllCommand((PBYTE)pcCommand, 0xA6, 0x000000);//set to local
		//m_cComPortLaserController.SendData(pcCommand, 8);
		GetDlgItem(IDC_STATIC_STATUS)->SetWindowTextW(L"精密电流源连接成功!");
		BOOL bRet, close;
		pcCommand[10] = 'F';
		close = m_cComPortCurrentSource.SendData(pcCommand, 20);
		if (close) {
			bRet = m_cComPortCurrentSource.ClosePort();
			if (bRet)
			{
				GetDlgItem(IDC_STATIC_STATUS)->SetWindowTextW(L"精密电流源串口关闭成功！");
				m_iSelfChecking = false;
			}
			else
				GetDlgItem(IDC_STATIC_STATUS)->SetWindowTextW(L"精密电流源串口关闭失败！");

			GetDlgItem(IDC_STATIC_CURRENTSOURCE_CONNECT)->SetWindowTextW(L"连接电流源");
			return;
		}

	}
	//int num = m_cComboLaserCtl.GetCurSel();
	::GetPrivateProfileString(_T("System Parameters"), _T("ComboCurren"), _T("0"), str.GetBuffer(20), 20, _T(".//config.ini"));
	int num = _ttoi(str);//字符串转int
	//int num = m_iCurrentsource;
	if (num >= 9)
	{
		pcCommName.Format(_T("\\\\.\\COM%d"), num + 1);
	}
	else
	{
		pcCommName.Format(L"COM%d", num + 1);
	}
	m_bValidVerify = m_cComPortCurrentSource.OpenPort((LPCWSTR)pcCommName, false);  //打开串口
	if (!m_bValidVerify)
	{
		GetDlgItem(IDC_STATIC_STATUS)->SetWindowTextW(L"精密电流源连接失败!");
		return;
	}
	m_bValidVerify = m_cComPortCurrentSource.SetPortParm(9600);//串口波特率设置为9600
	if (!m_bValidVerify)
	{
		GetDlgItem(IDC_STATIC_STATUS)->SetWindowTextW(L"通信设置失败!");
		m_cComPortCurrentSource.ClosePort();
		return;
	}
	else
	{
		DWORD dwRev = 0;
		/*MakeLaserControllCommand((PBYTE)pcCommand, 0xA0, 0x000000);
		m_bValidVerify = m_cComPortLaserController.SendData(pcCommand, 8);
		MakeLaserControllCommand((PBYTE)pcCommand, 0xA1, 0x000000);*/
		//pcCommand[1] = 'O';
		m_bValidVerify = m_cComPortCurrentSource.SendData(pcCommand, 20);
		//m_bValidVerify = m_cComPortCurrentSource.SendData(pcCommand, 4);
		//m_bValidVerify = m_cComPortCurrentSource.SendData(pcCommand, 4);
		if (m_bValidVerify)
		{
			//m_cComPortLaserController.RecData(pcCommand, 32, &dwRev);
			GetDlgItem(IDC_STATIC_STATUS)->SetWindowTextW(L"精密电流源连接设置成功!");
			GetDlgItem(IDC_STATIC_CURRENTSOURCE_CONNECT)->SetWindowTextW(L"断开电流源");
			m_iSelfChecking = true;
			GetDlgItem(IDC_STATIC_CURRENTSOURCE_CONNECT)->RedrawWindow();
			return;
		}
		//MakeLaserControllCommand((PBYTE)pcCommand, 0x7E, 0x000000);

		//m_bValidVerify = m_cComPortLaserController.SendData(pcCommand, 8);
		//if (m_bValidVerify)
		//{
		//	//MakeLaserControllCommand((PBYTE)pcCommand, 0xE0, 0x000000); 
		//	//m_bValidVerify = m_cComPortLaserController.SendData(pcCommand, 8);
		//	m_cComPortLaserController.RecData(pcCommand, 32, &dwRev);
		//	GetDlgItem(IDC_STATIC_STATUS)->SetWindowTextW(L"激光控制器恒温模式设置成功!");
		//	m_cEditWavelengthFixed.EnableWindow(1);
		//	m_cEditLaserPower.EnableWindow(1);
		//	GetDlgItem(IDC_STATIC_START)->SetWindowTextW(L"断开激光器");
		//	m_bLaserControllerConnected = true;
		//	GetDlgItem(IDC_STATIC_START)->RedrawWindow();
		//	return;
		//}
	}
	m_cComPortCurrentSource.ClosePort();
	GetDlgItem(IDC_STATIC_START)->RedrawWindow();
}


void CPowerMeterDlg::OnStnClickedStaticMultimeterConnect()//连接万用表
{
	// TODO: 在此添加控件通知处理程序代码
	CString str, pcCommName;
	char pcCommand[20] = { '$','$', 'R', 'E','A','D','V','L','T','R','A','N','G','E','#','#'};
	if (m_bMultimeterConnect)
	{
		//MakeLaserControllCommand((PBYTE)pcCommand, 0xA3, 0x000000);//disable current 
		//m_cComPortLaserController.SendData(pcCommand,4);
		//MakeLaserControllCommand((PBYTE)pcCommand, 0xA6, 0x000000);//set to local
		//m_cComPortLaserController.SendData(pcCommand, 8);
		GetDlgItem(IDC_STATIC_STATUS)->SetWindowTextW(L"万用表连接成功!");
		BOOL bRet;
		bRet = m_cComPortMultimeter.ClosePort();
		if (bRet)
		{
			GetDlgItem(IDC_STATIC_STATUS)->SetWindowTextW(L"万用表串口关闭成功！");
			m_bMultimeterConnect = false;
		}
		else
			GetDlgItem(IDC_STATIC_STATUS)->SetWindowTextW(L"万用表串口关闭失败！");

		GetDlgItem(IDC_STATIC_MULTIMETER_CONNECT)->SetWindowTextW(L"连接万用表");
		return;
	}

	::GetPrivateProfileString(_T("System Parameters"), _T("ComboMultimeter"), _T("0"), str.GetBuffer(20), 20, _T(".//config.ini"));
	int num = _ttoi(str);//字符串转int
	if (num >= 9)
	{
		pcCommName.Format(_T("\\\\.\\COM%d"), num + 1);
	}
	else
	{
		pcCommName.Format(L"COM%d", num + 1);
	}
	m_bValidVerify = m_cComPortMultimeter.OpenPort((LPCWSTR)pcCommName, false);  //打开串口
	if (!m_bValidVerify)
	{
		GetDlgItem(IDC_STATIC_STATUS)->SetWindowTextW(L"万用表连接失败!");
		return;
	}
	m_bValidVerify = m_cComPortMultimeter.SetPortParm(115200);//串口波特率设置为115200
	if (!m_bValidVerify)
	{
		GetDlgItem(IDC_STATIC_STATUS)->SetWindowTextW(L"通信设置失败!");
		m_cComPortMultimeter.ClosePort();
		return;
	}
	else
	{
		DWORD dwRev = 0;
		//pcCommand[1] = 'O';
		m_bValidVerify = m_cComPortMultimeter.SendData(pcCommand, 18);
		if (m_bValidVerify)
		{
			GetDlgItem(IDC_STATIC_STATUS)->SetWindowTextW(L"万用表连接设置成功!");
			GetDlgItem(IDC_STATIC_MULTIMETER_CONNECT)->SetWindowTextW(L"断开万用表");
			m_bMultimeterConnect = true;
			GetDlgItem(IDC_STATIC_MULTIMETER_CONNECT)->RedrawWindow();
			return;
		}
	}
	m_cComPortMultimeter.ClosePort();
	GetDlgItem(IDC_STATIC_START)->RedrawWindow();
}


void CPowerMeterDlg::OnBnClickedCheck()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();

	if (m_bCheckElectricBridge)
	{
		m_pLineSerie_Temperature->SetVisible(TRUE);
	}
	else
	{
		m_pLineSerie_Temperature->SetVisible(FALSE);
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


void CPowerMeterDlg::OnStnClickedStaticGpib488Connect()
{
	// TODO: 在此添加控件通知处理程序代码
	#define GPIB0                 0        // Board handle
	#define ARRAYSIZE             27       // Size of read buffer
	#define BDINDEX               0        // Board Index
	#define PRIMARY_ADDR_OF_PPS   5        // Primary address of device
	#define NO_SECONDARY_ADDR     0        // Secondary address of device
	#define TIMEOUT               T10s     // Timeout value = 10 seconds
	#define EOTMODE               1        // Enable the END message
	#define EOSMODE               0        // Disable the EOS mode

	if (!m_bDevValid) {
		m_iDev = ibdev(BDINDEX, PRIMARY_ADDR_OF_PPS, NO_SECONDARY_ADDR,
			TIMEOUT, EOTMODE, EOSMODE);
		m_bDevValid = true;
	}
	static char ValueStr[ARRAYSIZE + 1];
	//string Str_rev;
	ibwrt(m_iDev, "*IDN?\r\n", 7L);
	//ibwrt(Dev, "D10Measurements:\r", 17L);
	//if (Ibsta() & ERR)
	//{
	//	GPIBCleanup(Dev, "Unable to write to the Power Supply");
	//}
	ibrd(m_iDev, ValueStr, ARRAYSIZE);
	GetDlgItem(IDC_STATIC_STATUS)->SetWindowTextW((LPCTSTR)(CString)ValueStr);
	ibwrt(m_iDev, "D3Measurements:\r", 17L);
	//if (Ibsta() & ERR)
	//{
	//	GPIBCleanup(Dev, "Unable to read data from Power Supply");
	//}
	//ValueStr[Ibcnt() - 1] = '\0';
	//printf("Data read: %s\n", ValueStr);
}

//void GPIBCleanup(int Dev, const char * ErrorMsg)
//{
//	static char ErrorMnemonic[29][5] = {
//		"EDVR", "ECIC", "ENOL", "EADR", "EARG",
//		"ESAC", "EABO", "ENEB", "EDMA", "",
//		"EOIP", "ECAP", "EFSO", "",     "EBUS",
//		"ESTB", "ESRQ", "",     "",      "",
//		"ETAB", "ELCK", "EARM", "EHDL",  "",
//		"",     "EWIP", "ERST", "EPWR" };
//	printf("Error : %s\nibsta = 0x%x iberr = %d (%s)\n",
//		ErrorMsg, Ibsta(), Iberr(), ErrorMnemonic[Iberr()]);
//	if (Dev != -1)
//	{
//		printf("Cleanup: Taking device off-line\n");
//		ibonl(Dev, 0);
//	}
//}

void CPowerMeterDlg::OnStnClickedStaticSelfcheck()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_iSelfChecking == 1)
	{
		m_iSelfChecking = 0; //idel
		GetDlgItem(IDC_STATIC_SELFCHECK)->SetWindowTextW(L"停止自检");
	}
	else
	{
		m_iSelfChecking = 1;	//in selfchecking
		HANDLE  hThread;
		hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)SelfCheckThread, this, 0, NULL);
		if (hThread != NULL)
		{
			CloseHandle(hThread);
			GetDlgItem(IDC_STATIC_SELFCHECK)->SetWindowTextW(L"开始自检");
			GetDlgItem(IDC_STATIC_START)->EnableWindow(FALSE);
		}
		else m_iSelfChecking = 0;

	}
	GetDlgItem(IDC_STATIC_SELFCHECK)->RedrawWindow();
}

void CPowerMeterDlg::OnStnClickedStaticGpib488Connect2()
{
	// TODO: 在此添加控件通知处理程序代码
	static char ValueStr[ARRAYSIZE + 1];
	if (m_bDevValid) {
		//ibwrt(Dev, "D10Measurements:\r", 17L);
		ibrd(m_iDev, ValueStr, 123);
		GetDlgItem(IDC_STATIC_STATUS)->SetWindowTextW((LPCTSTR)(L"测量值:" + (CString)ValueStr));
		//ibrd(Dev, ValueStr, ARRAYSIZE);
		//GetDlgItem(IDC_STATIC_STATUS)->SetWindowTextW((LPCTSTR)ValueStr);
	}
	else {

	}
}


void CPowerMeterDlg::GetConfigFromINI()
{
	DWORD dwBufferSize = 1024, dwRet;
	CString csConfigIniFile = _T(".\\PowerMeter.ini"), csSubIniFile;
	TCHAR ptValue[256];

	dwRet = GetPrivateProfileString(_T("Light"), _T("PowerEstimated"), _T("10"), ptValue, sizeof(ptValue), csConfigIniFile);
	m_dbLightPowerEstimate = _ttof(ptValue);

	// 电流源COM口
	dwRet = GetPrivateProfileString(_T("ComPort"), _T("CurrentSource"), _T("COM1"), ptValue, sizeof(ptValue), csConfigIniFile);	
	m_csComPortCurrentSource = ptValue;
	// 二极管COM口
	dwRet = GetPrivateProfileString(_T("ComPort"), _T("PDA"), _T("COM1"), ptValue, sizeof(ptValue), csConfigIniFile);
	m_csComPortPDA = ptValue;
	// 万用表COM口
	dwRet = GetPrivateProfileString(_T("ComPort"), _T("Multimeter"), _T("COM1"), ptValue, sizeof(ptValue), csConfigIniFile);
	m_csComMultimeter = ptValue;
	// 快门COM口
	dwRet = GetPrivateProfileString(_T("ComPort"), _T("Shutter"), _T("COM1"), ptValue, sizeof(ptValue), csConfigIniFile);
	m_csComPortShutter = ptValue;
	// 额外连接COM口
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
			TRACE("自检正常");
			GetDlgItem(IDC_STATIC_STATUS)->SetWindowText(_T("自检正常"));
			GetDlgItem(IDC_STATIC_RESULT)->SetWindowText(_T("自检正常"));
			m_iSelfChecking = 2;
			break;
		case THREAD_STATE_ERROR:
			TRACE("自检异常");
			GetDlgItem(IDC_STATIC_STATUS)->SetWindowText(_T("自检异常"));
			GetDlgItem(IDC_STATIC_RESULT)->SetWindowText(_T("自检异常"));
			m_iSelfChecking = -1;
			break;
		case THREAD_STATE_TERMINATED:
			TRACE("自检中止");
			GetDlgItem(IDC_STATIC_STATUS)->SetWindowText(_T("自检中止"));
			GetDlgItem(IDC_STATIC_RESULT)->SetWindowText(_T("自检中止"));
			m_iSelfChecking = 0;
			break;
		}
		GetDlgItem(IDC_STATIC_START)->EnableWindow(TRUE);
		GetDlgItem(IDC_STATIC_SELFCHECK)->SetWindowTextW(L"开始自检");
		GetDlgItem(IDC_STATIC_SELFCHECK)->RedrawWindow();
		break;
	case THREAD_MEASURE_STATUS:
		switch (lParam)
		{
		case THREAD_STATE_SUCCESS:
			GetDlgItem(IDC_STATIC_STATUS)->SetWindowText(_T("测量完成"));
			GetDlgItem(IDC_STATIC_RESULT)->SetWindowText(_T("测量完成"));
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
			//结果按需输出给协作程序；
			break;
		default:
		case THREAD_STATE_ERROR:
		case THREAD_STATE_TERMINATED:
			GetDlgItem(IDC_STATIC_STATUS)->SetWindowText(_T("测量中止"));
			GetDlgItem(IDC_STATIC_RESULT)->SetWindowText(_T("测量中止"));
			break;
		}

		m_bInMesuring = false;
		GetDlgItem(IDC_STATIC_START)->SetWindowTextW(L"开始测量");
		GetDlgItem(IDC_STATIC_START)->RedrawWindow();
		break;

	case THREAD_MEASURE_DATA:
		MeasureData MeasureData = *PMeasureData(lParam);

		if (MeasureData.bFlag & (1 << LineSerie_Temperature))
		{
			m_pLineSerie_Temperature->AddPoint(MeasureData.dwIdex, MeasureData.dbTemperature);
			m_dbTemperature = floor(MeasureData.dbTemperature *1000.0 + 0.5)/1000.0;
		}
		if (MeasureData.bFlag & (1 << LineSerie_Voltage))
		{
			m_pLineSerie_Voltage->AddPoint(MeasureData.dwIdex, MeasureData.dbVoltage);
			m_dbVoltage = floor(MeasureData.dbVoltage *1000.0 + 0.5) / 1000.0;
		}

		if (MeasureData.bFlag & (1 << LineSerie_OutputCurrent))
		{
			m_pLineSerie_OutputCurrent->AddPoint(MeasureData.dwIdex, MeasureData.dbOutputCurrent);
			m_dbOutputCurrent= floor(MeasureData.dbOutputCurrent *1000.0 +0.5) / 1000.0;
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
			m_dbElectrHighTemperature = floor(MeasureData.dbTemperature *1000.0 + 0.5) / 1000.0;//需要计算
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