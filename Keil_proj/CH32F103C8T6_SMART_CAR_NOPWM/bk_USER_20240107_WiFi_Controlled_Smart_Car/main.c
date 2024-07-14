//ͷ�ļ�
#include "stm32f10x.h"
#include "stm32f10x_exti.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_tim.h"

#define SPEED_DUTY 700

//��������
void GPIO_Configuration(void);
void LED_Config(void);
void INPUT_Config(void);
void NVIC_Configuration(void);
void EXTI9_5_IRQHandler(void); // Interrupt Handler: MOTOR & LED Control
void PWM_Init(void);

//=============================================================================
//�ļ����ƣ�Delay
//���ܸ�Ҫ����ʱ
//����˵����nCount����ʱ����
//�������أ���
//=============================================================================
void Delay(uint32_t nCount)
{
  for(; nCount != 0; nCount--);
}

// Key Scan
uint8_t Button_scan(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
	// check if the button is pressed
	if(GPIO_ReadInputDataBit(GPIOx, GPIO_Pin) == 1)
	{
		// wait for the button is released, filter the chattering
		while(GPIO_ReadInputDataBit(GPIOx, GPIO_Pin) == 1);
		return 1;
	}
	else return 0;
}

int16_t Speed6 = 0; //after power on, default is stop
int16_t Speed7 = 0; //after power on, default is stop
int16_t Speed8 = SPEED_DUTY; //after power on, default is stop
int16_t Speed9 = SPEED_DUTY; //after power on, default is stop

//=============================================================================
//�ļ����ƣ�main
//���ܸ�Ҫ��������
//����˵������
//�������أ�int
//=============================================================================
int main(void)
{
	LED_Config();	// LED control
	INPUT_Config();	// Input Pin Control
	NVIC_Configuration(); // Nested Vector Interrupt Control
	GPIO_Configuration(); // MOTOR control

	GPIO_ResetBits(GPIOA,GPIO_Pin_2);
	GPIO_ResetBits(GPIOA,GPIO_Pin_3);
	GPIO_ResetBits(GPIOA,GPIO_Pin_6);
	GPIO_ResetBits(GPIOA,GPIO_Pin_7);
	GPIO_ResetBits(GPIOA,GPIO_Pin_15);	
	GPIO_ResetBits(GPIOB,GPIO_Pin_0);
	GPIO_ResetBits(GPIOB,GPIO_Pin_1);
	GPIO_ResetBits(GPIOB,GPIO_Pin_3);
	GPIO_ResetBits(GPIOC,GPIO_Pin_13);

	PWM_Init(); // PWM
//	Delay(1000); // Filter the unexpected interrupt from ESP8266	
		
	while (1)	{
	}
}

//=============================================================================
//�ļ����ƣ�GPIO_Configuration
//���ܸ�Ҫ��GPIO��ʼ��
//����˵������
//�������أ���
//=============================================================================
void GPIO_Configuration(void)
{
//  GPIO_InitTypeDef GPIO_InitStructure; // LED
//	GPIO_InitTypeDef GPIO_PB08_VOICE_DIN; // Voice digital control in
//	GPIO_InitTypeDef GPIO_PA01_VOICE_AIN; // Voice analog control in
	GPIO_InitTypeDef GPIO_MOTOR_InitStruct;
//  GPIO_InitTypeDef GPIO_PA15_MOTOR_FR1; // Front Right1
//	GPIO_InitTypeDef GPIO_PB03_MOTOR_FR2; // Front Right2
//  GPIO_InitTypeDef GPIO_PA02_MOTOR_FL1; // Front Left1
//	GPIO_InitTypeDef GPIO_PA03_MOTOR_FL2; // Front Left2
//  GPIO_InitTypeDef GPIO_PA06_MOTOR_BR1; // Back Right1
//	GPIO_InitTypeDef GPIO_PA07_MOTOR_BR2; // Back Right2
//  GPIO_InitTypeDef GPIO_PB00_MOTOR_BL1; // Back Left1
//	GPIO_InitTypeDef GPIO_PB01_MOTOR_BL2; // Back Left2
  
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA , ENABLE); 				
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB , ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE); // For TIM2 GPIOA Remap
	GPIO_PinRemapConfig(GPIO_PartialRemap1_TIM2, ENABLE); // For TIM2 GPIOA Remap
	
//=============================================================================
//LED -> PC13 (PC13/PC14/PC15 speed should not exceed 2MHz)
//=============================================================================			 
//  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
//  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
//  GPIO_Init(GPIOC, &GPIO_InitStructure);
//=============================================================================
//VOICE CONTROL IN -> Digital in: PB08 / Analog in: PA01
//=============================================================================			 
//  GPIO_PB08_VOICE_DIN.GPIO_Pin = GPIO_Pin_8;
//  GPIO_PB08_VOICE_DIN.GPIO_Mode = GPIO_Mode_IN_FLOATING; 
//  GPIO_Init(GPIOB, &GPIO_PB08_VOICE_DIN);
//  GPIO_PA01_VOICE_AIN.GPIO_Pin = GPIO_Pin_1;
//  GPIO_PA01_VOICE_AIN.GPIO_Mode = GPIO_Mode_AIN; 
//  GPIO_Init(GPIOA, &GPIO_PA01_VOICE_AIN);
//=============================================================================
//MOTOR TIM2/TIM3 -> PWM output
//=============================================================================			 
  GPIO_MOTOR_InitStruct.GPIO_Pin = GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_15;
  GPIO_MOTOR_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_MOTOR_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP; 
  GPIO_Init(GPIOA, &GPIO_MOTOR_InitStruct);
  GPIO_MOTOR_InitStruct.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_3;
  GPIO_Init(GPIOB, &GPIO_MOTOR_InitStruct);
}

void LED_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
}

void INPUT_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;				
	EXTI_InitTypeDef EXTI_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9;//EXTI pins, PB6-PB9
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOB, &GPIO_InitStructure);			

	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource8);
	EXTI_InitStructure.EXTI_Line = EXTI_Line8;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource9);
	EXTI_InitStructure.EXTI_Line = EXTI_Line9;
	EXTI_Init(&EXTI_InitStructure);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource6);
	EXTI_InitStructure.EXTI_Line = EXTI_Line6;
	EXTI_Init(&EXTI_InitStructure);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource7);
	EXTI_InitStructure.EXTI_Line = EXTI_Line7;
	EXTI_Init(&EXTI_InitStructure);
}

void NVIC_Configuration(void)
{
	NVIC_InitTypeDef NVIC_InitStructure; 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);	
  NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn; 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;	
	NVIC_Init(&NVIC_InitStructure);						 
}

void EXTI9_5_IRQHandler(void)
{
  if(EXTI_GetFlagStatus(EXTI_Line8) != RESET) // Forward
  {
		// LED ON/OFF
		GPIO_WriteBit(GPIOC, GPIO_Pin_13, (BitAction)((1-GPIO_ReadOutputDataBit(GPIOC, GPIO_Pin_13))));
		
		// MOTOR CONTROL
		if(Speed8==SPEED_DUTY)
		{
			Speed8=0;
		}
		else
		{
			Speed8=SPEED_DUTY;	
		}
		TIM_SetCompare1(TIM2, Speed8);//TIM2 CH1 F
		TIM_SetCompare2(TIM2, 0);	
		TIM_SetCompare3(TIM2, Speed8);//TIM2 CH3 F
		TIM_SetCompare4(TIM2, 0);		
		TIM_SetCompare1(TIM3, Speed8);//TIM3 CH1 F
		TIM_SetCompare2(TIM3, 0);	
		TIM_SetCompare3(TIM3, Speed8);//TIM3 CH3 F
		TIM_SetCompare4(TIM3, 0);
		
		// Clear the interrupt
    EXTI_ClearFlag(EXTI_Line8);
  }	else if(EXTI_GetFlagStatus(EXTI_Line9) != RESET)  // Backward
  {
		// LED ON/OFF
		GPIO_WriteBit(GPIOC, GPIO_Pin_13, (BitAction)((1-GPIO_ReadOutputDataBit(GPIOC, GPIO_Pin_13))));
		
		// MOTOR CONTROL
		if(Speed9==SPEED_DUTY)
		{
			Speed9=0;
		}
		else
		{
			Speed9=SPEED_DUTY;	
		}
		TIM_SetCompare1(TIM2, 0);
		TIM_SetCompare2(TIM2, Speed9);	
		TIM_SetCompare3(TIM2, 0);
		TIM_SetCompare4(TIM2, Speed9);		
		TIM_SetCompare1(TIM3, 0);
		TIM_SetCompare2(TIM3, Speed9);	
		TIM_SetCompare3(TIM3, 0);
		TIM_SetCompare4(TIM3, Speed9);
		
		// Clear the interrupt
    EXTI_ClearFlag(EXTI_Line9);
  }	else if(EXTI_GetFlagStatus(EXTI_Line6) != RESET)  // Right
  {
		// LED ON/OFF
		GPIO_WriteBit(GPIOC, GPIO_Pin_13, (BitAction)((1-GPIO_ReadOutputDataBit(GPIOC, GPIO_Pin_13))));
		
		// MOTOR CONTROL
		if(Speed6==SPEED_DUTY)
		{
			Speed6=0;
		}
		else
		{
			Speed6=SPEED_DUTY;	
		}
		TIM_SetCompare1(TIM2, 0);
		TIM_SetCompare2(TIM2, Speed6);	
		TIM_SetCompare3(TIM2, Speed6);
		TIM_SetCompare4(TIM2, 0);		
		TIM_SetCompare1(TIM3, Speed6);
		TIM_SetCompare2(TIM3, 0);	
		TIM_SetCompare3(TIM3, 0);
		TIM_SetCompare4(TIM3, Speed6);
		
		// Clear the interrupt
    EXTI_ClearFlag(EXTI_Line6);
  }	else if(EXTI_GetFlagStatus(EXTI_Line7) != RESET)  // Left
  {
		// LED ON/OFF
		GPIO_WriteBit(GPIOC, GPIO_Pin_13, (BitAction)((1-GPIO_ReadOutputDataBit(GPIOC, GPIO_Pin_13))));
		
		// MOTOR CONTROL
		if(Speed7==SPEED_DUTY)
		{
			Speed7=0;
		}
		else
		{
			Speed7=SPEED_DUTY;	
		}
		TIM_SetCompare1(TIM2, Speed7);
		TIM_SetCompare2(TIM2, 0);	
		TIM_SetCompare3(TIM2, 0);
		TIM_SetCompare4(TIM2, Speed7);		
		TIM_SetCompare1(TIM3, 0);
		TIM_SetCompare2(TIM3, Speed7);	
		TIM_SetCompare3(TIM3, Speed7);
		TIM_SetCompare4(TIM3, 0);
		
		// Clear the interrupt
    EXTI_ClearFlag(EXTI_Line7);
  }
}

void PWM_Init(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2|RCC_APB1Periph_TIM3, ENABLE);

	TIM_InternalClockConfig(TIM2);
	TIM_InternalClockConfig(TIM3);
	
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period = 1000;		//ARR
	TIM_TimeBaseInitStructure.TIM_Prescaler = 36 - 1;		//PSC
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure);
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStructure);

	TIM_OCStructInit(&TIM_OCInitStructure);
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 0;		//CCR
	TIM_OC1Init(TIM2, &TIM_OCInitStructure);
	TIM_OC2Init(TIM2, &TIM_OCInitStructure);
	TIM_OC3Init(TIM2, &TIM_OCInitStructure);
	TIM_OC4Init(TIM2, &TIM_OCInitStructure);	
	TIM_OC1Init(TIM3, &TIM_OCInitStructure);
	TIM_OC2Init(TIM3, &TIM_OCInitStructure);
	TIM_OC3Init(TIM3, &TIM_OCInitStructure);
	TIM_OC4Init(TIM3, &TIM_OCInitStructure);

	TIM_Cmd(TIM2, ENABLE);
	TIM_Cmd(TIM3, ENABLE);	
}

