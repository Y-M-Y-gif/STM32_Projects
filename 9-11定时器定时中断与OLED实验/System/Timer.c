#include "stm32f10x.h"                  // Device header

//跨文件使用变量需声明
extern uint16_t Num;

void Timer_Init(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);//注意这里是APB1

	//显式声明更规范，其实选择内部时钟可以不写，因为上电默认就是内部时钟
	TIM_InternalClockConfig(TIM2);
	
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	
	TIM_TimeBaseInitStruct.TIM_ClockDivision=TIM_CKD_DIV1;//可以忽略
	TIM_TimeBaseInitStruct.TIM_CounterMode=TIM_CounterMode_Up;
	//下面配置周期，注意都是16位大小，配置的时候均衡一下
	TIM_TimeBaseInitStruct.TIM_Prescaler=7200-1;//配置预分频器
	TIM_TimeBaseInitStruct.TIM_Period=10000-1;//配置定时周期
	TIM_TimeBaseInitStruct.TIM_RepetitionCounter=0;//高级定时器才有
	
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStruct);
	
	//手动清除更新中断标志位,避免刚初始化完就进入中断
	TIM_ClearFlag(TIM2,TIM_FLAG_Update);
	
	//开启中断到NVIC的通路
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);
	
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//两位子优先级
	
	NVIC_InitTypeDef NVIC_InitStruct;
	
	NVIC_InitStruct.NVIC_IRQChannel = TIM2_IRQn;          //选择中断通道
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;  //抢占优先级(0~3)
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;         //子优先级(0~3)
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE; //使能这个中断通道
	
	NVIC_Init(&NVIC_InitStruct);
	
	//使能计数器
	TIM_Cmd(TIM2,ENABLE);
}

void TIM2_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM2,TIM_IT_Update)==SET){
		Num++;
		TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
	}

}
