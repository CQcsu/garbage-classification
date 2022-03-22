#include "main.h"

u8 Rxbuff[3];                    //UART1接收到的数据
u8 rxbuff[4];                    //UART2接收到的数据
u8 rxbuff1[4];
u8 Txbuff[8];                    //UART1发送的数据
u16 Angles[8];                   //舵机的当前角度
int UR2_Distance;                //1号超声波测得的平均距离
int UR4_Distance;				 //2号超声波测得的平均距离
int DISTANCE;
int UR2_Distance_real;           //1号超声波的最终采用距离
int UR4_Distance_real;			 //2号超声波的最终采用距离

//超声波调试开关
#if DEBUG_YU41
void YU41_Data(void)
{
	int i,j;
	for(j=0;j<20;j++)
	{
		USART_SendData(UART4,0x55);
		while(USART_GetFlagStatus(UART4,USART_FLAG_TC)!=SET);
		delay_ms(TIME_YU41);
		USART_SendData(USART1,0xAA);
		while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);
		for(i=0;i<4;i++)
		{
			USART_SendData(USART1,rxbuff1[i]);
			while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);
		}
		delay_ms(10);
	}
}
#endif

void SendData(void)              //UART1发送信息一帧为8个字节
{
	int i;
	for(i=0;i<8;i++)													//串口1向agx循环发送8次数据
	{
		USART_SendData(USART1,Txbuff[i]);								
		while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);			//等待发送完毕
	}
}

void  Get_UR2_Distance(void)            //1号超声波测距，取三次的平均值
{
	int i;
	int sum=0;
	UR2_Distance = 0;
	for(i=0;i<3;i++)
	{
		USART_SendData(USART2,YU41_START);			//发送超声波工作开始信号
		while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);		//等待发送完毕

		delay_ms(TIME_YU41);			//延时0.4秒，等待超声波响应

		if(rxbuff[0] == YU41_HEADER)	//判断串口2接收的第一位数据是否为超声波数据包头
		{
			if(rxbuff[3] == ((rxbuff[0] + rxbuff[1] + rxbuff[2])&0x00FF))		//检验超声波数据是否正确
				if((DISTANCE_MIN <= (rxbuff[1]*256+ rxbuff[2])) && ((rxbuff[1]*256+ rxbuff[2]) <= DISTANCE_MAX))	//去除错误数据
				{
					UR2_Distance += rxbuff[1]*256+ rxbuff[2];         				//计算距离
					sum++;														//计算数据接收成功并数据正确的次数
				}
		} 
    }
	UR2_Distance = UR2_Distance / sum ;													//计算距离的平均值
}

void Get_UR4_Distance(void)														//2号超声波测距，取三次平均值
{
	int i;
	int sum=0;
	UR4_Distance = 0;
	for(i=0;i<3;i++)
	{
		USART_SendData(UART4,YU41_START);										//发送超声波工作信号
		while(USART_GetFlagStatus(UART4,USART_FLAG_TC)!=SET);                   //等待发送完毕

		delay_ms(TIME_YU41);                                                    //延时0.4秒，等待超声波响应

		if(rxbuff1[0] == YU41_HEADER)											//判断串口2接收的第一位数据是否为超声波数据包头
		{
			if(rxbuff1[3] == ((rxbuff1[0] + rxbuff1[1] + rxbuff1[2])&0x00FF))   //检验超声波数据是否正确
				if((DISTANCE_MIN <= (rxbuff1[1]*256+ rxbuff1[2])) && ((rxbuff1[1]*256+ rxbuff1[2]) <= DISTANCE_MAX))     //去除错误数据
				{
					UR4_Distance += rxbuff1[1]*256+ rxbuff1[2];						//计算距离
					sum++;														//计算数据接收成功并数据正确的次数
				}
		} 
	}
	UR4_Distance = UR4_Distance / sum ;													//计算距离的平均值
}

int main(void)  
	
{
	u8 key;                         //按键
	int angle; 
	int i;
	uart1_init(115200);           									//连接agx, 波特率为115200
	uart2_init(9600);              									//连接1号超声波，波特率为9600
	uart4_init(9600);												//连接2号超声波，波特率为9600
	TIM_PWM_Init(20000-1,84-1);     								//PWM初始化，舵机频率为50hz
	delay_init(168);               									//延时初始化，使用外部高速晶振，系统时钟为168MHz
	KEY_Init();                     								//按键初始化
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);   				//中断优先级分组
	Angle_Init();          											//舵机初始化，各舵机转至初始角度
	for(i=0;i<8;i++)
		Angles[i]=0;
		
	while(1){
	UR2_Distance_real = 1000;     				   //初始值为1000
	UR4_Distance_real = 1000;                      
		
	if(Rxbuff[0] == U1_FLAG)                       //串口1中断进入标志位
	{
		#if WORK                                   //正常运行程序段
		if (Rxbuff[1] == U1_RX_FLAG)               //检测从agx接收的数据，包头是否正确
		{
			if(Rxbuff[2] == DEGREES_0)             //转到超声波位置进行满载检测
				angle = 180; 
			else if(Rxbuff[2] == DEGREES_90)     
				angle = 270;
			else if(Rxbuff[2] == DEGREES_180)      
				angle = 0;  
			else if(Rxbuff[2] == DEGREES_270)      
			    angle = 90;
			
			Set_Base_Angle(angle);            //转到满载检测处
			
			Get_UR2_Distance();                     //超声波1满载检测
			Get_UR4_Distance();					    //超声波2满载检测
			
			if((UR2_Distance > DISTANCE_MIN) && (UR2_Distance < DISTANCE_MAX))		//判断1号超声波测得的距离平均值是否为有效数据
				UR2_Distance_real = UR2_Distance;									//为有效值，则2号超声波的最终数据为2号超声波测得的平均数据
			
			if((UR4_Distance > DISTANCE_MIN) && (UR4_Distance < DISTANCE_MAX))      //判断2号超声波测得的距离平均值是否为有效数据
				UR4_Distance_real = UR4_Distance;									//为有效值，则2号超声波的最终数据为2号超声波测得的平均数据
			
			if(UR4_Distance_real < UR2_Distance_real)     							//取两个距离的最小值为最终距离 DISTANCE
				DISTANCE = UR4_Distance_real;
			else if(UR2_Distance_real < UR4_Distance_real)
				DISTANCE = UR2_Distance_real;
				
			if(DISTANCE <= DISTANCE_TV)        					//如果小于满载阈值，判断为满载
					Txbuff[1] = DISTANCE_FULL;					//Txbuff[1]置0
			else if(DISTANCE > DISTANCE_TV)						//如果大于满载阈值，判断为不满载
					Txbuff[1] = DISTANCE_EMPTY;					//TXbuff[1]置1
			
		    if(Rxbuff[2] == DEGREES_0)                          //将垃圾桶转至投放口位置进行投放
				angle = 0; 
			else if(Rxbuff[2] == DEGREES_90)     
				angle = 90;
			else if(Rxbuff[2] == DEGREES_180)      
				angle = 180;  
			else if(Rxbuff[2] == DEGREES_270)      
			    angle = 270;
			
			Set_Base_Angle(angle);                       //垃圾桶转至投放口
			
	        if(Txbuff[1] == DISTANCE_EMPTY)                     //如果不满载，则投放垃圾
				On_Off();                                       //挡板开合，投放垃圾，延时4秒

			Txbuff[0]=U1_TX_FLAG;                               //为给agx发送的数据赋值
			Txbuff[2]=DISTANCE >> 8;							//发给agx的距离的高8位
			Txbuff[3]=DISTANCE;                                 //发给agx的距离的低8位
			Txbuff[4]=UR2_Distance >> 8;						//1号超声波测得的距离的高8位
			Txbuff[5]=UR2_Distance;								//1号超声波测得的距离的低8位
			Txbuff[6]=UR4_Distance >> 8;						//2号超声波测得的距离的高8位
			Txbuff[7]=UR4_Distance;								//2号超声波测得的距离的低8位
			
			SendData();              							//发送数据，u8类型，一帧8个字节
			
		}
		#endif
		
		#if DEBUG_YU41                                     //超声波调试程序段
		if(Rxbuff[1] == U1_RX_FLAG)
		{
			if(Rxbuff[2] == DEGREES_0)             //转到超声波位置进行满载检测
				angle = 180; 
			else if(Rxbuff[2] == DEGREES_90)     
				angle = 270;
			else if(Rxbuff[2] == DEGREES_180)      
				angle = 0;  
			else if(Rxbuff[2] == DEGREES_270)      
			angle = 90;
			
			Set_Angle(MOTOR_BASE,angle);
			delay_ms(TIME_ANGLE);
			
			YU41_Data();      
	    }		
		#endif
		
		#if WORK_RS                                //仅可回收垃圾满载检测
	    if (Rxbuff[1] == U1_RX_FLAG)
		{
			if(Rxbuff[2] == DEGREES_0)             //转到超声波位置进行满载检测
				angle = 0; 
			else if(Rxbuff[2] == DEGREES_90)     
				angle = 270;
			else if(Rxbuff[2] == DEGREES_180)      
				angle = 180;  
			else if(Rxbuff[2] == DEGREES_270)      
			angle = 270;
			
			Set_Angle(MOTOR_BASE,angle);
			delay_ms(TIME_ANGLE);
			
			if(Rxbuff[2] == DEGREES_90)            //可回收垃圾满载检测
			{
				Get_Distance();                      
				Get_distance();
				
				if((Distance > 5) && (Distance < 400))
					Distance_real = Distance;
				if((distance > 5) && (distance < 400))
					distance_real = distance;
				
				if(distance_real < Distance_real)
					DISTANCE = distance_real;
				else if(Distance_real < distance_real)
					DISTANCE = Distance_real;
					
				if(DISTANCE <= DISTANCE_TV)
						Txbuff[1] = DISTANCE_FULL;
				else if(DISTANCE > DISTANCE_TV)
						Txbuff[1] = DISTANCE_EMPTY;
				
				if((Txbuff[1] == 1) && (Rxbuff[2] == DEGREES_90))
				{
					angle = 90;
					Set_Angle(MOTOR_BASE,angle);
					delay_ms(TIME_ANGLE);
					
					On_Off();                               //投放垃圾
				}
		    }
			if((Rxbuff[2] == DEGREES_0) || (Rxbuff[2] == DEGREES_180) || (Rxbuff[2] == DEGREES_270))
			{
				On_Off(); 
			}

			Txbuff[0]=U1_TX_FLAG; 
			Txbuff[2]=DISTANCE >> 8;
			Txbuff[3]=DISTANCE;
			Txbuff[4]=Distance >> 8;
			Txbuff[5]=Distance;
			Txbuff[6]=distance >> 8;
			Txbuff[7]=distance;
			
			SendData();
		}
		#endif

		
		#if WORK_GAME
	    if (Rxbuff[1] == U1_RX_FLAG)
		{
			if(Rxbuff[2] == DEGREES_0)                   //对应垃圾桶转动到投放口
				angle = 0; 
			else if(Rxbuff[2] == DEGREES_90)     
				angle = 90;
			else if(Rxbuff[2] == DEGREES_180)      
				angle = 180;  
			else if(Rxbuff[2] == DEGREES_270)      
			    angle = 270;

			Set_Base_Angle(angle);                      //垃圾桶转动到投放口 最小延时0，最大延时1.8秒
			
		    On_Off();                                   //挡板开合，投放垃圾,延时1秒
			
			angle = Get_Angle(MOTOR_BASE);
			
			Txbuff[0] = U1_TX_FLAG; 
			Txbuff[1] = angle>>8;
			Txbuff[2] = angle;
			SendData();                                 //发送数据
        }
		
		#endif
	}
	
	key=KEY_Scan(1);
	
	if(key==1)      									//按键，按一下桶转动90度
	{
		Key_Angle();	
	}
	//按键按下就不发送数据也不接受数据，同时超声波距离为0
	for(i=0;i<3;i++)
		Rxbuff[i]=NULL;
	
	for(i=0;i<8;i++)
	    Txbuff[i]=NULL;
	
	DISTANCE=0;
	delay_ms(10);
	}
}



