#pragma once


// CEstimateLightPowerDlg �Ի���

class CEstimateLightPowerDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CEstimateLightPowerDlg)

public:
	CEstimateLightPowerDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CEstimateLightPowerDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_POWER_ESTIMATED };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	double m_dbLightPowerEstimated;
	afx_msg void OnBnClickedOk();
};
