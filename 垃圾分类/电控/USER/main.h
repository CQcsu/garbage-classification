#ifndef __MAIN_H
#define __MAIN_H
#include "stdio.h"	
#include "stm32f4xx.h"
#include "usart.h"
#include "delay.h"
#include "pwm.h"
#include "led.h"
#include "beep.h"
#include "key.h"	

//���ؼ�������ֵ
#define DISTANCE_TV   150

//���غϸ�ֵ�����ֵ
#define DISTANCE_MAX  400
#define DISTANCE_MIN  5

// ��������ͷ
#define YU41_HEADER   0xFF
#define YU41_START    0x55

//��ʱ
#define TIME_YU41     	80 			//��������ʱ
#define TIME_ANGLE_90 	750			//ת��90����ʱ
#define TIME_ANGLE_180	1300		//ת��180����ʱ
#define TIME_ANGLE_270	1800        //ת��270����ʱ
#define TIME_ON_OFF   	500         //���忪����ʱ������������ʱ

//���
#define MOTOR_BASE    1				//һ�Ŷ��������
#define MOTOR_RIGHT   2				//���Ŷ�����ҵ���
#define MOTOR_LEFT    3				//���Ŷ�����󵲰�

//�Ƕ�
#define DEGREES_0      0x01			//����ת��0��
#define DEGREES_90     0x02			//����ת��90��
#define DEGREES_180    0x03			//����ת��180��
#define DEGREES_270    0x04			//����ת��270��

//��־λ
#define U1_FLAG        0x01			//����1��־λ
#define U1_RX_FLAG     0x01			//����1���ձ�־λ
#define U1_TX_FLAG     0x01			//����1���ͱ�־λ
#define DISTANCE_FULL  0x00			//����Ͱ���ر�־λ
#define DISTANCE_EMPTY 0x01			//����Ͱδ���ر�־λ

//���������Կ���
#define DEBUG_YU41     0

//ȫ�����ؿ���
#define WORK           0

//�ɻ������ؿ���
#define WORK_RS        0

//��������ο���
#define WORK_GAME      1


extern u8 Rxbuff[3];                    //UART1���յ�������
extern u8 rxbuff[4];                    //UART2���յ�������
extern u8 rxbuff1[4];
extern u8 Txbuff[8];                    //UART1���͵�����
extern u16 Angles[8];                   //����ĵ�ǰ�Ƕ�
extern int UR2_Distance;                //1�ų�������õ�ƽ������
extern int UR4_Distance;				 //2�ų�������õ�ƽ������
extern int DISTANCE;
extern int UR2_Distance_real;            //1�ų����������ղ��þ���
extern int UR4_Distance_real;			 //2�ų����������ղ��þ���

#endif







