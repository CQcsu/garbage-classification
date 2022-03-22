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

//满载检测距离阈值
#define DISTANCE_TV   150

//满载合格值检测阈值
#define DISTANCE_MAX  400
#define DISTANCE_MIN  5

// 超声波包头
#define YU41_HEADER   0xFF
#define YU41_START    0x55

//延时
#define TIME_YU41     	80 			//超声波延时
#define TIME_ANGLE_90 	750			//转动90度延时
#define TIME_ANGLE_180	1300		//转动180度延时
#define TIME_ANGLE_270	1800        //转动270度延时
#define TIME_ON_OFF   	500         //挡板开合延时，进行两次延时

//舵机
#define MOTOR_BASE    1				//一号舵机，底盘
#define MOTOR_RIGHT   2				//二号舵机，右挡板
#define MOTOR_LEFT    3				//三号舵机，左挡板

//角度
#define DEGREES_0      0x01			//底盘转至0度
#define DEGREES_90     0x02			//底盘转至90度
#define DEGREES_180    0x03			//底盘转至180度
#define DEGREES_270    0x04			//底盘转至270度

//标志位
#define U1_FLAG        0x01			//串口1标志位
#define U1_RX_FLAG     0x01			//串口1接收标志位
#define U1_TX_FLAG     0x01			//串口1发送标志位
#define DISTANCE_FULL  0x00			//垃圾桶满载标志位
#define DISTANCE_EMPTY 0x01			//垃圾桶未满载标志位

//超声波调试开关
#define DEBUG_YU41     0

//全部满载开关
#define WORK           0

//可回收满载开关
#define WORK_RS        0

//决赛程序段开关
#define WORK_GAME      1


extern u8 Rxbuff[3];                    //UART1接收到的数据
extern u8 rxbuff[4];                    //UART2接收到的数据
extern u8 rxbuff1[4];
extern u8 Txbuff[8];                    //UART1发送的数据
extern u16 Angles[8];                   //舵机的当前角度
extern int UR2_Distance;                //1号超声波测得的平均距离
extern int UR4_Distance;				 //2号超声波测得的平均距离
extern int DISTANCE;
extern int UR2_Distance_real;            //1号超声波的最终采用距离
extern int UR4_Distance_real;			 //2号超声波的最终采用距离

#endif







