#include "stdafx.h"

struct CardData
{
	unsigned char szCardNO[20];
	unsigned char szVIPCardNO[20];
	unsigned char szName[64];
	unsigned char szPhoneNO[15];
	unsigned char szBalance[20];
	unsigned char szProject1[30];
	unsigned char szProject2[30];
	unsigned char szProject3[30];
	int iAmount;
	int iProjectTimes1;
	int iProjectTimes2;
	int iProjectTimes3;
	BOOL bIsUsed;
}*PCARD_DATA, CARD_DATA;

struct ProjectData
{
	unsigned char szProjectIndex[5];
	int iTimes;
	int iAddTimes;
	int iProjectIndex;
	unsigned char szProjectName[30];
}PROJECT_DATA;

int GetFileAbsluteAddress(char* pszFileName, char* pszFileFullPath);
int WriteLog(CString strLog);
int WriteInitLog(CString strLog);