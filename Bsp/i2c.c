#include "i2c.h"

u8 TEST = 1;
unsigned long TimeOut_Time;
unsigned char I2C_Err;

void Delay(void)
{
	Sys_tick=30;
	SysTick->CTRL|= SysTick_CTRL_TICKINT_Msk|SysTick_CTRL_ENABLE_Msk;   
  while(Sys_tick);
	Sys_tick=0;
//	uint8_t i;
//	for (i = 0; i < 30; i++);
}
void iic_Init(void){
    GPIO_InitTypeDef  GPIO_InitStructure;
    I2C_InitTypeDef I2C_InitStructure;
    RCC_ClocksTypeDef   rcc_clocks;

    /* GPIO Peripheral clock enable */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
   // RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);
//      /* Reset I2Cx IP */
   //RCC_APB1PeriphResetCmd(RCC_APB1Periph_I2C1, ENABLE);
//    /* Release reset signal of I2Cx IP */
    //RCC_APB1PeriphResetCmd(RCC_APB1Periph_I2C1, DISABLE);

//    /*I2C1 configuration*/
//   GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_I2C1); //注意，此处不能合并写成GPIO_PinSource6|GPIO_PinSource7
//  	GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_I2C1);

    //PB6: I2C1_SCL  PB7: I2C1_SDA
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;
	  GPIO_InitStructure.GPIO_Mode=GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    /* I2C Struct Initialize */
//    I2C_DeInit(I2C1);
//    I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
//    I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
//    I2C_InitStructure.I2C_OwnAddress1 = 0x00;
//    I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
//    I2C_InitStructure.I2C_ClockSpeed = 100000;
//    I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
//    I2C_Init(I2C1, &I2C_InitStructure);

    /* I2C Initialize */
   // I2C_Cmd(I2C1, ENABLE); 
    /*超时设置*/
	//	I2C_ClearFlag(I2C1,I2C_FLAG_BUSY);
    RCC_GetClocksFreq(&rcc_clocks);
    TimeOut_Time = (rcc_clocks.SYSCLK_Frequency /10000); 
}

//void iic_Test(void){
//	SDA_L;
//	delay(10);
//	SDA_H;
//	delay(10);
//}
//	
u8 iic_Start(){
	SDA_H;
	SCL_L;
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
	SCL_L;
	Delay();
	SDA_H;
	Delay();
	SCL_L;

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
//	  uint8_t readout;
//    u32 tmr;
//    tmr = TimeOut_Time;
//    while((--tmr)&&I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY));
//    if(tmr==0) I2C_Err = 1;

//    I2C_GenerateSTART(I2C1, ENABLE);
//    //发送I2C的START信号，接口自动从从设备编程主设备
//    tmr = TimeOut_Time;
//    while((--tmr)&&(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT)));
//    if(tmr==0) I2C_Err = 1;

//    I2C_Send7bitAddress(I2C1,deviceAddr,I2C_Direction_Transmitter);
//    tmr = TimeOut_Time;
//    while((--tmr)&&(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)));
//    if(tmr==0) I2C_Err = 1;

//    I2C_SendData(I2C1, registerAddr);
//    tmr = TimeOut_Time;
//    while((--tmr)&&(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_TRANSMITTED)));
//    if(tmr==0) I2C_Err = 1;
//    I2C_SendData(I2C1, data);
//    tmr = TimeOut_Time;
//    while((--tmr)&&(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_TRANSMITTED)));
//    if(tmr==0) I2C_Err = 1;
//     I2C_GenerateSTOP(I2C1, ENABLE);
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
	
//   	 uint8_t i;
//    u32 tmr;
//    u8 readout;
//    tmr = TimeOut_Time;
//    while((--tmr)&&I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY));
//    if(tmr==0) 
//			I2C_Err = 1;

//    I2C_GenerateSTART(I2C1, ENABLE);
//    //发送I2C的START信号，接口自动从从设备编程主设备
//    tmr = TimeOut_Time;
//    while((--tmr)&&(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT)));
//    if(tmr==0) 
//			I2C_Err = 1;


//    I2C_Send7bitAddress(I2C1,deviceAddr,I2C_Direction_Transmitter);
//    tmr = TimeOut_Time;
//    while((--tmr)&&(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)));
//    if(tmr==0) 
//			I2C_Err = 1;


//    I2C_SendData(I2C1, registerAddr);
//    tmr = TimeOut_Time;
//    while((--tmr)&&(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_TRANSMITTED)));
//    if(tmr==0) 
//			I2C_Err = 1;
//		
//    I2C_GenerateSTART(I2C1, ENABLE);
//    tmr = TimeOut_Time;
//    while((--tmr)&&(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT)));
//			I2C_Err = 1;
//    I2C_Send7bitAddress(I2C1, deviceAddr, I2C_Direction_Receiver);
//    tmr = TimeOut_Time;
//    while((--tmr)&&(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED)));
//    if(tmr==0) 
//			I2C_Err = 1;
//		
//    I2C_AcknowledgeConfig(I2C1, DISABLE);
//    I2C_GenerateSTOP(I2C1, ENABLE);
//    tmr = TimeOut_Time;
//    while((--tmr)&&(!(I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED))));  /* EV7 */
//    if(tmr==0) 
//			I2C_Err = 1;
//    readout= I2C_ReceiveData(I2C1);

//    I2C_AcknowledgeConfig(I2C1, ENABLE);
//		return readout;
}

void iic_burst_read(u8* array,u8 length,u8 deviceAddr,u8 regAddr)
{
   	 uint8_t i;
    u32 tmr;

    tmr = TimeOut_Time;
    while((--tmr)&&I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY));
    if(tmr==0) 

			I2C_Err = 1;

    I2C_GenerateSTART(I2C1, ENABLE);
    //发送I2C的START信号，接口自动从从设备编程主设备
    tmr = TimeOut_Time;
    while((--tmr)&&(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT)));
    if(tmr==0) 

			I2C_Err = 1;


    I2C_Send7bitAddress(I2C1,deviceAddr,I2C_Direction_Transmitter);
    tmr = TimeOut_Time;
    while((--tmr)&&(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)));
    if(tmr==0) 
			I2C_Err = 1;


    I2C_SendData(I2C1, regAddr);
    tmr = TimeOut_Time;
    while((--tmr)&&(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_TRANSMITTED)));
    if(tmr==0) 
			I2C_Err = 1;
		
    I2C_GenerateSTART(I2C1, ENABLE);
    tmr = TimeOut_Time;
    while((--tmr)&&(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT)));
			I2C_Err = 1;


    I2C_Send7bitAddress(I2C1, deviceAddr, I2C_Direction_Receiver);
    tmr = TimeOut_Time;
    while((--tmr)&&(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED)));
    if(tmr==0) 
			I2C_Err = 1;
    I2C_AcknowledgeConfig(I2C1, ENABLE);
		for(i=0;i<length-1;i++)
		{
			while((--tmr)&&(!(I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED))));		/* EV7 */
			if(tmr==0) 
				I2C_Err = 1;
			array[i]=I2C_ReceiveData(I2C1);
	  } 
    I2C_AcknowledgeConfig(I2C1, DISABLE);
    I2C_GenerateSTOP(I2C1, ENABLE);
    tmr = TimeOut_Time;
    while((--tmr)&&(!(I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED))));  /* EV7 */
    if(tmr==0) 
			I2C_Err = 1;
    array[i]= I2C_ReceiveData(I2C1);

    I2C_AcknowledgeConfig(I2C1, ENABLE);
}
