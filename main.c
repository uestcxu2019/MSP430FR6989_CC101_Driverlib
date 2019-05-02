/******************************************************************************
 *******************************copyright**************************************
 ******************************************************************************/

/******************************************************************************
 *
 * 		Author	: Xuzhi Liu
 * 		Date	: 2018.11.29
 * 		Brief	: ����CC1101������Ƶ�շ�ͨ��
 * 				   �Լ�д���ļ���ͷ�ļ�Ҳ�Ž�driverlib.h�ļ���
 *
 *
 * 			History
 * 					1.Author	: Xuzhi Liu
 * 					  Date		: 2018.11.29
 * 					  Mod		: 1.����ʹ��MSP430��Ƭ��,����uartͨ��,ʹ��USCI_A1_UART
 * 					  			        Ӳ������:	TX --> P3.4		EUSCI_A1_UART
 * 					  			  			RX --> P3.5
 * 					  			        ��driverlib.h�ļ�������������������궨��
 * 					  			  2.�޸�ϵͳʱ������
 * 					  			  				ACLK:	32Hz
 * 					  			  				SMCLK��   4MHz
 * 					  			  				MCLK :  4MHz
 *
 *
 * 					2.Author	: Xuzhi Liu
 * 					  Date		: 2018.11.30
 * 					  Mod		: 1.����SPIͨ��,ʹ��USCI_B1_SPI
 * 					  			        Ӳ������:
 * 					  			  		   SPI_CLK  --> P4.2
 * 					  			  		   SPI_CS   --> P4.3
 * 					  			  		   SPI_MOSI --> P4.0
 * 					  			  		   SPI_MISO --> P4.1
 * 					  			   2.�����ʱ����,����CPU_CLOCK=8MHz
 *
 *
 *					3.Author	: Xuzhi Liu
 * 					  Date		: 2018.12.03
 * 					  Mod		: ���CC1101�ļ�
 * 					  			  ����Ӳ������:
 * 					  			  		   GDIO0 --> P2.0
 *
 ***********************************************************************************/

#include "driverlib.h"

int main(void)
{
	uint8_t Tx_Buffer[5]={0x59,0x02,0x32,0x04,0x22};   //����Ҫ���͵�����
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
    *	Function:	���Ʋ���
*******************************************************************/
/*    	UART_Send_String("\n�ɹ����UARTͨѶ�ˣ�����\n");
    	GPIO_toggleOutputOnPin(LED1_GPIO_PORT,LED1_GPIO_PIN);
    	Delay_ms(100);*/
    	Write_Data(IOCFG2_ADDR,GDO_LOW);		//Ϩ��GDO0
    	Write_Data(IOCFG0_ADDR,GDO_LOW);		//Ϩ��GDO2
//    	UART_Send_String("\nϨ��LED\n");
    	Delay_ms(1000);				//��ʱ1s
    	Write_Data(IOCFG0_ADDR,GDO_HIGH);		//����GDO0
    	Write_Data(IOCFG2_ADDR,GDO_HIGH);		//����GDO2
//    	UART_Send_String("\n����LED\n");
    	Delay_ms(1000);

#elif		F
/*******************************************************************
    *	Author	:	xuzhi Liu
    *	Date	:	2018.12.03
    *	Function:	�շ�����
*******************************************************************/

    	Test_Write_Read_byte(IOCFG0_ADDR,0x12,IOCFG0_ADDR|READ_SINGLE, READ_SINGLE);
    	Delay_ms(1000);

#elif		T
/*******************************************************************
	*	Author	:	xuzhi Liu
	*	Date	:	2018.12.03
	*	Function:	�������ݰ�
*******************************************************************/
//    	CC1101_RFDataPack_Send(Tx_Buffer, sizeof(Tx_Buffer));
//		Delay_ms(1000);							//��ʱ1s

#endif

    }
}

