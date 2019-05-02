/*
 * bsp_uart.c
 *
 *  Created on: 2018��11��27��
 *      Author: xu
 *
 *      UART��ʼ�����ò���:	1.��λUCSWRST	(�ϵ�Ĭ����λ)
 *      					2.��ʼ��UART ��ؼĴ���
 *      					3.��ʼ��UART GPIO�˿�
 *      					4.����UCSWRST ��ʹ��UART����
 *      					5.��ѡʹ���ж�
 *		 ʹ�õ���USCI_A1_UART
 *       �����Ҫ��������UART��,ֻ��Ļ���ַ	UART_BASE_ADDR ��������ż���
 */
#include "bsp_uart.h"

/*
 *  ��������:UART_GPIO_Config
 *  ˵	�� : UART������ų�ʼ��
 *  ��	�� : ��
 *  ����ֵ  : ��
 */
void UART_GPIO_Config(void)
{
	//����TX����
	GPIO_setAsPeripheralModuleFunctionOutputPin(UART_GPIO_PORT,UART_GPIO_TX_PIN,GPIO_PRIMARY_MODULE_FUNCTION);

	//����RX����
	GPIO_setAsPeripheralModuleFunctionInputPin(UART_GPIO_PORT,UART_GPIO_RX_PIN,GPIO_PRIMARY_MODULE_FUNCTION);
}

#if   F
/*
 *  ��������:UART_InitConfig
 *  ˵	�� : ʱ��ѡ��SMCLK,������Ϊ9600
 *  ��	�� : UART��ʼ��,
 *  		���㹫ʽ:  http://software-dl.ti.com/msp430/msp430_public_sw/mcu/msp430/MSP430BaudRateConverter/index.html
 *  ��	�� : ��
 *  ����ֵ  : ��
 */
void UART_InitConfig(void)
{
	EUSCI_A_UART_initParam UARTInitStruct;
	UARTInitStruct.selectClockSource= EUSCI_A_UART_CLOCKSOURCE_SMCLK ;//ѡ��SMCKL��ΪUART��ʱ��
	UARTInitStruct.clockPrescalar = 6;
	UARTInitStruct.firstModReg = 8;
	UARTInitStruct.secondModReg = 17;
	UARTInitStruct.parity = EUSCI_A_UART_NO_PARITY;
	UARTInitStruct.msborLsbFirst = EUSCI_A_UART_LSB_FIRST;		//��λ����
	UARTInitStruct.numberofStopBits = EUSCI_A_UART_ONE_STOP_BIT;
	UARTInitStruct.uartMode = EUSCI_A_UART_MODE;
	UARTInitStruct.overSampling = EUSCI_A_UART_OVERSAMPLING_BAUDRATE_GENERATION;
	EUSCI_A_UART_init(UART_BASE_ADDR,&UARTInitStruct);			//UART_A0����ַ
}

#elif  T
/*
 *  ��������:UART_InitConfig
 *  ˵	�� : ʱ��ѡ��SMCLK(SMCLK=1MHz),������Ϊ115200
 *  ��	�� : UART��ʼ��,
 *  		���㹫ʽ:  http://software-dl.ti.com/msp430/msp430_public_sw/mcu/msp430/MSP430BaudRateConverter/index.html
 *  ��	�� : ��
 *  ����ֵ  : ��
 */
void UART_InitConfig(void)
{
	EUSCI_A_UART_initParam UARTInitStruct;
	UARTInitStruct.selectClockSource= EUSCI_A_UART_CLOCKSOURCE_SMCLK ;//ѡ��SMCKL��ΪUART��ʱ��
	UARTInitStruct.clockPrescalar = 8;
	UARTInitStruct.firstModReg = 0;
	UARTInitStruct.secondModReg = 214;
	UARTInitStruct.parity = EUSCI_A_UART_NO_PARITY;
	UARTInitStruct.msborLsbFirst = EUSCI_A_UART_LSB_FIRST;		//��λ����
	UARTInitStruct.numberofStopBits = EUSCI_A_UART_ONE_STOP_BIT;
	UARTInitStruct.uartMode = EUSCI_A_UART_MODE;
	UARTInitStruct.overSampling = EUSCI_A_UART_LOW_FREQUENCY_BAUDRATE_GENERATION;
	EUSCI_A_UART_init(UART_BASE_ADDR,&UARTInitStruct);			//UART_A0����ַ
}
#endif
/*
 *  ��������:UART_Init
 * 	˵	�� : UART��ʼ������,�ϸ��ղ�������,�ο�User Guide
 * 	��	�� : ��
 * 	����ֵ  : ��
 */
void UART_Init(void)
{
	EUSCI_A_UART_disable(UART_BASE_ADDR);
	UART_InitConfig();						//��ʼ��UART�Ĵ���
	UART_GPIO_Config();						//����GPIO
	EUSCI_A_UART_enable(UART_BASE_ADDR);	//ʹ��UART

	//��ѡʹ���ж�
	EUSCI_A_UART_enableInterrupt(UART_BASE_ADDR,EUSCI_A_UART_TRANSMIT_INTERRUPT);   //ʹ�ܷ����ж�
	EUSCI_A_UART_enableInterrupt(UART_BASE_ADDR,EUSCI_A_UART_RECEIVE_INTERRUPT);	//ʹ�ܽ����ж�
}


/*
 *  ��������:SendByte
 * 	˵	�� : �����ֽں���
 * 	��	�� : Data:Ҫ���͵�һ�ֽ�����
 * 			Length	 :�������ݵĳ���
 * 	����ֵ  : ��
 */
void SendByte(uint8_t Data)
{
	EUSCI_A_UART_transmitData(UART_BASE_ADDR,Data);
}


/*
 *  ��������:UART_Send
 * 	˵	�� : ���ݷ��ͺ���,������CPU����
 * 	��	�� : Tx_Buffer:Ҫ���͵�����
 * 			Length	 :�������ݵĳ���
 * 	����ֵ  : ��
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
 *  ��������:UART_Send_String
 * 	˵	�� : �����ַ���
 * 	��	�� : Tx_Buffer:Ҫ���͵�����
 * 			Length	 :�������ݵĳ���
 * 	����ֵ  : ��
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
 *  ��������:UART_ReceiveData
 * 	˵	�� : ���պ���,������CPU����
 * 	��	�� : Rx_Buffer:Ҫ���յ�����
 * 			Length	 :�������ݵĳ���
 * 	����ֵ  : ��
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

