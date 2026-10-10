#include "stm32f10x.h"    // Device header
#include "myI2C.h"
#include "MPU6050_Reg.h"

#define MPU6050_ADDRESS 0XD0

void MPU6050_WriteReg(uint8_t RegAddress,uint8_t Data)
{
	MyI2C_Start();
	
	MyI2C_SendByte(MPU6050_ADDRESS);
	MyI2C_ReceiveAck();
	MyI2C_SendByte(RegAddress);
	MyI2C_ReceiveAck();
	
	MyI2C_SendByte(Data);
	MyI2C_ReceiveAck();
	
	MyI2C_Stop();

}

uint8_t MPU6050_ReadReg(uint8_t RegAddress)
{
	uint8_t Data;
	
	MyI2C_Start();
	
	MyI2C_SendByte(MPU6050_ADDRESS);
	MyI2C_ReceiveAck();
	MyI2C_SendByte(RegAddress);
	MyI2C_ReceiveAck();
	
	MyI2C_Start();
	MyI2C_SendByte(MPU6050_ADDRESS | 0x01);
	MyI2C_ReceiveAck();
	
	Data=MyI2C_ReceiveByte();
	MyI2C_SendAck(1);
	
	MyI2C_Stop();

	return Data;
}


void MPU6050_Init(void)
{
	MyI2C_Init();
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

