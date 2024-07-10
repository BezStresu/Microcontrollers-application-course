#include "stm32f4xx_hal.h"
#include "usart.h"
#include "usart_unit_test.h"

void LED_Init(void)  {
  GPIO_InitTypeDef GPIO_InitStruct;
  __GPIOG_CLK_ENABLE();
  GPIO_InitStruct.Pin = GPIO_PIN_13;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP; // push-pull output
  GPIO_InitStruct.Pull = GPIO_NOPULL; // no pull
  GPIO_InitStruct.Speed = GPIO_SPEED_LOW; // analog pin bandwidth limited
  HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

} /* LED_Init */


int main(void){
	HAL_Init();
//	__GPIOG_CLK_ENABLE();
	USART_Init();
	
	USART_UnitTest();
	
	
	
	return 0;
}

