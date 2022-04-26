/********************************************************************/
/*              ��ȡ��д������ʾ��             */
/*                                                                  */
/*      ��δ�����ʾ��ͬ����д���һ��                            */
/*      GPIB��ʹ�� VISA �Ĵ��л������Ϣ�� VXI ������               */
/*                                                                  */
/*      ����Ĵ���������                                            */
/*      ����Դ������                                              */
/*      �������� VISA �Ự                                        */
/*      ʹ�� viWrite ��д��ʶ��ѯ                                   */
/*      ����ʹ�� viRead ��ȡ��Ӧ                                    */
/*      �ر� VISA �Ự                                              */
/********************************************************************/

#if defined(_MSC_VER) && !defined(_CRT_SECURE_NO_DEPRECATE)
/* Functions like strcpy are technically not secure because they do */
/* not contain a 'length'. But we disable this warning for the VISA */
/* examples since we never copy more than the actual buffer size.   */
#define _CRT_SECURE_NO_DEPRECATE
#endif

#include "stdafx.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "visa.h"

static ViSession defaultRM;
static ViSession instr;
static ViStatus status;
static ViUInt32 retCount;
static ViUInt32 writeCount;
static unsigned char buffer[100];
static char stringinput[512];

/*
* In every source code or header file that you use it is necessary to prototype
* your VISA variables at the beginning of the file. You need to declare the VISA
* session, VISA integers, VISA strings, VISA pointers, and VISA floating variables.
* Remember that if you are prototyping variables that are to be used as part of the
* VISA session that need this prototyping. As an example, above retCount has been
* prototyped as a static variable to this particular module.   It is an integer of
* bit length 32. If you are uncertain how to declare your VISA prototypes refer
* to the VISA help under the Section titled Type Assignments Table. The VISA
* help is located in your NI-VISA directory or folder.

* ����ʹ�õ�ÿ��Դ�����ͷ�ļ��У�����Ҫԭ���ļ���ͷ�� VISA ������ 
* ����Ҫ����VISA�Ự��VISA ������VISA �ַ�����VISA ָ��� VISA ����������
* ���ס����������ڶ�Ҫ������һ���ֵı�������ԭ�����
* ��Ҫ��ԭ����Ƶ� VISA �Ự�� ���磬����� retCount ��
* ԭ�ͻ�Ϊ����ض�ģ��ľ�̬������ ����һ������
* λ�� 32���������ȷ������������� VISA ԭ�ͣ������
* �ڱ���Ϊ���ͷ����Ĳ����µ� VISA �����С� ǩ֤
* ����λ������ NI-VISA Ŀ¼���ļ����С�
*/

int main(void)
{
	/*
	* First we must call viOpenDefaultRM to get the resource manager
	* handle.  We will store this handle in defaultRM.

	* �������Ǳ������ viOpenDefaultRM ����ȡ��Դ������
	* ���� ���ǽ����������洢�� defaultRM �С�
	*/
	status = viOpenDefaultRM(&defaultRM);
	if (status < VI_SUCCESS)
	{
		printf("�޷����� VISA ��Դ�������ĻỰ!\n");
		exit(EXIT_FAILURE);
	}

	/*
	* Now we will open a VISA session to a device at Primary Address 2.
	* You can use any address for your instrument. In this example we are
	* using GPIB Primary Address 2.

	* �������ǽ���һ��������ַ 2 ���豸�� VISA �Ự��
	* ������Ϊ��������ʹ���κε�ַ�� ����������У�������
	* ʹ�� GPIB ����ַ 2��������ʵ��ʹ�õ�ַΪ5��

	*
	* We must use the handle from viOpenDefaultRM and we must
	* also use a string that indicates which instrument to open.  This
	* is called the instrument descriptor.  The format for this string
	* can be found in the NI-VISA User Manual.

	* ���Ǳ���ʹ������ viOpenDefaultRM �ľ���������Ǳ���
	* ��ʹ��һ���ַ�����ָʾҪ�򿪵Ĺ��ߡ� ���Ϊ������������ 
	* ���ַ����ĸ�ʽ���� NI-VISA �û��ֲ����ҵ���

	* After opening a session to the device, we will get a handle to
	* the instrument which we will use in later VISA functions.
	* The two parameters in this function which are left blank are
	* reserved for future functionality.  These two parameters are
	* given the value VI_NULL.

	* ���豸�Ự�����ǽ����һ������������ǽ����Ժ�� VISA ������ʹ�õĹ��ߡ�
	* �ú��������յ�����������Ϊ�����Ĺ��ܱ����� �����������Ǹ���ֵ VI_NULL��
	*
	* This example will also work for serial or VXI instruments by changing
	* the instrument descriptor from GPIB0::2::INSTR to ASRL1::INSTR or
	* VXI0::2::INSTR depending on the necessary descriptor for your
	* instrument.

	* ��ʾ��Ҳ�����ڴ��л� VXI ������ֻ��ı�����������
	* �� GPIB0::2::INSTR �ĵ� ASRL1::INSTR ��
	* VXI0::2::INSTR ��ȡ��������������Ҫ������
	*/
	status = viOpen(defaultRM, "GPIB0::5::INSTR", VI_NULL, VI_NULL, &instr);
	if (status < VI_SUCCESS)
	{
		printf("�޷������豸�ĻỰ��\n");
		goto Close;
	}

	/*
	* Set timeout value to 5000 milliseconds (5 seconds).
	* ����ʱֵ����Ϊ 5000 ���루5 �룩��
	*/
	status = viSetAttribute(instr, VI_ATTR_TMO_VALUE, 5000);

	/*
	* At this point we now have a session open to the instrument at
	* Primary Address 2.  We can use this session handle to write
	* an ASCII command to the instrument.  We will use the viWrite function
	* to send the string "*IDN?", asking for the device's identification.

	* ��ʱ������������һ��λ��Primary Address 2�Ķ��������ŵĻỰ��
	* ���ǿ���������Ự�����д������ ASCII ��� 
	* ���ǽ�ʹ�� viWrite ���������ַ�����*IDN?����ѯ���豸�ı�ʶ��
	*/
	strcpy(stringinput, "*IDN?\r\n");
	status = viWrite(instr, (ViBuf)stringinput, (ViUInt32)strlen(stringinput), &writeCount);
	if (status < VI_SUCCESS)
	{
		printf("д���豸ʱ����\n");
		goto Close;
	}

	/*
	* Now we will attempt to read back a response from the device to
	* the identification query that was sent.  We will use the viRead
	* function to acquire the data.  We will try to read back 100 bytes.
	* After the data has been read the response is displayed.

	* �������ǽ����Դ��豸��ȡ�ѷ��͵ı�ʶ��ѯ��Ӧ��
	* ���ǽ�ʹ�� viRead��ȡ���ݵĺ����� ���ǽ����Իض� 100 ���ֽڡ�
	* ��ȡ���ݺ���ʾ��Ӧ��
	*/
	status = viRead(instr, buffer, 100, &retCount);
	if (status < VI_SUCCESS)
	{
		printf("���豸��ȡ��Ӧʱ����\n");
	}
	else
	{
		printf("Data read: %*s\n", retCount, buffer);
	}


	/*
	* Now we will close the session to the instrument using
	* viClose. This operation frees all system resources.
	* �������ǽ�ʹ��viClose�ر��������ĻỰ���˲����ͷ�����ϵͳ��Դ��
	*/
Close:
	printf("�ر�ͨ��\n���س�����.");
	fflush(stdin);
	getchar();
	status = viClose(instr);
	status = viClose(defaultRM);

	return 0;
}
