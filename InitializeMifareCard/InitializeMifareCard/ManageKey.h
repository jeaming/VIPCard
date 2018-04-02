#pragma once
#include "afxwin.h"



// CManageKey 对话框

class CManageKey : public CDialog
{
	DECLARE_DYNAMIC(CManageKey)

public:
	CManageKey(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CManageKey();

// 对话框数据
	enum { IDD = IDD_MANAGEKEY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CEdit m_EdtSectorName;
	CString m_strSector1L;
	CString m_strSector1R;
	CString m_strSector2L;
	CString m_strSector2R;
	CString m_strSector3L;
	CString m_strSector3R;
	CString m_strSector4L;
	CString m_strSector4R;
	CString m_strSector5L;
	CString m_strSector5R;
	CString m_strSector6L;
	CString m_strSector6R;
	CString m_strSector7L;
	CString m_strSector7R;
	CString m_strSector8L;
	CString m_strSector8R;
	CString m_strSector9L;
	CString m_strSector9R;
	CString m_strSector10L;
	CString m_strSector10R;
	CString m_strSector11L;
	CString m_strSector11R;
	CString m_strSector12L;
	CString m_strSector12R;
	CString m_strSector13L;
	CString m_strSector13R;
	CString m_strSector14L;
	CString m_strSector14R;
	CString m_strSector15L;
	CString m_strSector15R;
	CString m_strSector16L;
	CString m_strSector16R;
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedBtnGetkey();
	int CheckKey(void);
protected:
	CString m_strKeyData;
public:
	afx_msg void OnBnClickedButton2();
//	CEdit m_EdtLoadPIN;
	CEdit m_EdtPursePIN;
	CEdit m_EdtLoadPIN;
	CString m_strLoadPIN;
	CString m_strPursePIN;
	int GetKeyData(unsigned char* pszKeyData);
	unsigned char m_KeyData[512];
};
