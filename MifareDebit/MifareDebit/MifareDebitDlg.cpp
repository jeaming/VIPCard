
// MifareDebitDlg.cpp : 实现文件
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



// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
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


// CMifareDebitDlg 对话框




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


// CMifareDebitDlg 消息处理程序

BOOL CMifareDebitDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
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

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	memset(m_KeyData, 0, sizeof(m_KeyData));
	if(GetKeyData(m_KeyData) != 0)
	{
		AfxMessageBox("初始化配置文件失败！");
		exit(1);
	}

	CVerifyPINDlg ver(m_KeyData);
	if(ver.DoModal() == IDCANCEL)
	{
		exit(0);
	}

	//连接读写器
	if(!rf.m_isConnected)
	{
		if(rf.Dev_Connect(100, 115200))
		{
			m_StDisp.SetWindowTextA("打开读写器失败，请检查读写器连接！");	
		}
	}
	else
		m_StDisp.SetWindowTextA("读写器已连接.");

	m_EdtTitles1.SetWindowTextA("	项目名称");
	m_EdtTitles3.SetWindowTextA("	项目名称");
	m_EdtTitles2.SetWindowTextA("     次数");
	m_EdtTitles4.SetWindowTextA("     次数");

	if(SetProjectList())
		AfxMessageBox("取项目列表失败，请检查配置文件！");

	//m_CmbProject.InsertString(0, "项目列表");
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


	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CMifareDebitDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CMifareDebitDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CMifareDebitDlg::OnBnClickedBtnBegin()
{
	// TODO: 在此添加控件通知处理程序代码
	
	if(!m_isInitialized)
	{
		AfxMessageBox("请先获取卡片信息！");
		return;
	}

	if(m_iCardStatus == 0)
	{//初始化
		m_StBalance.SetWindowTextA("充值金额：");
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
		//m_EdtName.SetWindowTextA("王林");
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
	{//充值

		//iProjects = m_iProjects;
		m_iProjects = m_iProjectsCount;
		//启用金额输入
		m_StBalance.SetWindowTextA("存入金额：");
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
	// TODO: 在此添加控件通知处理程序代码

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
		AfxMessageBox("操作员姓名不能为空！");
		return;
	}

	//数据处理
	if(m_iCardStatus == 0)
	{//初始化

		m_EdtBalance.GetWindowTextA((LPTSTR)szAmount, 8);
		iDataLen = strlen((char*)szAmount);
		for(int i = 0; i != iDataLen; ++i)
		{
			if(szAmount[i] > 0x39 || szAmount[i] < 0x30)
			{
				AfxMessageBox("金额数据不能为0-9以外的其他值！");
				m_StDisp.SetWindowTextA("金额数据不能为0-9以外的其他值！");
				return;
			}
		}
		m_cardData.iAmount = atoi((char*)szAmount);/////////得到金额
		strTemp.Format("将个人化以下信息：\r\n");

		//日志开始
		//时间
		char szTime[32];
		time_t t0 = time(0);
		memset(szTime, 0, 32);
		strftime(szTime, 32, "%Y%m%d%H%M%S", localtime(&t0));
		m_strLog.Format("%s", szTime);

		m_strLog.AppendFormat("	%s", szOperator);
		/////////得到姓名
		m_EdtName.GetWindowTextA((LPTSTR)m_cardData.szName, 21);
		strTemp.AppendFormat("姓名：%s\t", m_cardData.szName);
		/////////得到联系电话
		m_EdtPhoneNO.GetWindowTextA((LPTSTR)m_cardData.szPhoneNO, 13);
		iDataLen = strlen((char*)m_cardData.szPhoneNO);
		for(int i = 0; i != iDataLen; ++i)
		{
			if(m_cardData.szPhoneNO[i] > 0x39 || m_cardData.szPhoneNO[i] < 0x30)
			{
				AfxMessageBox("联系电话数据不能为0-9以外的其他值！");
				m_StDisp.SetWindowTextA("联系电话数据不能为0-9以外的其他值！");
				return;
			}
		}
		strTemp.AppendFormat("联系电话：%s \r\n", m_cardData.szPhoneNO);

		//附属卡卡号
		m_EdtCardNO.GetWindowTextA((LPTSTR)m_cardData.szCardNO, 13);
		if(CheckNumberData(m_cardData.szCardNO))
		{
			AfxMessageBox("附属卡卡号数据有误，请检查！");
			m_StDisp.SetWindowTextA("附属卡卡号数据有误，请检查！");
			return;
		}
		strTemp.AppendFormat("附属卡卡号：%s\t", m_cardData.szCardNO);

		//VIP卡号
		m_EdtVIPCardNO.GetWindowTextA((LPTSTR)m_cardData.szVIPCardNO, 13);
		if(CheckNumberData(m_cardData.szVIPCardNO))
		{
			AfxMessageBox("VIP卡号数据有误，请检查！");
			m_StDisp.SetWindowTextA("VIP卡号数据有误，请检查！");
			return;
		}
		strTemp.AppendFormat("VIP卡号：%s\r\n", m_cardData.szVIPCardNO);
		strTemp.AppendFormat("初始金额：%s\r\n", szAmount);
		
		m_strLog.AppendFormat("	%s	%s", m_cardData.szCardNO, m_cardData.szName);
		m_strLog.AppendFormat("	%s	%s", m_cardData.szVIPCardNO, m_cardData.szPhoneNO);//日志
		m_strLog.AppendFormat("	%d", m_cardData.iAmount);

		//ProjectData project[10];
		//memset(project, 0, sizeof(project));
		for(int i = 1; i != 11-m_iProjectLeft; ++i)
		{
			if(GetProjectData(&project[i - 1], i))
				return;
			project[i-1].iTimes = 0;
			strTemp.AppendFormat("项目%d：%s	充值次数：%d \r\n", i, project[i - 1].szProjectName, project[i - 1].iAddTimes);
			m_strLog.AppendFormat("	%s	%d	%d",	project[i - 1].szProjectName, project[i-1].iAddTimes, project[i-1].iTimes + project[i-1].iAddTimes);
		}

		////得到项目名称1
		//memset(szTimes1, 0, 10);
		//m_EdtProject1.GetWindowTextA((LPTSTR)m_cardData.szProject1, 15);
		//m_EdtTimes1.GetWindowTextA((LPTSTR)szTimes1, 4);
		//if(iDataLen = strlen((char*)m_cardData.szProject1) > 0)
		//{
		//	if(CheckNumberData(szTimes1))
		//	{
		//		AfxMessageBox("项目1数据有误，请检查！");
		//		m_StDisp.SetWindowTextA("项目1数据有误，请检查！");
		//		return;
		//	}
		//	
		//	m_cardData.iProjectTimes3 = atoi((char*)szTimes1);
		//	strTemp.AppendFormat("项目1: %s\t 初始次数：%s\r\n", m_cardData.szProject1, szTimes1);
		//	iProjects = 1;
		//}
		//
		////得到项目数据2
		//memset(szTimes2, 0, 10);
		//m_EdtProject2.GetWindowTextA((LPTSTR)m_cardData.szProject2, 15);
		//m_EdtTimes2.GetWindowTextA((LPTSTR)szTimes2, 4);
		//if((iDataLen = strlen((char*)m_cardData.szProject2) > 0))
		//{
		//	if(CheckNumberData(szTimes2))
		//	{
		//		AfxMessageBox("项目2数据有误，请检查！");
		//		m_StDisp.SetWindowTextA("项目2数据有误，请检查！");
		//		return;
		//	}
		//	m_cardData.iProjectTimes3 = atoi((char*)szTimes2);
		//	strTemp.AppendFormat("项目2: %s\t 初始次数：%s\r\n", m_cardData.szProject2, szTimes2);
		//	iProjects = iProjects*10 + 2;
		//}

		////得到项目数据3
		//memset(szTimes3, 0, 10);
		//m_EdtProject3.GetWindowTextA((LPTSTR)m_cardData.szProject3, 15);
		//m_EdtTimes3.GetWindowTextA((LPTSTR)szTimes3, 4);
		//if((iDataLen = strlen((char*)m_cardData.szProject3) > 0))
		//{
		//	if(CheckNumberData(szTimes3))
		//	{
		//		AfxMessageBox("项目3数据有误，请检查！");
		//		m_StDisp.SetWindowTextA("项目3数据有误，请检查！");
		//		return;
		//	}

		//	m_cardData.iProjectTimes3 = atoi((char*)szTimes3);
		//	strTemp.AppendFormat("项目3: %s\t 初始次数：%s\r\n", m_cardData.szProject3, szTimes3);
		//	iProjects = iProjects*10 +3;
		//}

		if(MessageBox((LPCTSTR)strTemp, (LPCTSTR)"数据确认", MB_OKCANCEL) == IDCANCEL)
			return;

		WriteInitLog(m_strLog);

		strTemp.Format("上一笔<%s>初始化失败：", szTime);

		//开始写入数据
		iBlock = 20;
		memset(szSectorKey, 0, 40);
		memcpy(szSectorKey, m_KeyData + (iBlock/4*24 + 12), 12);
		if(rf.Dev_Reset(1))
		{
			AfxMessageBox("卡片复位失败！");
			m_StDisp.SetWindowTextA("卡片复位失败！");
			return;
		}

		if(rf.LoadKey(rf.KEY_R, iBlock/4, szSectorKey, rf.HEX))
		{
			AfxMessageBox("读写器装载密钥失败！");
			m_StDisp.SetWindowTextA("读写器装载密钥失败！");
			return;
		}
		if(rf.Anthentication(rf.KEY_R, iBlock/4))
		{
			AfxMessageBox("认证密钥失败！");
			m_StDisp.SetWindowTextA("认证密钥失败！");
			return;
		}//认证完成

		if(rf.InitialValue(iBlock, m_cardData.iAmount))
		{//写入金额
			AfxMessageBox("写入金额失败！");
			m_StDisp.SetWindowTextA("写入金额失败！");
			return;			
		}

		iBlock = 21;////////写入姓名
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
			AfxMessageBox("写入姓名信息失败！");
			m_StDisp.SetWindowTextA("写入姓名信息失败！");
			strTemp += "写入姓名信息失败";
			WriteLog(strTemp);
			return;	
		}

		iBlock = 22;//联系电话、卡号、VIP卡号、状态
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
			AfxMessageBox("写入VIP卡号信息失败！");
			m_StDisp.SetWindowTextA("写入VIP卡号信息失败！");
			strTemp += "写入VIP卡号信息失败";
			WriteLog(strTemp);
			return;	
		}

		for(int i = 1; i != 11 -m_iProjectLeft; ++i)
		{
			iRet = WriteProjectBlock(&project[i - 1]);
			if(iRet)
			{
				strTemp.Format("写项目%d失败，请重试！", i);
				AfxMessageBox(strTemp);
				strTemp.Format("上一笔<%s>初始化失败：写项目%d失败", szTime, i);
				WriteLog(strTemp);
				return;
			}
		}

		///////写入项目信息
		////7扇区其他项目
		////认证7扇区
		//iBlock = 28;
		//memset(szSectorKey, 0, 40);
		//memcpy(szSectorKey, m_KeyData + iBlock/4*24 + 12, 12);
		//if(rf.Dev_Reset(1))
		//{
		//	AfxMessageBox("未找到卡片，请检查！");
		//	m_StDisp.SetWindowTextA("未找到卡片，请检查！");
		//	return;
		//}

		//if(rf.LoadKey(rf.KEY_R, iBlock/4, szSectorKey, rf.HEX))
		//{
		//	AfxMessageBox("读写器装载密钥失败！");
		//	m_StDisp.SetWindowTextA("读写器装载密钥失败！");
		//	return;
		//}
		//if(rf.Anthentication(rf.KEY_R, iBlock/4))
		//{
		//	AfxMessageBox("认证密钥失败！");
		//	m_StDisp.SetWindowTextA("认证密钥失败！");
		//	return;
		//}
		////认证完成

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
		//		AfxMessageBox("充值失败(3)!");
		//		m_StDisp.SetWindowTextA("充值失败(3)!");
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
		//		AfxMessageBox("充值失败(2)!");
		//		m_StDisp.SetWindowTextA("充值失败(2)!");
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
		//		AfxMessageBox("充值失败(3)!");
		//		m_StDisp.SetWindowTextA("充值失败(3)!");
		//		return;
		//	}
		//	iProjects /= 10;
		//}
		AfxMessageBox("发卡成功！");
		m_StDisp.SetWindowTextA("发卡成功！");
		//this->OnBnClickedBtnCardinfo();

	}
	else if(m_iCardStatus == 1)
	{
		//金额数据
		m_EdtBalance.GetWindowTextA((LPTSTR)szAmount, 7);
		iDataLen = strlen((char*)szAmount);
		for(int i = 0; i != iDataLen; ++i)
		{
			if(szAmount[i] > 0x39 || szAmount[i] < 0x30)
			{
				AfxMessageBox("金额数据不能为0-9以外的其他值！");
				m_StDisp.SetWindowTextA("金额数据不能为0-9以外的其他值！");
				return;
			}
		}
		lAmount = atoi((char*)szAmount);/////////得到金额

		strTemp.Format("将对以下数据进行充值：\r\n");
		strTemp.AppendFormat("卡号：%s		姓名：%s\r\n", m_cardData.szCardNO, m_cardData.szName);
		if(lAmount > 0)
		{
			strTemp.AppendFormat("充值金额：%d\r\n", lAmount);
			m_hasLoadProject = 1;
		}
		iProjects = m_iProjects;

		//日志开始
		//时间
		char szTime[32];
		time_t t0 = time(0);
		memset(szTime, 0, 32);
		strftime(szTime, 32, "%Y%m%d%H%M%S", localtime(&t0));
		m_strLog.Format("%s", szTime);		
		m_strLog.AppendFormat("	%s", szOperator);
		m_strLog.AppendFormat("	%s	%s", m_cardData.szCardNO, m_cardData.szName);
		m_strLog.AppendFormat("	%s	%d	%d", m_cardData.szPhoneNO, m_cardData.iAmount, lAmount, m_cardData.iAmount + lAmount);//日志

		for(int i = 1; i != 11 - m_iProjectLeft; ++i)
		{
			if(GetProjectData(&project[i - 1], i))
				return;
			if(project[i-1].iAddTimes > 0)
			{
				strTemp.AppendFormat("项目%d：%s	充值次数：%d \r\n", i, project[i - 1].szProjectName, project[i - 1].iAddTimes);
				m_strLog.AppendFormat("	%s	%d	%d", project[i-1].szProjectName, project[i - 1].iAddTimes, project[i -1].iTimes + project[i-1].iAddTimes);
				m_hasLoadProject = 1;
			}
		}

		//if(iProjects%10 == 3)
		//{
		//	//得到项目数据3
		//	memset(szTimes3, 0, 10);
		//	m_EdtTimes3.GetWindowTextA((LPTSTR)szTimes3, 4);
		//	if(CheckNumberData(szTimes3))
		//	{
		//		AfxMessageBox("项目3数据有误，请检查！");
		//		m_StDisp.SetWindowTextA("项目3数据有误，请检查！");
		//		return;
		//	}
		//	strTemp.AppendFormat("项目3：%s, 充值次数：%s\r\n", m_cardData.szProject3, szTimes3);
		//	iProjects /= 10;
		//}
		//
		//if(iProjects%10 == 2)
		//{
		//	//得到项目数据2
		//	memset(szTimes2, 0, 10);
		//	m_EdtTimes2.GetWindowTextA((LPTSTR)szTimes2, 4);
		//	if(CheckNumberData(szTimes2))
		//	{
		//		AfxMessageBox("项目2数据有误，请检查！");
		//		m_StDisp.SetWindowTextA("项目2数据有误，请检查！");
		//		return;
		//	}
		//	strTemp.AppendFormat("项目2：%s, 充值次数：%s\r\n", m_cardData.szProject2, szTimes2);
		//	iProjects /= 10;
		//}
		//if(iProjects == 1)
		//{
		//	//得到项目名称1
		//	memset(szTimes1, 0, 10);
		//	m_EdtTimes1.GetWindowTextA((LPTSTR)szTimes1, 4);
		//	if(CheckNumberData(szTimes1))
		//	{
		//		AfxMessageBox("项目1数据有误，请检查！");
		//		m_StDisp.SetWindowTextA("项目1数据有误，请检查！");
		//		return;
		//	}
		//	strTemp.AppendFormat("项目1：%s, 充值次数：%s\r\n", m_cardData.szProject1, szTimes1);	
		//}

		if(!m_hasLoadProject)
		{
			AfxMessageBox("无充值项目，退出充值！");	
			this->OnBnClickedBtnCardinfo();
			m_StBalance.SetWindowTextA("余额：");
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

		if(MessageBox((LPCTSTR)strTemp,(LPCTSTR)"充值数据确认", MB_OKCANCEL) == IDCANCEL)
			return;

		//写日志
		WriteLog(m_strLog);

		strTemp.Format("上一笔<%s>初始化失败：", szTime);

		//开始写数据
	//	//5扇区认证
	//	iBlock = 20;
	//	memset(szSectorKey, 0, 40);
	//	memcpy(szSectorKey, m_KeyData + iBlock/4*24 + 12, 12);
	//	if(rf.Dev_Reset(1))
	//	{
	//		AfxMessageBox("未找到卡片，请检查！");
	//		m_StDisp.SetWindowTextA("未找到卡片，请检查！");
	//		return;
	//	}

	//	if(rf.LoadKey(rf.KEY_R, iBlock/4, szSectorKey, rf.HEX))
	//	{
	//		AfxMessageBox("读写器装载密钥失败！");
	//		m_StDisp.SetWindowTextA("读写器装载密钥失败！");
	//		return;
	//	}
	//	if(rf.Anthentication(rf.KEY_R, iBlock/4))
	//	{
	//		AfxMessageBox("认证密钥失败！");
	//		m_StDisp.SetWindowTextA("认证密钥失败！");
	//		return;
	//	}

	//	//20块加值
	//	if(lAmount > 0)
	//	{
	//		if(rf.Increment(iBlock, lAmount))
	//		{
	//			strTemp.Format("充值消费金额失败，返回值：%d", iRet);
	//			AfxMessageBox(strTemp);
	//			m_StDisp.SetWindowTextA(strTemp);
	//			return;
	//		}
	//	}

	//	//7扇区其他项目
	//	//认证7扇区
	//	iBlock = 28;
	//	memset(szSectorKey, 0, 40);
	//	memcpy(szSectorKey, m_KeyData + iBlock/4*24 + 12, 12);
	//	if(rf.Dev_Reset(1))
	//	{
	//		AfxMessageBox("未找到卡片，请检查！");
	//		m_StDisp.SetWindowTextA("未找到卡片，请检查！");
	//		return;
	//	}

	//	if(rf.LoadKey(rf.KEY_R, iBlock/4, szSectorKey, rf.HEX))
	//	{
	//		AfxMessageBox("读写器装载密钥失败！");
	//		m_StDisp.SetWindowTextA("读写器装载密钥失败！");
	//		return;
	//	}
	//	if(rf.Anthentication(rf.KEY_R, iBlock/4))
	//	{
	//		AfxMessageBox("认证密钥失败！");
	//		m_StDisp.SetWindowTextA("认证密钥失败！");
	//		return;
	//	}
	//	//认证完成
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
	//			AfxMessageBox("充值失败(3)!");
	//			m_StDisp.SetWindowTextA("充值失败(3)!");
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
	//			AfxMessageBox("充值失败(2)!");
	//			m_StDisp.SetWindowTextA("充值失败(2)!");
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
	//			AfxMessageBox("充值失败(3)!");
	//			m_StDisp.SetWindowTextA("充值失败(3)!");
	//			return;
	//		}
	//	}
	//	AfxMessageBox("充值成功！");
	//	m_StDisp.SetWindowTextA("充值成功！");
	//	this->OnBnClickedBtnCardinfo();

	//}
	//else
	//{
	//	AfxMessageBox("卡片状态未知，请检查卡片！");
	//	m_StDisp.SetWindowTextA("卡片状态未知，请检查卡片！");
	//	return;
	//}
		
		//开始写数据
		for(int i = 1; i != 11 - m_iProjectLeft; ++i)
		{
			iRet = WriteProjectBlock(&project[i - 1]);
			if(iRet)
			{
				strTemp.Format("写%d项目失败！", i);
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
			strTemp.Format("增加卡片余额失败！增加金额：%d", lAmount);
			AfxMessageBox(strTemp);
			strTemp.Format("上一笔<%s>充值失败：增加卡片余额失败", szTime);
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
			strTemp += "认证扇区密钥失败";
			WriteLog(strTemp);
			return;
		}

		if(rf.Write(iBlock, szBlockData, rf.HEX))
		{
			AfxMessageBox("写项目信息失败！");
			strTemp += "写项目信息失败";
			WriteLog(strTemp);
			return;
		}
		
		AfxMessageBox("充值成功！");
		m_StDisp.SetWindowTextA("充值成功！");
		//this->OnBnClickedBtnCardinfo();
	}

	m_StBalance.SetWindowTextA("余额：");
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

	//得到扇区密钥
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
	// TODO: 在此添加控件通知处理程序代码

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



	//设备操作机及密钥认证
	//连接读写器
	if(!rf.m_isConnected)
	{
		if(rf.Dev_Connect(100, 115200))
		{
			AfxMessageBox("连接读写器失败，请检查设备！");
			m_StDisp.SetWindowTextA("连接读写器失败，请检查设备！");
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
		AfxMessageBox("读卡号失败！请检查卡片！");
		return;
	}

	//卡状态77 未个人化； 01 已个人化，待充值； 03 已锁定； 
	memset(szCardStatus, 0, 5);
	memcpy(szCardStatus, szBlockData + 30, 2);
	if(!memcmp(szCardStatus, "03", 2))
	{
		AfxMessageBox("卡片已锁定，请确认当前卡片！");
		m_StDisp.SetWindowTextA("卡片已锁定，请确认当前卡片！");
		m_iCardStatus = 3;
		return;
	}
	else if(!memcmp(szCardStatus, "01", 2))
	{//////个人化，充值
		m_StDisp.SetWindowTextA("卡片已个人化，请充值！");

		//卡号
		memcpy(m_cardData.szCardNO, szBlockData, 12);
		m_EdtCardNO.SetWindowTextA((LPCTSTR)m_cardData.szCardNO);

		//vip卡号
		memcpy(m_cardData.szVIPCardNO, szBlockData + 12, 12);
		m_EdtVIPCardNO.SetWindowTextA((LPCTSTR)m_cardData.szVIPCardNO);

		unsigned char szProjectCount[5];
		memset(szProjectCount, 0, 5);
		memcpy(szProjectCount, szBlockData + 24, 2);
		m_iProjectsCount = rf.Hex2Dec(szProjectCount);///////项目数量
		m_iProjectLeft = 10 - m_iProjectsCount;
		if(m_iProjectsCount > 10 || m_iProjectsCount < 0)
		{
			AfxMessageBox("项目数量有误，请检查卡片！");
			return;
		}

		//读姓名
		iBlock = 21;
		memset(szBlockData, 0, 40);
		if(rf.ReadMifareBlock(iBlock, szBlockData, rf.HEX))
		{
			AfxMessageBox("卡片状态失败！请检查卡片！");
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

		//联系电话
		memcpy(m_cardData.szPhoneNO, szBlockData + 20, 12);
		m_EdtPhoneNO.SetWindowTextA((LPCTSTR)m_cardData.szPhoneNO);

		iBlock = 20;
		if(rf.ReadValue(iBlock, &lMoney))
		{
			AfxMessageBox("取余额失败！");
			m_StDisp.SetWindowTextA("取余额失败！");
			return;
		}

		strTemp.Format("%d", lMoney);
		m_cardData.iAmount = lMoney;
		m_EdtBalance.SetWindowTextA((LPCTSTR)strTemp);//显示余额

		//读项目信息
		memset(project, 0, sizeof(project));
		for(int i = 1; i != m_iProjectsCount + 1; ++i)
		{
			if(ReadProjectBlock(&project[i - 1], i))
				return;
		}
		//显示项目数据
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

		////得到左密钥
		////信息区块为28 29 30
		//iBlock = 28;
		//memset(szSectorKey, 0, 40);

		//memcpy(szSectorKey, m_KeyData + (iBlock/4)*24, 12);
		//if(rf.LoadKey(rf.KEY_L, iBlock/4, szSectorKey, rf.HEX))
		//{
		//	AfxMessageBox("认证密钥失败！");
		//	m_StDisp.SetWindowTextA("认证密钥失败！");
		//	return;
		//}

		//if(rf.Anthentication(rf.KEY_L, iBlock/4))
		//{
		//	AfxMessageBox("认证密钥失败2！");
		//	m_StDisp.SetWindowTextA("认证密钥失败2！");
		//	return;
		//}
		////认证密钥完成

		////读项目1
		//int iDataLen = 0;

		//memset(szBlockData, 0, 40);
		//if(rf.ReadMifareBlock(iBlock, szBlockData, rf.HEX))
		//{
		//	AfxMessageBox("读卡内信息失败(1)!");
		//	m_StDisp.SetWindowTextA("读卡内信息失败(1)!");
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

		////项目2
		//iBlock = 29;
		//memset(szBlockData, 0, 40);
		//if(rf.ReadMifareBlock(iBlock, szBlockData, rf.HEX))
		//{
		//	AfxMessageBox("读卡内信息失败(1)!");
		//	m_StDisp.SetWindowTextA("读卡内信息失败(1)!");
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
		////项目3
		//iBlock = 30;
		//memset(szBlockData, 0, 40);
		//if(rf.ReadMifareBlock(iBlock, szBlockData, rf.HEX))
		//{
		//	AfxMessageBox("读卡内信息失败(1)!");
		//	m_StDisp.SetWindowTextA("读卡内信息失败(1)!");
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
		
		m_BtnBegin.SetWindowTextA("开始充值");
		m_BtnLoad.SetWindowTextA("充值");
		m_iCardStatus = 1;
	}
	else if(!memcmp(szCardStatus, "77", 2))
	{//未个人化
		m_StDisp.SetWindowTextA("该卡为出厂状态，请开卡并保存相应服务信息！");

		//读联系方式\卡号信息
		iBlock = 22;
		memset(szBlockData, 0, 40);
		if(rf.ReadMifareBlock(iBlock, szBlockData, rf.HEX))
		{
			AfxMessageBox("读卡号失败！请检查卡片！");
			return;
		}

		//卡号 待写入
		//memcpy(m_cardData.szCardNO, szBlockData + 12, 6);
		//m_EdtCardNO.SetWindowTextA((LPCTSTR)m_cardData.szCardNO);		

		m_iProjects = 0;
		m_iProjectsCount = 0;
		m_iProjectLeft = 10;
		m_iCardStatus = 0;
		
		m_BtnBegin.SetWindowTextA("开始发卡");
		m_BtnLoad.SetWindowTextA("发卡");
	}
	else
	{
		m_StDisp.SetWindowTextA("未知卡片!");
	}
	//启用控件
	m_StBalance.SetWindowTextA("余额：");
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
{//1 表示有非数字字符
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
	// TODO: 在此添加控件通知处理程序代码

	CDialog::OnCancel();
}

int CMifareDebitDlg::AuthSector(int iSector, int iKeyModal)
{
	//得到左密钥
	//信息区块为20 21 22
	unsigned char szSectorKey[40];
	memset(szSectorKey, 0, 40);
	if(rf.Dev_Reset(1))
	{
		AfxMessageBox("卡片复位失败！");
		m_StDisp.SetWindowTextA("卡片复位失败！");
		return 1;
	}
	
	if(iKeyModal == rf.KEY_L)
		memcpy(szSectorKey, m_KeyData + iSector*24, 12);
	else
		memcpy(szSectorKey, m_KeyData + iSector*24 + 12, 12);

	if(rf.LoadKey(iKeyModal, iSector, szSectorKey, rf.HEX))
	{
		AfxMessageBox("认证密钥失败！");
		m_StDisp.SetWindowTextA("认证密钥失败！");
		return 1;	
	}

	if(rf.Anthentication(iKeyModal, iSector))
	{
		AfxMessageBox("认证密钥失败2！");
		m_StDisp.SetWindowTextA("认证密钥失败2！");
		return 1;
	}
	//认证密钥完成

	return 0;
}

int CMifareDebitDlg::SetProjectList(void)
{///////////////////////初始化时设置COMBO列表
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
		AfxMessageBox("未定义的项目，请查证数据！");
		return 2;
	}

	iRet = AuthSector(iBlock/4, rf.KEY_R);
	if(iRet)
	{
		AfxMessageBox("认证卡片密钥失败，请检查卡片！");
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
		AfxMessageBox("写项目数据失败，请检查卡片！");
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
		AfxMessageBox("未定义的项目，请检查数据！");
		return 3;
	}

	iRet = AuthSector(iBlock/4, rf.KEY_L);
	if(iRet)
	{
		AfxMessageBox("认证密钥失败，请检查卡片！");
		return 3;
	}
	
	memset(szBlockData, 0, BLOCK_LEN);
	if(rf.ReadMifareBlock(iBlock, szBlockData, rf.HEX))
	{
		AfxMessageBox("读项目数据失败，请检查卡片！");
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
	// TODO: 在此添加控件通知处理程序代码

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
	// TODO: 在此添加控件通知处理程序代码
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
			AfxMessageBox("充值后的次数大于上限值(999)次，请检查！");
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
			AfxMessageBox("充值后的次数大于上限值(999)次，请检查！");
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
			AfxMessageBox("充值后的次数大于上限值(999)次，请检查！");
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
			AfxMessageBox("充值后的次数大于上限值(999)次，请检查！");
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
			AfxMessageBox("充值后的次数大于上限值(999)次，请检查！");
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
			AfxMessageBox("充值后的次数大于上限值(999)次，请检查！");
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
			AfxMessageBox("充值后的次数大于上限值(999)次，请检查！");
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
			AfxMessageBox("充值后的次数大于上限值(999)次，请检查！");
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
			AfxMessageBox("充值后的次数大于上限值(999)次，请检查！");
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
			AfxMessageBox("充值后的次数大于上限值(999)次，请检查！");
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

	// TODO:  在此添加额外的初始化

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CMifareDebitDlg::OnBnClickedBtnRemoveEmpty()
{
	// TODO: 在此添加控件通知处理程序代码
	if((MessageBox("该操作将移除卡内所有次数为0的项目\r\n确认移除?", "移除确认 - 浙江美融科技有限公司", MB_OKCANCEL)) == IDCANCEL)
		return;
	m_StDisp.SetWindowTextA("正在移除卡内空项目, 请耐心等待 ...");

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

	m_StDisp.SetWindowTextA("正在写卡 ...");

	//重置项目数和剩余项目
	m_iProjectsCount = j;
	m_iProjectLeft = 10 - j;

	//写项目数量信息
	iBlock = 22;
	if(AuthSector(iBlock/4, rf.KEY_L))
		return;

	if(rf.ReadMifareBlock(iBlock, szBlockData, rf.HEX))
	{
		m_StDisp.SetWindowTextA("写项目数信息失败1!");
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
		m_StDisp.SetWindowTextA("写项目数信息失败2!");
		rf.Dev_Beep(3);
		rf.Dev_Beep(3);
		return;
	}
	///////////////项目数量修改完成

	for(int i = 0; i != j; ++i)
	{
		if(WriteProjectBlock(&tempProj[i]))
			return;
	}

	m_StDisp.SetWindowTextA("移除0次项目完成!");
	rf.Dev_Beep(10);
	m_BtnRmvEmpty.EnableWindow(FALSE);
	this->OnBnClickedBtnCardinfo();
}


void CMifareDebitDlg::OnBnClickedBtnChangeName()
{
	// TODO: 在此添加控件通知处理程序代码
	m_EdtName.SetWindowTextA("");
	m_EdtPhoneNO.SetWindowTextA("");
	m_EdtName.EnableWindow(TRUE);
	m_EdtPhoneNO.EnableWindow(TRUE);

	m_iBeginChangeName = 1;
	m_BtnDoChange.EnableWindow(TRUE);
}


void CMifareDebitDlg::OnBnClickedBtnChangeNameDo()
{
	// TODO: 在此添加控件通知处理程序代码
	if(m_iBeginChangeName == 1)
	{
		unsigned char szName[32];
		unsigned char szPhoneNO[20];
		CString strTemp;

		memset(szPhoneNO, 0, 20);
		memset(szName, 0, 32);

		strTemp.Format("将修改以下数据：\r\n");

		/////////得到姓名
		m_EdtName.GetWindowTextA((LPTSTR)szName, 11);
		strTemp.AppendFormat("姓名：%s\r\n", szName);
		/////////得到联系电话
		m_EdtPhoneNO.GetWindowTextA((LPTSTR)szPhoneNO, 13);
		if(CheckNumberData(szPhoneNO))
		{
			AfxMessageBox("联系电话数据不能为0-9以外的其他值！");
			m_StDisp.SetWindowTextA("联系电话数据不能为0-9以外的其他值！");
			return;
		}
		strTemp.AppendFormat("联系电话：%s \r\n", szPhoneNO);

		//if(strlen((char*)szName) == 0 || strlen((char*)szPhoneNO) == 0)
		//{
		//	AfxMessageBox("姓名和电话号码不能为空！");
		//	m_StDisp.SetWindowTextA("姓名和电话号码不能为空！");
		//	return;
		//}

		if(MessageBox(strTemp, "个人信息修改确认 - 浙江美融科技有限公司", MB_OKCANCEL) == IDOK)
		{
			strTemp.Format("个人信息修改操作	-附属卡号：%s	-VIP卡号：%s	-(修改前)姓名：%s	-(修改前)电话号码: %s	-(修改后)姓名：%s	-(修改后)电话号码：%s", 
				m_cardData.szCardNO, m_cardData.szVIPCardNO, m_cardData.szName, m_cardData.szPhoneNO, szName, szPhoneNO);
			WriteLog(strTemp);

			unsigned char szBlockData[BLOCK_LEN];
			int iBlock;
			int iLen;

			memset(szBlockData, 0, BLOCK_LEN);
			iLen = strlen((char*)szName);
			if(iLen > 10)
			{
				WriteLog("信息变更失败：姓名长度错误");
				AfxMessageBox("姓名长度错误！");
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
				WriteLog("信息变更失败：电话号码长度错误");
				AfxMessageBox("电话号码长度错误！");
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
				WriteLog("信息变更失败：认证扇区密钥失败");
				AfxMessageBox("认证扇区密钥失败！");
				m_EdtName.EnableWindow(FALSE);
				m_EdtPhoneNO.EnableWindow(FALSE);
				m_iBeginChangeName = 0;
				m_BtnDoChange.EnableWindow(FALSE);
				return;
			}

			if(rf.Write(iBlock, szBlockData, rf.HEX))
			{
				WriteLog("信息变更失败：写入数据失败");
				AfxMessageBox("写入数据失败！");
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
		AfxMessageBox("信息变更成功！");
	}
}


void CMifareDebitDlg::OnBnClickedBtnChangeNameCancel()
{
	// TODO: 在此添加控件通知处理程序代码
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
	// TODO: 在此添加控件通知处理程序代码
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
		AfxMessageBox("取默认打印机失败，请选择打印机！");
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

		//走纸方向 横向/纵向
		lpDevMode->dmOrientation = DMORIENT_PORTRAIT;
	}
	//unlock;
	::GlobalUnlock(printinfo.hDevMode);

	ResetDC(printinfo.hDC, lpDevMode);

	HDC printDC = printinfo.hDC;
	CDC cdc;
	cdc.Attach(printDC);


	CString strTemp;
	strTemp.Format("打印测试");
	cdc.TextOutA(0, 0, strTemp);

}
