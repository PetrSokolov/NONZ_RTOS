/**
  ******************************************************************************
  * File Name          : main.c
  * Date               : 26/06/2015 16:45:03
  * Description        : Main program body
  ******************************************************************************
  *
  * COPYRIGHT(c) 2015 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "cmsis_os.h"
#include "adc.h"
#include "dma.h"
#include "i2c.h"
#include "spi.h"
#include "tim.h"
#include "usb_otg.h"
#include "gpio.h"

/* USER CODE BEGIN Includes */
#include "_Objects_Definitions\__ObjectsDefinitions.hpp"
#include "_Objects_Definitions\__ConstantsDefinitions.hpp"
#include "main.hpp"
/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void MX_FREERTOS_Init(void);

/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

int main(void)


{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration----------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* Configure the system clock */
  SystemClock_Config();

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_ADC1_Init();
  MX_ADC3_Init();
  MX_I2C1_Init();
  MX_SPI1_Init();
  MX_SPI2_Init();
  MX_TIM1_Init();
  MX_TIM2_Init();
  MX_USB_OTG_FS_PCD_Init();

  /* USER CODE BEGIN 2 */
  TIM1->CCR1 = 0x300;
  TIM1->CCR2 = 0x300;
  TIM2->CCR2 = 0x60;
  
  // Настройка deathTime
  pwm.init();
/* bugfix */   pwmDeathTime.setValue(10);
  htim1.Instance->BDTR &= ~TIM_BDTR_DTG;
  htim1.Instance->BDTR |= pwm.computeDeathTime(pwmDeathTime.getValueFlt());
  
  HAL_TIM_Base_Start(&htim1);
  // Выходной сигнал для ацп
  HAL_TIM_OC_Start(&htim2, TIM_CHANNEL_2);
  // Выходной сигнал для ацп
  HAL_TIM_OC_Start(&htim1, TIM_CHANNEL_3);
  
//  HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_1);
//  HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_2);
//  pwm.start();

//  HAL_TIMEx_PWMN_Start(&htim1,TIM_CHANNEL_1);
//  HAL_TIMEx_PWMN_Start(&htim1,TIM_CHANNEL_2);

  __HAL_RCC_DMA2_CLK_ENABLE();
  HAL_ADC_Start_DMA(&hadc1, (uint32_t*)adc1.getBufer(), adc1.getBuferSize());
  HAL_ADC_Start_DMA(&hadc3, (uint32_t*)adc3.getBufer(), adc3.getBuferSize());
  


//------------------------------------------------------------------------------
  //  Низкоуровневая инициализация
//------------------------------------------------------------------------------
  //  Разрешение выходов буферов
  GPIOC->BSRR = BIT_14;

  //  Инициализация SPI портов
  

//------------------------------------------------------------------------------
//  Агрегация объектов
//  Определяется только при инициализации программы
//------------------------------------------------------------------------------
  
  mainMenu.addObserver( &menuEngine );  //  Объект menuEngine подписался на рассылку событий, объявленных в IControlCommands
  
//------------------------------------------------------------------------------
//  Начальные условия
//------------------------------------------------------------------------------
  menuEngine.setMenuValue("");
 
// Затычка на время отсутствия FRAM. Инициализация float данных

  vICalibrating.setValue(vICalibrating.getValue());
  vUDcBusCodeUCal.setValue(vUDcBusCodeUCal.getValue());
  vUDcBusCodeZero.setValue(vUDcBusCodeZero.getValue());
  vIChargeCodeICal.setValue(vIChargeCodeICal.getValue());
  vUChargeCodeUCal.setValue(vUChargeCodeUCal.getValue());
  vIChargeCodeZero.setValue(vIChargeCodeZero.getValue());
  vUChargeCodeZero.setValue(vUChargeCodeZero.getValue());
  vDcBusLoadVoltageDifferent.setValue(vDcBusLoadVoltageDifferent.getValue());

  /* USER CODE END 2 */

  /* Call init function for freertos objects (in freertos.c) */
  MX_FREERTOS_Init();
  
  /* Start scheduler */
  osKernelStart();
  
  /* We should never get here as control is now taken by the scheduler */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
  /* USER CODE END WHILE */

  /* USER CODE BEGIN 3 */

  }
  /* USER CODE END 3 */

}

/** System Clock Configuration
*/
void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;

  __PWR_CLK_ENABLE();

  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 25;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  HAL_RCC_OscConfig(&RCC_OscInitStruct);

  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_SYSCLK|RCC_CLOCKTYPE_PCLK1
                              |RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;
  HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5);

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

#ifdef USE_FULL_ASSERT

/**
   * @brief Reports the name of the source file and the source line number
   * where the assert_param error has occurred.
   * @param file: pointer to the source file name
   * @param line: assert_param error line source number
   * @retval None
   */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
    ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */

}

#endif

/**
  * @}
  */ 

/**
  * @}
*/ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
