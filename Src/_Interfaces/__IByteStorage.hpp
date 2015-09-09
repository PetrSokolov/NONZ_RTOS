//#####################################################################################
//	Файл				__IByteStorage.hpp
//	Название:		Интерфейс, описывает протокол обмена с хранилищем байт (FRAM и тп.)
//	Автор:			Бабкин.П.А
//	Вер. | гггг-ммм-дд | Внес изменения | Описание изменений
//	=====|=============|================|==============================================
// 	 1.0 | 2015 сент 8 | Бабкин П.А	 	| Первый релиз
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
    virtual void readBytes (uint32_t storageAdres, uint16_t* bufer, uint16_t size) =0;	 ///< Чтение
    virtual void writeBytes(uint32_t storageAdres, uint16_t* bufer, uint16_t size) =0;  ///< Запись
};

} //src

#endif

