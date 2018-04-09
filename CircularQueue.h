/*
 * CircularQueue.h
 *
 *  Created on: 2018年2月12日
 *      Author: QH101-PC-LPH
 *  warring: the queue buff size must large then or equal to queue size
 */



#ifndef USER_CIRCULARQUEUE_H_
#define USER_CIRCULARQUEUE_H_
#include "stdint.h"
typedef struct{
	uint8_t *dataBuff;
	uint32_t front;
	uint32_t rear;
	//uint32_t dataLen;
	uint32_t capacity;
	uint32_t unitSize;
}__attribute__((packed))CIRCULAR_QUEUE;

typedef CIRCULAR_QUEUE* CIRCULAR_QUEUE_t;


void CircularQueueInit(CIRCULAR_QUEUE *CircularQueue_t,uint8_t *buf,uint32_t capacity,uint32_t unitSize);
uint32_t CircularQueueInsert(CIRCULAR_QUEUE *CircularQueue_t,uint8_t *data,uint32_t len,uint32_t timeout);
void CircularQueueInsertForce(CIRCULAR_QUEUE *CircularQueue_t,uint8_t *data,uint32_t len);
uint32_t CircularQueueTake(CIRCULAR_QUEUE *CircularQueue_t,uint8_t *data,uint32_t len,uint32_t timeout);
int8_t CircularQueueTakeUnit(CIRCULAR_QUEUE *CircularQueue_t,uint8_t *data,uint32_t timeout);
void CircularQueueInsertUnitForce(CIRCULAR_QUEUE *CircularQueue_t,uint8_t *data);
int8_t CircularQueueInsertUnitWait(CIRCULAR_QUEUE *CircularQueue_t,uint8_t *data,uint32_t timeout);
void CircularQueueClear(CIRCULAR_QUEUE *CircularQueue_t);
uint32_t CircularQueueLenth(CIRCULAR_QUEUE *CircularQueue_t);













#endif /* USER_CIRCULARQUEUE_H_ */
