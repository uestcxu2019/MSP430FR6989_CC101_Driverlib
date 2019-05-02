/*
 * bsp_cs_clk.c
 *
 *  Created on: 2018年11月30日
 *      Author: xu
 */

/*
 *  说	明 : 外部低速晶振引脚配置
 *  参	数 : 无
 *  返回值  : 无
 */
#include "bsp_cs_clk.h"

void E_LFXT_Config(void)
{
	GPIO_setAsPeripheralModuleFunctionOutputPin(LXIN_PROT,LXOUT_PIN,GPIO_PRIMARY_MODULE_FUNCTION);
	GPIO_setAsPeripheralModuleFunctionInputPin(LXIN_PROT,LXIN_PIN,GPIO_PRIMARY_MODULE_FUNCTION);
}


/*
 *  说	明 : 时钟初始化配置
 *  参	数 : 无
 *  返回值  : 无
 */
void CS_Init_Config(void)
{
	E_LFXT_Config();	//使能外部晶振引脚

//	CSCTL0 = 0xA500;
	//时钟源选择,选择外部低速晶振
	CS_setExternalClockSource(32.768,0);
	CS_turnOnLFXT(CS_LFXT_DRIVE_0);		//不分频
	CS_initClockSignal(CS_ACLK,CS_LFXTCLK_SELECT,CS_CLOCK_DIVIDER_1);	//ACLK时钟源选择外部晶振,1分频
	CS_initClockSignal(CS_MCLK,CS_DCOCLK_SELECT,CS_CLOCK_DIVIDER_1);	//MCLK时钟源选择DCO,2分频
	CS_initClockSignal(CS_SMCLK,CS_DCOCLK_SELECT,CS_CLOCK_DIVIDER_1);	//SMCLK时钟源选择DCO,2分频

	CS_turnOnSMCLK();			//开启SMCLK
	CS_setDCOFreq(CS_DCORSEL_0,CS_DCOFSEL_3);		//设置DCO时钟频率,4MHz

	CS_enableClockRequest(CS_ACLK);				//允许对时钟寄存器进行修改
	CS_enableClockRequest(CS_MCLK);				//允许对时钟寄存器进行修改
	CS_enableClockRequest(CS_SMCLK);			//允许对时钟寄存器进行修改
}





