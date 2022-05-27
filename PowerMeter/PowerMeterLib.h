#pragma once
#include "PowerMeterDlg.h"

typedef void(*CallbackFunction)(PCHAR  pcData, DWORD  dwDataSize);
//PVOID psConnectDevice(DWORD  dwComm, DWORD  dwFrameSize, CallbackFunction callbackFunc = NULL);
BOOL  psDisconnectDevice(PVOID pHandle);
LONG  psWriteData(PVOID pHandle, PCHAR  pcData, DWORD  dwDataSize);
BOOL  psSetPort(PVOID pHandle, int BaudRate = CBR_9600, int bytesize = 8, int parity = NOPARITY, int stopbits = ONESTOPBIT);
void  MeasureThread(LPVOID lpParameter);
void SelfCheckThread(LPVOID lpParameter);
DWORD CreateID();

bool m_bQJ58Connect = false;//电桥开关
bool m_bMultimeterConnect = false;//万用表开关
bool m_bCurrentSourceConnect = false;//精密电流源开关
bool m_bShutterConnect = false;//快门开关
bool m_bExtCtrlConnect = false;//额外控制开关
bool m_bPDAConnect = false;//二极管开关

int  m_iDev;//电桥
Cport	m_cComPortCurrentSource;//精密电流源
Cport	m_cComPortMultimeter;//万用表
Cport	m_cComPortShutter;//快门
Cport	m_cComPortExtCtrl;//额外控制
Cport	m_cComPortPDA;//二极管

BOOL bEleBridgeCheck();
BOOL bMultiMeterCheck(CPowerMeterDlg *pdlgMain);
BOOL bCurrentSourceCheck(CPowerMeterDlg *pdlgMain);
BOOL bShutterCheck(CPowerMeterDlg *pdlgMain);

double temperatureMeasure(CPowerMeterDlg *pdlgMain);
double VoltageMeasure(CPowerMeterDlg *pdlgMain);