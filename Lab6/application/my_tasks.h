#ifndef _MY_TASKS_H_
#define _MY_TASKS_H_

#include "stm32f4xx_hal.h"
#include "FreeRTOS.h"
#include "task.h"

typedef struct {
        GPIO_TypeDef* gpio;    // GPIO port
        uint16_t      pin;     // GPIO pin
        TickType_t    ticks;   // delay expressed in system ticks
} BlinkParams;

void taskLED(void*);
void taskLED2(void*);
void taskUSART(void*);
void taskDELAY(void*);
void taskSEMPH(void*);
void taskHELLO(void*);

void taskTXQueue(void*);
void taskRXQueue(void*);

#endif
