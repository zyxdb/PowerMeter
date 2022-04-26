/*
* Filename - 4882query.c
*
* This sample application is comprised of three basic parts:
*
*  1. Initialization 初始化
*  2. Main Body 主体
*  3. Cleanup 清理

*
* The Initialization portion consists of initializing the bus and the
* GPIB interface board so that the GPIB board is Controller-In-Charge
* (CIC). Next it finds all the listeners and then clears all the
* devices on the bus.

* 初始化部分包括初始化总线和GPIB 接口板，
* 使 GPIB 板为主管控制器(CIC)。 接下来它会找到所有的侦听器，
* 然后清除所有总线上的设备。
*
* In the Main Body, this application queries a device for its
* identification code by issuing the '*IDN?' command. Many
* instruments respond to this command with an identification string.
* Note, 488.2 compliant devices are required to respond to this
* command.

* 在主体中，此应用程序查询设备的通过发出“*IDN？”来识别代码命令。 
* 许多仪器使用标识字符串响应此命令。
* 注意，488.2 兼容设备需要对此做出响应命令。
*
* The last step, Cleanup, takes the board offline.
* 最后一步，清理，使电路板脱机。
*/

#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>

#include "ni4882.h"

#define ARRAYSIZE  100                 // Size of read buffer
#define GPIB0        0                 // Board handle

static int        Num_Listeners;              // Number of listeners on GPIB GPIB 上的侦听器数量
static int        loop;                       // Loop counter 循环计数器
static Addr4882_t Instruments[32];            // Array of primary addresses 主地址数组
static Addr4882_t Result[31];                 // Array of listen addresses 监听地址数组
static char       ReadBuffer[ARRAYSIZE + 1];  // Read data buffer 读取数据缓冲区
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
	*  INITIALIZATION SECTION 初始化部分
	*
	* ====================================================================
	*/

	/*
	* Your board needs to be the Controller-In-Charge in order to find
	* all listeners on the GPIB.  To accomplish this, the function
	* SendIFC is called.  If the error bit ERR is set in ibsta, call
	* GPIBCleanup with an error message.

	* 您的开发板需要是主管控制器才能找到GPIB 上的所有侦听器。
	* 为此，函数SendIFC 被调用。 
	* 如果在 ibsta 中设置了错误位 ERR，请调用带有错误消息的 GPIBCleanup。
	*/
	SendIFC(GPIB0);
	if (Ibsta() & ERR)
	{
		GPIBCleanup(GPIB0, "无法打开开发板");
		return 1;
	}

	/*
	* Create an array containing all valid GPIB primary addresses,
	* except for primary address 0.  Your GPIB interface board is at
	* address 0 by default.  This array (Instuments) will be given to
	* the function FindLstn to find all listeners.  The constant NOADDR,
	* defined in NI488.H, signifies the end of the array.

	* 创建一个包含所有有效 GPIB 主地址的数组，主地址 0 除外。
	* 您的 GPIB 接口板位于默认地址为 0。 
	* 这个数组（仪器）将被赋予函数 FindLstn 查找所有侦听器。 
	* 常数 NOADDR，在 NI488.H 中定义，表示数组的结束。
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

	* 打印消息告诉用户程序正在搜索所有活跃的监听。 
	* 找到总线上的所有监听。 储存监听地址于数组 Result 中。
	* 注意，仪器必须打开电源并使用 GPIB 线连接才能使用
	* FindLstn 来检测它们。如果在 ibsta 中设置了错误位 ERR，
	* 则调用带有错误消息的 GPIBCleanup。
	*/
	printf("找到总线上的所有监听......\n\n");

	FindLstn(GPIB0, Instruments, Result, 31);
	if (Ibsta() & ERR)
	{
		GPIBCleanup(GPIB0, "无法发出 FindLstn 调用");
		return 1;
	}

	/*
	* ibcntl contains the actual number of addresses stored in the
	* Result array. Assign the value of ibcntl to the variable
	* Num_Listeners. Print the number of listeners found.

	* ibcntl 包含 Result 数组中实际存储的地址数。
	* 将 ibcntl 的值赋给变量Num_Listeners。 
	* 打印找到的侦听器数量。
	*/
	Num_Listeners = Ibcnt();

	printf("找到的仪器数量 = %d\n\n", Num_Listeners);

	/*
	* The Result array contains the addresses of all listening devices
	* found by FindLstn. Use the constant NOADDR, as defined in
	* ni4882.h, to signify the end of the array.

	* Result 数组包含所有由 FindLstn 找到的监听设备的地址。
	* 使用定义在ni4882.h的常量 NOADDR，表示数组的结束。
	*/
	Result[Num_Listeners] = NOADDR;

	/*
	* DevClearList will send the GPIB Selected Device Clear (SDC)
	* command message to all the devices on the bus.  If the error bit
	* ERR is set in ibsta, call GPIBCleanup with an error message.

	* DevClearList 将发送 GPIB 选定设备清除(SDC)命令消息到总线上的所有设备。
	* 如果错误位ERR 在 ibsta 中设置，调用 GPIBCleanup 并显示错误消息。
	*/
	DevClearList(GPIB0, Result);
	if (Ibsta() & ERR)
	{
		GPIBCleanup(GPIB0, "无法清除设备");
		return 1;
	}

	/*
	* ====================================================================
	*
	* MAIN BODY SECTION 主体部分
	*
	* In this application, the Main Body communicates with the
	* instruments by writing a command to them and reading the individual
	* responses. This would be the right place to put other instrument
	* communication.

	* 在这个应用程序中，主体通过向仪器写入命令并读取各个响应来与仪器进行通信。
	* 这将是放置其他仪器通信的合适位置。
	*
	* ====================================================================
	*/

	/*
	* Send the identification query to each listen address in the array
	* (Result) using SendList.  The constant NLend, defined in NI488.H,
	* instructs the function SendList to append a linefeed character
	* with EOI asserted to the end of the message.  If the error bit ERR
	* is set in ibsta, call GPIBCleanup with an error message.

	* 使用 SendList 向数组（Result）中的每个监听地址发送标识查询。
	* 在 NI488.H 中定义的常量 NLend 指示函数 SendList 在消息末尾附加一个换行符和 EOI 断言。
	* 如果在 ibsta 中设置了错误位 ERR，请使用 GPIBCleanup 返回错误消息调用。
	*/
	SendList(GPIB0, Result, "*IDN?", 5L, NLend);
	if (Ibsta() & ERR)
	{
		GPIBCleanup(GPIB0, "无法写入设备");
		return 1;
	}

	/*
	* Read each device's identification code, one at a time.
	*
	* Establish a FOR loop to read each one of the device's
	* identification code. The variable LOOP will serve as a counter
	* for the FOR loop and as the index to the array Result.

	* 读取每个设备的识别码，一次一个。
	*
	* 建立一个 FOR 循环来读取每个设备的识别码。 
	* 变量 LOOP 将作为计数器用于 FOR 循环并作为数组 Result 的索引。
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

		* 读取每个设备返回的名称识别响应。将响应存储在数组 ReadBuffer 中。
		* 在 NI488.H 中定义的常量 STOPend 指示函数 Receive 在检测到 END 时终止读取。
		* 如果在 ibsta 中设置了错误位 ERR，请使用 GPIBCleanup 返回错误消息调用。
		*/
		Receive(GPIB0, Result[loop], ReadBuffer, ARRAYSIZE, STOPend);
		if (Ibsta() & ERR)
		{
			GPIBCleanup(GPIB0, "无法从设备读取");
			return 1;
		}

		/*
		* Assume that the returned string contains ASCII data. NULL
		* terminate the string using the value in ibcntl which is
		* the number of bytes read in. Use printf to display the
		* string.

		* 假设返回的字符串包含 ASCII 数据。
		* NULL 使用 ibcntl 中的值终止字符串，该值是读入的字节数。
		* 使用 printf 显示字符串。
		*/
		ReadBuffer[Ibcnt()] = '\0';
		printf("返回的字符串: %s", ReadBuffer);

	} /* End of FOR loop */

	  /*
	  * ====================================================================
	  *
	  * CLEANUP SECTION 清理部分
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

* 每次GPIB调用后，应用程序检查调用是否成功
* 如果 NI-488.2 调用失败，GPIB 驱动程序会设置全局状态变量中的相应位。
* 如果调用失败，此过程将打印一条错误消息，使板脱机并退出。
*/
void GPIBCleanup(int ud, const char * ErrorMsg)
{
	printf("Error : %s\nibsta = GPIB0x%x iberr = %d (%s)\n",
		ErrorMsg, Ibsta(), Iberr(), ErrorMnemonic[Iberr()]);
	printf("Cleanup: Taking board offline\n");
	ibonl(ud, 0);
}
