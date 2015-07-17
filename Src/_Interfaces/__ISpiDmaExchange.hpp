//#####################################################################################
//	Файл				__ISpiDmaExchange.hpp
//	Название:		Интерфейс класса, осуществляющего обмен по SPI
//	Автор:			Бабкин.П.А
//	Вер. | гггг-ммм-дд | Внес изменения | Описание изменений
//	=====|=============|================|==============================================
// 	 1.0 | 2015 июнь 17 | Бабкин П.А	 		| Первый релиз
//#####################################################################################


#ifndef ISPIDMAEXCHANGE_HPP
#define ISPIDMAEXCHANGE_HPP

#include "stdint.h"
#include "..\_Interfaces\__ISpiMessage.hpp"


namespace src{	 

class ISpiDmaExchange{
  public:
    virtual void    transferMessage (ISpiMessage* message) =0;
};

} //src

#endif
