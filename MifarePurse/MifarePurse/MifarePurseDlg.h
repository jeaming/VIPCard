
// MifarePurseDlg.h : ͷ�ļ�
//

#pragma once

//#ifndef COMM_H
//#include "CommonFile.h"
//#define COMM_H
//#endif
// CMifarePurseDlg �Ի���
class CMifarePurseDlg : public CDialogEx
{
// ����
public:
	CMifarePurseDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_MIFAREPURSE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


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
	CEdit m_EdtBalance;
	CEdit m_EdtCardNO;
	CEdit m_EdtName;
	CEdit m_EdtPhoneNO;
	afx_msg void OnBnClickedBtnPurchase();
	afx_msg void OnBnClickedBtnInfo();
	unsigned char m_KeyData[512];
	afx_msg void OnBnClickedBtnCredit();
	unsigned char m_szCardNO[40];
	unsigned char m_szBalance[40];
	unsigned char m_szName[60];
	unsigned char m_szPhoneNO[40];
	BOOL m_isInitialized;
	int GetKeyData(unsigned char* pszKeyData);
	CEdit m_EdtProjectTimes1;
	CEdit m_EdtProjectTimes2;
	CEdit m_EdtProjectTimes3;
	CEdit m_EdtProject1;
	CEdit m_EdtProject2;
	CEdit m_EdtProject3;
	CardData m_cardData;
	CEdit m_EdtVIPCardNO;
	afx_msg void OnBnClickedBtnClose();
};
