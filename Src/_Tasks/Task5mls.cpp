

#include "..\_Objects_Definitions\__ObjectsDefinitions.hpp"
#include "..\_Objects_Definitions\__ConstantsDefinitions.hpp"

void StartTask5mls(void const * argument)
{
  /* USER CODE BEGIN StartTask_2 */
  /* Infinite loop */
  for(;;)
  {
    osSemaphoreWait(binarySemTSK5mls, osWaitForever);
    displayLed4Digit.display();


  }
  /* USER CODE END StartTask_2 */
}
