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

bool m_bQJ58Connect = false;//电桥开关
bool m_bMultimeterConnect = false;//万用表开关
bool m_bCurrentSourceConnect = false;//万用表开关

int  m_iDev;//电桥
Cport	m_cComPortCurrentSource;//精密电流源
Cport	m_cComPortMultimeter;//万用表