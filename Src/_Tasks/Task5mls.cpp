

#include "..\_Objects_Definitions\__ObjectsDefinitions.hpp"
#include "..\_Objects_Definitions\__ConstantsDefinitions.hpp"

void StartTask5mls(void const * argument)
{
  /* USER CODE BEGIN StartTask_2 */
  /* Infinite loop */
  for(;;)
  {
    osSemaphoreWait(binarySemTSK5mls, osWaitForever);
    printf("Task 2 begin\n");
    displayLed4Digit.display();
     printf("Task 2 end\n");
//    osDelay(5);


  }
  /* USER CODE END StartTask_2 */
}
