#include "stdafx.h"
#include "DES.h"
#include "FileControl.h"

#define MAIN_KEY		"12345678900987654321ABCDEFFEDCBA"
#define		INI_FILE	"Set.dat"



int Hex2Asc(unsigned char* inHex, unsigned char* outAsc, long lPair_length)
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

int Asc2Hex(unsigned char* inAsc, unsigned char* outHex, long length)
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


 extern int GetKeyData(unsigned char* pszKeyData)
{
	CFile file;
	CString strTemp;
	CDES des;
	unsigned char szDesKey[40], szCardKey[512];
	int iKeyLen, iBlock = 0;
	int iDataLen = 0;

	file.Open(INI_FILE, CFile::modeRead);
	iKeyLen = file.Read(strTemp.GetBuffer(), 400);
	if(iKeyLen < 0)
	{
		return -1;
	}
	file.Close();



	//µÃµ½ÉÈÇøÃÜÔ¿
	memset(szDesKey, 0, 40);
	memset(szCardKey, 0, 512);
	Hex2Asc((unsigned char*)MAIN_KEY, szDesKey, 32);
	des.RunDes(des.DECRYPT, des.ECB, strTemp.GetBuffer(), (char*)szCardKey, iKeyLen, (char*)szDesKey, 16);
	Asc2Hex(szCardKey, pszKeyData, iKeyLen);
	iKeyLen = iKeyLen*2;

	pszKeyData[iKeyLen] = 0;

	return 0;
}