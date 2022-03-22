#include "pwm.h"
#include "main.h"

void TIM_PWM_Init(u32 arr,u32 psc)              //��ʼ��TIM4��TIM3��1,2��3,4ͨ����8·PWM��
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

//���ö���ĽǶȣ���һ�������Ƕ�����ڶ����ǽǶ�
void Set_Angle(int motor,int angle)       
{
	 int PWM_out = 500 + 2000 / 270*angle;  //����RCC��ֵ�����0-270�ȶ�Ӧռ�ձ�500-2500
	
	 if(motor == MOTOR_BASE)
		 TIM_SetCompare1(TIM4,PWM_out);
	 else if(motor == MOTOR_RIGHT)
		 TIM_SetCompare2(TIM4,PWM_out);
	 else if(motor == MOTOR_LEFT)
		 TIM_SetCompare3(TIM4,PWM_out);
	 
	 else if(motor == 4)
		 TIM_SetCompare4(TIM4,PWM_out);  //�޸������ռ�ձ�
	 else if(motor == 5)
		 TIM_SetCompare1(TIM3,PWM_out);
	 else if(motor == 6)
		 TIM_SetCompare2(TIM3,PWM_out);
	 else if(motor == 7)
		 TIM_SetCompare3(TIM3,PWM_out);
	 else if(motor == 8)
		 TIM_SetCompare4(TIM3,PWM_out);
	 Angles[motor-1] = angle;  //Angle��ŵ���8������ĽǶȣ�ʹ�õ���������д洢�����±��0��ʼ���������motor��1��ʼ����������Ҫ,motor-1
}

//Ͱ���ڲ�ͬ�Ƕȣ����ò�ͬ��ʱ��תͰ�ӵĹ�������Ҫ��ʱ������
void Set_Base_Angle(int angle_now)     
{
	int PWM_out = 500 + 2000 / 270 * angle_now;
    int angle_last = Get_Angle(MOTOR_BASE); //��ǰ�Ķ���Ƕ�
	
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


int Get_Angle(int motor)                   //��������ţ����ض����ǰ�Ƕ�
{
	return Angles[motor-1];
}

void On_Off(void)                            //����Ŀ��� 
{
    Set_Angle(MOTOR_RIGHT,160);   			//�Ҳ����̴򿪵ĽǶ�                  
    Set_Angle(MOTOR_LEFT,90);				//������̴򿪵ĽǶ� 
	delay_ms(TIME_ON_OFF);
	
	Set_Angle(MOTOR_RIGHT,270);
    Set_Angle(MOTOR_LEFT,0);
	delay_ms(TIME_ON_OFF);
}

void Angle_Init(void)                     //��ʼ�������״̬
{
    Set_Angle(MOTOR_BASE,0);              //ת��
	Set_Angle(MOTOR_RIGHT,270);
	Set_Angle(MOTOR_LEFT,0);
	Set_Angle(4,0);                       //����Ŀǰδʹ��
	Set_Angle(5,0);
	Set_Angle(6,0);
	Set_Angle(7,0);
	Set_Angle(8,0);
}
void Key_Angle(void)              		//����ɨ��
{
    int angle;
    angle = Get_Angle(MOTOR_BASE);
	if( 0 <= angle && angle <= 180)
		angle += 90;
	else
		angle = 0;
	Set_Base_Angle(angle);
}







