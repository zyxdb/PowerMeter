
// PowerMeter.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CPowerMeterApp:
// �йش����ʵ�֣������ PowerMeter.cpp
//

class CPowerMeterApp : public CWinApp
{
public:
	CPowerMeterApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

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