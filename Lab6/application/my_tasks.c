#include <stdio.h>
#include "stm32f4xx_hal.h"
#include "FreeRTOS.h"
#include "task.h"
#include "usart.h"
#include "semphr.h"

#include "my_tasks.h"
#include "usart.h"

static void activity(void);
static void USART_POLL_WriteString(const char *);
extern SemaphoreHandle_t mySemaphore;
extern QueueHandle_t myQueue;


// EX 601, 602, 603, 604, 605
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void taskLED(void* params)	
{
	// Toggle the LED on pin GPIOG.13
	while (params) {
    // toggle the LED
    HAL_GPIO_TogglePin(((BlinkParams*)params)->gpio, ((BlinkParams*)params)->pin);
    // introduce some delay
		vTaskDelay(((BlinkParams*)params)->ticks);
	} // while
	
	// Delete the task in case the provided argument is NULL
  vTaskDelete(NULL);
 
} /* taskLED */  


void taskUSART(void* params)
{
	// Do loopback on usart
	while (1) {
		char c;
		if(USART_GetChar(&c))
		{
			USART_PutChar(c);
		}
		vTaskDelay(10);
	} // while (1)
} /* taskUSART */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


// EX 606
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void taskDELAY(void* params)
{
    TickType_t xLastWakeTime = xTaskGetTickCount();
		
    // we are going to use these two trace signals: 
    TRACE_SetLow(5);
    TRACE_SetLow(6);
        
    while (1) {         
        // trace signal 5 toggles on every loop
        TRACE_Toggle(5);
        // trace 6 goes high at the start of activity
        TRACE_SetHigh(6);
        activity();
        // trace 6 goes low at the end of activity
        TRACE_SetLow(6);

        // delay task execution until 500ms - duration of activity
        vTaskDelayUntil( &xLastWakeTime, 500);
    }
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


// EX 607
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void taskSEMPH(void* params)
{
	while(true)
	{
		if(mySemaphore != NULL)
		{
			if( pdTRUE == xSemaphoreTake(mySemaphore, portMAX_DELAY) )
					HAL_GPIO_TogglePin(GPIOG, GPIO_PIN_13);
		}
	}
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


// EX 608
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void taskHELLO(void* params)
{
	static SemaphoreHandle_t myMutex;
	taskENTER_CRITICAL();
	if(NULL == myMutex)
	{
		myMutex = xSemaphoreCreateMutex();
		USART_POLL_WriteString("\r\n\r\nCREATE\r\n\r\n");
	}
	taskEXIT_CRITICAL();
  while (1)
	{
		vTaskDelay(10000);
		if( pdTRUE == xSemaphoreTake(myMutex, portMAX_DELAY) )
		{
			USART_POLL_WriteString("hello world\r\n");
		}
    xSemaphoreGive(myMutex);
  }
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


// EX 609
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void taskTXQueue(void* params)
{
	while(true)
	{
		TickType_t xLastWakeTime = xTaskGetTickCount();
		static unsigned int intToSend = 1;
		if (xQueueSend(myQueue, &intToSend, 1000) != pdPASS)
		{
			// Failed to put new element into the queue, even after 1000 ticks.
			USART_WriteString("ERROR: Failed to put new element into the queue\r\n");
		}
		//intToSend++;
		if(0 == intToSend)
		{
			intToSend++;
		}
		else
		{
			intToSend *= 2;
		}
		
		vTaskDelayUntil( &xLastWakeTime, 500);
	}
}

void taskRXQueue(void* params)
{
	const size_t buf_size = 62;
	char str[buf_size];
	while(true)
	{
		int element;
		if (xQueueReceive(myQueue, &element, 1000) == pdTRUE)
		{
			// element was received successfully
			snprintf(str, buf_size, "Got %u from myQueue\r\n", element);
			USART_WriteString(str);
		} 
		else 
		{
			// unable to receive elements from the queue in the given time = 1000 ticks
			USART_WriteString("ERROR: got NOTHING from myQueue\r\n");
		}
	}
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


static void activity(void)
{
    static volatile uint32_t period = 400000;
    volatile uint32_t i;
        
    // this loop executes 400000 or 80000 times,
    // which makes this activity once longer once shorter in time
    for (i = 0; i < period; i++) {
        __asm__ volatile ("NOP");
    }
        
    if (period == 400000) {
        period = 80000;
    } else {
        period = 400000;
    }
}

/** Writes string to USART using polling. */
static void USART_POLL_WriteString(const char *string)
{
  size_t i=0;
  // for each character in the given string
  while (string[i]) {
    // write the current character to USART's data register (DR)
    USART1->DR = string[i];
    // wait until status register (SR) indicates that the transmitter is empty again (TXE)
    while((USART1->SR & UART_FLAG_TXE) == 0) {
      ;
    }
    // move to the next character
    i++;
  }
}