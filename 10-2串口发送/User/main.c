#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "Serial.h"

int main(void)
{
	OLED_Init();
	
	Serial_Init();

	//Serial_SendByte(0x41);
	
//	uint8_t MyArray[]={0x42,0x43,0x44,0x45};
//	Serial_SendArray(MyArray,4);
	
//	Serial_SendString("Hello World!\r\n");
	
//	Serial_SendNumber(12345,5);
	
//	printf("Num=%d\r\n",666);
//注意要先勾选microLIB
	
	char String[100];
	sprintf(String,"注意！");
	Serial_SendString(String);

	
	while (1)
	{
		
	}
}
