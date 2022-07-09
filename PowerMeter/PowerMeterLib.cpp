#pragma once
#include "stdafx.h"
#include "windows.h"
#include "PowerMeter.h"
#include "PowerMeterDlg.h"
#include "PowerMeterLib.h"
#include "GPIB\ni4882.h"
#include <string>

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
BOOL bPDACheck(CPowerMeterDlg *pdlgMain);
BOOL bExtCtrlCheck(CPowerMeterDlg *pdlgMain);

double temperatureMeasure(CPowerMeterDlg *pdlgMain);
double VoltageMeasure(CPowerMeterDlg *pdlgMain);

void SelfCheckThread(LPVOID lpParameter)
{
	CPowerMeterDlg *pdlgMain = (CPowerMeterDlg *)lpParameter;
	
	do {
		// 自检返回结果判断
		int ReturnState = THREAD_STATE_SUCCESS;

		if (pdlgMain->m_iSelfChecking != 1) break;
		if (bEleBridgeCheck()) {
			TRACE("电桥通信正常\n");
			pdlgMain->GetDlgItem(IDC_STATIC_STATUS)->SetWindowText(_T("电桥通信正常"));
			pdlgMain->GetDlgItem(IDC_STATIC_RESULT)->SetWindowText(_T("电桥通信正常"));
		}
		else {
			TRACE("电桥通信失败\n");
			pdlgMain->GetDlgItem(IDC_STATIC_STATUS)->SetWindowText(_T("电桥通信失败"));
			pdlgMain->GetDlgItem(IDC_STATIC_RESULT)->SetWindowText(_T("电桥通信失败"));
			ReturnState = THREAD_STATE_ERROR;
		}
		Sleep(2000);

		if (pdlgMain->m_iSelfChecking != 1) break;
		if (bMultiMeterCheck(pdlgMain)) {
			TRACE("万用表通信正常\n");
			pdlgMain->GetDlgItem(IDC_STATIC_STATUS)->SetWindowText(_T("万用表通信正常"));
			pdlgMain->GetDlgItem(IDC_STATIC_RESULT)->SetWindowText(_T("万用表通信正常"));
		}
		else {
			TRACE("万用表通信失败\n");
			pdlgMain->GetDlgItem(IDC_STATIC_STATUS)->SetWindowText(_T("万用表通信失败"));
			pdlgMain->GetDlgItem(IDC_STATIC_RESULT)->SetWindowText(_T("万用表通信失败"));
			ReturnState = THREAD_STATE_ERROR;
		}
		Sleep(2000);

		if (pdlgMain->m_iSelfChecking != 1) break;
		if (bCurrentSourceCheck(pdlgMain)) {
			TRACE("电流源通信正常\n");
			pdlgMain->GetDlgItem(IDC_STATIC_STATUS)->SetWindowText(_T("电流源通信正常"));
			pdlgMain->GetDlgItem(IDC_STATIC_RESULT)->SetWindowText(_T("电流源通信正常"));
		}
		else {
			TRACE("电流源通信失败\n");
			pdlgMain->GetDlgItem(IDC_STATIC_STATUS)->SetWindowText(_T("电流源通信失败"));
			pdlgMain->GetDlgItem(IDC_STATIC_RESULT)->SetWindowText(_T("电流源通信失败"));
			ReturnState = THREAD_STATE_ERROR;
		}
		Sleep(2000);

		// 快门快速开关一次就当自检了。
		if (pdlgMain->m_iSelfChecking != 1) break;
		if (bShutterCheck(pdlgMain)) {
			TRACE("快门通信正常\n");
			pdlgMain->GetDlgItem(IDC_STATIC_STATUS)->SetWindowText(_T("快门通信正常"));
			pdlgMain->GetDlgItem(IDC_STATIC_RESULT)->SetWindowText(_T("快门通信正常"));
		}
		else {
			TRACE("快门通信失败\n");
			pdlgMain->GetDlgItem(IDC_STATIC_STATUS)->SetWindowText(_T("快门通信失败"));
			pdlgMain->GetDlgItem(IDC_STATIC_RESULT)->SetWindowText(_T("快门通信失败"));
			ReturnState = THREAD_STATE_ERROR;
		}
		Sleep(2000);

		// 外部协作串口通信还没协议。函数内部内容待补充
		if (pdlgMain->m_iSelfChecking != 1) break;
		if (bExtCtrlCheck(pdlgMain)) {
			TRACE("外部协作端口正常\n");
			pdlgMain->GetDlgItem(IDC_STATIC_STATUS)->SetWindowText(_T("外部协作端口正常"));
			pdlgMain->GetDlgItem(IDC_STATIC_RESULT)->SetWindowText(_T("外部协作端口正常"));
		}
		else {
			TRACE("外部协作端口异常\n");
			pdlgMain->GetDlgItem(IDC_STATIC_STATUS)->SetWindowText(_T("外部协作端口异常"));
			pdlgMain->GetDlgItem(IDC_STATIC_RESULT)->SetWindowText(_T("外部协作端口异常"));
			ReturnState = THREAD_STATE_ERROR;
		}
		Sleep(2000);

		if (pdlgMain->m_iSelfChecking != 1) break;
		if (bPDACheck(pdlgMain)) {
			TRACE("PSD通信正常\n");
			pdlgMain->GetDlgItem(IDC_STATIC_STATUS)->SetWindowText(_T("PSD通信正常"));
			pdlgMain->GetDlgItem(IDC_STATIC_RESULT)->SetWindowText(_T("PSD通信正常"));
		}
		else {
			TRACE("PSD通信异常\n");
			pdlgMain->GetDlgItem(IDC_STATIC_STATUS)->SetWindowText(_T("PSD通信异常"));
			pdlgMain->GetDlgItem(IDC_STATIC_RESULT)->SetWindowText(_T("PSD通信异常"));
			ReturnState = THREAD_STATE_ERROR;
		}
		Sleep(2000);

		pdlgMain->PostMessage(WM_THREAD, THREAD_SELFCHECK_STATUS, ReturnState);
	} while (0);
	if (pdlgMain->m_iSelfChecking != 1) pdlgMain->PostMessage(WM_THREAD, THREAD_SELFCHECK_STATUS, THREAD_STATE_TERMINATED);
}

MeasureData gMeasureData[4];
// 依据电压表反馈调控电流源稳定输出I(PID) 读电桥测量值，待测量腔电热稳定记为TO。若精密电流源输出足够准确稳定，此模块可不必使用PID调节。
MeasureData WaitforStable(CPowerMeterDlg *pdlgMain,DWORD dwStart,BOOL bElectricEnable=FALSE)
{
	CString  csString;
	SYSTEMTIME sysTime;
	LPARAM  lParam = LPARAM(THREAD_STATE_SUCCESS);
	double dbPidStability = 1, dbHeaterStability = 1;
	double measureCurrent = 0;
	DWORD dwTimeBegin, dwTimePeriod;
	dwTimeBegin = GetTickCount();
	while (pdlgMain->m_bInMesuring)
	{
		TRACE(_T("测电流电压热阻"));
		pdlgMain->GetDlgItem(IDC_STATIC_STATUS)->SetWindowText(_T("测电流电压热阻"));
		pdlgMain->GetDlgItem(IDC_STATIC_RESULT)->SetWindowText(_T("测电流电压热阻"));
		GetLocalTime(&sysTime);

		// 默认4位为0111
		gMeasureData[0].bFlag = (1 << LineSerie_Temperature) |
								(1 << LineSerie_Voltage)	 |
								(1 << LineSerie_OutputCurrent);

		// 整体测量启动时间 到 该次测量温度、电压、电流源之前所花费的时间
		gMeasureData[0].dwIdex = GetTickCount() / 100 - dwStart;

		// 计算温度（失败返回-1）
		gMeasureData[0].dbTemperature = temperatureMeasure(pdlgMain);
		// 测量失败时，将Flag与0110相与
		if (gMeasureData[0].dbTemperature < 0) {
			gMeasureData[0].bFlag &= 6;
		}

		// 计算电压
		gMeasureData[0].dbVoltage = VoltageMeasure(pdlgMain);
		// 测量失败时，将Flag与0101相与
		if (gMeasureData[0].dbVoltage < 0) {
			gMeasureData[0].bFlag &= 5;
		}

		// 计算电流
		measureCurrent = gMeasureData[0].dbVoltage / pdlgMain->m_dbHeaterResCheck;  // 实际加热电流 = 精密万用表测量电压值 / 采样电阻值
		gMeasureData[0].dbOutputCurrent = pdlgMain->m_dbOutputCurrent;
		// 测量失败时，将Flag与0011相与
		if (gMeasureData[0].dbOutputCurrent < 0) {
			gMeasureData[0].bFlag &= 3;
		}

		gMeasureData[0].bState = 0;
		pdlgMain->PostMessage(WM_THREAD, THREAD_MEASURE_DATA, LPARAM(&(gMeasureData[0])));

		if (bElectricEnable)
		{
			// 测量温度、电压、电流源花费的时间
			dwTimePeriod = GetTickCount() - dwTimeBegin;
			if (dwTimePeriod > pdlgMain->m_dbPidStableTime)
			{
				dbPidStability = abs(measureCurrent - pdlgMain->m_dbOutputCurrent) / pdlgMain->m_dbOutputCurrent;
				TRACE(_T("计算电流稳定度"));
				pdlgMain->GetDlgItem(IDC_STATIC_STATUS)->SetWindowText(_T("计算电流稳定度"));
				pdlgMain->GetDlgItem(IDC_STATIC_RESULT)->SetWindowText(_T("计算电流稳定度"));
				if (dbPidStability < pdlgMain->m_dbPidStability)
				{
					//pdlgMain->m_dbOutputCurrent = measureCurrent;
					TRACE(_T("计算输出电流并输出"));
					pdlgMain->GetDlgItem(IDC_STATIC_STATUS)->SetWindowText(_T("计算输出电流并输出"));
					pdlgMain->GetDlgItem(IDC_STATIC_RESULT)->SetWindowText(_T("计算输出电流并输出"));
					continue;
				}
			}
		}

		dwTimePeriod = GetTickCount() - dwTimeBegin;
		if (dwTimePeriod > pdlgMain->m_dbHeaterStableTime)
		{
			//dbHeaterStability=?
			TRACE(_T("计算加热腔稳定度"));
			pdlgMain->GetDlgItem(IDC_STATIC_STATUS)->SetWindowText(_T("计算加热腔稳定度"));
			pdlgMain->GetDlgItem(IDC_STATIC_RESULT)->SetWindowText(_T("计算加热腔稳定度"));
			if (dbHeaterStability < pdlgMain->m_dbHeaterStability)
			{
				TRACE(_T("等待加热腔稳定"));
				pdlgMain->GetDlgItem(IDC_STATIC_STATUS)->SetWindowText(_T("等待加热腔稳定"));
				pdlgMain->GetDlgItem(IDC_STATIC_RESULT)->SetWindowText(_T("等待加热腔稳定"));
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
	DWORD dwStart=GetTickCount()/100;  // 整体测量启动时间
	double dbLightTemperature, dbHeaterTemperatureH, dbHeaterTemperatureL, dbHeaterVoltageH, dbHeaterVoltageL;
	srand(dwStart);  // TODO 测试时直接随机出一个时间。后续需要改成真实时间。
	while (pdlgMain->m_bInMesuring)
	{
		TRACE("预热测温腔\n");
		pdlgMain->GetDlgItem(IDC_STATIC_STATUS)->SetWindowText(_T("预热测温腔"));
		pdlgMain->GetDlgItem(IDC_STATIC_RESULT)->SetWindowText(_T("预热测温腔"));
		Sleep(500);	
		if (pdlgMain->m_dbHeaterResHeat <= 0)
		{
			lParam = LPARAM(THREAD_STATE_PARAMS);
			break;
		}

		// 计算光功率预估值对应的电功率电流值 = sqrt(光功率预估值 / 加热电阻值)
		pdlgMain->m_dbOutputCurrent = sqrt(pdlgMain->m_dbLightPowerEstimate / pdlgMain->m_dbHeaterResHeat);
		// 执行电流输出函数
		SetCurrent(pdlgMain, pdlgMain->m_dbOutputCurrent,true);
		csString.Format(_T("电流输出 %f"), pdlgMain->m_dbOutputCurrent);
		TRACE(csString);
		pdlgMain->GetDlgItem(IDC_STATIC_STATUS)->SetWindowText(csString);
		pdlgMain->GetDlgItem(IDC_STATIC_RESULT)->SetWindowText(csString);
		WaitforStable(pdlgMain, dwStart,TRUE);
		Sleep(500);

		// 关闭电流源输出，提示开启待测光，开启快门。开始测量光加热温度。(待测光开启需要控制吗？)
		SetCurrent(pdlgMain, pdlgMain->m_dbOutputCurrent,false);
		SetShutter(pdlgMain, true);
		TRACE("撤去电功开启快门测量光功\n");
		pdlgMain->GetDlgItem(IDC_STATIC_STATUS)->SetWindowText(_T("开启快门测量光功"));
		pdlgMain->GetDlgItem(IDC_STATIC_RESULT)->SetWindowText(_T("开启快门测量光功"));
		gMeasureData[1] = WaitforStable(pdlgMain, dwStart, FALSE );
		gMeasureData[1].bState = 1;
		dbLightTemperature = gMeasureData[1].dbTemperature;
		pdlgMain->PostMessage(WM_THREAD, THREAD_MEASURE_DATA, LPARAM(&(gMeasureData[1])));
		Sleep(500);

		TRACE("关闭快门 预估替代电功\n");//pdlgMain->m_dbOutputCurrent=??
		pdlgMain->GetDlgItem(IDC_STATIC_STATUS)->SetWindowText(_T("关闭快门 预估替代电功"));
		pdlgMain->GetDlgItem(IDC_STATIC_RESULT)->SetWindowText(_T("关闭快门 预估替代电功"));
		gMeasureData[0]=WaitforStable(pdlgMain, dwStart, TRUE);
		Sleep(500);

		if (dbLightTemperature > gMeasureData[0].dbTemperature)
		{
			gMeasureData[2] = gMeasureData[0];
			while (pdlgMain->m_bInMesuring)
			{
				gMeasureData[2].bState = 2;
				dbHeaterVoltageL = gMeasureData[2].dbVoltage;
				dbHeaterTemperatureL = gMeasureData[2].dbTemperature;
				pdlgMain->m_dbOutputCurrent += sqrt(pdlgMain->m_dbHeaterPowerStep / pdlgMain->m_dbHeaterResHeat);
				TRACE("增加电流测量电热\n");//m_dbHeaterPowerStep   pdlgMain->m_dbOutputCurrent=??
				pdlgMain->GetDlgItem(IDC_STATIC_STATUS)->SetWindowText(_T("增加电流测量电热"));
				pdlgMain->GetDlgItem(IDC_STATIC_RESULT)->SetWindowText(_T("增加电流测量电热"));
				Sleep(500);
				gMeasureData[0] =WaitforStable(pdlgMain, dwStart, TRUE);
				if (dbLightTemperature < gMeasureData[0].dbTemperature)
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
				TRACE("减少电流测量电热\n");//m_dbHeaterPowerStep   pdlgMain->m_dbOutputCurrent=??
				pdlgMain->GetDlgItem(IDC_STATIC_STATUS)->SetWindowText(_T("减少电流测量电热"));
				pdlgMain->m_csResult = _T("");
				pdlgMain->GetDlgItem(IDC_STATIC_RESULT)->SetWindowText(_T("减少电流测量电热"));
				//Sleep(500);
				gMeasureData[0] = WaitforStable(pdlgMain, dwStart, TRUE);
				if (dbLightTemperature < gMeasureData[0].dbTemperature)
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

//数据接收函数
LONG lErrorCode = 0;
void ReceiveDataThread(LPVOID lpParameter)
{
	Cport *commPort = (Cport*)lpParameter;
	if (!commPort)
	{
		lErrorCode = ERROR_INVALID_PARAMETER;
		return;
	}
	char* pcDataRev = new char[commPort->Message];//8个bit
	while (commPort->m_bRunning)
	{
		DWORD dwRet;
		memset(pcDataRev, 0, commPort->Message);
		LONG XY = commPort->RecData(pcDataRev, commPort->Message, &dwRet);//数据首地址,要读取的数据最大字节数,用来接收返回成功读取的数据字节数
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

//PVOID psConnectDevice(DWORD  dwComm, DWORD  dwFrameSize, CallbackFunction callbackFunc)
//{
//	if (dwComm>64)
//	{
//		lErrorCode = ERROR_INVALID_PARAMETER;
//		return NULL;
//	}
//	CString pcCommName;
//	Cport *commPort = new  Cport;
//	if (!commPort)
//	{
//		lErrorCode = ERROR_NOT_ENOUGH_MEMORY;
//		return NULL;
//	}
//	if (dwComm > 9)
//	{
//		pcCommName.Format(_T("\\\\.\\COM%d"), dwComm);
//	}
//	else
//	{
//		pcCommName.Format(L"COM%d", dwComm);
//	}
//	BOOL 	bResult = FALSE;
//	HANDLE  hThread = NULL;
//	commPort->Message = dwFrameSize;
//	commPort->callbackFunc = callbackFunc;
//	bResult = commPort->OpenPort((LPCWSTR)pcCommName, false);  //打开串口
//	if (bResult)
//	{
//		hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ReceiveDataThread, commPort, 0, NULL);
//	}
//	if (hThread != NULL)
//	{
//		CloseHandle(hThread);
//		lErrorCode = ERROR_SUCCESS;
//		return  commPort;
//	}
//	else
//	{
//		commPort->ClosePort();
//		delete commPort;
//		lErrorCode = ERROR_UNKNOWN_FEATURE;
//		return NULL;
//	}
//}

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

// 电桥自检判断
BOOL bEleBridgeCheck() {
	// TODO: 在此添加控件通知处理程序代码
	#define GPIB0                 0        // Board handle
	#define ARRAYSIZE             512      // Size of read buffer 读写缓存大小
	#define BDINDEX               0        // Board Index 板卡索引号
	#define PRIMARY_ADDR_OF_PPS   5        // Primary address of device 定义被控测试仪器主地址
	#define NO_SECONDARY_ADDR     0        // Secondary address of device 定义被控测试仪器次地址
	#define TIMEOUT               T10s     // Timeout value = 10 seconds
	#define EOTMODE               1        // Enable the END message
	#define EOSMODE               0x140D   // Disable the EOS mode

	// 未建立通信时先建立通信
	if (!m_bQJ58Connect) {
		m_iDev = ibdev(BDINDEX, PRIMARY_ADDR_OF_PPS, NO_SECONDARY_ADDR,
			TIMEOUT, EOTMODE, EOSMODE);
		m_bQJ58Connect = true;
	}
	static char ValueStr[ARRAYSIZE + 1];
	ibwrt(m_iDev, "*IDN?\r\n", 7L);
	ibrd(m_iDev, ValueStr, ARRAYSIZE);
	// 接收返回值应为“QJ58A RAYSTING INSTRUMENT”,但是好像首位会返回一个空格，所以用第二位和第三位做判断
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
	int num = _ttoi(str);//字符串转int
	if (num >= 9)
	{
		pcCommName.Format(_T("\\\\.\\COM%d"), num + 1);
	}
	else
	{
		pcCommName.Format(L"COM%d", num + 1);
	}
	m_bValidVerify = m_cComPortMultimeter.OpenPort((LPCWSTR)pcCommName, false);  //打开串口
	if (!m_bValidVerify)
	{
		return false;
	}
	m_bValidVerify = m_cComPortMultimeter.SetPortParm(115200);//串口波特率设置为115200
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
	// TODO: 在此添加控件通知处理程序代码
	CString str, pcCommName;
	bool m_bValidVerify;
	char pcCommand[20] = { 'A','T', 'V' , ',','S','E','T',',','A','O','N',',','0','0','0','0','0','0','0',';' };

	str = pdlgMain->m_csComPortCurrentSource;
	int num = _ttoi(str);//字符串转int
						 //int num = m_iCurrentsource;
	if (num >= 9)
	{
		pcCommName.Format(_T("\\\\.\\COM%d"), num + 1);
	}
	else
	{
		pcCommName.Format(L"COM%d", num + 1);
	}
	m_bValidVerify = m_cComPortCurrentSource.OpenPort((LPCWSTR)pcCommName, false);  //打开串口
	if (!m_bValidVerify)
	{
		return false;
	}
	m_bValidVerify = m_cComPortCurrentSource.SetPortParm(9600);//串口波特率设置为9600
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
	// 快门自检程序
	CString str, pcCommName;
	bool m_bValidVerify;
	char sendOpen[1] = { 0xea };// 全开指令
	char sendClose[1] = { 0x0a };// 全关指令

	str = pdlgMain->m_csComPortShutter;
	int num = _ttoi(str);//字符串转int
	if (num >= 9)
	{
		pcCommName.Format(_T("\\\\.\\COM%d"), num + 1);
	}
	else
	{
		pcCommName.Format(L"COM%d", num + 1);
	}
	m_bValidVerify = m_cComPortShutter.OpenPort((LPCWSTR)pcCommName, false);  //打开串口
	if (!m_bValidVerify)
	{
		return false;
	}
	m_bValidVerify = m_cComPortShutter.SetPortParm(19200);//串口波特率设置为19200
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
	//char sendOpen[1] = { 0xea };// 全开指令
	//char sendClose[1] = { 0x0a };// 全关指令

	str = pdlgMain->m_csComPortExtCtrl;
	int num = _ttoi(str);//字符串转int
	if (num >= 9)
	{
		pcCommName.Format(_T("\\\\.\\COM%d"), num + 1);
	}
	else
	{
		pcCommName.Format(L"COM%d", num + 1);
	}
	m_bValidVerify = m_cComPortExtCtrl.OpenPort((LPCWSTR)pcCommName, false);  //打开串口
	if (!m_bValidVerify)
	{
		return false;
	}
	m_bValidVerify = m_cComPortExtCtrl.SetPortParm(115200);//串口波特率设置为115200
	if (!m_bValidVerify)
	{
		m_cComPortExtCtrl.ClosePort();
		return false;
	}
	else
	{
		// 还未确定通信协议 先直接返回TRUE
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
	//char sendOpen[1] = { 0xea };// 全开指令
	//char sendClose[1] = { 0x0a };// 全关指令

	str = pdlgMain->m_csComPortPDA;
	int num = _ttoi(str);//字符串转int
	if (num >= 9)
	{
		pcCommName.Format(_T("\\\\.\\COM%d"), num + 1);
	}
	else
	{
		pcCommName.Format(L"COM%d", num + 1);
	}
	m_bValidVerify = m_cComPortPDA.OpenPort((LPCWSTR)pcCommName, false);  //打开串口
	if (!m_bValidVerify)
	{
		return false;
	}
	m_bValidVerify = m_cComPortPDA.SetPortParm(115200);//串口波特率设置为115200
	if (!m_bValidVerify)
	{
		m_cComPortPDA.ClosePort();
		return false;
	}
	else
	{
		// 还未确定通信协议 先直接返回TRUE
		m_bPDAConnect = true;
		return true;
	}
	m_cComPortPDA.ClosePort();
	return false;
}

double temperatureMeasure(CPowerMeterDlg *pdlgMain) {
	// 当电桥没连接时
	if (!m_bQJ58Connect) {
		TRACE(_T("电桥未连接"));
		pdlgMain->GetDlgItem(IDC_STATIC_STATUS)->SetWindowText(_T("电桥未连接"));
		pdlgMain->GetDlgItem(IDC_STATIC_RESULT)->SetWindowText(_T("电桥未连接"));
		Sleep(500);
		return -1;
	}
	static char ValueStr[ARRAYSIZE + 1];
	const char* sendMess = "D3Measurements: ";
	ibwrt(m_iDev, sendMess, strlen(sendMess));
	ibrd(m_iDev, ValueStr, ARRAYSIZE);
	std::string readStr = ValueStr;
	// 接收值出错的情况
	if (readStr.find("Measurement") == std::string::npos) {
		TRACE(_T("获取电桥测量值出错"));
		pdlgMain->GetDlgItem(IDC_STATIC_STATUS)->SetWindowText(_T("获取电桥测量值出错"));
		pdlgMain->GetDlgItem(IDC_STATIC_RESULT)->SetWindowText(_T("获取电桥测量值出错"));
		return -1;
	}
	double tempRes = 0;
	// 根据通信协议，R 为电阻值，S 为温度值.找字符串里S的坐标
	int pos = readStr.find('S');
	if (pos != std::string::npos) {
		// 防止字符串越界，定义一下右边界
		std::string tmp(readStr, pos + 1, min(pos + 9, readStr.size()));
		tempRes = atof(tmp.c_str());
	}
	return tempRes;
}

double VoltageMeasure(CPowerMeterDlg *pdlgMain) {
	if (!m_bMultimeterConnect) {
		TRACE(_T("万用表未连接"));
		pdlgMain->GetDlgItem(IDC_STATIC_STATUS)->SetWindowText(_T("万用表未连接"));
		pdlgMain->GetDlgItem(IDC_STATIC_RESULT)->SetWindowText(_T("万用表未连接"));
		Sleep(500);
		return -1;
	}
	char pcCommand[11] = { '$','$', 'R', 'E','A','D','V','L','T','#','#' };
	DWORD dwRev = 0;
	bool m_bValidVerify = m_cComPortMultimeter.SendData(pcCommand, 11);
	if (m_bValidVerify)
	{
		// 接收值形如：$$READVLT: DC:-123.45:V##
		// 这里可能涉及到一个字符串转double的问题。之后如果有报错再来修改。
		LONG XY = m_cComPortMultimeter.RecData(pcCommand, 32, &dwRev);
		if (XY) {
			return (double)dwRev;
		}
	}
	TRACE(_T("获取万用表测量值出错"));
	pdlgMain->GetDlgItem(IDC_STATIC_STATUS)->SetWindowText(_T("获取万用表测量值出错"));
	pdlgMain->GetDlgItem(IDC_STATIC_RESULT)->SetWindowText(_T("获取万用表测量值出错"));
	return -1;
}

// 电流源设定电流值 switchState为true代表开，false代表关
bool SetCurrent(CPowerMeterDlg *pdlgMain,double measureCurrent,bool switchState) {
	if (!m_bCurrentSourceConnect) {
		TRACE(_T("电流源未连接"));
		pdlgMain->GetDlgItem(IDC_STATIC_STATUS)->SetWindowText(_T("电流源未连接"));
		pdlgMain->GetDlgItem(IDC_STATIC_RESULT)->SetWindowText(_T("电流源未连接"));
		Sleep(500);
		return false;
	}
	if (switchState) {
		// 分为3步：1、设置电流档位 2、设置电流值 3、开启输出电流

		// 1、设置电流档位:最后的0000000为1mA档，0000001为10mA档，0000002为100mA档
		std::string pcCommand = "ATV,SET,RGE,0000000;";
		DWORD dwRev = 0;
		bool m_bValidVerify = m_cComPortMultimeter.SendData((char*)pcCommand.c_str(), 21);
		if (!m_bValidVerify)
		{
			TRACE(_T("设置电流档位出错"));
			pdlgMain->GetDlgItem(IDC_STATIC_STATUS)->SetWindowText(_T("设置电流档位出错"));
			pdlgMain->GetDlgItem(IDC_STATIC_RESULT)->SetWindowText(_T("设置电流档位出错"));
			Sleep(500);
			return false;
		}
		// 2、设置电流值:参数取值范围0000000 ~ 1100000.
		// TODO: 添加电流取值。这里没看懂如何转换。
		pcCommand = "ATV,SET,CUR,";
		std::string currentVal = std::to_string(measureCurrent) + ";";
		pcCommand += currentVal;
		dwRev = 0;
		m_bValidVerify = m_cComPortMultimeter.SendData((char*)pcCommand.c_str(), 21);
		if (!m_bValidVerify)
		{
			TRACE(_T("设定电流值出错"));
			pdlgMain->GetDlgItem(IDC_STATIC_STATUS)->SetWindowText(_T("设定电流值出错"));
			pdlgMain->GetDlgItem(IDC_STATIC_RESULT)->SetWindowText(_T("设定电流值出错"));
			Sleep(500);
			return false;
		}
		// 3、打开输出开关
		pcCommand = "ATV,SET,AON,0000000;";
		dwRev = 0;
		m_bValidVerify = m_cComPortMultimeter.SendData((char*)pcCommand.c_str(), 21);
		if (!m_bValidVerify)
		{
			TRACE(_T("打开输出开关出错"));
			pdlgMain->GetDlgItem(IDC_STATIC_STATUS)->SetWindowText(_T("打开输出开关出错"));
			pdlgMain->GetDlgItem(IDC_STATIC_RESULT)->SetWindowText(_T("打开输出开关出错"));
			Sleep(500);
			return false;
		}
		pdlgMain->GetDlgItem(IDC_STATIC_STATUS)->SetWindowText(_T("设定电流值成功"));
		pdlgMain->GetDlgItem(IDC_STATIC_RESULT)->SetWindowText(_T("设定电流值成功"));
		Sleep(500);
		return true;
	}
	else {
		// 1、关闭开关
		std::string pcCommand = "ATV,SET,AOF,0000000;";
		DWORD dwRev = 0;
		bool m_bValidVerify = m_cComPortMultimeter.SendData((char*)pcCommand.c_str(), 21);
		if (!m_bValidVerify)
		{
			TRACE(_T("关闭电流源失败"));
			pdlgMain->GetDlgItem(IDC_STATIC_STATUS)->SetWindowText(_T("关闭电流源失败"));
			pdlgMain->GetDlgItem(IDC_STATIC_RESULT)->SetWindowText(_T("关闭电流源失败"));
			Sleep(500);
			return false;
		}
		pdlgMain->GetDlgItem(IDC_STATIC_STATUS)->SetWindowText(_T("关闭电流源成功"));
		pdlgMain->GetDlgItem(IDC_STATIC_RESULT)->SetWindowText(_T("关闭电流源成功"));
		Sleep(500);
		return true;
	}
}

// 快门开关函数 switchState为true代表开，false代表关
bool SetShutter(CPowerMeterDlg *pdlgMain, bool switchState) {
	char sendOpen[1] = { 0xea };  // 全开指令
	char sendClose[1] = { 0x0a }; // 全关指令
	if (!m_bShutterConnect) {
		TRACE(_T("快门未连接"));
		pdlgMain->GetDlgItem(IDC_STATIC_STATUS)->SetWindowText(_T("快门未连接"));
		pdlgMain->GetDlgItem(IDC_STATIC_RESULT)->SetWindowText(_T("快门未连接"));
		Sleep(500);
		return false;
	}
	if (switchState) {
		// 1、打开开关
		DWORD dwRev = 0;
		bool m_bValidVerify = m_cComPortMultimeter.SendData(sendOpen, 1);
		if (!m_bValidVerify)
		{
			TRACE(_T("打开快门开关出错"));
			pdlgMain->GetDlgItem(IDC_STATIC_STATUS)->SetWindowText(_T("打开快门开关出错"));
			pdlgMain->GetDlgItem(IDC_STATIC_RESULT)->SetWindowText(_T("打开快门开关出错"));
			Sleep(500);
			return false;
		}
		pdlgMain->GetDlgItem(IDC_STATIC_STATUS)->SetWindowText(_T("打开快门开关成功"));
		pdlgMain->GetDlgItem(IDC_STATIC_RESULT)->SetWindowText(_T("打开快门开关成功"));
		Sleep(500);
		return true;
	}
	else {
		// 1、关闭开关
		DWORD dwRev = 0;
		bool m_bValidVerify = m_cComPortMultimeter.SendData(sendClose, 1);
		if (!m_bValidVerify)
		{
			TRACE(_T("关闭快门开关失败"));
			pdlgMain->GetDlgItem(IDC_STATIC_STATUS)->SetWindowText(_T("关闭快门开关失败"));
			pdlgMain->GetDlgItem(IDC_STATIC_RESULT)->SetWindowText(_T("关闭快门开关失败"));
			Sleep(500);
			return false;
		}
		pdlgMain->GetDlgItem(IDC_STATIC_STATUS)->SetWindowText(_T("关闭快门开关成功"));
		pdlgMain->GetDlgItem(IDC_STATIC_RESULT)->SetWindowText(_T("关闭快门开关成功"));
		Sleep(500);
		return true;
	}
}