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

void shiftMode(uint8_t times)
{
	for(uint8_t i=0;i<times;i++){
		
		GPIO_ResetBits(GPIOB, GPIO_Pin_12);
        GPIO_ResetBits(GPIOA, GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5);
        Delay_ms(50);
		
		GPIO_SetBits(GPIOB, GPIO_Pin_12);
        GPIO_SetBits(GPIOA, GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5);
        Delay_ms(50);
	}
	
}

int main(void)
{
	KEY_Init();
	LED_Init();
	Buzzer_Init();
	
	uint8_t previous=Bit_SET,current=Bit_SET;
	int8_t pos=0;//要移位判负，所以用有符号整型
	
	uint8_t alarmMode=0;//是否为报警状态
	uint8_t alarmState=0;//报警状态下
	
	while(1)
	{

		previous=current;
		current=GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_11);
		if(previous!=current){
			if(current==Bit_SET){//按键松开变化一次
				alarmMode=!alarmMode;
				if(alarmMode){
					alarmState=1;
				}else{
					GPIO_SetBits(GPIOB,GPIO_Pin_12);
					GPIO_SetBits(GPIOA, GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5);
				}
								
			}
			Delay_ms(50);//消抖
		}
		
		if(alarmMode){
            // 报警模式：所有 LED 同步高频闪烁，蜂鸣器与灯光同步间歇鸣响
            if(alarmState)
            {
                GPIO_ResetBits(GPIOA,GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5);      // LED 全亮
                GPIO_ResetBits(GPIOB, GPIO_Pin_12);  // 蜂鸣器响
            }
            else
            {
                GPIO_SetBits(GPIOA, GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5);        // LED 全灭
                GPIO_SetBits(GPIOB, GPIO_Pin_12);    // 蜂鸣器停
            }

            alarmState = !alarmState;
            Delay_ms(50); // 高频闪烁周期：50ms 亮/响，50ms 灭/停
        }else{
			if(pos<=5){
				GPIO_Write(GPIOA, ~(1<<pos));//低电平点亮所以按位取反
			}else{
				pos=0;
				GPIO_Write(GPIOA, ~(1<<pos));
			}
			pos++;

			Delay_ms(100);
		}
	}
}


	