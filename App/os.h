#ifndef __os_h
#define __os_h

#include "stm32f4xx.h"
//#include "i2c.h"
#include <arm_math.h>
#include "Sys_Time.h"
typedef void(*Runable)(void);

typedef struct TASK
{
  Runable t_Run;
	unsigned long t_Interval;
  unsigned long t_Delay;
	unsigned char t_Flag;
}TASK;

#define uint unsigned int
#define uchar unsigned char
#define ulong unsigned long
#define u8  unsigned char
#define  p_f_max  10  


void f_add(void add(),ulong time,ulong Delay);   
extern TASK os_Tasks[p_f_max];
void tim_1ms();
void os_start();
void f_add(void add(),ulong time,ulong Delay);

void f_kill(uchar num);

void os_run();


#endif
