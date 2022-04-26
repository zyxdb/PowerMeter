#pragma once
#include "stdafx.h"
#include "Commons.h"


Cport::Cport()
{
 	overloop=false;
	handle=NULL;
	m_bRunning = FALSE;
	m_bSetting = FALSE;
	m_bReading = FALSE;
}

Cport::~Cport()
{
}
//打开串口
BOOL Cport::OpenPort(LPCWSTR pcCommName,bool OVERLOOP)
{
	//同步或者异步传输(OVERLOOP为NULL时，同步传输；FILE_FLAG_OVERLAPPED为异步传输)
	DWORD style;
	if (OVERLOOP){
		overloop = true;
		style = FILE_FLAG_OVERLAPPED;
	}
	else{
		overloop = false;
		style = NULL;
	}
	//打开串口
	handle = CreateFile(pcCommName,GENERIC_READ|GENERIC_WRITE,0,NULL,OPEN_EXISTING,style,NULL);
	if (handle==INVALID_HANDLE_VALUE){
		return false;
	}
	GetCommState(handle,&Dcb);
	m_bRunning = TRUE;
	return true;
}
//关闭串口
BOOL Cport::ClosePort()
{
	bool value=false;
	m_bRunning = FALSE;
	if (handle&&handle!=INVALID_HANDLE_VALUE)
		value=CloseHandle(handle);
	Sleep(300);
	return value;
}

//设置DCB（Device Control Block）
BOOL Cport::SetPortParm(int BaudRate,int bytesize,int parity,int stopbits)
{
	//设置波特率
	m_bSetting = TRUE;
	int i;
	for (i = 0; i<10; i++)
	{
		if (m_bReading)
		{
			ClearInputBuffer();
			Sleep(300);
		}
		else break;
	}
	if (i >= 10) return FALSE;

	Dcb.BaudRate=BaudRate;
	Dcb.ByteSize=bytesize;
    Dcb.Parity=parity;
    Dcb.StopBits=stopbits;
	if(SetCommState(handle,&Dcb)){
		SetupComm(handle,1024,1024);
		COMMTIMEOUTS TimeOuts; //设定读超时
		TimeOuts.ReadIntervalTimeout=100;/* Maximum time between read chars. */
		TimeOuts.ReadTotalTimeoutMultiplier=10;/* Multiplier of characters.        */
		TimeOuts.ReadTotalTimeoutConstant=300; /* Constant in milliseconds.        */
		TimeOuts.WriteTotalTimeoutMultiplier=7;/* Multiplier of characters.        */
		TimeOuts.WriteTotalTimeoutConstant=300; /* Constant in milliseconds.        */
		SetCommTimeouts(handle,&TimeOuts); //设置超时
		m_bSetting = FALSE;
		return true;
	}
	else
	{
		m_bSetting = FALSE;
		return false;
	}
}
  

//发送数据
LONG Cport::SendData(char* pOutpuData,DWORD dwSize)
{
	if(!handle) return -1;
	DWORD dwErr;
	COMSTAT cs;
	ClearCommError(handle,&dwErr,&cs);
	static	OVERLAPPED ov;
	if (!overloop){
		if(WriteFile(handle,pOutpuData,dwSize,&dwSize,NULL))
		{
			return dwSize;
		}
		else{
			PurgeComm(handle, PURGE_TXABORT| PURGE_RXABORT|PURGE_TXCLEAR|PURGE_RXCLEAR);
			return 0;
		}
	}
	else{
		ov.hEvent=CreateEvent(NULL,TRUE,FALSE,L"eCommSend");

		if(WriteFile(handle,pOutpuData,dwSize,&dwSize,&ov))
		{
			return dwSize;
		}
		else
		{
			if(ERROR_IO_PENDING==(dwErr=GetLastError()))
			{
				WaitForSingleObject(ov.hEvent, INFINITE);
				ResetEvent(ov.hEvent);
				return ov.InternalHigh;
			}
		}
		return 0;
	}
}

//接收数据
LONG Cport::RecData(char* pOutpuData,int size,DWORD* num)
{
	DWORD	nByteRead=size;
	DWORD	dwEvent,dwError;
	COMSTAT cs;
	static	OVERLAPPED ov;
	if (!overloop){
		if(ReadFile(handle,pOutpuData,size,num,NULL)){
			return *num;
		}
		else{
			PurgeComm(handle, PURGE_TXABORT| PURGE_RXABORT|PURGE_TXCLEAR|PURGE_RXCLEAR);
			ClearCommError(handle,&dwError,&cs);
			return 0;
		}
	}
	else{

		DWORD dwErr;
		//ReadFile(handle,pOutpuData,size,&nByteRead,&ov);
		ReadFile(handle,pOutpuData,nByteRead,&nByteRead,&ov);
		if (!nByteRead)
		{
			if(ERROR_IO_PENDING==(dwErr=GetLastError()))
			{
				ClearCommError(handle,&dwError,&cs);
				WaitForSingleObject(ov.hEvent, INFINITE);
				ResetEvent(ov.hEvent);
				return nByteRead;
			}
		}
		else  return nByteRead;
		ClearCommError(handle,&dwError,&cs);
		return 0;
	}
}


//接收数据
COMSTAT Cport::ClearInputBuffer()
{
	DWORD	dwError;
	COMSTAT cs;
	PurgeComm(handle,PURGE_RXABORT |PURGE_RXCLEAR);
	ClearCommError(handle, &dwError, &cs);	
	return cs;
}

BOOL Cport::SetFlowControl(DWORD dwType)//0:NONE  1:xON/xOff  2：Haware
{
	//设置波特率
	m_bSetting = TRUE;
	int i;
	for (i = 0; i < 10; i++)
	{
		if (m_bReading)
		{
			ClearInputBuffer();
			Sleep(300);
		}
		else break;
	}
	if (i >= 10) return FALSE;


	Dcb.fOutxCtsFlow = 0;
	Dcb.fRtsControl = 0;
	Dcb.fInX = 0;
	Dcb.fOutX = 0;
	switch (dwType)
	{
		case 2://2：Haware
			Dcb.fOutxCtsFlow = 1;
			Dcb.fRtsControl = 2;
			break;
		case 1://1:xON/xOff  
			Dcb.fInX = 1;
			Dcb.fOutX = 1;
			break;
		case 0://0:NONE  
		default:
			break;
	}

	if (SetCommState(handle, &Dcb)) 
	{
		m_bSetting = FALSE;
		return true;
	}
	else
	{
		m_bSetting = FALSE;
		return false;
	}
}
