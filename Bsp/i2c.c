#include "i2c.h"
u8 TEST = 1;

void iic_Init(void){
  
	
}

void iic_Test(void){
//	pinWrite(&SDA,LOW);
//	delay(10);
//	pinWrite(&SDA,HIGH);
//	delay(10);
}

void Delay(void){
	//delay10us(1);
	 u8 i=30; //这里可以优化速度        ，经测试最低到5还能写入
   while(i) 
   { 
     i--; 
   }  
}

u8 iic_Start(void){
//	pinWrite(&SDA,HIGH);
//	pinWrite(&SCL,HIGH);
//	Delay();
//	if(!pinRead(&SDA)) return 0;	//为低电平表示总线忙
//	pinWrite(&SDA,LOW);
//	Delay();
//	pinWrite(&SCL,LOW);
//	return 1;
}

void iic_Stop(void){
//	pinWrite(&SCL,LOW);
//	Delay();
//	pinWrite(&SDA,LOW);
//	Delay();
//	pinWrite(&SCL,HIGH);
//	Delay();
//	pinWrite(&SDA,HIGH);
//	Delay();
//	pinWrite(&SCL,LOW);
}

void iic_Ack(void){
//	pinWrite(&SCL,LOW);
//	Delay();
//	pinWrite(&SDA,LOW);
//	Delay();
//	pinWrite(&SCL,HIGH);
//	Delay();
//	pinWrite(&SCL,LOW);
//	Delay();
}

void iic_NoAck(void){
//	pinWrite(&SCL,LOW);
//	Delay();
//	pinWrite(&SDA,HIGH);
//	Delay();
//	pinWrite(&SCL,HIGH);
//	Delay();
//	pinWrite(&SCL,LOW);
//	Delay();
}

u8 iic_WaitAck(void){
//	pinWrite(&SCL,LOW);
//	Delay();
//	pinWrite(&SDA,HIGH);
//	Delay();
//	pinWrite(&SCL,HIGH);
//	Delay();
//	if(pinRead(&SDA)){
//		pinWrite(&SCL,LOW);
//		Delay();
//		return 0;
//	}
//	pinWrite(&SCL,LOW);
//	Delay();
//	return 1;
}

void iic_WriteByte(u8 data){
//	int i;
//	for(i = 0;i<8;i++){	
//		pinWrite(&SCL,LOW);
//		Delay();
//		if(data & 0x80)
//			pinWrite(&SDA,HIGH);
//		else
//			pinWrite(&SDA,LOW);
//		data <<= 1;
//		Delay();
//		pinWrite(&SCL,HIGH);
//		Delay();
//	}
//	pinWrite(&SCL,LOW);
}

u8 iic_ReadByte(void){
//	u8 i;
//	u8 data = 0;
//	
//	pinWrite(&SDA,HIGH);
//	for(i = 0;i<8;i++){
//		data <<= 1;
//		pinWrite(&SCL,LOW);
//		Delay();
//		pinWrite(&SCL,HIGH);
//		Delay();
//		if(pinRead(&SDA))
//			data |= 0x01;
//	}	
//	pinWrite(&SCL,LOW);
//	return data;
}

u8 seyByte(u8 deviceAddr,u8 registerAddr,u8 data){
//	iic_Start();
//	iic_WriteByte(deviceAddr);
//	iic_WaitAck();
//	iic_WriteByte(registerAddr);
//	iic_WaitAck();
//	iic_WriteByte(data);
//	iic_WaitAck();
//	iic_Stop();
//	return 1;
}

u8 hearByte(u8 deviceAddr,u8 registerAddr){
//	u8 data;
//	iic_Start();
//	iic_WriteByte(deviceAddr);
//	iic_WaitAck();
//	iic_WriteByte(registerAddr);
//	iic_WaitAck();
//	
//	iic_Start();
//	iic_WriteByte(deviceAddr + 1);
//	iic_WaitAck();
//	data = iic_ReadByte();
//	iic_NoAck();
//	iic_Stop();
//	return data;
}

u8 getTest(void){
	return TEST;
}

void resetTest(void){
	TEST = 1;
}
