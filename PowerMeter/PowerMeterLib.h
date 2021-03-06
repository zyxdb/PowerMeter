#ifndef POWERMETERLIB_H
#define POWERMETERLIB_H

#pragma once


typedef void(*CallbackFunction)(PCHAR  pcData, DWORD  dwDataSize);
//PVOID psConnectDevice(DWORD  dwComm, DWORD  dwFrameSize, CallbackFunction callbackFunc = NULL);
BOOL  psDisconnectDevice(PVOID pHandle);
LONG  psWriteData(PVOID pHandle, PCHAR  pcData, DWORD  dwDataSize);
BOOL  psSetPort(PVOID pHandle, int BaudRate = CBR_9600, int bytesize = 8, int parity = NOPARITY, int stopbits = ONESTOPBIT);
void  MeasureThread(LPVOID lpParameter);
void  SelfCheckThread(LPVOID lpParameter);
DWORD CreateID();
bool SetCurrent(CPowerMeterDlg *pdlgMain, double measureCurrent,bool switchState);
bool SetShutter(CPowerMeterDlg *pdlgMain, bool switchState);

#endif