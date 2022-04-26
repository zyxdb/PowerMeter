#include "stdafx.h"
#include "windows.h"
#include "PowerMeter.h"
#include "commons.h"
#include "PowerMeterLib.h"
#include "PowerMeterDlg.h"

void SelfCheckThread(LPVOID lpParameter)
{
	CPowerMeterDlg *pdlgMain = (CPowerMeterDlg *)lpParameter;
	
	do {
		if (pdlgMain->m_iSelfChecking != 1) break;;
		TRACE("����ͨ������\n");
		pdlgMain->GetDlgItem(IDC_STATIC_STATUS)->SetWindowText(_T("����ͨ������"));
		pdlgMain->GetDlgItem(IDC_STATIC_RESULT)->SetWindowText(_T("����ͨ������"));
		Sleep(2000);
		if (pdlgMain->m_iSelfChecking != 1) break;;
		TRACE("���ñ�ͨ������\n");
		pdlgMain->GetDlgItem(IDC_STATIC_STATUS)->SetWindowText(_T("���ñ�ͨ������"));
		pdlgMain->GetDlgItem(IDC_STATIC_RESULT)->SetWindowText(_T("���ñ�ͨ������"));
		Sleep(2000);
		if (pdlgMain->m_iSelfChecking != 1) break;;
		TRACE("����ͨ������\n");
		pdlgMain->GetDlgItem(IDC_STATIC_STATUS)->SetWindowText(_T("����ͨ������"));
		pdlgMain->GetDlgItem(IDC_STATIC_RESULT)->SetWindowText(_T("����ͨ������"));
		Sleep(2000);
		if (pdlgMain->m_iSelfChecking != 1) break;;
		TRACE("�ⲿЭ���˿�����\n");
		pdlgMain->GetDlgItem(IDC_STATIC_STATUS)->SetWindowText(_T("�ⲿЭ���˿�����"));
		pdlgMain->GetDlgItem(IDC_STATIC_RESULT)->SetWindowText(_T("�ⲿЭ���˿�����"));
		Sleep(2000);
		if (pdlgMain->m_iSelfChecking != 1) break;;
		TRACE("PSDͨ������\n");
		pdlgMain->GetDlgItem(IDC_STATIC_STATUS)->SetWindowText(_T("PSDͨ������"));
		pdlgMain->GetDlgItem(IDC_STATIC_RESULT)->SetWindowText(_T("PSDͨ������"));
		Sleep(2000);
		pdlgMain->PostMessage(WM_THREAD, THREAD_SELFCHECK_STATUS, THREAD_STATE_SUCCESS);
	} while (0);
	if (pdlgMain->m_iSelfChecking != 1) pdlgMain->PostMessage(WM_THREAD, THREAD_SELFCHECK_STATUS, THREAD_STATE_TERMINATED);
}

MeasureData gMeasureData[4];
MeasureData WaitforStable(CPowerMeterDlg *pdlgMain,DWORD dwStart,BOOL bElectricEnable=FALSE)
{
	CString  csString;
	SYSTEMTIME sysTime;
	LPARAM  lParam = LPARAM(THREAD_STATE_SUCCESS);
	double dbPidStability = 1, dbHeaterStability = 1;
	DWORD dwTimeBegin, dwTimePeriod;
	dwTimeBegin = GetTickCount();
	while (pdlgMain->m_bInMesuring)
	{
		TRACE(_T("�������ѹ����"));
		pdlgMain->GetDlgItem(IDC_STATIC_STATUS)->SetWindowText(_T("�������ѹ����"));
		pdlgMain->GetDlgItem(IDC_STATIC_RESULT)->SetWindowText(_T("�������ѹ����"));
		GetLocalTime(&sysTime);
		gMeasureData[0].bFlag = (1 << LineSerie_Temperature) |
			(1 << LineSerie_Voltage) |
			(1 << LineSerie_OutputCurrent);
		gMeasureData[0].dwIdex = GetTickCount() / 100 - dwStart;
		gMeasureData[0].dbTemperature = rand();
		gMeasureData[0].dbVoltage = rand();
		gMeasureData[0].dbOutputCurrent = pdlgMain->m_dbOutputCurrent;
		gMeasureData[0].bState = 0;
		pdlgMain->PostMessage(WM_THREAD, THREAD_MEASURE_DATA, LPARAM(&(gMeasureData[0])));

		if (bElectricEnable)
		{
			dwTimePeriod = GetTickCount() - dwTimeBegin;
			if (dwTimePeriod > pdlgMain->m_dbPidStableTime)
			{
				//dbPidStability=?
				TRACE(_T("��������ȶ���"));
				pdlgMain->GetDlgItem(IDC_STATIC_STATUS)->SetWindowText(_T("��������ȶ���"));
				pdlgMain->GetDlgItem(IDC_STATIC_RESULT)->SetWindowText(_T("��������ȶ���"));
				if (dbPidStability < pdlgMain->m_dbPidStability)
				{
					//pdlgMain->m_dbOutputCurrent=??
					TRACE(_T("����������������"));
					pdlgMain->GetDlgItem(IDC_STATIC_STATUS)->SetWindowText(_T("����������������"));
					pdlgMain->GetDlgItem(IDC_STATIC_RESULT)->SetWindowText(_T("����������������"));
					continue;
				}
			}
		}

		dwTimePeriod = GetTickCount() - dwTimeBegin;
		if (dwTimePeriod > pdlgMain->m_dbHeaterStableTime)
		{
			//dbHeaterStability=?
			TRACE(_T("�������ǻ�ȶ���"));
			pdlgMain->GetDlgItem(IDC_STATIC_STATUS)->SetWindowText(_T("�������ǻ�ȶ���"));
			pdlgMain->GetDlgItem(IDC_STATIC_RESULT)->SetWindowText(_T("�������ǻ�ȶ���"));
			if (dbHeaterStability < pdlgMain->m_dbHeaterStability)
			{
				TRACE(_T("�ȴ�����ǻ�ȶ�"));
				pdlgMain->GetDlgItem(IDC_STATIC_STATUS)->SetWindowText(_T("�ȴ�����ǻ�ȶ�"));
				pdlgMain->GetDlgItem(IDC_STATIC_RESULT)->SetWindowText(_T("�ȴ�����ǻ�ȶ�"));
				continue;
			}
			else break;
		}
	}
	return gMeasureData[0];
}

void MeasureThread(LPVOID lpParameter)
{
	CPowerMeterDlg *pdlgMain = (CPowerMeterDlg *)lpParameter;
	CString  csString;
	LPARAM  lParam = LPARAM(THREAD_STATE_SUCCESS);
	double dbPidStability=1,dbHeaterStability=1;
	DWORD dwStart=GetTickCount()/100;
	double dbLigheTemperature, dbHeaterTemperatureH, dbHeaterTemperatureL, dbHeaterVoltageH, dbHeaterVoltageL;
	srand(dwStart);
	while(pdlgMain->m_bInMesuring)
	{
		TRACE("Ԥ�Ȳ���ǻ\n");
		pdlgMain->GetDlgItem(IDC_STATIC_STATUS)->SetWindowText(_T("Ԥ�Ȳ���ǻ"));
		pdlgMain->GetDlgItem(IDC_STATIC_RESULT)->SetWindowText(_T("Ԥ�Ȳ���ǻ"));
		Sleep(500);	
		if (pdlgMain->m_dbHeaterResHeat <= 0)
		{
			lParam = LPARAM(THREAD_STATE_PARAMS);
			break;
		}
		pdlgMain->m_dbOutputCurrent = sqrt(pdlgMain->m_dbLightPowerEstimate / pdlgMain->m_dbHeaterResHeat);
		csString.Format(_T("������� %f"), pdlgMain->m_dbOutputCurrent);
		TRACE(csString);
		pdlgMain->GetDlgItem(IDC_STATIC_STATUS)->SetWindowText(csString);
		pdlgMain->GetDlgItem(IDC_STATIC_RESULT)->SetWindowText(csString);
		WaitforStable(pdlgMain, dwStart,TRUE);


		TRACE("��ȥ�繦�������Ų����⹦\n");
		pdlgMain->GetDlgItem(IDC_STATIC_STATUS)->SetWindowText(_T("�������Ų����⹦"));
		pdlgMain->GetDlgItem(IDC_STATIC_RESULT)->SetWindowText(_T("�������Ų����⹦"));
		gMeasureData[1] =WaitforStable(pdlgMain, dwStart, FALSE );
		gMeasureData[1].bState = 1;
		dbLigheTemperature = gMeasureData[1].dbTemperature;
		pdlgMain->PostMessage(WM_THREAD, THREAD_MEASURE_DATA, LPARAM(&(gMeasureData[1])));
		Sleep(500);

		TRACE("�رտ��� Ԥ������繦\n");//pdlgMain->m_dbOutputCurrent=??
		pdlgMain->GetDlgItem(IDC_STATIC_STATUS)->SetWindowText(_T("�رտ��� Ԥ������繦"));
		pdlgMain->GetDlgItem(IDC_STATIC_RESULT)->SetWindowText(_T("�رտ��� Ԥ������繦"));
		gMeasureData[0]=WaitforStable(pdlgMain, dwStart, TRUE);
		Sleep(500);

		if (dbLigheTemperature > gMeasureData[0].dbTemperature)
		{
			gMeasureData[2] = gMeasureData[0];
			while (pdlgMain->m_bInMesuring)
			{
				gMeasureData[2].bState = 2;
				dbHeaterVoltageL = gMeasureData[2].dbVoltage;
				dbHeaterTemperatureL = gMeasureData[2].dbTemperature;
				pdlgMain->m_dbOutputCurrent += sqrt(pdlgMain->m_dbHeaterPowerStep / pdlgMain->m_dbHeaterResHeat);
				TRACE("���ӵ�����������\n");//m_dbHeaterPowerStep   pdlgMain->m_dbOutputCurrent=??
				pdlgMain->GetDlgItem(IDC_STATIC_STATUS)->SetWindowText(_T("���ӵ�����������"));
				pdlgMain->GetDlgItem(IDC_STATIC_RESULT)->SetWindowText(_T("���ӵ�����������"));
				gMeasureData[0] =WaitforStable(pdlgMain, dwStart, TRUE);
				if (dbLigheTemperature < gMeasureData[0].dbTemperature)
				{
					gMeasureData[3] = gMeasureData[0];
					gMeasureData[3].bState = 3;
					dbHeaterVoltageH = gMeasureData[3].dbVoltage;
					dbHeaterTemperatureH = gMeasureData[3].dbTemperature;
					break;
				}
				else gMeasureData[2] = gMeasureData[0];

			}
		}
		else
		{
			gMeasureData[3] = gMeasureData[0];

			while (pdlgMain->m_bInMesuring)
			{
				gMeasureData[3].bState = 3;				
				dbHeaterVoltageH = gMeasureData[3].dbVoltage;
				dbHeaterTemperatureH = gMeasureData[3].dbTemperature;
				pdlgMain->m_dbOutputCurrent -= sqrt(pdlgMain->m_dbHeaterPowerStep / pdlgMain->m_dbHeaterResHeat);
				TRACE("���ٵ�����������\n");//m_dbHeaterPowerStep   pdlgMain->m_dbOutputCurrent=??
				pdlgMain->GetDlgItem(IDC_STATIC_STATUS)->SetWindowText(_T("���ٵ�����������"));
				pdlgMain->GetDlgItem(IDC_STATIC_RESULT)->SetWindowText(_T("���ٵ�����������"));
				gMeasureData[0] = WaitforStable(pdlgMain, dwStart, TRUE);
				if (dbLigheTemperature < gMeasureData[0].dbTemperature)
				{
					gMeasureData[2] = gMeasureData[0];
					gMeasureData[2].bState = 2;
					dbHeaterVoltageL = gMeasureData[2].dbVoltage;
					dbHeaterTemperatureL = gMeasureData[2].dbTemperature;
					break;
				}
				else gMeasureData[3] = gMeasureData[0];
			}
		}

		pdlgMain->PostMessage(WM_THREAD, THREAD_MEASURE_DATA, LPARAM(&(gMeasureData[2])));
		pdlgMain->PostMessage(WM_THREAD, THREAD_MEASURE_DATA, LPARAM(&(gMeasureData[3])));
		break;
	}
	pdlgMain->PostMessage(WM_THREAD, THREAD_MEASURE_STATUS, lParam);
}

//���ݽ��պ���
LONG lErrorCode = 0;
void ReceiveDataThread(LPVOID lpParameter)
{
	Cport *commPort = (Cport*)lpParameter;
	if (!commPort)
	{
		lErrorCode = ERROR_INVALID_PARAMETER;
		return;
	}
	char* pcDataRev = new char[commPort->Message];//8��bit
	while (commPort->m_bRunning)
	{
		DWORD dwRet;
		memset(pcDataRev, 0, commPort->Message);
		LONG XY = commPort->RecData(pcDataRev, commPort->Message, &dwRet);//�����׵�ַ,Ҫ��ȡ����������ֽ���,�������շ��سɹ���ȡ�������ֽ���
		if (XY) {
			//GetSpectrumData(pcDataRev, sData);
		}
		if (commPort->callbackFunc)

			if (XY && commPort->callbackFunc)
			{
				commPort->callbackFunc(pcDataRev, dwRet);
			}
	}
	delete[] pcDataRev;
}

PVOID psConnectDevice(DWORD  dwComm, DWORD  dwFrameSize, CallbackFunction callbackFunc)
{
	if (dwComm>64)
	{
		lErrorCode = ERROR_INVALID_PARAMETER;
		return NULL;
	}
	CString pcCommName;
	Cport *commPort = new  Cport;
	if (!commPort)
	{
		lErrorCode = ERROR_NOT_ENOUGH_MEMORY;
		return NULL;
	}
	if (dwComm > 9)
	{
		pcCommName.Format(_T("\\\\.\\COM%d"), dwComm);
	}
	else
	{
		pcCommName.Format(L"COM%d", dwComm);
	}
	BOOL 	bResult = FALSE;
	HANDLE  hThread = NULL;
	commPort->Message = dwFrameSize;
	commPort->callbackFunc = callbackFunc;
	bResult = commPort->OpenPort((LPCWSTR)pcCommName, false);  //�򿪴���
	if (bResult)
	{
		hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ReceiveDataThread, commPort, 0, NULL);
	}
	if (hThread != NULL)
	{
		CloseHandle(hThread);
		lErrorCode = ERROR_SUCCESS;
		return  commPort;
	}
	else
	{
		commPort->ClosePort();
		delete commPort;
		lErrorCode = ERROR_UNKNOWN_FEATURE;
		return NULL;
	}
}


LONG  psWriteData(PVOID pHandle, PCHAR  pcData, DWORD  dwDataSize)
{
	if (!(pHandle && pcData && dwDataSize))
	{
		lErrorCode = ERROR_INVALID_PARAMETER;
		return 0;
	}
	Cport *commPort = (Cport*)pHandle;

	return commPort->SendData(pcData, dwDataSize);
}

BOOL  psSetPort(PVOID pHandle, int BaudRate, int bytesize, int parity, int stopbits)
{
	if (!pHandle ||
		BaudRate<CBR_110 || BaudRate>CBR_256000 * 4 ||
		bytesize<5 || bytesize>8 ||
		parity<NOPARITY || parity>SPACEPARITY ||
		stopbits<ONESTOPBIT || stopbits>TWOSTOPBITS)
	{
		lErrorCode = ERROR_INVALID_PARAMETER;
		return FALSE;
	}
	Cport *commPort = (Cport*)pHandle;

	return commPort->SetPortParm(BaudRate, bytesize, parity, stopbits);
}

BOOL  psDisconnectDevice(PVOID pHandle)
{
	if (!pHandle)
	{
		lErrorCode = ERROR_INVALID_PARAMETER;
		return FALSE;
	}
	Cport *commPort = (Cport*)pHandle;
	commPort->ClosePort();
	delete commPort;
	return TRUE;
}

DWORD CreateID()
{
	SYSTEMTIME sysTime;
	GetLocalTime(&sysTime);
	DWORD dwTime, dwDate;

	dwTime = ((((sysTime.wHour * 60 + sysTime.wMinute) * 60 + sysTime.wSecond) * 10 + (sysTime.wMilliseconds % 10)) & 0x000fffff) |
		((((sysTime.wYear * 12 + sysTime.wMonth) * 31 + sysTime.wDay) & 0x00000fff) << 20);

	GetLocalTime(&sysTime);
	dwDate = ((((sysTime.wHour * 60 + sysTime.wMinute) * 60 + sysTime.wSecond) * 10 + (sysTime.wMilliseconds % 10)) & 0x000fffff) |
		((((sysTime.wYear * 12 + sysTime.wMonth) * 31 + sysTime.wDay) & 0x00000fff) << 20);
	return dwDate;
}