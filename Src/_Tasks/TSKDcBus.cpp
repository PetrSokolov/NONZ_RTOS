#include "..\_Objects_Definitions\__ObjectsDefinitions.hpp"
#include "..\_Objects_Definitions\__ConstantsDefinitions.hpp"

void StartTaskDcBus(void const * argument)
{
  osEvent dcBusCommand;
  
  for(;;)
  {
    dcBusCommand = osMessageGet(dcBus.retQueueDcBus(), osWaitForever);   
    switch(dcBusCommand.value.v){
      case DISCONNECT: { printf("DcBus Disconnect\n"); }break;
      case CONNECT:    { printf("DcBus Connect\n"); }break;
      case COMMAND1:   { printf("DcBus Command 1\n"); }break;
    }
    
//    dcBus.activate(true);

  }


}

