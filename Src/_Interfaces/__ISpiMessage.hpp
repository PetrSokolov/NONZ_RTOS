//#####################################################################################
//	Файл				__ISpiMessage.hpp
//	Название:		Интерфейс класса, инициирующего обмен по SPI
//	Автор:			Бабкин.П.А
//	Вер. | гггг-ммм-дд | Внес изменения | Описание изменений
//	=====|=============|================|==============================================
// 	 1.0 | 2015 июнь 17 | Бабкин П.А	 		| Первый релиз
//#####################################################################################


#ifndef ISPIMESSAGE_HPP
#define ISPIMESSAGE_HPP

#include "stdint.h"
#include "FreeRTOS.h"
#include "cmsis_os.h"
#include "..\_Objects_Definitions\__ConstantsDefinitions.hpp"


namespace src{	 

class ISpiMessage{
  public:
    virtual inline uint8_t*  getPointerToTransmit(void) =0;	     ///< Возвращает указатель на буфер-передатчик
    virtual inline uint8_t*  getPointerToRecieve(void) =0;       ///< Возвращает указатель на буфер-приемник
    virtual inline uint8_t*  getPointerToRecievedBytes(void) =0; ///< Возвращает указатель на принятые данные
    virtual inline uint16_t  getSizeOfTranscieve(void) =0;       ///< Возвращает количество байт на весь обмен
    virtual inline uint16_t  getSizeOfTransmit(void) =0;         ///< Возвращает количество байт на передачу
    virtual inline uint16_t  getSizeOfRecieve(void) =0;          ///< Возвращает количество байт на прием
    virtual inline uint8_t   getChipSelect(void) =0;             ///< Возвращает код выбора устройстка на шине SPI
    virtual inline uint16_t  getSpiFrequency(void) =0;           ///< Возвращает скорость передачи [кГц]
    virtual inline SpiMode   getSpiMode  (void) =0;              ///< Возвращает режим SPI
    virtual inline uint16_t  getTransferMode  (void) =0;         ///< Возвращает режим передачи данных (8бит, 8+16бит, 16бит)
    virtual inline uint8_t   getModeOleg (void) =0;              ///< Режим "Олег". Между байтами пауза.
    virtual inline void      setModeOleg (uint8_t mode) =0;      ///< Режим "Олег". Между байтами пауза.
    virtual inline osSemaphoreId  retSetmaphore (void) =0;       ///< Возвращяет семафор. Обозначает конец обработки пакета
    //  Добавить выбор режима расчета CRC (pic, stm-hardware)
    //  Добавить настройку режима DMA (8bit, 16bit, 8x+16y bit)
};

/** Режимы передачи. Передача может осуществляться в 8, 16 битном режимах.
    Так же 8+16 бит. В этом случае производится передача n-байт в 8-битном, остальные в 16-битном режиме.
*/
enum TransferMode{  MODE_16BIT =0x00, ///< В этом режиме все данные передаются побайтно(по 8 бит)
                    MODE_8BIT1_OTHER16BIT  =0x01, ///< В этом режиме первый байт передается в 8 битном, остальные в 16 битном режиме
                    MODE_8BIT  =0xFFFF  ///< В этом режиме все данные передаются по 16 бит
                 };

/** Разрядность данных.
    Используется для конфигурауии SPI и DMA
*/
enum DataWidth   {  WIDTH_8BIT,  ///< Разрядность данных 8 бит
                    WIDTH_16BIT  ///< Разрядность данных 16 бит
                 };

class SpiPacket : public ISpiMessage{
  public:
    SpiPacket(uint16_t sizeBuferTx, uint16_t sizeBuferRx, uint8_t chipSelect, SpiMode spiMode, TransferMode transferMode, uint16_t spiFrequency)
     {
       _sizeBuferTx = sizeBuferTx;
       _sizeBuferRx = sizeBuferRx;
       _chipSelect = chipSelect;
       _spiMode = spiMode;
       _transferMode = transferMode;
	     _spiFrequency = spiFrequency;
       _done = 0;
       _modeOleg = 0;
       _buferTx = new uint8_t[sizeBuferTx];
       _buferRx = new uint8_t[sizeBuferTx+sizeBuferRx];
       _pointerBuferTx = _buferTx;
       _pointerBuferRx = _buferRx;
       
       // Бинарный семафор. Опреляет доступ к объекту SpiPacket
       osSemaphoreDef(Sem);
       _binarySem = osSemaphoreCreate(osSemaphore(Sem), 1);
       
       // Проверка выделенной памяти
       if(_buferTx == NULL || _buferRx == NULL || _binarySem == NULL){
         _allocateError = true;
       }
         else { _allocateError = false; }
     }
    ~SpiPacket ()
     {
       delete _buferTx;
       delete _buferRx;
       osSemaphoreDelete (_binarySem);
     }
     
  //  Методы интерфейса ISpiMessage
    virtual inline uint8_t*  getPointerToTransmit(void) { return _buferTx; }                     ///< Возвращает указатель на буфер-передатчик
    virtual inline uint8_t*  getPointerToRecieve(void)  { return _buferRx; }                     ///< Возвращает указатель на буфер-приемник
    virtual inline uint8_t*  getPointerToRecievedBytes(void) {return (_buferRx + _sizeBuferTx);} ///< Возвращает указатель на принятые данные
    virtual inline uint16_t  getSizeOfTranscieve(void)  { return (_sizeBuferTx + _sizeBuferRx);} ///< Возвращает количество байт на весь обмен
    virtual inline uint16_t  getSizeOfTransmit(void)    { return _sizeBuferTx; }                 ///< Возвращает количество байт на передачу
    virtual inline uint16_t  getSizeOfRecieve(void)     { return _sizeBuferRx; }                 ///< Возвращает количество байт на прием
    virtual inline uint8_t   getChipSelect(void)        { return _chipSelect; }                  ///< Возвращает код выбора устройстка на шине SPI
    virtual inline uint16_t  getSpiFrequency(void)      { return _spiFrequency; }                ///< Возвращает скорость передачи [кГц]
    virtual inline SpiMode   getSpiMode  (void)         { return _spiMode; }                     ///< Возвращает режим SPI
    virtual inline uint16_t  getTransferMode  (void)    { return _transferMode; }                ///< Возвращает режим передачи данных (8бит, 16бит, 8+16бит)
    virtual inline uint8_t   getModeOleg (void)         { return _modeOleg; }                    ///< Временно. Режим "Олег". Между байтами пауза.
    virtual inline void      setModeOleg (uint8_t mode) { _modeOleg = mode; }                    ///< Временно. Установка режима "Олег". Между байтами пауза.
    virtual inline osSemaphoreId  retSetmaphore (void)  { return _binarySem; }                   ///< Возвращяет семафор. Обозначает конец обработки пакета

  inline void      putTxByte(uint8_t byte)    { if ((_pointerBuferTx - _buferTx) < _sizeBuferTx)  { *_pointerBuferTx++ = byte; } }    ///< Разместить байт в буфере на передачу
  inline void      resetBuf(void)             { _pointerBuferTx = _buferTx; _pointerBuferRx = _buferRx; }    ///< Сброс указателей на начало буфера
  inline bool      getAllocateError(void)     { return _allocateError; }
    osSemaphoreId _binarySem;
    
  private:
    uint8_t* _buferTx;        ///<  Буфер на передачу
    uint8_t* _buferRx;        ///<  Буфер на прием
    uint8_t* _pointerBuferTx; ///<  Указатель на данные в буфере
    uint8_t* _pointerBuferRx; ///<  Указатель на данные в буфере
    uint16_t _sizeBuferTx;    ///<  Размер буфера на передачу
    uint16_t _sizeBuferRx;    ///<  Размер буфера на прием
    uint16_t _spiFrequency;   ///<  Частота SPI
    uint8_t  _chipSelect;     ///<  код выбора SPI устройства
    uint8_t  _done;           ///<  Выполнено. Устанавливается после окончания обмена
    uint8_t  _modeOleg;       ///<  Режим "Олег". Между байтами пауза.
    SpiMode  _spiMode;        ///<  Режим SPI (0,1,2,3)
    TransferMode _transferMode;///< Режим передачи. Передача может осуществляться в 8, 16 битном режимах.
    bool     _allocateError;   ///< Ошибка выделения памяти
};
} //src

#endif

