#include "stm32f10x.h"                  // Device header

uint16_t myDMA_size;

void myDMA_Init(uint32_t AddrA,uint32_t AddrB,uint16_t size)
{
	myDMA_size=size;
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);
	
	DMA_InitTypeDef DMA_InitStruct;
	DMA_InitStruct.DMA_MemoryBaseAddr=AddrA;
	DMA_InitStruct.DMA_MemoryDataSize=DMA_PeripheralDataSize_Byte;
	DMA_InitStruct.DMA_MemoryInc=DMA_MemoryInc_Enable;
	DMA_InitStruct.DMA_PeripheralBaseAddr=AddrB;
	DMA_InitStruct.DMA_PeripheralDataSize=DMA_PeripheralDataSize_Byte;
	DMA_InitStruct.DMA_PeripheralInc=DMA_PeripheralInc_Enable;
	DMA_InitStruct.DMA_BufferSize=size;//指定传输计数器的值
	DMA_InitStruct.DMA_DIR=DMA_DIR_PeripheralDST;//外设站点作为源
	DMA_InitStruct.DMA_M2M=DMA_M2M_Enable;//实际上为是否软件触发，对应是否存->存
	DMA_InitStruct.DMA_Mode=DMA_Mode_Normal;//配置自动重装寄存器
	DMA_InitStruct.DMA_Priority=DMA_Priority_Medium;
	DMA_Init(DMA1_Channel1,&DMA_InitStruct);
	
	DMA_Cmd(DMA1_Channel1,DISABLE);
}

void myDMA_Transfer(void)
{
	DMA_Cmd(DMA1_Channel1,DISABLE);//先失能才能重新赋值
	DMA_SetCurrDataCounter(DMA1_Channel1,myDMA_size);
	DMA_Cmd(DMA1_Channel1,ENABLE);
	while(DMA_GetFlagStatus(DMA1_FLAG_TC1)==RESET);
	DMA_ClearFlag(DMA1_FLAG_TC1);
	

}
