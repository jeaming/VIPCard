#pragma once


// CReloadDlg �Ի���

class CReloadDlg : public CDialog
{
	DECLARE_DYNAMIC(CReloadDlg)

public:
	CReloadDlg(CWnd* pParent = NULL);   // ��׼���캯��
	//CReloadDlg(unsigned char* pszCardNO, unsigned char* pszName, unsigned char* pszBalance, unsigned char* pszKeyData, CWnd* pParent /*=NULL*/);
	virtual ~CReloadDlg();

// �Ի�������
	enum { IDD = IDD_RELOADDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnCancel();
	CEdit m_EdtCardNO;
	CEdit m_EdtBalance;
	CEdit m_EdtName;
	CEdit m_EdtPhoneNO;
	CEdit m_EdtPurMoney;
	CEdit m_EdtTime;
	unsigned char m_szCardNO[40];
	unsigned char m_szName[40];
	unsigned char m_szBalance[32];
	afx_msg void OnBnClickedBtnBegin();
	afx_msg void OnBnClickedBtnLoad();
	CButton m_BtnBegin;
	CButton m_BtnLoad;
	unsigned char m_szPhoneNO[40];
	unsigned char m_KeyData[512];
	int GetKeyData(unsigned char* pszKeyData);
	virtual BOOL OnInitDialog();
};
