// VerifyPINDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MifareDebit.h"
#include "VerifyPINDlg.h"
#include "afxdialogex.h"


// CVerifyPINDlg 对话框

IMPLEMENT_DYNAMIC(CVerifyPINDlg, CDialog)

CVerifyPINDlg::CVerifyPINDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CVerifyPINDlg::IDD, pParent)
	, m_KeyData(NULL)
{

	m_iCount = 0;
}

CVerifyPINDlg::CVerifyPINDlg(unsigned char* pszKeyData, CWnd* pParent /*=NULL*/)
	: CDialog(CVerifyPINDlg::IDD, pParent)
{
	m_iCount = 0;
	m_KeyData = pszKeyData;
}

CVerifyPINDlg::~CVerifyPINDlg()
{
	m_KeyData = NULL;
}

void CVerifyPINDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_PIN, m_EdtPIN);
}


BEGIN_MESSAGE_MAP(CVerifyPINDlg, CDialog)
END_MESSAGE_MAP()


// CVerifyPINDlg 消息处理程序


BOOL CVerifyPINDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if(m_iCount == 3)
		this->OnCancel();

	if(pMsg->message == WM_KEYDOWN)
	{
		if(pMsg->wParam == VK_ESCAPE)
		{
			this->OnCancel();
		}
		if(pMsg->wParam == VK_RETURN)
		{
			CString strTemp;
			m_EdtPIN.GetWindowTextA(strTemp);
			if(strTemp.Compare((LPCTSTR)m_szLoadPIN))
			{
				AfxMessageBox("密码不正确，请重新输入！");
				m_EdtPIN.SetWindowTextA("");
				m_iCount ++;
				return 0;
			}
			else
				this->OnOK();
		}
	}

	return CDialog::PreTranslateMessage(pMsg);
}


BOOL CVerifyPINDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	char szLoadPINLen[5];
	int iLen;

	memset(szLoadPINLen, 0, 5);
	memset(m_szLoadPIN, 0, 64);

	memcpy(szLoadPINLen, m_KeyData + 384, 2);
	iLen = atoi(szLoadPINLen);
	memcpy(m_szLoadPIN, m_KeyData + 386, iLen);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
