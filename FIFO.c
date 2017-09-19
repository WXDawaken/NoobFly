#include "FIFO.h"
#include <stdlib.h>
unsigned char FIFO_Read(FIFO* fifo)
{
  unsigned char ch;
 ch=fifo->data[fifo->read_ptr];
  fifo->read_ptr=(fifo->read_ptr+1)%fifo->size;
  if(fifo->read_ptr==fifo->write_ptr)
		fifo->isempty=1;
	if(fifo->isfull)
		fifo->isfull=0;
  return ch;
}

void FIFO_Write(FIFO* fifo,unsigned char ch)
{
	fifo->data[fifo->write_ptr]=ch;
	fifo->write_ptr=(fifo->write_ptr+1)%fifo->size;
	if(fifo->write_ptr==fifo->read_ptr)
	  fifo->isfull=1;
	if(fifo->isempty)
	  fifo->isempty=0;
}

void FIFO_Init(FIFO *fifo,int size)
{
  fifo->size=size;
	fifo->data=(unsigned char*)malloc(100*sizeof(unsigned char));
	fifo->read_ptr=fifo->write_ptr=0;
	fifo->isempty=1;
	fifo->isfull=0;
}