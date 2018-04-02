#pragma once


//#ifndef COMM_H
//#include "CommonFile.h"
//#define COMM_H
//#endif

#include "RFReader.h"
#ifndef RFR_H
#define RFR_H
#endif
// CPurseCardDlg 对话框

class CPurseCardDlg : public CDialog
{
	DECLARE_DYNAMIC(CPurseCardDlg)

public:
	CPurseCardDlg(CWnd* pParent = NULL);   // 标准构造函数
	CPurseCardDlg(CardData* pCardData, unsigned char* pszKeyData, CRFReader* rf,
		CWnd* pParent = NULL);
	virtual ~CPurseCardDlg();

// 对话框数据
	enum { IDD = IDD_PURSECARDDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedCheckProject();
	afx_msg void OnBnClickedCheckProject1();
	afx_msg void OnBnClickedCheckProject2();
	afx_msg void OnBnClickedCheckProject3();
	CButton m_ChkProject;
	CButton m_ChkProject1;
	CButton m_ChkProject2;
	CButton m_ChkProject3;
	CEdit m_EdtAmount;
	CEdit m_EdtCardNO;
	CEdit m_EdtMoney;
	CEdit m_EdtName;
	CEdit m_EdtProject2;
	CEdit m_EdtTimes1;
	afx_msg void OnBnClickedBtnPure();
	int GetKeyData(unsigned char* pszKeyData);
	virtual BOOL OnInitDialog();
	unsigned char m_KeyData[512];
	CEdit m_EdtProject3;
	CEdit m_EdtTimes2;
	CEdit m_EdtTimes3;
	CardData* m_pCardData;
private:
	CRFReader* rf_p;
public:
	CEdit m_EdtVIPCardNO;
	CEdit m_EdtPhoneNO;
	CEdit m_EdtProject;
	CEdit m_EdtProject1;
	CStatic m_StTimes1;
	CStatic m_StBalance;
	CStatic m_StTimes3;
	CStatic m_StTimes2;

public:
	int WriteProjectBlock(ProjectData* pData);
	int ReadProjectBlock(ProjectData* pData, int iProject);
	int GetProjectData(ProjectData* pData, int iProject);
	int SetProjectData(ProjectData proData, int iProject);
	CEdit m_EdtProject4;
	CEdit m_EdtProject5;
	CEdit m_EdtProject6;
	CEdit m_EdtProject7;
	CEdit m_EdtProject8;
	CEdit m_EdtProject9;
	CEdit m_EdtProject10;
	CEdit m_EdtTimes10;
	CEdit m_EdtTimes4;
	CEdit m_EdtTimes5;
	CEdit m_EdtTimes6;
	CEdit m_EdtTimes7;
	CEdit m_EdtTimes8;
	CEdit m_EdtTimes9;
	CStatic m_StTimes10;
	CStatic m_StTimes4;
	CStatic m_StTimes5;
	CStatic m_StTimes6;
	CStatic m_StTimes7;
	CStatic m_StTimes8;
	CStatic m_StTimes9;
	CButton m_ChkProject10;
	CButton m_ChkProject4;
	CButton m_ChkProject5;
	CButton m_ChkProject6;
	CButton m_ChkProject7;
	CButton m_ChkProject8;
	CButton m_ChkProject9;
	afx_msg void OnBnClickedBtnReadcard();
	int ReadPersonalData(void);

public:
	int AuthSector(int iSector, int iKeyModal);
	int m_iProjects;
	ProjectData projectData[10];
	afx_msg void OnBnClickedCheckProject4();
	afx_msg void OnBnClickedCheckProject5();
	afx_msg void OnBnClickedCheckProject6();
	afx_msg void OnBnClickedCheckProject7();
	afx_msg void OnBnClickedCheckProject8();
	afx_msg void OnBnClickedCheckProject9();
	afx_msg void OnBnClickedCheckProject10();
	CString m_strLog;
	CEdit m_EdtOperator;
};
