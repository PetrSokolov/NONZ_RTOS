//#####################################################################################
//	Файл				__IByteStoragePacket.hpp
//	Название:		Интерфейс, описывает протокол пакетов типа ByteStoragePacket.
//                  Пакет используется для дальнейшего обмена с хранилищами типа IByteStorage (MRAM)
//	Автор:			Бабкин.П.А
//	Вер. | гггг-ммм-дд | Внес изменения | Описание изменений
//	=====|=============|================|==============================================
// 	 1.0 | 2015 сент 7 | Бабкин П.А	 	|Первый релиз
//#####################################################################################


#ifndef IBYTESTORAGEPACKET_HPP
#define IBYTESTORAGEPACKET_HPP

#include "stdint.h"
#include "FreeRTOS.h"
#include "cmsis_os.h"
#include "..\_Objects_Definitions\__ConstantsDefinitions.hpp"


namespace src{	 

// Вырианты предназначения пакета. Может содержать данные для чтения или записи
enum PacketAction { ACTION_READ, ACTION_WRITE};

class IByteStoragePacket{
  public:
    virtual inline uint16_t* getClientBufer(void) =0;         ///< Возвращает указатель на буфер клиента
    virtual inline uint16_t  getSize(void) =0; 			      ///< Возвращает количество байт
    virtual inline uint32_t  getStorageAdres(void) =0; 	      ///< Возвращает адрес в хранилище(FRAM и тп)
    virtual inline PacketAction  getAction(void) =0; 	      ///< Возвращает предназначение пакета
    virtual inline osSemaphoreId  retSetmaphore (void) =0;    ///< Возвращяет семафор. Обозначает конец обработки пакета
};


// Класс, реализующий интерфейс IByteStoragePacket
// Если необходимо передать данные, создается экземпляр данного класса и в него передается вся необходимая информация. Затем уничтожается.

class ByteStoragePacket : public IByteStoragePacket{
  public:
    ByteStoragePacket(uint16_t* clientBufer, uint16_t size, uint32_t storageAdres, PacketAction action)
     {
       _size = size;
       _clientBufer = clientBufer;
	   _storageAdres = storageAdres;
	   _action = action;
       
       // Бинарный семафор. Опреляет доступ к объекту StoragePacket
       osSemaphoreDef(Sem);
       _binarySem = osSemaphoreCreate(osSemaphore(Sem), 1);

     }
    ~ByteStoragePacket ()
     {
       osSemaphoreDelete (_binarySem);
     }
     
  //  Методы интерфейса IByteStoragePacket
    virtual inline uint16_t* getClientBufer(void)      { return _clientBufer; } ///< Возвращает указатель на буфер клиента
    virtual inline uint16_t  getSize(void)             { return _size; }         ///< Возвращает количество слов
    virtual inline uint32_t  getStorageAdres(void)     { return _storageAdres; } ///< Возвращает адрес в хранилище(FRAM и тп)
    virtual inline PacketAction  getAction(void)       { return _action; }    ///< Возвращает предназначение пакета
    virtual inline osSemaphoreId  retSetmaphore (void) { return _binarySem; }    ///< Возвращяет семафор. Обозначает конец обработки пакета

//  Методы класса	
//  inline void      putTxByte(uint8_t byte)    { if ((_pointerBuferTx - _buferTx) < _sizeBuferTx)  { *_pointerBuferTx++ = byte; } }    ///< Разместить байт в буфере на передачу
    
  private:
    uint16_t         _size;
    uint32_t         _storageAdres;
    uint16_t*        _clientBufer;		// Буфер
	  PacketAction     _action;      // Временно. Назначение пакета (чтение/запись)
    osSemaphoreId    _binarySem;
};
} //src

#endif

