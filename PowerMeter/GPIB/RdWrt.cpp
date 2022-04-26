/********************************************************************/
/*              读取和写入仪器示例             */
/*                                                                  */
/*      这段代码演示了同步读写命令到一个                            */
/*      GPIB，使用 VISA 的串行或基于消息的 VXI 仪器。               */
/*                                                                  */
/*      代码的大致流程是                                            */
/*      打开资源管理器                                              */
/*      打开仪器的 VISA 会话                                        */
/*      使用 viWrite 编写标识查询                                   */
/*      尝试使用 viRead 读取响应                                    */
/*      关闭 VISA 会话                                              */
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

* 在您使用的每个源代码或头文件中，都需要原型文件开头的 VISA 变量。 
* 您需要声明VISA会话、VISA 整数、VISA 字符串、VISA 指针和 VISA 浮动变量。
* 请记住，如果您正在对要用作的一部分的变量进行原型设计
* 需要此原型设计的 VISA 会话。 例如，上面的 retCount 是
* 原型化为这个特定模块的静态变量。 它是一个整数
* 位长 32。如果您不确定如何声明您的 VISA 原型，请参阅
* 在标题为类型分配表的部分下的 VISA 帮助中。 签证
* 帮助位于您的 NI-VISA 目录或文件夹中。
*/

int main(void)
{
	/*
	* First we must call viOpenDefaultRM to get the resource manager
	* handle.  We will store this handle in defaultRM.

	* 首先我们必须调用 viOpenDefaultRM 来获取资源管理器
	* 处理。 我们将把这个句柄存储在 defaultRM 中。
	*/
	status = viOpenDefaultRM(&defaultRM);
	if (status < VI_SUCCESS)
	{
		printf("无法打开与 VISA 资源管理器的会话!\n");
		exit(EXIT_FAILURE);
	}

	/*
	* Now we will open a VISA session to a device at Primary Address 2.
	* You can use any address for your instrument. In this example we are
	* using GPIB Primary Address 2.

	* 现在我们将打开一个到主地址 2 的设备的 VISA 会话。
	* 您可以为您的仪器使用任何地址。 在这个例子中，我们是
	* 使用 GPIB 主地址 2。（代码实际使用地址为5）

	*
	* We must use the handle from viOpenDefaultRM and we must
	* also use a string that indicates which instrument to open.  This
	* is called the instrument descriptor.  The format for this string
	* can be found in the NI-VISA User Manual.

	* 我们必须使用来自 viOpenDefaultRM 的句柄并且我们必须
	* 还使用一个字符串来指示要打开的工具。 这称为仪器描述符。 
	* 此字符串的格式可在 NI-VISA 用户手册中找到。

	* After opening a session to the device, we will get a handle to
	* the instrument which we will use in later VISA functions.
	* The two parameters in this function which are left blank are
	* reserved for future functionality.  These two parameters are
	* given the value VI_NULL.

	* 打开设备会话后，我们将获得一个句柄——我们将在以后的 VISA 功能中使用的工具。
	* 该函数中留空的两个参数是为将来的功能保留。 这两个参数是给定值 VI_NULL。
	*
	* This example will also work for serial or VXI instruments by changing
	* the instrument descriptor from GPIB0::2::INSTR to ASRL1::INSTR or
	* VXI0::2::INSTR depending on the necessary descriptor for your
	* instrument.

	* 此示例也适用于串行或 VXI 仪器，只需改变仪器描述符
	* 从 GPIB0::2::INSTR 改到 ASRL1::INSTR 或
	* VXI0::2::INSTR ，取决于您的仪器必要描述。
	*/
	status = viOpen(defaultRM, "GPIB0::5::INSTR", VI_NULL, VI_NULL, &instr);
	if (status < VI_SUCCESS)
	{
		printf("无法打开与设备的会话。\n");
		goto Close;
	}

	/*
	* Set timeout value to 5000 milliseconds (5 seconds).
	* 将超时值设置为 5000 毫秒（5 秒）。
	*/
	status = viSetAttribute(instr, VI_ATTR_TMO_VALUE, 5000);

	/*
	* At this point we now have a session open to the instrument at
	* Primary Address 2.  We can use this session handle to write
	* an ASCII command to the instrument.  We will use the viWrite function
	* to send the string "*IDN?", asking for the device's identification.

	* 此时，我们现在有一个位于Primary Address 2的对仪器开放的会话，
	* 我们可以用这个会话句柄来写仪器的 ASCII 命令。 
	* 我们将使用 viWrite 函数发送字符串“*IDN?”，询问设备的标识。
	*/
	strcpy(stringinput, "*IDN?\r\n");
	status = viWrite(instr, (ViBuf)stringinput, (ViUInt32)strlen(stringinput), &writeCount);
	if (status < VI_SUCCESS)
	{
		printf("写入设备时出错\n");
		goto Close;
	}

	/*
	* Now we will attempt to read back a response from the device to
	* the identification query that was sent.  We will use the viRead
	* function to acquire the data.  We will try to read back 100 bytes.
	* After the data has been read the response is displayed.

	* 现在我们将尝试从设备读取已发送的标识查询响应。
	* 我们将使用 viRead获取数据的函数。 我们将尝试回读 100 个字节。
	* 读取数据后显示响应。
	*/
	status = viRead(instr, buffer, 100, &retCount);
	if (status < VI_SUCCESS)
	{
		printf("从设备读取响应时出错\n");
	}
	else
	{
		printf("Data read: %*s\n", retCount, buffer);
	}


	/*
	* Now we will close the session to the instrument using
	* viClose. This operation frees all system resources.
	* 现在我们将使用viClose关闭与仪器的会话。此操作释放所有系统资源。
	*/
Close:
	printf("关闭通信\n按回车继续.");
	fflush(stdin);
	getchar();
	status = viClose(instr);
	status = viClose(defaultRM);

	return 0;
}
