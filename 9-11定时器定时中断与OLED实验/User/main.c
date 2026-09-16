#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "Timer.h"

uint16_t Num;

int main(void)
{
	OLED_Init();
	Timer_Init();
	
	//显示4行16列
	
//	OLED_ShowChar(1,1,'A');
//	
//	OLED_ShowString(1,3,"Hello World!");
//	
//	OLED_ShowNum(2,1,12345,6);//会在高位补0
//	OLED_ShowNum(2,7,12345,4);//会截去高位
//	OLED_ShowSignedNum(3,1,12345,5);
//  //OLED_ShowHexNum与OLED_ShowBinNum同理，注意输入参数都要十六进制
	
//	OLED_Clear();

	OLED_ShowString(1,1,"Num:");
	 while(1)
    {
		OLED_ShowNum(1,5,Num,5);
    }
	
}


