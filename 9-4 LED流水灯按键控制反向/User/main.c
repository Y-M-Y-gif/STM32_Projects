#include "stm32f10x.h"                  // Device header
#include "Delay.h" 

//按键对应引脚PB11初始化
void KEY_Init(void)
{
    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

    GPIO_InitTypeDef GPIO_InitStructure;
	
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	
    GPIO_Init(GPIOB, &GPIO_InitStructure);
}

//流水灯初始化
void LED_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
}

//蜂鸣器初始化
void Buzzer_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	GPIO_SetBits(GPIOB,GPIO_Pin_12);
}

int main(void)
{
	KEY_Init();
	LED_Init();
	Buzzer_Init();
	
	uint8_t previous=Bit_SET,current=Bit_SET;
	uint8_t dir_flag = 0; 
	
	while(1)
	{

		previous=current;
		current=GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_11);
		if(previous!=current){
			if(current==Bit_SET){//按键松开
				dir_flag=1;
			}
		}
		
		
}

	