/**
  ******************************************************************************
  * @file    FreeRTOS\FreeRTOS_Semaphore\Src\main.c
  * @author  MCD Application Team
  * @version V1.2.0
  * @date    19-June-2015
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2015 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "cmsis_os.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
#define semtstSTACK_SIZE      configMINIMAL_STACK_SIZE
/* Private variables ---------------------------------------------------------*/
osThreadId SemThread1Handle, SemThread2Handle;
osSemaphoreId osSemaphore;
/* Private function prototypes -----------------------------------------------*/
static void SemaphoreThread1(void const *argument);
static void SemaphoreThread2(void const *argument);
static void SystemClock_Config(void);
static void Error_Handler(void);

/* Private functions ---------------------------------------------------------*/
/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
int main(void)
{
  /* STM32F3xx HAL library initialization:
       - Configure the Flash prefetch
       - Systick timer is configured by default as source of time base, but user 
         can eventually implement his proper time base source (a general purpose 
         timer for example or other time source), keeping in mind that Time base 
         duration should be kept 1ms since PPP_TIMEOUT_VALUEs are defined and 
         handled in milliseconds basis.
       - Set NVIC Group Priority to 4
       - Low Level Initialization
     */
  HAL_Init();

  /* Initialize LEDs */
  BSP_LED_Init(LED1);
  BSP_LED_Init(LED2);

  /* Configure the system clock to 72 MHz */
  SystemClock_Config();

  /* Define used semaphore */
  osSemaphoreDef(SEM);

  /* Create the semaphore used by the two threads. */
  osSemaphore = osSemaphoreCreate(osSemaphore(SEM) , 1);

  /* Create the first Thread */
  osThreadDef(SEM_Thread1, SemaphoreThread1, osPriorityLow, 0, semtstSTACK_SIZE);
  SemThread1Handle = osThreadCreate(osThread(SEM_Thread1), (void *) osSemaphore);

  /* Create the second Thread */
  osThreadDef(SEM_Thread2, SemaphoreThread2, osPriorityIdle, 0, semtstSTACK_SIZE);
  SemThread2Handle = osThreadCreate(osThread(SEM_Thread2), (void *) osSemaphore);

  /* Start scheduler */
  osKernelStart(NULL, NULL);

  /* We should never get here as control is now taken by the scheduler */
  for (;;);

}

/**
  * @brief  Semaphore Thread 1 function
  * @param  argument: shared semaphore
  * @retval None
  */
static void SemaphoreThread1(void const *argument)
{
  uint32_t count = 0;
  osSemaphoreId semaphore = (osSemaphoreId) argument;

  for (;;)
  {

    if (semaphore != NULL)
    {
      /* Try to obtain the semaphore. */
      if (osSemaphoreWait(semaphore , 100) == osOK)
      {
        count = osKernelSysTick() + 5000;

        while (count >= osKernelSysTick())
        {
          /* Toggle LED1 */
          BSP_LED_Toggle(LED1);

          /* Delay 200 ms */
          osDelay(200);
        }

        /* Turn off LED1*/
        BSP_LED_Off(LED1);

        /* Release the semaphore. */
        osSemaphoreRelease(semaphore);

        /* Suspend ourseleves to execute thread 2 (lower priority)  */
        osThreadSuspend(NULL);
      }
    }
  }
}

/**
  * @brief  Semaphore Thread 2 function
  * @param  argument: shared semaphore
  * @retval None
  */
static void SemaphoreThread2(void const *argument)
{
  uint32_t count = 0;
  osSemaphoreId semaphore = (osSemaphoreId) argument;

  for (;;)
  {
    if (semaphore != NULL)
    {
      /* Try to obtain the semaphore. */
      if (osSemaphoreWait(semaphore , 0) == osOK)
      {
        /* Resume Thread 1 (higher priority)*/
        osThreadResume(SemThread1Handle);

        count = osKernelSysTick() + 5000;

        /* Toggle LED2 every 200 ms for 5 seconds*/
        while (count >= osKernelSysTick())
        {
          BSP_LED_Toggle(LED2);

          osDelay(200);
        }

        /* Turn off LED2 */
        BSP_LED_Off(LED2);

        /* Release the semaphore to unblock Thread 1 (higher priority)  */
        osSemaphoreRelease(semaphore);
      }
    }
  }
}

/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow :
  *            System Clock source            = PLL (HSE)
  *            SYSCLK(Hz)                     = 72000000
  *            HCLK(Hz)                       = 72000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 2
  *            APB2 Prescaler                 = 1
  *            HSE Frequency(Hz)              = 8000000
  *            HSE PREDIV                     = 1
  *            PLLMUL                         = RCC_PLL_MUL9 (9)
  *            Flash Latency(WS)              = 2
  * @param  None
  * @retval None
  */
static void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_OscInitTypeDef RCC_OscInitStruct;

  /* Enable HSE Oscillator and activate PLL with HSE as source */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct)!= HAL_OK)
  {
    Error_Handler();
  }

  /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2
     clocks dividers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2)!= HAL_OK)
  {
    Error_Handler();
  }
}
/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
static void Error_Handler(void)
{
    /* Turn LED1 on */
    BSP_LED_On(LED1);
    while(1)
    {
    }
}

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
  ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {}
}
#endif


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
