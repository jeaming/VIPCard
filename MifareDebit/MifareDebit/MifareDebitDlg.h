
// MifareDebitDlg.h : 头文件
//

#pragma once

#ifndef COMM_H
#include "CommonFile.h"
#include "afxwin.h"
#define COMM_H
#endif

// CMifareDebitDlg 对话框
class CMifareDebitDlg : public CDialogEx
{
// 构造
public:
	CMifareDebitDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_MIFAREDEBIT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg void OnBnClickedBtnBegin();
	afx_msg void OnBnClickedBtnCardinfo();
	afx_msg void OnBnClickedBtnClose();
	afx_msg void OnBnClickedBtnLoad();
	afx_msg void OnBnClickedBtnAddp();
	afx_msg void OnBnClickedBtnRemovep();
	DECLARE_MESSAGE_MAP()
public:
	CButton m_BtnBegin;
	CEdit m_EdtBalance;
	CEdit m_EdtCardNO;
	CEdit m_EdtName;
	CEdit m_EdtPhoneNO;
	CEdit m_EdtProject1;
	CEdit m_EdtProject2;
	CEdit m_EdtProject3;
	CEdit m_EdtTimes1;
	CEdit m_EdtTimes2;
	CEdit m_EdtTimes3;
	CEdit m_EdtVIPCardNO;
	CStatic m_StBalance;
	CStatic m_StTimes1;
	CStatic m_StTimes2;
	CStatic m_StTimes3;
	CComboBox m_CmbProject;
	int m_iListCount;
	CEdit m_EdtProject10;
	CEdit m_EdtProject4;
	CEdit m_EdtProject5;
	CEdit m_EdtProject6;
	CEdit m_EdtProject7;
	CEdit m_EdtProject8;
	CEdit m_EdtProject9;
	CEdit m_EdtTimes10;
	CEdit m_EdtTimes4;
	CEdit m_EdtTimes5;
	CEdit m_EdtTimes6;
	CEdit m_EdtTimes7;
	CEdit m_EdtTimes8;
	CEdit m_EdtTimes9;
	CButton m_BtnAddProject;
	CButton m_BtnRemoveProject;
	CEdit m_EdtTitles1;
	CEdit m_EdtTitles2;
	CEdit m_EdtTitles3;
	CEdit m_EdtTitles4;
	int m_iProjectsCount;
	BOOL m_isInitialized;
	int m_iProjects;
	unsigned char m_KeyData[512];
	CButton m_BtnLoad;
	CardData m_cardData;
	CStatic m_StDisp;
	ProjectData project[10];
private:
	int m_iCardStatus;//0 个人化；1 充值；3 锁定
public:
	int CheckNumberData(unsigned char* pszData);
	int AuthSector(int iSector, int iKeyModal);
	int SetProjectList(void);
	int GetKeyData(unsigned char* pszKeyData);
	int WriteProjectBlock(ProjectData* pData);
	int ReadProjectBlock(ProjectData* pData, int iProject);
	int m_iProjectLeft;
	int GetProjectData(ProjectData* pData, int iProject);
	int SetProjectData(ProjectData proData, int iProject);
	CString m_strLog;
	int m_hasLoadProject;
	afx_msg void OnBnClickedBtnRemoveEmpty();
	CButton m_BtnRmvEmpty;
	CButton m_BtnDoChange;
	afx_msg void OnBnClickedBtnChangeName();
	afx_msg void OnBnClickedBtnChangeNameDo();
	afx_msg void OnBnClickedBtnChangeNameCancel();
	int m_iBeginChangeName;
	afx_msg void OnBnClickedButtonTest();
	CEdit m_EdtOperator;
};
