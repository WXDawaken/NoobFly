#include "mpu.h"
int Tref=0;
u16 C[7]={0};
long TEMP,Pressure;
void mpu9150_Init(void){
	setByte(MPU6050_Addr,PWR_MGMT_1,0x00);
	setByte(MPU6050_Addr,SMPLRT_DIV,0x07);
	setByte(MPU6050_Addr,CONFIG,0x06);
	setByte(MPU6050_Addr,GYRO_CONFIG,0x18);
	setByte(MPU6050_Addr,ACCEL_CONFIG,0x01);
	setByte(MPU6050_Addr,I2C_MASTER_CFG,I2C_MASTER_DIS);
	setByte(MPU6050_Addr,INT_PIN_CFG,I2C_BYPASS_SET);
	MS5611_Init();
}

void mpu9150_Read(u8 *ACCEL,u8 *GYRO,u8 *MAG){
	u8 state;
	double start;
	*(ACCEL + 0) = hearByte(MPU6050_Addr,ACCEL_XOUT_H);
	*(ACCEL + 1) = hearByte(MPU6050_Addr,ACCEL_XOUT_L);
	*(ACCEL + 2) = hearByte(MPU6050_Addr,ACCEL_YOUT_H);
	*(ACCEL + 3) = hearByte(MPU6050_Addr,ACCEL_YOUT_L);
	*(ACCEL + 4) = hearByte(MPU6050_Addr,ACCEL_ZOUT_H);
	*(ACCEL + 5) = hearByte(MPU6050_Addr,ACCEL_ZOUT_L);
	
	*(GYRO + 0)  = hearByte(MPU6050_Addr,GYRO_XOUT_H);
	*(GYRO + 1)  = hearByte(MPU6050_Addr,GYRO_XOUT_L);
	*(GYRO + 2)  = hearByte(MPU6050_Addr,GYRO_YOUT_H);
	*(GYRO + 3)  = hearByte(MPU6050_Addr,GYRO_YOUT_L);
	*(GYRO + 4)  = hearByte(MPU6050_Addr,GYRO_ZOUT_H);
	*(GYRO + 5)  = hearByte(MPU6050_Addr,GYRO_ZOUT_L);

	setByte(AK8975_Addr,AK_CNTL,AK_MODE_SINGLE_MEASURE);
	while((state=hearByte(AK8975_Addr,AK_STATUS_1))!=AK_DATA_READY);
	*(MAG + 0)  = hearByte(AK8975_Addr,AK_HYH);
	*(MAG + 1)  = hearByte(AK8975_Addr,AK_HYL);
	*(MAG + 2)  = hearByte(AK8975_Addr,AK_HXH);
	*(MAG + 3)  = hearByte(AK8975_Addr,AK_HXL);
	*(MAG + 4)  = hearByte(AK8975_Addr,AK_HZH);
	*(MAG + 5)  = hearByte(AK8975_Addr,AK_HZL);
	// MS5611_Get_Pressure();
}

void mpu9150_RawData(float *ACCEL,float *GYRO,float *MAG)
{

	 int i;
	 short temp;
	 u8 ac[6],gy[6],mg[6];
	// printf("- - - - - - raw sensor data - - - - - -\n");
   mpu9150_Read(ac,gy,mg);
	// printf("\nac:");
	 for(i=0;i<3;i++)
   {
	    temp=ac[i*2]<<8|ac[i*2+1];
		  ACCEL[i]=temp/16384.0f;
		//  printf("%f\t",ACCEL[i]);
	 }
	// printf("\ngy:");
	 for(i=0;i<3;i++)
   {
	    temp=gy[i*2]<<8|gy[i*2+1];
		  GYRO[i]=temp/16.4f;
		  GYRO[i]=GYRO[i]/180*PI;
		//  printf("%f\t",GYRO[i]);
	 }
	  //printf("\nmag:");
	 for(i=0;i<3;i++)
	 {
      temp=mg[i*2]<<8|mg[i*2+1];
		  MAG[i]=(i==2?-1:1)*temp/3.0f;
		//  printf("%f\t",MAG[i]);
	 }
	 //printf("\n- - - - - - - - - - - - - - - - - - - - - - - - - - -\n");
}


void mpu9150_Adjust_Val(float *ac_adjust,float *gy_adjust,float *mag_adjust)
{
	int i,j;
	float t_ac[3],t_gy[3],t_mg[3];
	float ac_max[3]={0,0,1},ac_min[3]={0,0,1},gy_max[3]={0},gy_min[3]={0};
	float ac_mid[3],gy_mid[3];
	u8 mg_ad[3];
	mpu9150_RawData(t_ac,t_gy,t_mg);
	ac_max[0]=t_ac[0];
	ac_max[1]=t_ac[1];
	ac_max[2]=t_ac[2];
	gy_max[0]=t_gy[0];
	gy_max[1]=t_gy[1];
	gy_max[2]=t_gy[2];
	for(int i=0;i<199;i++)
	{
	  mpu9150_RawData(t_ac,t_gy,t_mg);
		for(j=0;j<3;j++)
		{
		  if(t_ac[j]>ac_max[j])
			{
				ac_max[j]=t_ac[j];
			}
			else if(t_ac[j]<ac_min[j])
			{
			  ac_min[j]=t_ac[j];
			}
		  if(t_gy[j]>gy_max[j])
			{
				gy_max[j]=t_gy[j];
			}
			else if(t_gy[j]<gy_min[j])
			{
			  gy_min[j]=t_gy[j];
			}
		}
	}
	for(i=0;i<3;i++)
	{
	  ac_mid[i]=(ac_max[i]+ac_min[i])/2.0f;
	  gy_mid[i]=(gy_max[i]+gy_min[i])/2.0f;
		ac_adjust[i]=(i<2?0:1)-ac_mid[i];
		gy_adjust[i]=-gy_mid[i];
	}
	AK8975_Adjust_Val(mg_ad);
	for(i=0;i<3;i++)
	  mag_adjust[i]=(mg_ad[i]-128)*0.5f/128+1;
//	printf("az max %f min %f mid %f",ac_max[2],ac_min[2],ac_mid[2]);
}
void AK8975_Adjust_Val(u8 *adjust)
{
   setByte(AK8975_Addr,AK_CNTL,AK_MODE_FUSE_ROM);
   adjust[0]=hearByte(AK8975_Addr,AK_ASAX);
   adjust[1]=hearByte(AK8975_Addr,AK_ASAY);
	 adjust[2]=hearByte(AK8975_Addr,AK_ASAY);
}

void Inverse_Byte(u8 *origin_l,int num)
{
  int i;
	u8 temp;
	for(i=0;i<(num-1)/2;i++)
	{
	   temp=origin_l[i];
		 origin_l[i]=origin_l[num-1-i];
		 origin_l[num-1-i]=origin_l[i];
	}
}
void MS5611_Init()
{
	int i;
  iic_Command(MS5611_Addr,MS_RESET);
  Delay10ms();
//	Delay10ms();
	for(i=0;i<7;i++)
	{
   iic_burst_read((u8*)(&C[i]),2,MS5611_Addr,MS_PROM|(i*2),DIRECT_MSB);
	 Inverse_Byte((u8*)(&C[i]),2);
	}
  Tref=C[5]>>8;
	#ifdef __DEBUG
//	   printf("%d\n",Tref);
//	for(i=0;i<7;i++)
//	 printf("%d\n",C[i]);
	#endif
}
void Delay10ms(void)
{
    double start=Milsec();
	  while((Milsec()-start)!=10);
//	uint8_t i;
//	for (i = 0; i < 30; i++);
}
void Delay1ms(void)
{
	  double start=Milsec();
	  while((Milsec()-start)!=1);
//	uint8_t i;
//	for (i = 0; i < 30; i++);
}
long MS5611_Get_Pressure()
{
  u32 D_Pressure,D_Temperature;
	int dT;
  long long Offset,Sens;
	long long off2=0,sens2=0,t2=0;
	int Temp,dt2;
  iic_Command(MS5611_Addr,MS_D1|MS_RESOLUTION_256);
  Delay1ms();
  iic_burst_read((u8*)(&D_Pressure),3,MS5611_Addr,MS_ADC_READ,DIRECT_MSB);
	//Inverse_Byte((u8*)(&D_Pressure),3);
	if(!D_Pressure)
		return Pressure;
	iic_Command(MS5611_Addr,MS_D2|MS_RESOLUTION_256);
  Delay1ms();
  iic_burst_read((u8*)(&D_Temperature),3,MS5611_Addr,MS_ADC_READ,DIRECT_MSB);
//	Inverse_Byte((u8*)(&D_Temperature),3);
  dT=D_Temperature-Tref;
	Temp=(dT*((uint32_t)C[6]))>>23;
  TEMP=2000+Temp;	
	dt2=Temp*Temp;
	Offset=((uint32_t)C[2]<<16)+(((uint32_t)C[4]*dT)>>7);
	Sens=((uint32_t)C[1]<<15)+(((uint32_t)C[3]*dT)>>8);
	if(TEMP<2000)
	{
	  t2=(dT*dT)>>31;
	  off2=5*dt2>>1;
	  sens2=5*dt2>>2;
		if(TEMP<-1500)
		{
		   dt2=(TEMP+1500)*(TEMP+1500);
			 off2=off2+7*dt2;
			 sens2=sens2+11*dt2/2;
		}
		TEMP-=t2;
		Offset-=off2;
		Sens-=sens2;
	}
	Pressure=(((D_Pressure*Sens)>>21)-Offset)>>15;
	#ifdef __DEBUG
	 // printf("%ld\n",Pressure);
	#endif
	return Pressure;
}



