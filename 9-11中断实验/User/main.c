#include "stm32f10x.h"                  // Device header
#include "Delay.h"

void App_OnBoardLED_Init(void);
void App_Button_Init(void);

int main(void)
{
	App_OnBoardLED_Init();
	App_Button_Init();
	
	 while(1)
    {
    }
	
}

void App_OnBoardLED_Init(){
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_13;
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_Out_OD;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOC,&GPIO_InitStruct);
	
	GPIO_WriteBit(GPIOC,GPIO_Pin_13,Bit_SET);
	
}
void App_Button_Init(void){
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);

	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_5|GPIO_Pin_6;
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_IPU;

	GPIO_Init(GPIOA,&GPIO_InitStruct);
	
	//为EXTI的线分配引脚并配置参数
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource5);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource6);
	
	EXTI_InitTypeDef EXTI_InitStruct;
	EXTI_InitStruct.EXTI_Mode=EXTI_Mode_Interrupt;
	EXTI_InitStruct.EXTI_Trigger=EXTI_Trigger_Rising;//松开按键
	EXTI_InitStruct.EXTI_Line=EXTI_Line5|EXTI_Line6;
	EXTI_InitStruct.EXTI_LineCmd=ENABLE;
	
	EXTI_Init(&EXTI_InitStruct);
	
	//配置中断
	NVIC_InitTypeDef NVIC_InitStruct;
	NVIC_InitStruct.NVIC_IRQChannel=EXTI9_5_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=0;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority=0;
	
	NVIC_Init(&NVIC_InitStruct);

}


//中断响应函数
void EXTI9_5_IRQHandler(void){
	//注意清除标志位，避免一直处于中断状态
	if(EXTI_GetFlagStatus(EXTI_Line5)==SET){
		EXTI_ClearFlag(EXTI_Line5);
		GPIO_WriteBit(GPIOC,GPIO_Pin_13,Bit_RESET);
	}
	
	if(EXTI_GetFlagStatus(EXTI_Line6)==SET){
		EXTI_ClearFlag(EXTI_Line6);
		GPIO_WriteBit(GPIOC,GPIO_Pin_13,Bit_SET);
	}
	
}

