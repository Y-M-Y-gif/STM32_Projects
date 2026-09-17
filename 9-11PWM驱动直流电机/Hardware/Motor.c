#include "stm32f10x.h"                  // Device header
#include "PWM.h"

void Motor_Init(void)
{
	//电机方向控制引脚初始化
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_Out_PP;//这里不是AF！
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_4|GPIO_Pin_5;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	
	PWM_Init();
}

void Motor_SetSpeed(int8_t Speed)
{
	if(Speed>=0){
		GPIO_SetBits(GPIOA,GPIO_Pin_4);
		GPIO_ResetBits(GPIOA,GPIO_Pin_5);
		PWM_SetCompare3(Speed);
	}else{
		GPIO_SetBits(GPIOA,GPIO_Pin_5);
		GPIO_ResetBits(GPIOA,GPIO_Pin_4);
		PWM_SetCompare3(-Speed);
	}
}
