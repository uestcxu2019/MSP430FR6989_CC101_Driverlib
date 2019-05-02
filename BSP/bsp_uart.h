/*
 * bsp_uart.h
 *
 *  Created on: 2018年11月27日
 *      Author: xu
 */

#ifndef BSP_BSP_UART_H_
#define BSP_BSP_UART_H_

#include "driverlib.h"


#define		UART_BASE_ADDR			EUSCI_A1_BASE
//uart管脚配置
#define		UART_GPIO_PORT			GPIO_PORT_P3
#define		UART_GPIO_TX_PIN		GPIO_PIN4
#define		UART_GPIO_RX_PIN		GPIO_PIN5


void UART_GPIO_Config(void);
void UART_InitConfig(void);
void UART_Init(void);

void SendByte(uint8_t Data);
void UART_Send(uint8_t *Tx_Buffer,uint16_t Length);
void UART_Send_String(uint8_t *str);
void Myprintf(uint8_t *str);
void UART_ReceiveData(uint8_t *Rx_Buffer,uint8_t Length);

#endif /* BSP_BSP_UART_H_ */
