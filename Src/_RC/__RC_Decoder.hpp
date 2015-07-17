//#####################################################################################
//	Файл				RC_Module.hpp
//	Название:		Обработка инфракрасного датчика ПДУ.
//	Автор:			Бабкин.П.А
//	Вер. | гггг-ммм-дд |     	Кто		 		| Описание изменений
//	=====|=============|================|==============================================
// 	 1.0 | 2015 июнь 2 | Бабкин П.А	 		| Первый релиз
// 	-----|-------------|----------------|----------------------------------------------
//#####################################################################################
#ifndef RC_MODULE_HPP
#define RC_MODULE_HPP


#include "stdint.h"
#include <list>

//#include "..\\_Interfaces\__IObservable.hpp"

using namespace std;

#ifdef __cplusplus
 extern "C" {
#endif 

namespace src{	 

  //-------------------------------------------------------------------------------------
  //  - длительность  синхроимпульса   - 0.01 млс;                                                                
  //  - длительность передачи  единицы - 2  млс;
  //  - длительность  передачи нуля    - 8  млс.
  //  - пауза между посылками          - 12 млс
  //-------------------------------------------------------------------------------------
class RcDecoder /*: public ISubject*/{
  public:
          void      putSample  (uint32_t eventTime);                //  Положить отсчет времени пришедшего импульса
  inline  uint32_t  getTimer   (void)  { return _timer; }           //  Возвращает текущую принятую команду
  inline  uint16_t  getMessage (void)  { return _message; }         //  Возвращает текущую принятую команду
  inline  void      timerHandler (uint32_t time)  { if(_timer < 60000) { _timer += time; } else { _inputMessage =0; _message =0; } } //  Обработчик таймера. T>20млс - окончание передачи

  //  Методы интерфейса ISubject
//  void  addObserver (IObserver* observer);
//  void  removeObserver (IObserver* observer);
//  void  notyfyObservers (void);
  

  private:
    uint32_t _timer;              //  Таймер, считает интервал между импульсами [мкс]
    uint16_t _message;            //  Текущаее сообщение ПДУ
    uint16_t _inputMessage_1;     //  Сообщение ПДУ, соответствующее предыдущему импульсу [мкс]
    uint16_t _inputMessage;       //  Формирование входной посылки

//    list<IObserver*> _observers; // Список наблюдателей
};
	

}	// namespace src
#ifdef __cplusplus
};
#endif
#endif	//define RC_MODULE_HPP
