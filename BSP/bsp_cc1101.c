/*
 * bsp_cc1101.c
 *
 *  Created on: 2018年11月30日
 *  Author: xu
 *	说明：命令cmd:(一个字节) 有一个报头位,R(1)W(0), 突发位(B:1为突发位，即连续读取或写入，且地址要求低于0x2F),地址位
 *				 A7   		A6   	A5   A4    A3    A2    A1    A0
 *				 R/W	  	B		A5-A0: 地址位
 *				当地址位大于0x2F时，突发位将不再表示突发
 *
 *	 特别注意:	单字节发送时
 *					时钟频率小于等于9MHz时,在发送地址和数据之间不需要延时
 *			  		如果时钟频率大于9MHz时,在发送地址和发送数据之间需要添加延时
 *					具体时钟频率是多少以及SPI通信的最大速率,请参考1101数据手册
 *				连续发送时
 *					时钟频率小于等于6.5MHz时,在发送地址和数据之间不需要延时
 *
**/
#include "bsp_cc1101.h"

/**
  *	描	述:	写入和读取数据测试函数,经过测试发现,读数据必须要分成两步骤才能读到正确的数据
  * 参	数: Write_addr: 通用寄存器地址,可以从文件bsp_c1101.h 中找到
  *			Write_Data: 写入的数据
  *			Read_addr : 读取数据的地址
  *			Read_cmd  :	读取命令
  * 返回值:  无
**/

void Test_Write_Read_byte(uint8_t Write_addr,uint8_t Write_Data,uint8_t Read_addr, uint8_t Read_cmd)
{
	uint8_t data = 0x00;
	SPI_CS_LOW();				/*拉低片选*/
	while((GPIO_getInputPinValue(SPI_MISO_PORT, SPI_MISO_PIN)& 0x80) == GPIO_INPUT_PIN_HIGH); //等待MISO引脚为低
	Delay_us(20);					/*延时*/
	SPI_Send(Write_addr);		/*发送要写的寄存器的地址*/
	SPI_Send(Write_Data);		/*发送数据,发送数据参考1101数据手册*/
	Delay_us(50);
	SPI_Send(Read_addr);
	data = SPI_Send(Read_cmd);
	SPI_CS_HIGH();				/*拉高片选*/
	printf("\n打印数据:%x\n",data);
}

/*
**	描	述：只写命令写入函数,对于选通指令(13个),只需要发送一次指令即可,不需要发送数据(单字节指令)
**			发送选通指令会使内部状态或模式发生改变
**			注:当在一个函数中使用两次此函数时,在某种情况下和函数Write_Data()相当
**	参	数：CMD:选通指令,CC1101头文件查找
**	返回值：无
*/

void Write_CMD(uint8_t CMD)
{
	SPI_CS_LOW();						/*拉低片选*/
	while((GPIO_getInputPinValue(SPI_MISO_PORT, SPI_MISO_PIN)& 0x80) == GPIO_INPUT_PIN_HIGH); //等待MISO引脚为低
	SPI_Send(CMD);						/*发送指令,读寄存器的地址*/
//	Delay_us(2);						/*延时*/
	SPI_CS_HIGH();						/*拉高片选*/
}


/*
**	描	述：写数据函数,两次发送,第一次发送指令,第二次发送数据
**	参	数：Write_Addr:要写入的地址,CC1101头文件查找
**			Write_data:要写入的数据
**	返回值：无
*/
void Write_Data(uint8_t Write_Addr,uint8_t Write_data)
{
	SPI_CS_LOW();							/* 拉低片选 */
	while((GPIO_getInputPinValue(SPI_MISO_PORT, SPI_MISO_PIN)& 0x80) == GPIO_INPUT_PIN_HIGH); //等待MISO引脚为低
	SPI_Send(Write_Addr);					/*发送指令,读寄存器的地址*/
	SPI_Send(Write_data);					/* 发送数据 */
//	Delay_us(2);							/*延时*/
	SPI_CS_HIGH();							/*拉高片选*/
}


/************************************************************************************************
**	描	述：单字节回读CC1101数据或状态,
**		  	特别注意：不可回读SLEEP和XOFF状态,拉低CSn时，会使其从SLEEP和XOFF状态进入到IDLE(空闲)状态
**	参	数：Addr:要读取的地址, Cmd:读取方式, CC1101头文件查找
**	返回值：返回读取到的状态或数据
*************************************************************************************************/
uint8_t Read_byte(uint8_t Addr, uint8_t Cmd)
{
	uint8_t Read_data =0x00;
	SPI_CS_LOW();								/*拉低片选*/
	while((GPIO_getInputPinValue(SPI_MISO_PORT, SPI_MISO_PIN)& 0x80) == GPIO_INPUT_PIN_HIGH); //等待MISO引脚为低
	SPI_Send(Addr);								/*发送指令,发送要读取的地址*/
	Read_data = SPI_Send(Cmd);					/*发送回读方式*/
	Delay_us(2);								/*延时*/
	SPI_CS_HIGH();								/*拉高片选*/

	printf("\n读到的数据是：%x\n",Read_data);

	return Read_data;
}

/************************************************************************************************
**	描	述：CC1101初始化,配置模式: 数据速率:1.2kBaud,调试方式:GFSK,频带:868MHz
**			其中有大部分寄存器可以不用配置,但现在是用于测试,所以全部配置。后续正式使用时可以删除
**			注: 注释掉的是默认值
**	参	数：
**	返回值：无
*************************************************************************************************/

void CC1101_Init(void)
{
	//Date rate:250kBaud,Dev:127kkHz, Mod:GFSK, RX BW:540kHz,base frequency:433MHz,optimized for current consumption
		Write_Data(IOCFG2_ADDR,0x06);       //发送/接收到同步字时置位
		Write_Data(IOCFG0_ADDR,0x2E);       //配置为高组态.

		//配置为433MHz
		Write_Data(FREQ2,0x10);             //频率控制词汇，高字节。必须配置
		Write_Data(FREQ1,0xA7);             //频率控制词汇，中间字节。必须配置
		Write_Data(FREQ0,0x62);             //频率控制词汇，低字节，必须配置

		//250Kbaud
		Write_Data(MDMCFG4,0x2D);           //调制器配置。必须配置
		Write_Data(MDMCFG3,0x3B);           //调制器配置。必须配置
		Write_Data(MDMCFG2,0x13);           //调制器配置。电流优化时需要配置

		Write_Data(DEVIATN,0x62);           //调制器设置.必须配置
		Write_Data(MCSM0,0x18);             //主通信控制状态机配置.必须配置
		Write_Data(FOCCFG,0x1D);            //频率偏移补偿配置。必须配置

		//发送输出功率配置
//		Write_Data(PATABLE,0x1D);			//输出功率控制
}


/************************************************************************************************
**	描	述：写入数据到发送缓冲区(多字节数据)
**	参	数：pBuffer:要写入的数据,len:写入数据的长度
**	返回值：无
*************************************************************************************************/

void WriteTxFITO(uint8_t * pBuffer,uint8_t len)
{
	//第一步:发送数据的长度
	Write_Data(WRITE_SINGLE_FIFO,len);			//发送长度字节,长度字节不能写在要发送的数据的数组里

	//第二步:可选发送地址字节
//	Write_Data(WRITE_BURST_FIFO,0x02);			//可选发送地址字节,设置地址为0x02

	//第三步:发送数据
	Write_burst(WRITE_BURST_FIFO,pBuffer,len);
}

/************************************************************************************************
**	描	述：单字节写入数据到发送缓冲区
**	参	数：pBuffer:要写入的数据
**	返回值：无
*************************************************************************************************/

void Test_Write_Single_TxFITO(uint8_t pBuffer)
{
	SPI_CS_LOW();								//拉低片选
	Write_Data(WRITE_SINGLE_FIFO,pBuffer);		//数据写入缓存区,每个字节之间的延时已在SPI_Send函数中写入延时了.
	Delay_us(30);								//延时30us
	SPI_CS_HIGH();								//拉高片选
}

/************************************************************************************************
**	描	述：读取接收缓冲区的数据【注：此函数还需测试】
**	参	数：pBuffer:需要读取的数据,len:读取数据的长度
**	返回值：无
*************************************************************************************************/

void ReadRxFIFO(uint8_t *data,uint16_t len)
{
	Read_burst(READ_BURST_FIFO,data,len);
}
/************************************************************************************************
**	描	述：发送数据,将缓冲区数据全部发送出去(多数据)
**	参	数：pBuffer:需要读取的数据,len:读取数据的长度
**	返回值：无
*************************************************************************************************/

void CC1101_RFDataPack_Send(uint8_t *pBuff, uint16_t len)
{
    Write_CMD(SFTX);    			//清空发送缓冲区,只能在IDLE状态下清空
    WriteTxFITO(pBuff, len);     	//写入数据到发送缓冲区
//	Delay_us(10);					//延时1s
    Write_CMD(STX);     			//进入发送模式开始发送数据,初始化配置过程中已经配置好发送完成后进入空闲模式
	while(GPIO_getInputPinValue(IOCFG2_GPIO_PORT, IOCFG2_GPIO_PIN) !=GPIO_INPUT_PIN_HIGH)
	{
		//发送到同步字时置位,到数据包末尾取消置位,如果GDI2不为1,啥也不干,空等
	}
	while(GPIO_getInputPinValue(IOCFG2_GPIO_PORT, IOCFG2_GPIO_PIN) ==GPIO_INPUT_PIN_HIGH);	//条件为真,说明数据包还未发完
}


/************************************************************************************************
**	描	述：接收数据,将接收缓冲区数据全部接收 【此函数还需测试】
**	参	数：pBuffer:需要接收的数据,len:接收数据的长度
**	返回值：无
*************************************************************************************************/

void CC1101_RFDataPack_Rceive(uint8_t *pBuff, uint16_t len)
{
	Read_byte(RXBYTES,READ_SINGLE);
	Write_CMD(SRX);   				//进入接收模式
	Read_byte(MARCSTATE,READ_SINGLE);
//	while(!(GPIO_getInputPinValue(TXRX_GPIO_PORT, TXRX_GPIO_PIN))) //等待接收置位
//
//	while(GPIO_getInputPinValue(TXRX_GPIO_PORT, TXRX_GPIO_PIN) == RESET) //等待清零,结束发送

	Read_byte(RXBYTES,READ_SINGLE);
    ReadRxFIFO(pBuff,len);     		//接收数据
	Delay_ms(1000);					//延时1s
//	while(GPIO_getInputPinValue(TXRX_GPIO_PORT, TXRX_GPIO_PIN) != RESET) //等待接收置位
//
//	while(GPIO_getInputPinValue(TXRX_GPIO_PORT, TXRX_GPIO_PIN) == RESET) //等待清零,结束发送

	Write_CMD(SFRX);
    Write_CMD(SIDLE);   //退出当前模式
}


/************************************************************************************************
**	描	述：单字节发送数据,将发送缓冲区的数据发送出去
**	参	数：pBuffer:需要读取的数据
**	返回值：无
*************************************************************************************************/

void Test_RF_Single_Send(uint8_t pData)
{
	Write_CMD(SIDLE);   			//进入空闲模式
    Write_CMD(SFTX);    			//清空发送缓冲区,只能在IDLE状态下清空
    Test_Write_Single_TxFITO(pData);     	//写入数据到发送缓冲区
    Write_CMD(STX);     			//开始发送数据
	Delay_ms(1000);					//延时1s
//	while(GPIO_getInputPinValue(TXRX_GPIO_PORT, TXRX_GPIO_PIN) != RESET) //等待发送置位
//
//	while(GPIO_getInputPinValue(TXRX_GPIO_PORT, TXRX_GPIO_PIN) == RESET) //等待清零,结束发送

	Write_CMD(SFTX);
    Write_CMD(SIDLE);   //退出当前模式
}

/************************************************************************************************
**	描	述：测试连续发送和读取数据
**	参	数：Write_Addr:要写入数据的地址
**			pBuffer   :写入的数据
**			Read_Addr :读取数据的地址
**			Read_Cmd		  :读取方式
**			Length	  :写入数据的长度
**	返回值：无
*************************************************************************************************/

void Write_Read_burst(uint8_t Write_Addr,uint8_t *pbuffer,uint8_t Read_Addr,uint8_t Length)
{
	uint8_t data[]={0x00};					//数据长度根据接收的长度自定
	uint8_t i = 0;
	SPI_CS_LOW();							//拉低片选
	while((GPIO_getInputPinValue(SPI_MISO_PORT, SPI_MISO_PIN)& 0x80) == GPIO_INPUT_PIN_HIGH); //等待MISO引脚为低
	Delay_us(20);							//延时
	SPI_Send(Write_Addr);					//发送要写的寄存器的地址,突发位必须为1
	for(i=0; i < Length;i++)
	{
		SPI_Send(pbuffer[i]);				//循环发送数据
	}
	SPI_CS_HIGH();							//拉高片选,结束连续发送,连续发送时,必须通过拉高片选结束发送

	Delay_us(50);

	SPI_CS_LOW();							//拉低片选
	Delay_us(50);
	SPI_Send(Read_Addr);
	for(i=0; i < Length;i++)
	{
		data[i] = SPI_Send(0);				//连续读取数据,经测试,把0改成READ_BURST也没问题
	}
	SPI_CS_HIGH();							//拉高片选
	for(i=0; i < Length;i++)
	{
		printf("\n打印数据:%x\n",data[i]);
	}
}


/************************************************************************************************
**	描	述：连续发送数据
**	参	数：Write_Addr:要写入数据的地址
**			pBuffer   :写入的数据
**			Length	  :写入数据的长度
**	返回值：无
*************************************************************************************************/

void Write_burst(uint8_t Write_Addr,uint8_t *pbuffer,uint8_t Length)
{
	uint8_t i = 0;
	SPI_CS_LOW();							//拉低片选
	while((GPIO_getInputPinValue(SPI_MISO_PORT, SPI_MISO_PIN)& 0x80) == GPIO_INPUT_PIN_HIGH); //等待MISO引脚为低
//	Delay_us(20);							//延时
	SPI_Send(Write_Addr);					//发送要写的寄存器的地址,突发位必须为1

	for(i=0; i < Length;i++)
	{
		SPI_Send(pbuffer[i]);				//循环发送数据
	}
	SPI_CS_HIGH();							//拉高片选,结束连续发送,连续发送时,必须通过拉高片选结束发送
}


/************************************************************************************************
**	描	述：连续读取数据
**	参	数：Read_Addr :要读取数据的地址
**			pBuffer   :将读取的数据存入到pBuffer中
**			Length	  :读取数据的长度
**	返回值：无
*************************************************************************************************/

void Read_burst(uint8_t Read_Addr,uint8_t *data,uint8_t Length)
{
	uint8_t i = 0;
	SPI_CS_LOW();							//拉低片选
	while((GPIO_getInputPinValue(SPI_MISO_PORT, SPI_MISO_PIN)& 0x80) == GPIO_INPUT_PIN_HIGH); //等待MISO引脚为低
	SPI_Send(Read_Addr);
	for(i=0; i < Length;i++)
	{
		data[i] = SPI_Send(0);				//连续读取数据,经测试,把0改成READ_BURST也没问题
	}
	SPI_CS_HIGH();							//拉高片选
//	for(i=0; i < Length;i++)
//	{
//		printf("\n打印数据:%x\n",data[i]);
//	}
}

/************************************************************************************************
**	描	述：系统复位
**	参	数：无
**	返回值：无
*************************************************************************************************/

void CC1101_Reset(void)
{
	SPI_CS_LOW();							//拉低片选
	while((GPIO_getInputPinValue(SPI_MISO_PORT, SPI_MISO_PIN)& 0x80) == GPIO_INPUT_PIN_HIGH); //等待MISO引脚为低
	Write_CMD(SRES);
//	Delay_ms(20);							//延时20us
	SPI_CS_HIGH();							//拉高片选
}






