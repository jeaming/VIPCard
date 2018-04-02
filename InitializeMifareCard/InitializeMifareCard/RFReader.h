#pragma once
class CRFReader
{
public:
	CRFReader(void);
	~CRFReader(void);
	int Dev_Connect(int iPort, long lBaud);
	bool m_isConnected;
	HANDLE m_hDev;
	int Dev_Close(void);
	int Dev_Reset(int iCardType);
	char m_szResetInfo[128];
	unsigned char m_szCpuResetInfo[128];
	unsigned char m_szProResetInfo[128];
	unsigned char m_szSamResetInfo[128];
	int Hex2Asc(unsigned char* inHex, unsigned char* outAsc, long lPair_length);
	int Asc2Hex(unsigned char* inAsc, unsigned char* outHex, long length);
	int ICC_SendCommand(char* sCommand, char* sRetST, char* sRetData);	
	int ICS_SendCommand(char* sCommand, char* sRetST, char* sRetData);
	int AddHeadForReader(char* pszCommand);
	int GetGivingString(char* pSource, char* pDest, int iLen);
	void Dev_GetHandle(HANDLE* hDev);
	int Dev_GetDeviceVer(char* pszDeviceSN);
	int Anthentication(unsigned char keyMode, unsigned char szSectorNo);
	// 16进制 写Mifare one
	int Write(unsigned char chAddr, unsigned char* pszData, int iMode);
	int Increment(unsigned char chAddr, unsigned long lValue);
	int Decrement(unsigned char chAddr, unsigned long lValue);
	int InitialValue(unsigned char chAddr, unsigned long lValue);
	// 重置
	int DevReset(int iMs);
	// mode=0 一次一张 1多张
	int RequestCard(unsigned char chMode, unsigned long* lSnr);
	int SwitchDataMode(unsigned char* pszData, int* iLen, int iMode);
	enum DataType
	{
		HEX2ASC = 0,
		ASC2HEX,
		ASC = 10,
		HEX
	};
	enum KEY_TYPE
	{
		KEY_L = 0,
		KEY_R = 4
	};
	// Mifare one
	int LoadKey(unsigned char chKeyMode, unsigned char chSectorNo, unsigned char* szKey, int iMode);
	int ReadMifareBlock(unsigned char chBlock, unsigned char* pszData, int iMode);
	int Dev_Beep(int iMsc);
	int Dev_CardHalt(void);
};