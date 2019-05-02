/*
 * bsp_cc1101.c
 *
 *  Created on: 2018��11��30��
 *  Author: xu
 *	˵��������cmd:(һ���ֽ�) ��һ����ͷλ,R(1)W(0), ͻ��λ(B:1Ϊͻ��λ����������ȡ��д�룬�ҵ�ַҪ�����0x2F),��ַλ
 *				 A7   		A6   	A5   A4    A3    A2    A1    A0
 *				 R/W	  	B		A5-A0: ��ַλ
 *				����ַλ����0x2Fʱ��ͻ��λ�����ٱ�ʾͻ��
 *
 *	 �ر�ע��:	���ֽڷ���ʱ
 *					ʱ��Ƶ��С�ڵ���9MHzʱ,�ڷ��͵�ַ������֮�䲻��Ҫ��ʱ
 *			  		���ʱ��Ƶ�ʴ���9MHzʱ,�ڷ��͵�ַ�ͷ�������֮����Ҫ�����ʱ
 *					����ʱ��Ƶ���Ƕ����Լ�SPIͨ�ŵ��������,��ο�1101�����ֲ�
 *				��������ʱ
 *					ʱ��Ƶ��С�ڵ���6.5MHzʱ,�ڷ��͵�ַ������֮�䲻��Ҫ��ʱ
 *
**/
#include "bsp_cc1101.h"

/**
  *	��	��:	д��Ͷ�ȡ���ݲ��Ժ���,�������Է���,�����ݱ���Ҫ�ֳ���������ܶ�����ȷ������
  * ��	��: Write_addr: ͨ�üĴ�����ַ,���Դ��ļ�bsp_c1101.h ���ҵ�
  *			Write_Data: д�������
  *			Read_addr : ��ȡ���ݵĵ�ַ
  *			Read_cmd  :	��ȡ����
  * ����ֵ:  ��
**/

void Test_Write_Read_byte(uint8_t Write_addr,uint8_t Write_Data,uint8_t Read_addr, uint8_t Read_cmd)
{
	uint8_t data = 0x00;
	SPI_CS_LOW();				/*����Ƭѡ*/
	while((GPIO_getInputPinValue(SPI_MISO_PORT, SPI_MISO_PIN)& 0x80) == GPIO_INPUT_PIN_HIGH); //�ȴ�MISO����Ϊ��
	Delay_us(20);					/*��ʱ*/
	SPI_Send(Write_addr);		/*����Ҫд�ļĴ����ĵ�ַ*/
	SPI_Send(Write_Data);		/*��������,�������ݲο�1101�����ֲ�*/
	Delay_us(50);
	SPI_Send(Read_addr);
	data = SPI_Send(Read_cmd);
	SPI_CS_HIGH();				/*����Ƭѡ*/
	printf("\n��ӡ����:%x\n",data);
}

/*
**	��	����ֻд����д�뺯��,����ѡָͨ��(13��),ֻ��Ҫ����һ��ָ���,����Ҫ��������(���ֽ�ָ��)
**			����ѡָͨ���ʹ�ڲ�״̬��ģʽ�����ı�
**			ע:����һ��������ʹ�����δ˺���ʱ,��ĳ������ºͺ���Write_Data()�൱
**	��	����CMD:ѡָͨ��,CC1101ͷ�ļ�����
**	����ֵ����
*/

void Write_CMD(uint8_t CMD)
{
	SPI_CS_LOW();						/*����Ƭѡ*/
	while((GPIO_getInputPinValue(SPI_MISO_PORT, SPI_MISO_PIN)& 0x80) == GPIO_INPUT_PIN_HIGH); //�ȴ�MISO����Ϊ��
	SPI_Send(CMD);						/*����ָ��,���Ĵ����ĵ�ַ*/
//	Delay_us(2);						/*��ʱ*/
	SPI_CS_HIGH();						/*����Ƭѡ*/
}


/*
**	��	����д���ݺ���,���η���,��һ�η���ָ��,�ڶ��η�������
**	��	����Write_Addr:Ҫд��ĵ�ַ,CC1101ͷ�ļ�����
**			Write_data:Ҫд�������
**	����ֵ����
*/
void Write_Data(uint8_t Write_Addr,uint8_t Write_data)
{
	SPI_CS_LOW();							/* ����Ƭѡ */
	while((GPIO_getInputPinValue(SPI_MISO_PORT, SPI_MISO_PIN)& 0x80) == GPIO_INPUT_PIN_HIGH); //�ȴ�MISO����Ϊ��
	SPI_Send(Write_Addr);					/*����ָ��,���Ĵ����ĵ�ַ*/
	SPI_Send(Write_data);					/* �������� */
//	Delay_us(2);							/*��ʱ*/
	SPI_CS_HIGH();							/*����Ƭѡ*/
}


/************************************************************************************************
**	��	�������ֽڻض�CC1101���ݻ�״̬,
**		  	�ر�ע�⣺���ɻض�SLEEP��XOFF״̬,����CSnʱ����ʹ���SLEEP��XOFF״̬���뵽IDLE(����)״̬
**	��	����Addr:Ҫ��ȡ�ĵ�ַ, Cmd:��ȡ��ʽ, CC1101ͷ�ļ�����
**	����ֵ�����ض�ȡ����״̬������
*************************************************************************************************/
uint8_t Read_byte(uint8_t Addr, uint8_t Cmd)
{
	uint8_t Read_data =0x00;
	SPI_CS_LOW();								/*����Ƭѡ*/
	while((GPIO_getInputPinValue(SPI_MISO_PORT, SPI_MISO_PIN)& 0x80) == GPIO_INPUT_PIN_HIGH); //�ȴ�MISO����Ϊ��
	SPI_Send(Addr);								/*����ָ��,����Ҫ��ȡ�ĵ�ַ*/
	Read_data = SPI_Send(Cmd);					/*���ͻض���ʽ*/
	Delay_us(2);								/*��ʱ*/
	SPI_CS_HIGH();								/*����Ƭѡ*/

	printf("\n�����������ǣ�%x\n",Read_data);

	return Read_data;
}

/************************************************************************************************
**	��	����CC1101��ʼ��,����ģʽ: ��������:1.2kBaud,���Է�ʽ:GFSK,Ƶ��:868MHz
**			�����д󲿷ּĴ������Բ�������,�����������ڲ���,����ȫ�����á�������ʽʹ��ʱ����ɾ��
**			ע: ע�͵�����Ĭ��ֵ
**	��	����
**	����ֵ����
*************************************************************************************************/

void CC1101_Init(void)
{
	//Date rate:250kBaud,Dev:127kkHz, Mod:GFSK, RX BW:540kHz,base frequency:433MHz,optimized for current consumption
		Write_Data(IOCFG2_ADDR,0x06);       //����/���յ�ͬ����ʱ��λ
		Write_Data(IOCFG0_ADDR,0x2E);       //����Ϊ����̬.

		//����Ϊ433MHz
		Write_Data(FREQ2,0x10);             //Ƶ�ʿ��ƴʻ㣬���ֽڡ���������
		Write_Data(FREQ1,0xA7);             //Ƶ�ʿ��ƴʻ㣬�м��ֽڡ���������
		Write_Data(FREQ0,0x62);             //Ƶ�ʿ��ƴʻ㣬���ֽڣ���������

		//250Kbaud
		Write_Data(MDMCFG4,0x2D);           //���������á���������
		Write_Data(MDMCFG3,0x3B);           //���������á���������
		Write_Data(MDMCFG2,0x13);           //���������á������Ż�ʱ��Ҫ����

		Write_Data(DEVIATN,0x62);           //����������.��������
		Write_Data(MCSM0,0x18);             //��ͨ�ſ���״̬������.��������
		Write_Data(FOCCFG,0x1D);            //Ƶ��ƫ�Ʋ������á���������

		//���������������
//		Write_Data(PATABLE,0x1D);			//������ʿ���
}


/************************************************************************************************
**	��	����д�����ݵ����ͻ�����(���ֽ�����)
**	��	����pBuffer:Ҫд�������,len:д�����ݵĳ���
**	����ֵ����
*************************************************************************************************/

void WriteTxFITO(uint8_t * pBuffer,uint8_t len)
{
	//��һ��:�������ݵĳ���
	Write_Data(WRITE_SINGLE_FIFO,len);			//���ͳ����ֽ�,�����ֽڲ���д��Ҫ���͵����ݵ�������

	//�ڶ���:��ѡ���͵�ַ�ֽ�
//	Write_Data(WRITE_BURST_FIFO,0x02);			//��ѡ���͵�ַ�ֽ�,���õ�ַΪ0x02

	//������:��������
	Write_burst(WRITE_BURST_FIFO,pBuffer,len);
}

/************************************************************************************************
**	��	�������ֽ�д�����ݵ����ͻ�����
**	��	����pBuffer:Ҫд�������
**	����ֵ����
*************************************************************************************************/

void Test_Write_Single_TxFITO(uint8_t pBuffer)
{
	SPI_CS_LOW();								//����Ƭѡ
	Write_Data(WRITE_SINGLE_FIFO,pBuffer);		//����д�뻺����,ÿ���ֽ�֮�����ʱ����SPI_Send������д����ʱ��.
	Delay_us(30);								//��ʱ30us
	SPI_CS_HIGH();								//����Ƭѡ
}

/************************************************************************************************
**	��	������ȡ���ջ����������ݡ�ע���˺���������ԡ�
**	��	����pBuffer:��Ҫ��ȡ������,len:��ȡ���ݵĳ���
**	����ֵ����
*************************************************************************************************/

void ReadRxFIFO(uint8_t *data,uint16_t len)
{
	Read_burst(READ_BURST_FIFO,data,len);
}
/************************************************************************************************
**	��	������������,������������ȫ�����ͳ�ȥ(������)
**	��	����pBuffer:��Ҫ��ȡ������,len:��ȡ���ݵĳ���
**	����ֵ����
*************************************************************************************************/

void CC1101_RFDataPack_Send(uint8_t *pBuff, uint16_t len)
{
    Write_CMD(SFTX);    			//��շ��ͻ�����,ֻ����IDLE״̬�����
    WriteTxFITO(pBuff, len);     	//д�����ݵ����ͻ�����
//	Delay_us(10);					//��ʱ1s
    Write_CMD(STX);     			//���뷢��ģʽ��ʼ��������,��ʼ�����ù������Ѿ����ú÷�����ɺ�������ģʽ
	while(GPIO_getInputPinValue(IOCFG2_GPIO_PORT, IOCFG2_GPIO_PIN) !=GPIO_INPUT_PIN_HIGH)
	{
		//���͵�ͬ����ʱ��λ,�����ݰ�ĩβȡ����λ,���GDI2��Ϊ1,ɶҲ����,�յ�
	}
	while(GPIO_getInputPinValue(IOCFG2_GPIO_PORT, IOCFG2_GPIO_PIN) ==GPIO_INPUT_PIN_HIGH);	//����Ϊ��,˵�����ݰ���δ����
}


/************************************************************************************************
**	��	������������,�����ջ���������ȫ������ ���˺���������ԡ�
**	��	����pBuffer:��Ҫ���յ�����,len:�������ݵĳ���
**	����ֵ����
*************************************************************************************************/

void CC1101_RFDataPack_Rceive(uint8_t *pBuff, uint16_t len)
{
	Read_byte(RXBYTES,READ_SINGLE);
	Write_CMD(SRX);   				//�������ģʽ
	Read_byte(MARCSTATE,READ_SINGLE);
//	while(!(GPIO_getInputPinValue(TXRX_GPIO_PORT, TXRX_GPIO_PIN))) //�ȴ�������λ
//
//	while(GPIO_getInputPinValue(TXRX_GPIO_PORT, TXRX_GPIO_PIN) == RESET) //�ȴ�����,��������

	Read_byte(RXBYTES,READ_SINGLE);
    ReadRxFIFO(pBuff,len);     		//��������
	Delay_ms(1000);					//��ʱ1s
//	while(GPIO_getInputPinValue(TXRX_GPIO_PORT, TXRX_GPIO_PIN) != RESET) //�ȴ�������λ
//
//	while(GPIO_getInputPinValue(TXRX_GPIO_PORT, TXRX_GPIO_PIN) == RESET) //�ȴ�����,��������

	Write_CMD(SFRX);
    Write_CMD(SIDLE);   //�˳���ǰģʽ
}


/************************************************************************************************
**	��	�������ֽڷ�������,�����ͻ����������ݷ��ͳ�ȥ
**	��	����pBuffer:��Ҫ��ȡ������
**	����ֵ����
*************************************************************************************************/

void Test_RF_Single_Send(uint8_t pData)
{
	Write_CMD(SIDLE);   			//�������ģʽ
    Write_CMD(SFTX);    			//��շ��ͻ�����,ֻ����IDLE״̬�����
    Test_Write_Single_TxFITO(pData);     	//д�����ݵ����ͻ�����
    Write_CMD(STX);     			//��ʼ��������
	Delay_ms(1000);					//��ʱ1s
//	while(GPIO_getInputPinValue(TXRX_GPIO_PORT, TXRX_GPIO_PIN) != RESET) //�ȴ�������λ
//
//	while(GPIO_getInputPinValue(TXRX_GPIO_PORT, TXRX_GPIO_PIN) == RESET) //�ȴ�����,��������

	Write_CMD(SFTX);
    Write_CMD(SIDLE);   //�˳���ǰģʽ
}

/************************************************************************************************
**	��	���������������ͺͶ�ȡ����
**	��	����Write_Addr:Ҫд�����ݵĵ�ַ
**			pBuffer   :д�������
**			Read_Addr :��ȡ���ݵĵ�ַ
**			Read_Cmd		  :��ȡ��ʽ
**			Length	  :д�����ݵĳ���
**	����ֵ����
*************************************************************************************************/

void Write_Read_burst(uint8_t Write_Addr,uint8_t *pbuffer,uint8_t Read_Addr,uint8_t Length)
{
	uint8_t data[]={0x00};					//���ݳ��ȸ��ݽ��յĳ����Զ�
	uint8_t i = 0;
	SPI_CS_LOW();							//����Ƭѡ
	while((GPIO_getInputPinValue(SPI_MISO_PORT, SPI_MISO_PIN)& 0x80) == GPIO_INPUT_PIN_HIGH); //�ȴ�MISO����Ϊ��
	Delay_us(20);							//��ʱ
	SPI_Send(Write_Addr);					//����Ҫд�ļĴ����ĵ�ַ,ͻ��λ����Ϊ1
	for(i=0; i < Length;i++)
	{
		SPI_Send(pbuffer[i]);				//ѭ����������
	}
	SPI_CS_HIGH();							//����Ƭѡ,������������,��������ʱ,����ͨ������Ƭѡ��������

	Delay_us(50);

	SPI_CS_LOW();							//����Ƭѡ
	Delay_us(50);
	SPI_Send(Read_Addr);
	for(i=0; i < Length;i++)
	{
		data[i] = SPI_Send(0);				//������ȡ����,������,��0�ĳ�READ_BURSTҲû����
	}
	SPI_CS_HIGH();							//����Ƭѡ
	for(i=0; i < Length;i++)
	{
		printf("\n��ӡ����:%x\n",data[i]);
	}
}


/************************************************************************************************
**	��	����������������
**	��	����Write_Addr:Ҫд�����ݵĵ�ַ
**			pBuffer   :д�������
**			Length	  :д�����ݵĳ���
**	����ֵ����
*************************************************************************************************/

void Write_burst(uint8_t Write_Addr,uint8_t *pbuffer,uint8_t Length)
{
	uint8_t i = 0;
	SPI_CS_LOW();							//����Ƭѡ
	while((GPIO_getInputPinValue(SPI_MISO_PORT, SPI_MISO_PIN)& 0x80) == GPIO_INPUT_PIN_HIGH); //�ȴ�MISO����Ϊ��
//	Delay_us(20);							//��ʱ
	SPI_Send(Write_Addr);					//����Ҫд�ļĴ����ĵ�ַ,ͻ��λ����Ϊ1

	for(i=0; i < Length;i++)
	{
		SPI_Send(pbuffer[i]);				//ѭ����������
	}
	SPI_CS_HIGH();							//����Ƭѡ,������������,��������ʱ,����ͨ������Ƭѡ��������
}


/************************************************************************************************
**	��	����������ȡ����
**	��	����Read_Addr :Ҫ��ȡ���ݵĵ�ַ
**			pBuffer   :����ȡ�����ݴ��뵽pBuffer��
**			Length	  :��ȡ���ݵĳ���
**	����ֵ����
*************************************************************************************************/

void Read_burst(uint8_t Read_Addr,uint8_t *data,uint8_t Length)
{
	uint8_t i = 0;
	SPI_CS_LOW();							//����Ƭѡ
	while((GPIO_getInputPinValue(SPI_MISO_PORT, SPI_MISO_PIN)& 0x80) == GPIO_INPUT_PIN_HIGH); //�ȴ�MISO����Ϊ��
	SPI_Send(Read_Addr);
	for(i=0; i < Length;i++)
	{
		data[i] = SPI_Send(0);				//������ȡ����,������,��0�ĳ�READ_BURSTҲû����
	}
	SPI_CS_HIGH();							//����Ƭѡ
//	for(i=0; i < Length;i++)
//	{
//		printf("\n��ӡ����:%x\n",data[i]);
//	}
}

/************************************************************************************************
**	��	����ϵͳ��λ
**	��	������
**	����ֵ����
*************************************************************************************************/

void CC1101_Reset(void)
{
	SPI_CS_LOW();							//����Ƭѡ
	while((GPIO_getInputPinValue(SPI_MISO_PORT, SPI_MISO_PIN)& 0x80) == GPIO_INPUT_PIN_HIGH); //�ȴ�MISO����Ϊ��
	Write_CMD(SRES);
//	Delay_ms(20);							//��ʱ20us
	SPI_CS_HIGH();							//����Ƭѡ
}






