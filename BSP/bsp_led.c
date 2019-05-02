/*
 * bsp_led.c
 *
 *  Created on: 2018年11月30日
 *      Author: xu
 */
#include "bsp_led.h"


/*
 *  说	明 : GPIO引脚初始化
 *  参	数 : 无
 *  返回值  : 无
 */
void GPIO_InitConfig(void)
{
	//初始化,将所有的引脚初始化配置为输出电平
	//配置为输出方向
	GPIO_setAsOutputPin(GPIO_PORT_P1,GPIO_PIN_ALL8 );
	GPIO_setAsOutputPin(GPIO_PORT_P2,GPIO_PIN_ALL8 );
	GPIO_setAsOutputPin(GPIO_PORT_P3,GPIO_PIN_ALL8 );
	GPIO_setAsOutputPin(GPIO_PORT_P4,GPIO_PIN_ALL8 );
	GPIO_setAsOutputPin(GPIO_PORT_P5,GPIO_PIN_ALL8 );
	GPIO_setAsOutputPin(GPIO_PORT_P6,GPIO_PIN_ALL8 );
	GPIO_setAsOutputPin(GPIO_PORT_P7,GPIO_PIN_ALL8 );
	GPIO_setAsOutputPin(GPIO_PORT_P8,GPIO_PIN_ALL8 );
	GPIO_setAsOutputPin(GPIO_PORT_P9,GPIO_PIN_ALL8 );
	GPIO_setAsOutputPin(GPIO_PORT_P10,GPIO_PIN_ALL8 );
	GPIO_setAsOutputPin(GPIO_PORT_PJ,GPIO_PIN_ALL8 );

	//配置为输出低电平
	GPIO_setOutputLowOnPin(GPIO_PORT_P1,GPIO_PIN_ALL8);
	GPIO_setOutputLowOnPin(GPIO_PORT_P2,GPIO_PIN_ALL8);
	GPIO_setOutputLowOnPin(GPIO_PORT_P3,GPIO_PIN_ALL8);
	GPIO_setOutputLowOnPin(GPIO_PORT_P4,GPIO_PIN_ALL8);
	GPIO_setOutputLowOnPin(GPIO_PORT_P5,GPIO_PIN_ALL8);
	GPIO_setOutputLowOnPin(GPIO_PORT_P6,GPIO_PIN_ALL8);
	GPIO_setOutputLowOnPin(GPIO_PORT_P7,GPIO_PIN_ALL8);
	GPIO_setOutputLowOnPin(GPIO_PORT_P8,GPIO_PIN_ALL8);
	GPIO_setOutputLowOnPin(GPIO_PORT_P9,GPIO_PIN_ALL8);
	GPIO_setOutputLowOnPin(GPIO_PORT_P10,GPIO_PIN_ALL8);
	GPIO_setOutputLowOnPin(GPIO_PORT_PJ,GPIO_PIN_ALL8);
}


/*
 *  说	明 : LED相关引脚初始化
 *  参	数 : 无
 *  返回值  : 无
 */
void LED_Config(void)
{
	//配置GDIO0为输入下拉
	GPIO_setAsInputPinWithPullDownResistor(IOCFG0_GPIO_PORT,IOCFG0_GPIO_PIN);

	//配置GDIO2为输入下拉
	GPIO_setAsInputPinWithPullDownResistor(IOCFG2_GPIO_PORT,IOCFG2_GPIO_PIN);
}


