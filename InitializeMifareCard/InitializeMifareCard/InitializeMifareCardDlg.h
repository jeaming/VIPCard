
// InitializeMifareCardDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"


// CInitializeMifareCardDlg �Ի���
class CInitializeMifareCardDlg : public CDialogEx
{
// ����
public:
	CInitializeMifareCardDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_INITIALIZEMIFARECARD_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��

public://event
	afx_msg void OnBnClickedBtnDestroy();
	afx_msg void OnBnClickedButton1();//Key manager
	afx_msg void OnBnClickedBtnInit();


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CEdit m_EdtName;
	CEdit m_EdtCardNO;
	CEdit m_EdtPhoneNO;
	CEdit m_EdtAmount;
	BOOL m_isConnected;
	unsigned char m_szKeyData[512];
	CStatic m_StInit;

public://function

	int m_CardCount;
	afx_msg void OnBnClickedBtnClose();
	int GetKeyData(unsigned char* pszKeyData);
	unsigned char m_KeyData[512];
	afx_msg void OnBnClickedBtnStop();
	int m_isStoped;
	CButton m_btnStartInit;
	CButton m_btnStopInit;
	CButton m_btnDestroy;
	CButton m_btnClose;
};
