/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2020,��ɿƼ�
 * All rights reserved.
 * ��������QQȺ��һȺ��179029047(����)  ��Ⱥ��244861897(����)  ��Ⱥ��824575535
 *
 * �����������ݰ�Ȩ������ɿƼ����У�δ��������������ҵ��;��
 * ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
 *
 * @file       		main
 * @company	   		�ɶ���ɿƼ����޹�˾
 * @author     		��ɿƼ�(QQ790875685)
 * @version    		�鿴doc��version�ļ� �汾˵��
 * @Software 		MDK FOR C51 V9.60
 * @Target core		STC8H8K64S4
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2020-06-01
 ********************************************************************************************************************/

#include "headfile.h"
#include "menu.h"
#include "my_menu.h"
#include "device.h"
//board.h�ļ���FOSC��ֵ����Ϊ0,������Զ�ʶ��ϵͳƵ��

/*board.h�ļ���FOSC��ֵ���ò�Ϊ0����ϵͳƵ��ΪFOSC��ֵ��
��ʹ��stc-isp�������س����ʱ����Ҫ��IRCƵ������ΪFOSC��ֵ*/

/*��board_init��,�Ѿ���P54��������Ϊ��λ��
�����Ҫʹ��P54����,������board.c�ļ��е�board_init()������ɾ��SET_P54_RESRT����*/


void main()
{

	DisableGlobalIRQ();		//�ر����ж�
	board_init();			//��ʼ���ڲ��Ĵ�������ɾ���˾���롣

	encoder_init();
    inductance_init();	
	wireless_ch573_init();
	key_init();
	oled_init();
	delay_init();
	menu_init();
	//�˴���д�û�����(���磺�����ʼ�������)
	EnableGlobalIRQ();		//�������ж�

    while(1)
	{
		MenuCmd(key_scan());
		// delay_ms(1000);
		// MenuRender(1);
		
    }
}

