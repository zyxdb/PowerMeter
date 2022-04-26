#pragma once
typedef void(*CallbackFunction)(PCHAR  pcData, DWORD  dwDataSize);
PVOID psConnectDevice(DWORD  dwComm, DWORD  dwFrameSize, CallbackFunction callbackFunc = NULL);
BOOL  psDisconnectDevice(PVOID pHandle);
LONG  psWriteData(PVOID pHandle, PCHAR  pcData, DWORD  dwDataSize);
BOOL  psSetPort(PVOID pHandle, int BaudRate = CBR_9600, int bytesize = 8, int parity = NOPARITY, int stopbits = ONESTOPBIT);
void  MeasureThread(LPVOID lpParameter);
void SelfCheckThread(LPVOID lpParameter);
DWORD CreateID();
BOOL bEleBridgeCheck();
BOOL bMultiMeterCheck(CPowerMeterDlg *pdlgMain);
BOOL bCurrentSourceCheck(CPowerMeterDlg *pdlgMain);

bool m_bQJ58Connect = false;//���ſ���
bool m_bMultimeterConnect = false;//���ñ���
bool m_bCurrentSourceConnect = false;//���ñ���

int  m_iDev;//����
Cport	m_cComPortCurrentSource;//���ܵ���Դ
Cport	m_cComPortMultimeter;//���ñ�