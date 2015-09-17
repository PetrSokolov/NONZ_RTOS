

#include "..\_Objects_Definitions\__ObjectsDefinitions.hpp"
#include "..\_Objects_Definitions\__ConstantsDefinitions.hpp"

#include "__Task3.hpp"


void StartTaskRC(void const * argument)
{
  // USER CODE BEGIN StartTask03
  // Infinite loop
  osEvent rc_time;
  for(;;)
  {
    rc_time = osMessageGet(queueRc, osWaitForever);
    rc.rcDecoder.putSample(rc_time.value.v);
  }
  // USER CODE END StartTask03
}
