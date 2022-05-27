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

bool m_bQJ58Connect = false;//���ſ���
bool m_bMultimeterConnect = false;//���ñ���
bool m_bCurrentSourceConnect = false;//���ܵ���Դ����
bool m_bShutterConnect = false;//���ſ���
bool m_bExtCtrlConnect = false;//������ƿ���
bool m_bPDAConnect = false;//�����ܿ���

int  m_iDev;//����
Cport	m_cComPortCurrentSource;//���ܵ���Դ
Cport	m_cComPortMultimeter;//���ñ�
Cport	m_cComPortShutter;//����
Cport	m_cComPortExtCtrl;//�������
Cport	m_cComPortPDA;//������

BOOL bEleBridgeCheck();
BOOL bMultiMeterCheck(CPowerMeterDlg *pdlgMain);
BOOL bCurrentSourceCheck(CPowerMeterDlg *pdlgMain);
BOOL bShutterCheck(CPowerMeterDlg *pdlgMain);

double temperatureMeasure(CPowerMeterDlg *pdlgMain);
double VoltageMeasure(CPowerMeterDlg *pdlgMain);