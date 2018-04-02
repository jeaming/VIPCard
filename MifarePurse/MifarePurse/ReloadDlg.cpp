// ReloadDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MifarePurse.h"
#include "ReloadDlg.h"
#include "afxdialogex.h"
#include "RFReader.h"
#include "DES.h"

////////////////////////////////////////////
///////////////////////////////////////////////////SELF DEFINATION
CRFReader rf_load;

#define MAIN_KEY		"12345678900987654321ABCDEFFEDCBA"
#define		INI_FILE	"Set.dat"
#define		DEFAULT_KEY	"FFFFFFFFFFFF"

/////////////////////////////////////////////////////

// CReloadDlg 对话框

IMPLEMENT_DYNAMIC(CReloadDlg, CDialog)

CReloadDlg::CReloadDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CReloadDlg::IDD, pParent)
{

}

//CReloadDlg::CReloadDlg(unsigned char* pszCardNO, unsigned char* pszName, unsigned char* pszBalance, unsigned char* pszKeyData,
//	CWnd* pParent /*=NULL*/)
//	: CDialog(CReloadDlg::IDD, pParent)
//{
//	memcpy(m_szCardNO, pszCardNO, 6);
//	memcpy(m_szName,pszName, 30);
//	memcpy(m_szBalance, pszBalance, 16);
//	memcpy(m_KeyData, pszKeyData, strlen((char*)pszKeyData));
//}

CReloadDlg::~CReloadDlg()
{
	rf_load.Dev_Close();
}

void CReloadDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_CARDNO, m_EdtCardNO);
	DDX_Control(pDX, IDC_EDIT_MONEY, m_EdtBalance);
	DDX_Control(pDX, IDC_EDIT_NAME, m_EdtName);
	DDX_Control(pDX, IDC_EDIT_PHONENO, m_EdtPhoneNO);
	DDX_Control(pDX, IDC_EDIT_PUR, m_EdtPurMoney);
	DDX_Control(pDX, IDC_EDIT_PURTIME, m_EdtTime);
	DDX_Control(pDX, IDC_BTN_BEGIN, m_BtnBegin);
	DDX_Control(pDX, IDC_BTN_LOAD, m_BtnLoad);
}


BEGIN_MESSAGE_MAP(CReloadDlg, CDialog)
	ON_BN_CLICKED(IDC_BTN_CANCEL, &CReloadDlg::OnBnClickedBtnCancel)
	ON_BN_CLICKED(IDC_BTN_BEGIN, &CReloadDlg::OnBnClickedBtnBegin)
	ON_BN_CLICKED(IDC_BTN_LOAD, &CReloadDlg::OnBnClickedBtnLoad)
END_MESSAGE_MAP()


// CReloadDlg 消息处理程序


void CReloadDlg::OnBnClickedBtnCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialog::OnCancel();
}



void CReloadDlg::OnBnClickedBtnBegin()
{
	// TODO: 在此添加控件通知处理程序代码

	int iRet = 0, iBlock;
	CString strTemp;
	unsigned char szSectorKey[40], szBlockData[40];
	unsigned long lMoney = 0;
	
	//设备操作机及密钥认证
	//连接读写器
	if(!rf_load.m_isConnected)
	{
		if(rf_load.Dev_Connect(100, 115200))
		{
			AfxMessageBox("连接读写器失败，请检查设备！");
			return;
		}
	}

	//卡片复位
	if((iRet = rf_load.Dev_Reset(1)))
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
	if(rf_load.LoadKey(rf_load.KEY_L, iBlock/4, szSectorKey, rf_load.HEX))
	{
		AfxMessageBox("认证密钥失败！");
		return;
	}

	if(rf_load.Anthentication(rf_load.KEY_L, iBlock/4))
	{
		AfxMessageBox("认证密钥失败2！");
		return;
	}
	//认证密钥完成

	if(rf_load.ReadValue(iBlock, &lMoney))
	{
		AfxMessageBox("取余额失败！");
		return;
	}

	strTemp.Format("%d", lMoney);
	m_EdtBalance.SetWindowTextA((LPCTSTR)strTemp);//显示余额

	//读姓名
	memset(szBlockData, 0, 40);
	memset(m_szName, 0, 60);
	iBlock = 21;
	memset(szBlockData, 0, 40);
	if(rf_load.ReadMifareBlock(iBlock, szBlockData, rf_load.HEX))
	{
		AfxMessageBox("读姓名失败！请检查卡片！");
		return;
	}
	rf_load.Hex2Asc(szBlockData, m_szName, 32);
	m_szName[16] = 0;
	strTemp.Format("%s", m_szName);
	strTemp.Remove(0x20);
	m_EdtName.SetWindowTextA((LPCTSTR)strTemp);

	//读联系方式\卡号信息
	iBlock = 22;
	memset(szBlockData, 0, 40);
	if(rf_load.ReadMifareBlock(iBlock, szBlockData, rf_load.HEX))
	{
		AfxMessageBox("读卡号失败！请检查卡片！");
		return;
	}
	//联系电话
	memset(m_szPhoneNO, 0, 40);
	memcpy(m_szPhoneNO, szBlockData, 12);
	m_EdtPhoneNO.SetWindowTextA((LPCTSTR)m_szPhoneNO);

	//卡号
	memset(m_szCardNO, 0, 40);
	memcpy(m_szCardNO, szBlockData + 12, 6);
	m_EdtCardNO.SetWindowTextA((LPCTSTR)m_szCardNO);


	m_BtnLoad.EnableWindow(true);
	m_BtnBegin.EnableWindow(false);
}


void CReloadDlg::OnBnClickedBtnLoad()
{
	// TODO: 在此添加控件通知处理程序代码
	int iRet = 0, iBlock, iCount;
	CString strTemp;
	unsigned char szSectorKey[40], szCardData[128];
	unsigned char szAmount[10], szName[100], szPhoneNO[32], szCardNO[40];
	unsigned long lMoney = 0;
	int iAmount, iDataLen = 0, iDataOffset = 0;


	//组织初始化数据
	//初始化卡片金额
	memset(szCardData, 0, 128);
	memset(szAmount, 0, 10);
	memset(szPhoneNO, 0, 32);
	memset(szName, 0, 100);
	memset(szCardNO, 0, 40);

	m_EdtName.GetWindowTextA((LPTSTR)szName, 64);
	m_EdtCardNO.GetWindowTextA((LPTSTR)szCardNO, 40);
	m_EdtPhoneNO.GetWindowTextA((LPTSTR)szPhoneNO, 32);
	m_EdtBalance.GetWindowTextA((LPTSTR)szAmount, 20);

	iDataLen = strlen((char*)szCardNO);
	if(iDataLen > 6 || iDataLen <= 0)
	{//卡号过长
		AfxMessageBox("卡号过长或过短，请检查！");
		return;		
	}
	for(int i=0; i != iDataLen; ++i)
	{
		if(szCardNO[i] > 0x39 || szCardNO[i] < 0x30)
		{
			AfxMessageBox("卡号不能为数字0-9以外数据！");
			return;
		}
	}

	iDataLen = strlen((char*)szPhoneNO);
	if(iDataLen > 12 || iDataLen < 8)
	{//联系电话过长
		AfxMessageBox("联系电话过长或过短，请检查！");
		return;		
	};
	for(int i=0; i != iDataLen; ++i)
	{
		if(szPhoneNO[i] > 0x39 || szPhoneNO[i] < 0x30)
		{
			AfxMessageBox("联系电话不能为数字0-9以外数据！");
			return;
		}
	}

	iDataLen = strlen((char*)szAmount);
	if(iDataLen > 12 || iDataLen <= 0)
	{//金额不正确
		AfxMessageBox("金额不能空，请检查！");
		return;		
	};
	for(int i=0; i != iDataLen;  ++i)
	{
		if(szAmount[i] > 0x39 || szAmount[i] < 0x30)
		{
			AfxMessageBox("金额不能为数字0-9以外数据！");
			return;
		}
	}

	//设备操作机及密钥认证
	//连接读写器
	if(!rf_load.m_isConnected)
	{
		if(rf_load.Dev_Connect(100, 115200))
		{
			AfxMessageBox("连接读写器失败，请检查设备！");
			return;
		}
	}

	//认证5扇区
	rf_load.Dev_Reset(1);
	iBlock = 21;
	iDataOffset = iBlock/4 * 24 + 12;//sector 5, key right
	memset(szSectorKey, 0, 40);
	memcpy(szSectorKey, m_KeyData+ iDataOffset, 12);
	if(rf_load.LoadKey(rf_load.KEY_R, iBlock/4, szSectorKey, rf_load.HEX))
	{
		strTemp.Format("扇区<%d>装载密钥失败，请检查卡片!", iBlock/4);
		AfxMessageBox(strTemp);
		return;
	}
	if(rf_load.Anthentication(rf_load.KEY_R, iBlock/4))
	{
		strTemp.Format("扇区<%d>认证密钥失败，请检查卡片!", iBlock/4);
		AfxMessageBox(strTemp);
		return;
	}
	//////////////////////////////////////////初始金额
	iBlock = 20;//5扇区0块
	iAmount = atoi((char*)szAmount);
	if((iRet = rf_load.Increment(iBlock, iAmount)))
	{
		strTemp.Format("初始化金额失败，退出操作! 返回值：%d ", iRet);
		AfxMessageBox(strTemp);
		return;
	}

	//////////////////////////////////////////名字
	iBlock = 21;//1块 名字16Byte
	iDataLen = strlen((char*)szName);
	memcpy(szCardData, szName, iDataLen);
	rf_load.SwitchDataMode(szCardData, &iDataLen, rf_load.ASC2HEX);
	if(iDataLen /2 > 16)
	{
		AfxMessageBox("名字过长，请检查数据！");
		return;		
	}
	while(iDataLen/2 != 17)
	{
		memcpy(szCardData + iDataLen, "20", 2);
		iDataLen += 2;
	}
	szCardData[32] = 0;
	if((iRet = rf_load.Write(iBlock, szCardData, rf_load.HEX)))
	{
		strTemp.Format("写名字信息失败，退出操作! 返回值：%d ", iRet);
		AfxMessageBox(strTemp);
		return;
	}

	//////////////////////////////////////////卡号、联系电话
	memset(szCardData, 0x30, 100);
	iBlock = 22;
	iDataLen = strlen((char*)szPhoneNO);
	iCount = 0;
	while(iDataLen != 12)
	{
		szCardData[iCount] = 0x30;
		iDataLen ++;
		iCount++;
	}
	memcpy(szCardData + iCount, szPhoneNO, 12 - iCount);

	iDataLen = strlen((char*)szCardNO);
	iCount = 12;
	while(iDataLen != 6)
	{
		szCardData[iCount] = 0x30;
		iDataLen ++;
		iCount ++;
	}
	memcpy(szCardData + iCount, szCardNO, 18 - iCount);
	szCardData[32] = 0;
	if((iRet = rf_load.Write(iBlock, szCardData, rf_load.HEX)))
	{
		strTemp.Format("写卡号信息失败，退出操作! 返回值：%d ", iRet);
		AfxMessageBox(strTemp);
		return;
	}
	//AfxMessageBox("初始化卡片完成！");
	int iCardNO = 0;

	iCardNO = atoi((char*)szCardNO);
	++iCardNO;
	//strTemp.Format("%d", iCardNO);
	_itoa(iCardNO, (char*)szCardNO, 1000);
	m_EdtCardNO.SetWindowTextA((LPCTSTR)szCardNO);

	rf_load.Dev_Beep(2);


	m_BtnLoad.EnableWindow(false);
	m_BtnBegin.EnableWindow(true);
}


int CReloadDlg::GetKeyData(unsigned char* pszKeyData)
{	
	CFile file;
	CString strTemp;
	CDES des;
	unsigned char szDesKey[40], szCardKey[512], szKeyData[512];
	//unsigned char szCardData[128];
	int iKeyLen;
	int iDataLen = 0;

	memset(szKeyData, 0, 512);
	file.Open(INI_FILE, CFile::modeRead);
	iKeyLen = file.Read(szKeyData, 400);
	if(iKeyLen < 0)
	{
		return -1;
	}
	file.Close();

	//得到扇区密钥
	memset(szDesKey, 0, 40);
	memset(szCardKey, 0, 512);
	rf_load.Hex2Asc((unsigned char*)MAIN_KEY, szDesKey, 32);
	des.RunDes(des.DECRYPT, des.ECB, (char*)szKeyData, (char*)szCardKey, iKeyLen, (char*)szDesKey, 16);
	rf_load.Asc2Hex(szCardKey, pszKeyData, iKeyLen);
	iKeyLen = iKeyLen*2;

	pszKeyData[iKeyLen] = 0;

	return 0;
}


BOOL CReloadDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化

	memset(m_KeyData, 0, sizeof(m_KeyData));
	if(GetKeyData(m_KeyData))
	{
		AfxMessageBox("初始化配置文件失败！");
		exit(1);
	}


	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
