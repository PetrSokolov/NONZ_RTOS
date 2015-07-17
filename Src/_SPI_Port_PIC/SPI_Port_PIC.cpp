//#####################################################################################
//	Файл				SPI_Port_PIC.cpp
//	Название:		SPI порт, реализованный на микроконтроллере PIC.
//	Автор:			Бабкин.П.А
//	Вер. | гггг-ммм-дд |     	Кто		 		| Описание изменений
//	=====|=============|================|==============================================
// 	 1.0 | 2015 июнь 23 | Бабкин П.А	 		| Первый релиз
// 	-----|-------------|----------------|----------------------------------------------
//#####################################################################################

#include "stdint.h"
#include "__SPI_Port_PIC.hpp"
#include "..\_Objects_Definitions\__ObjectsDefinitions.hpp"
#include "..\_Objects_Definitions\__ConstantsDefinitions.hpp"


using namespace src;

void SpiPortPic::init (void)
{
//SpiPacket(uint16_t sizeBuferTx, uint16_t sizeBuferRx, uint8_t chipSelect, uint8_t spiMode, uint16_t spiFrequency)
  SpiPacket packet(4, 0, _chipSelect, _spiMode, MODE_8BIT, _spiFrequency);

  /* bugfix сделать разрешение буферов отдельно*/
  if (!(GPIOB->ODR & BIT_0)){
    GPIOB->BSRR = BIT_0;
    osDelay(1);
  }
  // Семафор у пакета забираю, Возвращается он в него после окончания отсылки
  osSemaphoreWait(packet.retSetmaphore(), osWaitForever);
    osDelay(1);
    packet.resetBuf();
    packet.setModeOleg(1);
    packet.putTxByte (0x40);  // Передача
    packet.putTxByte (0x12);  // Регистр GPIOA
    packet.putTxByte (_portAData);
    packet.putTxByte ( crcCalc (3, packet.getPointerToTransmit()) );
    _spiHandler->transferMessage(&packet);

  osSemaphoreWait(packet.retSetmaphore(), osWaitForever);
    osDelay(1);
    packet.resetBuf();
    packet.setModeOleg(1);
    packet.putTxByte (0x40);  // Передача
    packet.putTxByte (0x13);  // Регистр GPIOB
    packet.putTxByte (_portBData);
    packet.putTxByte ( crcCalc (3, packet.getPointerToTransmit()) );
    _spiHandler->transferMessage(&packet);

  osSemaphoreWait(packet.retSetmaphore(), osWaitForever);
    osDelay(1);
    packet.resetBuf();
    packet.setModeOleg(1);
    packet.putTxByte (0x40);  // Передача
    packet.putTxByte (0x14);  // Регистр OLATA
    packet.putTxByte (_portAData);
    packet.putTxByte ( crcCalc (3, packet.getPointerToTransmit()) );
    _spiHandler->transferMessage(&packet);
 
  osSemaphoreWait(packet.retSetmaphore(), osWaitForever);
    osDelay(1);
    packet.resetBuf();
    packet.setModeOleg(1);
    packet.putTxByte (0x40);  // Передача
    packet.putTxByte (0x15);  // Регистр OLATB
    packet.putTxByte (_portBData);
    packet.putTxByte ( crcCalc (3, packet.getPointerToTransmit()) );
    _spiHandler->transferMessage(&packet);

  osSemaphoreWait(packet.retSetmaphore(), osWaitForever);
    osDelay(1);
    packet.resetBuf();
    packet.setModeOleg(1);
    packet.putTxByte (0x40);  // Передача
    packet.putTxByte (0x00);  // Регистр DIRA
    packet.putTxByte (_portADir);
    packet.putTxByte ( crcCalc (3, packet.getPointerToTransmit()) );
    _spiHandler->transferMessage(&packet);

  osSemaphoreWait(packet.retSetmaphore(), osWaitForever);
    osDelay(1);
    packet.resetBuf();
    packet.setModeOleg(1);
    packet.putTxByte (0x40);  // Передача
    packet.putTxByte (0x01);  // Регистр DIRB
    packet.putTxByte (_portBDir);
    packet.putTxByte ( crcCalc (3, packet.getPointerToTransmit()) );
    _spiHandler->transferMessage(&packet);

  osSemaphoreWait(packet.retSetmaphore(), osWaitForever);// == pdTRUE){
  // Пакет отработан - можно удалять
}


//--------------------------------------------------------------------------------------------------------------------
//  Возвращает TRUE в случае успешного чтения
uint8_t SpiPortPic::readRegister  (uint8_t& data, uint8_t reg)
{
  uint8_t crc_;
  uint8_t rxMessage_[3];
  SpiPacket packet(2, 2, _chipSelect, _spiMode, MODE_8BIT, _spiFrequency);

  osSemaphoreWait(packet.retSetmaphore(), osWaitForever);
    packet.resetBuf();
    packet.setModeOleg(1);
    packet.putTxByte (0x41);  // Прием
    packet.putTxByte (reg);
    _spiHandler->transferMessage(&packet);
  osSemaphoreWait(packet.retSetmaphore(), osWaitForever);// == pdTRUE){
  // Пакет отработан - можно удалять
  rxMessage_[0] = packet.getPointerToTransmit()[0];
  rxMessage_[1] = packet.getPointerToTransmit()[1];
  rxMessage_[2] = packet.getPointerToRecieve()[2];
  crc_ = crcCalc (3, rxMessage_);
  if (crc_ == packet.getPointerToRecieve()[3]){
    data = packet.getPointerToRecieve()[2];
    return pdTRUE;
  }
   else{ return pdFALSE; }
}


//--------------------------------------------------------------------------------------------------------------------
void    SpiPortPic::writeRegister (uint8_t data, uint8_t reg)
{
  SpiPacket packet(4, 0, _chipSelect, _spiMode, MODE_8BIT, _spiFrequency);

  osSemaphoreWait(packet.retSetmaphore(), osWaitForever);
    packet.resetBuf();
    packet.setModeOleg(1);
    packet.putTxByte (0x40);  // Запись
    packet.putTxByte (reg);
    packet.putTxByte (data);
    packet.putTxByte ( crcCalc (3, packet.getPointerToTransmit()) );
    _spiHandler->transferMessage(&packet);
  osSemaphoreWait(packet.retSetmaphore(), osWaitForever);// == pdTRUE){
  // Пакет отработан - можно удалять
}



//---------------------------------------------------------------------------------------------------------------------
// Расчет CRC
uint8_t SpiPortPic::crcCalc (uint16_t lenth, uint8_t *data)
{
/*   Подпрограмма подсчета сигнатуры.
 Вход - данные в ячейке DSIG.
 Выход - результат в ячейке SIGS.*/
  uint16_t i_;
  uint16_t crc_=0xAA;
  uint16_t temp_;
  
  for(i_=0; i_<lenth; i_++){
    crc_^= 0xFF;  // Инвертирую данные
    crc_ <<= 1;
    crc_ |= 0x0001;
    temp_ = crc_;
    crc_ ^= data[i_];
    if ((temp_ & 0x100) != 0){
      crc_ ^= 0x65;
    }
    crc_ ^= 0xFF;  // Инвертирую данные
    crc_=crc_ & 0xff;
//    data++;
  }
return (uint8_t)crc_;
}

