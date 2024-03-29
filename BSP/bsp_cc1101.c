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
	SPI_CS_HIGH();							/*拉高片选*/
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
	SPI_Send(Write_Addr);					//发送要写的寄存器的地址,突发位必须为1

	for(i=0; i < Length;i++)
	{
		SPI_Send(pbuffer[i]);				//循环发送数据
	}
	SPI_CS_HIGH();							//拉高片选,结束连续发送,连续发送时,必须通过拉高片选结束发送
}


/************************************************************************************************
**	描	述：CC1101系统复位
**	参	数：无
**	返回值：无
*************************************************************************************************/
void CC1101_Reset(void)
{
	Write_CMD(SRES);
}


/************************************************************************************************
**	描	述：CC1101初始化,配置模式: 数据速率:1.2kBaud,调试方式:GFSK,频带:433MHz
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
	Write_Data(PATABLE,0x1D);			//输出功率控制
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
**	描	述：发送数据,将缓冲区数据全部发送出去(多数据)
**	参	数：pBuffer:需要发送的数据,len:发送数据的长度
**	返回值：无
*************************************************************************************************/
void CC1101_RFDataPack_Send(uint8_t *pBuff, uint16_t len)
{
    Write_CMD(SFTX);    			//清空发送缓冲区,只能在IDLE状态下清空
    WriteTxFITO(pBuff, len);     	//写入数据到发送缓冲区
    Write_CMD(STX);     			//进入发送模式开始发送数据,初始化配置过程中已经配置好发送完成后进入空闲模式
    Delay_us(1200);
    // 此种方式会出现卡死，耗时
/*   while(GPIO_getInputPinValue(IOCFG2_GPIO_PORT, IOCFG2_GPIO_PIN) !=GPIO_INPUT_PIN_HIGH)
	{
		//发送到同步字时置位,到数据包末尾取消置位,如果GDI2不为1,啥也不干,空等
	}
	while(GPIO_getInputPinValue(IOCFG2_GPIO_PORT, IOCFG2_GPIO_PIN) ==GPIO_INPUT_PIN_HIGH);	//条件为真,说明数据包还未发完*/
}
