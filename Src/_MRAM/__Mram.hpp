//#####################################################################################
//	Файл				Mram.hpp
//	Название:		Обработка MRAM памяти
//	Автор:			Бабкин.П.А
//	Вер. | гггг-ммм-дд |     	Кто		 		| Описание изменений
//	=====|=============|================|==============================================
// 	 1.0 | 2015 июль 15| Бабкин П.А	    | Первый релиз
// 	-----|-------------|----------------|----------------------------------------------
//#####################################################################################
#ifndef MRAM_HPP
#define MRAM_HPP


#include "stdint.h"
#include <list>
#include "..\_Interfaces\__IByteStorage.hpp"
#include "..\_Interfaces\__ISpiMessage.hpp"
#include "..\_Interfaces\__ISpiDmaExchange.hpp"
#include "..\_Objects_Definitions\__ConstantsDefinitions.hpp"

using namespace std;

namespace src{	 

/** \defgroup Non_volatile_storage Энергонезависимая память */
/** \defgroup MRAM Магниторезистивная память
    \ingroup Non_volatile_storage
    \brief Данный модуль содержит обработку энергонезависимой памяти
  
  По SPI обмен осуществляется через интерфейс ISpiMessage.
  @{
*/  

/// Коды команд MRAM
  enum MramCommandCodes{WREN =0x06, ///<Write Enable
                        WRDI =0x04, ///<Write Disable
                        RDSR =0x05, ///<Read Status Register
                        WRSR =0x01, ///<Write Status Register
                        READ =0x03, ///<Read Data Bytes
                        WRITE=0x02, ///<Write Data Bytes
                        SLEEP=0xB9, ///<Enter Sleep Mode
                        WAKE =0xAB  ///<Exit Sleep Mode
                       };
  
/**
  \brief Класс, описывающий работу с MRAM памятью

  В классе представлен интерфейс для работы с микросхемой MR25H256.
*/
class Mram : public IByteStorage{
  public:
/** \brief Конструктор 
    \param[in] chipSelect   Номер устройства на шине SPI
    \param[in] spiMode      Режим SPI
    \param[in] spiFrequency Частота SPI
    \param[in] spiHandler   Обработчик SPI посылок
*/
    Mram(uint8_t chipSelect, SpiMode spiMode, uint16_t spiFrequency, ISpiDmaExchange* spiHandler)
     {
       _chipSelect   = chipSelect;
       _spiMode      = spiMode;
       _spiFrequency = spiFrequency;
       _spiHandler   = spiHandler;
       
       // Бинарный семафор. Упорядочивает доступ к MRAM
       osSemaphoreDef(Sem);
       _binarySem = osSemaphoreCreate(osSemaphore(Sem), 1);
     }

    ~Mram()
      {
        osSemaphoreDelete (_binarySem);
      }

    // Методы интерфейса IByteStorage
    virtual void readBytes (uint32_t storageAdres, uint16_t* bufer, uint16_t size);	 ///< Чтение
    virtual void writeBytes(uint32_t storageAdres, uint16_t* bufer, uint16_t size);  ///< Запись
      
  private:
    void writeEnable(void);        /*!< \brief Разрешить запись */
    void writeDisable(void);       /*!< \brief Запретить запись */
    void readStatusRegister(void); /*!< \brief Прочитать регистр статуса */
    void writeStatusRegister(uint8_t statusRegister);/*!< \brief Записать регистр статуса \param[in] statusRegister Записываемое значение регистра*/
    bool readDataBytes (uint32_t memoryAdres, uint16_t* data, uint16_t size);     /*!< \brief Прочитать данные */
    bool writeDataBytes(uint32_t memoryAdres, uint16_t* data, uint16_t size);     /*!< \brief Записать данные */
    void enterSleepMode(void);     /*!< \brief Включить энергосберегающий режим */
    void exitSleepMode(void);      /*!< \brief Выключить энергосберегающий режим */
    void setWriteProtect(void);    /*!< \brief Установить запрет на запись ножкой /WP \warning В зависимости от проекта обозначить в реализации GPIO*/ 
    void resetWriteProtect(void);  /*!< \brief Снять запрет на запись ножкой /WP */
  
    ISpiDmaExchange* _spiHandler;    ///<  Агрегация обработчика задач на SPI
    uint8_t          _chipSelect;    ///<  Номер устройства на шине SPI
    SpiMode          _spiMode;       ///<  Режим SPI
    uint16_t         _spiFrequency;  ///<  Частота SPI
    uint8_t          _statusRegister;///<  Регистр статуса
    osSemaphoreId    _binarySem;     ///<  Упорядочивает доступ к MRAM
};
  
/**
 @}
*/
}	// namespace src
#endif	//define RC_HPP
