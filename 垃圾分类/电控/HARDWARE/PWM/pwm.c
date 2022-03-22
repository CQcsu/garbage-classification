#include "pwm.h"
#include "main.h"

void TIM_PWM_Init(u32 arr,u32 psc)              //初始化TIM4和TIM3的1,2，3,4通道，8路PWM；
{		 					 
	
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE); 
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);  	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE); 	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE); 	
	 
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource6,GPIO_AF_TIM3);
    GPIO_PinAFConfig(GPIOC,GPIO_PinSource7,GPIO_AF_TIM3);
    GPIO_PinAFConfig(GPIOC,GPIO_PinSource8,GPIO_AF_TIM3);
    GPIO_PinAFConfig(GPIOC,GPIO_PinSource9,GPIO_AF_TIM3);	
	
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource6,GPIO_AF_TIM4);
    GPIO_PinAFConfig(GPIOB,GPIO_PinSource7,GPIO_AF_TIM4);
    GPIO_PinAFConfig(GPIOB,GPIO_PinSource8,GPIO_AF_TIM4);
    GPIO_PinAFConfig(GPIOB,GPIO_PinSource9,GPIO_AF_TIM4);	
	
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;     
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;       
	GPIO_Init(GPIOC,&GPIO_InitStructure);            
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;     
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        
	GPIO_Init(GPIOB,&GPIO_InitStructure);              
	  
	TIM_TimeBaseStructure.TIM_Prescaler=psc;  
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; 
	TIM_TimeBaseStructure.TIM_Period=arr;   
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseStructure);
	TIM_TimeBaseInit(TIM4,&TIM_TimeBaseStructure);
	
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; 
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; 
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; 
	
	TIM_OC1Init(TIM3, &TIM_OCInitStructure); 
	TIM_OC2Init(TIM3, &TIM_OCInitStructure);
	TIM_OC3Init(TIM3, &TIM_OCInitStructure);
	TIM_OC4Init(TIM3, &TIM_OCInitStructure);
	
	TIM_OC1Init(TIM4, &TIM_OCInitStructure);  
	TIM_OC2Init(TIM4, &TIM_OCInitStructure);
	TIM_OC3Init(TIM4, &TIM_OCInitStructure);
	TIM_OC4Init(TIM4, &TIM_OCInitStructure);

	TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);  
  TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);
	TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);
	TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);
	
	TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable); 
    TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);
	TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);
	TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable);
	
    TIM_ARRPreloadConfig(TIM3,ENABLE);
	TIM_ARRPreloadConfig(TIM4,ENABLE);
	
	TIM_Cmd(TIM3, ENABLE);  
	TIM_Cmd(TIM4, ENABLE);
										  
}  

//设置舵机的角度，第一个参数是舵机，第二个是角度
void Set_Angle(int motor,int angle)       
{
	 int PWM_out = 500 + 2000 / 270*angle;  //计算RCC的值，舵机0-270度对应占空比500-2500
	
	 if(motor == MOTOR_BASE)
		 TIM_SetCompare1(TIM4,PWM_out);
	 else if(motor == MOTOR_RIGHT)
		 TIM_SetCompare2(TIM4,PWM_out);
	 else if(motor == MOTOR_LEFT)
		 TIM_SetCompare3(TIM4,PWM_out);
	 
	 else if(motor == 4)
		 TIM_SetCompare4(TIM4,PWM_out);  //修改输出的占空比
	 else if(motor == 5)
		 TIM_SetCompare1(TIM3,PWM_out);
	 else if(motor == 6)
		 TIM_SetCompare2(TIM3,PWM_out);
	 else if(motor == 7)
		 TIM_SetCompare3(TIM3,PWM_out);
	 else if(motor == 8)
		 TIM_SetCompare4(TIM3,PWM_out);
	 Angles[motor-1] = angle;  //Angle存放的是8个舵机的角度（使用的是数组进行存储），下标从0开始，而这里的motor从1开始计数，所以要,motor-1
}

//桶子在不同角度，采用不同延时。转桶子的过程中需要延时来控制
void Set_Base_Angle(int angle_now)     
{
	int PWM_out = 500 + 2000 / 270 * angle_now;
    int angle_last = Get_Angle(MOTOR_BASE); //当前的舵机角度
	
	if(angle_now == angle_last)
	{
		TIM_SetCompare1(TIM4,PWM_out);
	}
	else if(((angle_now == 0) && (angle_last == 90)) || ((angle_now == 90) && (angle_last == 180)) ||
		    ((angle_now == 180) && (angle_last == 270)) || ((angle_now == 90) && (angle_last == 0)) || 
	        ((angle_now == 180) && (angle_last == 90)) || ((angle_now == 270) && (angle_last == 180))
	       )
	{
		TIM_SetCompare1(TIM4,PWM_out);
		delay_ms(TIME_ANGLE_90);
	}
	else if(((angle_now == 0) && (angle_last == 180)) || ((angle_now == 90) && (angle_last == 270)) ||
		    ((angle_now == 180) && (angle_last == 0)) || ((angle_now == 270) && (angle_last == 90))
	       )
	{
		TIM_SetCompare1(TIM4,PWM_out);
		delay_ms(TIME_ANGLE_180);
	}
	else if(((angle_now == 0) && (angle_last == 270)) || ((angle_now == 270) && (angle_last == 0)))
	{
		TIM_SetCompare1(TIM4,PWM_out);
		delay_ms(TIME_ANGLE_270);
	}
	
	Angles[MOTOR_BASE - 1] = angle_now;
}


int Get_Angle(int motor)                   //传入舵机标号，返回舵机当前角度
{
	return Angles[motor-1];
}

void On_Off(void)                            //挡板的开合 
{
    Set_Angle(MOTOR_RIGHT,160);   			//右侧托盘打开的角度                  
    Set_Angle(MOTOR_LEFT,90);				//左侧托盘打开的角度 
	delay_ms(TIME_ON_OFF);
	
	Set_Angle(MOTOR_RIGHT,270);
    Set_Angle(MOTOR_LEFT,0);
	delay_ms(TIME_ON_OFF);
}

void Angle_Init(void)                     //初始化舵机的状态
{
    Set_Angle(MOTOR_BASE,0);              //转盘
	Set_Angle(MOTOR_RIGHT,270);
	Set_Angle(MOTOR_LEFT,0);
	Set_Angle(4,0);                       //以下目前未使用
	Set_Angle(5,0);
	Set_Angle(6,0);
	Set_Angle(7,0);
	Set_Angle(8,0);
}
void Key_Angle(void)              		//按键扫描
{
    int angle;
    angle = Get_Angle(MOTOR_BASE);
	if( 0 <= angle && angle <= 180)
		angle += 90;
	else
		angle = 0;
	Set_Base_Angle(angle);
}







