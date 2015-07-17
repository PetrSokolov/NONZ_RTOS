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
void Mram::readDataBytes(void)      
{
 SpiPacket packet(3, 8, _chipSelect, _spiMode, MODE_8BIT1_OTHER16BIT, _spiFrequency);
  
  osSemaphoreWait(packet.retSetmaphore(), osWaitForever);
  packet.putTxByte (READ);  // READ MRAM
  packet.putTxByte (0x00);   // ADR
  packet.putTxByte (0x00);   // ADR
  _spiHandler->transferMessage(&packet);
  // Ожидание конца передачи
  osSemaphoreWait(packet.retSetmaphore(), osWaitForever);
  packet.putTxByte (0x00);   // ADR
}
//-------------------------------------------------------------------------------------
/* Записать данные */
void Mram::writeDataBytes(void)
{
 SpiPacket packet(1+2+4, 0, _chipSelect, _spiMode, MODE_8BIT1_OTHER16BIT, _spiFrequency);
  
  osSemaphoreWait(packet.retSetmaphore(), osWaitForever);
  packet.putTxByte (WRITE);  // READ MRAM
  packet.putTxByte (0x00);   // ADR
  packet.putTxByte (0x00);   // ADR
  packet.putTxByte (0x01);   // DATA
  packet.putTxByte (0x02);   // DATA
  packet.putTxByte (0x03);   // DATA
  packet.putTxByte (0x04);   // DATA
  _spiHandler->transferMessage(&packet);
  // Ожидание конца передачи
  osSemaphoreWait(packet.retSetmaphore(), osWaitForever);
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

