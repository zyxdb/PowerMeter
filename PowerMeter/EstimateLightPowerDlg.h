#pragma once


// CEstimateLightPowerDlg 对话框

class CEstimateLightPowerDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CEstimateLightPowerDlg)

public:
	CEstimateLightPowerDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CEstimateLightPowerDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_POWER_ESTIMATED };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	double m_dbLightPowerEstimated;
	afx_msg void OnBnClickedOk();
};
