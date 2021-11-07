//zadanie c.5 Horka Forintova

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "usart.h"
#include "gpio.h"

char check1[4] = "ledO";
char on[4] = {'O', 'N', '\n'};
char off[5] = {'O', 'F', 'F', '\n'};
uint8_t led = 0;


void SystemClock_Config(void);

void process_serial_data(uint8_t ch);

int main(void)
{

  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_SYSCFG);
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);

  NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);

  SystemClock_Config();

  MX_GPIO_Init();
  MX_USART2_UART_Init();

  USART2_RegisterCallback(process_serial_data);


  while (1)
  {
	  if(led == 1)
	  {
		  for(int i=0; i<3;i++)
		  {
		  LL_USART_TransmitData8(USART2, on[i]);
		  LL_mDelay(100);
		  }
	  }

	  else if(led == 0)
	  {
		  for(int i=0; i<4;i++){
		  LL_USART_TransmitData8(USART2, off[i]);
		  LL_mDelay(100);
		  }
	  }
	  LL_USART_TransmitData8(USART2, '\r');
	  LL_mDelay(5000); //5s delay
  }
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  LL_FLASH_SetLatency(LL_FLASH_LATENCY_0);

  if(LL_FLASH_GetLatency() != LL_FLASH_LATENCY_0)
  {
  Error_Handler();  
  }
  LL_RCC_HSI_Enable();

   /* Wait till HSI is ready */
  while(LL_RCC_HSI_IsReady() != 1)
  {
    
  }
  LL_RCC_HSI_SetCalibTrimming(16);
  LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_1);
  LL_RCC_SetAPB2Prescaler(LL_RCC_APB1_DIV_1);
  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_HSI);

   /* Wait till System clock is ready */
  while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_HSI)
  {
  
  }
  LL_Init1msTick(8000000);
  LL_SYSTICK_SetClkSource(LL_SYSTICK_CLKSOURCE_HCLK);
  LL_SetSystemCoreClock(8000000);
}

void process_serial_data(uint8_t ch)
{
	static uint8_t count1 = 0;
	static uint8_t count2 = 0;


	if(ch == check1[count1])
	{
		count1++;
	}

	else if((ch == 'N') && (count1 == 4))
	{
		LL_GPIO_SetOutputPin(GPIOB, LL_GPIO_PIN_3);
		led = 1;
		count1 = 0;
		count2 = 0;
	}

	else if((ch == 'F') && (count1 >= 4))
	{
		count2++;

		if(count2 == 2)
		{
			LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_3);
			led = 0;
			count1 = 0;
			count2 = 0;
		}

		if((count2 == 1) && (ch != 'F'))
		{
			count1=0;
			count2=0;

			if(ch == check1[count1])
			{
				count1++;
			}
			return;
		}
	}

	else if(count1 >= 1 && ((ch == '\n')||(ch == '\r')||(ch == ' ')))
	{}

	else
	{
		count1=0;

		if(ch == check1[count1])
		{
			count1++;
		}
		return;
	}
}


/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(char *file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
