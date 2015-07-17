//#####################################################################################
//	Файл				SPI_Port_PIC.hpp
//	Название:		SPI порт, реализованный на микроконтроллере PIC.
//	Автор:			Бабкин.П.А
//	Вер. | гггг-ммм-дд |     	Кто		 		| Описание изменений
//	=====|=============|================|==============================================
// 	 1.0 | 2015 июнь 23 | Бабкин П.А	 		| Первый релиз
// 	-----|-------------|----------------|----------------------------------------------
//#####################################################################################
#ifndef SPIPORTPIC_HPP
#define SPIPORTPIC_HPP

#include "stm32f4xx_hal.h"
#include "stdint.h"
#include "..\_Interfaces\__ISpiMessage.hpp"
#include "..\_Interfaces\__ISpiDmaExchange.hpp"


using namespace std;


namespace src{	 

//-------------------------------------------------------------------------------------
//  Обработчик обмена по SPI.
//  При вызове метода sendMessage\recieveMessage, указатель на объект типа ISpiMessage
//  добавляется в соответствующий контейнер. После обработки удаляется.
//-------------------------------------------------------------------------------------
class SpiPortPic{
  public:
    SpiPortPic(uint8_t chipSelect, SpiMode spiMode, uint16_t spiFrequency, ISpiDmaExchange* spiHandler, uint8_t portAData, uint8_t portBData, uint8_t portADir, uint8_t portBDir)
      {
        _chipSelect   = chipSelect;
        _spiMode      = spiMode;
        _spiFrequency = spiFrequency;
        _spiHandler   = spiHandler; 
        _portAData    = portAData;
        _portBData    = portBData;
        _portADir     = portADir;
        _portBDir     = portBDir;
      }

           void    init          (void);
    inline SpiMode getSpiMode    (void)  { return _spiMode; }
    inline uint8_t getChipSelect (void)  { return _chipSelect; }
           void    writeRegister (uint8_t  data, uint8_t reg);
           uint8_t readRegister  (uint8_t& data, uint8_t reg);

  private:
    uint8_t  crcCalc (uint16_t lenth, uint8_t *data);    // Расчет CRC
    ISpiDmaExchange* _spiHandler;    //  Агрегация обработчика задач на SPI
    uint8_t  _chipSelect;
    SpiMode  _spiMode;
    uint16_t _spiFrequency;
    uint8_t   crc_calc(uint8_t len, uint8_t* data);
    uint8_t  _portAData;
    uint8_t  _portBData;
    uint8_t  _portADir;
    uint8_t  _portBDir;
};


}	// namespace src

#endif
