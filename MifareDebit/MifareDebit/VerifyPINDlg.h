#pragma once


// CVerifyPINDlg 对话框

class CVerifyPINDlg : public CDialog
{
	DECLARE_DYNAMIC(CVerifyPINDlg)

public:
	CVerifyPINDlg(CWnd* pParent = NULL);   // 标准构造函数
	CVerifyPINDlg(unsigned char* pszKeyData, CWnd* pParent = NULL);
	virtual ~CVerifyPINDlg();

// 对话框数据
	enum { IDD = IDD_VERIFYPINDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CEdit m_EdtPIN;
	int m_iCount;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	unsigned char m_szPINData[40];
	unsigned char*m_KeyData;
	unsigned char m_szLoadPIN[64];
	virtual BOOL OnInitDialog();
};
