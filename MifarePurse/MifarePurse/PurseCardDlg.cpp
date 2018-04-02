// PurseCardDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MifarePurse.h"
#include "PurseCardDlg.h"
#include "afxdialogex.h"
#include "DES.h"

//#ifndef COMM_H
//#include "CommonFile.h"
//#define COMM_H
//#endif

#ifndef RFR_H
#include "RFReader.h"
#define RFR_H
#endif


///////////////////////////////////////////////////SELF DEFINATION
//CRFReader rf_p;



#define MAIN_KEY		"12345678900987654321ABCDEFFEDCBA"
#define		INI_FILE	"Set.dat"
#define		DEFAULT_KEY	"FFFFFFFFFFFF"
#define BLOCK_LEN	40


/////////////////////////////////////////////


// CPurseCardDlg 对话框

IMPLEMENT_DYNAMIC(CPurseCardDlg, CDialog)

CPurseCardDlg::CPurseCardDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPurseCardDlg::IDD, pParent)
	, m_pCardData(NULL)
	, rf_p(NULL)
	, m_iProjects(0)
	, m_strLog(_T(""))
{
}

CPurseCardDlg::CPurseCardDlg(CardData* pCardData, unsigned char* pszKeyData, CRFReader* rf,
		CWnd* pParent)
	: CDialog(CPurseCardDlg::IDD, pParent)
{
	memcpy(m_KeyData, pszKeyData, 384);//16扇区* 24
	m_pCardData = pCardData;
	rf_p = rf;
}

CPurseCardDlg::~CPurseCardDlg()
{
	m_pCardData = NULL;
	rf_p = NULL;
}

void CPurseCardDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHECK_PROJECT, m_ChkProject);
	DDX_Control(pDX, IDC_CHECK_PROJECT1, m_ChkProject1);
	DDX_Control(pDX, IDC_CHECK_PROJECT2, m_ChkProject2);
	DDX_Control(pDX, IDC_CHECK_PROJECT3, m_ChkProject3);
	DDX_Control(pDX, IDC_EDIT_CARDNO, m_EdtCardNO);
	DDX_Control(pDX, IDC_EDIT_MONEY, m_EdtMoney);
	DDX_Control(pDX, IDC_EDIT_NAME, m_EdtName);
	DDX_Control(pDX, IDC_EDIT_AMOUNT, m_EdtAmount);
	DDX_Control(pDX, IDC_EDIT_TIMES1, m_EdtTimes1);
	DDX_Control(pDX, IDC_EDIT_TIMES2, m_EdtTimes2);
	DDX_Control(pDX, IDC_EDIT_TIMES3, m_EdtTimes3);
	DDX_Control(pDX, IDC_EDIT_PROJECT, m_EdtProject);
	DDX_Control(pDX, IDC_EDIT_PORJECT1, m_EdtProject1);
	DDX_Control(pDX, IDC_EDIT_PORJECT2, m_EdtProject2);
	DDX_Control(pDX, IDC_EDIT_PORJECT3, m_EdtProject3);
	DDX_Control(pDX, IDC_EDIT_VIPCARD, m_EdtVIPCardNO);
	DDX_Control(pDX, IDC_EDIT_PHONE, m_EdtPhoneNO);
	DDX_Control(pDX, IDC_STATIC_TIMES1, m_StTimes1);
	DDX_Control(pDX, IDC_STATIC_BALANCE, m_StBalance);
	DDX_Control(pDX, IDC_STATIC_TIMES3, m_StTimes3);
	DDX_Control(pDX, IDC_STATIC_TIMES2, m_StTimes2);
	DDX_Control(pDX, IDC_EDIT_PORJECT4, m_EdtProject4);
	DDX_Control(pDX, IDC_EDIT_PORJECT5, m_EdtProject5);
	DDX_Control(pDX, IDC_EDIT_PORJECT6, m_EdtProject6);
	DDX_Control(pDX, IDC_EDIT_PORJECT7, m_EdtProject7);
	DDX_Control(pDX, IDC_EDIT_PORJECT8, m_EdtProject8);
	DDX_Control(pDX, IDC_EDIT_PORJECT9, m_EdtProject9);
	DDX_Control(pDX, IDC_EDIT_PORJECT10, m_EdtProject10);
	DDX_Control(pDX, IDC_EDIT_TIMES10, m_EdtTimes10);
	DDX_Control(pDX, IDC_EDIT_TIMES4, m_EdtTimes4);
	DDX_Control(pDX, IDC_EDIT_TIMES5, m_EdtTimes5);
	DDX_Control(pDX, IDC_EDIT_TIMES6, m_EdtTimes6);
	DDX_Control(pDX, IDC_EDIT_TIMES7, m_EdtTimes7);
	DDX_Control(pDX, IDC_EDIT_TIMES8, m_EdtTimes8);
	DDX_Control(pDX, IDC_EDIT_TIMES9, m_EdtTimes9);
	DDX_Control(pDX, IDC_STATIC_TIMES10, m_StTimes10);
	DDX_Control(pDX, IDC_STATIC_TIMES4, m_StTimes4);
	DDX_Control(pDX, IDC_STATIC_TIMES5, m_StTimes5);
	DDX_Control(pDX, IDC_STATIC_TIMES6, m_StTimes6);
	DDX_Control(pDX, IDC_STATIC_TIMES7, m_StTimes7);
	DDX_Control(pDX, IDC_STATIC_TIMES8, m_StTimes8);
	DDX_Control(pDX, IDC_STATIC_TIMES9, m_StTimes9);
	DDX_Control(pDX, IDC_CHECK_PROJECT10, m_ChkProject10);
	DDX_Control(pDX, IDC_CHECK_PROJECT4, m_ChkProject4);
	DDX_Control(pDX, IDC_CHECK_PROJECT5, m_ChkProject5);
	DDX_Control(pDX, IDC_CHECK_PROJECT6, m_ChkProject6);
	DDX_Control(pDX, IDC_CHECK_PROJECT7, m_ChkProject7);
	DDX_Control(pDX, IDC_CHECK_PROJECT8, m_ChkProject8);
	DDX_Control(pDX, IDC_CHECK_PROJECT9, m_ChkProject9);
	DDX_Control(pDX, IDC_EDIT_OPERATOR, m_EdtOperator);
}


BEGIN_MESSAGE_MAP(CPurseCardDlg, CDialog)
	ON_BN_CLICKED(IDCANCEL, &CPurseCardDlg::OnBnClickedCancel)
	ON_BN_CLICKED(ID_BTN_PURE, &CPurseCardDlg::OnBnClickedBtnPure)
	ON_BN_CLICKED(IDC_CHECK_PROJECT3, &CPurseCardDlg::OnBnClickedCheckProject3)
	ON_BN_CLICKED(IDC_CHECK_PROJECT1, &CPurseCardDlg::OnBnClickedCheckProject1)
	ON_BN_CLICKED(IDC_CHECK_PROJECT2, &CPurseCardDlg::OnBnClickedCheckProject2)
	ON_BN_CLICKED(IDC_CHECK_PROJECT, &CPurseCardDlg::OnBnClickedCheckProject)
	ON_BN_CLICKED(ID_BTN_READCARD, &CPurseCardDlg::OnBnClickedBtnReadcard)
	ON_BN_CLICKED(IDC_CHECK_PROJECT4, &CPurseCardDlg::OnBnClickedCheckProject4)
	ON_BN_CLICKED(IDC_CHECK_PROJECT5, &CPurseCardDlg::OnBnClickedCheckProject5)
	ON_BN_CLICKED(IDC_CHECK_PROJECT6, &CPurseCardDlg::OnBnClickedCheckProject6)
	ON_BN_CLICKED(IDC_CHECK_PROJECT7, &CPurseCardDlg::OnBnClickedCheckProject7)
	ON_BN_CLICKED(IDC_CHECK_PROJECT8, &CPurseCardDlg::OnBnClickedCheckProject8)
	ON_BN_CLICKED(IDC_CHECK_PROJECT9, &CPurseCardDlg::OnBnClickedCheckProject9)
	ON_BN_CLICKED(IDC_CHECK_PROJECT10, &CPurseCardDlg::OnBnClickedCheckProject10)
END_MESSAGE_MAP()


// CPurseCardDlg 消息处理程序


void CPurseCardDlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialog::OnCancel();
}

void CPurseCardDlg::OnBnClickedBtnPure()
{
	// TODO: 在此添加控件通知处理程序代码
	time_t t0 = time(0);
	int iBlock;
	unsigned char szBlockData[40], szSectorKey[40];
	unsigned char szOperator[20];

	//无消费项目
	if(!m_ChkProject1.GetCheck() && !m_ChkProject2.GetCheck() &&
		!m_ChkProject3.GetCheck() && !m_ChkProject4.GetCheck() &&
		!m_ChkProject5.GetCheck() && !m_ChkProject6.GetCheck() &&
		!m_ChkProject7.GetCheck() && !m_ChkProject8.GetCheck() &&
		!m_ChkProject9.GetCheck() && !m_ChkProject10.GetCheck() &&
		!m_ChkProject.GetCheck())
	{
		AfxMessageBox((LPCTSTR)"没有消费的项目！");
		return;
	}

	memset(szOperator, 0, sizeof(szOperator));
	m_EdtOperator.GetWindowTextA((LPTSTR)szOperator, 20);
	if(strlen((char*)szOperator) == 0)
	{
		AfxMessageBox("操作员姓名不能为空, 请输入！");
		return;
	}
	
	//校验卡号，是否一致
	if(rf_p->Dev_Reset(1))
	{
		AfxMessageBox("未找到卡片！");
		return;
	}

	iBlock = 22;
	memset(szSectorKey, 0, sizeof(szSectorKey));
	memcpy(szSectorKey, m_KeyData + iBlock/4 * 24, 12);
	//szSectorKey[12] = 0;
	if(rf_p ->LoadKey(rf_p->KEY_L,iBlock/4, szSectorKey, rf_p->HEX))
	{
		AfxMessageBox("读卡器装载密钥失败！");
		return;
	}
	if(rf_p->Anthentication(rf_p->KEY_L, iBlock/4))
	{
		AfxMessageBox("认证密钥失败！");
		return;
	}
	//认证完成

	//读卡号
	if(rf_p->ReadMifareBlock(iBlock, szBlockData, rf_p->HEX))
	{
		AfxMessageBox("读卡号失败！");
		return;
	}
	////比对卡号
	if(memcmp(m_pCardData->szCardNO, szBlockData, 12))
	{
		AfxMessageBox("卡片被替换，请检查卡片！");
		return;
	}

	//卡片OK，开始消费
	//取得消费数据
	unsigned char szAmount[10];
	int iTimes = 0;
	BOOL isAuthed = FALSE;
	long lAmount = 0;
	int iDataLen = 0;
	CString strTemp;

	if(m_ChkProject.GetCheck())
	{
		memset(szAmount, 0, 10);
		m_EdtAmount.GetWindowTextA((LPTSTR)szAmount, 7);
		lAmount = atoi((char*)szAmount);
		if(lAmount > m_pCardData->iAmount)
		{
			strTemp.Format("输入的消费金额(%ld)大于余额(%d)，请确认！", lAmount, m_pCardData->iAmount);
			lAmount = 0;
			AfxMessageBox(strTemp);
			return;
		}
	}

	//if(m_ChkProject1.GetCheck())
	//{//确认并组织数据
	//	memset(szPurseTimes1, 0, 10);
	//	m_EdtTimes1.GetWindowTextA((LPTSTR)szPurseTimes1, 4);
	//	iProject1 = atoi((char*)szPurseTimes1);
	//	if(iProject1 > m_pCardData->iProjectTimes1)
	//	{
	//		strTemp.Format("输入的消费次数(%d)大于剩余次数(%d)，请确认！", iProject1, m_pCardData->iProjectTimes1);
	//		AfxMessageBox(strTemp);
	//		return;
	//	}
	//	
	//	////组织数据
	//	memset(szPurseTimes1, 0, 10);
	//	sprintf_s((char*)szPurseTimes1, 5, "%02X", m_pCardData->iProjectTimes1 - iProject1);
	//	memset(szBlockData1, 0, 40);
	//	iDataLen = 0;
	//	memcpy(szBlockData1, "01", 2);
	//	memcpy(szBlockData1 + 2, szPurseTimes1, 2);
	//	memset(szTempData, 0, 40);
	//	memcpy(szTempData, m_pCardData->szProject1, (iDataLen = strlen((char*)m_pCardData->szProject1)));
	//	rf_p->SwitchDataMode(szTempData, &iDataLen, rf_p->ASC2HEX);
	//	memcpy(szBlockData1 + 4, szTempData, iDataLen);
	//	iDataLen += 4;
	//	while(iDataLen != 32)
	//	{
	//		memcpy(szBlockData1 + iDataLen, "20", 2);
	//		iDataLen += 2;
	//	}
	//}

	//if(m_ChkProject2.GetCheck())
	//{//确认并组织数据
	//	memset(szPurseTimes2, 0, 10);
	//	m_EdtTimes2.GetWindowTextA((LPTSTR)szPurseTimes2, 4);
	//	iProject2 = atoi((char*)szPurseTimes2);
	//	if(iProject2 > m_pCardData->iProjectTimes2)
	//	{
	//		strTemp.Format("输入的消费次数(%d)大于剩余次数(%d)，请确认！", iProject2, m_pCardData->iProjectTimes2);
	//		AfxMessageBox(strTemp);
	//		return;
	//	}
	//	////组织数据
	//	memset(szPurseTimes2, 0, 10);
	//	sprintf_s((char*)szPurseTimes2, 5, "%02X", m_pCardData->iProjectTimes2 - iProject2);
	//	memset(szBlockData2, 0, 40);
	//	iDataLen = 0;
	//	memcpy(szBlockData2, "02", 2);
	//	memcpy(szBlockData2 + 2, szPurseTimes2, 2);
	//	memset(szTempData, 0, 40);
	//	memcpy(szTempData, m_pCardData->szProject2, (iDataLen = strlen((char*)m_pCardData->szProject2)));
	//	rf_p->SwitchDataMode(szTempData, &iDataLen, rf_p->ASC2HEX);
	//	memcpy(szBlockData2 + 4, szTempData, iDataLen);
	//	iDataLen += 4;
	//	while(iDataLen != 32)
	//	{
	//		memcpy(szBlockData2 + iDataLen, "20", 2);
	//		iDataLen += 2;
	//	}
	//}

	//if(m_ChkProject3.GetCheck())
	//{//确认并组织数据
	//	memset(szPurseTimes3, 0, 10);
	//	m_EdtTimes3.GetWindowTextA((LPTSTR)szPurseTimes3, 4);
	//	iProject3 = atoi((char*)szPurseTimes3);
	//	if(iProject3 > m_pCardData->iProjectTimes3)
	//	{
	//		strTemp.Format("输入的消费次数(%d)大于剩余次数(%d)，请确认！", iProject3, m_pCardData->iProjectTimes3);
	//		AfxMessageBox(strTemp);
	//		return;
	//	}
	//	////组织数据
	//	memset(szPurseTimes3, 0, 10);
	//	sprintf_s((char*)szPurseTimes3, 5, "%02X", m_pCardData->iProjectTimes3 - iProject3);
	//	memset(szBlockData3, 0, 40);
	//	iDataLen = 0;
	//	memcpy(szBlockData3, "03", 2);
	//	memcpy(szBlockData3 + 2, szPurseTimes3, 2);
	//	memset(szTempData, 0, 40);
	//	memcpy(szTempData, m_pCardData->szProject3, (iDataLen = strlen((char*)m_pCardData->szProject3)));
	//	rf_p->SwitchDataMode(szTempData, &iDataLen, rf_p->ASC2HEX);
	//	memcpy(szBlockData3 + 4, szTempData, iDataLen);
	//	iDataLen += 4;
	//	while(iDataLen != 32)
	//	{
	//		memcpy(szBlockData3 + iDataLen, "20", 2);
	//		iDataLen += 2;
	//	}
	//}
	strTemp.Format("姓名：%s	卡号：%s\r\n", m_pCardData->szName, m_pCardData->szCardNO);
	
	//日志开始
	//时间
	char szTime[32];
	memset(szTime, 0, 32);
	strftime(szTime, 32, "%Y%m%d%H%M%S", localtime(&t0));
	//姓名 卡号 联系电话
	m_strLog.Format("%s", szTime);

	m_strLog.AppendFormat("\t%s", szOperator);

	m_strLog.AppendFormat("	%s	%s	%s", m_pCardData->szName, m_pCardData->szCardNO, m_pCardData->szPhoneNO);
	m_strLog.AppendFormat("	%d	%d	%d", m_pCardData->iAmount, lAmount, m_pCardData->iAmount - lAmount);

	strTemp.AppendFormat("将消费以下项目：\r\n");
	if(lAmount > 0)
	{
		strTemp.AppendFormat("项目:		消费金额：%d\r\n", lAmount);
	}


	if(m_ChkProject1.GetCheck())
	{
		if(GetProjectData(&projectData[0], 1))
			return;
		if(projectData[0].iDecTimes > 0)
		{
			strTemp.AppendFormat("项目1:%s		消费次数：%d\r\n", projectData[0].szProjectName, projectData[0].iDecTimes);
			m_strLog.AppendFormat("	%s	%d	%d", projectData[0].szProjectName, projectData[0].iDecTimes, projectData[0].iTimes - projectData[0].iDecTimes);
		}
	}
	if(m_ChkProject2.GetCheck())
	{
		if(GetProjectData(&projectData[1], 2))
			return;
		if(projectData[1].iDecTimes > 0)
		{
			strTemp.AppendFormat("项目2:%s		消费次数：%d\r\n", projectData[1].szProjectName, projectData[1].iDecTimes);			
			m_strLog.AppendFormat("	%s	%d	%d", projectData[1].szProjectName, projectData[1].iDecTimes, projectData[1].iTimes - projectData[1].iDecTimes);
		}
	}
	if(m_ChkProject3.GetCheck())
	{
		if(GetProjectData(&projectData[2], 3))
			return;
		
		if(projectData[2].iDecTimes > 0)
		{
			strTemp.AppendFormat("项目3:%s		消费次数：%d\r\n", projectData[2].szProjectName, projectData[2].iDecTimes);
			m_strLog.AppendFormat("	%s	%d	%d", projectData[2].szProjectName, projectData[2].iDecTimes, projectData[2].iTimes - projectData[2].iDecTimes);
		}
	}
	if(m_ChkProject4.GetCheck())
	{
		if(GetProjectData(&projectData[3], 4))
			return;
		if(projectData[3].iDecTimes > 0)
		{
			strTemp.AppendFormat("项目4:%s		消费次数：%d\r\n", projectData[3].szProjectName, projectData[3].iDecTimes);
			m_strLog.AppendFormat("	%s	%d	%d", projectData[3].szProjectName, projectData[3].iDecTimes, projectData[3].iTimes - projectData[3].iDecTimes);
		}
	}
	if(m_ChkProject5.GetCheck())
	{
		if(GetProjectData(&projectData[4], 5))
			return;
		
		if(projectData[4].iDecTimes > 0)
		{
			strTemp.AppendFormat("项目5:%s		消费次数：%d\r\n", projectData[4].szProjectName, projectData[4].iDecTimes);
			m_strLog.AppendFormat("	%s	%d	%d", projectData[4].szProjectName, projectData[4].iDecTimes, projectData[4].iTimes - projectData[4].iDecTimes);
		}
	}
	if(m_ChkProject6.GetCheck())
	{
		if(GetProjectData(&projectData[5], 6))
			return;
		
		if(projectData[5].iDecTimes > 0)
		{
			strTemp.AppendFormat("项目6:%s		消费次数：%d\r\n", projectData[5].szProjectName, projectData[5].iDecTimes);
			m_strLog.AppendFormat("	%s	%d	%d", projectData[5].szProjectName, projectData[5].iDecTimes, projectData[5].iTimes - projectData[5].iDecTimes);
		}
	}
	if(m_ChkProject7.GetCheck())
	{
		if(GetProjectData(&projectData[6], 7))
			return;
		
		if(projectData[6].iDecTimes > 0)
		{
			strTemp.AppendFormat("项目7:%s		消费次数：%d\r\n", projectData[6].szProjectName, projectData[6].iDecTimes);
			m_strLog.AppendFormat("	%s	%d	%d", projectData[6].szProjectName, projectData[6].iDecTimes, projectData[6].iTimes - projectData[6].iDecTimes);
		}
	}
	if(m_ChkProject8.GetCheck())
	{
		if(GetProjectData(&projectData[7], 8))
			return;
		
		if(projectData[7].iDecTimes > 0)
		{
			strTemp.AppendFormat("项目8:%s		消费次数：%d\r\n", projectData[7].szProjectName, projectData[7].iDecTimes);
			m_strLog.AppendFormat("	%s	%d	%d", projectData[7].szProjectName, projectData[7].iDecTimes, projectData[7].iTimes - projectData[7].iDecTimes);
		}
	}
	if(m_ChkProject9.GetCheck())
	{
		if(GetProjectData(&projectData[8], 9))
			return;
		if(projectData[8].iDecTimes > 0)
		{
			strTemp.AppendFormat("项目9:%s		消费次数：%d\r\n", projectData[8].szProjectName, projectData[8].iDecTimes);
			m_strLog.AppendFormat("	%s	%d	%d", projectData[8].szProjectName, projectData[8].iDecTimes, projectData[8].iTimes - projectData[8].iDecTimes);
		}
	}
	if(m_ChkProject10.GetCheck())
	{
		if(GetProjectData(&projectData[9], 10))
			return;
		if(projectData[9].iDecTimes > 0)
		{
			strTemp.AppendFormat("项目10:%s		消费次数：%d\r\n", projectData[9].szProjectName, projectData[9].iDecTimes);
			m_strLog.AppendFormat("	%s	%d	%d", projectData[9].szProjectName, projectData[9].iDecTimes, projectData[9].iTimes - projectData[9].iDecTimes);
		}
	}

	if(MessageBox((LPCTSTR)strTemp, (LPCTSTR)"消费确认", MB_OKCANCEL) == IDCANCEL)
		return;

	//开始写数据
	//防掉电功能待添加 120519

	WriteLog(m_strLog);
	
	//消费金额
	if(lAmount > 0)
	{	
		//认证扇区
		if(rf_p->Dev_Reset(1))
		{
			rf_p->Dev_Beep(5);
			rf_p->Dev_Beep(5);
			AfxMessageBox("未找到卡片！");
			strTemp.Format("上一笔交易<%s>消费失败：未找到卡片", szTime);
			WriteLog(strTemp);
			return;
		}

		iBlock = 20;
		memset(szSectorKey, 0, sizeof(szSectorKey));
		memcpy(szSectorKey, m_KeyData + iBlock/4 * 24, 24);
		if(rf_p ->LoadKey(rf_p->KEY_R,iBlock/4, szSectorKey + 12, rf_p->HEX))
		{
			rf_p->Dev_Beep(5);
			rf_p->Dev_Beep(5);
			AfxMessageBox("读卡器装载密钥失败！");
			strTemp.Format("上一笔交易<%s>消费失败：读卡器装载密钥失败", szTime);
			WriteLog(strTemp);
			return;
		}
		if(rf_p->Anthentication(rf_p->KEY_R, iBlock/4))
		{
			rf_p->Dev_Beep(5);
			rf_p->Dev_Beep(5);
			AfxMessageBox("认证密钥失败！");
			strTemp.Format("上一笔交易<%s>消费失败：认证密钥失败", szTime);
			WriteLog(strTemp);
			return;
		}
		//认证完成

		//扣款
		if(rf_p->Decrement(iBlock, lAmount))
		{
			rf_p->Dev_Beep(5);
			rf_p->Dev_Beep(5);
			AfxMessageBox("扣款消费失败，请确认卡片！");
			strTemp.Format("上一笔交易<%s>消费失败：扣款消费失败", szTime);
			WriteLog(strTemp);
			return;
		}
		
		//刷新显示
		m_pCardData->iAmount -= lAmount;
		strTemp.Format("%d", m_pCardData->iAmount);
		m_EdtAmount.SetWindowTextA((LPCTSTR)strTemp);
		m_EdtAmount.EnableWindow(FALSE);
		m_ChkProject.SetCheck(0);
		m_StBalance.SetWindowTextA("消费金额：");
	}

	for(int i = 0; i != m_pCardData->iProjectsCount; ++i)
	{
		if(projectData[i].iDecTimes > 0)
		{
			if(WriteProjectBlock(&projectData[i]))
				return;
			projectData[i].iTimes -= projectData[i].iDecTimes;
			projectData[i].iDecTimes = 0;
		}
	}

	//界面修饰
	m_ChkProject1.SetCheck(FALSE);
	m_ChkProject2.SetCheck(FALSE);
	m_ChkProject3.SetCheck(FALSE);
	m_ChkProject4.SetCheck(FALSE);
	m_ChkProject5.SetCheck(FALSE);
	m_ChkProject6.SetCheck(FALSE);
	m_ChkProject7.SetCheck(FALSE);
	m_ChkProject8.SetCheck(FALSE);
	m_ChkProject9.SetCheck(FALSE);
	m_ChkProject10.SetCheck(FALSE);

	strTemp.Format("%d", projectData[0].iTimes);
	m_EdtTimes1.SetWindowTextA(strTemp);
	m_EdtTimes1.EnableWindow(FALSE);
	strTemp.Format("%d", projectData[1].iTimes);
	m_EdtTimes2.SetWindowTextA(strTemp);
	m_EdtTimes2.EnableWindow(FALSE);
	strTemp.Format("%d", projectData[2].iTimes);
	m_EdtTimes3.SetWindowTextA(strTemp);
	m_EdtTimes3.EnableWindow(FALSE);
	strTemp.Format("%d", projectData[3].iTimes);
	m_EdtTimes4.SetWindowTextA(strTemp);
	m_EdtTimes4.EnableWindow(FALSE);
	strTemp.Format("%d", projectData[4].iTimes);
	m_EdtTimes5.SetWindowTextA(strTemp);
	m_EdtTimes5.EnableWindow(FALSE);
	strTemp.Format("%d", projectData[5].iTimes);
	m_EdtTimes6.SetWindowTextA(strTemp);
	m_EdtTimes6.EnableWindow(FALSE);
	strTemp.Format("%d", projectData[6].iTimes);
	m_EdtTimes7.SetWindowTextA(strTemp);
	m_EdtTimes7.EnableWindow(FALSE);
	strTemp.Format("%d", projectData[7].iTimes);
	m_EdtTimes8.SetWindowTextA(strTemp);
	m_EdtTimes8.EnableWindow(FALSE);
	strTemp.Format("%d", projectData[8].iTimes);
	m_EdtTimes9.SetWindowTextA(strTemp);
	m_EdtTimes9.EnableWindow(FALSE);
	strTemp.Format("%d", projectData[9].iTimes);
	m_EdtTimes10.SetWindowTextA(strTemp);
	m_EdtTimes10.EnableWindow(FALSE);

	rf_p->Dev_Beep(10);
	AfxMessageBox("消费完成！");

}

void CPurseCardDlg::OnBnClickedBtnReadcard()
{
	// TODO: 在此添加控件通知处理程序代码
	
	if(ReadPersonalData())
		return;

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
	
	memset(projectData, 0, sizeof(projectData));
	//取项目区数据
	for(int i = 0;i != m_pCardData->iProjectsCount; ++i)
	{
		if(ReadProjectBlock(&projectData[i], i + 1))
		{
			AfxMessageBox("读取卡片项目信息失败！");
			break;
		}
		if(SetProjectData(projectData[i], i + 1))
		{
			AfxMessageBox("显示卡片项目信息失败！");
			break;
		}
	}

	AfxMessageBox("读卡成功！");
	//rf_p->Dev_Beep(5);
}

int CPurseCardDlg::GetKeyData(unsigned char* pszKeyData)
{	
	CFile file;
	CString strTemp;
	CDES des;
	unsigned char szDesKey[40], szCardKey[512], szKeyData[512];
	int iKeyLen, iDataLen = 0;

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
	rf_p->Hex2Asc((unsigned char*)MAIN_KEY, szDesKey, 32);
	des.RunDes(des.DECRYPT, des.ECB, (char*)szKeyData, (char*)szCardKey, iKeyLen, (char*)szDesKey, 16);
	rf_p->Asc2Hex(szCardKey, pszKeyData, iKeyLen);
	iKeyLen = iKeyLen*2;

	pszKeyData[iKeyLen] = 0;

	return 0;
}

BOOL CPurseCardDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	CString strTemp;

	m_EdtName.SetWindowTextA((LPCTSTR)m_pCardData->szName);
	m_EdtCardNO.SetWindowTextA((LPCTSTR)m_pCardData->szCardNO);
	m_EdtVIPCardNO.SetWindowTextA((LPCTSTR)m_pCardData->szVIPCardNO);
	m_EdtPhoneNO.SetWindowTextA((LPCTSTR)m_pCardData->szPhoneNO);
	strTemp.Format("%d", m_pCardData->iAmount);
	m_EdtAmount.SetWindowTextA((LPCTSTR)strTemp);
	m_EdtMoney.SetWindowTextA((LPCTSTR)strTemp);

	if(m_pCardData->iAmount > 0)
	{
		m_ChkProject.EnableWindow(TRUE);
	}

	
	memset(projectData, 0, sizeof(projectData));
	//取项目区数据
	for(int i = 0;i != m_pCardData->iProjectsCount; ++i)
	{
		if(ReadProjectBlock(&projectData[i], i + 1))
		{
			AfxMessageBox("读取卡片项目信息失败！");
			break;
		}
		if(SetProjectData(projectData[i], i + 1))
		{
			AfxMessageBox("显示卡片项目信息失败！");
			break;
		}
	}
	
	m_EdtAmount.LimitText(9);
	m_EdtAmount.LimitText(9);
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

	//if(m_pCardData->iProjectTimes1 >= 0)
	//{
	//	m_EdtProject1.SetWindowTextA((LPCTSTR)m_pCardData->szProject1);
	//	strTemp.Format("%d", m_pCardData->iProjectTimes1);
	//	m_EdtTimes1.SetWindowTextA((LPCTSTR)strTemp);
	//	m_ChkProject1.EnableWindow(TRUE);
	//}
	//if(m_pCardData->iProjectTimes2 >= 0)
	//{
	//	m_EdtProject2.SetWindowTextA((LPCTSTR)m_pCardData->szProject2);
	//	strTemp.Format("%d", m_pCardData->iProjectTimes2);
	//	m_EdtTimes2.SetWindowTextA((LPCTSTR)strTemp);
	//	m_ChkProject2.EnableWindow(TRUE);
	//}
	//if(m_pCardData->iProjectTimes3 >= 0)
	//{
	//	m_EdtProject3.SetWindowTextA((LPCTSTR)m_pCardData->szProject3);
	//	strTemp.Format("%d", m_pCardData->iProjectTimes3);
	//	m_EdtTimes3.SetWindowTextA((LPCTSTR)strTemp);
	//	m_ChkProject3.EnableWindow(TRUE);
	//}


	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CPurseCardDlg::OnBnClickedCheckProject()
{
	// TODO: 在此添加控件通知处理程序代码
	if(m_ChkProject.GetCheck())
	{
		m_EdtAmount.EnableWindow(TRUE);
		m_EdtAmount.SetWindowTextA("0");
		m_StBalance.SetWindowTextA("消费金额：");
	}
	else
	{
		CString strTemp;
		m_EdtAmount.EnableWindow(false);
		strTemp.Format("%d", m_pCardData->iAmount);
		m_EdtAmount.SetWindowTextA((LPCTSTR)strTemp);
		m_StBalance.SetWindowTextA("当前余额：");
	}
}

int CPurseCardDlg::SetProjectData(ProjectData projectData, int iProject)
{
	CString strTemp;
	if(iProject == 1)
	{
		strTemp.Format("%d", projectData.iTimes);
		m_EdtProject1.SetWindowTextA((LPCTSTR)projectData.szProjectName);
		m_EdtTimes1.SetWindowTextA((LPCTSTR)strTemp);
		m_ChkProject1.EnableWindow(TRUE);
	}
	else if(iProject == 2)
	{
		strTemp.Format("%d", projectData.iTimes);
		m_EdtProject2.SetWindowTextA((LPCTSTR)projectData.szProjectName);
		m_EdtTimes2.SetWindowTextA((LPCTSTR)strTemp);
		m_ChkProject2.EnableWindow(TRUE);
	}
	else if(iProject == 3)
	{
		strTemp.Format("%d", projectData.iTimes);
		m_EdtProject3.SetWindowTextA((LPCTSTR)projectData.szProjectName);
		m_EdtTimes3.SetWindowTextA((LPCTSTR)strTemp);
		m_ChkProject3.EnableWindow(TRUE);
	}
	else if(iProject == 4)
	{
		strTemp.Format("%d", projectData.iTimes);
		m_EdtProject4.SetWindowTextA((LPCTSTR)projectData.szProjectName);
		m_EdtTimes4.SetWindowTextA((LPCTSTR)strTemp);
		m_ChkProject4.EnableWindow(TRUE);
	}
	else if(iProject == 5)
	{
		strTemp.Format("%d", projectData.iTimes);
		m_EdtProject5.SetWindowTextA((LPCTSTR)projectData.szProjectName);
		m_EdtTimes5.SetWindowTextA((LPCTSTR)strTemp);
		m_ChkProject5.EnableWindow(TRUE);
	}
	else if(iProject == 6)
	{
		strTemp.Format("%d", projectData.iTimes);
		m_EdtProject6.SetWindowTextA((LPCTSTR)projectData.szProjectName);
		m_EdtTimes6.SetWindowTextA((LPCTSTR)strTemp);
		m_ChkProject6.EnableWindow(TRUE);
	}
	else if(iProject == 7)
	{
		strTemp.Format("%d", projectData.iTimes);
		m_EdtProject7.SetWindowTextA((LPCTSTR)projectData.szProjectName);
		m_EdtTimes7.SetWindowTextA((LPCTSTR)strTemp);
		m_ChkProject7.EnableWindow(TRUE);
	}
	else if(iProject == 8)
	{
		strTemp.Format("%d", projectData.iTimes);
		m_EdtProject8.SetWindowTextA((LPCTSTR)projectData.szProjectName);
		m_EdtTimes8.SetWindowTextA((LPCTSTR)strTemp);
		m_ChkProject8.EnableWindow(TRUE);
	}
	else if(iProject == 9)
	{
		strTemp.Format("%d", projectData.iTimes);
		m_EdtProject9.SetWindowTextA((LPCTSTR)projectData.szProjectName);
		m_EdtTimes9.SetWindowTextA((LPCTSTR)strTemp);
		m_ChkProject9.EnableWindow(TRUE);
	}
	else if(iProject == 10)
	{
		strTemp.Format("%d", projectData.iTimes);
		m_EdtProject10.SetWindowTextA((LPCTSTR)projectData.szProjectName);
		m_EdtTimes10.SetWindowTextA((LPCTSTR)strTemp);
		m_ChkProject10.EnableWindow(TRUE);
	}

	return 0;	
}

int CPurseCardDlg::GetProjectData(ProjectData* pData, int iProject)
{
	unsigned char szTimes[5];
	memset(szTimes, 0, 5);
	if(iProject == 1)
	{
		m_EdtTimes1.GetWindowTextA((LPTSTR)szTimes, 4);
		pData->iDecTimes = atoi((char*)szTimes);
		if(pData->iDecTimes > pData->iTimes)
		{
			CString strTemp;
			strTemp.Format("项目%d中可用余次(%d)小于消费次数(%d)，请确认！", iProject, pData->iTimes, pData->iDecTimes);
			pData->iDecTimes = 0;
			AfxMessageBox(strTemp);
			return 1;
		}
	}
	else if(iProject == 2)
	{
		m_EdtTimes2.GetWindowTextA((LPTSTR)szTimes, 4);
		pData->iDecTimes = atoi((char*)szTimes);
		if(pData->iDecTimes > pData->iTimes)
		{
			CString strTemp;
			strTemp.Format("项目%d中可用余次(%d)小于消费次数(%d)，请确认！", iProject, pData->iTimes, pData->iDecTimes);
			pData->iDecTimes = 0;
			AfxMessageBox(strTemp);
			return 1;
		}
	}
	else if(iProject == 3)
	{
		m_EdtTimes3.GetWindowTextA((LPTSTR)szTimes, 4);
		pData->iDecTimes = atoi((char*)szTimes);
		if(pData->iDecTimes > pData->iTimes)
		{
			CString strTemp;
			strTemp.Format("项目%d中可用余次(%d)小于消费次数(%d)，请确认！", iProject, pData->iTimes, pData->iDecTimes);
			pData->iDecTimes = 0;
			AfxMessageBox(strTemp);
			return 1;
		}
	}
	else if(iProject == 4)
	{
		m_EdtTimes4.GetWindowTextA((LPTSTR)szTimes, 4);
		pData->iDecTimes = atoi((char*)szTimes);
		if(pData->iDecTimes > pData->iTimes)
		{
			CString strTemp;
			strTemp.Format("项目%d中可用余次(%d)小于消费次数(%d)，请确认！", iProject, pData->iTimes, pData->iDecTimes);
			pData->iDecTimes = 0;
			AfxMessageBox(strTemp);
			return 1;
		}
	}
	else if(iProject == 5)
	{
		m_EdtTimes5.GetWindowTextA((LPTSTR)szTimes, 4);
		pData->iDecTimes = atoi((char*)szTimes);
		if(pData->iDecTimes > pData->iTimes)
		{
			CString strTemp;
			strTemp.Format("项目%d中可用余次(%d)小于消费次数(%d)，请确认！", iProject, pData->iTimes, pData->iDecTimes);
			pData->iDecTimes = 0;
			AfxMessageBox(strTemp);
			return 1;
		}
	}
	else if(iProject == 6)
	{
		m_EdtTimes6.GetWindowTextA((LPTSTR)szTimes, 4);
		pData->iDecTimes = atoi((char*)szTimes);
		if(pData->iDecTimes > pData->iTimes)
		{
			CString strTemp;
			strTemp.Format("项目%d中可用余次(%d)小于消费次数(%d)，请确认！", iProject, pData->iTimes, pData->iDecTimes);
			pData->iDecTimes = 0;
			AfxMessageBox(strTemp);
			return 1;
		}
	}
	else if(iProject == 7)
	{
		m_EdtTimes7.GetWindowTextA((LPTSTR)szTimes, 4);
		pData->iDecTimes = atoi((char*)szTimes);
		if(pData->iDecTimes > pData->iTimes)
		{
			CString strTemp;
			strTemp.Format("项目%d中可用余次(%d)小于消费次数(%d)，请确认！", iProject, pData->iTimes, pData->iDecTimes);
			pData->iDecTimes = 0;
			AfxMessageBox(strTemp);
			return 1;
		}
	}
	else if(iProject == 8)
	{
		m_EdtTimes8.GetWindowTextA((LPTSTR)szTimes, 4);
		pData->iDecTimes = atoi((char*)szTimes);
		if(pData->iDecTimes > pData->iTimes)
		{
			CString strTemp;
			strTemp.Format("项目%d中可用余次(%d)小于消费次数(%d)，请确认！", iProject, pData->iTimes, pData->iDecTimes);
			pData->iDecTimes = 0;
			AfxMessageBox(strTemp);
			return 1;
		}
	}
	else if(iProject == 9)
	{
		m_EdtTimes9.GetWindowTextA((LPTSTR)szTimes, 4);
		pData->iDecTimes = atoi((char*)szTimes);
		if(pData->iDecTimes > pData->iTimes)
		{
			CString strTemp;
			strTemp.Format("项目%d中可用余次(%d)小于消费次数(%d)，请确认！", iProject, pData->iTimes, pData->iDecTimes);
			pData->iDecTimes = 0;
			AfxMessageBox(strTemp);
			return 1;
		}
	}
	else if(iProject == 10)
	{
		m_EdtTimes10.GetWindowTextA((LPTSTR)szTimes, 4);
		pData->iDecTimes = atoi((char*)szTimes);
		if(pData->iDecTimes > pData->iTimes)
		{
			CString strTemp;
			strTemp.Format("项目%d中可用余次(%d)小于消费次数(%d)，请确认！", iProject, pData->iTimes, pData->iDecTimes);
			pData->iDecTimes = 0;
			AfxMessageBox(strTemp);
			return 1;
		}
	}

	return 0;
}

int CPurseCardDlg::WriteProjectBlock(ProjectData* pData)
{
	int iRet = 0, iBlock = 24, iDataLen = 0;
	CString strTemp;
	unsigned char szBlockData[BLOCK_LEN];

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
	default: return 2;
	}

	iRet = AuthSector(iBlock/4, rf_p->KEY_R);
	if(iRet)
	{
		AfxMessageBox("认证卡片密钥失败，请检查卡片！");
		return 2;
	}

	//memset(szBlockData, 0, BLOCK_LEN);
	//strTemp.Format("%02X", pData->iProjectIndex);
	memcpy(szBlockData, pData->szProjectIndex, 2);
	strTemp.Format("%04X", pData->iTimes - pData->iDecTimes);
	memcpy(szBlockData + 2, strTemp, 4);

	strTemp.Format("%s", pData->szProjectName);
	rf_p->Asc2Hex((unsigned char*)strTemp.GetBuffer(), szBlockData + 6, (iDataLen = strTemp.GetLength()));
	iDataLen = iDataLen * 2 + 6;

	while(iDataLen < 32)
	{
		memcpy(szBlockData + iDataLen, "20", 2);
		iDataLen += 2;
	}

	if((iRet = rf_p->Write(iBlock, szBlockData, rf_p->HEX)) != 0)
	{
		AfxMessageBox("写项目数据失败，请检查卡片！");
		return 2;
	}

	return 0;
}

int CPurseCardDlg::ReadProjectBlock(ProjectData* pData, int iProject)
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

	iRet = AuthSector(iBlock/4, rf_p->KEY_L);
	if(iRet)
	{
		AfxMessageBox("认证密钥失败，请检查卡片！");
		return 3;
	}
	
	memset(szBlockData, 0, BLOCK_LEN);
	if(rf_p->ReadMifareBlock(iBlock, szBlockData, rf_p->HEX))
	{
		AfxMessageBox("读项目数据失败，请检查卡片！");
		return 3;
	}

	memset(szTempData, 0, 30);
	memcpy(pData->szProjectIndex, szBlockData, 2);
	pData->szProjectIndex[2] = 0;
	pData->iProjectIndex = rf_p->Hex2Dec(pData->szProjectIndex);
	memcpy(szTempData, szBlockData + 2, 4);
	pData->iTimes = rf_p->Hex2Dec(szTempData);

	rf_p->Hex2Asc(szBlockData + 6, szTempData, 26);
	strTemp.Format("%.26s", szTempData);
	strTemp.Remove(0x20);

	//memset(pData->szProjectName, 0, sizeof(pData->szProjectName));
	memcpy(pData->szProjectName, strTemp, strTemp.GetLength());
	//pData->szProjectName[strTemp.GetLength()] = 0;

	return 0;
}

int CPurseCardDlg::ReadPersonalData(void)
{
	int iBlock = 0, iDataLen;
	long lAmount = 0;
	CString strTemp;
	unsigned char szTempData[40];
	unsigned char szBlockData[40];

	iBlock = 20;//余额
	if(AuthSector(iBlock/4, rf_p->KEY_L))
	{
		AfxMessageBox("卡片认证失败！");
		return 2;
	}

	if(rf_p->ReadValue(iBlock, (unsigned long*)&lAmount))
	{
		AfxMessageBox("读卡余额失败！");
		return 2;
	}
	strTemp.Format("%ld", lAmount);
	m_pCardData->iAmount = lAmount;
	memcpy(m_pCardData->szBalance, strTemp, strTemp.GetLength());
	//显示余额信息
	m_EdtAmount.SetWindowTextA(strTemp);
	m_EdtMoney.SetWindowTextA(strTemp);

	iBlock = 21;
	memset(szBlockData, 0, 40);
	if(rf_p->ReadMifareBlock(iBlock, szBlockData, rf_p->HEX))
	{
		AfxMessageBox("读姓名信息失败！");
		return 2;
	}

	//联系电话
	memcpy(m_pCardData->szPhoneNO, szBlockData + 20, 12);
	m_EdtPhoneNO.SetWindowTextA((LPCTSTR)m_pCardData->szPhoneNO);

	iDataLen = 20;
	szBlockData[20] = 0;
	rf_p->SwitchDataMode(szBlockData, &iDataLen, rf_p->HEX2ASC);
	strTemp.Format("%s", szBlockData);
	strTemp.Remove(0x20);

	//名字
	m_EdtName.SetWindowTextA(strTemp);

	iBlock = 22;
	memset(szBlockData, 0, 40);
	if(rf_p->ReadMifareBlock(iBlock, szBlockData, rf_p->HEX))
	{
		AfxMessageBox("读卡号信息失败！");
		return 2;
	}

	memcpy(m_pCardData->szCardNO, szBlockData, 12);
	m_pCardData->szCardNO[12] = 0;
	memcpy(m_pCardData->szVIPCardNO, szBlockData + 12, 12);
	m_pCardData->szVIPCardNO[12] = 0;

	//显示卡号与VIP卡号
	m_EdtCardNO.SetWindowTextA((LPCTSTR)m_pCardData->szCardNO);
	m_EdtVIPCardNO.SetWindowTextA((LPCTSTR)m_pCardData->szVIPCardNO);

	memset(szTempData, 0, 40);
	memcpy(szTempData, szBlockData + 24, 2);//项目数量

	m_iProjects = rf_p->Hex2Dec(szTempData);
	m_pCardData->iProjectsCount = m_iProjects;

	return 0;
}

int CPurseCardDlg::AuthSector(int iSector, int iKeyModal)
{
	//得到左密钥
	//信息区块为20 21 22
	unsigned char szSectorKey[40];
	memset(szSectorKey, 0, 40);
	if(rf_p->Dev_Reset(1))
	{
		AfxMessageBox("卡片复位失败！");
		return 1;
	}
	if(iKeyModal == rf_p->KEY_L)
		memcpy(szSectorKey, m_KeyData + iSector*24, 12);
	else
		memcpy(szSectorKey, m_KeyData + iSector*24 + 12, 12);

	if(rf_p->LoadKey(iKeyModal, iSector, szSectorKey, rf_p->HEX))
	{
		AfxMessageBox("认证密钥失败！");
		return 1;	
	}

	if(rf_p->Anthentication(iKeyModal, iSector))
	{
		AfxMessageBox("认证密钥失败2！");
		return 1;
	}
	//认证密钥完成

	return 0;
}

void CPurseCardDlg::OnBnClickedCheckProject1()
{
	// TODO: 在此添加控件通知处理程序代码
	if(m_ChkProject1.GetCheck())
	{
		m_EdtTimes1.EnableWindow(TRUE);
		m_EdtTimes1.SetWindowTextA("");
	}
	else
	{
		m_EdtTimes1.EnableWindow(false);
		CString strTemp;
		strTemp.Format("%d", projectData[0].iTimes);
		m_EdtTimes1.SetWindowTextA((LPCTSTR)strTemp);
	}
}

void CPurseCardDlg::OnBnClickedCheckProject2()
{
	// TODO: 在此添加控件通知处理程序代码

	if(m_ChkProject2.GetCheck())
	{
		m_EdtTimes2.EnableWindow(TRUE);
		m_EdtTimes2.SetWindowTextA("");
	}
	else
	{
		m_EdtTimes2.EnableWindow(FALSE);
		CString strTemp;
		strTemp.Format("%d", projectData[1].iTimes);
		m_EdtTimes2.SetWindowTextA((LPCTSTR)strTemp);
	}
}

void CPurseCardDlg::OnBnClickedCheckProject3()
{
	// TODO: 在此添加控件通知处理程序代码

	if(m_ChkProject3.GetCheck())
	{
		m_EdtTimes3.EnableWindow(TRUE);
		m_EdtTimes3.SetWindowTextA("");
	}
	else
	{
		m_EdtTimes3.EnableWindow(FALSE);
		CString strTemp;
		strTemp.Format("%d", projectData[2].iTimes);
		m_EdtTimes3.SetWindowTextA((LPCTSTR)strTemp);
	}
}

void CPurseCardDlg::OnBnClickedCheckProject4()
{
	// TODO: 在此添加控件通知处理程序代码

	if(m_ChkProject4.GetCheck())
	{
		m_EdtTimes4.EnableWindow(TRUE);
		m_EdtTimes4.SetWindowTextA("");
	}
	else
	{
		m_EdtTimes4.EnableWindow(FALSE);
		CString strTemp;
		strTemp.Format("%d", projectData[3].iTimes);
		m_EdtTimes4.SetWindowTextA((LPCTSTR)strTemp);
	}
}

void CPurseCardDlg::OnBnClickedCheckProject5()
{
	// TODO: 在此添加控件通知处理程序代码
	
	if(m_ChkProject5.GetCheck())
	{
		m_EdtTimes5.EnableWindow(TRUE);
		m_EdtTimes5.SetWindowTextA("");
	}
	else
	{
		m_EdtTimes5.EnableWindow(FALSE);
		CString strTemp;
		strTemp.Format("%d", projectData[4].iTimes);
		m_EdtTimes5.SetWindowTextA((LPCTSTR)strTemp);
	}
}

void CPurseCardDlg::OnBnClickedCheckProject6()
{
	// TODO: 在此添加控件通知处理程序代码
	
	if(m_ChkProject6.GetCheck())
	{
		m_EdtTimes6.EnableWindow(TRUE);
		m_EdtTimes6.SetWindowTextA("");
	}
	else
	{
		m_EdtTimes6.EnableWindow(FALSE);
		CString strTemp;
		strTemp.Format("%d", projectData[5].iTimes);
		m_EdtTimes6.SetWindowTextA((LPCTSTR)strTemp);
	}
}

void CPurseCardDlg::OnBnClickedCheckProject7()
{
	// TODO: 在此添加控件通知处理程序代码
	
	if(m_ChkProject7.GetCheck())
	{
		m_EdtTimes7.EnableWindow(TRUE);
		m_EdtTimes7.SetWindowTextA("");
	}
	else
	{
		m_EdtTimes7.EnableWindow(FALSE);
		CString strTemp;
		strTemp.Format("%d", projectData[6].iTimes);
		m_EdtTimes7.SetWindowTextA((LPCTSTR)strTemp);
	}
}

void CPurseCardDlg::OnBnClickedCheckProject8()
{
	// TODO: 在此添加控件通知处理程序代码
	
	if(m_ChkProject8.GetCheck())
	{
		m_EdtTimes8.EnableWindow(TRUE);
		m_EdtTimes8.SetWindowTextA("");
	}
	else
	{
		m_EdtTimes8.EnableWindow(FALSE);
		CString strTemp;
		strTemp.Format("%d", projectData[7].iTimes);
		m_EdtTimes8.SetWindowTextA((LPCTSTR)strTemp);
	}
}

void CPurseCardDlg::OnBnClickedCheckProject9()
{
	// TODO: 在此添加控件通知处理程序代码
	
	if(m_ChkProject9.GetCheck())
	{
		m_EdtTimes9.EnableWindow(TRUE);
		m_EdtTimes9.SetWindowTextA("");
	}
	else
	{
		m_EdtTimes9.EnableWindow(FALSE);
		CString strTemp;
		strTemp.Format("%d", projectData[8].iTimes);
		m_EdtTimes9.SetWindowTextA((LPCTSTR)strTemp);
	}
}

void CPurseCardDlg::OnBnClickedCheckProject10()
{
	// TODO: 在此添加控件通知处理程序代码
	
	if(m_ChkProject10.GetCheck())
	{
		m_EdtTimes10.EnableWindow(TRUE);
		m_EdtTimes10.SetWindowTextA("");
	}
	else
	{
		m_EdtTimes10.EnableWindow(FALSE);
		CString strTemp;
		strTemp.Format("%d", projectData[9].iTimes);
		m_EdtTimes10.SetWindowTextA((LPCTSTR)strTemp);
	}
}
