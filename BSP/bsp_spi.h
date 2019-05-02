/*
 * bsp_spi.h
 *
 *  Created on: 2018年11月30日
 *      Author: xu
 *      使用USCI_B1的 SPI功能
 */

#ifndef BSP_BSP_SPI_H_
#define BSP_BSP_SPI_H_

#include "driverlib.h"

//基地址
#define	 SPI_BASE_ADDR			EUSCI_B1_BASE

//引脚配置
//CLK
#define  SPI_CLK_PORT			GPIO_PORT_P4
#define  SPI_CLK_PIN			GPIO_PIN2

//CS
#define  SPI_CS_PORT			GPIO_PORT_P4
#define  SPI_CS_PIN				GPIO_PIN3

//MOSI
#define  SPI_MOSI_PORT			GPIO_PORT_P4
#define  SPI_MOSI_PIN			GPIO_PIN0

//MISO
#define  SPI_MISO_PORT			GPIO_PORT_P4
#define  SPI_MISO_PIN			GPIO_PIN1



void SPI_CS_HIGH(void);
void SPI_CS_LOW(void);

void SPI_Init(void);
uint8_t SPI_Send(uint8_t data);

#endif /* BSP_BSP_SPI_H_ */
