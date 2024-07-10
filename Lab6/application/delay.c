#include "stm32f4xx_hal.h"
#include "delay.h"

static volatile int _ticks;

/**
 * System Tick Interrupt Service Routine
 */
void SysTick_Handler(void) 
{
	_ticks++;
} /* SysTick_Handler */

	
void delay(unsigned int msDelay)
{
	unsigned int start = _ticks;
	while((_ticks) < (start + msDelay));
}