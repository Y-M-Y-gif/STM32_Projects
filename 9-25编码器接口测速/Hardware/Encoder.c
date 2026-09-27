#include "stm32f10x.h"                  // Device header

void Encoder_Init(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);

	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_IPU;
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	
	//TIM_InternalClockConfig(TIM3)编码器接口是一个带方向控制的外部时钟，因此内部时钟无效

	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	TIM_TimeBaseStructInit(&TIM_TimeBaseInitStruct);//先初始配置，后面按需修改
	TIM_TimeBaseInitStruct.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseInitStruct.TIM_Period=65535-1;
	TIM_TimeBaseInitStruct.TIM_Prescaler=1-1;
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStruct);
	
	
	TIM_ICInitTypeDef TIM_ICInitStruct;
	TIM_ICStructInit(&TIM_ICInitStruct);//也是先赋基本初值
	TIM_ICInitStruct.TIM_Channel=TIM_Channel_1;
	TIM_ICInitStruct.TIM_ICFilter=0xF;
	TIM_ICInitStruct.TIM_ICPolarity=TIM_ICPolarity_BothEdge;
	TIM_ICInit(TIM3,&TIM_ICInitStruct);
	TIM_ICInitStruct.TIM_Channel=TIM_Channel_2;
	TIM_ICInitStruct.TIM_ICFilter=0xF;
	TIM_ICInitStruct.TIM_ICPolarity=TIM_ICPolarity_BothEdge;
	TIM_ICInit(TIM3,&TIM_ICInitStruct);
	
	//配置编码器接口
	TIM_EncoderInterfaceConfig(TIM3,TIM_EncoderMode_TI12,TIM_ICPolarity_Rising,TIM_ICPolarity_Rising);
	//硬件编码器本身固定会检测 TI1、TI2 的所有边沿,不再用这个参数选择捕获哪一类边沿！因而这里表示控制反相与否
	
	TIM_Cmd(TIM3,ENABLE);
}

int16_t Encode_Get(void)
{
	return TIM_GetCounter(TIM3);
}
