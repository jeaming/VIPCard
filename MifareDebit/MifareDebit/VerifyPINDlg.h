#pragma once


// CVerifyPINDlg �Ի���

class CVerifyPINDlg : public CDialog
{
	DECLARE_DYNAMIC(CVerifyPINDlg)

public:
	CVerifyPINDlg(CWnd* pParent = NULL);   // ��׼���캯��
	CVerifyPINDlg(unsigned char* pszKeyData, CWnd* pParent = NULL);
	virtual ~CVerifyPINDlg();

// �Ի�������
	enum { IDD = IDD_VERIFYPINDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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
