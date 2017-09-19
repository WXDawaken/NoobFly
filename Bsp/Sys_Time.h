#ifndef __SYS_TIME_H_
#define __SYS_TIME_H_
extern volatile unsigned long long Sys_tick;
extern volatile unsigned long long microsecs;
extern volatile unsigned long long milsecs;
extern volatile unsigned long long seconds;
extern volatile unsigned long long mintues;
extern volatile unsigned long long hours;
extern volatile unsigned long long days;
extern volatile unsigned long long months;
extern volatile unsigned long long years;
void Sys_Time_Init(long long System_Freq);
#define Microsec() microsecs+(Milsec())*1000
#define Milsec()  milsecs+(Seconds())*1000
#define Seconds() seconds+(Minutes())*60
#define Minutes() mintues+(Hours())*24
#define Hours() hours+(Days())*24
#define Days() days
#define Months() months
#define Years() years
#endif