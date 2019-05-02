/*
 * bsp_spi.c
 *
 *  Created on: 2018年11月30日
 *      Author: xu
 *      SPI初始化配置			【复用功能通过datasheet里查找】
 *
 *      		硬件说明:
 *      		 	EUSCI_B1_SPI:						//IO复用改为Secondary module function is selected
 *      						SPI_CLK  --> P4.2
* 					  		    SPI_CS   --> P4.3
* 					  			SPI_MOSI --> P4.0
* 					  			SPI_MISO --> P4.1
 *
 *      			EUSCI_A0_SPI:						//IO复用改为Primary module function is selected
 *      						SPI_CLK  --> P2.2
* 					  		    SPI_CS   --> P2.3
* 					  			SPI_MOSI --> P4.2
* 					  			SPI_MISO --> P4.3
 */

#include "bsp_spi.h"

/*
 *  说	明 : SPI相关引脚初始化
 *  参	数 : 无
 *  返回值  : 无
 */
void SPI_GPIO_Config(void)
{
	//CLK
	GPIO_setAsPeripheralModuleFunctionOutputPin(SPI_CLK_PORT,SPI_CLK_PIN,GPIO_SECONDARY_MODULE_FUNCTION);

	//MOSI
	GPIO_setAsPeripheralModuleFunctionOutputPin(SPI_MOSI_PORT,SPI_MOSI_PIN,GPIO_SECONDARY_MODULE_FUNCTION);

	//MISO
	GPIO_setAsPeripheralModuleFunctionInputPin(SPI_MISO_PORT,SPI_MISO_PIN,GPIO_SECONDARY_MODULE_FUNCTION);
}

/*
 *  说	明 : SPI片选引脚输出高电平
 *  参	数 : 无
 *  返回值  : 无
 */
void SPI_CS_HIGH(void)
{
	GPIO_setAsOutputPin(SPI_CS_PORT,SPI_CS_PIN);		//配置为输出方向
	GPIO_setOutputHighOnPin(SPI_CS_PORT,SPI_CS_PIN);	//输出高电平
}

/*
 *  说	明 : SPI片选引脚输出低电平
 *  参	数 : 无
 *  返回值  : 无
 */
void SPI_CS_LOW(void)
{
	GPIO_setAsOutputPin(SPI_CS_PORT,SPI_CS_PIN);		//配置为输出方向
	GPIO_setOutputLowOnPin(SPI_CS_PORT,SPI_CS_PIN);		//输出低电平
}

/*
 *  说	明 : SPI寄存器初始化配置
 *  参	数 : 无
 *  返回值  : 无
 */
void SPI_Init_Config(void)
{
	EUSCI_B_SPI_initMasterParam SPInitStruct={0};
	SPInitStruct.selectClockSource = EUSCI_B_SPI_CLOCKSOURCE_SMCLK;
	SPInitStruct.clockSourceFrequency = CS_getSMCLK();
	SPInitStruct.desiredSpiClock= CS_getSMCLK();
	SPInitStruct.msbFirst= EUSCI_B_SPI_MSB_FIRST;
	SPInitStruct.clockPhase = EUSCI_B_SPI_PHASE_DATA_CAPTURED_ONFIRST_CHANGED_ON_NEXT;
	SPInitStruct.clockPolarity = EUSCI_B_SPI_CLOCKPOLARITY_INACTIVITY_LOW;
	SPInitStruct.spiMode= EUSCI_B_SPI_4PIN_UCxSTE_ACTIVE_LOW;

	EUSCI_B_SPI_initMaster(SPI_BASE_ADDR,&SPInitStruct);
}

/*
 *  说	明 : SPI始化配置
 *  参	数 : 无
 *  返回值  : 无
 */
void SPI_Init(void)
{
	EUSCI_B_SPI_disable(SPI_BASE_ADDR);
	SPI_Init_Config();
	SPI_GPIO_Config();
	EUSCI_B_SPI_select4PinFunctionality(SPI_BASE_ADDR,EUSCI_B_SPI_ENABLE_SIGNAL_FOR_4WIRE_SLAVE);
	EUSCI_B_SPI_enable(SPI_BASE_ADDR);

//	EUSCI_B_SPI_enableInterrupt(SPI_BASE_ADDR,EUSCI_B_SPI_TRANSMIT_INTERRUPT);
}

/*
 *  说	明 : SPI发送一个字节函数
 */
uint8_t SPI_Send(uint8_t data)
{
	uint8_t read_temp = 0x00;

//	while(!(HWREG16(SPI_BASE_ADDR + OFS_UCAxIFG) & UCTXIFG));	//判定发送缓存区是否为空,发送缓存区为空发送数据
	EUSCI_B_SPI_transmitData(SPI_BASE_ADDR,data);

//	while(!(HWREG16(SPI_BASE_ADDR + OFS_UCAxIFG) & UCRXIFG));	//判定是否收到数据

	read_temp = EUSCI_B_SPI_receiveData(SPI_BASE_ADDR);	//读取收到的数据

	return read_temp;
}
