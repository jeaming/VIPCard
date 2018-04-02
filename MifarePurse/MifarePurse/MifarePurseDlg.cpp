
// MifarePurseDlg.cpp : 实现文件
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


// CMifarePurseDlg 对话框




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


// CMifarePurseDlg 消息处理程序

BOOL CMifarePurseDlg::OnInitDialog()
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
	if(GetKeyData(m_KeyData))
	{
		AfxMessageBox("初始化配置文件失败！");
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


	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CMifarePurseDlg::OnPaint()
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
HCURSOR CMifarePurseDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

//消费
void CMifarePurseDlg::OnBnClickedBtnPurchase()
{
	// TODO: 在此添加控件通知处理程序代码
	if(!m_isInitialized)
		this->OnBnClickedBtnInfo();

	if(m_cardData.iFlag != 1)
	{
		AfxMessageBox("卡片状态异常，无法消费！");
		return;
	}

	CPurseCardDlg dlg(&m_cardData, m_KeyData, &rf);
	dlg.DoModal();
	m_isInitialized = FALSE;
}

//储值
void CMifarePurseDlg::OnBnClickedBtnCredit()
{
	// TODO: 在此添加控件通知处理程序代码
	
	if(!m_isInitialized)
		this->OnBnClickedBtnInfo();
	rf.Dev_Close();
	CReloadDlg dlg;
	dlg.DoModal();
	m_isInitialized = FALSE;

}

//个人信息
void CMifarePurseDlg::OnBnClickedBtnInfo()
{
	// TODO: 在此添加控件通知处理程序代码
	int iRet = 0, iBlock, iDataLen = 0;
	CString strTemp;
	unsigned char szSectorKey[40], szBlockData[40];
	//unsigned char szProjectTimes[5], szProjectName[40];
	unsigned long lMoney = 0;
	//int iProjectTimes;
	
	//设备操作机及密钥认证
	//连接读写器
	if(!rf.m_isConnected)
	{
		if(rf.Dev_Connect(100, 115200))
		{
			AfxMessageBox("连接读写器失败，请检查设备！");
			return;
		}
	}

	//卡片复位
	if((iRet = rf.Dev_Reset(1)))
	{
		strTemp.Format("卡片复位失败，返回值：%d", iRet);
		AfxMessageBox(strTemp);
		return;
	}

	//得到左密钥
	//信息区块为20 21 22
	iBlock = 20;
	memset(szSectorKey, 0, 40);

	memcpy(szSectorKey, m_KeyData + (iBlock/4)*24, 12);
	if(rf.LoadKey(rf.KEY_L, iBlock/4, szSectorKey, rf.HEX))
	{
		AfxMessageBox("认证密钥失败！");
		return;
	}

	if(rf.Anthentication(rf.KEY_L, iBlock/4))
	{
		AfxMessageBox("认证密钥失败2！");
		return;
	}
	//认证密钥完成

	if(rf.ReadValue(iBlock, &lMoney))
	{
		AfxMessageBox("取余额失败！");
		return;
	}

	strTemp.Format("%d", lMoney);
	m_cardData.iAmount = lMoney;
	m_EdtBalance.SetWindowTextA((LPCTSTR)strTemp);//显示余额

	//读姓名
	memset(szBlockData, 0, 40);
	memset(m_szName, 0, 60);
	iBlock = 21;
	memset(szBlockData, 0, 40);
	if(rf.ReadMifareBlock(iBlock, szBlockData, rf.HEX))
	{
		AfxMessageBox("读姓名失败！请检查卡片！");
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

	//联系电话
	memcpy(m_cardData.szPhoneNO, szBlockData + 20, 12);
	m_cardData.szPhoneNO[12] = 0;
	m_EdtPhoneNO.SetWindowTextA((LPCTSTR)m_cardData.szPhoneNO);


	//读联系方式\卡号信息
	iBlock = 22;
	memset(szBlockData, 0, 40);
	if(rf.ReadMifareBlock(iBlock, szBlockData, rf.HEX))
	{
		AfxMessageBox("读卡号失败！请检查卡片！");
		return;
	}


	//卡号
	memcpy(m_cardData.szCardNO, szBlockData, 12);
	m_EdtCardNO.SetWindowTextA((LPCTSTR)m_cardData.szCardNO);

	//VIP卡号设定为12====================================================120520
	memcpy(m_cardData.szVIPCardNO, szBlockData + 12, 12);
	m_EdtVIPCardNO.SetWindowTextA((LPCTSTR)m_cardData.szVIPCardNO);

	unsigned char szProjectsCount[10];

	memset(szProjectsCount, 0, 10);
	memcpy(szProjectsCount, szBlockData + 24, 2);

	m_cardData.iProjectsCount = rf.Hex2Dec(szProjectsCount);

	//卡片标识
	memcpy(szProjectsCount, szBlockData + 30, 2);
	m_cardData.iFlag = atoi((char*)szProjectsCount);
	if(m_cardData.iFlag == 1)
	{
	}
	else if(m_cardData.iFlag == 0)
	{
		AfxMessageBox("该卡为未个人化卡片！");
		return;
	}
	else
	{
		AfxMessageBox("卡片状态不正确，请检查！");
		return;
	}


	///////////////////////////////////////////读7扇区数据,3块可用项目
	////卡片复位
	//if((iRet = rf.Dev_Reset(1)))
	//{
	//	strTemp.Format("卡片复位失败，返回值：%d", iRet);
	//	AfxMessageBox(strTemp);
	//	return;
	//}

	////得到左密钥
	////信息区块为28 29 30
	//iBlock = 28;
	//memset(szSectorKey, 0, 40);

	//memcpy(szSectorKey, m_KeyData + (iBlock/4)*24, 12);
	//if(rf.LoadKey(rf.KEY_L, iBlock/4, szSectorKey, rf.HEX))
	//{
	//	AfxMessageBox("3认证密钥失败！");
	//	return;
	//}

	//if(rf.Anthentication(rf.KEY_L, iBlock/4))
	//{
	//	AfxMessageBox("4认证密钥失败！");
	//	return;
	//}
	////认证完成

	////读28块
	//memset(szBlockData, 0, 40);
	//if(rf.ReadMifareBlock(iBlock, szBlockData, rf.HEX))
	//{
	//	AfxMessageBox("读信息失败，请检查卡片！");
	//	return;
	//}
	////28块如果等于01 则表明有数据
	//if(!memcmp(szBlockData, "01", 2))
	//{
	//	//处理数据
	//	memset(szProjectTimes, 0, 5);
	//	memset(szProjectName, 0, 40);
	//	memcpy(szProjectTimes, szBlockData + 2, 2);
	//	m_cardData.iProjectTimes1 = rf.Hex2Dec(szProjectTimes);
	//	strTemp.Format("%d", m_cardData.iProjectTimes1);
	//	m_EdtProjectTimes1.SetWindowTextA(strTemp);//项目1 剩余次数

	//	iDataLen = 28;
	//	memcpy(szProjectName, szBlockData + 4, iDataLen);
	//	rf.SwitchDataMode(szProjectName, &iDataLen, rf.HEX2ASC);
	//	strTemp.Format("%s",szProjectName);
	//	strTemp.Remove(0x20);
	//	memcpy(m_cardData.szProject1, strTemp, strTemp.GetLength());
	//	m_EdtProject1.SetWindowTextA((LPCTSTR)strTemp);//项目名称
	//}
	//
	////读29块
	//iBlock = 29;
	//memset(szBlockData, 0, 40);
	//if(rf.ReadMifareBlock(iBlock, szBlockData, rf.HEX))
	//{
	//	AfxMessageBox("读信息失败，请检查卡片！");
	//	return;
	//}
	////29块如果等于02 则表明有数据
	//if(!memcmp(szBlockData, "02", 2))
	//{
	//	//处理数据
	//	memset(szProjectTimes, 0, 5);
	//	memset(szProjectName, 0, 40);
	//	memcpy(szProjectTimes, szBlockData + 2, 2);
	//	m_cardData.iProjectTimes2 = rf.Hex2Dec(szProjectTimes);
	//	strTemp.Format("%d", m_cardData.iProjectTimes2);
	//	m_EdtProjectTimes2.SetWindowTextA(strTemp);//项目2 剩余次数

	//	iDataLen = 28;
	//	memcpy(szProjectName, szBlockData + 4, iDataLen);
	//	rf.SwitchDataMode(szProjectName, &iDataLen, rf.HEX2ASC);
	//	strTemp.Format("%s",szProjectName);
	//	strTemp.Remove(0x20);
	//	memcpy(m_cardData.szProject2, strTemp, strTemp.GetLength());
	//	m_EdtProject2.SetWindowTextA((LPCTSTR)strTemp);//项目名称
	//}

	////读30块
	//iBlock = 30;
	//memset(szBlockData, 0, 40);
	//if(rf.ReadMifareBlock(iBlock, szBlockData, rf.HEX))
	//{
	//	AfxMessageBox("读信息失败，请检查卡片！");
	//	return;
	//}
	////29块如果等于03 则表明有数据
	//if(!memcmp(szBlockData, "03", 2))
	//{
	//	//处理数据
	//	memset(szProjectTimes, 0, 5);
	//	memset(szProjectName, 0, 40);
	//	memcpy(szProjectTimes, szBlockData + 2, 2);
	//	m_cardData.iProjectTimes3 = rf.Hex2Dec(szProjectTimes);
	//	strTemp.Format("%d", m_cardData.iProjectTimes3);
	//	m_EdtProjectTimes3.SetWindowTextA(strTemp);//项目3 剩余次数

	//	iDataLen = 28;
	//	memcpy(szProjectName, szBlockData + 4, iDataLen);
	//	rf.SwitchDataMode(szProjectName, &iDataLen, rf.HEX2ASC);
	//	strTemp.Format("%s",szProjectName);
	//	strTemp.Remove(0x20);
	//	memcpy(m_cardData.szProject3, strTemp, strTemp.GetLength());
	//	m_EdtProject3.SetWindowTextA((LPCTSTR)strTemp);//项目名称
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


void CMifarePurseDlg::OnBnClickedBtnClose()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialog::OnCancel();
}
