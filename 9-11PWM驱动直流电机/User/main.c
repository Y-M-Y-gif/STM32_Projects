#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "PWM.h"
#include "Servo.h"
#include "Key.h"
#include "Motor.h"

uint16_t Num;//Timer.c用到了，勿删！
uint8_t i;

int main(void)
{
	OLED_Init();
	PWM_Init();
	Key_Init();
	Motor_Init();
	
	Motor_SetSpeed(50);
	
	 while(1)
    {
		
    }
	
}


