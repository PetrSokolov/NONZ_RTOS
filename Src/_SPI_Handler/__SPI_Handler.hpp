//#####################################################################################
//	Файл				SPI_Handler.hpp
//	Название:		Обработчик запросов на обмен по SPI
//	Автор:			Бабкин.П.А
//	Вер. | гггг-ммм-дд |     	Кто		 		| Описание изменений
//	=====|=============|================|==============================================
// 	 1.0 | 2015 июнь 17 | Бабкин П.А	 		| Первый релиз
// 	-----|-------------|----------------|----------------------------------------------
//#####################################################################################
#ifndef SPIHANDLER_HPP
#define SPIHANDLER_HPP

#include "stm32f4xx_hal.h"
#include "stdint.h"
#include <list>
//#include "..\_Interfaces\__ISpiMessage.hpp"
#include "..\_Interfaces\__ISpiDmaExchange.hpp"

//#include "..\\_Interfaces\__IObservable.hpp"

using namespace std;


namespace src{	 

//-------------------------------------------------------------------------------------
//  Обработчик обмена по SPI.
//  При вызове метода sendMessage\recieveMessage, указатель на объект типа ISpiMessage
//  добавляется в соответствующий контейнер. После обработки удаляется.
//-------------------------------------------------------------------------------------
class SpiHandler : public ISpiDmaExchange{
  public:
    SpiHandler (SPI_HandleTypeDef* hspi, DMA_HandleTypeDef* hdma_spi_tx, DMA_HandleTypeDef* hdma_spi_rx, uint32_t spiClkFrequency)
      {
        _hspi = hspi; 
        _spiClkFrequency = spiClkFrequency;
        _hdma_spi_tx     = hdma_spi_tx;
        _hdma_spi_rx     = hdma_spi_rx;
        _currentMessage  = NULL;
        delaySpiOlegMode = 0;

        //  Отключение CS
        chipDeselect();

       // Бинарный семафор. Упорядочивает доступ к SPI
       osSemaphoreDef(Sem);
       _binarySem = osSemaphoreCreate(osSemaphore(Sem), 1);

        // Бинарный семафор. Обеспечивает задержку между байтами в режиме "Олег"
       osSemaphoreDef(SemOlegMode);
       _binarySemOlegMode = osSemaphoreCreate(osSemaphore(SemOlegMode), 1);

        //  Создание очереди на отсылку
       osMessageQDef(Queue, 16, ISpiMessage*);
       _queue = osMessageCreate(osMessageQ(Queue), NULL);

	   //  Очередь на передачу значения межбайтовой задержки в прерывание
	   osMessageQDef(QueuePicDelay, 16, uint16_t);
       _queuePicDelay = osMessageCreate(osMessageQ(QueuePicDelay), NULL);

      }
     
    ~SpiHandler()
      {
        osSemaphoreDelete (_binarySem);
        osSemaphoreDelete (_binarySemOlegMode);
      }
             
      
    //  Методы интерфейса ISpiDmaExchange
    virtual void transferMessage    (ISpiMessage* message);
  
    //  Методы класса
           void           handler               (void);
    inline osSemaphoreId  retSetmaphore         (void)  { return _binarySem; }         ///< Возвращает семафор.
    inline osSemaphoreId  retSetmaphoreOlegMode (void)  { return _binarySemOlegMode; } ///< Возвращает семафор.
    inline osMessageQId   retMessageQPicDelay   (void)  { return _queuePicDelay; }     ///< Возвращает очередь
    
    // Открытое поле данных класса
    uint32_t delaySpiOlegMode;                  //  Счетчик задержки между байтами

  private:
    virtual void    chipSelect        (uint8_t cs);            ///< Выбрать устройство на SPI шине
    virtual void    chipDeselect      (void);                  ///< Отменить выбор устройства на SPI шине
            uint8_t getReady          (void);                  ///< Возвращает занятость SPI
            void    setSpiFrequency   (ISpiMessage* message);  ///< Устанавливает делитель, соответствующий заданной скорости передачи
            void    setupDmaDataWidth (uint32_t dataWidth);    ///< Установка размера данных DMA-обмена
            void    setupSpi          (SpiMode mode, DataWidth dataWidth);          ///< Установка режима SPI

    SPI_HandleTypeDef* _hspi;              ///<  Указатель на структуру настроек HAL SPI
    DMA_HandleTypeDef* _hdma_spi_tx;       ///<  Указатель на структуру настроек DMA (spi tx channel)
    DMA_HandleTypeDef* _hdma_spi_rx;       ///<  Указатель на структуру настроек DMA (spi rx channel)
    uint32_t           _spiClkFrequency;   ///<  Частота синхронизации SPI [кГц](APB1, APB2)
    ISpiMessage*       _currentMessage;    ///<  Текущее сообщение
    osMessageQId       _queue;             ///<  Очередь сообщений (пакетов) на передачу
    osMessageQId       _queuePicDelay;     ///<  Очередь на передачу значения межбайтовой задержки в прерывание
    osSemaphoreId      _binarySem;         ///<  Упорядочивает доступ к SPI
    osSemaphoreId      _binarySemOlegMode; ///<  Бинарный семафор. Обеспечивает задержку между байтами в режиме "Олег"
};

//---------------------------------------------------------------------------------------------------------------------------4
// 
  class Spi2Handler : public SpiHandler{
  public:
    Spi2Handler (SPI_HandleTypeDef* hspi, DMA_HandleTypeDef* hdma_spi_tx, DMA_HandleTypeDef* hdma_spi_rx, uint32_t spiClkFrequency) : SpiHandler ( hspi, hdma_spi_tx, hdma_spi_rx, spiClkFrequency)
      {}
  
  protected:
    virtual void chipSelect (uint8_t cs);                         // Выбрать устройство на SPI шине
    virtual void chipDeselect (void);                             // Отменить выбор устройства на SPI шине
};
	

}	// namespace src

#endif	//define RC_MODULE_HPP
