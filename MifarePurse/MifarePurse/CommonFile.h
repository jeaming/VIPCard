#include "stdafx.h"

struct CardData
{
	unsigned char szCardNO[20];
	unsigned char szVIPCardNO[20];
	unsigned char szName[64];
	unsigned char szPhoneNO[15];
	unsigned char szBalance[20];
	int iAmount;
	int iProjectsCount;
	int iFlag;
	BOOL bIsUsed;
}*PCARD_DATA, CARD_DATA;

struct ProjectData
{
	unsigned char szProjectIndex[5];
	int iDecTimes;
	int iAddTimes;
	int iTimes;
	int iProjectIndex;
	unsigned char szProjectName[30];
}PROJECT_DATA;

int GetFileAbsluteAddress(char* pszFileName, char* pszFileFullPath);
int WriteLog(CString strLog);