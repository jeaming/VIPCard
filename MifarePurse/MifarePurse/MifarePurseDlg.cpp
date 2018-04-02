
// MifarePurseDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MifarePurse.h"
#include "MifarePurseDlg.h"
#include "afxdialogex.h"
#include "PurseCardDlg.h"
#include "ReloadDlg.h"
#include "RFReader.h"
#include "DES.h"
#include "VerifyDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


///////////////////////////////////////////////////SELF DEFINATION
CRFReader rf;



#define MAIN_KEY		"12345678900987654321ABCDEFFEDCBA"
#define		INI_FILE	".\\Set.dat"
#define		DEFAULT_KEY	"FFFFFFFFFFFF"



/////////////////////////////////////////////


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CMifarePurseDlg �Ի���




CMifarePurseDlg::CMifarePurseDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMifarePurseDlg::IDD, pParent)
	, m_isInitialized(FALSE)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDI_ICON_PURSE);
}

void CMifarePurseDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_BALANCE, m_EdtBalance);
	DDX_Control(pDX, IDC_EDIT_CARDNO, m_EdtCardNO);
	DDX_Control(pDX, IDC_EDIT_NAME, m_EdtName);
	DDX_Control(pDX, IDC_EDIT_PHONENO, m_EdtPhoneNO);
	DDX_Control(pDX, IDC_EDIT_LEFT1, m_EdtProjectTimes1);
	DDX_Control(pDX, IDC_EDIT_LEFT2, m_EdtProjectTimes2);
	DDX_Control(pDX, IDC_EDIT_LEFT3, m_EdtProjectTimes3);
	DDX_Control(pDX, IDC_EDIT_PROJECT1, m_EdtProject1);
	DDX_Control(pDX, IDC_EDIT_PROJECT2, m_EdtProject2);
	DDX_Control(pDX, IDC_EDIT_PROJECT3, m_EdtProject3);
	DDX_Control(pDX, IDC_EDIT_VIPCARDNO, m_EdtVIPCardNO);
}

BEGIN_MESSAGE_MAP(CMifarePurseDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_PURCHASE, &CMifarePurseDlg::OnBnClickedBtnPurchase)
	ON_BN_CLICKED(IDC_BTN_INFO, &CMifarePurseDlg::OnBnClickedBtnInfo)
	ON_BN_CLICKED(IDC_BTN_CREDIT, &CMifarePurseDlg::OnBnClickedBtnCredit)
	ON_BN_CLICKED(IDC_BTN_CLOSE, &CMifarePurseDlg::OnBnClickedBtnClose)
END_MESSAGE_MAP()


// CMifarePurseDlg ��Ϣ�������

BOOL CMifarePurseDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	memset(m_KeyData, 0, sizeof(m_KeyData));
	if(GetKeyData(m_KeyData))
	{
		AfxMessageBox("��ʼ�������ļ�ʧ�ܣ�");
		exit(1);
	}

	CVerifyDlg ver(m_KeyData);
	if(ver.DoModal() == IDCANCEL)
	{
		exit(1);
	}
	memset(m_cardData.szBalance, 0, sizeof(m_cardData.szBalance));
	memset(m_cardData.szCardNO, 0, sizeof(m_cardData.szCardNO));
	memset(m_cardData.szName, 0, sizeof(m_cardData.szCardNO));
	memset(m_cardData.szPhoneNO,  0, sizeof(m_cardData.szPhoneNO));
	memset(m_cardData.szVIPCardNO, 0, sizeof(m_cardData.szVIPCardNO));
	m_cardData.iAmount = 0;
	m_cardData.iProjectsCount = 0;
	m_cardData.bIsUsed = FALSE;


	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CMifarePurseDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CMifarePurseDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CMifarePurseDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

//����
void CMifarePurseDlg::OnBnClickedBtnPurchase()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(!m_isInitialized)
		this->OnBnClickedBtnInfo();

	if(m_cardData.iFlag != 1)
	{
		AfxMessageBox("��Ƭ״̬�쳣���޷����ѣ�");
		return;
	}

	CPurseCardDlg dlg(&m_cardData, m_KeyData, &rf);
	dlg.DoModal();
	m_isInitialized = FALSE;
}

//��ֵ
void CMifarePurseDlg::OnBnClickedBtnCredit()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	
	if(!m_isInitialized)
		this->OnBnClickedBtnInfo();
	rf.Dev_Close();
	CReloadDlg dlg;
	dlg.DoModal();
	m_isInitialized = FALSE;

}

//������Ϣ
void CMifarePurseDlg::OnBnClickedBtnInfo()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int iRet = 0, iBlock, iDataLen = 0;
	CString strTemp;
	unsigned char szSectorKey[40], szBlockData[40];
	//unsigned char szProjectTimes[5], szProjectName[40];
	unsigned long lMoney = 0;
	//int iProjectTimes;
	
	//�豸����������Կ��֤
	//���Ӷ�д��
	if(!rf.m_isConnected)
	{
		if(rf.Dev_Connect(100, 115200))
		{
			AfxMessageBox("���Ӷ�д��ʧ�ܣ������豸��");
			return;
		}
	}

	//��Ƭ��λ
	if((iRet = rf.Dev_Reset(1)))
	{
		strTemp.Format("��Ƭ��λʧ�ܣ�����ֵ��%d", iRet);
		AfxMessageBox(strTemp);
		return;
	}

	//�õ�����Կ
	//��Ϣ����Ϊ20 21 22
	iBlock = 20;
	memset(szSectorKey, 0, 40);

	memcpy(szSectorKey, m_KeyData + (iBlock/4)*24, 12);
	if(rf.LoadKey(rf.KEY_L, iBlock/4, szSectorKey, rf.HEX))
	{
		AfxMessageBox("��֤��Կʧ�ܣ�");
		return;
	}

	if(rf.Anthentication(rf.KEY_L, iBlock/4))
	{
		AfxMessageBox("��֤��Կʧ��2��");
		return;
	}
	//��֤��Կ���

	if(rf.ReadValue(iBlock, &lMoney))
	{
		AfxMessageBox("ȡ���ʧ�ܣ�");
		return;
	}

	strTemp.Format("%d", lMoney);
	m_cardData.iAmount = lMoney;
	m_EdtBalance.SetWindowTextA((LPCTSTR)strTemp);//��ʾ���

	//������
	memset(szBlockData, 0, 40);
	memset(m_szName, 0, 60);
	iBlock = 21;
	memset(szBlockData, 0, 40);
	if(rf.ReadMifareBlock(iBlock, szBlockData, rf.HEX))
	{
		AfxMessageBox("������ʧ�ܣ����鿨Ƭ��");
		return;
	}
	iDataLen = 20;
	memset(m_cardData.szName, 0, sizeof(m_cardData.szName));
	memcpy(m_cardData.szName, szBlockData, iDataLen);
	rf.SwitchDataMode(m_cardData.szName, &iDataLen, rf.HEX2ASC);
	strTemp.Format("%s", m_cardData.szName);
	strTemp.Remove(0x20);
	m_EdtName.SetWindowTextA((LPCTSTR)strTemp);
	memcpy(m_cardData.szName, strTemp, strTemp.GetLength());
	m_cardData.szName[strTemp.GetLength()] = 0;

	//��ϵ�绰
	memcpy(m_cardData.szPhoneNO, szBlockData + 20, 12);
	m_cardData.szPhoneNO[12] = 0;
	m_EdtPhoneNO.SetWindowTextA((LPCTSTR)m_cardData.szPhoneNO);


	//����ϵ��ʽ\������Ϣ
	iBlock = 22;
	memset(szBlockData, 0, 40);
	if(rf.ReadMifareBlock(iBlock, szBlockData, rf.HEX))
	{
		AfxMessageBox("������ʧ�ܣ����鿨Ƭ��");
		return;
	}


	//����
	memcpy(m_cardData.szCardNO, szBlockData, 12);
	m_EdtCardNO.SetWindowTextA((LPCTSTR)m_cardData.szCardNO);

	//VIP�����趨Ϊ12====================================================120520
	memcpy(m_cardData.szVIPCardNO, szBlockData + 12, 12);
	m_EdtVIPCardNO.SetWindowTextA((LPCTSTR)m_cardData.szVIPCardNO);

	unsigned char szProjectsCount[10];

	memset(szProjectsCount, 0, 10);
	memcpy(szProjectsCount, szBlockData + 24, 2);

	m_cardData.iProjectsCount = rf.Hex2Dec(szProjectsCount);

	//��Ƭ��ʶ
	memcpy(szProjectsCount, szBlockData + 30, 2);
	m_cardData.iFlag = atoi((char*)szProjectsCount);
	if(m_cardData.iFlag == 1)
	{
	}
	else if(m_cardData.iFlag == 0)
	{
		AfxMessageBox("�ÿ�Ϊδ���˻���Ƭ��");
		return;
	}
	else
	{
		AfxMessageBox("��Ƭ״̬����ȷ�����飡");
		return;
	}


	///////////////////////////////////////////��7��������,3�������Ŀ
	////��Ƭ��λ
	//if((iRet = rf.Dev_Reset(1)))
	//{
	//	strTemp.Format("��Ƭ��λʧ�ܣ�����ֵ��%d", iRet);
	//	AfxMessageBox(strTemp);
	//	return;
	//}

	////�õ�����Կ
	////��Ϣ����Ϊ28 29 30
	//iBlock = 28;
	//memset(szSectorKey, 0, 40);

	//memcpy(szSectorKey, m_KeyData + (iBlock/4)*24, 12);
	//if(rf.LoadKey(rf.KEY_L, iBlock/4, szSectorKey, rf.HEX))
	//{
	//	AfxMessageBox("3��֤��Կʧ�ܣ�");
	//	return;
	//}

	//if(rf.Anthentication(rf.KEY_L, iBlock/4))
	//{
	//	AfxMessageBox("4��֤��Կʧ�ܣ�");
	//	return;
	//}
	////��֤���

	////��28��
	//memset(szBlockData, 0, 40);
	//if(rf.ReadMifareBlock(iBlock, szBlockData, rf.HEX))
	//{
	//	AfxMessageBox("����Ϣʧ�ܣ����鿨Ƭ��");
	//	return;
	//}
	////28���������01 �����������
	//if(!memcmp(szBlockData, "01", 2))
	//{
	//	//��������
	//	memset(szProjectTimes, 0, 5);
	//	memset(szProjectName, 0, 40);
	//	memcpy(szProjectTimes, szBlockData + 2, 2);
	//	m_cardData.iProjectTimes1 = rf.Hex2Dec(szProjectTimes);
	//	strTemp.Format("%d", m_cardData.iProjectTimes1);
	//	m_EdtProjectTimes1.SetWindowTextA(strTemp);//��Ŀ1 ʣ�����

	//	iDataLen = 28;
	//	memcpy(szProjectName, szBlockData + 4, iDataLen);
	//	rf.SwitchDataMode(szProjectName, &iDataLen, rf.HEX2ASC);
	//	strTemp.Format("%s",szProjectName);
	//	strTemp.Remove(0x20);
	//	memcpy(m_cardData.szProject1, strTemp, strTemp.GetLength());
	//	m_EdtProject1.SetWindowTextA((LPCTSTR)strTemp);//��Ŀ����
	//}
	//
	////��29��
	//iBlock = 29;
	//memset(szBlockData, 0, 40);
	//if(rf.ReadMifareBlock(iBlock, szBlockData, rf.HEX))
	//{
	//	AfxMessageBox("����Ϣʧ�ܣ����鿨Ƭ��");
	//	return;
	//}
	////29���������02 �����������
	//if(!memcmp(szBlockData, "02", 2))
	//{
	//	//��������
	//	memset(szProjectTimes, 0, 5);
	//	memset(szProjectName, 0, 40);
	//	memcpy(szProjectTimes, szBlockData + 2, 2);
	//	m_cardData.iProjectTimes2 = rf.Hex2Dec(szProjectTimes);
	//	strTemp.Format("%d", m_cardData.iProjectTimes2);
	//	m_EdtProjectTimes2.SetWindowTextA(strTemp);//��Ŀ2 ʣ�����

	//	iDataLen = 28;
	//	memcpy(szProjectName, szBlockData + 4, iDataLen);
	//	rf.SwitchDataMode(szProjectName, &iDataLen, rf.HEX2ASC);
	//	strTemp.Format("%s",szProjectName);
	//	strTemp.Remove(0x20);
	//	memcpy(m_cardData.szProject2, strTemp, strTemp.GetLength());
	//	m_EdtProject2.SetWindowTextA((LPCTSTR)strTemp);//��Ŀ����
	//}

	////��30��
	//iBlock = 30;
	//memset(szBlockData, 0, 40);
	//if(rf.ReadMifareBlock(iBlock, szBlockData, rf.HEX))
	//{
	//	AfxMessageBox("����Ϣʧ�ܣ����鿨Ƭ��");
	//	return;
	//}
	////29���������03 �����������
	//if(!memcmp(szBlockData, "03", 2))
	//{
	//	//��������
	//	memset(szProjectTimes, 0, 5);
	//	memset(szProjectName, 0, 40);
	//	memcpy(szProjectTimes, szBlockData + 2, 2);
	//	m_cardData.iProjectTimes3 = rf.Hex2Dec(szProjectTimes);
	//	strTemp.Format("%d", m_cardData.iProjectTimes3);
	//	m_EdtProjectTimes3.SetWindowTextA(strTemp);//��Ŀ3 ʣ�����

	//	iDataLen = 28;
	//	memcpy(szProjectName, szBlockData + 4, iDataLen);
	//	rf.SwitchDataMode(szProjectName, &iDataLen, rf.HEX2ASC);
	//	strTemp.Format("%s",szProjectName);
	//	strTemp.Remove(0x20);
	//	memcpy(m_cardData.szProject3, strTemp, strTemp.GetLength());
	//	m_EdtProject3.SetWindowTextA((LPCTSTR)strTemp);//��Ŀ����
	//}

	m_isInitialized = TRUE;

}

int CMifarePurseDlg::GetKeyData(unsigned char* pszKeyData)
{	
	CFile file;
	CString strTemp;
	CDES des;
	unsigned char szDesKey[40], szCardKey[512], szKeyData[512];
	int iDataLen = 0, iKeyLen;

	memset(szKeyData, 0, 512);
	file.Open(INI_FILE, CFile::modeRead);
	iKeyLen = file.Read(szKeyData, 500);
	if(iKeyLen < 0)
	{
		return -1;
	}
	file.Close();

	//�õ�������Կ
	memset(szDesKey, 0, 40);
	memset(szCardKey, 0, 512);
	rf.Hex2Asc((unsigned char*)MAIN_KEY, szDesKey, 32);
	des.RunDes(des.DECRYPT, des.ECB, (char*)szKeyData, (char*)szCardKey, iKeyLen, (char*)szDesKey, 16);
	rf.Asc2Hex(szCardKey, pszKeyData, iKeyLen);
	iKeyLen = iKeyLen*2;

	pszKeyData[iKeyLen] = 0;

	return 0;
}


void CMifarePurseDlg::OnBnClickedBtnClose()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDialog::OnCancel();
}
