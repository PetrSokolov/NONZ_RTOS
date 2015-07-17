//#####################################################################################
//	Файл				_Parameters.h
//	Название:		Классы настроечных параметров.
//	Автор:			Бабкин.П.А
//	Вер. | гггг-ммм-дд | Внес изменения | Описание изменений
//	=====|=============|================|==============================================
// 	 1.0 | 2015 апр 27 | Бабкин П.А	 		| Первый релиз
// 	-----|-------------|----------------|----------------------------------------------
//#####################################################################################

#ifndef PARAMETERS_H
#define PARAMETERS_H

#include "stdint.h"
#include "math.h"
#include <map>

#include "..\_Interfaces\__IMenuItem.hpp"
#include "..\_Interfaces\__IVariable.hpp"
#include "..\_Interfaces\__IMbItem.hpp"


#ifdef __cplusplus
 extern "C" {
#endif 

#define BYTES_TO_LONG(st,ml) (((uint32_t)st<<16)|(ml&0xFFFF))
   
using namespace std;


namespace src{	 

//class MapsOfParameters;

//====================================================================================================
  // Список типов параметров
enum {
  TYPE_SINGLE_REGISTER,
  TYPE_DOUBLE_REGISTER,
  TYPE_FLOAT
};
 
//=================================================================================================
//  Класс, содержащий объекты - переменные
//  В контейнер буду класть объекты - настроечные параметры
//=================================================================================================
class ContainerOfVariables{
  public:
    ContainerOfVariables();
    void        putContent (IVariable* variable);
    IVariable*  getContent (uint16_t id);         // Возвращает указатель на объект типа IVariable, соответствующий заданному id
//    uint8_t     isVariable (uint16_t id);         // Возвращает 1, если id содержится в карте _variablesMap. Иначе возвращает 0
    uint16_t    getCountOfContents(void);         // Возвращает общее количество элементов в контейнере
    
  protected:
    map<uint16_t, IVariable*> _variablesMap;
    map<uint16_t, IVariable*>::iterator _i;
};


//=================================================================================================
//  Класс, определяющий группу настроечных параметров
//  Пустой элемент меню, работает только на отображение
//=================================================================================================
class GroupParameter : public IMenuItem{
  public:
      // Конструктор с параметрами    
    GroupParameter( uint16_t   id,
                    char*      menu,
                    char*      text
//                    MenuEngine& menuEngine
                  );

    // Определение методов интерфейса  IMenuItem
            void     putToMenu (IMenuItem* menuItem);       // Положить в меню элемент (объект IMenuItem)
     virtual inline uint16_t getId     (void)  { return _id; }      // Возвращает id параметра
     virtual inline char*    getMenu   (void)  { return _menu; }    // Возвращает указатель на индекс меню
     inline char*    getText   (void)  { return _text; }    // Возвращает указатель на текст меню
     inline uint8_t  getAccess (void)  { return _access; }  // Возвращает уровень доступа

  protected:
  // Данные, используемые интерфейсом IMenuItem
    uint16_t  _id;        // Идентификатор параметра. Для привязки к FRAM
    uint8_t   _access;    // Идентификатор уровня доступа
    char*     _menu;      // Идентификатор меню.
    char*     _text;      // Тестовая информация
};



//=================================================================================================
// Класс настроечного параметра. В ПЗУ хранится только текст (const char*)
//=================================================================================================
class Parameter : public IMenuItem, public IMbItem1Reg, public IVariable {
  public:
    // Конструктор без параметров
    Parameter() { }
  
    // Конструктор с параметрами
    Parameter(  uint16_t   id,
                char*      menu,
                char*      text,
                uint16_t   modbusAdr,
                int16_t    value,
                uint16_t   rw,
                int16_t    min,
                int16_t    max,
                uint16_t   access,
                uint16_t   def
             );
  
 
  //----------------------------------------------------------------------------------------------------------
  // Методы интерфейса IMenuItem  ------------------------------------------------------------------------------
            void     putToMenu (IMenuItem* menuItem);   // Положить в меню элемент (объект IMenuItem)
     virtual inline uint16_t getId     (void) { return _id; }       // Возвращает id параметра
     virtual inline char*    getMenu   (void) { return _menu; }     // Возвращает указатель на индекс меню
     inline char*    getText   (void) { return _text; }     // Возвращает указатель на текст меню
     inline uint8_t  getAccess (void) { return _flags.access; }   // Возвращает уровень доступа

     
  //----------------------------------------------------------------------------------------------------------
     // Методы интерфейса IMbItem1Reg  ----------------------------------------------------------------------------
    virtual        void     putToMbMap  (IMbItem1Reg* mbItem1Reg); // Положить объект в карту (объект IMbItem1Reg)
    virtual inline uint16_t com3Handler  (void) { return 0; }// Возвращает значение на команду 3 (чтение регистра)
    virtual inline void     com6Handler  (void) {}         // Возвращает результат на команду 6 (запись регистра)
    virtual inline void     com16Handler (void) {}         // Возвращает результат на команду 16 (запись последовательности регистров)
    virtual inline uint16_t  getAdr 		 (void)           { return _modbusAdr; }  // Возвращает модбас адрес объекта

    
  //----------------------------------------------------------------------------------------------------------
  // Методы интерфейса IVariable  ----------------------------------------------------------------------------
                    void     putToVarMap  (IVariable* variable);  // Положить в карту элемент (объект IVariable)
    // Методы, возвращающие значение параметра
     virtual inline int32_t  getValue  (void)            { return _value; }   // Возвращает значение параметра
     virtual inline void     setValue  (int32_t value)  { _value = value; }  // Устанавливает значение параметра
    // Методы, возвращающие атрибуты
     virtual inline int32_t  getMin    (void) { return _min; }        // Возвращает минимальное значение параметра
     virtual inline int32_t  getMax    (void) { return _max; }        // Возвращает максимальное значение параметра
     virtual inline uint16_t getVarId  (void) { return _id; }         // Возвращает id параметра
     virtual inline uint8_t  getRw 	   (void) { return _flags.rw; }   // Возвращает разрешение на запись параметра
    // Методы, используемые при редактировании параметра через меню
/*     virtual inline void startEditing  (void)  { editingValue = _value; }      // Начало редактирования параметра
     virtual inline void endEditing    (void)  { setValue ( editingValue ); }  // Завершение редактирования параметра. С последующей командой на сохранение.
     virtual inline void exitEditing   (void)  {}                              // Выход из редактирования параметра (без сохранения результата)
     virtual        void incValueHandler(uint16_t x, uint8_t power);            // Инкремент параметра
     virtual        void decValueHandler(uint16_t x, uint8_t power);            // Декремент параметра
     virtual        void enterHandler  (void) {}                                 // Обработчик ввода при редактировании параметра
     virtual inline int32_t getEditingValue(void)  { return editingValue; }   // Выход из редактирования параметра (без сохранения результата)
*/

  protected:
  // Данные, используемые интерфейсом IMenuItem
    uint16_t  _id;        // Идентификатор параметра. Для привязки к FRAM
    char*     _menu;      // Идентификатор меню.
    char*     _text;      // Тестовая информация

  // Данные, используемые интерфейсом IMbItem1Reg
    uint8_t   _modbusAdr; // Адрес ModBus
  
  // Данные, используемые интерфейсом IVariable
    int16_t   _value;     // Значение параметра
    int16_t   _min;       // Минимальное значение
    int16_t   _max;       // Максимальное значение
    uint16_t  _def;       // Значение по умолчанию

  //  Прочие данные
    struct {
      uint8_t type   :5;   // Тип параметра. Определяется в конструкторе. 0-Parameter, 1-Parameter2reg, 2-ParameterFlt, 3-резерв
      uint8_t rw     :1;   // Разрешение на запись
      uint8_t access :1;   // Доступ
    }_flags;            // Флаги дополнительных атрибутов

};

/* 
// Класс настроечного параметра. В ПЗУ хранится только текст (const char*)
class Parameter : public IMenuItem{
  public:
    // Конструктор без параметров
    Parameter() { }
  
    // Конструктор с параметрами
    Parameter(  uint16_t   id,
                char*      menu,
                char*      text,
                uint16_t   modbusAdr,
                uint16_t   value,
                uint16_t   rw,
                uint16_t   min,
                uint16_t   max,
                uint16_t   user,
                uint16_t   def,
                MapsOfParameters& mapsOfParameters,
                MenuEngine& menuEngine
             );

   // Конструктор с параметрами. Без записи в карты
    Parameter(  uint16_t   id,
                char*      menu,
                char*      text,
                uint16_t   modbusAdr,
                uint16_t   value,
                uint16_t   rw,
                uint16_t   min,
                uint16_t   max,
                uint16_t   user,
                uint16_t   def
             );

// Интерфейс работы с данными. В зависимости от типа данных.
// Объявляются стандартные методы (виртуально). Определяются при инициализации, в зависимости от размера параметра (16 или 32 бит)
  // Методы, возвращающие значение параметра
  virtual inline uint32_t getValue  (void)           { return _value; }      // Возвращает значение параметра
  virtual inline uint32_t getValue1 (void)           { return _value; }      // Возвращает значение параметра1. Заготовка для параметра из 2 регистров
  virtual inline uint32_t getValue2 (void)           { return _value; }      // Возвращает значение параметра2. Заготовка для параметра из 2 регистров
  virtual inline float    getValueFlt(void)          { return _value; }      // Возвращает значение параметра в формате float.  Заготовка для параметра, содержащего расчитавыемый float
  virtual inline void     setValue  (uint32_t value) { _value = value; }     // Устанавливает значение параметра
  virtual inline void     setValue1 (uint16_t value) { _value = value; }     // Устанавливает значение параметра. Заготовка для параметра из 2 регистров
  virtual inline void     setValue2 (uint16_t value) { _value = value; }     // Устанавливает значение параметра. Заготовка для параметра из 2 регистров
  // Методы, возвращающие атрибуты
  virtual inline uint16_t getType   (void)           { return _flags.type; } // Возвращает тип параметра
  virtual inline uint16_t getMbAdr  (void)           { return _modbusAdr; }  // Возвращает модбас адрес параметра
  virtual inline uint16_t getMbAdr2 (void)           { return _modbusAdr; }  // Возвращает дополнительный модбас адрес параметра. Заготовка для параметра из 2 регистров
  virtual inline uint32_t getMin    (void)           { return _min; }        // Возвращает минимальное значение параметра
  virtual inline uint32_t getMax    (void)           { return _max; }        // Возвращает максимальное значение параметра

  // Переопределение методов интерфейса  IMenuItem
    virtual inline uint16_t getId     (void)           { return _id; }         // Возвращает id параметра
    virtual inline char*    getMenu   (void)           { return _menu; }       // Возвращает указатель на индекс меню
    virtual inline char*    getText   (void)           { return _text; }       // Возвращает указатель на текст меню

  // Методы, используемые при редактировании параметра через меню
  virtual inline void startEditing  (void)           { editingValue = _value; }     // Начало редактирования параметра
  virtual inline void endEditing    (void)           { setValue ( editingValue ); } // Завершение редактирования параметра. С последующей командой на сохранение.
  virtual inline void exitEditing   (void)           { }                     // Выход из редактирования параметра (без сохранения результата)
  virtual        void incValueHandler(uint16_t x, uint8_t power);            // Инкремент параметра
  virtual        void decValueHandler(uint16_t x, uint8_t power);            // Декремент параметра
  virtual        void enterHandler  (void);                                  // Обработчик ввода при редактировании параметра
  virtual inline int32_t getEditingValue(void)      { return editingValue; }// Выход из редактирования параметра (без сохранения результата)


  // Перегрузка операторов
  inline Parameter & operator = (const Parameter & x);

  // Поле данных
  protected:
//     uint16_t  _id;        // Идентификатор параметра. Для привязки к FRAM
//     char*     _menu;      // Идентификатор меню.
//     char*     _text;      // Тестовая информация

     uint16_t  _modbusAdr; // Адрес модбас
     uint16_t  _value;     // Значение параметра
     uint16_t  _min;       // Минимальное значение
     uint16_t  _max;       // Максимальное значение
     uint16_t  _def;       // Значение по умолчанию
     static int32_t  editingValue;     // Переменная, используемая при редактировании параметра
     
     struct {
       uint8_t type  :5;   // Тип параметра. Определяется в конструкторе. 0-Parameter, 1-Parameter2reg, 2-ParameterFlt, 3-резерв
       uint8_t rw    :1;   // Разрешение на запись
       uint8_t user  :1;   // Доступ в режиме пользователя
       }_flags;            // Флаги дополнительных атрибутов

};

//---------------------------------------------------------------------------
// Перегрузка операторов. Может потребоваться при работе с контейнерами.
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Operator: "="
//---------------------------------------------------------------------------
inline Parameter & Parameter :: operator = (const Parameter & x)
{
//  setId   ( x.getId()   );
//  setMenu ( x.getMenu() );
//  setText ( x.getText() );
  _id        = x._id;
  _menu      = x._menu;
  _text      = x._text;

  _modbusAdr = x._modbusAdr;
  _value     = x._value;
  _min       = x._min;
  _max       = x._max;
  _def       = x._def;
  
  _flags.rw     = x._flags.rw;
  _flags.type   = x._flags.type;
  _flags.user   = x._flags.user;

   return *this;
}


//---------------------------------------------------------------------------
// Operators: "==", "!=", "<", ">", "<=", ">=", "&&", "||"
//---------------------------------------------------------------------------
inline bool operator == (Parameter &x, Parameter& y)     { return (x.getValue() == y.getValue()); }
inline bool operator != (Parameter &x, Parameter& y)     { return (x.getValue() != y.getValue()); }
inline bool operator <  (Parameter &x, Parameter& y)     { return (x.getValue() <  y.getValue()); }
inline bool operator >  (Parameter &x, Parameter& y)     { return (x.getValue() >  y.getValue()); }
inline bool operator <= (Parameter &x, Parameter& y)     { return (x.getValue() <= y.getValue()); }
inline bool operator >= (Parameter &x, Parameter& y)     { return (x.getValue() >= y.getValue()); }
inline bool operator && (Parameter &x, Parameter& y)     { return (x.getValue() && y.getValue()); }
inline bool operator || (Parameter &x, Parameter& y)     { return (x.getValue() || y.getValue()); }


*/
/*
//=================================================================================================
//  Класс, определяющий настроечный параметр, состоящий из 2 регистров (size = 1 - 32 бит)
//=================================================================================================
class Parameter2reg : public IMenuItem, public IMbItem2Reg, public IVariable{
public:
    // Конструктор без параметров
    Parameter2reg();

  
    // Конструктор с параметрами
    Parameter2reg(  uint16_t   id,
                char*      menu,
                char*      text,
                uint16_t   modbusAdr,
                uint16_t   modbusAdr2,
                uint16_t   value,
                uint16_t   value2,
                uint16_t   rw,
                uint16_t   min,
                uint16_t   min2,
                uint16_t   max,
                uint16_t   max2,
                uint16_t   user,
                uint16_t   def,
                uint16_t   def2
                );

  // Поле с дополнительными параметрами              
  uint16_t   _modbusAdr2;
  uint16_t   _value2;
  uint16_t   _min2;
  uint16_t   _max2;
  uint16_t   _def2;


// Переопределение методов под данный тип параметра

  // Методы, возвращающие значение параметра
 virtual inline uint32_t getValue  (void)           { return BYTES_TO_LONG(_value2, _value); }   // Возвращает значение параметра
 virtual inline uint32_t getValue1 (void)           { return _value; }                           // Возвращает значение параметра.
 virtual inline uint32_t getValue2 (void)           { return _value2; }                          // Возвращает значение параметра.
 virtual inline void     setValue  (uint32_t value) { _value2 = value >> 16; _value = value & 0xFFFF; } // Устанавливает значение параметра.
 virtual inline void     setValue1 (uint16_t value) { _value  = value; }                         // Устанавливает значение параметра.
 virtual inline void     setValue2 (uint16_t value) { _value2 = value; }                         // Устанавливает значение параметра.
 virtual inline uint16_t getMbAdr2 (void)           { return _modbusAdr2; }                      // Возвращает дополнительный модбас адрес параметра. Заготовка для 32бит параметра
 virtual inline uint32_t getMin    (void)           { return BYTES_TO_LONG(_min2, _min); }       // Возвращает минимальное значение параметра
 virtual inline uint32_t getMax    (void)           { return BYTES_TO_LONG(_max2, _max); }       // Возвращает максимальное значение параметра

  // Методы, используемые при редактировании параметра через меню
  virtual inline void startEditing  (void)           { editingValue = BYTES_TO_LONG(_value2, _value); }     // Начало редактирования параметра
  virtual inline void endEditing    (void)           { setValue ( editingValue ); } // Завершение редактирования параметра. С последующей командой на сохранение.
  virtual inline void exitEditing   (void)           { }                     // Выход из редактирования параметра (без сохранения результата)

};*/



//=================================================================================================
//  Класс, определяющий настроечный параметр c расчитаным float значением
//  Зная value и pow, расчитывается значение float
//=================================================================================================
class ParameterFlt : public Parameter{
  public:
    // Конструктор без параметров
    ParameterFlt() { };

    // Конструктор с параметрами
    ParameterFlt(  uint16_t   id,
                char*      menu,
                char*      text,
                uint16_t   modbusAdr,
                int16_t    value,
                uint16_t   rw,
                int16_t    min,
                int16_t    max,
                uint16_t   user,
                uint16_t   def,
                int16_t    power
                );

    float   _valueFlt;  // Значение в формате float
    int16_t _power;     // Степень. Для перевода из int в float

    // Переопределение методов под данный тип параметра
    virtual inline float getValueFlt (void)           { return _valueFlt; }                               // Возвращает значение параметра.
    virtual inline void  setValue    (int32_t value) { _value = value; _valueFlt = (float)value * pow(10.0, (int)_power); } // Устанавливает значение параметра.

    // Методы, используемые при редактировании параметра через меню
//    virtual inline void endEditing    (void)           { setValue ( editingValue ); } // Завершение редактирования параметра. С последующей командой на сохранение.
};



//=================================================================================================
//  Декоратор по интерфейсу IVariable.
//  Содержит в себе декорируемый объект
//=================================================================================================
class Decorator : public IVariable {
public:
    // Конструктор без параметров
    Decorator () { };

    // Конструктор с параметрами
    Decorator ( IVariable* variable )
    {
	    _variable = variable;
    }

protected:
  IVariable*  _variable;       // Декорируемый объект
};


//=================================================================================================
//  Декоратор по интерфейсу IVariable.
//  Конкретная реализация Декоратора. Переопределение методов объекта.
//  Изменяет поведение объекта под Функция калибровки по записи "1"
//=================================================================================================
class DecoratorCalibrated : public Decorator {
public:

  // Конструктор с параметрами
  DecoratorCalibrated ( IVariable* variable );

// Переопределение методов (декорирование)
  // Методы, возвращающие значение параметра
  virtual inline int32_t  getValue  (void)           { return _variable->getValue(); }    // Возвращает значение параметра
  virtual inline void     setValue  (int32_t value) { _variable->setValue  (value); }    // Устанавливает значение параметра

  // Методы, возвращающие атрибуты
  virtual inline int32_t  getMin    (void)           { return _variable->getMin(); }      // Возвращает минимальное значение параметра
  virtual inline int32_t  getMax    (void)           { return _variable->getMax(); }      // Возвращает максимальное значение параметра
  virtual inline uint16_t getVarId  (void)           { return _variable->getVarId(); }       // Возвращает id параметра
  virtual inline uint8_t  getRw 	  (void)           { return _variable->getRw(); }   // Возвращает разрешение на запись параметра

  // Методы, используемые при редактировании параметра через меню
/*  virtual inline void startEditing  (void)           { _variable->startEditing(); }       // Начало редактирования параметра
  virtual inline void endEditing    (void)           { _variable->endEditing();   }       // Завершение редактирования параметра. С последующей командой на сохранение.
  virtual inline void exitEditing   (void)           { _variable->exitEditing();  }       // Выход из редактирования параметра (без сохранения результата)
  virtual 		 void incValueHandler (uint16_t x, uint8_t power) {_variable->incValueHandler(x, power);}  // Инкремент параметра
  virtual 		 void decValueHandler (uint16_t x, uint8_t power) {_variable->decValueHandler(x, power);}  // Декремент параметра
  virtual      void enterHandler    (void);                                               // Обработчик ввода при редактировании параметра
  virtual inline int32_t getEditingValue(void)      { return _variable->getEditingValue(); }// 
*/


  // Методы класса DecoratorCalibrated
          inline void setCalibrating(void)           { _calibrating = 1; }                 // Включить режим калибровки
          inline void clrCalibrating(void)           { _calibrating = 0; }                 // Выключить режим калибровки
private:

// Данные класса DecoratorCalibrated
  float    _autocalibratingValue;                                                          // Калибровочный коэффициент. Вычисленный автоматически.
  float    _calibratingValue;                                                              // Значение расчитанного калибровочного коэффициента
  uint8_t  _calibrating;                                                                   // Статус калибровки. 0-нет, 1-включена
};

}	// namespace src

#ifdef __cplusplus
};
#endif
#endif	//define PARAMETERS_H



