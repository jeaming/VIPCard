// VerifyDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MifarePurse.h"
#include "VerifyDlg.h"
#include "afxdialogex.h"


// CVerifyDlg �Ի���

IMPLEMENT_DYNAMIC(CVerifyDlg, CDialog)

CVerifyDlg::CVerifyDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CVerifyDlg::IDD, pParent)
	, m_iCount(0)
{

}

CVerifyDlg::CVerifyDlg(unsigned char* pszKeyData, CWnd* pParent /*=NULL*/)
	: CDialog(CVerifyDlg::IDD, pParent)
	, m_iCount(0)
{
	m_KeyData = pszKeyData;
}

CVerifyDlg::~CVerifyDlg()
{
	m_KeyData = NULL;
}

void CVerifyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_PIN, m_EdtPIN);
}


BEGIN_MESSAGE_MAP(CVerifyDlg, CDialog)
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()


// CVerifyDlg ��Ϣ�������


void CVerifyDlg::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	CDialog::OnKeyDown(nChar, nRepCnt, nFlags);
	
}


BOOL CVerifyDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���

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
			if(strTemp.Compare((LPCTSTR)m_szPursePIN))
			{
				AfxMessageBox("���벻��ȷ�����������룡");
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


BOOL CVerifyDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	char szPINLen[5];
	int iLoadKeyLen = 0;
	int iLen = 0;

	memset(szPINLen, 0, 5);
	memset(m_szPursePIN, 0, 64);

	memcpy(szPINLen, m_KeyData + 384, 2);
	iLoadKeyLen = atoi(szPINLen) + 2;

	memcpy(szPINLen, m_KeyData + 384 + iLoadKeyLen, 2);
	iLen = atoi(szPINLen);

	memcpy(m_szPursePIN, m_KeyData + 386 + iLoadKeyLen, iLen);	

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}
