
// PowerMeter.h : PROJECT_NAME 应用程序的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号


// CPowerMeterApp:
// 有关此类的实现，请参阅 PowerMeter.cpp
//

class CPowerMeterApp : public CWinApp
{
public:
	CPowerMeterApp();

// 重写
public:
	virtual BOOL InitInstance();

// 实现

	DECLARE_MESSAGE_MAP()
};

extern CPowerMeterApp theApp;


typedef struct {
	BYTE  bFlag;
	BYTE  bState;
	DWORD dwIdex;
	double dbTemperature;
	double dbVoltage;
	double dbOutputCurrent;
} MeasureData, *PMeasureData;


#define THREAD_SELFCHECK_STATUS 0  
#define THREAD_MEASURE_STATUS   1  
#define THREAD_MEASURE_DATA  2  


#define THREAD_STATE_IDEL		0
#define THREAD_STATE_RUNNING	1  
#define THREAD_STATE_SUCCESS	0x7fffffff 
#define THREAD_STATE_ERROR		-1
#define THREAD_STATE_TERMINATED	-2
#define THREAD_STATE_PARAMS		-3