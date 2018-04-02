#include "stdafx.h"
#ifndef	COMM_H
#include "CommonFile.h"
#define COMM_H
#endif



int GetFileAbsluteAddress(char* pszFileName, char* pszFileFullPath)
{
	//time_t t0 = time(0);
	char szHome[200], szFilePath[200], szDriver[10], szPath[100], szFile[60], szExt[20], szTime[30];
	//char szFileDirectory[200];

	memset(szFilePath, 0, 200);
	memset(szHome, 0, 200);
	memset(szDriver, 0, 10);
	memset(szPath, 0, 100);
	memset(szFile, 0, 60);
	memset(szExt, 0, 20);
	memset(szTime, 0, 30);

	//strftime(szTime, 30, "%Y%m%d", localtime(&t0));
	//memcpy(szTime + 8, ".TXT", 4);

	::GetModuleFileNameA(AfxGetInstanceHandle(), (LPSTR)szHome, 200);
	_splitpath_s((char*)szHome, szDriver, szPath, szFile, szExt);

	sprintf_s(pszFileFullPath, 200, "%s%s\\%s", szDriver, szPath, pszFileName);

	////\LOG_C.TXT
	//memset(szFileDirectory, 0, 200);
	//sprintf_s((char*)szFileDirectory, 200, "%s%s\\LOG", szDriver, szPath);

	//sprintf_s((char*)szFilePath, 200, "%s%s\\LOG\\%s", szDriver, szPath, szTime);
	//if(::GetFileAttributesA(szFilePath) == 0xFFFFFFFF)
	//{
	//	CreateDirectoryA(szFileDirectory, NULL);
	//}

	return 0;
}

int WriteLog(CString strLog)
{
	CFile file;
	CStringA strTemp;
	time_t t0 = time(0);
	char szHome[200], szFilePath[200], szDriver[10], szPath[100], szFile[60], szExt[20], szTime[30];
	char szFileDirectory[200], szTiming[20];

	memset(szFilePath, 0, 200);
	memset(szHome, 0, 200);
	memset(szDriver, 0, 10);
	memset(szPath, 0, 100);
	memset(szFile, 0, 60);
	memset(szExt, 0, 20);
	memset(szTime, 0, 30);
	memset(szTiming, 0, 20);

	strftime(szTime, 30, "%Y%m%d", localtime(&t0));
	strftime(szTiming, 20, "%H:%M:%S ||", localtime(&t0));
	/*memcpy(szTime + 8, ".TXT", 4);*/

	::GetModuleFileNameA(AfxGetInstanceHandle(), (LPSTR)szHome, 200);
	_splitpath_s(szHome, szDriver, szPath, szFile, szExt);
	//\LOG_C.TXT
	memset(szFileDirectory, 0, 200);
	sprintf_s(szFileDirectory, 200, "%s%s\\LOG", szDriver, szPath);
	if(::GetFileAttributesA(szFileDirectory) == 0xFFFFFFFF)
	{
		CreateDirectoryA(szFileDirectory, NULL);
	}

	sprintf_s(szFileDirectory, 200, "%s%s\\LOG\\%s", szDriver, szPath, szTime);
	if(::GetFileAttributesA(szFileDirectory) == 0xFFFFFFFF)
	{
		CreateDirectoryA(szFileDirectory, NULL);
	}
	sprintf_s(szFilePath, 200, "%s%s\\LOG\\%s\\充值.TXT", szDriver, szPath, szTime);
	if(!file.Open((LPCTSTR)CString(szFilePath), CFile::modeCreate | CFile::modeReadWrite | CFile::modeNoTruncate))
		return 1;

	strTemp = "";
	strTemp = strTemp + strLog + "\r\n";

	file.SeekToEnd();
	file.Write(szTiming, 11);
	file.Write(strTemp, strTemp.GetLength());
	file.Close();

	sprintf_s(szFileDirectory, 200, "C:\\WINDOWS\\LOG", szDriver, szPath);
	if(::GetFileAttributesA(szFileDirectory) == 0xFFFFFFFF)
	{
		CreateDirectoryA(szFileDirectory, NULL);
	}

	sprintf_s(szFileDirectory, 200, "C:\\WINDOWS\\LOG\\%s", szTime);
	if(::GetFileAttributesA(szFileDirectory) == 0xFFFFFFFF)
	{
		CreateDirectoryA(szFileDirectory, NULL);
	}

	sprintf_s(szFilePath, 200, "C:\\WINDOWS\\LOG\\%s\\充值.TXT", szTime);
	if(!file.Open((LPCTSTR)CString(szFilePath), CFile::modeCreate | CFile::modeReadWrite | CFile::modeNoTruncate))
		return 1;

	file.SeekToEnd();
	file.Write(szTiming, 11);
	file.Write(strTemp, strTemp.GetLength());
	file.Close();
	return 0;
}

int WriteInitLog(CString strLog)
{
	CFile file;
	CStringA strTemp;
	time_t t0 = time(0);
	char szHome[200], szFilePath[200], szDriver[10], szPath[100], szFile[60], szExt[20], szTime[30];
	char szFileDirectory[200], szTiming[20];

	memset(szFilePath, 0, 200);
	memset(szHome, 0, 200);
	memset(szDriver, 0, 10);
	memset(szPath, 0, 100);
	memset(szFile, 0, 60);
	memset(szExt, 0, 20);
	memset(szTime, 0, 30);
	memset(szTiming, 0, 20);

	strftime(szTime, 30, "%Y%m%d", localtime(&t0));
	strftime(szTiming, 20, "%H:%M:%S ||", localtime(&t0));
	//memcpy(szTime + 8, ".TXT", 4);

	::GetModuleFileNameA(AfxGetInstanceHandle(), (LPSTR)szHome, 200);
	_splitpath_s(szHome, szDriver, szPath, szFile, szExt);
	//\LOG_C.TXT
	memset(szFileDirectory, 0, 200);
	sprintf_s(szFileDirectory, 200, "%s%s\\LOG", szDriver, szPath);
	if(::GetFileAttributesA(szFileDirectory) == 0xFFFFFFFF)
	{
		CreateDirectoryA(szFileDirectory, NULL);
	}
	
	sprintf_s(szFileDirectory, 200, "%s%s\\LOG\\%s", szDriver, szPath, szTime);
	if(::GetFileAttributesA(szFileDirectory) == 0xFFFFFFFF)
	{
		CreateDirectoryA(szFileDirectory, NULL);
	}

	sprintf_s(szFilePath, 200, "%s%s\\LOG\\%s\\个人化.TXT", szDriver, szPath, szTime);


	if(!file.Open((LPCTSTR)CString(szFilePath), CFile::modeCreate | CFile::modeReadWrite | CFile::modeNoTruncate))
		return 1;

	strTemp = "";
	strTemp = strTemp + strLog + "\r\n";

	file.SeekToEnd();
	file.Write(szTiming, 11);
	file.Write(strTemp, strTemp.GetLength());
	file.Close();


	sprintf_s(szFileDirectory, 200, "C:\\WINDOWS\\LOG", szDriver, szPath);
	if(::GetFileAttributesA(szFileDirectory) == 0xFFFFFFFF)
	{
		CreateDirectoryA(szFileDirectory, NULL);
	}

	sprintf_s(szFileDirectory, 200, "C:\\WINDOWS\\LOG\\%s", szTime);
	if(::GetFileAttributesA(szFileDirectory) == 0xFFFFFFFF)
	{
		CreateDirectoryA(szFileDirectory, NULL);
	}

	sprintf_s(szFilePath, 200, "C:\\WINDOWS\\LOG\\%s\\个人化.TXT", szTime);
	if(!file.Open((LPCTSTR)CString(szFilePath), CFile::modeCreate | CFile::modeReadWrite | CFile::modeNoTruncate))
		return 1;

	file.SeekToEnd();
	file.Write(szTiming, 11);
	file.Write(strTemp, strTemp.GetLength());
	file.Close();
	return 0;
}
