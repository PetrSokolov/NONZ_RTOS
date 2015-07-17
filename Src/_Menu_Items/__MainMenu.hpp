//#####################################################################################
//	Файл				__MainMenu.h
//	Название:		Менеджер меню.
//	Автор:			Бабкин.П.А
//	Вер. | гггг-ммм-дд | Внес изменения | Описание изменений
//	=====|=============|================|==============================================
// 	 1.0 | 2015 июнь 4 | Бабкин П.А	 		| Первый релиз
// 	-----|-------------|----------------|----------------------------------------------
//#####################################################################################

#ifndef MAINMENU_HPP
#define MAINMENU_HPP

#include "stdint.h"
#include <stdio.h>
#include <list>

#include "..\_Interfaces\__IControlCommands.hpp"
#include "..\_Interfaces\__IObservable.hpp"
#include "..\_Interfaces\__IDisplayed.hpp"

#ifdef __cplusplus
 extern "C" {
#endif 

using namespace std;
   
namespace src{	 

  
//------------------------------------------------------------------------------------
//  Класс MainMenu выполяняет задачу распределения команд от источников к потребителям
//  по интерфейсу IControlCommands.
//  Потребители обслуживаются как наблюдатели (Observers).
//------------------------------------------------------------------------------------
  class MainMenu : public IDisplayed, public IControlCommands{
  public:
    //  Методы интерфейса Наблюдателя
    void  addObserver (IControlCommands* observer);
    void  removeObserver (IControlCommands* observer);
//    void  notyfyObservers (void);   // Не используется

    //  Методы интерфейса IControlCommands
    virtual inline void rcClear(void)  { for (_it=_observers.begin(); _it!=_observers.end(); _it++) {(*_it)->rcClear();}  }
    virtual inline void rcOpen(void)  { for (_it=_observers.begin(); _it!=_observers.end(); _it++) {(*_it)->rcOpen();}  }
    virtual inline void rcClose(void)  { for (_it=_observers.begin(); _it!=_observers.end(); _it++) {(*_it)->rcClose();}  }
    virtual inline void rcPlus(void)  { for (_it=_observers.begin(); _it!=_observers.end(); _it++) {(*_it)->rcPlus();}  }
    virtual inline void rcDown(void)  { for (_it=_observers.begin(); _it!=_observers.end(); _it++) {(*_it)->rcDown();}  }
    virtual inline void rcRight(void)  { for (_it=_observers.begin(); _it!=_observers.end(); _it++) {(*_it)->rcRight();}  }
    virtual inline void rcMinus(void)  { for (_it=_observers.begin(); _it!=_observers.end(); _it++) {(*_it)->rcMinus();}  }
    virtual inline void rcEnter(void)  { for (_it=_observers.begin(); _it!=_observers.end(); _it++) {(*_it)->rcEnter();}  }

    //  Методы интерфейса IDisplayed
    virtual void getString (string& str, uint16_t row);
    virtual void getRow (uint16_t& row) { row = 0; }

    //  Методы класса
    inline void     setRemote      (uint8_t remote)        { _remote = remote; }
    inline void     setProgramming (uint8_t programming)   { _programming = programming; }
    inline uint8_t  getRemote      (void)                  { return _remote; }
    inline uint8_t  getProgramming (void)                  { return _programming; }

  private:
    list<IControlCommands*> _observers;    //  Список наблюдателей
    list<IControlCommands*>::iterator _it; //  Итератор контейнера
    uint8_t _remote;                       //  Режим ДУ/МУ
    uint8_t _programming;                  //  Режим Программирования меню
};

}	// namespace src

#ifdef __cplusplus
}
#endif
#endif	// ifdef




