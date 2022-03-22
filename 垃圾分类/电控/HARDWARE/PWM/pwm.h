#ifndef _TIMER_H
#define _TIMER_H
#include "sys.h"

void TIM_PWM_Init(u32 arr,u32 psc);
void Set_Base_Angle(int angle_now);
void Set_Angle(int motor,int angle);
int  Get_Angle(int motor);
void On_Off(void);
void Angle_Init(void);
void Key_Angle(void);
	
extern u16 Angles[8];                        //存储舵机当前角度

#endif
