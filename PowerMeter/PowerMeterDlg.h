#ifndef POWERMETERDLG_H
#define POWERMETERDLG_H

#pragma once
#include "Commons.h"
#include "ChartCtrl/ChartCtrl.h"
#include "afxwin.h"
#include <deque>

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
	//afx_msg void OnTimer(UINT_PTR nIDEvent);
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
	bool m_bDevValid = false;

	CChartCtrl m_ChartCtrl;
	//CChartLineSerie *m_pLineSerie_Temperature;
	CChartLineSerie *m_pLineSerie_Voltage;
	CChartLineSerie *m_pLineSerie_OutputCurrent;

	/* 绘图开关 */

	BOOL m_bCheckElectricBridge;//电桥绘图开关
	BOOL m_bCheckVoltage;//测量电压绘图开关
	BOOL m_bCheckHeaterCurrent;//测量电流绘图开关
	BOOL m_bCheckOutputCurrent;//输出电流绘图开关


	bool m_bValidVerify;


public:
	int   m_iWorkItem;

	CImage m_cImgLogo;
	CStatic m_cStaticLogo;
	//afx_msg void OnStnClickedStaticCurrentsourceConnect();
	//afx_msg void OnStnClickedStaticMultimeterConnect();
	afx_msg void OnBnClickedCheck();
	//afx_msg void OnStnClickedStaticGpib488Connect();
	afx_msg void OnStnClickedStaticSelfcheck();
	//afx_msg void OnStnClickedStaticGpib488Connect2();
	CString m_csComPortCurrentSource;
	CString m_csComPortPDA;
	CString m_csComMultimeter;
	CString m_csComPortShutter;
	CString m_csComPortExtCtrl;
	double m_dbHeaterStability;
	double m_dbHeaterStableTime;
	double m_dbHeaterResCheck;			// 采样电阻值
	double m_dbHeaterResHeat;			// 加热电阻值
	double m_dbHeaterPowerStep;
	double m_dbPidStability;			// PID稳定度
	double m_dbPidStableTime;			// PID稳定时间
	double m_dbPidParamA;				// PID参数A
	double m_dbPidParamB;				// PID参数B
	double m_dbPidParamC;				// PID参数C
	CString m_csResult;
	int		m_iSelfChecking;
	
	double m_dbLightPowerEstimate;		// 预估待测光功率
	double m_dbLightPowerMeasue;		// 光功率测量值
	double m_dbLightTemperature;		// 光加热温度 
	double m_dbElectrHighTemperature;	// 温度测量值
	double m_dbElectrHighVoltage;		
	double m_dbElectrHighCurrent;
	double m_dbElectrHighPower;
	double m_dbElectrLowTemperature;
	double m_dbElectrLowVoltage;
	double m_dbElectrLowCurrent;
	double m_dbElectrLowPower;
	double m_dbTemperature;
	double m_dbVoltage;
	double m_dbOutputCurrent;			// 电流值
	double xAxis = -1;
	std::deque <double> xpAxisNums, voltageNums, currentNums,dcNums;
	double avgNum = 0;
	afx_msg void OnBnClickedButton2();
};

#endif