#pragma once
#include "Commons.h"
#include "ChartCtrl/ChartCtrl.h"
#include "afxwin.h"

// CPowerMeterDlg 对话框

#define MENUID_DATACOLLECT  WM_USER+10
#define MENUID_SYSPARAM WM_USER+11
#define WM_THREAD     WM_USER+100
#define WM_ERROR     WM_USER+101

#define ColorTemperature		RGB(255, 80, 80)//电桥的颜色
#define ColorVoltage			RGB(100, 100, 255)//电压的颜色
#define ColorOutputCurrent		RGB(0, 200, 0)//电流颜色设
enum {
	LineSerie_Temperature=0,
	LineSerie_Voltage,
	LineSerie_OutputCurrent,
	LineSerie_All
};

class CPowerMeterDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CPowerMeterDlg)

public:
	CPowerMeterDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CPowerMeterDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_POWERMETER_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnStnClickedStaticStart();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	LRESULT		 OnThreadMessage(WPARAM wParam, LPARAM lParam);
	void GetConfigFromINI();
	void SaveConfigToINI();
	void BackupINI();
	CMenu *m_pcSysMenu;
	HICON m_hIcon;
	HBRUSH  m_hBrushOn;
	HBRUSH  m_hBrushRed;
	HBRUSH  m_hBrushGreen;
	HBRUSH  m_hBrushBlue;
	HBRUSH  m_hBrush[LineSerie_All];
	CFont	m_cFontResult;
	CFont	m_cFontValue;

	bool m_bInMesuring;//测量开关

	int  m_iDev;
	bool m_bDevValid = false;

	CChartCtrl m_ChartCtrl;
	CChartLineSerie *m_pLineSerie_Temperature;
	CChartLineSerie *m_pLineSerie_Voltage;
	CChartLineSerie *m_pLineSerie_OutputCurrent;

	/* 绘图开关 */

	BOOL m_bCheckElectricBridge;//电桥绘图开关
	BOOL m_bCheckVoltage;//测量电压绘图开关
	BOOL m_bCheckHeaterCurrent;//测量电流绘图开关
	BOOL m_bCheckOutputCurrent;//输出电流绘图开关

	bool m_bMultimeterConnect = false;//万用表开关
	bool m_bValidVerify;


public:
	int   m_iWorkItem;
	Cport	m_cComPortCurrentSource;//精密电流源
	Cport	m_cComPortMultimeter;//万用表
	CImage m_cImgLogo;
	CStatic m_cStaticLogo;
	afx_msg void OnStnClickedStaticCurrentsourceConnect();
	afx_msg void OnStnClickedStaticMultimeterConnect();
	afx_msg void OnBnClickedCheck();
	afx_msg void OnStnClickedStaticGpib488Connect();
	afx_msg void OnStnClickedStaticSelfcheck();
	afx_msg void OnStnClickedStaticGpib488Connect2();
	CString m_csComPortCurrentSource;
	CString m_csComPortPDA;
	CString m_csComMultimeter;
	CString m_csComPortShutter;
	CString m_csComPortExtCtrl;
	double m_dbHeaterStability;
	double m_dbHeaterStableTime;
	double m_dbHeaterResCheck;
	double m_dbHeaterResHeat;
	double m_dbHeaterPowerStep;
	double m_dbPidStability;
	double m_dbPidStableTime;
	double m_dbPidParamA;
	double m_dbPidParamB;
	double m_dbPidParamC;
	CString m_csResult;
	int		m_iSelfChecking;
	double m_dbLightPowerEstimate;
	double m_dbLightPowerMeasue;
	double m_dbLightTemperature;
	double m_dbElectrHighTemperature;
	double m_dbElectrHighVoltage;
	double m_dbElectrHighCurrent;
	double m_dbElectrHighPower;
	double m_dbElectrLowTemperature;
	double m_dbElectrLowVoltage;
	double m_dbElectrLowCurrent;
	double m_dbElectrLowPower;
	double m_dbTemperature;
	double m_dbVoltage;
	double m_dbOutputCurrent;

	afx_msg void OnStnClickedStaticShutter();
};