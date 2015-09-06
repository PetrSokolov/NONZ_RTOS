//#####################################################################################
//	Файл				ByteStorageController.hpp
//	Название:		Энергонезависимая память. Агрегирует накопитель по интерфейсу IByteStorage
//	Автор:			Бабкин.П.А
//	Вер. | гггг-ммм-дд |     	Кто		 		| Описание изменений
//	=====|=============|================|==============================================
// 	 1.0 | 2015 сент 6 | Бабкин П.А	    | Первый релиз
// 	-----|-------------|----------------|----------------------------------------------
//#####################################################################################
#ifndef STORAGECONTROLLER_HPP
#define STORAGECONTROLLER_HPP


#include "stdint.h"
#include "..\_Interfaces\__IByteStorage.hpp"
#include "..\_Interfaces\__IByteStorageController.hpp"
#include "..\_Objects_Definitions\__ConstantsDefinitions.hpp"

using namespace std;

namespace src{	 

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


//  Структура, содержащая информацию об обмене.
typedef struct {
    uint16_t* _source;
    uint16_t* _destination;
    uint16_t  _size;
} ByteStorageMessage;
					   

class ByteStorageController{
  public:
    ByteStorageController(IByteStorage* storage)
     {
       _storage = storage;
       
       // Mail - очередь пакетов. Пакеты содержат информацию об обмене. Передаются накопителю.
       osMailQDef(mail, 16, ByteStorageMessage);                    // Define mail queue
       osMailQId  mail;
     }

	inline bool putMail (ByteStorageMessage* message);

  private:  
    osMailQId        _mail;
    osSemaphoreId    _binarySem;     ///<  Упорядочивает доступ к MRAM
};
  
/**
 @}
*/
}	// namespace src
#endif	//define NONVOLATILESTORAGE
