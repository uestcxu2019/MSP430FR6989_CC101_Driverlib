/*
 * bsp_spi.c
 *
 *  Created on: 2018��11��30��
 *      Author: xu
 *      SPI��ʼ������
 */

#include "bsp_spi.h"

/*
 *  ˵	�� : SPI������ų�ʼ��
 *  ��	�� : ��
 *  ����ֵ  : ��
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
 *  ˵	�� : SPIƬѡ��������ߵ�ƽ
 *  ��	�� : ��
 *  ����ֵ  : ��
 */
void SPI_CS_HIGH(void)
{
	GPIO_setAsOutputPin(SPI_CS_PORT,SPI_CS_PIN);		//����Ϊ�������
	GPIO_setOutputHighOnPin(SPI_CS_PORT,SPI_CS_PIN);	//����ߵ�ƽ
}

/*
 *  ˵	�� : SPIƬѡ��������͵�ƽ
 *  ��	�� : ��
 *  ����ֵ  : ��
 */
void SPI_CS_LOW(void)
{
	GPIO_setAsOutputPin(SPI_CS_PORT,SPI_CS_PIN);		//����Ϊ�������
	GPIO_setOutputLowOnPin(SPI_CS_PORT,SPI_CS_PIN);		//����͵�ƽ
}

/*
 *  ˵	�� : SPI�Ĵ�����ʼ������
 *  ��	�� : ��
 *  ����ֵ  : ��
 */
void SPI_Init_Config(void)
{
	EUSCI_B_SPI_initMasterParam SPInitStruct;
	SPInitStruct.selectClockSource = EUSCI_B_SPI_CLOCKSOURCE_SMCLK;
	SPInitStruct.clockSourceFrequency = 1000000;
	SPInitStruct.desiredSpiClock= 1000000;
	SPInitStruct.msbFirst= EUSCI_B_SPI_MSB_FIRST;
	SPInitStruct.clockPhase = EUSCI_B_SPI_PHASE_DATA_CAPTURED_ONFIRST_CHANGED_ON_NEXT;
	SPInitStruct.clockPolarity = EUSCI_B_SPI_CLOCKPOLARITY_INACTIVITY_LOW;
	SPInitStruct.spiMode= EUSCI_B_SPI_4PIN_UCxSTE_ACTIVE_LOW;

	EUSCI_B_SPI_initMaster(SPI_BASE_ADDR,&SPInitStruct);
}

/*
 *  ˵	�� : SPIʼ������
 *  ��	�� : ��
 *  ����ֵ  : ��
 */
void SPI_Init(void)
{
	EUSCI_B_SPI_disable(SPI_BASE_ADDR);
	SPI_Init_Config();
	SPI_GPIO_Config();
	EUSCI_B_SPI_select4PinFunctionality(SPI_BASE_ADDR,EUSCI_B_SPI_ENABLE_SIGNAL_FOR_4WIRE_SLAVE);
	EUSCI_B_SPI_enable(SPI_BASE_ADDR);
}

/*
 *  ˵	�� : SPI����һ���ֽں���
 */
uint8_t SPI_Send(uint8_t data)
{
	uint8_t read_temp = 0x00;

//	while(!(HWREG16(SPI_BASE_ADDR + OFS_UCAxIFG) & UCTXIFG));	//�ж����ͻ������Ƿ�Ϊ��,���ͻ�����Ϊ�շ�������
	EUSCI_B_SPI_transmitData(SPI_BASE_ADDR,data);

//	while(!(HWREG16(SPI_BASE_ADDR + OFS_UCAxIFG) & UCRXIFG));	//�ж��Ƿ��յ�����

	read_temp = EUSCI_B_SPI_receiveData(SPI_BASE_ADDR);	//��ȡ�յ�������

	return read_temp;
}
