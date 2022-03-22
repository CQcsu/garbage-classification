#include "main.h"

u8 Rxbuff[3];                    //UART1���յ�������
u8 rxbuff[4];                    //UART2���յ�������
u8 rxbuff1[4];
u8 Txbuff[8];                    //UART1���͵�����
u16 Angles[8];                   //����ĵ�ǰ�Ƕ�
int UR2_Distance;                //1�ų�������õ�ƽ������
int UR4_Distance;				 //2�ų�������õ�ƽ������
int DISTANCE;
int UR2_Distance_real;           //1�ų����������ղ��þ���
int UR4_Distance_real;			 //2�ų����������ղ��þ���

//���������Կ���
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

void SendData(void)              //UART1������Ϣһ֡Ϊ8���ֽ�
{
	int i;
	for(i=0;i<8;i++)													//����1��agxѭ������8������
	{
		USART_SendData(USART1,Txbuff[i]);								
		while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);			//�ȴ��������
	}
}

void  Get_UR2_Distance(void)            //1�ų�������࣬ȡ���ε�ƽ��ֵ
{
	int i;
	int sum=0;
	UR2_Distance = 0;
	for(i=0;i<3;i++)
	{
		USART_SendData(USART2,YU41_START);			//���ͳ�����������ʼ�ź�
		while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);		//�ȴ��������

		delay_ms(TIME_YU41);			//��ʱ0.4�룬�ȴ���������Ӧ

		if(rxbuff[0] == YU41_HEADER)	//�жϴ���2���յĵ�һλ�����Ƿ�Ϊ���������ݰ�ͷ
		{
			if(rxbuff[3] == ((rxbuff[0] + rxbuff[1] + rxbuff[2])&0x00FF))		//���鳬���������Ƿ���ȷ
				if((DISTANCE_MIN <= (rxbuff[1]*256+ rxbuff[2])) && ((rxbuff[1]*256+ rxbuff[2]) <= DISTANCE_MAX))	//ȥ����������
				{
					UR2_Distance += rxbuff[1]*256+ rxbuff[2];         				//�������
					sum++;														//�������ݽ��ճɹ���������ȷ�Ĵ���
				}
		} 
    }
	UR2_Distance = UR2_Distance / sum ;													//��������ƽ��ֵ
}

void Get_UR4_Distance(void)														//2�ų�������࣬ȡ����ƽ��ֵ
{
	int i;
	int sum=0;
	UR4_Distance = 0;
	for(i=0;i<3;i++)
	{
		USART_SendData(UART4,YU41_START);										//���ͳ����������ź�
		while(USART_GetFlagStatus(UART4,USART_FLAG_TC)!=SET);                   //�ȴ��������

		delay_ms(TIME_YU41);                                                    //��ʱ0.4�룬�ȴ���������Ӧ

		if(rxbuff1[0] == YU41_HEADER)											//�жϴ���2���յĵ�һλ�����Ƿ�Ϊ���������ݰ�ͷ
		{
			if(rxbuff1[3] == ((rxbuff1[0] + rxbuff1[1] + rxbuff1[2])&0x00FF))   //���鳬���������Ƿ���ȷ
				if((DISTANCE_MIN <= (rxbuff1[1]*256+ rxbuff1[2])) && ((rxbuff1[1]*256+ rxbuff1[2]) <= DISTANCE_MAX))     //ȥ����������
				{
					UR4_Distance += rxbuff1[1]*256+ rxbuff1[2];						//�������
					sum++;														//�������ݽ��ճɹ���������ȷ�Ĵ���
				}
		} 
	}
	UR4_Distance = UR4_Distance / sum ;													//��������ƽ��ֵ
}

int main(void)  
	
{
	u8 key;                         //����
	int angle; 
	int i;
	uart1_init(115200);           									//����agx, ������Ϊ115200
	uart2_init(9600);              									//����1�ų�������������Ϊ9600
	uart4_init(9600);												//����2�ų�������������Ϊ9600
	TIM_PWM_Init(20000-1,84-1);     								//PWM��ʼ�������Ƶ��Ϊ50hz
	delay_init(168);               									//��ʱ��ʼ����ʹ���ⲿ���پ���ϵͳʱ��Ϊ168MHz
	KEY_Init();                     								//������ʼ��
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);   				//�ж����ȼ�����
	Angle_Init();          											//�����ʼ���������ת����ʼ�Ƕ�
	for(i=0;i<8;i++)
		Angles[i]=0;
		
	while(1){
	UR2_Distance_real = 1000;     				   //��ʼֵΪ1000
	UR4_Distance_real = 1000;                      
		
	if(Rxbuff[0] == U1_FLAG)                       //����1�жϽ����־λ
	{
		#if WORK                                   //�������г����
		if (Rxbuff[1] == U1_RX_FLAG)               //����agx���յ����ݣ���ͷ�Ƿ���ȷ
		{
			if(Rxbuff[2] == DEGREES_0)             //ת��������λ�ý������ؼ��
				angle = 180; 
			else if(Rxbuff[2] == DEGREES_90)     
				angle = 270;
			else if(Rxbuff[2] == DEGREES_180)      
				angle = 0;  
			else if(Rxbuff[2] == DEGREES_270)      
			    angle = 90;
			
			Set_Base_Angle(angle);            //ת�����ؼ�⴦
			
			Get_UR2_Distance();                     //������1���ؼ��
			Get_UR4_Distance();					    //������2���ؼ��
			
			if((UR2_Distance > DISTANCE_MIN) && (UR2_Distance < DISTANCE_MAX))		//�ж�1�ų�������õľ���ƽ��ֵ�Ƿ�Ϊ��Ч����
				UR2_Distance_real = UR2_Distance;									//Ϊ��Чֵ����2�ų���������������Ϊ2�ų�������õ�ƽ������
			
			if((UR4_Distance > DISTANCE_MIN) && (UR4_Distance < DISTANCE_MAX))      //�ж�2�ų�������õľ���ƽ��ֵ�Ƿ�Ϊ��Ч����
				UR4_Distance_real = UR4_Distance;									//Ϊ��Чֵ����2�ų���������������Ϊ2�ų�������õ�ƽ������
			
			if(UR4_Distance_real < UR2_Distance_real)     							//ȡ�����������СֵΪ���վ��� DISTANCE
				DISTANCE = UR4_Distance_real;
			else if(UR2_Distance_real < UR4_Distance_real)
				DISTANCE = UR2_Distance_real;
				
			if(DISTANCE <= DISTANCE_TV)        					//���С��������ֵ���ж�Ϊ����
					Txbuff[1] = DISTANCE_FULL;					//Txbuff[1]��0
			else if(DISTANCE > DISTANCE_TV)						//�������������ֵ���ж�Ϊ������
					Txbuff[1] = DISTANCE_EMPTY;					//TXbuff[1]��1
			
		    if(Rxbuff[2] == DEGREES_0)                          //������Ͱת��Ͷ�ſ�λ�ý���Ͷ��
				angle = 0; 
			else if(Rxbuff[2] == DEGREES_90)     
				angle = 90;
			else if(Rxbuff[2] == DEGREES_180)      
				angle = 180;  
			else if(Rxbuff[2] == DEGREES_270)      
			    angle = 270;
			
			Set_Base_Angle(angle);                       //����Ͱת��Ͷ�ſ�
			
	        if(Txbuff[1] == DISTANCE_EMPTY)                     //��������أ���Ͷ������
				On_Off();                                       //���忪�ϣ�Ͷ����������ʱ4��

			Txbuff[0]=U1_TX_FLAG;                               //Ϊ��agx���͵����ݸ�ֵ
			Txbuff[2]=DISTANCE >> 8;							//����agx�ľ���ĸ�8λ
			Txbuff[3]=DISTANCE;                                 //����agx�ľ���ĵ�8λ
			Txbuff[4]=UR2_Distance >> 8;						//1�ų�������õľ���ĸ�8λ
			Txbuff[5]=UR2_Distance;								//1�ų�������õľ���ĵ�8λ
			Txbuff[6]=UR4_Distance >> 8;						//2�ų�������õľ���ĸ�8λ
			Txbuff[7]=UR4_Distance;								//2�ų�������õľ���ĵ�8λ
			
			SendData();              							//�������ݣ�u8���ͣ�һ֡8���ֽ�
			
		}
		#endif
		
		#if DEBUG_YU41                                     //���������Գ����
		if(Rxbuff[1] == U1_RX_FLAG)
		{
			if(Rxbuff[2] == DEGREES_0)             //ת��������λ�ý������ؼ��
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
		
		#if WORK_RS                                //���ɻ����������ؼ��
	    if (Rxbuff[1] == U1_RX_FLAG)
		{
			if(Rxbuff[2] == DEGREES_0)             //ת��������λ�ý������ؼ��
				angle = 0; 
			else if(Rxbuff[2] == DEGREES_90)     
				angle = 270;
			else if(Rxbuff[2] == DEGREES_180)      
				angle = 180;  
			else if(Rxbuff[2] == DEGREES_270)      
			angle = 270;
			
			Set_Angle(MOTOR_BASE,angle);
			delay_ms(TIME_ANGLE);
			
			if(Rxbuff[2] == DEGREES_90)            //�ɻ����������ؼ��
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
					
					On_Off();                               //Ͷ������
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
			if(Rxbuff[2] == DEGREES_0)                   //��Ӧ����Ͱת����Ͷ�ſ�
				angle = 0; 
			else if(Rxbuff[2] == DEGREES_90)     
				angle = 90;
			else if(Rxbuff[2] == DEGREES_180)      
				angle = 180;  
			else if(Rxbuff[2] == DEGREES_270)      
			    angle = 270;

			Set_Base_Angle(angle);                      //����Ͱת����Ͷ�ſ� ��С��ʱ0�������ʱ1.8��
			
		    On_Off();                                   //���忪�ϣ�Ͷ������,��ʱ1��
			
			angle = Get_Angle(MOTOR_BASE);
			
			Txbuff[0] = U1_TX_FLAG; 
			Txbuff[1] = angle>>8;
			Txbuff[2] = angle;
			SendData();                                 //��������
        }
		
		#endif
	}
	
	key=KEY_Scan(1);
	
	if(key==1)      									//��������һ��Ͱת��90��
	{
		Key_Angle();	
	}
	//�������¾Ͳ���������Ҳ���������ݣ�ͬʱ����������Ϊ0
	for(i=0;i<3;i++)
		Rxbuff[i]=NULL;
	
	for(i=0;i<8;i++)
	    Txbuff[i]=NULL;
	
	DISTANCE=0;
	delay_ms(10);
	}
}



