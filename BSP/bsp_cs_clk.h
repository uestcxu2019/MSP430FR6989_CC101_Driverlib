/*
 * bsp_cs_clk.h
 *
 *  Created on: 2018Äê11ÔÂ30ÈÕ
 *      Author: xu
 */

#ifndef BSP_BSP_CS_CLK_H_
#define BSP_BSP_CS_CLK_H_

#include "driverlib.h"


#define	 LXIN_PROT		GPIO_PORT_PJ
#define	 LXIN_PIN		GPIO_PIN4

#define	 LXOUT_PROT		GPIO_PORT_PJ
#define  LXOUT_PIN 		GPIO_PIN5


void CS_Init_Config(void);


#endif /* BSP_BSP_CS_CLK_H_ */
