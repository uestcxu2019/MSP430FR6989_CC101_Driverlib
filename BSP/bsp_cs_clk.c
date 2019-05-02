/*
 * bsp_cs_clk.c
 *
 *  Created on: 2018��11��30��
 *      Author: xu
 */

/*
 *  ˵	�� : �ⲿ���پ�����������
 *  ��	�� : ��
 *  ����ֵ  : ��
 */
#include "bsp_cs_clk.h"

void E_LFXT_Config(void)
{
	GPIO_setAsPeripheralModuleFunctionOutputPin(LXIN_PROT,LXOUT_PIN,GPIO_PRIMARY_MODULE_FUNCTION);
	GPIO_setAsPeripheralModuleFunctionInputPin(LXIN_PROT,LXIN_PIN,GPIO_PRIMARY_MODULE_FUNCTION);
}


/*
 *  ˵	�� : ʱ�ӳ�ʼ������
 *  ��	�� : ��
 *  ����ֵ  : ��
 */
void CS_Init_Config(void)
{
	E_LFXT_Config();	//ʹ���ⲿ��������

//	CSCTL0 = 0xA500;
	//ʱ��Դѡ��,ѡ���ⲿ���پ���
	CS_setExternalClockSource(32.768,0);
	CS_turnOnLFXT(CS_LFXT_DRIVE_0);		//����Ƶ
	CS_initClockSignal(CS_ACLK,CS_LFXTCLK_SELECT,CS_CLOCK_DIVIDER_1);	//ACLKʱ��Դѡ���ⲿ����,1��Ƶ
	CS_initClockSignal(CS_MCLK,CS_DCOCLK_SELECT,CS_CLOCK_DIVIDER_1);	//MCLKʱ��Դѡ��DCO,2��Ƶ
	CS_initClockSignal(CS_SMCLK,CS_DCOCLK_SELECT,CS_CLOCK_DIVIDER_1);	//SMCLKʱ��Դѡ��DCO,2��Ƶ

	CS_turnOnSMCLK();			//����SMCLK
	CS_setDCOFreq(CS_DCORSEL_0,CS_DCOFSEL_3);		//����DCOʱ��Ƶ��,4MHz

	CS_enableClockRequest(CS_ACLK);				//�����ʱ�ӼĴ��������޸�
	CS_enableClockRequest(CS_MCLK);				//�����ʱ�ӼĴ��������޸�
	CS_enableClockRequest(CS_SMCLK);			//�����ʱ�ӼĴ��������޸�
}





