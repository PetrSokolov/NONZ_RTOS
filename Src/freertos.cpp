/**
  ******************************************************************************
  * File Name          : freertos.c
  * Date               : 26/06/2015 16:44:59
  * Description        : Code for freertos applications
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
#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os.h"

/* USER CODE BEGIN Includes */     
#include "stm32f4xx_hal.h"
#include ".\_Objects_Definitions\__ConstantsDefinitions.hpp"

/* USER CODE END Includes */

/* Variables -----------------------------------------------------------------*/
osThreadId Task500mlsHandle;
osThreadId Task1000mlsHandle;
osThreadId Task5mlsHandle;
osThreadId TaskRCHandle;
osThreadId TaskSpi1Handle;
osThreadId TaskSpi2Handle;
osMessageQId queueRc;

osTimerId Timer5mlsHandle;
osTimerId Timer500mlsHandle;
osTimerId Timer1000mlsHandle;
osTimerId myTimer02Handle;
osTimerId myTimer03Handle;
osTimerId myOnceTimer03Handle;
osMutexId Mutex01Handle;
osMutexId Mutex02Handle;
osSemaphoreId binarySemSpiDelayOlegMode;
osSemaphoreId binarySemTSK5mls;
osSemaphoreId binarySemTSK500mls;
osSemaphoreId binarySemTSK1000mls;
osSemaphoreId myCountingSem01Handle;
osSemaphoreId myCountingSem02Handle;

/* USER CODE BEGIN Variables */

/* USER CODE END Variables */

/* Function prototypes -------------------------------------------------------*/
void StartTask500mls(void const * argument);
void StartTask1000mls(void const * argument);
void StartTask5mls(void const * argument);
void StartTaskRC(void const * argument);
void StartTaskSpi1(void const * argument);
void StartTaskSpi2(void const * argument);
void Timer5mlsCallback(void const * argument);
void Timer500mlsCallback(void const * argument);
void Timer1000mlsCallback(void const * argument);
void Timer2Callback02(void const * argument);
void Timer3Callback03(void const * argument);
void OnceCallback03(void const * argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */
/* Hook prototypes */
#ifdef __cplusplus
 extern "C" {
#endif 

void vApplicationStackOverflowHook(xTaskHandle xTask, signed char *pcTaskName);
void vApplicationMallocFailedHook(void);

#ifdef __cplusplus
 };
#endif 

   
/* USER CODE BEGIN 4 */
void vApplicationStackOverflowHook(xTaskHandle xTask, signed char *pcTaskName)
{
   /* Run time stack overflow checking is performed if
   configCHECK_FOR_STACK_OVERFLOW is defined to 1 or 2. This hook function is
   called if a stack overflow is detected. */
//  printf("Stack Overflow Task-%s\n", pcTaskName);
}
/* USER CODE END 4 */

/* USER CODE BEGIN 5 */
void vApplicationMallocFailedHook(void)
{
   /* vApplicationMallocFailedHook() will only be called if
   configUSE_MALLOC_FAILED_HOOK is set to 1 in FreeRTOSConfig.h. It is a hook
   function that will get called if a call to pvPortMalloc() fails.
   pvPortMalloc() is called internally by the kernel whenever a task, queue,
   timer or semaphore is created. It is also called by various parts of the
   demo application. If heap_1.c or heap_2.c are used, then the size of the
   heap available to pvPortMalloc() is defined by configTOTAL_HEAP_SIZE in
   FreeRTOSConfig.h, and the xPortGetFreeHeapSize() API function can be used
   to query the size of free heap space that remains (although it does not
   provide information on how the remaining heap might be fragmented). */
//  printf("Malloc Failed\n");

}


/* Init FreeRTOS */

void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */
       
  /* USER CODE END Init */

  /* Create the mutex(es) */
  /* definition and creation of Mutex01 */
  osMutexDef(Mutex01);
  Mutex01Handle = osMutexCreate(osMutex(Mutex01));

  /* definition and creation of Mutex02 */
  osMutexDef(Mutex02);
  Mutex02Handle = osMutexCreate(osMutex(Mutex02));

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */
//-------------------------------------------------------------------------------------------------
  /* Create the semaphores(s) */
  /* definition and creation of binarySemSpiDelayOlegMode */
//  osSemaphoreDef(binarySemSpiDelay);
//  binarySemSpiDelayOlegMode = osSemaphoreCreate(osSemaphore(binarySemSpiDelay), 1);

  /* definition and creation of BinarySem */
  osSemaphoreDef(Sem5mls);
  binarySemTSK5mls = osSemaphoreCreate(osSemaphore(Sem5mls), 1);

  /* definition and creation of BinarySem */
  osSemaphoreDef(Sem500mls);
  binarySemTSK500mls = osSemaphoreCreate(osSemaphore(Sem500mls), 1);

  /* definition and creation of BinarySem */
  osSemaphoreDef(Sem1000mls);
  binarySemTSK1000mls = osSemaphoreCreate(osSemaphore(Sem1000mls), 1);

  /* definition and creation of myCountingSem01 */
  osSemaphoreDef(myCountingSem01);
  myCountingSem01Handle = osSemaphoreCreate(osSemaphore(myCountingSem01), 2);

  /* definition and creation of myCountingSem02 */
  osSemaphoreDef(myCountingSem02);
  myCountingSem02Handle = osSemaphoreCreate(osSemaphore(myCountingSem02), 2);

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

//-------------------------------------------------------------------------------------------------
  /* Create the timer(s) */
  /* definition and creation of Timer500mls */
  osTimerDef(Timer5mls, Timer5mlsCallback);
  Timer5mlsHandle = osTimerCreate(osTimer(Timer5mls), osTimerPeriodic, NULL);

  /* definition and creation of Timer500mls */
  osTimerDef(Timer500mls, Timer500mlsCallback);
  Timer500mlsHandle = osTimerCreate(osTimer(Timer500mls), osTimerPeriodic, NULL);

  /* definition and creation of Timer1000mls */
  osTimerDef(Timer1000mls, Timer1000mlsCallback);
  Timer1000mlsHandle = osTimerCreate(osTimer(Timer1000mls), osTimerPeriodic, NULL);

  /* definition and creation of myTimer02 */
  osTimerDef(myTimer02, Timer2Callback02);
  myTimer02Handle = osTimerCreate(osTimer(myTimer02), osTimerPeriodic, NULL);

  /* definition and creation of myTimer03 */
  osTimerDef(myTimer03, Timer3Callback03);
  myTimer03Handle = osTimerCreate(osTimer(myTimer03), osTimerPeriodic, NULL);

/* definition and creation of myOnceTimer03 */
  osTimerDef(myOnceTimer03, OnceCallback03);
  myOnceTimer03Handle = osTimerCreate(osTimer(myOnceTimer03), osTimerOnce, NULL);

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */


//-------------------------------------------------------------------------------------------------
  /* Create the thread(s) */
  /* definition and creation of Task1 */
  osThreadDef(Task500mls, StartTask500mls, osPriorityNormal, 0, 512);
  Task500mlsHandle = osThreadCreate(osThread(Task500mls), NULL);

  /* definition and creation of Task1 */
  osThreadDef(Task1000mls, StartTask1000mls, osPriorityNormal, 0, 512);
  Task1000mlsHandle = osThreadCreate(osThread(Task1000mls), NULL);

  /* definition and creation of Task_2 */
  osThreadDef(Task5mls, StartTask5mls, osPriorityNormal, 0, 256);
  Task5mlsHandle = osThreadCreate(osThread(Task5mls), NULL);

  /* definition and creation of Task03 */
  osThreadDef(TaskRC, StartTaskRC, osPriorityLow, 0, 256);
  TaskRCHandle = osThreadCreate(osThread(TaskRC), NULL);

  osThreadDef(TaskSpi1, StartTaskSpi1, osPriorityNormal, 0, 256);
  TaskSpi1Handle = osThreadCreate(osThread(TaskSpi1), NULL);

  osThreadDef(TaskSpi2, StartTaskSpi2, osPriorityNormal, 0, 256);
  TaskSpi2Handle = osThreadCreate(osThread(TaskSpi2), NULL);
  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */


//-------------------------------------------------------------------------------------------------
  /* Create the queue(s) */

  /* definition and creation of myQueue02 */
  osMessageQDef(QueueRc, 16, uint16_t);
  queueRc = osMessageCreate(osMessageQ(QueueRc), NULL);

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */
  
  // Start OS Timers
  osTimerStart (Timer5mlsHandle, 5); 
  osTimerStart (Timer500mlsHandle, 300); 
  osTimerStart (Timer1000mlsHandle, 1000); 
  osTimerStart (myTimer02Handle, 100); 
  osTimerStart (myTimer03Handle, 500); 

}

/* StartDefaultTask1 function */
/*void StartDefaultTask1(void const * argument)
{

  // USER CODE BEGIN StartDefaultTask1 
  // Infinite loop 
  for(;;)
  {
    GPIOC->BSRR = BIT_14;
//    GPIOD->BSRR = (BIT_8|BIT_9|BIT_10|BIT_11|BIT_12); // Управляющие пины
    GPIOD->BSRR = (BIT_8); // Управляющие пины
    GPIOD->BSRR = ((BIT_0|BIT_1|BIT_2|BIT_3|BIT_4|BIT_5|BIT_6|BIT_7) & 0xFF)<<16; // 7 сегментов + точка
    osDelay(500);
    GPIOD->BSRR = 0x52;
    GPIOD->BSRR = BIT_8<<16;
    osDelay(500);
  }
  // USER CODE END StartDefaultTask1 
}*/

/* StartTask_2 function */
/*void StartTask_2(void const * argument)
{
  // USER CODE BEGIN StartTask_2 
  // Infinite loop 
  for(;;)
  {
//    GPIOD->BSRR = (BIT_8|BIT_9|BIT_10|BIT_11|BIT_12); // Управляющие пины
//    GPIOD->BSRR = ((BIT_0|BIT_1|BIT_2|BIT_3|BIT_4|BIT_5|BIT_6|BIT_7) & 0xFF)<<16; // 7 сегментов + точка
    osDelay(300);
//    GPIOD->BSRR = 0x53;
    GPIOD->BSRR = BIT_9<<16;
    osDelay(300);
    GPIOD->BSRR = BIT_10<<16;
  }
  // USER CODE END StartTask_2 
}
*/
/* StartTask03 function */
/*void StartTask03(void const * argument)
{
  // USER CODE BEGIN StartTask03
  // Infinite loop
  for(;;)
  {
    osDelay(1);
  }
  // USER CODE END StartTask03
}
*/

/* Timer5mlsCallback function */
void Timer5mlsCallback(void const * argument)
{
  osSemaphoreRelease(binarySemTSK5mls);
}

/* Timer500mlsCallback function */
void Timer500mlsCallback(void const * argument)
{
  osSemaphoreRelease(binarySemTSK500mls);
}

/* Timer1000mlsCallback function */
void Timer1000mlsCallback(void const * argument)
{
//  osSemaphoreRelease(binarySemTSK1000mls);
}

/* Timer2Callback02 function */
void Timer2Callback02(void const * argument)
{
  /* USER CODE BEGIN Timer2Callback02 */
  
  /* USER CODE END Timer2Callback02 */
}

/* Timer3Callback03 function */
void Timer3Callback03(void const * argument)
{
  /* USER CODE BEGIN Timer2Callback02 */
  
  /* USER CODE END Timer2Callback02 */
}

/* OnceCallback03 function */
void OnceCallback03(void const * argument)
{
  /* USER CODE BEGIN OnceCallback03 */
  
  /* USER CODE END OnceCallback03 */
}

/* USER CODE BEGIN Application */
     
/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
