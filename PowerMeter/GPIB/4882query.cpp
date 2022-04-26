/*
* Filename - 4882query.c
*
* This sample application is comprised of three basic parts:
*
*  1. Initialization ��ʼ��
*  2. Main Body ����
*  3. Cleanup ����

*
* The Initialization portion consists of initializing the bus and the
* GPIB interface board so that the GPIB board is Controller-In-Charge
* (CIC). Next it finds all the listeners and then clears all the
* devices on the bus.

* ��ʼ�����ְ�����ʼ�����ߺ�GPIB �ӿڰ壬
* ʹ GPIB ��Ϊ���ܿ�����(CIC)�� �����������ҵ����е���������
* Ȼ��������������ϵ��豸��
*
* In the Main Body, this application queries a device for its
* identification code by issuing the '*IDN?' command. Many
* instruments respond to this command with an identification string.
* Note, 488.2 compliant devices are required to respond to this
* command.

* �������У���Ӧ�ó����ѯ�豸��ͨ��������*IDN������ʶ�������� 
* �������ʹ�ñ�ʶ�ַ�����Ӧ�����
* ע�⣬488.2 �����豸��Ҫ�Դ�������Ӧ���
*
* The last step, Cleanup, takes the board offline.
* ���һ��������ʹ��·���ѻ���
*/

#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>

#include "ni4882.h"

#define ARRAYSIZE  100                 // Size of read buffer
#define GPIB0        0                 // Board handle

static int        Num_Listeners;              // Number of listeners on GPIB GPIB �ϵ�����������
static int        loop;                       // Loop counter ѭ��������
static Addr4882_t Instruments[32];            // Array of primary addresses ����ַ����
static Addr4882_t Result[31];                 // Array of listen addresses ������ַ����
static char       ReadBuffer[ARRAYSIZE + 1];  // Read data buffer ��ȡ���ݻ�����
static char       ErrorMnemonic[29][5] = {
	"EDVR", "ECIC", "ENOL", "EADR", "EARG",
	"ESAC", "EABO", "ENEB", "EDMA", "",
	"EOIP", "ECAP", "EFSO", "",     "EBUS",
	"ESTB", "ESRQ", "",     "",      "",
	"ETAB", "ELCK", "EARM", "EHDL",  "",
	"",     "EWIP", "ERST", "EPWR" };


static void GPIBCleanup(int ud, const char * ErrorMsg);


int __cdecl main(void)
{
	/*
	* ====================================================================
	*
	*  INITIALIZATION SECTION ��ʼ������
	*
	* ====================================================================
	*/

	/*
	* Your board needs to be the Controller-In-Charge in order to find
	* all listeners on the GPIB.  To accomplish this, the function
	* SendIFC is called.  If the error bit ERR is set in ibsta, call
	* GPIBCleanup with an error message.

	* ���Ŀ�������Ҫ�����ܿ����������ҵ�GPIB �ϵ�������������
	* Ϊ�ˣ�����SendIFC �����á� 
	* ����� ibsta �������˴���λ ERR������ô��д�����Ϣ�� GPIBCleanup��
	*/
	SendIFC(GPIB0);
	if (Ibsta() & ERR)
	{
		GPIBCleanup(GPIB0, "�޷��򿪿�����");
		return 1;
	}

	/*
	* Create an array containing all valid GPIB primary addresses,
	* except for primary address 0.  Your GPIB interface board is at
	* address 0 by default.  This array (Instuments) will be given to
	* the function FindLstn to find all listeners.  The constant NOADDR,
	* defined in NI488.H, signifies the end of the array.

	* ����һ������������Ч GPIB ����ַ�����飬����ַ 0 ���⡣
	* ���� GPIB �ӿڰ�λ��Ĭ�ϵ�ַΪ 0�� 
	* ������飨�������������躯�� FindLstn ���������������� 
	* ���� NOADDR���� NI488.H �ж��壬��ʾ����Ľ�����
	*/
	for (loop = 0; loop < 30; loop++)
	{
		Instruments[loop] = (Addr4882_t)(loop + 1);
	}
	Instruments[30] = NOADDR;

	/*
	* Print message to tell user that the program is searching for all
	* active listeners.  Find all of the listeners on the bus.  Store
	* the listen addresses in the array Result.  Note, the instruments
	* must be powered on and connected with a GPIB cable in order for
	* FindLstn to detect them.If the error bit ERR is set in ibsta, call
	* GPIBCleanup with an error message.

	* ��ӡ��Ϣ�����û����������������л�Ծ�ļ����� 
	* �ҵ������ϵ����м����� ���������ַ������ Result �С�
	* ע�⣬��������򿪵�Դ��ʹ�� GPIB �����Ӳ���ʹ��
	* FindLstn ��������ǡ������ ibsta �������˴���λ ERR��
	* ����ô��д�����Ϣ�� GPIBCleanup��
	*/
	printf("�ҵ������ϵ����м���......\n\n");

	FindLstn(GPIB0, Instruments, Result, 31);
	if (Ibsta() & ERR)
	{
		GPIBCleanup(GPIB0, "�޷����� FindLstn ����");
		return 1;
	}

	/*
	* ibcntl contains the actual number of addresses stored in the
	* Result array. Assign the value of ibcntl to the variable
	* Num_Listeners. Print the number of listeners found.

	* ibcntl ���� Result ������ʵ�ʴ洢�ĵ�ַ����
	* �� ibcntl ��ֵ��������Num_Listeners�� 
	* ��ӡ�ҵ���������������
	*/
	Num_Listeners = Ibcnt();

	printf("�ҵ����������� = %d\n\n", Num_Listeners);

	/*
	* The Result array contains the addresses of all listening devices
	* found by FindLstn. Use the constant NOADDR, as defined in
	* ni4882.h, to signify the end of the array.

	* Result ������������� FindLstn �ҵ��ļ����豸�ĵ�ַ��
	* ʹ�ö�����ni4882.h�ĳ��� NOADDR����ʾ����Ľ�����
	*/
	Result[Num_Listeners] = NOADDR;

	/*
	* DevClearList will send the GPIB Selected Device Clear (SDC)
	* command message to all the devices on the bus.  If the error bit
	* ERR is set in ibsta, call GPIBCleanup with an error message.

	* DevClearList ������ GPIB ѡ���豸���(SDC)������Ϣ�������ϵ������豸��
	* �������λERR �� ibsta �����ã����� GPIBCleanup ����ʾ������Ϣ��
	*/
	DevClearList(GPIB0, Result);
	if (Ibsta() & ERR)
	{
		GPIBCleanup(GPIB0, "�޷�����豸");
		return 1;
	}

	/*
	* ====================================================================
	*
	* MAIN BODY SECTION ���岿��
	*
	* In this application, the Main Body communicates with the
	* instruments by writing a command to them and reading the individual
	* responses. This would be the right place to put other instrument
	* communication.

	* �����Ӧ�ó����У�����ͨ��������д�������ȡ������Ӧ������������ͨ�š�
	* �⽫�Ƿ�����������ͨ�ŵĺ���λ�á�
	*
	* ====================================================================
	*/

	/*
	* Send the identification query to each listen address in the array
	* (Result) using SendList.  The constant NLend, defined in NI488.H,
	* instructs the function SendList to append a linefeed character
	* with EOI asserted to the end of the message.  If the error bit ERR
	* is set in ibsta, call GPIBCleanup with an error message.

	* ʹ�� SendList �����飨Result���е�ÿ��������ַ���ͱ�ʶ��ѯ��
	* �� NI488.H �ж���ĳ��� NLend ָʾ���� SendList ����Ϣĩβ����һ�����з��� EOI ���ԡ�
	* ����� ibsta �������˴���λ ERR����ʹ�� GPIBCleanup ���ش�����Ϣ���á�
	*/
	SendList(GPIB0, Result, "*IDN?", 5L, NLend);
	if (Ibsta() & ERR)
	{
		GPIBCleanup(GPIB0, "�޷�д���豸");
		return 1;
	}

	/*
	* Read each device's identification code, one at a time.
	*
	* Establish a FOR loop to read each one of the device's
	* identification code. The variable LOOP will serve as a counter
	* for the FOR loop and as the index to the array Result.

	* ��ȡÿ���豸��ʶ���룬һ��һ����
	*
	* ����һ�� FOR ѭ������ȡÿ���豸��ʶ���롣 
	* ���� LOOP ����Ϊ���������� FOR ѭ������Ϊ���� Result ��������
	*/
	for (loop = 0; loop < Num_Listeners; loop++)
	{
		/*
		* Read the name identification response returned from each
		* device. Store the response in the array ReadBuffer.  The
		* constant STOPend, defined in NI488.H, instructs the
		* function Receive to terminate the read when END is detected.
		* If the error bit ERR is set in ibsta, call GPIBCleanup with
		* an error message.

		* ��ȡÿ���豸���ص�����ʶ����Ӧ������Ӧ�洢������ ReadBuffer �С�
		* �� NI488.H �ж���ĳ��� STOPend ָʾ���� Receive �ڼ�⵽ END ʱ��ֹ��ȡ��
		* ����� ibsta �������˴���λ ERR����ʹ�� GPIBCleanup ���ش�����Ϣ���á�
		*/
		Receive(GPIB0, Result[loop], ReadBuffer, ARRAYSIZE, STOPend);
		if (Ibsta() & ERR)
		{
			GPIBCleanup(GPIB0, "�޷����豸��ȡ");
			return 1;
		}

		/*
		* Assume that the returned string contains ASCII data. NULL
		* terminate the string using the value in ibcntl which is
		* the number of bytes read in. Use printf to display the
		* string.

		* ���践�ص��ַ������� ASCII ���ݡ�
		* NULL ʹ�� ibcntl �е�ֵ��ֹ�ַ�������ֵ�Ƕ�����ֽ�����
		* ʹ�� printf ��ʾ�ַ�����
		*/
		ReadBuffer[Ibcnt()] = '\0';
		printf("���ص��ַ���: %s", ReadBuffer);

	} /* End of FOR loop */

	  /*
	  * ====================================================================
	  *
	  * CLEANUP SECTION ������
	  *
	  * ====================================================================
	  */

	  /*
	  * Take the board offline.
	  */
	ibonl(GPIB0, 0);

	return 0;
}


/*
* After each GPIB call, the application checks whether the call
* succeeded. If an NI-488.2 call fails, the GPIB driver sets the
* corresponding bit in the global status variable. If the call
* failed, this procedure prints an error message, takes the board
* offline and exits.

* ÿ��GPIB���ú�Ӧ�ó���������Ƿ�ɹ�
* ��� NI-488.2 ����ʧ�ܣ�GPIB �������������ȫ��״̬�����е���Ӧλ��
* �������ʧ�ܣ��˹��̽���ӡһ��������Ϣ��ʹ���ѻ����˳���
*/
void GPIBCleanup(int ud, const char * ErrorMsg)
{
	printf("Error : %s\nibsta = GPIB0x%x iberr = %d (%s)\n",
		ErrorMsg, Ibsta(), Iberr(), ErrorMnemonic[Iberr()]);
	printf("Cleanup: Taking board offline\n");
	ibonl(ud, 0);
}
