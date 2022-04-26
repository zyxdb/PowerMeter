#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// CSysParamDlg �Ի���

class CSysParamDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSysParamDlg)

public:
	CSysParamDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CSysParamDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SYSPARAM_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:



	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
	CComboBox m_cComboCurrentSource;//����Դ
	CComboBox m_cComboMultimeter;//���ñ�
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
