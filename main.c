/******************************************************************************
 *******************************copyright**************************************
 ******************************************************************************/

/******************************************************************************
 *
 * 		Author	: Xuzhi Liu
 * 		Date	: 2018.11.29
 * 		Brief	: 用于CC1101无线射频收发通信
 * 				   自己写的文件的头文件也放进driverlib.h文件中
 *
 *
 * 			History
 * 					1.Author	: Xuzhi Liu
 * 					  Date		: 2018.11.29
 * 					  Mod		: 1.初次使用MSP430单片机,测试uart通信,使用USCI_A1_UART
 * 					  			        硬件连接:	TX --> P3.4		EUSCI_A1_UART
 * 					  			  			RX --> P3.5
 * 					  			        在driverlib.h文件中添加条件编译条件宏定义
 * 					  			  2.修改系统时钟配置
 * 					  			  				ACLK:	32Hz
 * 					  			  				SMCLK：   4MHz
 * 					  			  				MCLK :  4MHz
 *
 *
 * 					2.Author	: Xuzhi Liu
 * 					  Date		: 2018.11.30
 * 					  Mod		: 1.配置SPI通信,使用USCI_B1_SPI
 * 					  			        硬件连接:
 * 					  			  		   SPI_CLK  --> P4.2
 * 					  			  		   SPI_CS   --> P4.3
 * 					  			  		   SPI_MOSI --> P4.0
 * 					  			  		   SPI_MISO --> P4.1
 * 					  			   2.添加延时函数,基于CPU_CLOCK=8MHz
 *
 *
 *					3.Author	: Xuzhi Liu
 * 					  Date		: 2018.12.03
 * 					  Mod		: 添加CC1101文件
 * 					  			  增加硬件连接:
 * 					  			  		   GDIO0 --> P2.0
 *
 ***********************************************************************************/

#include "driverlib.h"

int main(void)
{
	uint8_t Tx_Buffer[5]={0x59,0x02,0x32,0x04,0x22};   //定义要发送的数据
    WDT_A_hold(WDT_A_BASE);		// Stop watchdog timer
    PMM_unlockLPM5();
    GPIO_InitConfig();
//   CS_Init_Config();
    LED_Config();
    UART_Init();
    SPI_Init();

    CC1101_Reset();
	CC1101_Init();
	Write_CMD(SFTX);
//    SendByte(data);
	CC1101_RFDataPack_Send(Tx_Buffer, sizeof(Tx_Buffer));
    while(1)
    {
#if			F
/*******************************************************************
    *	Author	:	xuzhi Liu
    *	Date	:	2018.12.03
    *	Function:	闪灯测试
*******************************************************************/
/*    	UART_Send_String("\n成功完成UART通讯了，哈哈\n");
    	GPIO_toggleOutputOnPin(LED1_GPIO_PORT,LED1_GPIO_PIN);
    	Delay_ms(100);*/
    	Write_Data(IOCFG2_ADDR,GDO_LOW);		//熄灭GDO0
    	Write_Data(IOCFG0_ADDR,GDO_LOW);		//熄灭GDO2
//    	UART_Send_String("\n熄灭LED\n");
    	Delay_ms(1000);				//延时1s
    	Write_Data(IOCFG0_ADDR,GDO_HIGH);		//点亮GDO0
    	Write_Data(IOCFG2_ADDR,GDO_HIGH);		//点亮GDO2
//    	UART_Send_String("\n点亮LED\n");
    	Delay_ms(1000);

#elif		F
/*******************************************************************
    *	Author	:	xuzhi Liu
    *	Date	:	2018.12.03
    *	Function:	收发测试
*******************************************************************/

    	Test_Write_Read_byte(IOCFG0_ADDR,0x12,IOCFG0_ADDR|READ_SINGLE, READ_SINGLE);
    	Delay_ms(1000);

#elif		T
/*******************************************************************
	*	Author	:	xuzhi Liu
	*	Date	:	2018.12.03
	*	Function:	发送数据包
*******************************************************************/
//    	CC1101_RFDataPack_Send(Tx_Buffer, sizeof(Tx_Buffer));
//		Delay_ms(1000);							//延时1s

#endif

    }
}

