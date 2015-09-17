//#####################################################################################
//	Файл				SPI_Handler.cpp
//	Название:		Обработчик запросов на обмен по SPI
//	Автор:			Бабкин.П.А
//	Вер. | гггг-ммм-дд |     	Кто		 		| Описание изменений
//	=====|=============|================|==============================================
// 	 1.0 | 2015 июнь 17 | Бабкин П.А	 		| Первый релиз
// 	-----|-------------|----------------|----------------------------------------------
//#####################################################################################

#include "stdint.h"
#include "__SPI_Handler.hpp"
#include "..\_Objects_Definitions\__ObjectsDefinitions.hpp"
#include "..\_Objects_Definitions\__ConstantsDefinitions.hpp"


using namespace src;

//  Добавление сообщения в список на передачу
void SpiHandler::transferMessage  (ISpiMessage* message)
{
  osMessagePut(_queue, (uint32_t)message, 2/*osWaitForever*/);
}
  
//  Возвращает занятость SPI
uint8_t SpiHandler::getReady (void)
{
 return (HAL_SPI_GetState(_hspi) == HAL_SPI_STATE_READY );
}


// Выбрать устройство на SPI шине
void SpiHandler::chipSelect (uint8_t cs)
{
  cs &= 0xF;
  GPIOE->BSRR = (0xF-cs)<<16;
}


// Снять выбор устройства на SPI шине
void SpiHandler::chipDeselect (void)
{
  GPIOE->BSRR = (BIT_0|BIT_1|BIT_2|BIT_3);
}

//  Удаление сообщения из списока на прием
void SpiHandler::handler (void)
{
// Потом убрать эти промежуточные данные
  uint16_t  sizeOfTranscieve_;
  uint8_t* pointerToTransmit_;
  uint8_t* pointerToRecieve_;

  osEvent mesage_;
  
  //  Ожидание пакета на передачу
  mesage_ = osMessageGet(_queue, osWaitForever);
//    printf("TSKspi start\n");
  if (mesage_.status == osEventMessage){

    _currentMessage = (ISpiMessage*)mesage_.value.p;
  
    setSpiFrequency(_currentMessage);
    sizeOfTranscieve_    = _currentMessage->getSizeOfTranscieve();
    pointerToTransmit_ = _currentMessage->getPointerToTransmit();
    pointerToRecieve_ = _currentMessage->getPointerToRecieve();

    // Захват ресурса SPI
    osSemaphoreWait(_binarySem, osWaitForever);

    //  Установка режима SPI в соответствии с требованиями передаваемого пакета
//    setupSpiMode(_currentMessage->getSpiMode(), WIDTH_8BIT);
    
//    chipSelect(_currentMessage->getChipSelect());

    if (!_currentMessage->getModeOleg()){
      // Здесь ращзместить блок раздельной отсылки 8+16 бит
      switch (_currentMessage->getTransferMode()){
        case MODE_8BIT:
              {
                setupSpi(_currentMessage->getSpiMode(), WIDTH_8BIT);
                setupDmaDataWidth(WIDTH_8BIT);
                chipSelect(_currentMessage->getChipSelect());
                HAL_SPI_TransmitReceive_DMA(_hspi, pointerToTransmit_, pointerToRecieve_, sizeOfTranscieve_); 
              }break;
        case MODE_8BIT1_OTHER16BIT:
              {
                setupSpi(_currentMessage->getSpiMode(), WIDTH_8BIT);
                setupDmaDataWidth(WIDTH_8BIT);
                chipSelect(_currentMessage->getChipSelect());
                
                HAL_SPI_TransmitReceive_DMA(_hspi, pointerToTransmit_, pointerToRecieve_, 1); 

                osSemaphoreWait(_binarySem, osWaitForever); // Ожидание конца передачи
                setupSpi(_currentMessage->getSpiMode(), WIDTH_16BIT);
                setupDmaDataWidth(WIDTH_16BIT);
                HAL_SPI_TransmitReceive_DMA(_hspi, pointerToTransmit_+1, pointerToRecieve_+1, (sizeOfTranscieve_-1)>>1); 

              }break;
        case MODE_16BIT:
              {
                setupSpi(_currentMessage->getSpiMode(), WIDTH_16BIT);
                setupDmaDataWidth(WIDTH_16BIT);
                chipSelect(_currentMessage->getChipSelect());
                HAL_SPI_TransmitReceive_DMA(_hspi, pointerToTransmit_, pointerToRecieve_, sizeOfTranscieve_>>1); 
              }break;
      }
    }
     else{  // Режим "Олег"
       setupSpi(_currentMessage->getSpiMode(), WIDTH_8BIT);
//       setupDmaDataWidth(WIDTH_8BIT);
       chipSelect(_currentMessage->getChipSelect());

       for (uint16_t i =0; i<sizeOfTranscieve_; i++){
         if(i == (sizeOfTranscieve_-1)){
           _currentMessage->setModeOleg(0); // Передается последний байт
         }
         HAL_SPI_TransmitReceive_DMA(_hspi, pointerToTransmit_++, pointerToRecieve_++, 1);
         osSemaphoreWait(_binarySemOlegMode, 0);
          osMessagePut(_queuePicDelay, 3, osWaitForever);
         osSemaphoreWait(_binarySemOlegMode, 1);
       }
     }

    //  Ожидание завершения работы ресурса (SPI)
    //  Ожидание необходимо, так как DMA использует источник данных (пакет)
    //  Затем отдается семафор пакету, пакет можно удалять.
    osSemaphoreWait(_binarySem, osWaitForever); 
    
    osSemaphoreRelease(_binarySem);
  
    chipDeselect();

    //  Освобождение семафора текущего пакета
    osSemaphoreRelease(_currentMessage->retSetmaphore());
//    printf("TSKspi %X stop\n\n", _hspi);

  }
}



//--------------------------------------------------------------------------------------------------------
///< Установка размера данных DMA-обмена
void SpiHandler::setupDmaDataWidth (uint32_t dataWidth)
{
  switch (dataWidth){
    case WIDTH_8BIT:{
      _hdma_spi_tx->Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
      _hdma_spi_tx->Init.MemDataAlignment    = DMA_MDATAALIGN_BYTE;
      _hdma_spi_rx->Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
      _hdma_spi_rx->Init.MemDataAlignment    = DMA_MDATAALIGN_BYTE;
    }break;
    case WIDTH_16BIT:{
      _hdma_spi_tx->Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
      _hdma_spi_tx->Init.MemDataAlignment    = DMA_MDATAALIGN_HALFWORD;
      _hdma_spi_rx->Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
      _hdma_spi_rx->Init.MemDataAlignment    = DMA_MDATAALIGN_HALFWORD;
    }break;
  
  }
  HAL_DMA_Init(_hdma_spi_tx);
  HAL_DMA_Init(_hdma_spi_rx);
}

//--------------------------------------------------------------------------------------------------------
///< Установка режима SPI
void SpiHandler::setupSpi (SpiMode mode, DataWidth dataWidth)
{
  if (dataWidth == WIDTH_8BIT){
           _hspi->Init.DataSize = SPI_DATASIZE_8BIT; }
    else { _hspi->Init.DataSize = SPI_DATASIZE_16BIT; }
  _hspi->Init.CLKPolarity = mode & BIT_1;//SPI_POLARITY_LOW;
  _hspi->Init.CLKPhase    = mode & BIT_0;//SPI_PHASE_1EDGE;

  _hspi->Instance->CR1 &= ~(SPI_DATASIZE_16BIT | SPI_POLARITY_HIGH | SPI_PHASE_1EDGE);
  _hspi->Instance->CR1 |=  (_hspi->Init.DataSize | _hspi->Init.CLKPolarity | _hspi->Init.CLKPhase);
}

//--------------------------------------------------------------------------------------------------------
void SpiHandler::setSpiFrequency(ISpiMessage* message){

 uint16_t tmp_, br_;
  br_ = message->getSpiFrequency();
 tmp_ = _spiClkFrequency/(message->getSpiFrequency());
    
 //  Устанавливается максимальный делитель, затем подбирается подходящий, если скорость соответствует диапазону делителя
 br_ = SPI_CR1_BR_2|SPI_CR1_BR_1|SPI_CR1_BR_0;
 if(tmp_<= 128) { br_ = SPI_CR1_BR_2|SPI_CR1_BR_1             ; }
 if(tmp_<=  64) { br_ = SPI_CR1_BR_2|             SPI_CR1_BR_0; }
 if(tmp_<=  32) { br_ = SPI_CR1_BR_2                          ; }
 if(tmp_<=  16) { br_ =              SPI_CR1_BR_1|SPI_CR1_BR_0; }
 if(tmp_<=   8) { br_ =              SPI_CR1_BR_1             ; }
 if(tmp_<=   4) { br_ =                           SPI_CR1_BR_0; }
 if(tmp_<=   2) { br_ = 0; }
 _hspi->Instance->CR1 &= ~SPI_CR1_BR;
 _hspi->Instance->CR1 |= br_;
}



//----------------------------------------------------------------------------------------------------
//  SPI2

// Выбрать устройство на SPI шине
void Spi2Handler::chipSelect (uint8_t cs)
{
  switch (cs){
    case CS_PORT_0:
    case CS_PORT_1:
    case CS_PORT_2:
    case CS_PORT_3:
    case CS_PORT_4:
    case CS_PORT_5:
    case CS_PORT_6:
    case CS_PORT_7:
    case CS_PORT_8:
    case CS_PORT_9:
    case CS_PORT_10: GPIOE->BSRR = ((0xF-cs)<<4)<<16; break;
    case CS_MRAM:    GPIOE->BSRR = (uint32_t)BIT_12<<16;       break;
    case CS_FLASH:   GPIOE->BSRR = (uint32_t)BIT_13<<16;       break;
  }
}

// Снять выбор устройства на SPI шине
void Spi2Handler::chipDeselect (void)
{
  GPIOE->BSRR = (BIT_4|BIT_5|BIT_6|BIT_7|BIT_12|BIT_13);
}
