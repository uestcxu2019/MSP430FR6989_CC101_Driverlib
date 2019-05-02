/*
 * bsp_led.c
 *
 *  Created on: 2018��11��30��
 *      Author: xu
 */
#include "bsp_led.h"


/*
 *  ˵	�� : GPIO���ų�ʼ��
 *  ��	�� : ��
 *  ����ֵ  : ��
 */
void GPIO_InitConfig(void)
{
	//��ʼ��,�����е����ų�ʼ������Ϊ�����ƽ
	//����Ϊ�������
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

	//����Ϊ����͵�ƽ
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
 *  ˵	�� : LED������ų�ʼ��
 *  ��	�� : ��
 *  ����ֵ  : ��
 */
void LED_Config(void)
{
	//����GDIO0Ϊ��������
	GPIO_setAsInputPinWithPullDownResistor(IOCFG0_GPIO_PORT,IOCFG0_GPIO_PIN);

	//����GDIO2Ϊ��������
	GPIO_setAsInputPinWithPullDownResistor(IOCFG2_GPIO_PORT,IOCFG2_GPIO_PIN);
}


