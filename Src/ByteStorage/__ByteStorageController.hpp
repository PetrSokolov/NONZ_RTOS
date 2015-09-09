//#####################################################################################
//	Файл				__ByteStorageController.hpp
//	Название:		Контроллер, . Агрегирует накопитель по интерфейсу IByteStorage
//	Автор:			Бабкин.П.А
//	Вер. | гггг-ммм-дд |     	Кто		 		| Описание изменений
//	=====|=============|================|==============================================
// 	 1.0 | 2015 сент 6 | Бабкин П.А	    | Первый релиз
// 	-----|-------------|----------------|----------------------------------------------
//#####################################################################################
#ifndef BYTESTORAGECONTROLLER_HPP
#define BYTESTORAGECONTROLLER_HPP


#include "stdint.h"
#include "..\_Interfaces\__IByteStorage.hpp"
#include "..\_Interfaces\__IByteStoragePacket.hpp"
#include "..\_Interfaces\__IByteStorageExchange.hpp"
#include "..\_Objects_Definitions\__ConstantsDefinitions.hpp"

using namespace std;

namespace src{	 


//  Структура, содержащая информацию об обмене.
/*typedef struct {
    uint16_t* _source;
    uint16_t* _destination;
    uint16_t  _size;
} ByteStorageMessage;
*/					   

class ByteStorageController : public IByteStorageExchange{
  public:
    ByteStorageController(IByteStorage* storage)
     {
       // Агрегация хранилища
	     _storage = storage;
       // _mail - очередь пакетов. Пакеты содержат информацию об обмене. Передаются накопителю.
       _mail = xQueueCreate(3, sizeof(ByteStoragePacket*));
     }

    ~ByteStorageController()
     {
	   vQueueDelete(_mail);
	 }

    // Интерфейс IByteStorageExchange
    virtual void transferPacket (IByteStoragePacket* packet);
   
    // Методы класса
    void handler (void);  // Обработчик. Бесконечно ожидает пакеты из очереди

  private:  
    xQueueHandle        _mail;          // Очередь пакетов
    IByteStoragePacket* _currentPacket; // Текущий обрабатываемый пакет
    IByteStorage*       _storage;       // Агрегация хранилища
};
  
/**
 @}
*/
}	// namespace src
#endif	//define NONVOLATILESTORAGE
