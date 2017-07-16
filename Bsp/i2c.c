#include "i2c.h"

u8 TEST = 1;
unsigned long TimeOut_Time;
unsigned char I2C_Err;
void iic_Init(void){
    GPIO_InitTypeDef  GPIO_InitStructure;
    I2C_InitTypeDef I2C_InitStructure;
    RCC_ClocksTypeDef   rcc_clocks;

    /* GPIO Peripheral clock enable */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA|RCC_AHB1Periph_GPIOB|RCC_AHB1Periph_GPIOC, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);
      /* Reset I2Cx IP */
    RCC_APB1PeriphResetCmd(RCC_APB1Periph_I2C1, ENABLE);
    /* Release reset signal of I2Cx IP */
    RCC_APB1PeriphResetCmd(RCC_APB1Periph_I2C1, DISABLE);

    /*I2C1 configuration*/
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_I2C1); //注意，此处不能合并写成GPIO_PinSource6|GPIO_PinSource7
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_I2C1);

    //PB6: I2C1_SCL  PB7: I2C1_SDA
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    /* I2C Struct Initialize */
    I2C_DeInit(I2C1);
    I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
    I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
    I2C_InitStructure.I2C_OwnAddress1 = 0x00;
    I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
    I2C_InitStructure.I2C_ClockSpeed = 100000;
    I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
    I2C_Init(I2C1, &I2C_InitStructure);

    /* I2C Initialize */
    I2C_Cmd(I2C1, ENABLE); 
    /*超时设置*/
    RCC_GetClocksFreq(&rcc_clocks);
    TimeOut_Time = (rcc_clocks.SYSCLK_Frequency /10000); 
}

void iic_Test(void){
//	pinWrite(&SDA,LOW);
//	delay(10);
//	pinWrite(&SDA,HIGH);
//	delay(10);
}
	
u8 iic_Start(I2C_TypeDef *I2Cx){
//	pinWrite(&SDA,HIGH);
//	pinWrite(&SCL,HIGH);
//	Delay();
//	if(!pinRead(&SDA)) return 0;	//为低电平表示总线忙
//	pinWrite(&SDA,LOW);
//	Delay();
//	pinWrite(&SCL,LOW);
//	return 1;
	unsigned long tmr;
	tmr=TimeOut_Time;
	while((--tmr)&& I2C_GetFlagStatus(I2Cx,I2C_FLAG_BUSY));
	if(!tmr)
		 return 0xff;
	I2C_GenerateSTART(I2Cx,ENABLE);
	tmr=TimeOut_Time;
	while((--tmr)&&(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT)));
	if(!tmr)
		 return 0xff;
	else 
		return 0;
}

u8 iic_Stop(I2C_TypeDef *I2Cx){
//	pinWrite(&SCL,LOW);
//	Delay();
//	pinWrite(&SDA,LOW);
//	Delay();
//	pinWrite(&SCL,HIGH);
//	Delay();
//	pinWrite(&SDA,HIGH);
//	Delay();
//	pinWrite(&SCL,LOW);
	  unsigned long tmr;
	  I2C_GenerateSTOP(I2Cx, ENABLE);
    tmr = TimeOut_Time;
    while((--tmr)&&(!(I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_RECEIVED))));  /* EV7 */
    if(tmr==0) 
			return 0xff;
		else
			return 0;
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

void iic_WriteByte(I2C_TypeDef *I2Cx,u8 data){
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

u8 iic_ReadByte(I2C_TypeDef *I2Cx){
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

u8 seyByte(I2C_TypeDef *I2Cx,u8 deviceAddr,u8 registerAddr,u8 data){
//	iic_Start();
//	iic_WriteByte(deviceAddr);
//	iic_WaitAck();
//	iic_WriteByte(registerAddr);
//	iic_WaitAck();
//	iic_WriteByte(data);
//	iic_WaitAck();
//	iic_Stop();
//	return 1;
	  uint8_t readout;
    u32 tmr;
    tmr = TimeOut_Time;
    while((--tmr)&&I2C_GetFlagStatus(I2Cx, I2C_FLAG_BUSY));
    if(tmr==0) I2C_Err = 1;

    I2C_GenerateSTART(I2Cx, ENABLE);
    //发送I2C的START信号，接口自动从从设备编程主设备
    tmr = TimeOut_Time;
    while((--tmr)&&(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT)));
    if(tmr==0) I2C_Err = 1;

    I2C_Send7bitAddress(I2Cx,deviceAddr,I2C_Direction_Transmitter);
    tmr = TimeOut_Time;
    while((--tmr)&&(!I2C_CheckEvent(I2Cx,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)));
    if(tmr==0) I2C_Err = 1;

    I2C_SendData(I2Cx, registerAddr);
    tmr = TimeOut_Time;
    while((--tmr)&&(!I2C_CheckEvent(I2Cx,I2C_EVENT_MASTER_BYTE_TRANSMITTED)));
    if(tmr==0) I2C_Err = 1;
    I2C_SendData(I2Cx, data);
    tmr = TimeOut_Time;
    while((--tmr)&&(!I2C_CheckEvent(I2Cx,I2C_EVENT_MASTER_BYTE_TRANSMITTED)));
    if(tmr==0) I2C_Err = 1;
     I2C_GenerateSTOP(I2Cx, ENABLE);
}

u8 hearByte(I2C_TypeDef *I2Cx,u8 deviceAddr,u8 registerAddr){
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
	 uint8_t readout;
    u32 tmr;

    tmr = TimeOut_Time;
    while((--tmr)&&I2C_GetFlagStatus(I2Cx, I2C_FLAG_BUSY));
    if(tmr==0) 
		{
			I2C_Err = 1;
		  return 0xff;
		}
    I2C_GenerateSTART(I2Cx, ENABLE);
    //发送I2C的START信号，接口自动从从设备编程主设备
    tmr = TimeOut_Time;
    while((--tmr)&&(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT)));
    if(tmr==0) 
		{
			I2C_Err = 1;
		  return 0xff;
		}
			

    I2C_Send7bitAddress(I2Cx,deviceAddr,I2C_Direction_Transmitter);
    tmr = TimeOut_Time;
    while((--tmr)&&(!I2C_CheckEvent(I2Cx,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)));
    if(tmr==0) 
		{
			I2C_Err = 1;
		  return 0xff;
		}

    I2C_SendData(I2Cx, registerAddr);
    tmr = TimeOut_Time;
    while((--tmr)&&(!I2C_CheckEvent(I2Cx,I2C_EVENT_MASTER_BYTE_TRANSMITTED)));
    if(tmr==0) 
		{
			I2C_Err = 1;
		  return 0xff;
		}

    I2C_GenerateSTART(I2Cx, ENABLE);
    tmr = TimeOut_Time;
    while((--tmr)&&(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT)));
    if(tmr==0) 
		{
			I2C_Err = 1;
		  return 0xff;
		}

    I2C_Send7bitAddress(I2Cx, deviceAddr, I2C_Direction_Receiver);
    tmr = TimeOut_Time;
    while((--tmr)&&(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED)));
    if(tmr==0) 
		{
			I2C_Err = 1;
		  return 0xff;
		}

    I2C_AcknowledgeConfig(I2Cx, DISABLE);
    I2C_GenerateSTOP(I2Cx, ENABLE);
    tmr = TimeOut_Time;
    while((--tmr)&&(!(I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_RECEIVED))));  /* EV7 */
    if(tmr==0) 
		{
			I2C_Err = 1;
		  return 0xff;
		}

    readout = I2C_ReceiveData(I2Cx);

    I2C_AcknowledgeConfig(I2Cx, ENABLE);

    return readout;

}

void iic_burst_read(I2C_TypeDef *I2Cx,u8* array,u8 length,u8 deviceAddr,u8 regAddr)
{
   	 uint8_t i;
    u32 tmr;

    tmr = TimeOut_Time;
    while((--tmr)&&I2C_GetFlagStatus(I2Cx, I2C_FLAG_BUSY));
    if(tmr==0) 

			I2C_Err = 1;

    I2C_GenerateSTART(I2Cx, ENABLE);
    //发送I2C的START信号，接口自动从从设备编程主设备
    tmr = TimeOut_Time;
    while((--tmr)&&(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT)));
    if(tmr==0) 

			I2C_Err = 1;


    I2C_Send7bitAddress(I2Cx,deviceAddr,I2C_Direction_Transmitter);
    tmr = TimeOut_Time;
    while((--tmr)&&(!I2C_CheckEvent(I2Cx,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)));
    if(tmr==0) 
			I2C_Err = 1;


    I2C_SendData(I2Cx, regAddr);
    tmr = TimeOut_Time;
    while((--tmr)&&(!I2C_CheckEvent(I2Cx,I2C_EVENT_MASTER_BYTE_TRANSMITTED)));
    if(tmr==0) 
			I2C_Err = 1;
		
    I2C_GenerateSTART(I2Cx, ENABLE);
    tmr = TimeOut_Time;
    while((--tmr)&&(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT)));
			I2C_Err = 1;


    I2C_Send7bitAddress(I2Cx, deviceAddr, I2C_Direction_Receiver);
    tmr = TimeOut_Time;
    while((--tmr)&&(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED)));
    if(tmr==0) 
			I2C_Err = 1;
    I2C_AcknowledgeConfig(I2Cx, ENABLE);
		for(i=0;i<length-1;i++)
		{
			while((--tmr)&&(!(I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_RECEIVED))));		/* EV7 */
			if(tmr==0) 
				I2C_Err = 1;
			array[i]=I2C_ReceiveData(I2Cx);
	  } 
    I2C_AcknowledgeConfig(I2Cx, DISABLE);
    I2C_GenerateSTOP(I2Cx, ENABLE);
    tmr = TimeOut_Time;
    while((--tmr)&&(!(I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_RECEIVED))));  /* EV7 */
    if(tmr==0) 
			I2C_Err = 1;
    array[i]= I2C_ReceiveData(I2Cx);

    I2C_AcknowledgeConfig(I2Cx, ENABLE);

  


}

u8 getTest(void){
	return TEST;
}

void resetTest(void){
	TEST = 1;
}
