/*
 * bsp_uart.c
 *
 *  Created on: 2018年11月27日
 *      Author: xu
 *
 *      UART初始化配置步骤:	1.置位UCSWRST	(上电默认置位)
 *      					2.初始化UART 相关寄存器
 *      					3.初始化UART GPIO端口
 *      					4.清零UCSWRST 即使能UART串口
 *      					5.可选使能中断
 *		 使用的是USCI_A1_UART
 *       如果需要用其他的UART口,只需改基地址	UART_BASE_ADDR 和相关引脚即可
 */
#include "bsp_uart.h"

/*
 *  函数名称:UART_GPIO_Config
 *  说	明 : UART相关引脚初始化
 *  参	数 : 无
 *  返回值  : 无
 */
void UART_GPIO_Config(void)
{
	//配置TX引脚
	GPIO_setAsPeripheralModuleFunctionOutputPin(UART_GPIO_PORT,UART_GPIO_TX_PIN,GPIO_PRIMARY_MODULE_FUNCTION);

	//配置RX引脚
	GPIO_setAsPeripheralModuleFunctionInputPin(UART_GPIO_PORT,UART_GPIO_RX_PIN,GPIO_PRIMARY_MODULE_FUNCTION);
}

#if   F
/*
 *  函数名称:UART_InitConfig
 *  说	明 : 时钟选择SMCLK,波特率为9600
 *  功	能 : UART初始化,
 *  		计算公式:  http://software-dl.ti.com/msp430/msp430_public_sw/mcu/msp430/MSP430BaudRateConverter/index.html
 *  参	数 : 无
 *  返回值  : 无
 */
void UART_InitConfig(void)
{
	EUSCI_A_UART_initParam UARTInitStruct;
	UARTInitStruct.selectClockSource= EUSCI_A_UART_CLOCKSOURCE_SMCLK ;//选择SMCKL作为UART的时钟
	UARTInitStruct.clockPrescalar = 6;
	UARTInitStruct.firstModReg = 8;
	UARTInitStruct.secondModReg = 17;
	UARTInitStruct.parity = EUSCI_A_UART_NO_PARITY;
	UARTInitStruct.msborLsbFirst = EUSCI_A_UART_LSB_FIRST;		//高位现行
	UARTInitStruct.numberofStopBits = EUSCI_A_UART_ONE_STOP_BIT;
	UARTInitStruct.uartMode = EUSCI_A_UART_MODE;
	UARTInitStruct.overSampling = EUSCI_A_UART_OVERSAMPLING_BAUDRATE_GENERATION;
	EUSCI_A_UART_init(UART_BASE_ADDR,&UARTInitStruct);			//UART_A0基地址
}

#elif  T
/*
 *  函数名称:UART_InitConfig
 *  说	明 : 时钟选择SMCLK(SMCLK=1MHz),波特率为115200
 *  功	能 : UART初始化,
 *  		计算公式:  http://software-dl.ti.com/msp430/msp430_public_sw/mcu/msp430/MSP430BaudRateConverter/index.html
 *  参	数 : 无
 *  返回值  : 无
 */
void UART_InitConfig(void)
{
	EUSCI_A_UART_initParam UARTInitStruct;
	UARTInitStruct.selectClockSource= EUSCI_A_UART_CLOCKSOURCE_SMCLK ;//选择SMCKL作为UART的时钟
	UARTInitStruct.clockPrescalar = 8;
	UARTInitStruct.firstModReg = 0;
	UARTInitStruct.secondModReg = 214;
	UARTInitStruct.parity = EUSCI_A_UART_NO_PARITY;
	UARTInitStruct.msborLsbFirst = EUSCI_A_UART_LSB_FIRST;		//高位现行
	UARTInitStruct.numberofStopBits = EUSCI_A_UART_ONE_STOP_BIT;
	UARTInitStruct.uartMode = EUSCI_A_UART_MODE;
	UARTInitStruct.overSampling = EUSCI_A_UART_LOW_FREQUENCY_BAUDRATE_GENERATION;
	EUSCI_A_UART_init(UART_BASE_ADDR,&UARTInitStruct);			//UART_A0基地址
}
#endif
/*
 *  函数名称:UART_Init
 * 	说	明 : UART初始化函数,严格按照步骤配置,参考User Guide
 * 	参	数 : 无
 * 	返回值  : 无
 */
void UART_Init(void)
{
	EUSCI_A_UART_disable(UART_BASE_ADDR);
	UART_InitConfig();						//初始化UART寄存器
	UART_GPIO_Config();						//配置GPIO
	EUSCI_A_UART_enable(UART_BASE_ADDR);	//使能UART

	//可选使能中断
	EUSCI_A_UART_enableInterrupt(UART_BASE_ADDR,EUSCI_A_UART_TRANSMIT_INTERRUPT);   //使能发送中断
	EUSCI_A_UART_enableInterrupt(UART_BASE_ADDR,EUSCI_A_UART_RECEIVE_INTERRUPT);	//使能接收中断
}


/*
 *  函数名称:SendByte
 * 	说	明 : 发送字节函数
 * 	参	数 : Data:要发送的一字节数据
 * 			Length	 :发送数据的长度
 * 	返回值  : 无
 */
void SendByte(uint8_t Data)
{
	EUSCI_A_UART_transmitData(UART_BASE_ADDR,Data);
}


/*
 *  函数名称:UART_Send
 * 	说	明 : 数据发送函数,会阻塞CPU运行
 * 	参	数 : Tx_Buffer:要发送的数据
 * 			Length	 :发送数据的长度
 * 	返回值  : 无
 */
void UART_Send(uint8_t *Tx_Buffer,uint16_t Length)
{
	uint8_t i =0;

	for(i = 0; i < Length; i++)
	{
		EUSCI_A_UART_transmitData(UART_BASE_ADDR,Tx_Buffer[i]);
	}
}

/*
 *  函数名称:UART_Send_String
 * 	说	明 : 发送字符串
 * 	参	数 : Tx_Buffer:要发送的数据
 * 			Length	 :发送数据的长度
 * 	返回值  : 无
 */
void UART_Send_String(uint8_t *str)
{
	uint16_t  i=0;
	while(*(str+i) != '\0')
	{
		EUSCI_A_UART_transmitData(UART_BASE_ADDR,str[i]);
		while(!(HWREG16(UART_BASE_ADDR + OFS_UCAxIFG) & UCTXIFG));
		++i;
	}

}


/*
 *  函数名称:UART_ReceiveData
 * 	说	明 : 接收函数,会阻塞CPU运行
 * 	参	数 : Rx_Buffer:要接收的数据
 * 			Length	 :发送数据的长度
 * 	返回值  : 无
 */
void UART_ReceiveData(uint8_t *Rx_Buffer,uint8_t Length)
{
	uint16_t i =0;
	for(i=0;i<Length;i++)
	{
		Rx_Buffer[i] = EUSCI_A_UART_receiveData(UART_BASE_ADDR);
		while(!(HWREG16(UART_BASE_ADDR + OFS_UCAxIFG) & UCRXIFG));
	}
}

