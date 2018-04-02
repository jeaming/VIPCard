#pragma once


// CVerifyDlg 对话框

class CVerifyDlg : public CDialog
{
	DECLARE_DYNAMIC(CVerifyDlg)

public:
	CVerifyDlg(CWnd* pParent = NULL);   // 标准构造函数
	CVerifyDlg(unsigned char* pszKeyData, CWnd* pParent = NULL); 
	virtual ~CVerifyDlg();

// 对话框数据
	enum { IDD = IDD_VERIFYDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CEdit m_EdtPIN;
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	int m_iCount;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	unsigned char* m_KeyData;
	virtual BOOL OnInitDialog();
	unsigned char m_szPursePIN[64];
};
