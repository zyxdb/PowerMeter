#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// CSysParamDlg 对话框

class CSysParamDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSysParamDlg)

public:
	CSysParamDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSysParamDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SYSPARAM_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:



	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
	CComboBox m_cComboCurrentSource;//电流源
	CComboBox m_cComboMultimeter;//万用表
	CComboBox m_cComboPDA;
	CComboBox m_cComboShutter;
	CComboBox m_cComboExtCtrl;
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
	BOOL m_bModified;
};
