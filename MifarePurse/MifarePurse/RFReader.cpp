#include "StdAfx.h"
#include "RFReader.h"
#include "mwrf32.h"

#pragma comment(lib, "mwrf32.lib")

#define UNICODE

//使用接触式读卡
//#define CONTACT

CRFReader::CRFReader(void)
	: m_isConnected(false)
{
}


CRFReader::~CRFReader(void)
{
	Dev_Close();
}


int CRFReader::Dev_Connect(int iPort, long lBaud)
{
	m_hDev = rf_init(iPort, lBaud);
	if(m_hDev && (unsigned int)m_hDev != 0xffffffe0)
	{
		m_isConnected = TRUE;
//#ifdef	DEBUG_NEW
//		rf_beep(m_hDev, 20);
//#endif
	}
	else
		return -1;
	return 0;
}


int CRFReader::Dev_Close(void)
{
	int iRet = 0;

	if(m_isConnected )
	{
////如果为非接，完成halt()
//#ifndef CONTACK
//		iRet = rf_halt(m_hDev);
//#endif
		iRet = rf_exit(m_hDev);
		m_hDev = NULL;
		m_isConnected = FALSE;
	}
	return iRet;
}


//1 非接  2 接触式 3 sam
int CRFReader::Dev_Reset(int iCardType)
{
	int iRet;
	unsigned char szRecvBuf[128] = {0};
	unsigned long lDevSn;
	unsigned short sDevSn;
	unsigned char szSendBuf[128] = {0};
	unsigned char szRetST[10] = {0};
	unsigned char chRecvLen;
	unsigned char szRetInfoHex[512] ={0};

	memset(m_szResetInfo, 0, 128);


//使用接触式读卡
#ifdef CONTACT
	iRet = rf_cpureset(m_hDev, &chRecvLen, (UCHAR*)m_szResetInfo);
	return iRet;
#endif

	if(iCardType == 1)
	{
		iRet = rf_reset(m_hDev, 10);

		iRet = rf_request(m_hDev, 1, &sDevSn);
		iRet = rf_anticoll(m_hDev, 0, &lDevSn);
		iRet = rf_select(m_hDev, lDevSn, &chRecvLen);

		//iRet = rf_card(m_hDev, 0, &lDevSn);
		//iRet = rf_pro_rst(m_hDev, (UCHAR*)m_szResetInfo);
		//iRet = rf_config_card(m_hDev, 'A');
		//iRet = rf_pro_reset(m_hDev, &chRecvLen, (UCHAR*)m_szResetInfo);
		//Asc2Hex((UCHAR*)m_szResetInfo, szRetInfoHex, chRecvLen);

		return iRet;
	}
	else if(iCardType == 2)
	{
		iRet = rf_cpu_rst(m_hDev, (unsigned char)9600, (UCHAR*)m_szCpuResetInfo);
		return iRet;
	}
	else if(iCardType == 3)
	{
		iRet = rf_sam_rst(m_hDev, (unsigned char)9600, (UCHAR*)m_szSamResetInfo);
		return iRet;
	}
	else
		return -1;

	return iRet;
}


int CRFReader::Hex2Asc(unsigned char* inHex, unsigned char* outAsc, long lPair_length)
{
	char src1,src2,factor1,factor2;
	long len;
	factor1 = '7';
	factor2 = '7';
    _strupr_s( (char *)inHex, lPair_length + 1 );

	for (len=0; len < lPair_length; len++) 
	{
		src1 = *(inHex+len*2);
		src2 = *(inHex+len*2+1);
		if ((src1>='0') && (src1<='9'))
		    factor1 = '0';
	    else if ((src1>='A') && (src1<='F'))
		    factor1 = '7';
	    else 
			return 1;
		if ((src2>='0') && (src2<='9'))
		    factor2 = '0';
	    else if ((src2>='A') && (src2<='F'))
		    factor2 = '7';
	    else 
			return 1;
		src1 = src1 - factor1;
		src2 = src2 - factor2;
		*outAsc++ = (src1 << 4) | src2; 
	}
	return 0;
}


int CRFReader::Asc2Hex(unsigned char* inAsc, unsigned char* outHex, long length)
{
	UCHAR hLowbit,hHighbit;
    long i;
	for(i=0;i<length*2;i=i+2)
	{
		hLowbit=inAsc[i/2]&0x0f;
		hHighbit=inAsc[i/2]/16;
		if(hHighbit>=10)
			outHex[i]=hHighbit+'7';
		else
			outHex[i]=hHighbit+'0';
		if(hLowbit>=10)
			outHex[i+1]=hLowbit+'7';
		else
			outHex[i+1]=hLowbit+'0';
	}
	outHex[length*2]='\0';

	return 0;
}



int CRFReader::ICC_SendCommand(char* sCommand, char* sRetST, char* sRetData)
{	
	char pCommand_APDU[512];
	unsigned char sendBuf[512],recvBuf[512],ascBuf[512],statusBuf[4], ascBufTemp[512];
	int iStrLen;
	long lRetST;
	//unsigned char chRecvLen;
//	short recvLen;

	memset(pCommand_APDU,0,512);

	iStrLen = (int)strlen(pCommand_APDU);
	memcpy(pCommand_APDU,sCommand, iStrLen);
	//加入机具相关头
	//AddHeadForReader(pCommand_APDU);

	memset(sendBuf, 0, 512);
	Hex2Asc((unsigned char*)pCommand_APDU,sendBuf,iStrLen);

	//sendBuf[iStrLen/2] = 0x00;
	//for(int i = 0; i < iStrLen/2; i ++)
	//{
	//	sendBuf[ iStrLen/2 ] = sendBuf[ iStrLen/2 ] ^ sendBuf[i];
	//}

	//
	memset(recvBuf, 0, 512);
#ifdef CONTACT
	lRetST = rf_cpuapdu(hDev, (UCHAR)iStrLen/2, sendBuf, &chRecvLen, recvBuf);
#else
	//lRetST = rf_pro_command(hDev, (UCHAR)iStrLen/2, sendBuf , &chRecvLen, recvBuf, 7);//////////
	lRetST = rf_pro_trn(m_hDev, sendBuf , recvBuf);
#endif
	//if(lRetST != 0)
	//	return lRetST;
	//lRetST = (long)chRecvLen;
	Asc2Hex(recvBuf, ascBufTemp, lRetST);
	sprintf_s((char*)ascBuf, 512, "%s", ascBufTemp);
	
	if(lRetST == 2)
	{
		sprintf_s((char*)statusBuf, 4, "%2.2s",ascBuf);

		if(!strcmp((char*)statusBuf,"61"))
		{
			memcpy(pCommand_APDU,"00C0000000", 10);

			//加入机具相关头
			//AddHeadForReader(pCommand_APDU);

			iStrLen = 10;;
			Hex2Asc((unsigned char*)pCommand_APDU,sendBuf,iStrLen);
			//for(int ii = 0; ii < iStrLen/2; ii ++)
			//{
			//	sendBuf[ iStrLen/2 ] = sendBuf[ iStrLen/2 ] ^ sendBuf[ii];
			//}
			memset(recvBuf, 0, 512);
#ifdef CONTACT
	lRetST = rf_cpuapdu(m_hDev, (UCHAR)iStrLen/2, sendBuf, &chRecvLen, recvBuf);
#else
	//lRetST = rf_pro_command(m_hDev, (UCHAR)iStrLen/2, sendBuf , &chRecvLen, recvBuf, 10);//////////
	lRetST = rf_pro_trn(m_hDev, sendBuf , recvBuf);
#endif
			//lRetST = chRecvLen;
			Asc2Hex(recvBuf, ascBuf, lRetST);

			sprintf_s((char*)sRetST, 4, "%4.4s",ascBuf + lRetST * 2 - 4);
			GetGivingString(sRetData,(char*)ascBuf,lRetST * 2 - 4);
		}
		else
		{
			sprintf_s(sRetST, 4, "%4.4s",ascBuf);
			if (!strcmp(sRetST, "9000"))
				return 9000;
			return -111;
		}

	}
	else
	{
		sprintf_s(sRetST, 4, "%4.4s",ascBuf + (lRetST) * 2 - 4);
		GetGivingString(sRetData,(char*)ascBuf,lRetST * 2 - 4);
		if (!strcmp(sRetST, "9000"))
				return 9000;
		return -111;
	}

	return 9000;
}

int CRFReader::ICS_SendCommand(char* sCommand, char* sRetST, char* sRetData)
{
	char pCommand_APDU[128];
	unsigned char sendBuf[128],recvBuf[512],ascBuf[512],statusBuf[4], ascBufTemp[512];
	int iStrLen;
	long lRetST;
//	short recvLen;

	memset(pCommand_APDU,0,128);

	iStrLen = (int)strlen(pCommand_APDU);
	memcpy(pCommand_APDU,sCommand, iStrLen);
	//加入机具相关头
	AddHeadForReader(pCommand_APDU);

	memset(sendBuf, 0, 128);
	Hex2Asc((unsigned char*)pCommand_APDU,sendBuf,iStrLen);
	sendBuf[iStrLen/2] = 0x00;
	for(int i = 0; i < iStrLen/2; i ++)
	{
		sendBuf[ iStrLen/2 ] = sendBuf[ iStrLen/2 ] ^ sendBuf[i];
	}

	lRetST = rf_cpu_trn(m_hDev, sendBuf ,recvBuf);
	if(lRetST != 0)
		return -1;
	lRetST = recvBuf[2];
	Asc2Hex(recvBuf, ascBufTemp, lRetST + 3);
	sprintf_s((char*)ascBuf, 500, "%s", ascBufTemp + 6);
	
	if(lRetST == 2)
	{
		sprintf_s((char*)statusBuf, 2, "%2.2s",ascBuf);

		if(!memcmp((char*)statusBuf,"61", 2))
		{
			memcpy(pCommand_APDU,"00C0000000", 10);

			//加入机具相关头
			AddHeadForReader(pCommand_APDU);

			iStrLen = (int)strlen(pCommand_APDU);
			Hex2Asc((unsigned char*)pCommand_APDU,sendBuf,iStrLen);
			for(int ii = 0; ii < iStrLen/2; ii ++)
			{
				sendBuf[ iStrLen/2 ] = sendBuf[ iStrLen/2 ] ^ sendBuf[ii];
			}
			lRetST = rf_cpu_trn(m_hDev, sendBuf, recvBuf);
			Asc2Hex(recvBuf,ascBuf,lRetST);

			sprintf_s((char*)sRetST, 4, "%4.4s",ascBuf + lRetST * 2 - 4);
			GetGivingString(sRetData,(char*)ascBuf,lRetST * 2 - 4);
		}
		else
		{
			sprintf_s(sRetST, 4, "%4.4s",ascBuf);
			if (!strcmp(sRetST, "9000"))
				return 9000;
			return -111;
		}

	}
	else
	{
		sprintf_s(sRetST, 4, "%4.4s",ascBuf + (lRetST) * 2 - 4);
		GetGivingString(sRetData,(char*)ascBuf,lRetST * 2 - 4);
		if (!strcmp(sRetST, "9000"))
				return 9000;
		return -111;
	}

	return 9000;
}


int CRFReader::AddHeadForReader(char* pszCommand)
{
	char szTemp[256] = {0};
	int iLen;

	iLen = strlen(pszCommand) /2;

	sprintf_s(szTemp, 200,  "%s%02x%s", "000000", iLen, pszCommand);

	sprintf_s(pszCommand,  200, "%s", szTemp);
	return 0;
}

int CRFReader::GetGivingString(char* pSource, char* pDest, int iLen)
{
	for(int i=0;i < iLen;i++)
	{
		pSource[i] = pDest[i];
	}
	return 0;
}


void CRFReader::Dev_GetHandle(HANDLE* hDev)
{
	if(m_hDev)
		*hDev = m_hDev;
	else
		hDev = NULL;
}


int CRFReader::Dev_GetDeviceVer(char* pszDeviceSN)
{
	//rf_lib_ver((UCHAR*)pszDeviceSN);
	return 0;
}



int CRFReader::Anthentication(unsigned char keyMode, unsigned char szSectorNo)
{
	int iRet;
	iRet = rf_authentication(m_hDev, keyMode, szSectorNo);
	return (iRet);
}


// 16进制 写Mifare one, 是否16进制写
int CRFReader::Write(unsigned char chAddr, unsigned char* pszData, int iMode)
{
	if(iMode == HEX)
		return (rf_write_hex(m_hDev, chAddr, (char*)pszData));
	else
		return (rf_write(m_hDev, chAddr, pszData));
}


int CRFReader::Increment(unsigned char chAddr, unsigned long lValue)
{
	return (rf_increment(m_hDev, chAddr, lValue));
}


int CRFReader::Decrement(unsigned char chAddr, unsigned long lValue)
{
	return (rf_decrement(m_hDev, chAddr, lValue));
}



int CRFReader::InitialValue(unsigned char chAddr, unsigned long lValue)
{
	return (rf_initval(m_hDev, chAddr, lValue));
}


// 重置
int CRFReader::DevReset(int iMs)
{
	return (rf_reset(m_hDev, iMs));
}


// mode=0 一次一张 1多张
int CRFReader::RequestCard(unsigned char chMode, unsigned long* lSnr)
{
	return (rf_card(m_hDev, chMode, lSnr));
}



int CRFReader::SwitchDataMode(unsigned char* pszData, int* iLen, int iMode)
{
	unsigned char szTempData[1024];
	int length;
	length = *iLen;
	memset(szTempData, 0, 1024);
	if(iMode == HEX2ASC)
	{
		Hex2Asc(pszData, szTempData, *iLen);
		length = *iLen /2;
		memcpy(pszData, szTempData, length);
		pszData[length] = 0;
		*iLen = length;
	}
	else if(iMode == ASC2HEX)
	{
		Asc2Hex(pszData, szTempData, *iLen);
		length = *iLen * 2;
		memcpy(pszData, szTempData, length);
		pszData[length] = 0;
		*iLen = length;
	}
	return 0;
}


// Mifare one
int CRFReader::LoadKey(unsigned char chKeyMode, unsigned char chSectorNo, unsigned char* szKey, int iMode)
{
	if(iMode == HEX)
		return (rf_load_key_hex(m_hDev, chKeyMode, chSectorNo, (char*)szKey));
	else
		return (rf_load_key(m_hDev, chKeyMode, chSectorNo, szKey));
}

//iMode:是否16进制读
int CRFReader::ReadMifareBlock(unsigned char chBlock, unsigned char* pszData, int iMode)
{
	if(iMode == HEX)
		return (rf_read_hex(m_hDev, chBlock, (char*)pszData));
	else
		return (rf_read(m_hDev, chBlock, pszData));
}


int CRFReader::Dev_Beep(int iMsc)
{
	return rf_beep(m_hDev, (unsigned short)iMsc);
}


// 读余额
int CRFReader::ReadValue(unsigned char chAddr, unsigned long* lValue)
{
	return rf_readval(m_hDev, chAddr, lValue);
}

// 十六进制数据转换成10进制
int CRFReader::Hex2Dec(unsigned char* HexNum)
{
	int iLen = 0;
	int iNum[16];
	int iCount = 1;
	int iResult = 0;

	iLen = strlen((char*)HexNum);
	for(int i = iLen - 1; i >= 0; i --)
	{
		if ((HexNum[i]>='0') && (HexNum[i]<='9'))  
			iNum[i]	= HexNum[i] - 48;//字符0的ASCII值为48
		else if ((HexNum[i] >= 'a') && (HexNum[i] <= 'f'))  
			iNum[i] = HexNum[i] - 'a' +10;  
		else if ((HexNum[i]>='A') && (HexNum[i]<='F'))  
			iNum[i] = HexNum[i] - 'A' + 10;  
		else
			iNum[i]=0;
		iResult = iResult + iNum[i] * iCount;  
		iCount = iCount * 16;//十六进制(如果是八进制就在这里乘以8)    
	}
	return iResult;
}
