// ManageKey.cpp : 实现文件
//

#include "stdafx.h"
#include "InitializeMifareCard.h"
#include "ManageKey.h"
#include "afxdialogex.h"
#include "DES.h"
#include "RFReader.h"



/////////////////////////////////Self Define

#define MAIN_KEY		"12345678900987654321ABCDEFFEDCBA"
#define		INI_FILE	"Set.dat"



////////////////////////////////////End of Self define



// CManageKey 对话框

IMPLEMENT_DYNAMIC(CManageKey, CDialog)

CManageKey::CManageKey(CWnd* pParent /*=NULL*/)
	: CDialog(CManageKey::IDD, pParent)
	, m_strSector1L(_T(""))
	, m_strSector1R(_T(""))
	, m_strSector2L(_T(""))
	, m_strSector2R(_T(""))
	, m_strSector3L(_T(""))
	, m_strSector3R(_T(""))
	, m_strSector4L(_T(""))
	, m_strSector4R(_T(""))
	, m_strSector5L(_T(""))
	, m_strSector5R(_T(""))
	, m_strSector6L(_T(""))
	, m_strSector6R(_T(""))
	, m_strSector7L(_T(""))
	, m_strSector7R(_T(""))
	, m_strSector8L(_T(""))
	, m_strSector8R(_T(""))
	, m_strSector9L(_T(""))
	, m_strSector9R(_T(""))
	, m_strSector10L(_T(""))
	, m_strSector10R(_T(""))
	, m_strSector11L(_T(""))
	, m_strSector11R(_T(""))
	, m_strSector12L(_T(""))
	, m_strSector12R(_T(""))
	, m_strSector13L(_T(""))
	, m_strSector13R(_T(""))
	, m_strSector14L(_T(""))
	, m_strSector14R(_T(""))
	, m_strSector15L(_T(""))
	, m_strSector15R(_T(""))
	, m_strSector16L(_T(""))
	, m_strSector16R(_T(""))
	, m_strKeyData(_T(""))
{

	m_strLoadPIN = _T("");
	m_strPursePIN = _T("");
}

CManageKey::~CManageKey()
{
}

void CManageKey::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_EdtSectorName);
	DDX_Text(pDX, IDC_EDIT19, m_strSector1L);
	DDX_Text(pDX, IDC_EDIT36, m_strSector1R);
	DDX_Text(pDX, IDC_EDIT20, m_strSector2L);
	DDX_Text(pDX, IDC_EDIT37, m_strSector2R);
	DDX_Text(pDX, IDC_EDIT21, m_strSector3L);
	DDX_Text(pDX, IDC_EDIT38, m_strSector3R);
	DDX_Text(pDX, IDC_EDIT22, m_strSector4L);
	DDX_Text(pDX, IDC_EDIT39, m_strSector4R);
	DDX_Text(pDX, IDC_EDIT23, m_strSector5L);
	DDX_Text(pDX, IDC_EDIT40, m_strSector5R);
	DDX_Text(pDX, IDC_EDIT24, m_strSector6L);
	DDX_Text(pDX, IDC_EDIT41, m_strSector6R);
	DDX_Text(pDX, IDC_EDIT25, m_strSector7L);
	DDX_Text(pDX, IDC_EDIT42, m_strSector7R);
	DDX_Text(pDX, IDC_EDIT26, m_strSector8L);
	DDX_Text(pDX, IDC_EDIT43, m_strSector8R);
	DDX_Text(pDX, IDC_EDIT27, m_strSector9L);
	DDX_Text(pDX, IDC_EDIT44, m_strSector9R);
	DDX_Text(pDX, IDC_EDIT28, m_strSector10L);
	DDX_Text(pDX, IDC_EDIT45, m_strSector10R);
	DDX_Text(pDX, IDC_EDIT29, m_strSector11L);
	DDX_Text(pDX, IDC_EDIT46, m_strSector11R);
	DDX_Text(pDX, IDC_EDIT30, m_strSector12L);
	DDX_Text(pDX, IDC_EDIT47, m_strSector12R);
	DDX_Text(pDX, IDC_EDIT31, m_strSector13L);
	DDX_Text(pDX, IDC_EDIT48, m_strSector13R);
	DDX_Text(pDX, IDC_EDIT32, m_strSector14L);
	DDX_Text(pDX, IDC_EDIT49, m_strSector14R);
	DDX_Text(pDX, IDC_EDIT33, m_strSector15L);
	DDX_Text(pDX, IDC_EDIT50, m_strSector15R);
	DDX_Text(pDX, IDC_EDIT34, m_strSector16L);
	DDX_Text(pDX, IDC_EDIT51, m_strSector16R);
	DDX_Control(pDX, IDC_EDIT_PURSEPIN, m_EdtPursePIN);
	DDX_Control(pDX, IDC_EDIT_LOADPIN, m_EdtLoadPIN);
	DDX_Text(pDX, IDC_EDIT_LOADPIN, m_strLoadPIN);
	DDX_Text(pDX, IDC_EDIT_PURSEPIN, m_strPursePIN);
}


BEGIN_MESSAGE_MAP(CManageKey, CDialog)
	ON_BN_CLICKED(IDC_BUTTON1, &CManageKey::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CManageKey::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BTN_GETKEY, &CManageKey::OnBnClickedBtnGetkey)
END_MESSAGE_MAP()


// CManageKey 消息处理程序


BOOL CManageKey::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	
	((CEdit*)(GetDlgItem(IDC_EDIT1)))->SetWindowTextA("扇      区");
	((CEdit*)(GetDlgItem(IDC_EDIT2)))->SetWindowTextA("扇 区 一");
	((CEdit*)(GetDlgItem(IDC_EDIT3)))->SetWindowTextA("扇 区 二");
	((CEdit*)(GetDlgItem(IDC_EDIT4)))->SetWindowTextA("扇 区 三");
	((CEdit*)(GetDlgItem(IDC_EDIT5)))->SetWindowTextA("扇 区 四");
	((CEdit*)(GetDlgItem(IDC_EDIT6)))->SetWindowTextA("扇 区 五");
	((CEdit*)(GetDlgItem(IDC_EDIT7)))->SetWindowTextA("扇 区 六");
	((CEdit*)(GetDlgItem(IDC_EDIT8)))->SetWindowTextA("扇 区 七");
	((CEdit*)(GetDlgItem(IDC_EDIT9)))->SetWindowTextA("扇 区 八");
	((CEdit*)(GetDlgItem(IDC_EDIT10)))->SetWindowTextA("扇 区 九");
	((CEdit*)(GetDlgItem(IDC_EDIT11)))->SetWindowTextA("扇 区 十");
	((CEdit*)(GetDlgItem(IDC_EDIT12)))->SetWindowTextA("扇区十一");
	((CEdit*)(GetDlgItem(IDC_EDIT13)))->SetWindowTextA("扇区十二");
	((CEdit*)(GetDlgItem(IDC_EDIT14)))->SetWindowTextA("扇区十三");
	((CEdit*)(GetDlgItem(IDC_EDIT15)))->SetWindowTextA("扇区十四");
	((CEdit*)(GetDlgItem(IDC_EDIT16)))->SetWindowTextA("扇区十五");
	((CEdit*)(GetDlgItem(IDC_EDIT17)))->SetWindowTextA("扇区十六");
	((CEdit*)(GetDlgItem(IDC_EDIT18)))->SetWindowTextA("左密钥");
	((CEdit*)(GetDlgItem(IDC_EDIT35)))->SetWindowTextA("右密钥");

	((CEdit*)(GetDlgItem(IDC_EDIT19)))->SetWindowTextA("FFFFFFFFFFFF");
	((CEdit*)(GetDlgItem(IDC_EDIT20)))->SetWindowTextA("FFFFFFFFFFFF");
	((CEdit*)(GetDlgItem(IDC_EDIT21)))->SetWindowTextA("FFFFFFFFFFFF");
	((CEdit*)(GetDlgItem(IDC_EDIT22)))->SetWindowTextA("FFFFFFFFFFFF");
	((CEdit*)(GetDlgItem(IDC_EDIT23)))->SetWindowTextA("FFFFFFFFFFFF");
	((CEdit*)(GetDlgItem(IDC_EDIT24)))->SetWindowTextA("FFFFFFFFFFFF");
	((CEdit*)(GetDlgItem(IDC_EDIT25)))->SetWindowTextA("FFFFFFFFFFFF");
	((CEdit*)(GetDlgItem(IDC_EDIT26)))->SetWindowTextA("FFFFFFFFFFFF");
	((CEdit*)(GetDlgItem(IDC_EDIT27)))->SetWindowTextA("FFFFFFFFFFFF");
	((CEdit*)(GetDlgItem(IDC_EDIT28)))->SetWindowTextA("FFFFFFFFFFFF");
	((CEdit*)(GetDlgItem(IDC_EDIT29)))->SetWindowTextA("FFFFFFFFFFFF");
	((CEdit*)(GetDlgItem(IDC_EDIT30)))->SetWindowTextA("FFFFFFFFFFFF");
	((CEdit*)(GetDlgItem(IDC_EDIT31)))->SetWindowTextA("FFFFFFFFFFFF");
	((CEdit*)(GetDlgItem(IDC_EDIT32)))->SetWindowTextA("FFFFFFFFFFFF");
	((CEdit*)(GetDlgItem(IDC_EDIT33)))->SetWindowTextA("FFFFFFFFFFFF");
	((CEdit*)(GetDlgItem(IDC_EDIT34)))->SetWindowTextA("FFFFFFFFFFFF");
	((CEdit*)(GetDlgItem(IDC_EDIT36)))->SetWindowTextA("FFFFFFFFFFFF");
	((CEdit*)(GetDlgItem(IDC_EDIT37)))->SetWindowTextA("FFFFFFFFFFFF");
	((CEdit*)(GetDlgItem(IDC_EDIT38)))->SetWindowTextA("FFFFFFFFFFFF");
	((CEdit*)(GetDlgItem(IDC_EDIT39)))->SetWindowTextA("FFFFFFFFFFFF");
	((CEdit*)(GetDlgItem(IDC_EDIT40)))->SetWindowTextA("FFFFFFFFFFFF");
	((CEdit*)(GetDlgItem(IDC_EDIT41)))->SetWindowTextA("FFFFFFFFFFFF");
	((CEdit*)(GetDlgItem(IDC_EDIT42)))->SetWindowTextA("FFFFFFFFFFFF");
	((CEdit*)(GetDlgItem(IDC_EDIT43)))->SetWindowTextA("FFFFFFFFFFFF");
	((CEdit*)(GetDlgItem(IDC_EDIT44)))->SetWindowTextA("FFFFFFFFFFFF");
	((CEdit*)(GetDlgItem(IDC_EDIT45)))->SetWindowTextA("FFFFFFFFFFFF");
	((CEdit*)(GetDlgItem(IDC_EDIT46)))->SetWindowTextA("FFFFFFFFFFFF");
	((CEdit*)(GetDlgItem(IDC_EDIT47)))->SetWindowTextA("FFFFFFFFFFFF");
	((CEdit*)(GetDlgItem(IDC_EDIT48)))->SetWindowTextA("FFFFFFFFFFFF");
	((CEdit*)(GetDlgItem(IDC_EDIT49)))->SetWindowTextA("FFFFFFFFFFFF");
	((CEdit*)(GetDlgItem(IDC_EDIT50)))->SetWindowTextA("FFFFFFFFFFFF");
	((CEdit*)(GetDlgItem(IDC_EDIT51)))->SetWindowTextA("FFFFFFFFFFFF");
	
	((CEdit*)(GetDlgItem(IDC_EDIT19)))->LimitText(12);
	((CEdit*)(GetDlgItem(IDC_EDIT20)))->LimitText(12);
	((CEdit*)(GetDlgItem(IDC_EDIT21)))->LimitText(12);
	((CEdit*)(GetDlgItem(IDC_EDIT22)))->LimitText(12);
	((CEdit*)(GetDlgItem(IDC_EDIT23)))->LimitText(12);
	((CEdit*)(GetDlgItem(IDC_EDIT24)))->LimitText(12);
	((CEdit*)(GetDlgItem(IDC_EDIT25)))->LimitText(12);
	((CEdit*)(GetDlgItem(IDC_EDIT26)))->LimitText(12);
	((CEdit*)(GetDlgItem(IDC_EDIT27)))->LimitText(12);
	((CEdit*)(GetDlgItem(IDC_EDIT28)))->LimitText(12);
	((CEdit*)(GetDlgItem(IDC_EDIT29)))->LimitText(12);
	((CEdit*)(GetDlgItem(IDC_EDIT30)))->LimitText(12);
	((CEdit*)(GetDlgItem(IDC_EDIT31)))->LimitText(12);
	((CEdit*)(GetDlgItem(IDC_EDIT32)))->LimitText(12);
	((CEdit*)(GetDlgItem(IDC_EDIT33)))->LimitText(12);
	((CEdit*)(GetDlgItem(IDC_EDIT34)))->LimitText(12);
	((CEdit*)(GetDlgItem(IDC_EDIT36)))->LimitText(12);
	((CEdit*)(GetDlgItem(IDC_EDIT37)))->LimitText(12);
	((CEdit*)(GetDlgItem(IDC_EDIT38)))->LimitText(12);
	((CEdit*)(GetDlgItem(IDC_EDIT39)))->LimitText(12);
	((CEdit*)(GetDlgItem(IDC_EDIT40)))->LimitText(12);
	((CEdit*)(GetDlgItem(IDC_EDIT41)))->LimitText(12);
	((CEdit*)(GetDlgItem(IDC_EDIT42)))->LimitText(12);
	((CEdit*)(GetDlgItem(IDC_EDIT43)))->LimitText(12);
	((CEdit*)(GetDlgItem(IDC_EDIT44)))->LimitText(12);
	((CEdit*)(GetDlgItem(IDC_EDIT45)))->LimitText(12);
	((CEdit*)(GetDlgItem(IDC_EDIT46)))->LimitText(12);
	((CEdit*)(GetDlgItem(IDC_EDIT47)))->LimitText(12);
	((CEdit*)(GetDlgItem(IDC_EDIT48)))->LimitText(12);
	((CEdit*)(GetDlgItem(IDC_EDIT49)))->LimitText(12);
	((CEdit*)(GetDlgItem(IDC_EDIT50)))->LimitText(12);
	((CEdit*)(GetDlgItem(IDC_EDIT51)))->LimitText(12);

	m_EdtLoadPIN.SetWindowTextA("123456");
	m_EdtPursePIN.SetWindowTextA("123456");

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

//应用
void CManageKey::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	CRFReader rf;
	CDES des;
	CString strTemp;
	CFile file;
	unsigned char szKeyData[512], szDataOut[512];
	char szDesKey[40];
	int iLen, iKeyLen;
	
	if(CheckKey() != 0)
	{
		AfxMessageBox("密钥数据或长度错误，请检查！");
		return;
	}

	//密钥转换成ASC
	memset(szKeyData, 0, 512);
	iLen = m_strKeyData.GetLength();
	while(iLen%16)
	{
		m_strKeyData.Append("F");
		++iLen;
	}
	sprintf_s((char*)szKeyData, 512, "%s", m_strKeyData);
	rf.SwitchDataMode(szKeyData, &iLen, rf.HEX2ASC);

	//加密密钥转换成ASC
	memset(szDesKey, 0, 40);
	iKeyLen = 32;
	memcpy(szDesKey, MAIN_KEY, iKeyLen);
	rf.SwitchDataMode((unsigned char*)szDesKey, &iKeyLen, rf.HEX2ASC);

	//加密
	memset(szDataOut, 0, 512);
	des.RunDes(des.ENCRYPT, des.ECB, (char*)szKeyData, (char*)szDataOut, iLen, szDesKey, iKeyLen);

	//准备写入
	strTemp.Format((char*)szDataOut);

	//文件处理
	if(!file.Open((LPCTSTR)INI_FILE, CFile::modeReadWrite | CFile::modeCreate))
	{
		AfxMessageBox("打开配置文件失败！");
		return;
	}

	file.Write(szDataOut, iLen);

	file.Close();
	AfxMessageBox("密钥配置完成！");

	return;

}

int CManageKey::CheckKey(void)
{
	UpdateData(true);
	m_strKeyData = "";

	//1
	if(m_strSector1L.GetLength() == 0)
		return -1;

	m_strKeyData += m_strSector1L;

	if(m_strSector1R.GetLength() == 0)
		return -1;

	m_strKeyData += m_strSector1R;

	//2
	if(m_strSector2L.GetLength() == 0)
		return -1;

	m_strKeyData += m_strSector2L;

	if(m_strSector2R.GetLength() == 0)
		return -1;

	m_strKeyData += m_strSector2R;
	
	//3
	if(m_strSector3L.GetLength() == 0)
		return -1;

	m_strKeyData += m_strSector3L;

	if(m_strSector3R.GetLength() == 0)
		return -1;

	m_strKeyData += m_strSector3R;
	
	//4
	if(m_strSector4L.GetLength() == 0)
		return -1;

	m_strKeyData += m_strSector4L;

	if(m_strSector4R.GetLength() == 0)
		return -1;

	m_strKeyData += m_strSector4R;
	
	//5
	if(m_strSector5L.GetLength() == 0)
		return -1;

	m_strKeyData += m_strSector5L;

	if(m_strSector5R.GetLength() == 0)
		return -1;

	m_strKeyData += m_strSector5R;

	
	//6
	if(m_strSector6L.GetLength() == 0)
		return -1;

	m_strKeyData += m_strSector6L;

	if(m_strSector6R.GetLength() == 0)
		return -1;

	m_strKeyData += m_strSector6R;

	
	//7
	if(m_strSector7L.GetLength() == 0)
		return -1;

	m_strKeyData += m_strSector7L;

	if(m_strSector7R.GetLength() == 0)
		return -1;

	m_strKeyData += m_strSector7R;

	
	//8
	if(m_strSector8L.GetLength() == 0)
		return -1;

	m_strKeyData += m_strSector8L;

	if(m_strSector8R.GetLength() == 0)
		return -1;

	m_strKeyData += m_strSector8R;

	
	//9
	if(m_strSector9L.GetLength() == 0)
		return -1;

	m_strKeyData += m_strSector9L;

	if(m_strSector9R.GetLength() == 0)
		return -1;

	m_strKeyData += m_strSector9R;

	
	//10
	if(m_strSector10L.GetLength() == 0)
		return -1;

	m_strKeyData += m_strSector10L;

	if(m_strSector10R.GetLength() == 0)
		return -1;

	m_strKeyData += m_strSector10R;

	
	//11
	if(m_strSector11L.GetLength() == 0)
		return -1;

	m_strKeyData += m_strSector11L;

	if(m_strSector11R.GetLength() == 0)
		return -1;

	m_strKeyData += m_strSector11R;

	
	//12
	if(m_strSector12L.GetLength() == 0)
		return -1;

	m_strKeyData += m_strSector12L;

	if(m_strSector12R.GetLength() == 0)
		return -1;

	m_strKeyData += m_strSector12R;

	
	//13
	if(m_strSector13L.GetLength() == 0)
		return -1;

	m_strKeyData += m_strSector13L;

	if(m_strSector13R.GetLength() == 0)
		return -1;

	m_strKeyData += m_strSector13R;

	
	//14
	if(m_strSector14L.GetLength() == 0)
		return -1;

	m_strKeyData += m_strSector14L;

	if(m_strSector14R.GetLength() == 0)
		return -1;

	m_strKeyData += m_strSector14R;

	
	//15
	if(m_strSector15L.GetLength() == 0)
		return -1;

	m_strKeyData += m_strSector15L;

	if(m_strSector15R.GetLength() == 0)
		return -1;

	m_strKeyData += m_strSector15R;

	
	//16
	if(m_strSector16L.GetLength() == 0)
		return -1;

	m_strKeyData += m_strSector16L;

	if(m_strSector16R.GetLength() == 0)
		return -1;

	m_strKeyData += m_strSector16R;

	CString strTemp;

	if(m_strKeyData.GetLength() != 384)
		return -1;

	if(m_strLoadPIN.GetLength() == 0)
		return -1;

	m_strKeyData.AppendFormat("%02d", m_strLoadPIN.GetLength());
	m_strKeyData += m_strLoadPIN;
	if(m_strPursePIN.GetLength() == 0)
		return -1;

	m_strKeyData.AppendFormat("%02d", m_strPursePIN.GetLength());
	m_strKeyData += m_strPursePIN;

	////数据
	//unsigned char szData[512];
	//memset(szData, 0, 512);
	//sprintf_s((char*)szData, 512, "%s", m_strKeyData);
	//for(int i = 0; i != 384; ++i)
	//{
	//	if((m_strKeyData[i] < 0x30 || m_strKeyData[i] > 0x45) &&
	//		(m_strKeyData[i] < 0x41 || m_strKeyData[i] > 0x39))
	//		return -1;
	//}

	UpdateData(false);
	return 0;
}

//确定
void CManageKey::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	CRFReader rf;
	CDES des;
	CString strTemp;
	CFile file;
	unsigned char szKeyData[512], szDataOut[512];
	char szDesKey[40];
	int iLen, iKeyLen;
	
	if(CheckKey() != 0)
	{
		AfxMessageBox("密钥数据或长度错误，请检查！");
		return;
	}

	//密钥转换成ASC
	memset(szKeyData, 0, 512);
	iLen = m_strKeyData.GetLength();
	while(iLen%16)
	{
		m_strKeyData.Append("F");
		iLen ++;
	}
	sprintf_s((char*)szKeyData, 512, "%s", m_strKeyData);
	rf.SwitchDataMode(szKeyData, &iLen, rf.HEX2ASC);

	//加密密钥转换成ASC
	memset(szDesKey, 0, 40);
	iKeyLen = 32;
	memcpy(szDesKey, MAIN_KEY, iKeyLen);
	rf.SwitchDataMode((unsigned char*)szDesKey, &iKeyLen, rf.HEX2ASC);

	//加密
	memset(szDataOut, 0, 512);
	des.RunDes(des.ENCRYPT, des.ECB, (char*)szKeyData, (char*)szDataOut, iLen, szDesKey, iKeyLen);

	//准备写入
	strTemp.Format((char*)szDataOut);

	//文件处理
	if(!file.Open((LPCTSTR)INI_FILE, CFile::modeReadWrite | CFile::modeCreate))
	{
		AfxMessageBox("打开配置文件失败！");
		return;
	}

	file.Write(szDataOut, iLen);

	file.Close();

	this->OnOK();
}


void CManageKey::OnBnClickedBtnGetkey()
{
	// TODO: 在此添加控件通知处理程序代码

	UpdateData(true);
	memset(m_KeyData, 0, 512);
	int iOffSet = 0, iLen = 0;
	unsigned char szPINLen[10];
	unsigned char szLoadPIN[32];
	unsigned char szPursePIN[32];

	GetKeyData(m_KeyData);

	m_strSector1L.Format("%.12s", m_KeyData + iOffSet);
	m_strSector1R.Format("%.12s", m_KeyData + iOffSet + 12);

	iOffSet += 24;

	m_strSector2L.Format("%.12s", m_KeyData + iOffSet);
	m_strSector2R.Format("%.12s", m_KeyData + iOffSet + 12);

	iOffSet += 24;
	
	m_strSector3L.Format("%.12s", m_KeyData + iOffSet);
	m_strSector3R.Format("%.12s", m_KeyData + iOffSet + 12);

	iOffSet += 24;
	
	m_strSector4L.Format("%.12s", m_KeyData + iOffSet);
	m_strSector4R.Format("%.12s", m_KeyData + iOffSet + 12);

	iOffSet += 24;
	
	m_strSector5L.Format("%.12s", m_KeyData + iOffSet);
	m_strSector5R.Format("%.12s", m_KeyData + iOffSet + 12);

	iOffSet += 24;
	
	m_strSector6L.Format("%.12s", m_KeyData + iOffSet);
	m_strSector6R.Format("%.12s", m_KeyData + iOffSet + 12);

	iOffSet += 24;
	
	m_strSector7L.Format("%.12s", m_KeyData + iOffSet);
	m_strSector7R.Format("%.12s", m_KeyData + iOffSet + 12);

	iOffSet += 24;
	
	m_strSector8L.Format("%.12s", m_KeyData + iOffSet);
	m_strSector8R.Format("%.12s", m_KeyData + iOffSet + 12);

	iOffSet += 24;
	
	m_strSector9L.Format("%.12s", m_KeyData + iOffSet);
	m_strSector9R.Format("%.12s", m_KeyData + iOffSet + 12);

	iOffSet += 24;
	
	m_strSector10L.Format("%.12s", m_KeyData + iOffSet);
	m_strSector10R.Format("%.12s", m_KeyData + iOffSet + 12);

	iOffSet += 24;
	
	m_strSector11L.Format("%.12s", m_KeyData + iOffSet);
	m_strSector11R.Format("%.12s", m_KeyData + iOffSet + 12);

	iOffSet += 24;
	
	m_strSector12L.Format("%.12s", m_KeyData + iOffSet);
	m_strSector12R.Format("%.12s", m_KeyData + iOffSet + 12);

	iOffSet += 24;
	
	m_strSector13L.Format("%.12s", m_KeyData + iOffSet);
	m_strSector13R.Format("%.12s", m_KeyData + iOffSet + 12);

	iOffSet += 24;
	
	m_strSector14L.Format("%.12s", m_KeyData + iOffSet);
	m_strSector14R.Format("%.12s", m_KeyData + iOffSet + 12);

	iOffSet += 24;
	
	m_strSector15L.Format("%.12s", m_KeyData + iOffSet);
	m_strSector15R.Format("%.12s", m_KeyData + iOffSet + 12);

	iOffSet += 24;
	
	m_strSector16L.Format("%.12s", m_KeyData + iOffSet);
	m_strSector16R.Format("%.12s", m_KeyData + iOffSet + 12);

	iOffSet += 24;

	memset(szPINLen, 0, 10);
	memcpy(szPINLen, m_KeyData + iOffSet, 2);

	iOffSet += 2;

	//Load PIN
	memset(szLoadPIN, 0, 32);
	iLen = atoi((char*)szPINLen);
	if(iLen > 32)
	{
		iLen = 30;
		AfxMessageBox("充值程序登入PIN长度错误，请查证");
	}
	memcpy(szLoadPIN, m_KeyData + iOffSet, iLen);
	iOffSet += iLen;
	m_strLoadPIN.Format("%s", szLoadPIN);

	//Purse PIN
	memset(szPursePIN, 0, 32);
	memcpy(szPINLen, m_KeyData + iOffSet, 2);
	iOffSet += 2;
	iLen = atoi((char*)szPINLen);
	if(iLen > 32)
	{
		iLen  = 30;
		AfxMessageBox("消费程序登入PIN长度错误，请查证");
	}
	memcpy(szPursePIN, m_KeyData + iOffSet, iLen);
	iOffSet += iLen;
	m_strPursePIN.Format("%s", szPursePIN);

	AfxMessageBox("读取密钥成功");

	UpdateData(false);
}


int CManageKey::GetKeyData(unsigned char* pszKeyData)
{
	CRFReader rf;
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