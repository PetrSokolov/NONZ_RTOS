//#####################################################################################
//	Файл				Editor.h
//	Название:		Редактор. Работает с объектами типа IVariable
//	Автор:			Бабкин.П.А
//	Вер. | гггг-ммм-дд | Внес изменения | Описание изменений
//	=====|=============|================|==============================================
// 	 1.0 | 2015 июнь 10 | Бабкин П.А	 		| Первый релиз
// 	-----|-------------|----------------|----------------------------------------------
//#####################################################################################

#ifndef EDITOR_HPP
#define EDITOR_HPP

#include "stdint.h"
#include <stdio.h>
#include <string>

#include "..\_Interfaces\__IMenuItem.hpp"
#include "..\_Interfaces\__IVariable.hpp"
#include "..\_Interfaces\__IDisplayed.hpp"
#include "..\_Interfaces\__IControlCommands.hpp"



using namespace std;
   
namespace src{	 

  //===========================================================================================
  //  Редактор. Работает с объектами типа IVariable 
  //===========================================================================================

class Editor : public IControlCommands, public IDisplayed{
  public:

  //  Методы интерфейса IDisplayed
    virtual void getString (string& str, uint16_t row);
    virtual void getRow (uint16_t& row);

  //  Методы интерфейса IControlCommands
    virtual void rcPlus (void);
    virtual void rcMinus(void);
    virtual void rcEnter(void);
    virtual void rcClear(void);
    virtual void rcOpen (void);
    virtual void rcClose(void);
    virtual void rcDown (void);
    virtual void rcRight(void);

  // Методы, используемые при работе с объектом IVariable
    inline void setVariable    (IVariable* variable) { _variable = variable; startEditing(); }
    inline void startEditing   (void)                { editingValue = _variable->getValue(); }       // Начало редактирования параметра
    inline void endEditing     (void)                { _variable->setValue ( editingValue ); }       // Завершение редактирования параметра. С последующей командой на сохранение.
    inline void exitEditing    (void) {}                                                             // Выход из редактирования параметра (без сохранения результата)
           void incValueHandler(uint16_t x, uint8_t power);                                          // Инкремент параметра
           void decValueHandler(uint16_t x, uint8_t power);                                          // Декремент параметра
           void enterHandler  (void);                                                                // Обработчик ввода при редактировании параметра
    inline int32_t getEditingValue(void) { return editingValue; }                                    // Возвращает значение, обрабатываемое редактором

    // Режимы редактора
    inline uint8_t getViewerMode (void) { return _viewerMode; }
    inline void    setViewerMode (void) { _viewerMode = 1; }
    inline void    clrViewerMode (void) { _viewerMode = 0; }
    inline uint8_t getModificationMode (void) { return _modificationMode; }
    inline void    setModificationMode (void) { _modificationMode = 1; }
    inline void    clrModificationMode (void) { _modificationMode = 0; }

  private:
    IVariable*  _variable;               //  Редактируемый объект
    uint8_t     _viewerMode :1;          //  Режим просмотра значения (1-включени/0-отключен)
    uint8_t     _modificationMode :1;    //  Режим изменения значения (1-включени/0-отключен)
    static int32_t  editingValue;     // Переменная, используемая при редактировании параметра

  
};
  
}	// namespace src

#endif




