#include "i2c.h"

u8 TEST = 1;
unsigned long TimeOut_Time;
unsigned char I2C_Err;

void Delay(void)
{
//	Sys_tick=30;
//	SysTick->CTRL|= SysTick_CTRL_TICKINT_Msk|SysTick_CTRL_ENABLE_Msk;   
//  while(Sys_tick);
//	Sys_tick=0;
    double start=Microsec();
	  while((Microsec()-start)<2);
}
void iic_Init(void){
   GPIO_InitTypeDef  GPIO_InitStructure;
    /* GPIO Peripheral clock enable */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7|GPIO_Pin_6;
	  GPIO_InitStructure.GPIO_Mode=GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
}


u8 iic_Start(){
	SDA_H;
	SCL_H;
	Delay();
	if(!SDA_READ) return 0;	//为低电平表示总线忙
	SDA_L;
	Delay();
	SCL_L;
	return 1;

}

u8 iic_Stop(){
	SCL_L;
	Delay();
	SDA_L;
	Delay();
	SCL_H;
	Delay();
	SDA_H;
	Delay();
	SCL_L;
  return 0;
}

void iic_Ack(void){
	SCL_L;
	Delay();
	SDA_L;
	Delay();
	SCL_H;
	Delay();
	SCL_L;
	Delay();
}

void iic_NoAck(void){
	SCL_L;
	Delay();
	SDA_H;
	Delay();
	SCL_H;
	Delay();
	SCL_L;
	Delay();
}

u8 iic_WaitAck(void){
	SCL_L;
	Delay();
	SDA_H;
	Delay();
	SCL_H;
	Delay();
	if(SDA_READ){
		SCL_L;
		Delay();
		return 0;
	}
	SCL_L;
	Delay();
	return 1;
}

void iic_WriteByte(u8 data){
	int i;
	for(i = 0;i<8;i++){	
		SCL_L;
		Delay();
		if(data & 0x80)
			SDA_H;
		else
			SDA_L;
		data <<= 1;
		Delay();
		SCL_H;
		Delay();
	}
	SCL_L;
}

u8 iic_ReadByte(){
	u8 i;
	u8 data = 0;
	
	 SDA_H;
	for(i = 0;i<8;i++){
		data <<= 1;
		SCL_L;
		Delay();
		SCL_H;
		Delay();
		if(SDA_READ)
			data |= 0x01;
	}	
	SCL_L;
	return data;
}

u8 setByte(u8 deviceAddr,u8 registerAddr,u8 data){
	iic_Start();
	iic_WriteByte(deviceAddr);
	iic_WaitAck();
	iic_WriteByte(registerAddr);
	iic_WaitAck();
	iic_WriteByte(data);
	iic_WaitAck();
	iic_Stop();
	return 1;
}

u8 iic_Command(u8 deviceAddr,u8 cmd)
{
	iic_Start();
	iic_WriteByte(deviceAddr);
	iic_WaitAck();
	iic_WriteByte(cmd);
	iic_WaitAck();
	iic_Stop();
}
u8 hearByte(u8 deviceAddr,u8 registerAddr){
	u8 data;
	
	iic_Start();
	iic_WriteByte(deviceAddr);
	iic_WaitAck();
	iic_WriteByte(registerAddr);
	iic_WaitAck();
	
	iic_Start();
	iic_WriteByte(deviceAddr + 1);
	iic_WaitAck();
	data = iic_ReadByte();
	iic_NoAck();
	iic_Stop();
	return data;
}

void iic_burst_read(u8* array,u8 length,u8 deviceAddr,u8 regAddr,u8 direct)
{
   	 uint8_t i;
     iic_Start();
	   iic_WriteByte(deviceAddr);
	   iic_WaitAck();
	   iic_WriteByte(regAddr);
	   iic_WaitAck();
	   iic_Start();
	   iic_WriteByte(deviceAddr+1);
	   iic_WaitAck();
	   if(DIRECT_LSB == direct)
		 {
			 for(i=0;i<length-1;i++)
			 {
				 array[i]=iic_ReadByte();
				 iic_Ack();			 
			 }
	   }
		 else
		 {
		   for(i=length-1;i>0;i--)
			 {
				 array[i]=iic_ReadByte();
				 iic_Ack();		
			 }
		 }
		 array[i]=iic_ReadByte();
		 iic_NoAck();
		 iic_Stop();
}
