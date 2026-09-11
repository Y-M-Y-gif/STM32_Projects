#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "math.h"

void App_PWM_Init(void);

int main(void)
{
	App_PWM_Init();
	
	uint32_t tick=0;
	
	while(1)
	{
		float time=tick*1.0e-3f; //假装执行一轮循环耗时1ms
		float duty=0.5*(sin(2*3.14*time)+1);
		
		uint16_t crr1=duty*1000;
		
		TIM_SetCompare1(TIM1,crr1);
	}
}

void App_PWM_Init(void){
	//初始化IO引脚PA8和PB13
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_8;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_2MHz;
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_13;
	GPIO_Init(GPIOB,&GPIO_InitStruct);
	
	//配置时基单元	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;

	TIM_TimeBaseInitStruct.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseInitStruct.TIM_Period=999;
	TIM_TimeBaseInitStruct.TIM_Prescaler=71;
	TIM_TimeBaseInitStruct.TIM_RepetitionCounter=0;
	
	TIM_TimeBaseInit(TIM1,&TIM_TimeBaseInitStruct);
	
	TIM_ARRPreloadConfig(TIM1,ENABLE);
	
	TIM_CCPreloadControl(TIM1,ENABLE);
	
	TIM_Cmd(TIM1,ENABLE);
	
	//配置输出比较参数
	TIM_OCInitTypeDef TIM_OCInitStruct;
	
	TIM_OCInitStruct.TIM_OCMode=TIM_OCMode_PWM1;
	TIM_OCInitStruct.TIM_OCNPolarity=TIM_OCPolarity_High;
	TIM_OCInitStruct.TIM_OCPolarity-TIM_OCPolarity_High;
	TIM_OCInitStruct.TIM_OutputNState=TIM_OutputNState_Enable;
	TIM_OCInitStruct.TIM_OutputState=TIM_OutputNState_Enable;
	TIM_OCInitStruct.TIM_Pulse=0;
	
	TIM_OC1Init(TIM1,&TIM_OCInitStruct);
	
	TIM_CtrlPWMOutputs(TIM1,ENABLE);
	
	
}

	