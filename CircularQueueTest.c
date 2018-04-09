/*
 * CircularQueueTest.c
 *
 *  Created on: 2018年4月9日
 *      Author: LiPenghui from China
 */

#include "stdlib.h"
#include "stdio.h"
#include "CircularQueue.h"
#include "stdint.h"

CIRCULAR_QUEUE queue;
uint8_t queueBuf[256];

#define QUEUE_SIZE 60
#define QUEUE_UNIT_SIZE 2

typedef struct{
	uint8_t data1;
	uint8_t data2;
}__attribute__((packed)) DATA_STRUCT ;

int main(void)
{
	int i;
	DATA_STRUCT data;
	CircularQueueInit(&queue,queueBuf,QUEUE_SIZE,QUEUE_UNIT_SIZE);

	for(i=0;i<40;i++)
	{
		data.data1=i;
		data.data2=i*2;
		CircularQueueInsertForce(&queue,(uint8_t *)&data,1);
	}

	for(i=0;i<20;i++)
	{
		CircularQueueTakeUnit(&queue,(uint8_t *)&data,10);
		printf("%d:%02x,%02x\r\n",i,data.data1,data.data2);
	}

	return -1;
}
