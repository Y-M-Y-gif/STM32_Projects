#include "stm32f10x.h"                  // Device header

void PWM_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);//注意这里是APB1

	GPIO_InitTypeDef GPIO_InitStruct;
	
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_AF_PP;//这里是复用，要把引脚控制权转移给片上外设
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_1;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	
	
	//显式声明更规范，其实选择内部时钟可以不写，因为上电默认就是内部时钟
	TIM_InternalClockConfig(TIM2);
	
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	
	TIM_TimeBaseInitStruct.TIM_ClockDivision=TIM_CKD_DIV1;//可以忽略
	TIM_TimeBaseInitStruct.TIM_CounterMode=TIM_CounterMode_Up;
	//下面配置周期，注意都是16位大小，配置的时候均衡一下
	TIM_TimeBaseInitStruct.TIM_Prescaler=72-1;//配置预分频器
	TIM_TimeBaseInitStruct.TIM_Period=20000-1;//配置定时周期
	TIM_TimeBaseInitStruct.TIM_RepetitionCounter=0;//高级定时器才有
	
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStruct);
	
	//配置输出比较通道
	TIM_OCInitTypeDef TIM_OCInitStructure;
	TIM_OCStructInit(&TIM_OCInitStructure);//给不需要的变量也赋上初始值，避免未定义的行为
	TIM_OCInitStructure.TIM_OCMode=TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OCPolarity=TIM_OCPolarity_High;//有效电平为高电平
	TIM_OCInitStructure.TIM_OutputState=TIM_OutputState_Enable;//闭合开关，输出使能
	TIM_OCInitStructure.TIM_Pulse=0;//CCR的值
	TIM_OC2Init(TIM2,&TIM_OCInitStructure);
	
	//使能计数器
	TIM_Cmd(TIM2,ENABLE);
}

void PWM_SetCompare2(uint16_t Compare)
{
	TIM_SetCompare2(TIM2,Compare);//运行过程中改变CCR的值
}
