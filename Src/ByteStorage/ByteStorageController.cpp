//#####################################################################################
//	Файл				ByteStorageController.cpp
//	Название:		Контроллер, обрабатывающий пакеты типа IByteStorageExchange
//              Передает их хранилищу по интерфейсу IByteStorage
//	Автор:			Бабкин.П.А
//	Вер. | гггг-ммм-дд |     	Кто		 		| Описание изменений
//	=====|=============|================|==============================================
// 	 1.0 | 2015 сент 08| Бабкин П.А	    | Первый релиз
// 	-----|-------------|----------------|----------------------------------------------
//#####################################################################################

#include "stdio.h"
#include "__ByteStorageController.hpp"

using namespace src;
extern osThreadId TaskStCtrlHandle;


// Интерфейс IByteStorageExchange
//-------------------------------------------------------------------------------------
// Метод интерфейса. Вызывается по событию появления пакета в очереди
void ByteStorageController::transferPacket (IByteStoragePacket* packet)
{
  printf("Byte Storage Packet send in mail\n");
  xQueueSendToBack(_mail, packet, osWaitForever);
//  switch(packet->getAction()){
//    case ACTION_READ:  {_storage->readBytes(packet->getStorageAdres(), packet->getClientBufer(), packet->getSize()); }break;
//    case ACTION_WRITE: {_storage->writeBytes(packet->getStorageAdres(), packet->getClientBufer(), packet->getSize()); }break;
//    default: /*ERROR*/break;
//  }
}
//-------------------------------------------------------------------------------------
void ByteStorageController::handler(void)
{
  portBASE_TYPE xStatus;
  //  Ожидание пакета на передачу
  xStatus = xQueueReceive(_mail, &_currentPacket, osWaitForever);
  if (xStatus == pdPASS) {
    printf("Byte Storage mail recieved %d\n", (uint32_t)_currentPacket);
    //_currentPacket
    //  Освобождение семафора текущего пакета
    osSemaphoreRelease(_currentPacket->retSetmaphore());
    printf("Semaphore released %d\n", (uint32_t)_currentPacket);
//    printf("FreeHeapSize = %d\n", xPortGetFreeHeapSize());
//    printf("MinimumEverFreeHeapSize = %d\n", xPortGetMinimumEverFreeHeapSize());
//    printf("%s High Watermark = %d\n", pcTaskGetTaskName(TaskStCtrlHandle), uxTaskGetStackHighWaterMark(TaskStCtrlHandle));

  }
  
  

}
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------

