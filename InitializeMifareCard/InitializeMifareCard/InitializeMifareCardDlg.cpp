
// InitializeMifareCardDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "InitializeMifareCard.h"
#include "InitializeMifareCardDlg.h"
#include "afxdialogex.h"
#include "ManageKey.h"
#include "DES.h"
#include "RFReader.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/////////////////////////////////Self Define

#define MAIN_KEY		"12345678900987654321ABCDEFFEDCBA"
#define DESTROY_DATA	"00000000000000000000000000000000"
#define DEFAULT_ACCESS	"FFFFFFFFFFFFFF078069FFFFFFFFFFFF"
#define		INI_FILE	"Set.dat"
#define		CONTROL_FIELD		"68778969"
#define		DEFAULT_KEY	"FFFFFFFFFFFF"

CRFReader rf;

////////////////////////////////////End of Self define


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


// CInitializeMifareCardDlg 对话框




CInitializeMifareCardDlg::CInitializeMifareCardDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CInitializeMifareCardDlg::IDD, pParent)
	, m_isConnected(FALSE)
	, m_CardCount(0)
	, m_isStoped(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CInitializeMifareCardDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_NAME, m_EdtName);
	DDX_Control(pDX, IDC_EDIT_CARDNO, m_EdtCardNO);
	DDX_Control(pDX, IDC_EDIT_PHONENO, m_EdtPhoneNO);
	DDX_Control(pDX, IDC_EDIT_AMOUNT, m_EdtAmount);
	DDX_Control(pDX, IDC_STATIC_INIT, m_StInit);
	DDX_Control(pDX, IDC_BTN_INIT, m_btnStartInit);
	DDX_Control(pDX, IDC_BTN_STOP, m_btnStopInit);
	DDX_Control(pDX, IDC_BTN_DESTROY, m_btnDestroy);
	DDX_Control(pDX, IDC_BTN_CLOSE, m_btnClose);
}

BEGIN_MESSAGE_MAP(CInitializeMifareCardDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_INIT, &CInitializeMifareCardDlg::OnBnClickedBtnInit)
	ON_BN_CLICKED(IDC_BUTTON1, &CInitializeMifareCardDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BTN_DESTROY, &CInitializeMifareCardDlg::OnBnClickedBtnDestroy)
	ON_BN_CLICKED(IDC_BTN_CLOSE, &CInitializeMifareCardDlg::OnBnClickedBtnClose)
	ON_BN_CLICKED(IDC_BTN_STOP, &CInitializeMifareCardDlg::OnBnClickedBtnStop)
END_MESSAGE_MAP()


// CInitializeMifareCardDlg 消息处理程序

BOOL CInitializeMifareCardDlg::OnInitDialog()
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

	//m_EdtName.LimitText(32);
	//m_EdtCardNO.LimitText(6);
	//m_EdtPhoneNO.LimitText(12);
	//m_EdtAmount.LimitText(12);

	//m_EdtCardNO.SetWindowTextA("000001");
	//m_EdtPhoneNO.SetWindowTextA("000000000000");
	//m_EdtAmount.SetWindowTextA("0");
	memset(m_KeyData, 0, 512);
	GetKeyData(m_KeyData);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CInitializeMifareCardDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CInitializeMifareCardDlg::OnPaint()
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
HCURSOR CInitializeMifareCardDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CInitializeMifareCardDlg::OnBnClickedBtnInit()
{
	// TODO: 在此添加控件通知处理程序代码
	CFile file;
	CString strTemp;
	CDES des;
	MSG msg;
	unsigned char szKeyData[40];
	//unsigned char szAmount[10], szCardData[128], szName[100], szPhoneNO[32], szCardNO[40];
	int iRet, iDataOffset, iBlock = 0;
	int /*iAmount,*/ iDataLen = 0/*, iCount*/;
	int isFinashed = 0;

	//连读写器
	if(!m_isConnected)
	{
		iRet = rf.Dev_Connect(100, 115200);
		if(iRet)
		{
			AfxMessageBox("连接读写器失败,请检查连接！");
			return;
		}
		m_isConnected = TRUE;
	}

	//disable buttons
	m_btnClose.EnableWindow(FALSE);
	m_btnStartInit.EnableWindow(FALSE);
	m_btnDestroy.EnableWindow(FALSE);
	m_btnStopInit.EnableWindow(TRUE);

	m_isStoped = 0;
	while(!m_isStoped)
	{
		if(::PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
        {  
            ::TranslateMessage(&msg);  
            ::DispatchMessage(&msg);  
        } 

		rf.DevReset(5);//射频头复位

		if(rf.Dev_Reset(1))
		{
			m_StInit.SetWindowTextA("卡片复位失败，请重新放卡!");
			Sleep(100);
			continue;
		}

		for(int i = 0; i != 64; ++i)
		{
			if((i%4) == 3)
			{
				iDataOffset = i/4 * 24;
				memset(szKeyData, 0, sizeof(szKeyData));
				memcpy(szKeyData, m_KeyData + iDataOffset, 12);
				memcpy(szKeyData + 12, CONTROL_FIELD, 8);
				iDataOffset += 12;
				memcpy(szKeyData + 20, m_KeyData + iDataOffset, 12);
		
				if(rf.LoadKey(rf.KEY_L, i/4, (unsigned char*)DEFAULT_KEY, rf.HEX))
				{
					strTemp.Format("扇区<%d>装载密钥失败，请检查卡片!", i/4);
					m_StInit.SetWindowTextA(strTemp);
					Sleep(100);
					isFinashed = 0;
					break;
				}
				if(rf.Anthentication(rf.KEY_L, i/4))
				{
					strTemp.Format("扇区<%d>认证密钥失败，请检查卡片!", i/4);
					m_StInit.SetWindowTextA(strTemp);
					Sleep(100);
					isFinashed = 0;
					break;
				}
				if(rf.Write(i, szKeyData, rf.HEX))
				{
					strTemp.Format("块<%d>写数据失败, 请放入下一张卡!", i);
					m_StInit.SetWindowTextA(strTemp);
					Sleep(100);
					isFinashed = 0;	
					break;
				}
			}
			rf.Dev_CardHalt();
			isFinashed = 1;
		}
		if(!isFinashed)
			continue;
		++m_CardCount;
		strTemp.Format("完成发卡：<%d>张", m_CardCount);
		m_StInit.SetWindowTextA(strTemp);
		rf.Dev_Beep(10);
		Sleep(500);
	}
	rf.Dev_Close();
	m_isConnected = false;
}

//manage key
void CInitializeMifareCardDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	CManageKey mk;

	mk.DoModal();
}

/////////销卡
void CInitializeMifareCardDlg::OnBnClickedBtnDestroy()
{
	// TODO: 在此添加控件通知处理程序代码
	int iRet/*, iBlock*/;
	unsigned char /*szSectorKeyData[40],*/ szCardKey[20];
	CString strTemp;

	//连读写器
	if(!m_isConnected)
	{
		iRet = rf.Dev_Connect(100, 115200);
		if(iRet)
		{
			AfxMessageBox("连接读写器失败,请检查连接！");
			return;
		}
		m_isConnected = TRUE;
	}

	memset(szCardKey, 0, 20);
	for(int i = 1; i != 64; ++i)
	{
		if(i == 1 || i%4 == 0)
		{
			memcpy(szCardKey, m_KeyData + i/4*24 + 12, 12);
			rf.DevReset(5);//射频头复位
			//rf.Dev_CardHalt();
			rf.Dev_Reset(1);
			//Sleep(50);
			if(rf.LoadKey(rf.KEY_R, i/4, szCardKey, rf.HEX))
			{
				strTemp.Format("扇区<%d>装载密钥失败，请检查卡片!", i/4);
				AfxMessageBox(strTemp);
				return;
			}
			//Sleep(50);
			if(rf.Anthentication(rf.KEY_R, i/4))
			{
				strTemp.Format("扇区<%d>认证密钥失败，请检查卡片!", i/4);
				AfxMessageBox(strTemp);
				return;
			}
		}
		
			//Sleep(100);
		if((i%4) == 3)
		{
			//memcpy(szSectorKeyData, m_KeyData + i/4*24, 12);
			//memcpy(szSectorKeyData + 12, CONTROL_FIELD, 8);
			//memcpy(szSectorKeyData + 20, m_KeyData + i/4*24 + 12, 12);

			if(rf.Write(i, (unsigned char*)DEFAULT_ACCESS, rf.HEX))
			{
				strTemp.Format("块<%d>写密钥失败!", i);
				AfxMessageBox(strTemp);
				return;
			}
		}
		else
		{
			if(rf.Write(i, (unsigned char*)DESTROY_DATA, rf.HEX))
			{
				strTemp.Format("块<%d>写数据失败!", i);
				AfxMessageBox(strTemp);
				return;
			}
		}
	}

	rf.Dev_Beep(15);
	AfxMessageBox("销卡成功！");

}


void CInitializeMifareCardDlg::OnBnClickedBtnClose()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialog::OnCancel();
}


int CInitializeMifareCardDlg::GetKeyData(unsigned char* pszKeyData)
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


void CInitializeMifareCardDlg::OnBnClickedBtnStop()
{
	// TODO: 在此添加控件通知处理程序代码
	m_isStoped = 1;

	//disable buttons
	m_btnClose.EnableWindow(TRUE);
	m_btnStartInit.EnableWindow(TRUE);
	m_btnDestroy.EnableWindow(TRUE);
	m_btnStopInit.EnableWindow(FALSE);

}
