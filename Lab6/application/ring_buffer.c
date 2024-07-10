/* Includes ------------------------------------------------------------------*/
#include <assert.h>
#include "ring_buffer.h"


bool RingBuffer_Init(RingBuffer *ringBuffer, char *dataBuffer, size_t dataBufferSize) 
{
	assert(ringBuffer);
	assert(dataBuffer);
	assert(dataBufferSize > 0);
	
	if ((ringBuffer) && (dataBuffer) && (dataBufferSize > 0))
	{
		ringBuffer->buffer = dataBuffer;
		ringBuffer->head = 0;
		ringBuffer->tail = 0;
		ringBuffer->count = 0;
		ringBuffer->count_max = dataBufferSize;
		return true;
	}
	
	return false;
}

bool RingBuffer_Clear(RingBuffer *ringBuffer)
{
	assert(ringBuffer);
	
	if (ringBuffer) {
		ringBuffer->head = 0;
	  	ringBuffer->tail = 0;
	  	ringBuffer->count = 0;
		return true;
	}
	return false;
}

bool RingBuffer_IsEmpty(const RingBuffer *ringBuffer)
{
  	assert(ringBuffer);	
	return (0 == ringBuffer->count);
}

size_t RingBuffer_GetLen(const RingBuffer *ringBuffer)
{
	assert(ringBuffer);
	
	if (ringBuffer)
	{
		return ringBuffer->count;
	}
	return 0;
	
}

size_t RingBuffer_GetCapacity(const RingBuffer *ringBuffer)
{
	assert(ringBuffer);
	
	if (ringBuffer)
	{
		return ringBuffer->count_max;
	}
	return 0;	
}


bool RingBuffer_PutChar(RingBuffer *ringBuffer, char c)
{
	assert(ringBuffer);
	
	if (ringBuffer) {
		if(ringBuffer->count < ringBuffer->count_max)
		{
			ringBuffer->count++;
			ringBuffer->buffer[ringBuffer->head] = c;
			ringBuffer-> head++;
			if(ringBuffer->head == ringBuffer->count_max)
			{
				ringBuffer->head = 0;
			}
			return true;
		}

	}
	return false;
}

bool RingBuffer_GetChar(RingBuffer *ringBuffer, char *c)
{
	assert(ringBuffer);
	assert(c);
	
  	if ((ringBuffer) && (c))
	{
		if(ringBuffer->count > 0)
		{
			*c = ringBuffer->buffer[ringBuffer->tail];
			ringBuffer->count--;
			ringBuffer->tail++;
			if(ringBuffer->tail == ringBuffer->count_max)
			{
				ringBuffer->tail = 0;
			}
			return true;
		}
	}
	return false;
}
