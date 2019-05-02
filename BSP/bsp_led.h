/*
 * bsp_led.h
 *	硬件连接:LED1 --> P1.0
 *  Created on: 2018年11月30日
 *      Author: xu
 */

#ifndef BSP_BSP_LED_H_
#define BSP_BSP_LED_H_

#include "driverlib.h"

#define	 LED1_GPIO_PORT			GPIO_PORT_P1
#define  LED1_GPIO_PIN			GPIO_PIN0

//GDIO0
#define   IOCFG0_GPIO_PORT      GPIO_PORT_P2
#define   IOCFG0_GPIO_PIN       GPIO_PIN0

//GDIO2
#define	  IOCFG2_GPIO_PORT		GPIO_PORT_P3
#define	  IOCFG2_GPIO_PIN		GPIO_PIN2

void GPIO_InitConfig(void);
void LED_Config(void);

#endif /* BSP_BSP_LED_H_ */
