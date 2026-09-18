#include "stm32f10x.h"                  // Device header

void IC_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);//注意这里是APB1

	GPIO_InitTypeDef GPIO_InitStruct;
	
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_IPU;//这里是复用，要把引脚控制权转移给片上外设
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_6;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	
	
	//显式声明更规范，其实选择内部时钟可以不写，因为上电默认就是内部时钟
	TIM_InternalClockConfig(TIM3);
	
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	
	TIM_TimeBaseInitStruct.TIM_ClockDivision=TIM_CKD_DIV1;//可以忽略
	TIM_TimeBaseInitStruct.TIM_CounterMode=TIM_CounterMode_Up;
	//下面配置周期，注意都是16位大小，配置的时候均衡一下
	TIM_TimeBaseInitStruct.TIM_Prescaler=72-1;//配置预分频器
	TIM_TimeBaseInitStruct.TIM_Period=10000-1;//这里设置大一些，防止溢出
	TIM_TimeBaseInitStruct.TIM_RepetitionCounter=0;//高级定时器才有
	
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStruct);
	
	TIM_ICInitTypeDef TIM_ICInitStruct;
	TIM_ICInitStruct.TIM_Channel=TIM_Channel_1;
	TIM_ICInitStruct.TIM_ICFilter=0xF;//配置滤波器
	TIM_ICInitStruct.TIM_ICPolarity=TIM_ICPolarity_Rising;//上升沿触发
	TIM_ICInitStruct.TIM_ICPrescaler=TIM_ICPSC_DIV1;
	TIM_ICInitStruct.TIM_ICSelection=TIM_ICSelection_DirectTI;
	TIM_ICInit(TIM3,&TIM_ICInitStruct);
	
	TIM_SelectInputTrigger(TIM3,TIM_TS_TI1FP1);
	
	TIM_SelectSlaveMode(TIM3,TIM_SlaveMode_Reset);
	
	TIM_Cmd(TIM3,ENABLE);
	
}

uint32_t IC_GetFreq(void)
{
	return 1000000/TIM_GetCapture1(TIM3);
}
