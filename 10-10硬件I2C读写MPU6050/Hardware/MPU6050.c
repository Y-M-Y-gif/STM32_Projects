#include "stm32f10x.h"    // Device header
#include "MPU6050_Reg.h"

#define MPU6050_ADDRESS 0XD0

void MPU6050_WriteReg(uint8_t RegAddress,uint8_t Data)
{
	I2C_GenerateSTART(I2C2,ENABLE);
	while(I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_MODE_SELECT)!=SUCCESS);
	//检查EV5事件
	
	I2C_Send7bitAddress(I2C2,MPU6050_ADDRESS,I2C_Direction_Transmitter);
	while(I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)!=SUCCESS);
	//检查EV6事件
	
	I2C_SendData(I2C2,RegAddress);
	while(I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_BYTE_TRANSMITTING)!=SUCCESS);
	//等待EV8事件
	
	I2C_SendData(I2C2,Data);
	while(I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_BYTE_TRANSMITTED)!=SUCCESS);
	
	
	I2C_GenerateSTOP(I2C2,ENABLE);
}

uint8_t MPU6050_ReadReg(uint8_t RegAddress)
{
	uint8_t Data;
	
	I2C_GenerateSTART(I2C2,ENABLE);
	while(I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_MODE_SELECT)!=SUCCESS);
	//检查EV5事件
	
	I2C_Send7bitAddress(I2C2,MPU6050_ADDRESS,I2C_Direction_Transmitter);
	while(I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)!=SUCCESS);
	//检查EV6事件
	
	I2C_SendData(I2C2,RegAddress);
	while(I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_BYTE_TRANSMITTED)!=SUCCESS);
	//等待EV8事件
	
	I2C_GenerateSTART(I2C2,ENABLE);
	while(I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_MODE_SELECT)!=SUCCESS);
	//检查EV5事件
	
	I2C_Send7bitAddress(I2C2,MPU6050_ADDRESS,I2C_Direction_Receiver);
	while(I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED)!=SUCCESS);
	//检查EV6事件
	
	//后期扩展在这里加循环以及判断
	I2C_AcknowledgeConfig(I2C2,DISABLE);
	//赶在接收最后一个字节前发送非应答
	I2C_GenerateSTOP(I2C2,ENABLE);
	
	while(I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_BYTE_RECEIVED)!=SUCCESS);
	Data=I2C_ReceiveData(I2C2);
	
	I2C_AcknowledgeConfig(I2C2,ENABLE);
	
	return Data;
}


void MPU6050_Init(void)
{
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_AF_OD;
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_10|GPIO_Pin_11;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStruct);
	
	
	I2C_InitTypeDef I2C_InitStruct;
	I2C_InitStruct.I2C_Mode=I2C_Mode_I2C;
	I2C_InitStruct.I2C_Ack=I2C_Ack_Enable;
	I2C_InitStruct.I2C_AcknowledgedAddress=I2C_AcknowledgedAddress_7bit;
	I2C_InitStruct.I2C_ClockSpeed=50000;
	//最大加到400kHz
	I2C_InitStruct.I2C_DutyCycle=I2C_DutyCycle_2;
	//仅快速模式（>100k）下有效，低电平/高电平
	I2C_InitStruct.I2C_OwnAddress1=0x00;
	//一般STM32不作从机，这里就随便给了
	I2C_Init(I2C2,&I2C_InitStruct);
	
	I2C_Cmd(I2C2,ENABLE);
	
	MPU6050_WriteReg(MPU6050_PWR_MGMT_1,0X01);
	//电源Power寄存器1Management 管理
	/*
		bit7  DEVICE_RESET     0  不执行芯片硬件复位
		bit6  SLEEP             0  退出休眠！bit6=1 休眠；bit6=0 芯片唤醒，传感器上电工作
		bit5  CYCLE             0  不进入循环低功耗采样
		bit4  保留位            0  保留位
		bit3  TEMP_DIS          0  温度传感器开启
		bit2~0 CLKSEL [2:0]    001  时钟源选择：使用陀螺仪 X 轴的内部振荡器作为系统时钟（推荐）
	*/
	MPU6050_WriteReg(MPU6050_PWR_MGMT_2,0X00);
	//电源Power寄存器2Management 管理
	/*
		bit7  LP_WAKE_CTRL1    0  低功耗唤醒频率控制位 1，普通模式不用
		bit6  LP_WAKE_CTRL0    0  低功耗唤醒频率控制位 0，普通模式不用
		bit5  STBY_XA          0  加速度计 X 轴，0 = 正常工作；1 = 待机关闭
		bit4  STBY_YA          0  加速度计 Y 轴，0 = 正常工作；1 = 待机关闭
		bit3  STBY_ZA          0  加速度计 Z 轴，0 = 正常工作；1 = 待机关闭
		bit2  STBY_XG          0  陀螺仪 X 轴，0 = 正常工作；1 = 待机关闭
		bit1  STBY_YG          0  陀螺仪 Y 轴，0 = 正常工作；1 = 待机关闭
		bit0  STBY_ZG          0  陀螺仪 Z 轴，0 = 正常工作；1 = 待机关闭
	*/
	MPU6050_WriteReg(MPU6050_SMPLRT_DIV,0X09);
	//配置Sample Rate Divider 采样率分频寄存器
	MPU6050_WriteReg(MPU6050_CONFIG,0X06);
	//配置配置寄存器
	/*
		bit7  -           0  保留位
		bit6  -           0  保留位
		bit5~3 EXT_SYNC_SET[2:0] 000  外部同步External Synchronization关闭，不用外部触发
		bit2~0 DLPF_CFG[2:0]     110  数字低通滤波Digital Low-Pass Filter，带宽44Hz，滤波效果强
	*/
	MPU6050_WriteReg(MPU6050_GYRO_CONFIG,0X18);
	//Gyroscope Config 陀螺仪配置寄存器
	/*
		bit7  XG_ST       0  X轴陀螺仪自检关闭
		bit6  YG_ST       0  Y轴陀螺仪自检关闭
		bit5  ZG_ST       0  Z轴陀螺仪自检关闭
		bit4~3 FS_SEL[1:0] 11 陀螺仪量程 ±2000 °/s
		bit2~0 保留位     000  保留
	*/
	MPU6050_WriteReg(MPU6050_ACCEL_CONFIG,0X18);
	//Accelerometer Config 加速度计配置寄存器
	/*
		bit7  XA_ST       0  X轴加速度自检关闭
		bit6  YA_ST       0  Y轴加速度自检关闭
		bit5  ZA_ST       0  Z轴加速度自检关闭
		bit4~3 AFS_SEL[1:0] 11 加速度计量程 ±16g
		bit2~0 保留位     000  保留

	*/

}

void MPU6050_GetData(int16_t* AccX,int16_t* AccY,int16_t* AccZ,int16_t* GyroX,int16_t* GyroY,int16_t* GyroZ )
{
	uint8_t DataH,DataL;
	
	DataH=MPU6050_ReadReg(MPU6050_ACCEL_XOUT_H);
	DataL=MPU6050_ReadReg(MPU6050_ACCEL_XOUT_L);
	*AccX=(DataH<<8)| DataL;
	//小类型做运算，先升级到 int；运算结果再赋值给左边变量时，才按左边变量宽度截断
		
	DataH=MPU6050_ReadReg(MPU6050_ACCEL_YOUT_H);
	DataL=MPU6050_ReadReg(MPU6050_ACCEL_YOUT_L);
	*AccY=(DataH<<8)| DataL;
	
	DataH=MPU6050_ReadReg(MPU6050_ACCEL_ZOUT_H);
	DataL=MPU6050_ReadReg(MPU6050_ACCEL_ZOUT_L);
	*AccZ=(DataH<<8)| DataL;
	
	DataH=MPU6050_ReadReg(MPU6050_GYRO_XOUT_H);
	DataL=MPU6050_ReadReg(MPU6050_GYRO_XOUT_L);
	*GyroX=(DataH<<8)| DataL;      
	
	DataH=MPU6050_ReadReg(MPU6050_GYRO_YOUT_H);
	DataL=MPU6050_ReadReg(MPU6050_GYRO_YOUT_L);
	*GyroY=(DataH<<8)| DataL;     
	
	DataH=MPU6050_ReadReg(MPU6050_GYRO_ZOUT_H);
	DataL=MPU6050_ReadReg(MPU6050_GYRO_ZOUT_L);
	*GyroZ=(DataH<<8)| DataL;
	
}

uint8_t MPU6050_GetID(void)
{
	return MPU6050_ReadReg(MPU6050_WHO_AM_I);
}

