//#####################################################################################
//	Файл				RC_Module.cpp
//	Название:		Обработка инфракрасного датчика ПДУ.
//	Автор:			Бабкин.П.А
//	Вер. | гггг-ммм-дд |     	Кто		 		| Описание изменений
//	=====|=============|================|==============================================
// 	 1.0 | 2015 июнь 2 | Бабкин П.А	 		| Первый релиз
// 	-----|-------------|----------------|----------------------------------------------
//#####################################################################################

#include "__RC_Decoder.hpp"
#include "stdint.h"
#include "..\_Objects_Definitions\__ObjectsDefinitions.hpp"
#include "..\_Objects_Definitions\__ConstantsDefinitions.hpp"
#include "..\_Interfaces\__IControlCommands.hpp"


using namespace src;

//-------------------------------------------------------------------------------------
//  Метод putSample - вызывается по приходу импульса с ИК приемника
//-------------------------------------------------------------------------------------
void RcDecoder::putSample(uint32_t eventTime)
{
  _timer = eventTime;                      //  Значение таймера, когда пришел импульс
  if ((_timer>10000) && (_timer<60000)){   //  Принятый импульс соответствует интервалу между сообщениями
      if ( (_inputMessage & 0xF0) == 0xE0 ){
        if(_inputMessage == _inputMessage_1){         //  2 подряд одинаковых сообщения (защита от помехи)
          _message = _inputMessage;
/* bugfix */         rc.setCommand ( _message & 0x0F );
        }
      }
      else { _message =0; }                //  Принято не правильное сообщение
      _inputMessage =0;
  }
   else{
    if((_timer>7500) && (_timer<8500)){    //  Принятый импульс соответствует 0
      _inputMessage <<= 1;
      _inputMessage &= ~BIT_0;
    }
    else{                                  
    if((_timer>1500) && (_timer<2500)){    //  Принятый импульс соответствует 1
        _inputMessage <<= 1;
        _inputMessage |= BIT_0;
    }
     else{                                 //  Принятый импульс не принадлежит ни к одному интервалу
       _message =0;
       _inputMessage =0;
     }
    }
   }
  _timer =0;
  _inputMessage_1 = _inputMessage;
}


//-------------------------------------------------------------------------------------
//  Методы интерфейса IObservable
//-------------------------------------------------------------------------------------
/*
//  Добавление наблюдателя в список рассылки
void  RcDecoder::addObserver (IObserver* observer)
{
  _observers.push_back(observer);
}

//  Удаление наблюдателя из списка рассылки
void  RcDecoder::removeObserver (IObserver* observer)
{
  _observers.remove(observer);
}

//  Оповещение наблюдателей
void  RcDecoder::notyfyObservers (void)
{
  for (list<IObserver*>::iterator it=_observers.begin(); it!=_observers.end(); it++){
    (*it)->handleEvent(_message);
  }
}
*/
//-------------------------------------------------------------------------------------

