//#####################################################################################
//	Файл				__IByteStorage.hpp
//	Название:		Интерфейс, обеспечивающий чтение/запись байтов.
//	Автор:			Бабкин.П.А
//	Вер. | гггг-ммм-дд | Внес изменения | Описание изменений
//	=====|=============|================|==============================================
// 	 1.0 | 2015 сент 5 | Бабкин П.А	 	|Первый релиз
//#####################################################################################


#ifndef IBYTESTORAGE_HPP
#define IBYTESTORAGE_HPP

#include "stdint.h"
#include "FreeRTOS.h"
#include "cmsis_os.h"
#include "..\_Objects_Definitions\__ConstantsDefinitions.hpp"


namespace src{	 

class IByteStorage{
  public:
    virtual inline uint8_t*  getPointerToTransmit(void) =0;	     ///< Возвращает указатель на буфер-передатчик
    virtual inline uint8_t*  getPointerToRecieve(void) =0;       ///< Возвращает указатель на буфер-приемник
    virtual inline uint16_t  getSizeOfTransmit(void) =0;         ///< Возвращает количество байт на передачу
    virtual inline uint16_t  getSizeOfRecieve(void) =0;          ///< Возвращает количество байт на прием
    virtual inline osSemaphoreId  retSetmaphore (void) =0;       ///< Возвращяет семафор. Обозначает конец обработки пакета
};


// Класс объекта, реализующего интерфейс IByteStorage.
// Если необходимо сохранить данные, создается экземпляр данного класса и в него передается вся необходимая информация

class StoragePacket : public IByteStorage{
  public:
    StoragePacket(uint16_t sizeBuferTx, uint16_t sizeBuferRx)
     {
       _sizeBuferTx = sizeBuferTx;
       _sizeBuferRx = sizeBuferRx;
       _buferTx = new uint8_t[sizeBuferTx];
       _buferRx = new uint8_t[sizeBuferTx+sizeBuferRx];
       _pointerBuferTx = _buferTx;
       _pointerBuferRx = _buferRx;
       
       // Бинарный семафор. Опреляет доступ к объекту StoragePacket
       osSemaphoreDef(Sem);
       _binarySem = osSemaphoreCreate(osSemaphore(Sem), 1);

     }
    ~StoragePacket ()
     {
       delete _buferTx;
       delete _buferRx;
       osSemaphoreDelete (_binarySem);
     }
     
  //  Методы интерфейса IByteStorage
    virtual inline uint8_t*  getPointerToTransmit(void) { return _buferTx; }                     ///< Возвращает указатель на буфер-передатчик
    virtual inline uint8_t*  getPointerToRecieve(void)  { return _buferRx; }                     ///< Возвращает указатель на буфер-приемник
    virtual inline uint16_t  getSizeOfTransmit(void)    { return _sizeBuferTx; }                 ///< Возвращает количество байт на передачу
    virtual inline uint16_t  getSizeOfRecieve(void)     { return _sizeBuferRx; }                 ///< Возвращает количество байт на прием
    virtual inline osSemaphoreId  retSetmaphore (void)  { return _binarySem; }                   ///< Возвращяет семафор. Обозначает конец обработки пакета

  //  Методы класса	
  inline void      putTxByte(uint8_t byte)    { if ((_pointerBuferTx - _buferTx) < _sizeBuferTx)  { *_pointerBuferTx++ = byte; } }    ///< Разместить байт в буфере на передачу
  inline void      resetBuf(void)    { _pointerBuferTx = _buferTx; _pointerBuferRx = _buferRx; }    ///< Сброс указателей на начало буфера
    
  private:
    uint8_t* _buferTx;        ///<  Буфер на передачу
    uint8_t* _buferRx;        ///<  Буфер на прием
    uint8_t* _pointerBuferTx; ///<  Указатель на данные в буфере
    uint8_t* _pointerBuferRx; ///<  Указатель на данные в буфере
    uint16_t _sizeBuferTx;    ///<  Размер буфера на передачу
    uint16_t _sizeBuferRx;    ///<  Размер буфера на прием
    osSemaphoreId _binarySem;
};
} //src

#endif

