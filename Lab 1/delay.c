#include "stm32f4xx_hal.h"
// volatile
volatile unsigned int ticks;

void SysTick_Handler(void) {
	ticks++;
} 

void delay(unsigned int msDelay)  {
	unsigned stop = ticks + msDelay;
  while (ticks < stop) {}
} 