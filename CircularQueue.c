/*
 * CircularQueue.c
 *
 *  Created on: 2018年2月12日
 *      Author: QH101-PC-LPH
 */

#include "CircularQueue.h"
//#include "SysTick.h"

//#include "DebugMsg.h"

#include "time.h"

#define CirCularQueueDebugMsg(fmt,args...) debugMsg(fmt,##args)



#ifndef USER_CIRCULARQUEUE_C_
#define USER_CIRCULARQUEUE_C_


#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

#ifndef NULL
#define NULL 0
#endif



#define CircularQueue_GetSysMs() clock() //SysTick_ms()

#define CircularQueue_TimePeroid(tm) (clock()-tm)

#define CircularQueue_DelayMs(tm)  //delay_ms(tm)



/*
 * function name:CircularQueueSpace
 * brief:get the remain space of the queue
 * parameters:
 * 	1.CircularQueue_t--the pointer to the queue structure;
 * return: the remain space of the queue;
 */
__inline static uint32_t CircularQueueSpace(CIRCULAR_QUEUE *CircularQueue_t)
{
	if(CircularQueue_t->capacity>CircularQueueLenth(CircularQueue_t))
		return CircularQueue_t->capacity-CircularQueueLenth(CircularQueue_t);
	else
		return 0;
}


/*
 * function name:CircularQueueIsFull
 * brief: check the queue is full or not;
 */
__inline static int8_t CircularQueueIsFull(CIRCULAR_QUEUE *CircularQueue_t)
{
	if(CircularQueueSpace(CircularQueue_t)==0)
		return TRUE;
	return FALSE;
}

/*
 * function name:CircularQueueIsEmpty
 * brief:check the queue is empty or not;
 */
__inline static int8_t CircularQueueIsEmpty(CIRCULAR_QUEUE *CircularQueue_t)
{
	if(CircularQueueLenth(CircularQueue_t)==0)
		return TRUE;
	return FALSE;
}

/*
 * function name:CircularQueueFrontNext
 * brief: move the queue front position to next
 */
__inline static void CircularQueueFrontNext(CIRCULAR_QUEUE *CircularQueue_t)
{
	if(!CircularQueueIsEmpty(CircularQueue_t))
	{
		CircularQueue_t->front++;
		if(CircularQueue_t->front>CircularQueue_t->capacity)
			CircularQueue_t->front=0;
		//CircularQueue_t->dataLen--;
		//CirCularQueueDebugMsg("front:%d,rear:%d,len:%d\r\n",CircularQueue_t->front,CircularQueue_t->rear,CircularQueue_t->dataLen);
	}
}

/*
 * function name:CircularQueueRearNext
 * brief: move the queue rear position to next
 */
__inline static void CircularQueueRearNext(CIRCULAR_QUEUE *CircularQueue_t)
{
	if(!CircularQueueIsFull(CircularQueue_t))
	{
		CircularQueue_t->rear++;
		if(CircularQueue_t->rear>CircularQueue_t->capacity)
			CircularQueue_t->rear=0;
		//CircularQueue_t->dataLen++;
	}
}

/*
 * function name:CircularQueueSingleTake
 * brief: take an unit from the queue to the data buff
 * parameters:
 * 	1.CircularQueue_t--pointer of the queue
 * 	2.data--the pointer of the destination data address
 * return: TRUE--take success,FALSE--the queue is empty
 */
__inline static int8_t CircularQueueSingleTake(CIRCULAR_QUEUE *CircularQueue_t,uint8_t *data)
{
	uint32_t i;
	uint8_t *res;
	if(CircularQueueIsEmpty(CircularQueue_t))
	{
		for(i=0;i<CircularQueue_t->unitSize;i++)
		{
			data[i]=0;
		}
		return FALSE;
	}
	else
	{
		for(i=0,res=CircularQueue_t->dataBuff;i<CircularQueue_t->front;i++)
		{
			res+=CircularQueue_t->unitSize;
		}
		for(i=0;i<CircularQueue_t->unitSize;i++)
		{
			data[i]=res[i];
		}
		CircularQueueFrontNext(CircularQueue_t);
		return TRUE;
	}
}


/*
 * function name:CircularQueueSingleInsertForce
 * brief: insert one unit to queue by cover mode
 * parameters:
 * 	1.CircularQueue_t--pointer to the queue
 * 	2.data--the insert data pointer
 * return:null
 */
__inline static void CircularQueueSingleInsertForce(CIRCULAR_QUEUE *CircularQueue_t,uint8_t *data)
{
	uint8_t *dst;
	uint32_t i;
	if(CircularQueueIsFull(CircularQueue_t))
	{
		CircularQueueFrontNext(CircularQueue_t);
		//CircularQueueSingleTake(CircularQueue_t,dst);
	}
	for(i=0,dst=CircularQueue_t->dataBuff;i<CircularQueue_t->rear;i++)
	{
		dst+=CircularQueue_t->unitSize;
	}
	for(i=0;i<CircularQueue_t->unitSize;i++)
	{
		dst[i]=data[i];
	}

	CircularQueueRearNext(CircularQueue_t);

}


/*
 * function name:CircularQueueSingleInsertWait
 * brief: insert one unit to the queue by waiting mode,not cover the exited data
 * parameters:
 * 	1.CircularQueue_t--pointer of the queue
 * 	2.data--the pointer of the insert data buffer
 * 	3.timeout--wait time for the operator
 * return: TRUE--insert success without timeout, FALSE--insert failed
 */
__inline static int8_t CircularQueueSingleInsertWait(CIRCULAR_QUEUE *CircularQueue_t,uint8_t *data,uint32_t timeout)
{
	uint8_t *dst;
	uint32_t i;
	uint32_t tm;
	tm=CircularQueue_GetSysMs();
	while(CircularQueueIsFull(CircularQueue_t)&&CircularQueue_TimePeroid(tm)<timeout)
	{
		CircularQueue_DelayMs(50);
	}
	if(CircularQueueIsFull(CircularQueue_t))
	{
		return FALSE;
	}
	for(i=0,dst=CircularQueue_t->dataBuff;i<CircularQueue_t->rear;i++)
	{
		dst+=CircularQueue_t->unitSize;
	}
	for(i=0;i<CircularQueue_t->unitSize;i++)
	{
		dst[i]=data[i];
	}

	CircularQueueRearNext(CircularQueue_t);
	return TRUE;
}

uint32_t CircularQueueLenth(CIRCULAR_QUEUE *CircularQueue_t)
{
	if(CircularQueue_t->rear>=CircularQueue_t->front)
	{
		return CircularQueue_t->rear-CircularQueue_t->front;
	}
	return CircularQueue_t->capacity+CircularQueue_t->rear-CircularQueue_t->front+1;
}


/*
 * function name: CircularQueueInit
 * brief: queue initialize
 * parameters:
 * 	1.CircularQueue_t--pointer of the queue
 * 	2.buf--pointer of the queue data buffer
 * 	(warring:the buffer size must larger than the capacity,
 * 	buffer size less or equal to the capacity is not allowed)
 * 	3.capacity--the max size of the queue
 * 	4.unitSize--the unit size of the queue,indicate the single data size
 * return: NULL
 */
void CircularQueueInit(CIRCULAR_QUEUE *CircularQueue_t,uint8_t *buf,uint32_t capacity,uint32_t unitSize)
{
	CircularQueue_t->dataBuff=buf;
	CircularQueue_t->front=0;
	CircularQueue_t->rear=0;
	CircularQueue_t->unitSize=unitSize;
	CircularQueue_t->capacity=capacity;
}

/*
 * function name:CircularQueueInsert
 * brief:insert a continuous data unit to the queue with waiting mode
 * parameters:
 * 	1.CircularQueue_t--pointer of the queue
 * 	2.data--pointer of the insert data unit
 * 	3.len--the insert unit number
 * 	4.timeout--the operator timeout
 * return: insert success data unit number
 */
uint32_t CircularQueueInsert(CIRCULAR_QUEUE *CircularQueue_t,uint8_t *data,uint32_t len,uint32_t timeout)
{
	uint32_t i;
	uint32_t tm;

	for(i=0;i<len;)
	{
		if(CircularQueueSingleInsertWait(CircularQueue_t,data,0)==TRUE)
		{
			i++;
			data+=CircularQueue_t->unitSize;
		}
		else if(CircularQueue_TimePeroid(tm)>timeout)
		{
			break;
		}
	}
	return i;
}


/*
 * function name:CircularQueueInsertForce
 * brief:insert a continuous data unit to the queue with cover mode
 * parameters:
 * 	1.CircularQueue_t--pointer of the queue
 * 	2.data--pointer of the insert data unit
 * 	3.len--the insert unit number
 * return: null
 */
void CircularQueueInsertForce(CIRCULAR_QUEUE *CircularQueue_t,uint8_t *data,uint32_t len)
{
	uint32_t i;

	for(i=0;i<len;i++,data+=CircularQueue_t->unitSize)
	{
		CircularQueueSingleInsertForce(CircularQueue_t,data);
	}
}


/*
 * function name:CircularQueueTake
 * brief: take a continuous data unit to the queue with waiting mode
 * parameters:
 * 	1.CircularQueue_t--pointer of the queue
 * 	2.data--pointer of the take out data unit buffer
 * 	3.len--the take out unit number
 * 	4.timeout--the operator timeout
 * return: the take out data unit number
 */
uint32_t CircularQueueTake(CIRCULAR_QUEUE *CircularQueue_t,uint8_t *data,uint32_t len,uint32_t timeout)
{
	uint32_t i;
	uint32_t tm;
	tm=CircularQueue_GetSysMs();
	for(i=0;i<len;)
	{
		if(CircularQueueSingleTake(CircularQueue_t,data)==TRUE)
		{
			data+=CircularQueue_t->unitSize;
			i++;
		}
		else if(CircularQueue_TimePeroid(tm)>timeout)
		{
			break;
		}
	}
	return i;
}


/*
 * function CircularQueueTakeUnit
 * brief: take an unit from the queue to the data buff
 * parameters:
 * 	1.CircularQueue_t--pointer of the queue
 * 	2.data--the pointer of the destination data address
 * return: TRUE--take success,FALSE--the queue is empty
 */
int8_t CircularQueueTakeUnit(CIRCULAR_QUEUE *CircularQueue_t,uint8_t *data,uint32_t timeout)
{
	uint32_t tm;
	tm=CircularQueue_GetSysMs();
	do{	
		if(CircularQueueSingleTake(CircularQueue_t,data)==TRUE)
		{
			return TRUE;
		}
	}while(CircularQueue_TimePeroid(tm)<timeout);
	return FALSE;
}



/*
 * function name:CircularQueueSingleInsertForce
 * brief: insert one unit to queue by cover mode
 * parameters:
 * 	1.CircularQueue_t--pointer to the queue
 * 	2.data--the insert data pointer
 * return:null
 */
void CircularQueueInsertUnitForce(CIRCULAR_QUEUE *CircularQueue_t,uint8_t *data)
{
	CircularQueueSingleInsertForce(CircularQueue_t,data);
}



/*
 * function name:CircularQueueSingleInsertWait
 * brief: insert one unit to the queue by waiting mode,not cover the exited data
 * parameters:
 * 	1.CircularQueue_t--pointer of the queue
 * 	2.data--the pointer of the insert data buffer
 * 	3.timeout--wait time for the operator
 * return: TRUE--insert success without timeout, FALSE--insert failed
 */
int8_t CircularQueueInsertUnitWait(CIRCULAR_QUEUE *CircularQueue_t,uint8_t *data,uint32_t timeout)
{
	return CircularQueueSingleInsertWait(CircularQueue_t,data,timeout);
}

void CircularQueueClear(CIRCULAR_QUEUE *CircularQueue_t)
{
	//CircularQueue_t->dataLen=0;
	CircularQueue_t->front=0;
	CircularQueue_t->rear=0;
}


#endif /* USER_CIRCULARQUEUE_C_ */
