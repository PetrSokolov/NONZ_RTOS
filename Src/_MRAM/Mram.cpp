//#####################################################################################
//	Файл				Mram.cpp
//	Название:		Обработка MRAM памяти
//	Автор:			Бабкин.П.А
//	Вер. | гггг-ммм-дд |     	Кто		 		| Описание изменений
//	=====|=============|================|==============================================
// 	 1.0 | 2015 июль 15| Бабкин П.А	    | Первый релиз
// 	-----|-------------|----------------|----------------------------------------------
//#####################################################################################

#include "stdint.h"
#include "__Mram.hpp"
#include "..\_Objects_Definitions\__ObjectsDefinitions.hpp"
#include "..\_Interfaces\__IControlCommands.hpp"

using namespace src;

// Методы интерфейса IByteStorage
//-------------------------------------------------------------------------------------
void Mram::readBytes (uint32_t storageAdres, uint16_t* bufer, uint16_t size)
{
  readDataBytes(storageAdres, bufer, size);
}

//-------------------------------------------------------------------------------------
void Mram::writeBytes(uint32_t storageAdres, uint16_t* bufer, uint16_t size)
{
  writeDataBytes(storageAdres, bufer, size);
}

//-------------------------------------------------------------------------------------
/* Разрешить запись */
//The Write Enable (WREN) command sets the Write Enable Latch (WEL) bit in the status register to 1. The
//WEL bit must be set prior to writing in the status register or the memory.
void Mram::writeEnable(void)
{
 SpiPacket packet(1, 0, _chipSelect, _spiMode, MODE_8BIT, _spiFrequency);
  
  osSemaphoreWait(packet.retSetmaphore(), osWaitForever);
  packet.putTxByte (WREN);
  _spiHandler->transferMessage(&packet);
  // Ожидание конца передачи
  osSemaphoreWait(packet.retSetmaphore(), osWaitForever);
}
//-------------------------------------------------------------------------------------
/* Запретить запись */
void Mram::writeDisable(void)
{
 SpiPacket packet(1, 0, _chipSelect, _spiMode, MODE_8BIT, _spiFrequency);
  
  osSemaphoreWait(packet.retSetmaphore(), osWaitForever);
  packet.putTxByte (WRDI);
  _spiHandler->transferMessage(&packet);
  // Ожидание конца передачи
  osSemaphoreWait(packet.retSetmaphore(), osWaitForever);
}
//-------------------------------------------------------------------------------------
/* Прочитать регистр статуса */
void Mram::readStatusRegister(void)
{
 SpiPacket packet(1, 1, _chipSelect, _spiMode, MODE_8BIT, _spiFrequency);
  
  osSemaphoreWait(packet.retSetmaphore(), osWaitForever);
  packet.putTxByte (RDSR);
  _spiHandler->transferMessage(&packet);
  // Ожидание конца передачи
  osSemaphoreWait(packet.retSetmaphore(), osWaitForever);
  
  _statusRegister = packet.getPointerToRecievedBytes()[0];
}
//-------------------------------------------------------------------------------------
/* Записать регистр статуса */
void Mram::writeStatusRegister(uint8_t statusRegister)
{
 SpiPacket packet(2, 0, _chipSelect, _spiMode, MODE_8BIT, _spiFrequency);
  
  osSemaphoreWait(packet.retSetmaphore(), osWaitForever);
  packet.putTxByte (WRSR);
  packet.putTxByte (statusRegister);
  _spiHandler->transferMessage(&packet);
  // Ожидание конца передачи
  osSemaphoreWait(packet.retSetmaphore(), osWaitForever);
}
//-------------------------------------------------------------------------------------
/* Прочитать данные */
// добавить CRC
bool Mram::readDataBytes(uint32_t memoryAdres, uint16_t* data, uint16_t size)
{
 SpiPacket packet(3, size, _chipSelect, _spiMode, MODE_8BIT1_OTHER16BIT, _spiFrequency);
  
  osSemaphoreWait(packet.retSetmaphore(), osWaitForever);
  packet.putTxByte (READ);                       // READ MRAM
  packet.putTxByte ((memoryAdres & 0xFF00)>>8);  // ADR
  packet.putTxByte (memoryAdres & 0xFF);         // ADR
  _spiHandler->transferMessage(&packet);
  // Ожидание конца передачи
  osSemaphoreWait(packet.retSetmaphore(), osWaitForever);
  
  // DATA
  for(uint16_t index=0; index<size; index++){
    data[0] = ((uint16_t)packet.getPointerToRecievedBytes()[2*index]<<8)|packet.getPointerToRecievedBytes()[2*index+1];
    data++;
  }

  return true;
}
//-------------------------------------------------------------------------------------
/* Записать данные */
bool Mram::writeDataBytes(uint32_t memoryAdres, uint16_t* data, uint16_t size)
{
 SpiPacket packet(1+2+size*2, 0, _chipSelect, _spiMode, MODE_8BIT1_OTHER16BIT, _spiFrequency);
  
  osSemaphoreWait(packet.retSetmaphore(), osWaitForever);
  packet.putTxByte (WRITE);                      // READ MRAM
  packet.putTxByte ((memoryAdres & 0xFF00)>>8);  // ADR
  packet.putTxByte (memoryAdres & 0xFF);         // ADR

  // DATA
  for(uint16_t index=0; index<size; size++){
    packet.putTxByte ((*data)>>8);
    packet.putTxByte (*data & 0xFF);
    data++;
  }

  _spiHandler->transferMessage(&packet);

  // Ожидание конца передачи
  osSemaphoreWait(packet.retSetmaphore(), osWaitForever);
  return true;
}
//-------------------------------------------------------------------------------------
/* Включить энергосберегающий режим */
void Mram::enterSleepMode(void)
{
}
//-------------------------------------------------------------------------------------
/* Выключить энергосберегающий режим */
void Mram::exitSleepMode(void)
{
}
//-------------------------------------------------------------------------------------
/* Установить запрет на запись в статус-регистр. В зависимости от проекта обозначить в реализации GPIO*/ 
void Mram::setWriteProtect(void)
{
   GPIOB->BSRR = GPIO_PIN_8<<16;
}
//-------------------------------------------------------------------------------------
/* Снять запрет на запись в статус-регистр /WP */
void Mram::resetWriteProtect(void)
{
   GPIOB->BSRR = GPIO_PIN_8;
}
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------

