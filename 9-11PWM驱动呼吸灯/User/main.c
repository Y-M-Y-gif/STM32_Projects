#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "Timer.h"
#include "PWM.h"

uint8_t i;
uint16_t Num;//Timer.c用到了，别删

int main(void)
{
	OLED_Init();
	PWM_Init();
	
	 while(1)
    {
		for(i=0;i<=100;i++){
			PWM_SetCompare1(i);
			Delay_ms(10);
		}
		for(i=0;i<=100;i++){
			PWM_SetCompare1(100-i);
			Delay_ms(10);
		}
    }
	
}


