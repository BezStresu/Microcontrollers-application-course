#include <stdio.h>
#include "stm32f4xx_hal.h"
#include "usart.h"
#include "ring_buffer_unit_test.h"
#include "usart_unit_test.h"
#include "FreeRTOS.h"
#include "task.h"
#include "my_tasks.h"
#include "semphr.h"

void LED_Init(void);
void SystemClock_Config(void);
static void EXTI2_Init(void);
void EXTI2_IRQHandler(void);

static BlinkParams bp1 = { .gpio = GPIOG, .pin = GPIO_PIN_13, .ticks = 500};
static BlinkParams bp2 = { .gpio = GPIOG, .pin = GPIO_PIN_14, .ticks = 1000};  

SemaphoreHandle_t mySemaphore;
QueueHandle_t myQueue;
xSemaphoreHandle myMutex;
int main (void)
{
    // Initialize STM32Cube HAL library
    HAL_Init();
    // Initialize LED pins
    LED_Init();
    // Create mutex
    myMutex= xSemaphoreCreateMutex();
    // Create queue
    myQueue= xQueueCreate(100, 1);
    // Create RTOS tasks
    if (pdPASS != xTaskCreate(taskLED, "led", 200, NULL, 3, NULL )) {
    // should never get here, unless there is a memory allocation problem
    }
    // start FreeRTOS scheduler - the RTOS takes control over the microcontroller
    vTaskStartScheduler();
} /* main */


//int main(void) {
//  // Initialize STM32Cube HAL library
//  HAL_Init();
//	SystemClock_Config();
//	LED_Init();
//	USART_Init();
//	TRACE_Init();
//	EXTI2_Init();
//	
//	TaskHandle_t taskLedHandle;
//	TaskHandle_t taskLed2Handle;
//	TaskHandle_t taskUsartHandle;
//	TaskHandle_t taskSemaphore;
//	
//	mySemaphore = xSemaphoreCreateBinary();
//	myQueue = xQueueCreate(10, sizeof(int));
//	
//	EX 601, 602, 603, 604, 605
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	if (pdPASS != xTaskCreate(taskLED, "led", configMINIMAL_STACK_SIZE, &bp1, 3, &taskLedHandle))
//	{
//    printf("ERROR: Unable to create task led!\n");
//	}
//	TRACE_BindTaskWithTrace(taskLedHandle, 2);
//	
//	if (pdPASS != xTaskCreate(taskLED, "led2", configMINIMAL_STACK_SIZE, &bp2, 3, &taskLed2Handle))
//	{
//    printf("ERROR: Unable to create task led2!\n");
//	}
//	TRACE_BindTaskWithTrace(taskLed2Handle, 3);
//	if (pdPASS != xTaskCreate(taskUSART, "echo", configMINIMAL_STACK_SIZE, NULL, 3, &taskUsartHandle))
//	{
//    printf("ERROR: Unable to create task echo!\n");
//	}
//	TRACE_BindTaskWithTrace(taskUsartHandle, 4);
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


// EX 606
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	if (pdPASS != xTaskCreate(taskDELAY, "delay test", configMINIMAL_STACK_SIZE, NULL, 3, NULL))
//	{
//    printf("ERROR: Unable to create task delay test!\n");
//	}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


// EX 607
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	if (pdPASS != xTaskCreate(taskSEMPH, "semph test", configMINIMAL_STACK_SIZE, NULL, 3, NULL))
//	{
//    printf("ERROR: Unable to create task semph test!\n");
//	}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


// EX 608
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	if (pdPASS != xTaskCreate(taskHELLO, "mutex test", configMINIMAL_STACK_SIZE, NULL, 3, NULL))
//	{
//    printf("ERROR: Unable to create task mutex test!\n");
//	}
//	
//	if (pdPASS != xTaskCreate(taskHELLO, "mutex test2", configMINIMAL_STACK_SIZE, NULL, 3, NULL))
//	{
//    printf("ERROR: Unable to create task mutex test2!\n");
//	}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


// EX 609
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	if (pdPASS != xTaskCreate(taskTXQueue, "test TXQueue", configMINIMAL_STACK_SIZE, NULL, 3, NULL))
//	{
//    printf("ERROR: Unable to create task test TXQueue!\n");
//	}
//	
//	if (pdPASS != xTaskCreate(taskRXQueue, "test RXQueue", configMINIMAL_STACK_SIZE, NULL, 3, NULL))
//	{
//    printf("ERROR: Unable to create task test RXQueue!\n");
//	}
//	
//	vTaskStartScheduler();
//	
//	//RingBuffer_UnitTest();
//	//USART_UnitTest();
//	
//	
//	// UART
//	//
//	//USART_WriteString("Hello world\n\r");
//	//USART_WriteString("Enter your message in 5s\n\r");
//	
//} /* main */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/**
 * This function configures the LED pins
 */
void LED_Init(void)  {
  GPIO_InitTypeDef GPIO_InitStruct;

  // GPIO Ports Clock Enable
  __GPIOG_CLK_ENABLE();

  // Configure GPIO pin PG.13
  GPIO_InitStruct.Pin = GPIO_PIN_13;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP; // push-pull output
  GPIO_InitStruct.Pull = GPIO_NOPULL; // no pull
  GPIO_InitStruct.Speed = GPIO_SPEED_LOW; // analog pin bandwidth limited
  HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);
	
	GPIO_InitStruct.Pin = GPIO_PIN_14;
	HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);
	

} /* LED_Init */

void SystemClock_Config(void) {		//EX 606
    RCC_OscInitTypeDef RCC_OscInitStruct;
    RCC_ClkInitTypeDef RCC_ClkInitStruct;

    /* Enable Power Control clock */
    __HAL_RCC_PWR_CLK_ENABLE();

    /* The voltage scaling allows optimizing the power consumption when the
    device is clocked below the maximum system frequency (see datasheet). */
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

    /* Enable HSE Oscillator and activate PLL with HSE as source */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState = RCC_HSE_ON;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLM = 8;
    RCC_OscInitStruct.PLL.PLLN = 336;
    RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
    RCC_OscInitStruct.PLL.PLLQ = 7;
    HAL_RCC_OscConfig(&RCC_OscInitStruct);

    /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 clocks dividers */
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 |
    RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;
    HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5);
}

/**
 * Configures EXTI Line2 (connected to PG2 pin) in interrupt mode
 */
static void EXTI2_Init(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure;
        
  // Enable GPIOG clock
  __GPIOG_CLK_ENABLE();
  
  // Configure PG2 pin as input with EXTI interrupt on the falling edge and pull-up
  GPIO_InitStructure.Speed = GPIO_SPEED_LOW;
  GPIO_InitStructure.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStructure.Pull = GPIO_PULLUP;
  GPIO_InitStructure.Pin = GPIO_PIN_2;
  HAL_GPIO_Init(GPIOG, &GPIO_InitStructure);
 
  // Enable and set EXTI Line2 Interrupt to the lowest priority
  HAL_NVIC_SetPriority(EXTI2_IRQn, 15, 0);
  HAL_NVIC_EnableIRQ(EXTI2_IRQn);
}
 
/**
 * This function handles External line 2 interrupt request.
 */
void EXTI2_IRQHandler(void)
{
  // Check if EXTI line interrupt was detected
  if(__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_2) != RESET)  {
    // Clear the interrupt (has to be done for EXTI)
    __HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_2);
    // Toggle LED
    HAL_GPIO_TogglePin(GPIOG, GPIO_PIN_14);
		BaseType_t higherPriorityTaskWoken;
		xSemaphoreGiveFromISR(mySemaphore, &higherPriorityTaskWoken);
  }
}