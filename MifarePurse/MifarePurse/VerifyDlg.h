#pragma once


// CVerifyDlg �Ի���

class CVerifyDlg : public CDialog
{
	DECLARE_DYNAMIC(CVerifyDlg)

public:
	CVerifyDlg(CWnd* pParent = NULL);   // ��׼���캯��
	CVerifyDlg(unsigned char* pszKeyData, CWnd* pParent = NULL); 
	virtual ~CVerifyDlg();

// �Ի�������
	enum { IDD = IDD_VERIFYDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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
