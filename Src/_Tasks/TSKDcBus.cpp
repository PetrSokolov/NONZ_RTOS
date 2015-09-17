#include "..\_Objects_Definitions\__ObjectsDefinitions.hpp"
#include "..\_Objects_Definitions\__ConstantsDefinitions.hpp"

void StartTaskDcBus(void const * argument)
{
  //osEvent dcBusCommand;
  portBASE_TYPE xStatus;
  uint16_t dcBusCommand;
//  uint16_t Command =1;
  
  // Подключение DC-шины (отладочное)
//  osMessagePut(dcBus.retQueueDcBus(), CONNECT, 0);
//    uint16_t Command = CONNECT;

//    dcBus.setState(CONNECT);
//    xQueueSendToBack(dcBus.retQueueDcBus(), &dcBus.getState(), 0);
  dcBus.setCommand(CONNECT);
  
  for(;;)
  {
    //dcBusCommand = osMessageGet(dcBus.retQueueDcBus(), osWaitForever);  
    xStatus = xQueueReceive(dcBus.retQueueDcBus(), &dcBusCommand, osWaitForever); 
//    printf("TSKdcBus start\n");
    
    if (xStatus == pdPASS) {
//      switch(dcBusCommand.value.v){
      switch(dcBusCommand){
        case DISCONNECT: { printf("DcBus Disconnect...\n"); dcBus.activate(false); printf("DcBus Disconnected?\n"); }break;
        case CONNECT:    { printf("DcBus Connect...\n");    dcBus.activate(true); printf("DcBus Connected?\n"); }break;
        case COMMAND1:   { printf("DcBus Command 1\n"); }break;
      }
    }
//    printf("TSKdcBus stop\n\n");
    /* Принудительный вызов планировщика, позволяет
    * выполняться другой задаче.
    * Переключение на другую задачу произойдет быстрее,
    * чем окончится текущий квант времени.
    taskYIELD();
     */
  }


}

