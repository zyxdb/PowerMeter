#pragma once
#include "stdafx.h"
#include "windows.h"
#include "PowerMeter.h"
#include "commons.h"
#include "PowerMeterLib.h"
#include "PowerMeterDlg.h"
#include "GPIB\ni4882.h"

bool m_bQJ58Connect = false;//���ſ���
bool m_bMultimeterConnect = false;//���ñ���
bool m_bCurrentSourceConnect = false;//���ñ���
bool m_bShutterConnect = false;//���ſ���
bool m_bExtCtrlConnect = false;//������ƿ���
bool m_bPDAConnect = false;//�����ܿ���

int  m_iDev;//����
Cport	m_cComPortCurrentSource;//���ܵ���Դ
Cport	m_cComPortMultimeter;//���ñ�
Cport	m_cComPortShutter;//����
Cport	m_cComPortExtCtrl;//�������
Cport	m_cComPortPDA;//������

void SelfCheckThread(LPVOID lpParameter)
{
	CPowerMeterDlg *pdlgMain = (CPowerMeterDlg *)lpParameter;
	
	do {
		// �Լ췵�ؽ���ж�
		int ReturnState = THREAD_STATE_SUCCESS;

		if (pdlgMain->m_iSelfChecking != 1) break;
		if (bEleBridgeCheck()) {
			TRACE("����ͨ������\n");
			pdlgMain->GetDlgItem(IDC_STATIC_STATUS)->SetWindowText(_T("����ͨ������"));
			pdlgMain->GetDlgItem(IDC_STATIC_RESULT)->SetWindowText(_T("����ͨ������"));
		}
		else {
			TRACE("����ͨ��ʧ��\n");
			pdlgMain->GetDlgItem(IDC_STATIC_STATUS)->SetWindowText(_T("����ͨ��ʧ��"));
			pdlgMain->GetDlgItem(IDC_STATIC_RESULT)->SetWindowText(_T("����ͨ��ʧ��"));
			ReturnState = THREAD_STATE_ERROR;
		}
		Sleep(2000);

		if (pdlgMain->m_iSelfChecking != 1) break;
		if (bMultiMeterCheck(pdlgMain)) {
			TRACE("���ñ�ͨ������\n");
			pdlgMain->GetDlgItem(IDC_STATIC_STATUS)->SetWindowText(_T("���ñ�ͨ������"));
			pdlgMain->GetDlgItem(IDC_STATIC_RESULT)->SetWindowText(_T("���ñ�ͨ������"));
		}
		else {
			TRACE("���ñ�ͨ��ʧ��\n");
			pdlgMain->GetDlgItem(IDC_STATIC_STATUS)->SetWindowText(_T("���ñ�ͨ��ʧ��"));
			pdlgMain->GetDlgItem(IDC_STATIC_RESULT)->SetWindowText(_T("���ñ�ͨ��ʧ��"));
			ReturnState = THREAD_STATE_ERROR;
		}
		Sleep(2000);

		if (pdlgMain->m_iSelfChecking != 1) break;
		if (bCurrentSourceCheck(pdlgMain)) {
			TRACE("����Դͨ������\n");
			pdlgMain->GetDlgItem(IDC_STATIC_STATUS)->SetWindowText(_T("����Դͨ������"));
			pdlgMain->GetDlgItem(IDC_STATIC_RESULT)->SetWindowText(_T("����Դͨ������"));
		}
		else {
			TRACE("����Դͨ��ʧ��\n");
			pdlgMain->GetDlgItem(IDC_STATIC_STATUS)->SetWindowText(_T("����Դͨ��ʧ��"));
			pdlgMain->GetDlgItem(IDC_STATIC_RESULT)->SetWindowText(_T("����Դͨ��ʧ��"));
			ReturnState = THREAD_STATE_ERROR;
		}
		Sleep(2000);

		// ���ſ��ٿ���һ�ξ͵��Լ��ˡ�
		if (pdlgMain->m_iSelfChecking != 1) break;
		if (bShutterCheck(pdlgMain)) {
			TRACE("����ͨ������\n");
			pdlgMain->GetDlgItem(IDC_STATIC_STATUS)->SetWindowText(_T("����ͨ������"));
			pdlgMain->GetDlgItem(IDC_STATIC_RESULT)->SetWindowText(_T("����ͨ������"));
		}
		else {
			TRACE("����ͨ��ʧ��\n");
			pdlgMain->GetDlgItem(IDC_STATIC_STATUS)->SetWindowText(_T("����ͨ��ʧ��"));
			pdlgMain->GetDlgItem(IDC_STATIC_RESULT)->SetWindowText(_T("����ͨ��ʧ��"));
			ReturnState = THREAD_STATE_ERROR;
		}
		Sleep(2000);

		// �ⲿЭ������ͨ�Ż�ûЭ�顣�����ڲ����ݴ�����
		if (pdlgMain->m_iSelfChecking != 1) break;
		if (bExtCtrlCheck(pdlgMain)) {
			TRACE("�ⲿЭ���˿�����\n");
			pdlgMain->GetDlgItem(IDC_STATIC_STATUS)->SetWindowText(_T("�ⲿЭ���˿�����"));
			pdlgMain->GetDlgItem(IDC_STATIC_RESULT)->SetWindowText(_T("�ⲿЭ���˿�����"));
		}
		else {
			TRACE("�ⲿЭ���˿��쳣\n");
			pdlgMain->GetDlgItem(IDC_STATIC_STATUS)->SetWindowText(_T("�ⲿЭ���˿��쳣"));
			pdlgMain->GetDlgItem(IDC_STATIC_RESULT)->SetWindowText(_T("�ⲿЭ���˿��쳣"));
			ReturnState = THREAD_STATE_ERROR;
		}
		Sleep(2000);

		if (pdlgMain->m_iSelfChecking != 1) break;
		if (bPDACheck(pdlgMain)) {
			TRACE("PSDͨ������\n");
			pdlgMain->GetDlgItem(IDC_STATIC_STATUS)->SetWindowText(_T("PSDͨ������"));
			pdlgMain->GetDlgItem(IDC_STATIC_RESULT)->SetWindowText(_T("PSDͨ������"));
		}
		else {
			TRACE("PSDͨ���쳣\n");
			pdlgMain->GetDlgItem(IDC_STATIC_STATUS)->SetWindowText(_T("PSDͨ���쳣"));
			pdlgMain->GetDlgItem(IDC_STATIC_RESULT)->SetWindowText(_T("PSDͨ���쳣"));
			ReturnState = THREAD_STATE_ERROR;
		}
		Sleep(2000);

		pdlgMain->PostMessage(WM_THREAD, THREAD_SELFCHECK_STATUS, ReturnState);
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
		//if (XY) {
		//	//GetSpectrumData(pcDataRev, sData);
		//}
		//if (commPort->callbackFunc)

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

// �����Լ��ж�
BOOL bEleBridgeCheck() {
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	#define GPIB0                 0        // Board handle
	#define ARRAYSIZE             512      // Size of read buffer
	#define BDINDEX               0        // Board Index
	#define PRIMARY_ADDR_OF_PPS   5        // Primary address of device
	#define NO_SECONDARY_ADDR     0        // Secondary address of device
	#define TIMEOUT               T10s     // Timeout value = 10 seconds
	#define EOTMODE               1        // Enable the END message
	#define EOSMODE               0x140D   // Disable the EOS mode

	// δ����ͨ��ʱ�Ƚ���ͨ��
	if (!m_bQJ58Connect) {
		m_iDev = ibdev(BDINDEX, PRIMARY_ADDR_OF_PPS, NO_SECONDARY_ADDR,
			TIMEOUT, EOTMODE, EOSMODE);
		m_bQJ58Connect = true;
	}
	static char ValueStr[ARRAYSIZE + 1];
	ibwrt(m_iDev, "*IDN?\r\n", 7L);
	ibrd(m_iDev, ValueStr, ARRAYSIZE);
	// ���շ���ֵӦΪ��QJ58A RAYSTING INSTRUMENT��,���Ǻ�����λ�᷵��һ���ո������õڶ�λ�͵���λ���ж�
	if (ValueStr[1] == 'Q' && ValueStr[2] == 'J') {
		return true;
	}
	return false;
	//ibwrt(m_iDev, "D3Measurements:\r", 17L);
}

BOOL bMultiMeterCheck(CPowerMeterDlg *pdlgMain) {
	CString str, pcCommName;
	bool m_bValidVerify;
	char pcCommand[20] = { '$','$', 'R', 'E','A','D','V','L','T','R','A','N','G','E','#','#' };

	str = pdlgMain->m_csComMultimeter;
	int num = _ttoi(str);//�ַ���תint
	if (num >= 9)
	{
		pcCommName.Format(_T("\\\\.\\COM%d"), num + 1);
	}
	else
	{
		pcCommName.Format(L"COM%d", num + 1);
	}
	m_bValidVerify = m_cComPortMultimeter.OpenPort((LPCWSTR)pcCommName, false);  //�򿪴���
	if (!m_bValidVerify)
	{
		return false;
	}
	m_bValidVerify = m_cComPortMultimeter.SetPortParm(115200);//���ڲ���������Ϊ115200
	if (!m_bValidVerify)
	{
		m_cComPortMultimeter.ClosePort();
		return false;
	}
	else
	{
		DWORD dwRev = 0;
		m_bValidVerify = m_cComPortMultimeter.SendData(pcCommand, 18);
		if (m_bValidVerify)
		{
			LONG XY = m_cComPortMultimeter.RecData(pcCommand, 32, &dwRev);
			if (XY) {
				m_bMultimeterConnect = true;
				return true;
			}
		}
	}
	m_cComPortMultimeter.ClosePort();
	return false;
}

BOOL bCurrentSourceCheck(CPowerMeterDlg *pdlgMain) {
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString str, pcCommName;
	bool m_bValidVerify;
	char pcCommand[20] = { 'A','T', 'V' , ',','S','E','T',',','A','O','N',',','0','0','0','0','0','0','0',';' };

	str = pdlgMain->m_csComPortCurrentSource;
	int num = _ttoi(str);//�ַ���תint
						 //int num = m_iCurrentsource;
	if (num >= 9)
	{
		pcCommName.Format(_T("\\\\.\\COM%d"), num + 1);
	}
	else
	{
		pcCommName.Format(L"COM%d", num + 1);
	}
	m_bValidVerify = m_cComPortCurrentSource.OpenPort((LPCWSTR)pcCommName, false);  //�򿪴���
	if (!m_bValidVerify)
	{
		return false;
	}
	m_bValidVerify = m_cComPortCurrentSource.SetPortParm(9600);//���ڲ���������Ϊ9600
	if (!m_bValidVerify)
	{
		m_cComPortCurrentSource.ClosePort();
		return false;
	}
	else
	{
		DWORD dwRev = 0;
		m_bValidVerify = m_cComPortCurrentSource.SendData(pcCommand, 20);
		if (m_bValidVerify)
		{
			LONG XY = m_cComPortMultimeter.RecData(pcCommand, 32, &dwRev);
			if (XY) {
				m_bCurrentSourceConnect = true;
				return true;
			}
		}
	}
	m_cComPortCurrentSource.ClosePort();
	return false;
}

BOOL bShutterCheck(CPowerMeterDlg *pdlgMain) {
	// �����Լ����
	CString str, pcCommName;
	bool m_bValidVerify;
	//char pcCommand[20] = { 'A','T', 'V' , ',','S','E','T',',','A','O','N',',','0','0','0','0','0','0','0',';' };
	char sendOpen[1] = { 0xea };// ȫ��ָ��
	char sendClose[1] = { 0x0a };// ȫ��ָ��

	str = pdlgMain->m_csComPortShutter;
	int num = _ttoi(str);//�ַ���תint
	if (num >= 9)
	{
		pcCommName.Format(_T("\\\\.\\COM%d"), num + 1);
	}
	else
	{
		pcCommName.Format(L"COM%d", num + 1);
	}
	m_bValidVerify = m_cComPortShutter.OpenPort((LPCWSTR)pcCommName, false);  //�򿪴���
	if (!m_bValidVerify)
	{
		return false;
	}
	m_bValidVerify = m_cComPortShutter.SetPortParm(19200);//���ڲ���������Ϊ19200
	if (!m_bValidVerify)
	{
		m_cComPortShutter.ClosePort();
		return false;
	}
	else
	{
		DWORD dwRev = 0;
		m_bValidVerify = m_cComPortShutter.SendData(sendOpen, 1);
		bool tmp = m_cComPortShutter.SendData(sendClose, 1);
		if (m_bValidVerify && tmp)
		{
			m_bShutterConnect = true;
			return true;
		}
	}
	m_cComPortShutter.ClosePort();
	return false;
}

BOOL bExtCtrlCheck(CPowerMeterDlg *pdlgMain) {
	CString str, pcCommName;
	bool m_bValidVerify;
	//char pcCommand[20] = { 'A','T', 'V' , ',','S','E','T',',','A','O','N',',','0','0','0','0','0','0','0',';' };
	//char sendOpen[1] = { 0xea };// ȫ��ָ��
	//char sendClose[1] = { 0x0a };// ȫ��ָ��

	str = pdlgMain->m_csComPortExtCtrl;
	int num = _ttoi(str);//�ַ���תint
	if (num >= 9)
	{
		pcCommName.Format(_T("\\\\.\\COM%d"), num + 1);
	}
	else
	{
		pcCommName.Format(L"COM%d", num + 1);
	}
	m_bValidVerify = m_cComPortExtCtrl.OpenPort((LPCWSTR)pcCommName, false);  //�򿪴���
	if (!m_bValidVerify)
	{
		return false;
	}
	m_bValidVerify = m_cComPortExtCtrl.SetPortParm(115200);//���ڲ���������Ϊ115200
	if (!m_bValidVerify)
	{
		m_cComPortExtCtrl.ClosePort();
		return false;
	}
	else
	{
		// ��δȷ��ͨ��Э�� ��ֱ�ӷ���TRUE
		m_bExtCtrlConnect = true;
		return true;
	}
	m_cComPortExtCtrl.ClosePort();
	return false;
}

BOOL bPDACheck(CPowerMeterDlg *pdlgMain) {
	CString str, pcCommName;
	bool m_bValidVerify;
	//char pcCommand[20] = { 'A','T', 'V' , ',','S','E','T',',','A','O','N',',','0','0','0','0','0','0','0',';' };
	//char sendOpen[1] = { 0xea };// ȫ��ָ��
	//char sendClose[1] = { 0x0a };// ȫ��ָ��

	str = pdlgMain->m_csComPortPDA;
	int num = _ttoi(str);//�ַ���תint
	if (num >= 9)
	{
		pcCommName.Format(_T("\\\\.\\COM%d"), num + 1);
	}
	else
	{
		pcCommName.Format(L"COM%d", num + 1);
	}
	m_bValidVerify = m_cComPortPDA.OpenPort((LPCWSTR)pcCommName, false);  //�򿪴���
	if (!m_bValidVerify)
	{
		return false;
	}
	m_bValidVerify = m_cComPortPDA.SetPortParm(115200);//���ڲ���������Ϊ115200
	if (!m_bValidVerify)
	{
		m_cComPortPDA.ClosePort();
		return false;
	}
	else
	{
		// ��δȷ��ͨ��Э�� ��ֱ�ӷ���TRUE
		m_bPDAConnect = true;
		return true;
	}
	m_cComPortPDA.ClosePort();
	return false;
}