/* Includes ------------------------------------------------------------------*/
#include <assert.h>
#include "ring_buffer.h"
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

bool RingBuffer_Init(RingBuffer *ringBuffer, char *dataBuffer, size_t dataBufferSize) 
{
	assert(ringBuffer);
	assert(dataBuffer);
	assert(dataBufferSize > 0);
	
	if ((ringBuffer) && (dataBuffer) && (dataBufferSize > 0)) {
      ringBuffer->data = dataBuffer;
      ringBuffer->max_size = dataBufferSize;
      ringBuffer->tail = 0;
      ringBuffer->head = 0;
      ringBuffer->length = 0;
      return true;
	}
	
	return false;
}

bool RingBuffer_Clear(RingBuffer *ringBuffer)
{
	assert(ringBuffer);
	
	if (ringBuffer) {
		if(memset(ringBuffer->data, 0, ringBuffer->max_size)){
            ringBuffer->head = 0;
            ringBuffer->tail = 0;
            ringBuffer->length = 0;
            return true;
        }
	}
	return false;
}

bool RingBuffer_IsEmpty(const RingBuffer *ringBuffer)
{
  assert(ringBuffer);	
  return ringBuffer->length == 0;
}

size_t RingBuffer_GetLen(const RingBuffer *ringBuffer)
{
	assert(ringBuffer);
	
	if (ringBuffer) {
        return ringBuffer->length;
	}
	return 0;
	
}

size_t RingBuffer_GetCapacity(const RingBuffer *ringBuffer)
{
	assert(ringBuffer);
	
	if (ringBuffer) {
		return ringBuffer->max_size;
	}
	return 0;	
}


bool RingBuffer_PutChar(RingBuffer *ringBuffer, char c)
{
	assert(ringBuffer);
	
	if (ringBuffer) {
		if(RingBuffer_GetLen(ringBuffer) == ringBuffer->max_size){
            return false;
        }
        ringBuffer->data[ringBuffer->head] = c;
        ringBuffer->head++;
        if(ringBuffer->head == ringBuffer->max_size){
            ringBuffer->head = 0;
        }
        ringBuffer->length++;
		return true;
	}
	return false;
}

bool RingBuffer_GetChar(RingBuffer *ringBuffer, char *c)
{
	assert(ringBuffer);
	assert(c);
	
  if ((ringBuffer) && (c)) {
		if(RingBuffer_IsEmpty(ringBuffer)){
            return false;
        }
        *c = ringBuffer->data[ringBuffer->tail];
        ringBuffer->tail ++;
        if(ringBuffer->tail == ringBuffer->max_size){
            ringBuffer->tail = 0;
        }
        ringBuffer->length--;
        return true;
	}
	return false;
}