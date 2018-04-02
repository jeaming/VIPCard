
// MifareDebitDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MifareDebit.h"
#include "MifareDebitDlg.h"
#include "afxdialogex.h"
#include "DES.h"
#include "RFReader.h"
#include "VerifyPINDlg.h"
//#include "Commdlg32.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


///////////////////////////////////////////////////SELF DEFINATION
CRFReader rf;



#define MAIN_KEY		"12345678900987654321ABCDEFFEDCBA"
#define		INI_FILE	".\\Set.dat"
#define		DEFAULT_KEY	"FFFFFFFFFFFF"
#define MIFARE_KEY_LEN		384

#define		WIDTH	 30
#define		HEIGHT	60

#define BLOCK_LEN	40
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
public:
	virtual BOOL OnInitDialog();
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


// CMifareDebitDlg �Ի���




CMifareDebitDlg::CMifareDebitDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMifareDebitDlg::IDD, pParent)
	, m_iCardStatus(0)
	, m_isInitialized(FALSE)
	, m_iProjects(0)
	, m_iListCount(0)
	, m_iProjectsCount(0)
	, m_iProjectLeft(0)
	, m_strLog(_T(""))
	, m_hasLoadProject(0)
	, m_iBeginChangeName(0)
{
	//m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_hIcon = AfxGetApp()->LoadIcon(IDI_ICON_LOAD);
}

void CMifareDebitDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BTN_BEGIN, m_BtnBegin);
	//  DDX_Control(pDX, IDC_BTN_LOAD, m_BtnClose);
	DDX_Control(pDX, IDC_EDIT_BALANCE, m_EdtBalance);
	DDX_Control(pDX, IDC_EDIT_CARDNO, m_EdtCardNO);
	DDX_Control(pDX, IDC_EDIT_NAME, m_EdtName);
	DDX_Control(pDX, IDC_EDIT_PHONENO, m_EdtPhoneNO);
	DDX_Control(pDX, IDC_EDIT_PROJECT1, m_EdtProject1);
	DDX_Control(pDX, IDC_EDIT_PROJECT2, m_EdtProject2);
	DDX_Control(pDX, IDC_EDIT_PROJECT3, m_EdtProject3);
	//  DDX_Control(pDX, IDC_EDIT_TIMES1, m_EdtTimes);
	DDX_Control(pDX, IDC_EDIT_TIMES1, m_EdtTimes1);
	DDX_Control(pDX, IDC_EDIT_TIMES2, m_EdtTimes2);
	DDX_Control(pDX, IDC_EDIT_TIMES3, m_EdtTimes3);
	DDX_Control(pDX, IDC_EDIT_VIPCARDNO, m_EdtVIPCardNO);
	DDX_Control(pDX, IDC_ST_BALANCE, m_StBalance);
	DDX_Control(pDX, IDC_ST_TIMES1, m_StTimes1);
	DDX_Control(pDX, IDC_ST_TIMES2, m_StTimes2);
	DDX_Control(pDX, IDC_ST_TIMES3, m_StTimes3);
	DDX_Control(pDX, IDC_BTN_LOAD, m_BtnLoad);
	DDX_Control(pDX, IDC_STATIC_DISP, m_StDisp);
	DDX_Control(pDX, IDC_COMBO_PROJECTLIST, m_CmbProject);
	DDX_Control(pDX, IDC_EDIT_PROJECT10, m_EdtProject10);
	DDX_Control(pDX, IDC_EDIT_PROJECT4, m_EdtProject4);
	DDX_Control(pDX, IDC_EDIT_PROJECT5, m_EdtProject5);
	DDX_Control(pDX, IDC_EDIT_PROJECT6, m_EdtProject6);
	DDX_Control(pDX, IDC_EDIT_PROJECT7, m_EdtProject7);
	DDX_Control(pDX, IDC_EDIT_PROJECT8, m_EdtProject8);
	DDX_Control(pDX, IDC_EDIT_PROJECT9, m_EdtProject9);
	DDX_Control(pDX, IDC_EDIT_TIMES10, m_EdtTimes10);
	DDX_Control(pDX, IDC_EDIT_TIMES4, m_EdtTimes4);
	DDX_Control(pDX, IDC_EDIT_TIMES5, m_EdtTimes5);
	DDX_Control(pDX, IDC_EDIT_TIMES6, m_EdtTimes6);
	DDX_Control(pDX, IDC_EDIT_TIMES7, m_EdtTimes7);
	DDX_Control(pDX, IDC_EDIT_TIMES8, m_EdtTimes8);
	DDX_Control(pDX, IDC_EDIT_TIMES9, m_EdtTimes9);
	DDX_Control(pDX, IDC_BTN_ADDP, m_BtnAddProject);
	DDX_Control(pDX, IDC_BTN_REMOVEP, m_BtnRemoveProject);
	DDX_Control(pDX, IDC_EDIT_TITLE1, m_EdtTitles1);
	DDX_Control(pDX, IDC_EDIT_TITLE2, m_EdtTitles2);
	DDX_Control(pDX, IDC_EDIT_TITLE3, m_EdtTitles3);
	DDX_Control(pDX, IDC_EDIT_TITLE4, m_EdtTitles4);
	DDX_Control(pDX, IDC_BTN_REMOVE_EMPTY, m_BtnRmvEmpty);
	DDX_Control(pDX, IDC_BTN_CHANGE_NAME_DO, m_BtnDoChange);
	DDX_Control(pDX, IDC_EDIT_OPERATOR, m_EdtOperator);
}

BEGIN_MESSAGE_MAP(CMifareDebitDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_BEGIN, &CMifareDebitDlg::OnBnClickedBtnBegin)
	ON_BN_CLICKED(IDC_BTN_LOAD, &CMifareDebitDlg::OnBnClickedBtnLoad)
	ON_BN_CLICKED(IDC_BTN_CARDINFO, &CMifareDebitDlg::OnBnClickedBtnCardinfo)
	ON_BN_CLICKED(IDC_BTN_CLOSE, &CMifareDebitDlg::OnBnClickedBtnClose)
	ON_BN_CLICKED(IDC_BTN_ADDP, &CMifareDebitDlg::OnBnClickedBtnAddp)
	ON_BN_CLICKED(IDC_BTN_REMOVEP, &CMifareDebitDlg::OnBnClickedBtnRemovep)
	ON_BN_CLICKED(IDC_BTN_REMOVE_EMPTY, &CMifareDebitDlg::OnBnClickedBtnRemoveEmpty)
	ON_BN_CLICKED(IDC_BTN_CHANGE_NAME, &CMifareDebitDlg::OnBnClickedBtnChangeName)
	ON_BN_CLICKED(IDC_BTN_CHANGE_NAME_DO, &CMifareDebitDlg::OnBnClickedBtnChangeNameDo)
	ON_BN_CLICKED(IDC_BTN_CHANGE_NAME_CANCEL, &CMifareDebitDlg::OnBnClickedBtnChangeNameCancel)
	ON_BN_CLICKED(IDC_BUTTON_TEST, &CMifareDebitDlg::OnBnClickedButtonTest)
END_MESSAGE_MAP()


// CMifareDebitDlg ��Ϣ�������

BOOL CMifareDebitDlg::OnInitDialog()
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
	if(GetKeyData(m_KeyData) != 0)
	{
		AfxMessageBox("��ʼ�������ļ�ʧ�ܣ�");
		exit(1);
	}

	CVerifyPINDlg ver(m_KeyData);
	if(ver.DoModal() == IDCANCEL)
	{
		exit(0);
	}

	//���Ӷ�д��
	if(!rf.m_isConnected)
	{
		if(rf.Dev_Connect(100, 115200))
		{
			m_StDisp.SetWindowTextA("�򿪶�д��ʧ�ܣ������д�����ӣ�");	
		}
	}
	else
		m_StDisp.SetWindowTextA("��д��������.");

	m_EdtTitles1.SetWindowTextA("	��Ŀ����");
	m_EdtTitles3.SetWindowTextA("	��Ŀ����");
	m_EdtTitles2.SetWindowTextA("     ����");
	m_EdtTitles4.SetWindowTextA("     ����");

	if(SetProjectList())
		AfxMessageBox("ȡ��Ŀ�б�ʧ�ܣ����������ļ���");

	//m_CmbProject.InsertString(0, "��Ŀ�б�");
	m_CmbProject.SetCurSel(1);
	m_CmbProject.EnableWindow(FALSE);
	m_BtnAddProject.EnableWindow(FALSE);
	m_BtnRemoveProject.EnableWindow(FALSE);

	m_EdtCardNO.LimitText(12);
	m_EdtVIPCardNO.LimitText(12);
	m_EdtName.LimitText(10);
	m_EdtPhoneNO.LimitText(12);
	m_EdtBalance.LimitText(9);
	
	m_EdtTimes1.LimitText(3);
	m_EdtTimes2.LimitText(3);
	m_EdtTimes3.LimitText(3);
	m_EdtTimes4.LimitText(3);
	m_EdtTimes5.LimitText(3);
	m_EdtTimes6.LimitText(3);
	m_EdtTimes7.LimitText(3);
	m_EdtTimes8.LimitText(3);
	m_EdtTimes9.LimitText(3);
	m_EdtTimes10.LimitText(3);

	m_EdtOperator.LimitText(10);


	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CMifareDebitDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CMifareDebitDlg::OnPaint()
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
HCURSOR CMifareDebitDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CMifareDebitDlg::OnBnClickedBtnBegin()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	
	if(!m_isInitialized)
	{
		AfxMessageBox("���Ȼ�ȡ��Ƭ��Ϣ��");
		return;
	}

	if(m_iCardStatus == 0)
	{//��ʼ��
		m_StBalance.SetWindowTextA("��ֵ��");
		m_EdtName.EnableWindow(TRUE);
		m_EdtPhoneNO.EnableWindow(TRUE);
		m_EdtVIPCardNO.EnableWindow(TRUE);
		m_EdtBalance.EnableWindow(TRUE);
		m_EdtCardNO.EnableWindow(TRUE);
		m_EdtName.SetWindowTextA("");
		m_EdtPhoneNO.SetWindowTextA("");
		m_EdtVIPCardNO.SetWindowTextA("");
		m_EdtBalance.SetWindowTextA("");
		m_EdtCardNO.SetWindowTextA("");

		////////////temp
		//m_EdtCardNO.SetWindowTextA("888888888888");
		//m_EdtName.SetWindowTextA("����");
		//m_EdtPhoneNO.SetWindowTextA("13810001567");
		//m_EdtVIPCardNO.SetWindowTextA("12345678");
		//m_EdtBalance.SetWindowTextA("1000");

		/////////////////////////////////
		
		//m_EdtProject1.EnableWindow(TRUE);
		//m_EdtProject2.EnableWindow(TRUE);
		//m_EdtProject3.EnableWindow(TRUE);
		//m_EdtProject4.EnableWindow(TRUE);
		//m_EdtProject5.EnableWindow(TRUE);
		//m_EdtProject6.EnableWindow(TRUE);
		//m_EdtProject7.EnableWindow(TRUE);
		//m_EdtProject8.EnableWindow(TRUE);
		//m_EdtProject9.EnableWindow(TRUE);
		//m_EdtProject10.EnableWindow(TRUE);
		m_EdtTimes1.EnableWindow(TRUE);
		m_EdtTimes2.EnableWindow(TRUE);
		m_EdtTimes3.EnableWindow(TRUE);
		m_EdtTimes4.EnableWindow(TRUE);
		m_EdtTimes5.EnableWindow(TRUE);
		m_EdtTimes6.EnableWindow(TRUE);
		m_EdtTimes7.EnableWindow(TRUE);
		m_EdtTimes8.EnableWindow(TRUE);
		m_EdtTimes9.EnableWindow(TRUE);
		m_EdtTimes10.EnableWindow(TRUE);

	}
	else if(m_iCardStatus == 1)
	{//��ֵ

		//iProjects = m_iProjects;
		m_iProjects = m_iProjectsCount;
		//���ý������
		m_StBalance.SetWindowTextA("�����");
		m_EdtBalance.EnableWindow(TRUE);
		m_EdtBalance.SetWindowTextA("0");
		m_CmbProject.EnableWindow(FALSE);

		if(m_iProjects >= 1)
		{
			m_EdtTimes1.EnableWindow(TRUE);
			m_EdtTimes1.SetWindowTextA("0");
		}
		if(m_iProjects >= 2)
		{
			m_EdtTimes2.EnableWindow(TRUE);
			m_EdtTimes2.SetWindowTextA("0");
		}
		if(m_iProjects >= 3)
		{
			m_EdtTimes3.EnableWindow(TRUE);
			m_EdtTimes3.SetWindowTextA("0");
		}
		if(m_iProjects >= 4)
		{
			m_EdtTimes4.EnableWindow(TRUE);
			m_EdtTimes4.SetWindowTextA("0");
		}
		if(m_iProjects >= 5)
		{
			m_EdtTimes5.EnableWindow(TRUE);
			m_EdtTimes5.SetWindowTextA("0");
		}
		if(m_iProjects >= 6)
		{
			m_EdtTimes6.EnableWindow(TRUE);
			m_EdtTimes6.SetWindowTextA("0");
		}
		if(m_iProjects >= 7)
		{
			m_EdtTimes7.EnableWindow(TRUE);
			m_EdtTimes7.SetWindowTextA("0");
		}
		if(m_iProjects >= 8)
		{
			m_EdtTimes8.EnableWindow(TRUE);
			m_EdtTimes8.SetWindowTextA("0");
		}
		if(m_iProjects >= 9)
		{
			m_EdtTimes9.EnableWindow(TRUE);
			m_EdtTimes9.SetWindowTextA("0");
		}
		if(m_iProjects >= 10)
		{
			m_EdtTimes10.EnableWindow(TRUE);
			m_EdtTimes10.SetWindowTextA("0");
		}

	}

	if(m_iProjectLeft == 0)
		m_BtnAddProject.EnableWindow(FALSE);
	else
	{
		m_CmbProject.EnableWindow(TRUE);
		m_BtnAddProject.EnableWindow(TRUE);
	}

	m_isInitialized = FALSE;
	m_BtnLoad.EnableWindow(TRUE);
	m_BtnBegin.EnableWindow(FALSE);
}

void CMifareDebitDlg::OnBnClickedBtnLoad()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	CString strTemp;
	unsigned char szAmount[10];
	unsigned char szSectorKey[40];
	unsigned char szBlockData[40];
	unsigned char szOperator[20];
	int iDataLen = 0;
	long lAmount = 0;
	int iTimes1 = 0;
	int iTimes2 = 0;
	int iTimes3 = 0;
	int iProjects = 0;
	int iBlock = 0;
	int iRet = 0;

	m_hasLoadProject = 0;

	memset(szOperator, 0, sizeof(szOperator));
	m_EdtOperator.GetWindowTextA((LPTSTR)szOperator, 20);
	if(strlen((char*)szOperator) == 0)
	{
		AfxMessageBox("����Ա��������Ϊ�գ�");
		return;
	}

	//���ݴ���
	if(m_iCardStatus == 0)
	{//��ʼ��

		m_EdtBalance.GetWindowTextA((LPTSTR)szAmount, 8);
		iDataLen = strlen((char*)szAmount);
		for(int i = 0; i != iDataLen; ++i)
		{
			if(szAmount[i] > 0x39 || szAmount[i] < 0x30)
			{
				AfxMessageBox("������ݲ���Ϊ0-9���������ֵ��");
				m_StDisp.SetWindowTextA("������ݲ���Ϊ0-9���������ֵ��");
				return;
			}
		}
		m_cardData.iAmount = atoi((char*)szAmount);/////////�õ����
		strTemp.Format("�����˻�������Ϣ��\r\n");

		//��־��ʼ
		//ʱ��
		char szTime[32];
		time_t t0 = time(0);
		memset(szTime, 0, 32);
		strftime(szTime, 32, "%Y%m%d%H%M%S", localtime(&t0));
		m_strLog.Format("%s", szTime);

		m_strLog.AppendFormat("	%s", szOperator);
		/////////�õ�����
		m_EdtName.GetWindowTextA((LPTSTR)m_cardData.szName, 21);
		strTemp.AppendFormat("������%s\t", m_cardData.szName);
		/////////�õ���ϵ�绰
		m_EdtPhoneNO.GetWindowTextA((LPTSTR)m_cardData.szPhoneNO, 13);
		iDataLen = strlen((char*)m_cardData.szPhoneNO);
		for(int i = 0; i != iDataLen; ++i)
		{
			if(m_cardData.szPhoneNO[i] > 0x39 || m_cardData.szPhoneNO[i] < 0x30)
			{
				AfxMessageBox("��ϵ�绰���ݲ���Ϊ0-9���������ֵ��");
				m_StDisp.SetWindowTextA("��ϵ�绰���ݲ���Ϊ0-9���������ֵ��");
				return;
			}
		}
		strTemp.AppendFormat("��ϵ�绰��%s \r\n", m_cardData.szPhoneNO);

		//����������
		m_EdtCardNO.GetWindowTextA((LPTSTR)m_cardData.szCardNO, 13);
		if(CheckNumberData(m_cardData.szCardNO))
		{
			AfxMessageBox("���������������������飡");
			m_StDisp.SetWindowTextA("���������������������飡");
			return;
		}
		strTemp.AppendFormat("���������ţ�%s\t", m_cardData.szCardNO);

		//VIP����
		m_EdtVIPCardNO.GetWindowTextA((LPTSTR)m_cardData.szVIPCardNO, 13);
		if(CheckNumberData(m_cardData.szVIPCardNO))
		{
			AfxMessageBox("VIP���������������飡");
			m_StDisp.SetWindowTextA("VIP���������������飡");
			return;
		}
		strTemp.AppendFormat("VIP���ţ�%s\r\n", m_cardData.szVIPCardNO);
		strTemp.AppendFormat("��ʼ��%s\r\n", szAmount);
		
		m_strLog.AppendFormat("	%s	%s", m_cardData.szCardNO, m_cardData.szName);
		m_strLog.AppendFormat("	%s	%s", m_cardData.szVIPCardNO, m_cardData.szPhoneNO);//��־
		m_strLog.AppendFormat("	%d", m_cardData.iAmount);

		//ProjectData project[10];
		//memset(project, 0, sizeof(project));
		for(int i = 1; i != 11-m_iProjectLeft; ++i)
		{
			if(GetProjectData(&project[i - 1], i))
				return;
			project[i-1].iTimes = 0;
			strTemp.AppendFormat("��Ŀ%d��%s	��ֵ������%d \r\n", i, project[i - 1].szProjectName, project[i - 1].iAddTimes);
			m_strLog.AppendFormat("	%s	%d	%d",	project[i - 1].szProjectName, project[i-1].iAddTimes, project[i-1].iTimes + project[i-1].iAddTimes);
		}

		////�õ���Ŀ����1
		//memset(szTimes1, 0, 10);
		//m_EdtProject1.GetWindowTextA((LPTSTR)m_cardData.szProject1, 15);
		//m_EdtTimes1.GetWindowTextA((LPTSTR)szTimes1, 4);
		//if(iDataLen = strlen((char*)m_cardData.szProject1) > 0)
		//{
		//	if(CheckNumberData(szTimes1))
		//	{
		//		AfxMessageBox("��Ŀ1�����������飡");
		//		m_StDisp.SetWindowTextA("��Ŀ1�����������飡");
		//		return;
		//	}
		//	
		//	m_cardData.iProjectTimes3 = atoi((char*)szTimes1);
		//	strTemp.AppendFormat("��Ŀ1: %s\t ��ʼ������%s\r\n", m_cardData.szProject1, szTimes1);
		//	iProjects = 1;
		//}
		//
		////�õ���Ŀ����2
		//memset(szTimes2, 0, 10);
		//m_EdtProject2.GetWindowTextA((LPTSTR)m_cardData.szProject2, 15);
		//m_EdtTimes2.GetWindowTextA((LPTSTR)szTimes2, 4);
		//if((iDataLen = strlen((char*)m_cardData.szProject2) > 0))
		//{
		//	if(CheckNumberData(szTimes2))
		//	{
		//		AfxMessageBox("��Ŀ2�����������飡");
		//		m_StDisp.SetWindowTextA("��Ŀ2�����������飡");
		//		return;
		//	}
		//	m_cardData.iProjectTimes3 = atoi((char*)szTimes2);
		//	strTemp.AppendFormat("��Ŀ2: %s\t ��ʼ������%s\r\n", m_cardData.szProject2, szTimes2);
		//	iProjects = iProjects*10 + 2;
		//}

		////�õ���Ŀ����3
		//memset(szTimes3, 0, 10);
		//m_EdtProject3.GetWindowTextA((LPTSTR)m_cardData.szProject3, 15);
		//m_EdtTimes3.GetWindowTextA((LPTSTR)szTimes3, 4);
		//if((iDataLen = strlen((char*)m_cardData.szProject3) > 0))
		//{
		//	if(CheckNumberData(szTimes3))
		//	{
		//		AfxMessageBox("��Ŀ3�����������飡");
		//		m_StDisp.SetWindowTextA("��Ŀ3�����������飡");
		//		return;
		//	}

		//	m_cardData.iProjectTimes3 = atoi((char*)szTimes3);
		//	strTemp.AppendFormat("��Ŀ3: %s\t ��ʼ������%s\r\n", m_cardData.szProject3, szTimes3);
		//	iProjects = iProjects*10 +3;
		//}

		if(MessageBox((LPCTSTR)strTemp, (LPCTSTR)"����ȷ��", MB_OKCANCEL) == IDCANCEL)
			return;

		WriteInitLog(m_strLog);

		strTemp.Format("��һ��<%s>��ʼ��ʧ�ܣ�", szTime);

		//��ʼд������
		iBlock = 20;
		memset(szSectorKey, 0, 40);
		memcpy(szSectorKey, m_KeyData + (iBlock/4*24 + 12), 12);
		if(rf.Dev_Reset(1))
		{
			AfxMessageBox("��Ƭ��λʧ�ܣ�");
			m_StDisp.SetWindowTextA("��Ƭ��λʧ�ܣ�");
			return;
		}

		if(rf.LoadKey(rf.KEY_R, iBlock/4, szSectorKey, rf.HEX))
		{
			AfxMessageBox("��д��װ����Կʧ�ܣ�");
			m_StDisp.SetWindowTextA("��д��װ����Կʧ�ܣ�");
			return;
		}
		if(rf.Anthentication(rf.KEY_R, iBlock/4))
		{
			AfxMessageBox("��֤��Կʧ�ܣ�");
			m_StDisp.SetWindowTextA("��֤��Կʧ�ܣ�");
			return;
		}//��֤���

		if(rf.InitialValue(iBlock, m_cardData.iAmount))
		{//д����
			AfxMessageBox("д����ʧ�ܣ�");
			m_StDisp.SetWindowTextA("д����ʧ�ܣ�");
			return;			
		}

		iBlock = 21;////////д������
		iDataLen = strlen((char*)m_cardData.szName);
		while(iDataLen < 20)
		{
			memcpy(m_cardData.szName + iDataLen,  " ", 1);
			iDataLen += 2;
		}
		iDataLen = 10;
		rf.SwitchDataMode(m_cardData.szName, &iDataLen, rf.ASC2HEX);
		memcpy(m_cardData.szName + iDataLen, "000000000000", 12);
		iDataLen = strlen((char*)m_cardData.szPhoneNO);
		memcpy(m_cardData.szName - iDataLen + 32, m_cardData.szPhoneNO, iDataLen);
		m_cardData.szName[32] = 0;
		if(rf.Write(iBlock, m_cardData.szName, rf.HEX))
		{
			AfxMessageBox("д��������Ϣʧ�ܣ�");
			m_StDisp.SetWindowTextA("д��������Ϣʧ�ܣ�");
			strTemp += "д��������Ϣʧ��";
			WriteLog(strTemp);
			return;	
		}

		iBlock = 22;//��ϵ�绰�����š�VIP���š�״̬
		memset(szBlockData, 0x30, sizeof(szBlockData));/*
		iDataLen = strlen((char*)m_cardData.szPhoneNO);
		memcpy(szBlockData + (12 - iDataLen), m_cardData.szPhoneNO, iDataLen);*/

		memset(szBlockData, 0x30, 32);
		iDataLen = strlen((char*)m_cardData.szCardNO);
		memcpy(szBlockData + 12 - iDataLen, m_cardData.szCardNO, iDataLen);
		iDataLen = strlen((char*)m_cardData.szVIPCardNO);
		memcpy(szBlockData + 24 - iDataLen, m_cardData.szVIPCardNO, iDataLen);
		strTemp.Format("%02X", 10 - m_iProjectLeft);
		memcpy(szBlockData + 24, strTemp, 2);
		memcpy(szBlockData + 30, "01", 2);
		szBlockData[32] = 0;
		if(rf.Write(iBlock, szBlockData, rf.HEX))
		{
			AfxMessageBox("д��VIP������Ϣʧ�ܣ�");
			m_StDisp.SetWindowTextA("д��VIP������Ϣʧ�ܣ�");
			strTemp += "д��VIP������Ϣʧ��";
			WriteLog(strTemp);
			return;	
		}

		for(int i = 1; i != 11 -m_iProjectLeft; ++i)
		{
			iRet = WriteProjectBlock(&project[i - 1]);
			if(iRet)
			{
				strTemp.Format("д��Ŀ%dʧ�ܣ������ԣ�", i);
				AfxMessageBox(strTemp);
				strTemp.Format("��һ��<%s>��ʼ��ʧ�ܣ�д��Ŀ%dʧ��", szTime, i);
				WriteLog(strTemp);
				return;
			}
		}

		///////д����Ŀ��Ϣ
		////7����������Ŀ
		////��֤7����
		//iBlock = 28;
		//memset(szSectorKey, 0, 40);
		//memcpy(szSectorKey, m_KeyData + iBlock/4*24 + 12, 12);
		//if(rf.Dev_Reset(1))
		//{
		//	AfxMessageBox("δ�ҵ���Ƭ�����飡");
		//	m_StDisp.SetWindowTextA("δ�ҵ���Ƭ�����飡");
		//	return;
		//}

		//if(rf.LoadKey(rf.KEY_R, iBlock/4, szSectorKey, rf.HEX))
		//{
		//	AfxMessageBox("��д��װ����Կʧ�ܣ�");
		//	m_StDisp.SetWindowTextA("��д��װ����Կʧ�ܣ�");
		//	return;
		//}
		//if(rf.Anthentication(rf.KEY_R, iBlock/4))
		//{
		//	AfxMessageBox("��֤��Կʧ�ܣ�");
		//	m_StDisp.SetWindowTextA("��֤��Կʧ�ܣ�");
		//	return;
		//}
		////��֤���

		//if(iProjects %10 == 3)
		//{
		//	iBlock = 30;
		//	memset(szBlockData, 0, 40);
		//	//m_cardData.iProjectTimes3 = rf.Hex2Dec(szTimes3);
		//	m_cardData.iProjectTimes3 = atoi((char*)szTimes3);
		//	strTemp.Format("%02X", m_cardData.iProjectTimes3);
		//	memcpy(szBlockData, "03", 2);
		//	memcpy(szBlockData + 2, strTemp, 2);

		//	iDataLen = strlen((char*)m_cardData.szProject3);
		//	rf.SwitchDataMode(m_cardData.szProject3, &iDataLen, rf.ASC2HEX);
		//	memcpy(szBlockData + 4, m_cardData.szProject3, iDataLen);
		//	iDataLen = strlen((char*)szBlockData);
		//	while(iDataLen != 32)
		//	{
		//		memcpy(szBlockData + iDataLen, "20", 2);
		//		iDataLen += 2;
		//	}
		//	if(rf.Write(iBlock, szBlockData, rf.HEX))
		//	{
		//		AfxMessageBox("��ֵʧ��(3)!");
		//		m_StDisp.SetWindowTextA("��ֵʧ��(3)!");
		//		return;
		//	}
		//	iProjects /= 10;
		//}

		//if(iProjects %10 == 2)
		//{
		//	iBlock = 29;
		//	memset(szBlockData, 0, 40);
		//	//m_cardData.iProjectTimes2 = rf.Hex2Dec(szTimes2);
		//	m_cardData.iProjectTimes2 = atoi((char*)szTimes2);
		//	strTemp.Format("%02X", m_cardData.iProjectTimes2);
		//	memcpy(szBlockData, "02", 2);
		//	memcpy(szBlockData + 2, strTemp, 2);

		//	iDataLen = strlen((char*)m_cardData.szProject2);
		//	rf.SwitchDataMode(m_cardData.szProject2, &iDataLen, rf.ASC2HEX);
		//	memcpy(szBlockData + 4, m_cardData.szProject2, iDataLen);
		//	iDataLen = strlen((char*)szBlockData);
		//	while(iDataLen != 32)
		//	{
		//		memcpy(szBlockData + iDataLen, "20", 2);
		//		iDataLen += 2;
		//	}
		//	if(rf.Write(iBlock, szBlockData, rf.HEX))
		//	{
		//		AfxMessageBox("��ֵʧ��(2)!");
		//		m_StDisp.SetWindowTextA("��ֵʧ��(2)!");
		//		return;
		//	}
		//	iProjects /= 10;
		//}

		//if(iProjects == 1)
		//{
		//	iBlock = 28;
		//	memset(szBlockData, 0, 40);
		//	//m_cardData.iProjectTimes1 = rf.Hex2Dec(szTimes1);
		//	m_cardData.iProjectTimes1 = atoi((char*)szTimes1);
		//	strTemp.Format("%02X", m_cardData.iProjectTimes1);
		//	memcpy(szBlockData, "01", 2);
		//	memcpy(szBlockData + 2, strTemp, 2);

		//	iDataLen = strlen((char*)m_cardData.szProject1);
		//	rf.SwitchDataMode(m_cardData.szProject1, &iDataLen, rf.ASC2HEX);
		//	memcpy(szBlockData + 4, m_cardData.szProject1, iDataLen);
		//	iDataLen = strlen((char*)szBlockData);
		//	while(iDataLen != 32)
		//	{
		//		memcpy(szBlockData + iDataLen, "20", 2);
		//		iDataLen += 2;
		//	}
		//	if(rf.Write(iBlock, szBlockData, rf.HEX))
		//	{
		//		AfxMessageBox("��ֵʧ��(3)!");
		//		m_StDisp.SetWindowTextA("��ֵʧ��(3)!");
		//		return;
		//	}
		//	iProjects /= 10;
		//}
		AfxMessageBox("�����ɹ���");
		m_StDisp.SetWindowTextA("�����ɹ���");
		//this->OnBnClickedBtnCardinfo();

	}
	else if(m_iCardStatus == 1)
	{
		//�������
		m_EdtBalance.GetWindowTextA((LPTSTR)szAmount, 7);
		iDataLen = strlen((char*)szAmount);
		for(int i = 0; i != iDataLen; ++i)
		{
			if(szAmount[i] > 0x39 || szAmount[i] < 0x30)
			{
				AfxMessageBox("������ݲ���Ϊ0-9���������ֵ��");
				m_StDisp.SetWindowTextA("������ݲ���Ϊ0-9���������ֵ��");
				return;
			}
		}
		lAmount = atoi((char*)szAmount);/////////�õ����

		strTemp.Format("�����������ݽ��г�ֵ��\r\n");
		strTemp.AppendFormat("���ţ�%s		������%s\r\n", m_cardData.szCardNO, m_cardData.szName);
		if(lAmount > 0)
		{
			strTemp.AppendFormat("��ֵ��%d\r\n", lAmount);
			m_hasLoadProject = 1;
		}
		iProjects = m_iProjects;

		//��־��ʼ
		//ʱ��
		char szTime[32];
		time_t t0 = time(0);
		memset(szTime, 0, 32);
		strftime(szTime, 32, "%Y%m%d%H%M%S", localtime(&t0));
		m_strLog.Format("%s", szTime);		
		m_strLog.AppendFormat("	%s", szOperator);
		m_strLog.AppendFormat("	%s	%s", m_cardData.szCardNO, m_cardData.szName);
		m_strLog.AppendFormat("	%s	%d	%d", m_cardData.szPhoneNO, m_cardData.iAmount, lAmount, m_cardData.iAmount + lAmount);//��־

		for(int i = 1; i != 11 - m_iProjectLeft; ++i)
		{
			if(GetProjectData(&project[i - 1], i))
				return;
			if(project[i-1].iAddTimes > 0)
			{
				strTemp.AppendFormat("��Ŀ%d��%s	��ֵ������%d \r\n", i, project[i - 1].szProjectName, project[i - 1].iAddTimes);
				m_strLog.AppendFormat("	%s	%d	%d", project[i-1].szProjectName, project[i - 1].iAddTimes, project[i -1].iTimes + project[i-1].iAddTimes);
				m_hasLoadProject = 1;
			}
		}

		//if(iProjects%10 == 3)
		//{
		//	//�õ���Ŀ����3
		//	memset(szTimes3, 0, 10);
		//	m_EdtTimes3.GetWindowTextA((LPTSTR)szTimes3, 4);
		//	if(CheckNumberData(szTimes3))
		//	{
		//		AfxMessageBox("��Ŀ3�����������飡");
		//		m_StDisp.SetWindowTextA("��Ŀ3�����������飡");
		//		return;
		//	}
		//	strTemp.AppendFormat("��Ŀ3��%s, ��ֵ������%s\r\n", m_cardData.szProject3, szTimes3);
		//	iProjects /= 10;
		//}
		//
		//if(iProjects%10 == 2)
		//{
		//	//�õ���Ŀ����2
		//	memset(szTimes2, 0, 10);
		//	m_EdtTimes2.GetWindowTextA((LPTSTR)szTimes2, 4);
		//	if(CheckNumberData(szTimes2))
		//	{
		//		AfxMessageBox("��Ŀ2�����������飡");
		//		m_StDisp.SetWindowTextA("��Ŀ2�����������飡");
		//		return;
		//	}
		//	strTemp.AppendFormat("��Ŀ2��%s, ��ֵ������%s\r\n", m_cardData.szProject2, szTimes2);
		//	iProjects /= 10;
		//}
		//if(iProjects == 1)
		//{
		//	//�õ���Ŀ����1
		//	memset(szTimes1, 0, 10);
		//	m_EdtTimes1.GetWindowTextA((LPTSTR)szTimes1, 4);
		//	if(CheckNumberData(szTimes1))
		//	{
		//		AfxMessageBox("��Ŀ1�����������飡");
		//		m_StDisp.SetWindowTextA("��Ŀ1�����������飡");
		//		return;
		//	}
		//	strTemp.AppendFormat("��Ŀ1��%s, ��ֵ������%s\r\n", m_cardData.szProject1, szTimes1);	
		//}

		if(!m_hasLoadProject)
		{
			AfxMessageBox("�޳�ֵ��Ŀ���˳���ֵ��");	
			this->OnBnClickedBtnCardinfo();
			m_StBalance.SetWindowTextA("��");
			m_EdtName.EnableWindow(FALSE);
			m_EdtPhoneNO.EnableWindow(FALSE);
			m_EdtVIPCardNO.EnableWindow(FALSE);
			m_EdtBalance.EnableWindow(FALSE);
			m_EdtProject1.EnableWindow(FALSE);
			m_EdtProject2.EnableWindow(FALSE);
			m_EdtProject3.EnableWindow(FALSE);
			m_EdtProject4.EnableWindow(FALSE);
			m_EdtProject5.EnableWindow(FALSE);
			m_EdtProject6.EnableWindow(FALSE);
			m_EdtProject7.EnableWindow(FALSE);
			m_EdtProject8.EnableWindow(FALSE);
			m_EdtProject9.EnableWindow(FALSE);
			m_EdtProject10.EnableWindow(FALSE);
			m_EdtTimes1.EnableWindow(FALSE);
			m_EdtTimes2.EnableWindow(FALSE);
			m_EdtTimes3.EnableWindow(FALSE);
			m_EdtTimes4.EnableWindow(FALSE);
			m_EdtTimes5.EnableWindow(FALSE);
			m_EdtTimes6.EnableWindow(FALSE);
			m_EdtTimes7.EnableWindow(FALSE);
			m_EdtTimes8.EnableWindow(FALSE);
			m_EdtTimes9.EnableWindow(FALSE);
			m_EdtTimes10 .EnableWindow(FALSE);

			m_BtnLoad.EnableWindow(FALSE);
			m_BtnBegin.EnableWindow(TRUE);
			return;
		}

		if(MessageBox((LPCTSTR)strTemp,(LPCTSTR)"��ֵ����ȷ��", MB_OKCANCEL) == IDCANCEL)
			return;

		//д��־
		WriteLog(m_strLog);

		strTemp.Format("��һ��<%s>��ʼ��ʧ�ܣ�", szTime);

		//��ʼд����
	//	//5������֤
	//	iBlock = 20;
	//	memset(szSectorKey, 0, 40);
	//	memcpy(szSectorKey, m_KeyData + iBlock/4*24 + 12, 12);
	//	if(rf.Dev_Reset(1))
	//	{
	//		AfxMessageBox("δ�ҵ���Ƭ�����飡");
	//		m_StDisp.SetWindowTextA("δ�ҵ���Ƭ�����飡");
	//		return;
	//	}

	//	if(rf.LoadKey(rf.KEY_R, iBlock/4, szSectorKey, rf.HEX))
	//	{
	//		AfxMessageBox("��д��װ����Կʧ�ܣ�");
	//		m_StDisp.SetWindowTextA("��д��װ����Կʧ�ܣ�");
	//		return;
	//	}
	//	if(rf.Anthentication(rf.KEY_R, iBlock/4))
	//	{
	//		AfxMessageBox("��֤��Կʧ�ܣ�");
	//		m_StDisp.SetWindowTextA("��֤��Կʧ�ܣ�");
	//		return;
	//	}

	//	//20���ֵ
	//	if(lAmount > 0)
	//	{
	//		if(rf.Increment(iBlock, lAmount))
	//		{
	//			strTemp.Format("��ֵ���ѽ��ʧ�ܣ�����ֵ��%d", iRet);
	//			AfxMessageBox(strTemp);
	//			m_StDisp.SetWindowTextA(strTemp);
	//			return;
	//		}
	//	}

	//	//7����������Ŀ
	//	//��֤7����
	//	iBlock = 28;
	//	memset(szSectorKey, 0, 40);
	//	memcpy(szSectorKey, m_KeyData + iBlock/4*24 + 12, 12);
	//	if(rf.Dev_Reset(1))
	//	{
	//		AfxMessageBox("δ�ҵ���Ƭ�����飡");
	//		m_StDisp.SetWindowTextA("δ�ҵ���Ƭ�����飡");
	//		return;
	//	}

	//	if(rf.LoadKey(rf.KEY_R, iBlock/4, szSectorKey, rf.HEX))
	//	{
	//		AfxMessageBox("��д��װ����Կʧ�ܣ�");
	//		m_StDisp.SetWindowTextA("��д��װ����Կʧ�ܣ�");
	//		return;
	//	}
	//	if(rf.Anthentication(rf.KEY_R, iBlock/4))
	//	{
	//		AfxMessageBox("��֤��Կʧ�ܣ�");
	//		m_StDisp.SetWindowTextA("��֤��Կʧ�ܣ�");
	//		return;
	//	}
	//	//��֤���
	//	iProjects = m_iProjects;
	//	if(iProjects %10 == 3)
	//	{
	//		iBlock = 30;
	//		memset(szBlockData, 0, 40);
	//		//iTimes3 = rf.Hex2Dec(szTimes3);
	//		iTimes3 = atoi((char*)szTimes3);
	//		strTemp.Format("%02X", m_cardData.iProjectTimes3 + iTimes3);
	//		memcpy(szBlockData, "03", 2);
	//		memcpy(szBlockData + 2, strTemp, 2);

	//		iDataLen = strlen((char*)m_cardData.szProject3);
	//		rf.SwitchDataMode(m_cardData.szProject3, &iDataLen, rf.ASC2HEX);
	//		memcpy(szBlockData + 4, m_cardData.szProject3, iDataLen);
	//		iDataLen = strlen((char*)szBlockData);
	//		while(iDataLen != 32)
	//		{
	//			memcpy(szBlockData + iDataLen, "20", 2);
	//			iDataLen += 2;
	//		}
	//		if(rf.Write(iBlock, szBlockData, rf.HEX))
	//		{
	//			AfxMessageBox("��ֵʧ��(3)!");
	//			m_StDisp.SetWindowTextA("��ֵʧ��(3)!");
	//			return;
	//		}
	//		iProjects /= 10;
	//	}

	//	if(iProjects %10 == 2)
	//	{
	//		iBlock = 29;
	//		memset(szBlockData, 0, 40);
	//		//iTimes2 = rf.Hex2Dec(szTimes2);
	//		iTimes2 = atoi((char*)szTimes2);
	//		strTemp.Format("%02X", m_cardData.iProjectTimes2 + iTimes2);
	//		memcpy(szBlockData, "02", 2);
	//		memcpy(szBlockData + 2, strTemp, 2);

	//		iDataLen = strlen((char*)m_cardData.szProject2);
	//		rf.SwitchDataMode(m_cardData.szProject2, &iDataLen, rf.ASC2HEX);
	//		memcpy(szBlockData + 4, m_cardData.szProject2, iDataLen);
	//		iDataLen = strlen((char*)szBlockData);
	//		while(iDataLen != 32)
	//		{
	//			memcpy(szBlockData + iDataLen, "20", 2);
	//			iDataLen += 2;
	//		}
	//		if(rf.Write(iBlock, szBlockData, rf.HEX))
	//		{
	//			AfxMessageBox("��ֵʧ��(2)!");
	//			m_StDisp.SetWindowTextA("��ֵʧ��(2)!");
	//			return;
	//		}
	//		iProjects /= 10;
	//	}

	//	if(iProjects == 1)
	//	{
	//		iBlock = 28;
	//		memset(szBlockData, 0, 40);
	//		//iTimes1 = rf.Hex2Dec(szTimes1);
	//		iTimes1 = atoi((char*)szTimes1);
	//		strTemp.Format("%02X", m_cardData.iProjectTimes1 + iTimes1);
	//		memcpy(szBlockData, "01", 2);
	//		memcpy(szBlockData + 2, strTemp, 2);

	//		iDataLen = strlen((char*)m_cardData.szProject1);
	//		rf.SwitchDataMode(m_cardData.szProject1, &iDataLen, rf.ASC2HEX);
	//		memcpy(szBlockData + 4, m_cardData.szProject1, iDataLen);
	//		iDataLen = strlen((char*)szBlockData);
	//		while(iDataLen != 32)
	//		{
	//			memcpy(szBlockData + iDataLen, "20", 2);
	//			iDataLen += 2;
	//		}
	//		if(rf.Write(iBlock, szBlockData, rf.HEX))
	//		{
	//			AfxMessageBox("��ֵʧ��(3)!");
	//			m_StDisp.SetWindowTextA("��ֵʧ��(3)!");
	//			return;
	//		}
	//	}
	//	AfxMessageBox("��ֵ�ɹ���");
	//	m_StDisp.SetWindowTextA("��ֵ�ɹ���");
	//	this->OnBnClickedBtnCardinfo();

	//}
	//else
	//{
	//	AfxMessageBox("��Ƭ״̬δ֪�����鿨Ƭ��");
	//	m_StDisp.SetWindowTextA("��Ƭ״̬δ֪�����鿨Ƭ��");
	//	return;
	//}
		
		//��ʼд����
		for(int i = 1; i != 11 - m_iProjectLeft; ++i)
		{
			iRet = WriteProjectBlock(&project[i - 1]);
			if(iRet)
			{
				strTemp.Format("д%d��Ŀʧ�ܣ�", i);
				AfxMessageBox(strTemp);
				return;
			}
			//project[i-1].iTimes += project[i-1].iAddTimes;
			//project[i-1].iAddTimes = 0;
		}

		iBlock = 20;
		if(AuthSector(iBlock/4, rf.KEY_R))
			return;

		m_cardData.iAmount += lAmount;
		if(rf.Increment(iBlock, lAmount))
		{
	/*		rf.Dev_Beep(5);
			rf.Dev_Beep(5);*/
			strTemp.Format("���ӿ�Ƭ���ʧ�ܣ����ӽ�%d", lAmount);
			AfxMessageBox(strTemp);
			strTemp.Format("��һ��<%s>��ֵʧ�ܣ����ӿ�Ƭ���ʧ��", szTime);
			WriteLog(strTemp);
			return;
		}

		memset(szBlockData, 0x30, 32);
		iDataLen = strlen((char*)m_cardData.szCardNO);
		memcpy(szBlockData + 12 - iDataLen, m_cardData.szCardNO, iDataLen);
		iDataLen = strlen((char*)m_cardData.szVIPCardNO);
		memcpy(szBlockData + 24 - iDataLen, m_cardData.szVIPCardNO, iDataLen);
		strTemp.Format("%02X", 10 - m_iProjectLeft);
		memcpy(szBlockData + 24, strTemp, 2);
		memcpy(szBlockData + 30, "01", 2);

		iBlock = 22;
		if(AuthSector(iBlock/4, rf.KEY_R))
		{
			strTemp += "��֤������Կʧ��";
			WriteLog(strTemp);
			return;
		}

		if(rf.Write(iBlock, szBlockData, rf.HEX))
		{
			AfxMessageBox("д��Ŀ��Ϣʧ�ܣ�");
			strTemp += "д��Ŀ��Ϣʧ��";
			WriteLog(strTemp);
			return;
		}
		
		AfxMessageBox("��ֵ�ɹ���");
		m_StDisp.SetWindowTextA("��ֵ�ɹ���");
		//this->OnBnClickedBtnCardinfo();
	}

	m_StBalance.SetWindowTextA("��");
	m_EdtName.EnableWindow(FALSE);
	m_EdtPhoneNO.EnableWindow(FALSE);
	m_EdtVIPCardNO.EnableWindow(FALSE);
	m_EdtBalance.EnableWindow(FALSE);
	m_EdtVIPCardNO.SetWindowTextA("");
	m_EdtPhoneNO.SetWindowTextA("");
	m_EdtName.SetWindowTextA("");
	m_EdtBalance.SetWindowTextA("");
	m_EdtCardNO.SetWindowTextA("");
	m_EdtCardNO.EnableWindow(FALSE);
	m_EdtProject1.EnableWindow(FALSE);
	m_EdtProject2.EnableWindow(FALSE);
	m_EdtProject3.EnableWindow(FALSE);
	m_EdtProject4.EnableWindow(FALSE);
	m_EdtProject5.EnableWindow(FALSE);
	m_EdtProject6.EnableWindow(FALSE);
	m_EdtProject7.EnableWindow(FALSE);
	m_EdtProject8.EnableWindow(FALSE);
	m_EdtProject9.EnableWindow(FALSE);
	m_EdtProject10.EnableWindow(FALSE);
	m_EdtProject1.SetWindowTextA("");
	m_EdtProject2.SetWindowTextA("");
	m_EdtProject3.SetWindowTextA("");
	m_EdtProject4.SetWindowTextA("");
	m_EdtProject5.SetWindowTextA("");
	m_EdtProject6.SetWindowTextA("");
	m_EdtProject7.SetWindowTextA("");
	m_EdtProject8.SetWindowTextA("");
	m_EdtProject9.SetWindowTextA("");
	m_EdtProject10.SetWindowTextA("");
	m_EdtTimes1.EnableWindow(FALSE);
	m_EdtTimes2.EnableWindow(FALSE);
	m_EdtTimes3.EnableWindow(FALSE);
	m_EdtTimes4.EnableWindow(FALSE);
	m_EdtTimes5.EnableWindow(FALSE);
	m_EdtTimes6.EnableWindow(FALSE);
	m_EdtTimes7.EnableWindow(FALSE);
	m_EdtTimes8.EnableWindow(FALSE);
	m_EdtTimes9.EnableWindow(FALSE);
	m_EdtTimes10 .EnableWindow(FALSE);
	m_EdtTimes1.SetWindowTextA("");
	m_EdtTimes2.SetWindowTextA("");
	m_EdtTimes3.SetWindowTextA("");
	m_EdtTimes4.SetWindowTextA("");
	m_EdtTimes5.SetWindowTextA("");
	m_EdtTimes6.SetWindowTextA("");
	m_EdtTimes7.SetWindowTextA("");
	m_EdtTimes8.SetWindowTextA("");
	m_EdtTimes9.SetWindowTextA("");
	m_EdtTimes10.SetWindowTextA("");

	m_BtnLoad.EnableWindow(FALSE);
	m_BtnBegin.EnableWindow(TRUE);
	rf.Dev_Beep(10);
}

int CMifareDebitDlg::GetKeyData(unsigned char* pszKeyData)
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

void CMifareDebitDlg::OnBnClickedBtnCardinfo()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	int iRet = 0, iBlock, iDataLen = 0;
	CString strTemp;
	unsigned char /*szSectorKey[40], */szBlockData[40];
	//unsigned char szProjectTimes[5], szProjectName[40];
	unsigned long lMoney = 0;
	unsigned char szCardStatus[5];
	//int iProjectTimes;

	memset(m_cardData.szBalance, 0, sizeof(m_cardData.szBalance));
	memset(m_cardData.szCardNO, 0, sizeof(m_cardData.szCardNO));
	memset(m_cardData.szName, 0, sizeof(m_cardData.szCardNO));
	memset(m_cardData.szPhoneNO,  0, sizeof(m_cardData.szPhoneNO));
	memset(m_cardData.szProject1, 0, sizeof(m_cardData.szProject1));
	memset(m_cardData.szProject2, 0, sizeof(m_cardData.szProject2));
	memset(m_cardData.szProject3, 0, sizeof(m_cardData.szProject3));
	memset(m_cardData.szVIPCardNO, 0, sizeof(m_cardData.szVIPCardNO));
	m_cardData.iAmount = 0;
	m_cardData.bIsUsed = FALSE;

	
	memset(&project[0], 0, sizeof(project[0]));
	memset(&project[1], 0, sizeof(project[1]));
	memset(&project[2], 0, sizeof(project[2]));
	memset(&project[3], 0, sizeof(project[3]));
	memset(&project[4], 0, sizeof(project[4]));
	memset(&project[5], 0, sizeof(project[5]));
	memset(&project[6], 0, sizeof(project[6]));
	memset(&project[7], 0, sizeof(project[7]));
	memset(&project[8], 0, sizeof(project[8]));
	memset(&project[9], 0, sizeof(project[9]));

	m_EdtBalance.SetWindowTextA("");
	m_EdtProject1.SetWindowTextA("");
	m_EdtProject2.SetWindowTextA("");
	m_EdtProject3.SetWindowTextA("");
	m_EdtProject4.SetWindowTextA("");
	m_EdtProject5.SetWindowTextA("");
	m_EdtProject6.SetWindowTextA("");
	m_EdtProject7.SetWindowTextA("");
	m_EdtProject8.SetWindowTextA("");
	m_EdtProject9.SetWindowTextA("");
	m_EdtProject10.SetWindowTextA("");

	m_EdtVIPCardNO.SetWindowTextA("");
	m_EdtPhoneNO.SetWindowTextA("");
	m_EdtName.SetWindowTextA("");
	m_EdtBalance.SetWindowTextA("");
	m_EdtCardNO.SetWindowTextA("");

	m_EdtTimes1.SetWindowTextA("");
	m_EdtTimes2.SetWindowTextA("");
	m_EdtTimes3.SetWindowTextA("");
	m_EdtTimes4.SetWindowTextA("");
	m_EdtTimes5.SetWindowTextA("");
	m_EdtTimes6.SetWindowTextA("");
	m_EdtTimes7.SetWindowTextA("");
	m_EdtTimes8.SetWindowTextA("");
	m_EdtTimes9.SetWindowTextA("");
	m_EdtTimes10.SetWindowTextA("");



	//�豸����������Կ��֤
	//���Ӷ�д��
	if(!rf.m_isConnected)
	{
		if(rf.Dev_Connect(100, 115200))
		{
			AfxMessageBox("���Ӷ�д��ʧ�ܣ������豸��");
			m_StDisp.SetWindowTextA("���Ӷ�д��ʧ�ܣ������豸��");
			return;
		}
	}

	iBlock = 22;
	iRet = AuthSector(iBlock/4, rf.KEY_L);
	if(iRet)
		return;
	memset(szBlockData, 0, 40);
	if(rf.ReadMifareBlock(iBlock, szBlockData, rf.HEX))
	{
		AfxMessageBox("������ʧ�ܣ����鿨Ƭ��");
		return;
	}

	//��״̬77 δ���˻��� 01 �Ѹ��˻�������ֵ�� 03 �������� 
	memset(szCardStatus, 0, 5);
	memcpy(szCardStatus, szBlockData + 30, 2);
	if(!memcmp(szCardStatus, "03", 2))
	{
		AfxMessageBox("��Ƭ����������ȷ�ϵ�ǰ��Ƭ��");
		m_StDisp.SetWindowTextA("��Ƭ����������ȷ�ϵ�ǰ��Ƭ��");
		m_iCardStatus = 3;
		return;
	}
	else if(!memcmp(szCardStatus, "01", 2))
	{//////���˻�����ֵ
		m_StDisp.SetWindowTextA("��Ƭ�Ѹ��˻������ֵ��");

		//����
		memcpy(m_cardData.szCardNO, szBlockData, 12);
		m_EdtCardNO.SetWindowTextA((LPCTSTR)m_cardData.szCardNO);

		//vip����
		memcpy(m_cardData.szVIPCardNO, szBlockData + 12, 12);
		m_EdtVIPCardNO.SetWindowTextA((LPCTSTR)m_cardData.szVIPCardNO);

		unsigned char szProjectCount[5];
		memset(szProjectCount, 0, 5);
		memcpy(szProjectCount, szBlockData + 24, 2);
		m_iProjectsCount = rf.Hex2Dec(szProjectCount);///////��Ŀ����
		m_iProjectLeft = 10 - m_iProjectsCount;
		if(m_iProjectsCount > 10 || m_iProjectsCount < 0)
		{
			AfxMessageBox("��Ŀ�����������鿨Ƭ��");
			return;
		}

		//������
		iBlock = 21;
		memset(szBlockData, 0, 40);
		if(rf.ReadMifareBlock(iBlock, szBlockData, rf.HEX))
		{
			AfxMessageBox("��Ƭ״̬ʧ�ܣ����鿨Ƭ��");
			return;
		}

		iDataLen = 20;
		memset(m_cardData.szName, 0, sizeof(m_cardData.szName));
		memcpy(m_cardData.szName, szBlockData, iDataLen);
		rf.SwitchDataMode(m_cardData.szName, &iDataLen, rf.HEX2ASC);
		m_cardData.szName[10] = 0;
		strTemp.Format("%s", m_cardData.szName);
		strTemp.Remove(0x20);
		m_EdtName.SetWindowTextA((LPCTSTR)strTemp);
		//memcpy(m_cardData.szName, strTemp, strTemp.GetLength());
		m_cardData.szName[strTemp.GetLength()] = 0;

		//��ϵ�绰
		memcpy(m_cardData.szPhoneNO, szBlockData + 20, 12);
		m_EdtPhoneNO.SetWindowTextA((LPCTSTR)m_cardData.szPhoneNO);

		iBlock = 20;
		if(rf.ReadValue(iBlock, &lMoney))
		{
			AfxMessageBox("ȡ���ʧ�ܣ�");
			m_StDisp.SetWindowTextA("ȡ���ʧ�ܣ�");
			return;
		}

		strTemp.Format("%d", lMoney);
		m_cardData.iAmount = lMoney;
		m_EdtBalance.SetWindowTextA((LPCTSTR)strTemp);//��ʾ���

		//����Ŀ��Ϣ
		memset(project, 0, sizeof(project));
		for(int i = 1; i != m_iProjectsCount + 1; ++i)
		{
			if(ReadProjectBlock(&project[i - 1], i))
				return;
		}
		//��ʾ��Ŀ����
		if(m_iProjectsCount >= 1)
		{
			strTemp.Format("%d", project[0].iTimes);
			m_EdtProject1.SetWindowTextA((LPCTSTR)project[0].szProjectName);
			m_EdtTimes1.SetWindowTextA((LPCTSTR)strTemp);
		}
		if(m_iProjectsCount >= 2)
		{
			strTemp.Format("%d", project[1].iTimes);
			m_EdtProject2.SetWindowTextA((LPCTSTR)project[1].szProjectName);
			m_EdtTimes2.SetWindowTextA((LPCTSTR)strTemp);
		}
		if(m_iProjectsCount >= 3)
		{
			strTemp.Format("%d", project[2].iTimes);
			m_EdtProject3.SetWindowTextA((LPCTSTR)project[2].szProjectName);
			m_EdtTimes3.SetWindowTextA((LPCTSTR)strTemp);
		}
		if(m_iProjectsCount >= 4)
		{
			strTemp.Format("%d", project[3].iTimes);
			m_EdtProject4.SetWindowTextA((LPCTSTR)project[3].szProjectName);
			m_EdtTimes4.SetWindowTextA((LPCTSTR)strTemp);
		}
		if(m_iProjectsCount >= 5)
		{
			strTemp.Format("%d", project[4].iTimes);
			m_EdtProject5.SetWindowTextA((LPCTSTR)project[4].szProjectName);
			m_EdtTimes5.SetWindowTextA((LPCTSTR)strTemp);
		}
		if(m_iProjectsCount >= 6)
		{
			strTemp.Format("%d", project[5].iTimes);
			m_EdtProject6.SetWindowTextA((LPCTSTR)project[5].szProjectName);
			m_EdtTimes6.SetWindowTextA((LPCTSTR)strTemp);
		}
		if(m_iProjectsCount >= 7)
		{
			strTemp.Format("%d", project[6].iTimes);
			m_EdtProject7.SetWindowTextA((LPCTSTR)project[6].szProjectName);
			m_EdtTimes7.SetWindowTextA((LPCTSTR)strTemp);
		}
		if(m_iProjectsCount >= 8)
		{
			strTemp.Format("%d", project[7].iTimes);
			m_EdtProject8.SetWindowTextA((LPCTSTR)project[7].szProjectName);
			m_EdtTimes8.SetWindowTextA((LPCTSTR)strTemp);
		}
		if(m_iProjectsCount >= 9)
		{
			strTemp.Format("%d", project[8].iTimes);
			m_EdtProject9.SetWindowTextA((LPCTSTR)project[8].szProjectName);
			m_EdtTimes9.SetWindowTextA((LPCTSTR)strTemp);
		}
		if(m_iProjectsCount == 10)
		{
			strTemp.Format("%d", project[9].iTimes);
			m_EdtProject10.SetWindowTextA((LPCTSTR)project[9].szProjectName);
			m_EdtTimes10.SetWindowTextA((LPCTSTR)strTemp);
		}
		
		for(int i = 0; i != m_iProjectsCount; ++i)
		{
			if(project[i].iTimes == 0)
			{
				m_BtnRmvEmpty.EnableWindow(TRUE);
				break;
			}
		}

		////�õ�����Կ
		////��Ϣ����Ϊ28 29 30
		//iBlock = 28;
		//memset(szSectorKey, 0, 40);

		//memcpy(szSectorKey, m_KeyData + (iBlock/4)*24, 12);
		//if(rf.LoadKey(rf.KEY_L, iBlock/4, szSectorKey, rf.HEX))
		//{
		//	AfxMessageBox("��֤��Կʧ�ܣ�");
		//	m_StDisp.SetWindowTextA("��֤��Կʧ�ܣ�");
		//	return;
		//}

		//if(rf.Anthentication(rf.KEY_L, iBlock/4))
		//{
		//	AfxMessageBox("��֤��Կʧ��2��");
		//	m_StDisp.SetWindowTextA("��֤��Կʧ��2��");
		//	return;
		//}
		////��֤��Կ���

		////����Ŀ1
		//int iDataLen = 0;

		//memset(szBlockData, 0, 40);
		//if(rf.ReadMifareBlock(iBlock, szBlockData, rf.HEX))
		//{
		//	AfxMessageBox("��������Ϣʧ��(1)!");
		//	m_StDisp.SetWindowTextA("��������Ϣʧ��(1)!");
		//	return;
		//}

		//if(!memcmp(szBlockData, "01", 2))
		//{
		//	strTemp.Format("%.2s", szBlockData + 2);
		//	m_cardData.iProjectTimes1 = rf.Hex2Dec((unsigned char*)strTemp.GetBuffer());
		//	strTemp.Format("%d", m_cardData.iProjectTimes1);
		//	m_EdtTimes1.SetWindowTextA((LPCTSTR)strTemp);

		//	//strTemp.Format("%.28s", szBlockData + 4);
		//	iDataLen = 28;
		//	memcpy(m_cardData.szProject1, szBlockData + 4, 28);
		//	rf.SwitchDataMode(m_cardData.szProject1, &iDataLen, rf.HEX2ASC);
		//	m_EdtProject1.SetWindowTextA((LPCTSTR)m_cardData.szProject1);

		//	m_iProjects = 1;
		//}

		////��Ŀ2
		//iBlock = 29;
		//memset(szBlockData, 0, 40);
		//if(rf.ReadMifareBlock(iBlock, szBlockData, rf.HEX))
		//{
		//	AfxMessageBox("��������Ϣʧ��(1)!");
		//	m_StDisp.SetWindowTextA("��������Ϣʧ��(1)!");
		//	return;
		//}

		//if(!memcmp(szBlockData, "02", 2))
		//{
		//	strTemp.Format("%.2s", szBlockData + 2);
		//	m_cardData.iProjectTimes2 = rf.Hex2Dec((unsigned char*)strTemp.GetBuffer());
		//	strTemp.Format("%d", m_cardData.iProjectTimes2);
		//	m_EdtTimes2.SetWindowTextA((LPCTSTR)strTemp);

		//	//strTemp.Format("%.28s", szBlockData + 4);
		//	iDataLen = 28;
		//	memcpy(m_cardData.szProject2, szBlockData + 4, 28);
		//	rf.SwitchDataMode(m_cardData.szProject2, &iDataLen, rf.HEX2ASC);
		//	m_EdtProject2.SetWindowTextA((LPCTSTR)m_cardData.szProject2);

		//	m_iProjects = m_iProjects* 10 + 2;
		//}
		//
		////��Ŀ3
		//iBlock = 30;
		//memset(szBlockData, 0, 40);
		//if(rf.ReadMifareBlock(iBlock, szBlockData, rf.HEX))
		//{
		//	AfxMessageBox("��������Ϣʧ��(1)!");
		//	m_StDisp.SetWindowTextA("��������Ϣʧ��(1)!");
		//	return;
		//}

		//if(!memcmp(szBlockData, "03", 2))
		//{
		//	strTemp.Format("%.2s", szBlockData + 2);
		//	m_cardData.iProjectTimes3 = rf.Hex2Dec((unsigned char*)strTemp.GetBuffer());
		//	strTemp.Format("%d", m_cardData.iProjectTimes3);
		//	m_EdtTimes3.SetWindowTextA((LPCTSTR)strTemp);

		//	iDataLen = 28;
		//	memcpy(m_cardData.szProject3, szBlockData + 4, 28);
		//	rf.SwitchDataMode(m_cardData.szProject3, &iDataLen, rf.HEX2ASC);
		//	m_EdtProject3.SetWindowTextA((LPCTSTR)m_cardData.szProject3);

		//	m_iProjects = m_iProjects* 10 + 3;
		//}
		
		m_BtnBegin.SetWindowTextA("��ʼ��ֵ");
		m_BtnLoad.SetWindowTextA("��ֵ");
		m_iCardStatus = 1;
	}
	else if(!memcmp(szCardStatus, "77", 2))
	{//δ���˻�
		m_StDisp.SetWindowTextA("�ÿ�Ϊ����״̬���뿪����������Ӧ������Ϣ��");

		//����ϵ��ʽ\������Ϣ
		iBlock = 22;
		memset(szBlockData, 0, 40);
		if(rf.ReadMifareBlock(iBlock, szBlockData, rf.HEX))
		{
			AfxMessageBox("������ʧ�ܣ����鿨Ƭ��");
			return;
		}

		//���� ��д��
		//memcpy(m_cardData.szCardNO, szBlockData + 12, 6);
		//m_EdtCardNO.SetWindowTextA((LPCTSTR)m_cardData.szCardNO);		

		m_iProjects = 0;
		m_iProjectsCount = 0;
		m_iProjectLeft = 10;
		m_iCardStatus = 0;
		
		m_BtnBegin.SetWindowTextA("��ʼ����");
		m_BtnLoad.SetWindowTextA("����");
	}
	else
	{
		m_StDisp.SetWindowTextA("δ֪��Ƭ!");
	}
	//���ÿؼ�
	m_StBalance.SetWindowTextA("��");
	m_EdtName.EnableWindow(FALSE);
	m_EdtPhoneNO.EnableWindow(FALSE);
	m_EdtCardNO.EnableWindow(FALSE);
	m_EdtVIPCardNO.EnableWindow(FALSE);
	m_EdtBalance.EnableWindow(FALSE);
	m_EdtProject1.EnableWindow(FALSE);
	m_EdtProject2.EnableWindow(FALSE);
	m_EdtProject3.EnableWindow(FALSE);
	m_EdtProject4.EnableWindow(FALSE);
	m_EdtProject5.EnableWindow(FALSE);
	m_EdtProject6.EnableWindow(FALSE);
	m_EdtProject7.EnableWindow(FALSE);
	m_EdtProject8.EnableWindow(FALSE);
	m_EdtProject9.EnableWindow(FALSE);
	m_EdtProject10.EnableWindow(FALSE);
	m_EdtTimes1.EnableWindow(FALSE);
	m_EdtTimes2.EnableWindow(FALSE);
	m_EdtTimes3.EnableWindow(FALSE);
	m_EdtTimes4.EnableWindow(FALSE);
	m_EdtTimes5.EnableWindow(FALSE);
	m_EdtTimes6.EnableWindow(FALSE);
	m_EdtTimes7.EnableWindow(FALSE);
	m_EdtTimes8.EnableWindow(FALSE);
	m_EdtTimes9.EnableWindow(FALSE);
	m_EdtTimes10.EnableWindow(FALSE);
	m_BtnAddProject.EnableWindow(FALSE);
	m_BtnRemoveProject.EnableWindow(FALSE);

	m_BtnLoad.EnableWindow(FALSE);
	m_BtnBegin.EnableWindow(TRUE);

	m_isInitialized = TRUE;
}

int CMifareDebitDlg::CheckNumberData(unsigned char* pszData)
{//1 ��ʾ�з������ַ�
	int iDataLen = 0;

	if(!pszData)
		return 1;
	iDataLen = strlen((char*)pszData);
	for(int i = 0; i != iDataLen; ++i)
	{
		if(pszData[i] > 0x39 || pszData[i] < 0x30)
		{
			return 1;
		}
	}

	return 0;
}

void CMifareDebitDlg::OnBnClickedBtnClose()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	CDialog::OnCancel();
}

int CMifareDebitDlg::AuthSector(int iSector, int iKeyModal)
{
	//�õ�����Կ
	//��Ϣ����Ϊ20 21 22
	unsigned char szSectorKey[40];
	memset(szSectorKey, 0, 40);
	if(rf.Dev_Reset(1))
	{
		AfxMessageBox("��Ƭ��λʧ�ܣ�");
		m_StDisp.SetWindowTextA("��Ƭ��λʧ�ܣ�");
		return 1;
	}
	
	if(iKeyModal == rf.KEY_L)
		memcpy(szSectorKey, m_KeyData + iSector*24, 12);
	else
		memcpy(szSectorKey, m_KeyData + iSector*24 + 12, 12);

	if(rf.LoadKey(iKeyModal, iSector, szSectorKey, rf.HEX))
	{
		AfxMessageBox("��֤��Կʧ�ܣ�");
		m_StDisp.SetWindowTextA("��֤��Կʧ�ܣ�");
		return 1;	
	}

	if(rf.Anthentication(iKeyModal, iSector))
	{
		AfxMessageBox("��֤��Կʧ��2��");
		m_StDisp.SetWindowTextA("��֤��Կʧ��2��");
		return 1;
	}
	//��֤��Կ���

	return 0;
}

int CMifareDebitDlg::SetProjectList(void)
{///////////////////////��ʼ��ʱ����COMBO�б�
	char szFileName[MAX_PATH];
	char szFilePath[MAX_PATH], szKeyName[10];
	char szProjectName[MAX_PATH];
	int iFlag = 1, iCount = 1, iRet;
	CString strTemp, strIndex;

	memset(szFileName, 0, MAX_PATH);
	memset(szFilePath, 0, MAX_PATH);
	memset(szKeyName, 0, 10);
	
	memcpy(szFileName, "set_project.ini", 15);
	GetFileAbsluteAddress(szFileName, szFilePath);

	memset(szProjectName, 0, MAX_PATH);
	memcpy(szKeyName, "P", 2);
	while(1)
	{
		strIndex.Format("%s%d", szKeyName, iCount);
		if((iRet = ::GetPrivateProfileStringA("PROJECT", (LPCSTR)strIndex, "", (LPSTR)szProjectName, 20, szFilePath)) <= 0)
		{
			break;
		}
		strTemp.Format("%s", szProjectName);
		strTemp.Remove(0x20);
		m_CmbProject.AddString((LPCTSTR)strTemp);
		iCount ++;
	}
	m_iListCount = iCount;
	//m_CmbProject.SetCurSel(0);
	return 0;
}

int CMifareDebitDlg::WriteProjectBlock(ProjectData* pData)
{
	int iRet = 0, iBlock = 37, iDataLen = 0;
	CString strTemp;
	unsigned char szBlockData[BLOCK_LEN];

	if(pData == NULL)
		return 2;

	switch(pData->iProjectIndex)
	{
	case 1: iBlock = 24; break;
	case 2: iBlock = 25; break;
	case 3: iBlock = 26; break;
	case 4: iBlock = 28; break;
	case 5: iBlock = 29; break;
	case 6: iBlock = 30; break;
	case 7: iBlock = 32; break;
	case 8: iBlock = 33; break;
	case 9: iBlock = 34; break;
	case 10: iBlock = 36; break;
	default: iBlock = 37; break;
	}

	if(iBlock == 37)
	{
		AfxMessageBox("δ�������Ŀ�����֤���ݣ�");
		return 2;
	}

	iRet = AuthSector(iBlock/4, rf.KEY_R);
	if(iRet)
	{
		AfxMessageBox("��֤��Ƭ��Կʧ�ܣ����鿨Ƭ��");
		return 2;
	}

	//memset(szBlockData, 0, BLOCK_LEN);
	//strTemp.Format("%02X", pData->iProjectIndex);
	memcpy(szBlockData, pData->szProjectIndex, 2);
	strTemp.Format("%04X", pData->iTimes + pData->iAddTimes);
	memcpy(szBlockData + 2, strTemp, 4);

	strTemp.Format("%s", pData->szProjectName);
	rf.Asc2Hex((unsigned char*)strTemp.GetBuffer(), szBlockData + 6, (iDataLen = strTemp.GetLength()));
	iDataLen = iDataLen * 2 + 6;

	while(iDataLen < 32)
	{
		memcpy(szBlockData + iDataLen, "20", 2);
		iDataLen += 2;
	}
	szBlockData[32] = 0;

	if((iRet = rf.Write(iBlock, szBlockData, rf.HEX)) != 0)
	{
		AfxMessageBox("д��Ŀ����ʧ�ܣ����鿨Ƭ��");
		return 2;
	}

	return 0;
}

int CMifareDebitDlg::ReadProjectBlock(ProjectData* pData, int iProject)
{
	int iRet = 0, iBlock = 0;
	unsigned char szBlockData[BLOCK_LEN];
	unsigned char szTempData[30];
	CString strTemp;

	switch(iProject)
	{
	case 1: iBlock = 24; break;
	case 2: iBlock = 25; break;
	case 3: iBlock = 26; break;
	case 4: iBlock = 28; break;
	case 5: iBlock = 29; break;
	case 6: iBlock = 30; break;
	case 7: iBlock = 32; break;
	case 8: iBlock = 33; break;
	case 9: iBlock = 34; break;
	case 10: iBlock = 36; break;
	default: iBlock = 37; break;
	}

	if(iBlock == 37)
	{
		AfxMessageBox("δ�������Ŀ���������ݣ�");
		return 3;
	}

	iRet = AuthSector(iBlock/4, rf.KEY_L);
	if(iRet)
	{
		AfxMessageBox("��֤��Կʧ�ܣ����鿨Ƭ��");
		return 3;
	}
	
	memset(szBlockData, 0, BLOCK_LEN);
	if(rf.ReadMifareBlock(iBlock, szBlockData, rf.HEX))
	{
		AfxMessageBox("����Ŀ����ʧ�ܣ����鿨Ƭ��");
		return 3;
	}

	memset(szTempData, 0, 30);
	memcpy(pData->szProjectIndex, szBlockData, 2);
	pData->iProjectIndex = rf.Hex2Dec(pData->szProjectIndex);
	memcpy(szTempData, szBlockData + 2, 4);
	pData->iTimes = rf.Hex2Dec(szTempData);

	rf.Hex2Asc(szBlockData + 6, szTempData, 26);
	strTemp.Format("%.26s", szTempData);
	strTemp.Remove(0x20);

	memcpy(pData->szProjectName, strTemp, strTemp.GetLength());

	return 0;
}

void CMifareDebitDlg::OnBnClickedBtnAddp()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	if(m_iProjectLeft > 0)
	{
		CString strTemp;
		unsigned char szProjectName[30];
		int index = 0;
		memset(szProjectName, 0, 30);

		m_CmbProject.GetWindowTextA((LPTSTR)szProjectName, 14);
		index = m_CmbProject.GetCurSel();
		m_CmbProject.DeleteString(index);
		strTemp.Format("%s", szProjectName);
		if(m_iProjectLeft == 1)
		{
			m_EdtProject10.SetWindowTextA(strTemp);
			m_EdtTimes10.EnableWindow(TRUE);
			m_CmbProject.SetCurSel(1);
		}
		if(m_iProjectLeft == 2)
		{
			m_EdtProject9.SetWindowTextA(strTemp);
			m_EdtTimes9.EnableWindow(TRUE);
			m_CmbProject.SetCurSel(1);
		}
		if(m_iProjectLeft == 3)
		{
			m_EdtProject8.SetWindowTextA(strTemp);
			m_EdtTimes8.EnableWindow(TRUE);
			m_CmbProject.SetCurSel(1);
		}
		if(m_iProjectLeft == 4)
		{
			m_EdtProject7.SetWindowTextA(strTemp);
			m_EdtTimes7.EnableWindow(TRUE);
			m_CmbProject.SetCurSel(1);
		}
		if(m_iProjectLeft == 5)
		{
			m_EdtProject6.SetWindowTextA(strTemp);
			m_EdtTimes6.EnableWindow(TRUE);
			m_CmbProject.SetCurSel(1);
		}
		if(m_iProjectLeft == 6)
		{
			m_EdtProject5.SetWindowTextA(strTemp);
			m_EdtTimes5.EnableWindow(TRUE);
			m_CmbProject.SetCurSel(1);
		}
		if(m_iProjectLeft == 7)
		{
			m_EdtProject4.SetWindowTextA(strTemp);
			m_EdtTimes4.EnableWindow(TRUE);
			m_CmbProject.SetCurSel(1);
		}
		if(m_iProjectLeft == 8)
		{
			m_EdtProject3.SetWindowTextA(strTemp);
			m_EdtTimes3.EnableWindow(TRUE);
			m_CmbProject.SetCurSel(1);
		}
		if(m_iProjectLeft == 9)
		{
			m_EdtProject2.SetWindowTextA(strTemp);
			m_EdtTimes2.EnableWindow(TRUE);
			m_CmbProject.SetCurSel(1);
		}
		if(m_iProjectLeft == 10)
		{
			m_EdtProject1.SetWindowTextA(strTemp);
			m_EdtTimes1.EnableWindow(TRUE);
			m_CmbProject.SetCurSel(1);
		}
		m_iProjectLeft -= 1;
	}

	m_BtnRemoveProject.EnableWindow(TRUE);
	if(m_iProjectLeft == 0)
		m_BtnAddProject.EnableWindow(FALSE);
}

void CMifareDebitDlg::OnBnClickedBtnRemovep()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString strTemp;

	if(m_iProjectLeft <= (11 - m_iProjectsCount))
	{
		if(m_iProjectLeft == 0)
		{
			m_EdtProject10.GetWindowTextA(strTemp);
			m_CmbProject.AddString(strTemp);
			m_EdtProject10.SetWindowTextA("");
			m_EdtTimes10.SetWindowTextA("");

			if(m_iCardStatus)
				m_EdtTimes10.EnableWindow(FALSE);
		}
		if(m_iProjectLeft == 1)
		{
			m_EdtProject9.GetWindowTextA(strTemp);
			m_CmbProject.AddString(strTemp);
			m_EdtProject9.SetWindowTextA("");
			m_EdtTimes9.SetWindowTextA("");
			
			if(m_iCardStatus)
				m_EdtTimes9.EnableWindow(FALSE);
		}
		if(m_iProjectLeft == 2)
		{
			m_EdtProject8.GetWindowTextA(strTemp);
			m_CmbProject.AddString(strTemp);
			m_EdtProject8.SetWindowTextA("");
			m_EdtTimes8.SetWindowTextA("");
			
			if(m_iCardStatus)
				m_EdtTimes8.EnableWindow(FALSE);
		}
		if(m_iProjectLeft == 3)
		{
			m_EdtProject7.GetWindowTextA(strTemp);
			m_CmbProject.AddString(strTemp);
			m_EdtProject7.SetWindowTextA("");
			m_EdtTimes7.SetWindowTextA("");
			if(m_iCardStatus)
				m_EdtTimes7.EnableWindow(FALSE);
		}
		if(m_iProjectLeft == 4)
		{
			m_EdtProject6.GetWindowTextA(strTemp);
			m_CmbProject.AddString(strTemp);
			m_EdtProject6.SetWindowTextA("");
			m_EdtTimes6.SetWindowTextA("");
			if(m_iCardStatus)
				m_EdtTimes6.EnableWindow(FALSE);
		}
		if(m_iProjectLeft == 5)
		{
			m_EdtProject5.GetWindowTextA(strTemp);
			m_CmbProject.AddString(strTemp);
			m_EdtProject5.SetWindowTextA("");
			m_EdtTimes5.SetWindowTextA("");
			if(m_iCardStatus)
				m_EdtTimes5.EnableWindow(FALSE);
		}
		if(m_iProjectLeft == 6)
		{
			m_EdtProject4.GetWindowTextA(strTemp);
			m_CmbProject.AddString(strTemp);
			m_EdtProject4.SetWindowTextA("");
			m_EdtTimes4.SetWindowTextA("");
			if(m_iCardStatus)
				m_EdtTimes4.EnableWindow(FALSE);
		}
		if(m_iProjectLeft == 7)
		{
			m_EdtProject3.GetWindowTextA(strTemp);
			m_CmbProject.AddString(strTemp);
			m_EdtProject3.SetWindowTextA("");
			m_EdtTimes3.SetWindowTextA("");
			if(m_iCardStatus)
				m_EdtTimes3.EnableWindow(FALSE);
		}
		if(m_iProjectLeft == 8)
		{
			m_EdtProject2.GetWindowTextA(strTemp);
			m_CmbProject.AddString(strTemp);
			m_EdtProject2.SetWindowTextA("");
			m_EdtTimes2.SetWindowTextA("");
			if(m_iCardStatus)
				m_EdtTimes2.EnableWindow(FALSE);
		}
		if(m_iProjectLeft == 9)
		{
			m_EdtProject1.GetWindowTextA(strTemp);
			m_CmbProject.AddString(strTemp);
			m_EdtProject1.SetWindowTextA("");
			m_EdtTimes1.SetWindowTextA("");
			if(m_iCardStatus)
				m_EdtTimes1.EnableWindow(FALSE);
		}
		m_iProjectLeft += 1;
	}
	m_BtnAddProject.EnableWindow(TRUE);
	if(m_iProjectLeft == 10 || m_iProjectLeft == 10 - m_iProjectsCount)
		m_BtnRemoveProject.EnableWindow(FALSE);
}

int CMifareDebitDlg::GetProjectData(ProjectData* pData, int iProject)
{
	unsigned char szTimes[5];
	memset(szTimes, 0, 5);
	if(iProject == 1)
	{

		memset(pData->szProjectName, 0, sizeof(pData->szProjectName));
		m_EdtProject1.GetWindowTextA((LPTSTR)pData->szProjectName, 27);

		m_EdtTimes1.GetWindowTextA((LPTSTR)szTimes, 4);

		pData->iProjectIndex = 1;
		pData->iAddTimes = atoi((char*)szTimes);
		memcpy(pData->szProjectIndex, "01", 2);
		pData->szProjectIndex[3] = 0;

		if(pData->iAddTimes + pData->iTimes > 999)
		{
			AfxMessageBox("��ֵ��Ĵ�����������ֵ(999)�Σ����飡");
			pData->iAddTimes = 0;
			return 3;
		}
	}
	else if(iProject == 2)
	{

		memset(pData->szProjectName, 0, sizeof(pData->szProjectName));
		m_EdtProject2.GetWindowTextA((LPTSTR)pData->szProjectName, 27);
	
		m_EdtTimes2.GetWindowTextA((LPTSTR)szTimes, 4);
		
		pData->iProjectIndex = 2;
		pData->iAddTimes = atoi((char*)szTimes);
		memcpy(pData->szProjectIndex, "02", 2);
		pData->szProjectIndex[3] = 0;
		if(pData->iAddTimes + pData->iTimes > 999)
		{
			AfxMessageBox("��ֵ��Ĵ�����������ֵ(999)�Σ����飡");
			pData->iAddTimes = 0;
			return 3;
		}
	}
	else if(iProject == 3)
	{

		memset(pData->szProjectName, 0, sizeof(pData->szProjectName));
		m_EdtProject3.GetWindowTextA((LPTSTR)pData->szProjectName, 27);

		m_EdtTimes3.GetWindowTextA((LPTSTR)szTimes, 4);
		
		pData->iProjectIndex = 3;
		pData->iAddTimes = atoi((char*)szTimes);
		memcpy(pData->szProjectIndex, "03", 2);
		pData->szProjectIndex[3] = 0;
		if(pData->iAddTimes + pData->iTimes > 999)
		{
			AfxMessageBox("��ֵ��Ĵ�����������ֵ(999)�Σ����飡");
			pData->iAddTimes = 0;
			return 3;
		}
	}
	else if(iProject == 4)
	{

		memset(pData->szProjectName, 0, sizeof(pData->szProjectName));
		m_EdtProject4.GetWindowTextA((LPTSTR)pData->szProjectName, 27);
	
		m_EdtTimes4.GetWindowTextA((LPTSTR)szTimes, 4);
		
		pData->iProjectIndex = 4;
		pData->iAddTimes = atoi((char*)szTimes);
		memcpy(pData->szProjectIndex, "04", 2);
		pData->szProjectIndex[3] = 0;
		if(pData->iAddTimes + pData->iTimes > 999)
		{
			AfxMessageBox("��ֵ��Ĵ�����������ֵ(999)�Σ����飡");
			pData->iAddTimes = 0;
			return 3;
		}
	}
	else if(iProject == 5)
	{

		memset(pData->szProjectName, 0, sizeof(pData->szProjectName));
		m_EdtProject5.GetWindowTextA((LPTSTR)pData->szProjectName, 27);
	
		m_EdtTimes5.GetWindowTextA((LPTSTR)szTimes, 4);
		
		pData->iProjectIndex = 5;
		pData->iAddTimes = atoi((char*)szTimes);
		memcpy(pData->szProjectIndex, "05", 2);
		pData->szProjectIndex[3] = 0;
		if(pData->iAddTimes + pData->iTimes > 999)
		{
			AfxMessageBox("��ֵ��Ĵ�����������ֵ(999)�Σ����飡");
			pData->iAddTimes = 0;
			return 3;
		}
	}
	else if(iProject == 6)
	{

		memset(pData->szProjectName, 0, sizeof(pData->szProjectName));
		m_EdtProject6.GetWindowTextA((LPTSTR)pData->szProjectName, 27);
	
		m_EdtTimes6.GetWindowTextA((LPTSTR)szTimes, 4);
		
		pData->iProjectIndex = 6;
		pData->iAddTimes = atoi((char*)szTimes);
		memcpy(pData->szProjectIndex, "06", 2);
		pData->szProjectIndex[3] = 0;
		if(pData->iAddTimes + pData->iTimes > 999)
		{
			AfxMessageBox("��ֵ��Ĵ�����������ֵ(999)�Σ����飡");
			pData->iAddTimes = 0;
			return 3;
		}
	}
	else if(iProject == 7)
	{

		memset(pData->szProjectName, 0, sizeof(pData->szProjectName));
		m_EdtProject7.GetWindowTextA((LPTSTR)pData->szProjectName, 27);
	
		m_EdtTimes7.GetWindowTextA((LPTSTR)szTimes, 4);
		
		pData->iProjectIndex = 7;
		pData->iAddTimes = atoi((char*)szTimes);
		memcpy(pData->szProjectIndex, "07", 2);
		pData->szProjectIndex[3] = 0;
		if(pData->iAddTimes + pData->iTimes > 999)
		{
			AfxMessageBox("��ֵ��Ĵ�����������ֵ(999)�Σ����飡");
			pData->iAddTimes = 0;
			return 3;
		}
	}
	else if(iProject == 8)
	{

		memset(pData->szProjectName, 0, sizeof(pData->szProjectName));
		m_EdtProject8.GetWindowTextA((LPTSTR)pData->szProjectName, 27);

		m_EdtTimes8.GetWindowTextA((LPTSTR)szTimes, 4);
		
		pData->iProjectIndex = 8;
		pData->iAddTimes = atoi((char*)szTimes);
		memcpy(pData->szProjectIndex, "08", 2);
		pData->szProjectIndex[3] = 0;
		if(pData->iAddTimes + pData->iTimes > 999)
		{
			AfxMessageBox("��ֵ��Ĵ�����������ֵ(999)�Σ����飡");
			pData->iAddTimes = 0;
			return 3;
		}
	}
	else if(iProject == 9)
	{

		memset(pData->szProjectName, 0, sizeof(pData->szProjectName));
		m_EdtProject9.GetWindowTextA((LPTSTR)pData->szProjectName, 27);
	
		m_EdtTimes9.GetWindowTextA((LPTSTR)szTimes, 4);
		
		pData->iProjectIndex = 9;
		pData->iAddTimes = atoi((char*)szTimes);
		memcpy(pData->szProjectIndex, "09", 2);
		pData->szProjectIndex[3] = 0;
		if(pData->iAddTimes + pData->iTimes > 999)
		{
			AfxMessageBox("��ֵ��Ĵ�����������ֵ(999)�Σ����飡");
			pData->iAddTimes = 0;
			return 3;
		}
	}
	else if(iProject == 10)
	{

		memset(pData->szProjectName, 0, sizeof(pData->szProjectName));
		m_EdtProject10.GetWindowTextA((LPTSTR)pData->szProjectName, 27);

		m_EdtTimes10.GetWindowTextA((LPTSTR)szTimes, 4);
		
		pData->iProjectIndex = 10;
		pData->iAddTimes = atoi((char*)szTimes);
		memcpy(pData->szProjectIndex, "0A", 2);
		pData->szProjectIndex[3] = 0;
		if(pData->iAddTimes + pData->iTimes > 999)
		{
			AfxMessageBox("��ֵ��Ĵ�����������ֵ(999)�Σ����飡");
			pData->iAddTimes = 0;
			return 3;
		}
	}

	return 0;
}

int CMifareDebitDlg::SetProjectData(ProjectData proData, int iProject)
{
	CString strTemp;
	if(iProject == 1)
	{
		strTemp.Format("%d", proData.iTimes);
		m_EdtProject1.SetWindowTextA((LPCTSTR)proData.szProjectName);
		m_EdtTimes1.SetWindowTextA((LPCTSTR)strTemp);
	}
	else if(iProject == 2)
	{
		strTemp.Format("%d", proData.iTimes);
		m_EdtProject2.SetWindowTextA((LPCTSTR)proData.szProjectName);
		m_EdtTimes2.SetWindowTextA((LPCTSTR)strTemp);
	}
	else if(iProject == 3)
	{
		strTemp.Format("%d", proData.iTimes);
		m_EdtProject3.SetWindowTextA((LPCTSTR)proData.szProjectName);
		m_EdtTimes3.SetWindowTextA((LPCTSTR)strTemp);
	}
	else if(iProject == 4)
	{
		strTemp.Format("%d", proData.iTimes);
		m_EdtProject4.SetWindowTextA((LPCTSTR)proData.szProjectName);
		m_EdtTimes4.SetWindowTextA((LPCTSTR)strTemp);
	}
	else if(iProject == 5)
	{
		strTemp.Format("%d", proData.iTimes);
		m_EdtProject5.SetWindowTextA((LPCTSTR)proData.szProjectName);
		m_EdtTimes5.SetWindowTextA((LPCTSTR)strTemp);
	}
	else if(iProject == 6)
	{
		strTemp.Format("%d", proData.iTimes);
		m_EdtProject6.SetWindowTextA((LPCTSTR)proData.szProjectName);
		m_EdtTimes6.SetWindowTextA((LPCTSTR)strTemp);
	}
	else if(iProject == 7)
	{
		strTemp.Format("%d", proData.iTimes);
		m_EdtProject7.SetWindowTextA((LPCTSTR)proData.szProjectName);
		m_EdtTimes7.SetWindowTextA((LPCTSTR)strTemp);
	}
	else if(iProject == 8)
	{
		strTemp.Format("%d", proData.iTimes);
		m_EdtProject8.SetWindowTextA((LPCTSTR)proData.szProjectName);
		m_EdtTimes8.SetWindowTextA((LPCTSTR)strTemp);
	}
	else if(iProject == 9)
	{
		strTemp.Format("%d", proData.iTimes);
		m_EdtProject9.SetWindowTextA((LPCTSTR)proData.szProjectName);
		m_EdtTimes9.SetWindowTextA((LPCTSTR)strTemp);
	}
	else if(iProject == 10)
	{
		strTemp.Format("%d", proData.iTimes);
		m_EdtProject10.SetWindowTextA((LPCTSTR)proData.szProjectName);
		m_EdtTimes10.SetWindowTextA((LPCTSTR)strTemp);
	}

	return 0;
}

BOOL CAboutDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CMifareDebitDlg::OnBnClickedBtnRemoveEmpty()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if((MessageBox("�ò������Ƴ��������д���Ϊ0����Ŀ\r\nȷ���Ƴ�?", "�Ƴ�ȷ�� - �㽭���ڿƼ����޹�˾", MB_OKCANCEL)) == IDCANCEL)
		return;
	m_StDisp.SetWindowTextA("�����Ƴ����ڿ���Ŀ, �����ĵȴ� ...");

	ProjectData tempProj[10];
	unsigned char szBlockData[BLOCK_LEN];
	int iCount = 0, j = 0;
	int iBlock = 0;
	CString strTemp;

	memset(tempProj, 0, sizeof(tempProj));
	memset(szBlockData, 0, BLOCK_LEN);
	for(int i = 0; i != m_iProjectsCount; ++i)
	{
		if(project[i].iTimes != 0)
		{
			tempProj[j].iTimes = project[i].iTimes;
			tempProj[j].iProjectIndex = j + 1;
			sprintf_s((char*)tempProj[j].szProjectIndex, 5, "%02d", j + 1);
			sprintf_s((char*)tempProj[j].szProjectName, 32, "%s", project[i].szProjectName);
			tempProj[j].iAddTimes = 0;
			
			++j;
		}
	}

	m_StDisp.SetWindowTextA("����д�� ...");

	//������Ŀ����ʣ����Ŀ
	m_iProjectsCount = j;
	m_iProjectLeft = 10 - j;

	//д��Ŀ������Ϣ
	iBlock = 22;
	if(AuthSector(iBlock/4, rf.KEY_L))
		return;

	if(rf.ReadMifareBlock(iBlock, szBlockData, rf.HEX))
	{
		m_StDisp.SetWindowTextA("д��Ŀ����Ϣʧ��1!");
		rf.Dev_Beep(3);
		rf.Dev_Beep(3);
		return;
	}

	strTemp.Format("%02X", j);
	memcpy(szBlockData + 24, strTemp, 2);

	if(AuthSector(iBlock/4, rf.KEY_R))
		return;
	if(rf.Write(iBlock, szBlockData, rf.HEX))
	{
		m_StDisp.SetWindowTextA("д��Ŀ����Ϣʧ��2!");
		rf.Dev_Beep(3);
		rf.Dev_Beep(3);
		return;
	}
	///////////////��Ŀ�����޸����

	for(int i = 0; i != j; ++i)
	{
		if(WriteProjectBlock(&tempProj[i]))
			return;
	}

	m_StDisp.SetWindowTextA("�Ƴ�0����Ŀ���!");
	rf.Dev_Beep(10);
	m_BtnRmvEmpty.EnableWindow(FALSE);
	this->OnBnClickedBtnCardinfo();
}


void CMifareDebitDlg::OnBnClickedBtnChangeName()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_EdtName.SetWindowTextA("");
	m_EdtPhoneNO.SetWindowTextA("");
	m_EdtName.EnableWindow(TRUE);
	m_EdtPhoneNO.EnableWindow(TRUE);

	m_iBeginChangeName = 1;
	m_BtnDoChange.EnableWindow(TRUE);
}


void CMifareDebitDlg::OnBnClickedBtnChangeNameDo()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(m_iBeginChangeName == 1)
	{
		unsigned char szName[32];
		unsigned char szPhoneNO[20];
		CString strTemp;

		memset(szPhoneNO, 0, 20);
		memset(szName, 0, 32);

		strTemp.Format("���޸��������ݣ�\r\n");

		/////////�õ�����
		m_EdtName.GetWindowTextA((LPTSTR)szName, 11);
		strTemp.AppendFormat("������%s\r\n", szName);
		/////////�õ���ϵ�绰
		m_EdtPhoneNO.GetWindowTextA((LPTSTR)szPhoneNO, 13);
		if(CheckNumberData(szPhoneNO))
		{
			AfxMessageBox("��ϵ�绰���ݲ���Ϊ0-9���������ֵ��");
			m_StDisp.SetWindowTextA("��ϵ�绰���ݲ���Ϊ0-9���������ֵ��");
			return;
		}
		strTemp.AppendFormat("��ϵ�绰��%s \r\n", szPhoneNO);

		//if(strlen((char*)szName) == 0 || strlen((char*)szPhoneNO) == 0)
		//{
		//	AfxMessageBox("�����͵绰���벻��Ϊ�գ�");
		//	m_StDisp.SetWindowTextA("�����͵绰���벻��Ϊ�գ�");
		//	return;
		//}

		if(MessageBox(strTemp, "������Ϣ�޸�ȷ�� - �㽭���ڿƼ����޹�˾", MB_OKCANCEL) == IDOK)
		{
			strTemp.Format("������Ϣ�޸Ĳ���	-�������ţ�%s	-VIP���ţ�%s	-(�޸�ǰ)������%s	-(�޸�ǰ)�绰����: %s	-(�޸ĺ�)������%s	-(�޸ĺ�)�绰���룺%s", 
				m_cardData.szCardNO, m_cardData.szVIPCardNO, m_cardData.szName, m_cardData.szPhoneNO, szName, szPhoneNO);
			WriteLog(strTemp);

			unsigned char szBlockData[BLOCK_LEN];
			int iBlock;
			int iLen;

			memset(szBlockData, 0, BLOCK_LEN);
			iLen = strlen((char*)szName);
			if(iLen > 10)
			{
				WriteLog("��Ϣ���ʧ�ܣ��������ȴ���");
				AfxMessageBox("�������ȴ���");
				m_EdtName.EnableWindow(FALSE);
				m_EdtPhoneNO.EnableWindow(FALSE);
				m_iBeginChangeName = 0;
				m_BtnDoChange.EnableWindow(FALSE);
				return;
			}
			rf.Asc2Hex(szName, szBlockData, iLen);
			while(iLen*2 != 20)
			{
				memcpy(szBlockData + iLen * 2, "20", 2);
				iLen += 1;
			}

			memset(szBlockData + 20, 0x30, 12);
			iLen = strlen((char*)szPhoneNO);
			if(iLen > 12)
			{
				WriteLog("��Ϣ���ʧ�ܣ��绰���볤�ȴ���");
				AfxMessageBox("�绰���볤�ȴ���");
				m_EdtName.EnableWindow(FALSE);
				m_EdtPhoneNO.EnableWindow(FALSE);
				m_iBeginChangeName = 0;
				m_BtnDoChange.EnableWindow(FALSE);
				return;
			}

			memcpy(szBlockData + 32 - iLen, szPhoneNO, iLen);
			szBlockData[32] = 0;

			iBlock = 21;
			if(AuthSector(iBlock/4, rf.KEY_R))
			{
				WriteLog("��Ϣ���ʧ�ܣ���֤������Կʧ��");
				AfxMessageBox("��֤������Կʧ�ܣ�");
				m_EdtName.EnableWindow(FALSE);
				m_EdtPhoneNO.EnableWindow(FALSE);
				m_iBeginChangeName = 0;
				m_BtnDoChange.EnableWindow(FALSE);
				return;
			}

			if(rf.Write(iBlock, szBlockData, rf.HEX))
			{
				WriteLog("��Ϣ���ʧ�ܣ�д������ʧ��");
				AfxMessageBox("д������ʧ�ܣ�");
				m_EdtName.EnableWindow(FALSE);
				m_EdtPhoneNO.EnableWindow(FALSE);
				m_iBeginChangeName = 0;
				m_BtnDoChange.EnableWindow(FALSE);
				return;
			}
		}
		
		m_EdtName.EnableWindow(FALSE);
		m_EdtPhoneNO.EnableWindow(FALSE);

		m_iBeginChangeName = 0;
		m_BtnDoChange.EnableWindow(FALSE);
		rf.Dev_Beep(10);
		AfxMessageBox("��Ϣ����ɹ���");
	}
}


void CMifareDebitDlg::OnBnClickedBtnChangeNameCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(m_iBeginChangeName == 1)
	{
		m_EdtName.SetWindowTextA((LPCTSTR)m_cardData.szName);
		m_EdtPhoneNO.SetWindowTextA((LPCTSTR)m_cardData.szPhoneNO);
		m_EdtName.EnableWindow(FALSE);
		m_EdtPhoneNO.EnableWindow(FALSE);
		m_iBeginChangeName = 0;
	}
}


void CMifareDebitDlg::OnBnClickedButtonTest()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	PRINTDLG printinfo;

	printinfo.lStructSize = sizeof(printinfo);
	printinfo.hwndOwner = 0;
	printinfo.hDevMode = 0;
	printinfo.hDevNames = 0;

	//printinfo.Flags = PD_RETURNDC| PD_RETURNDEFAULT;
	printinfo.Flags = 0;

	CPrintDialog dlg(FALSE);

	dlg.DoModal();

	PrintDlg(&printinfo);
	DWORD dwRet = CommDlgExtendedError();
	if(dwRet)
	{
		AfxMessageBox("ȡĬ�ϴ�ӡ��ʧ�ܣ���ѡ���ӡ����");
		printinfo.Flags = 0;
		PrintDlg(&printinfo);
	}

	//////////////001-end/////////////////////
	//////////////self-defined paper size///////////////////

	LPDEVMODE lpDevMode = (LPDEVMODE)::GlobalLock(printinfo.hDevMode);
	if(lpDevMode)
	{//success
		lpDevMode->dmPaperSize = DMPAPER_USER;

		//self defination
		lpDevMode->dmFields = lpDevMode->dmFields|DM_PAPERSIZE|DM_PAPERLENGTH|DM_PAPERWIDTH;

		lpDevMode->dmPaperWidth = (short)WIDTH;
		lpDevMode->dmPaperLength = (short)HEIGHT;

		//��ֽ���� ����/����
		lpDevMode->dmOrientation = DMORIENT_PORTRAIT;
	}
	//unlock;
	::GlobalUnlock(printinfo.hDevMode);

	ResetDC(printinfo.hDC, lpDevMode);

	HDC printDC = printinfo.hDC;
	CDC cdc;
	cdc.Attach(printDC);


	CString strTemp;
	strTemp.Format("��ӡ����");
	cdc.TextOutA(0, 0, strTemp);

}
