//#####################################################################################
//	Файл				__IByteStorageExchange.hpp
//	Название:		Интерфейс, описывает работу с контроллером, обрабатывающим пакеты
//                  с данными на запись/чтение.
//	Автор:			Бабкин.П.А
//	Вер. | гггг-ммм-дд | Внес изменения | Описание изменений
//	=====|=============|================|==============================================
// 	 1.0 | 2015 сент 7 | Бабкин П.А	    | Первый релиз
//#####################################################################################


#ifndef IBYTESTORAGEEXCHANGE_HPP
#define IBYTESTORAGEEXCHANGE_HPP

#include "stdint.h"
//#include "..\_Interfaces\__ISpiMessage.hpp"


namespace src{	 

class IByteStorageExchange{
  public:
    virtual void    transferPacket (IByteStoragePacket* packet) =0;
};

} //src

#endif
