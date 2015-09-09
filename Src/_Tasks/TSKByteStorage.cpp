#include "..\_Objects_Definitions\__ObjectsDefinitions.hpp"
#include "..\_Objects_Definitions\__ConstantsDefinitions.hpp"

void StartTaskStCtrl(void const * argument)
{
  for(;;)
  {
    byteStorageController.handler();
//        osSemaphoreWait(binarySemTSK1000mls, osWaitForever);
  }


}

