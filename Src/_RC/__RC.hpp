//#####################################################################################
//	Файл				RC.hpp
//	Название:		Обработка инфракрасного датчика ПДУ.
//	Автор:			Бабкин.П.А
//	Вер. | гггг-ммм-дд |      Кто       | Описание изменений
//	=====|=============|================|==============================================
// 	 1.0 | 2015 июнь 4 | Бабкин П.А	    | Первый релиз
// 	-----|-------------|----------------|----------------------------------------------
//#####################################################################################
#ifndef RC_HPP
#define RC_HPP


#include "stdint.h"
#include <list>
#include "__RC_Decoder.hpp"
#include "..\_Interfaces\__IControlCommands.hpp"



using namespace std;

#ifdef __cplusplus
 extern "C" {
#endif 

namespace src{	 

//-------------------------------------------------------------------------------------
//  Rc - класс пульта дистанционного управления
//  Включает себя класс RC_Decoder - декодер команд с ИК приемника
//-------------------------------------------------------------------------------------
class Rc{
  public:
    Rc(IRcCommands* mainMenu)
      { _mainMenu = mainMenu; }
  
    void handler (void);    // Обработчик ПДУ.
    inline void     setCommand (uint16_t command)  { _command = command; }
    inline uint16_t getCommand (void)              { return _command; }
    inline void     clrCommand (void)              { _command = 0; }

    RcDecoder rcDecoder;

  private:
    IRcCommands* _mainMenu;  //  Агрегирование объекта класса MainMenu
    uint16_t      _command;   //  Команда на отработку. Устанавливается объектом rcDecoder. Обнуляется после отработки.
};
  
}	// namespace src
#ifdef __cplusplus
};
#endif
#endif	//define RC_HPP
