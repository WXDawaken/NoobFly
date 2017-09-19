#ifndef __FIFO_H_
#define __FIFO_H_
typedef struct FIFO_TYPE
{
 unsigned char *data;
 unsigned char read_ptr; 
 unsigned char write_ptr;
 unsigned char size;
 unsigned isfull;
 unsigned isempty;
}FIFO;
unsigned char FIFO_Read(FIFO* fifo);
void FIFO_Write(FIFO* fifo,unsigned char ch);
void FIFO_Init(FIFO *fifo,int size);
#endif