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
//#include "math.h"
#include <cmath>
#include <map>

#include "..\_Interfaces\__IMenuItem.hpp"
#include "..\_Interfaces\__IVariable.hpp"
#include "..\_Interfaces\__IMbItem.hpp"
#include "..\_Objects_Definitions\__ConstantsDefinitions.hpp"

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
class ParameterInt : public IMenuItem, public IMbItem1Reg, public IVariable {
  public:
    // Конструктор без параметров
//    ParameterInt() { }
  
    // Конструктор с параметрами
    ParameterInt(  uint16_t   id,
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
            void     putToMenu (IMenuItem* menuItem);               // Положить в меню элемент (объект IMenuItem)
     virtual inline uint16_t getId     (void) { return _id; }       // Возвращает id параметра
     virtual inline char*    getMenu   (void) { return _menu; }     // Возвращает указатель на индекс меню
     inline char*    getText   (void) { return _text; }             // Возвращает указатель на текст меню
     inline uint8_t  getAccess (void) { return _flags.access; }     // Возвращает уровень доступа

     
  //----------------------------------------------------------------------------------------------------------
     // Методы интерфейса IMbItem1Reg  ----------------------------------------------------------------------------
    virtual        void     putToMbMap  (IMbItem1Reg* mbItem1Reg);      // Положить объект в карту (объект IMbItem1Reg)
    virtual inline uint16_t com3Handler  (void) { return 0; }           // Возвращает значение на команду 3 (чтение регистра)
    virtual inline void     com6Handler  (void) {}                      // Возвращает результат на команду 6 (запись регистра)
    virtual inline void     com16Handler (void) {}                      // Возвращает результат на команду 16 (запись последовательности регистров)
    virtual inline uint16_t  getAdr 		 (void) { return _modbusAdr; }  // Возвращает модбас адрес объекта

    
  //----------------------------------------------------------------------------------------------------------
  // Методы интерфейса IVariable  ----------------------------------------------------------------------------
                    void     putToVarMap  (IVariable* variable);                  // Положить в карту элемент (объект IVariable)
    // Методы, возвращающие значение параметра
     virtual inline int32_t  getValue   (void)           { return _value; }       // Возвращает значение параметра
     virtual inline float    getValueFlt(void)           { return (float)_value; }// Возвращает значение параметра
     virtual inline Bool     setValue   (int32_t value)  { _value = value; return TRUE; }      // Устанавливает значение параметра
    // Методы, возвращающие атрибуты
     virtual inline int32_t  getMin     (void) { return _min; }                   // Возвращает минимальное значение параметра
     virtual inline int32_t  getMax     (void) { return _max; }                   // Возвращает максимальное значение параметра
     virtual inline uint16_t getVarId   (void) { return _id; }                    // Возвращает id параметра
     virtual inline uint8_t  getRw 	    (void) { return _flags.rw; }              // Возвращает разрешение на запись параметра

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



//=================================================================================================
//  Класс, определяющий настроечный параметр c расчитаным float значением
//  Зная value и pow, расчитывается значение float
//=================================================================================================
class ParameterFlt : public IMenuItem, public IMbItem1Reg, public IVariable{
  public:
    // Конструктор без параметров
//    ParameterFlt() { };

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
    //  Без записи в карты
    ParameterFlt(
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
  //----------------------------------------------------------------------------------------------------------
  // Методы интерфейса IMenuItem  ------------------------------------------------------------------------------
            void     putToMenu (IMenuItem* menuItem);           // Положить в меню элемент (объект IMenuItem)
     virtual inline uint16_t getId     (void) { return _id; }   // Возвращает id параметра
     virtual inline char*    getMenu   (void) { return _menu; } // Возвращает указатель на индекс меню
     inline char*    getText   (void) { return _text; }         // Возвращает указатель на текст меню
     inline uint8_t  getAccess (void) { return _flags.access; } // Возвращает уровень доступа

     
  //----------------------------------------------------------------------------------------------------------
     // Методы интерфейса IMbItem1Reg  ----------------------------------------------------------------------------
    virtual        void     putToMbMap  (IMbItem1Reg* mbItem1Reg);     // Положить объект в карту (объект IMbItem1Reg)
    virtual inline uint16_t com3Handler  (void) { return 0; }          // Возвращает значение на команду 3 (чтение регистра)
    virtual inline void     com6Handler  (void) {}                     // Возвращает результат на команду 6 (запись регистра)
    virtual inline void     com16Handler (void) {}                     // Возвращает результат на команду 16 (запись последовательности регистров)
    virtual inline uint16_t  getAdr 		 (void) { return _modbusAdr; } // Возвращает модбас адрес объекта

    
  //----------------------------------------------------------------------------------------------------------
  // Методы интерфейса IVariable  ----------------------------------------------------------------------------
                    void     putToVarMap  (IVariable* variable);              // Положить в карту элемент (объект IVariable)
    // Методы, возвращающие значение параметра
    virtual inline int32_t  getValue    (void)          { return _value; }   // Возвращает значение параметра
    virtual inline float    getValueFlt (void)          { return _valueFlt; }// Возвращает значение параметра.
    virtual inline Bool     setValue    (int32_t value) { _value = value; _valueFlt = (float)value * pow(10.0, (int)_power); return TRUE; } // Устанавливает значение параметра.
    //      inline void     setValue    (float value)  Определен ниже
    // Методы, возвращающие атрибуты
    virtual inline int32_t  getMin      (void) { return _min; }                // Возвращает минимальное значение параметра
    virtual inline int32_t  getMax      (void) { return _max; }                // Возвращает максимальное значение параметра
    virtual inline uint16_t getVarId    (void) { return _id; }                 // Возвращает id параметра
    virtual inline uint8_t  getRw 	    (void) { return _flags.rw; }           // Возвращает разрешение на запись параметра


  //----------------------------------------------------------------------------------------------------------
  // Методы класса  ----------------------------------------------------------------------------
    inline int16_t  getPower (void) { return _power; }
    inline Bool     setValue (float value) { _value = (int16_t)(value / pow(10.0, (int)_power)); _valueFlt = value; return TRUE; } // Устанавливает значение параметра.
   
     
  protected:
  // Данные, используемые интерфейсом IMenuItem
    uint16_t  _id;        // Идентификатор параметра. Для привязки к FRAM
    char*     _menu;      // Идентификатор меню.
    char*     _text;      // Тестовая информация

  // Данные, используемые интерфейсом IMbItem1Reg
    uint8_t   _modbusAdr; // Адрес ModBus
  
  // Данные, используемые интерфейсом IVariable
    float   _valueFlt;  // Значение в формате float
    int16_t   _value;     // Значение параметра
    int16_t   _min;       // Минимальное значение
    int16_t   _max;       // Максимальное значение
    uint16_t  _def;       // Значение по умолчанию

  //  Прочие данные
    int16_t _power;     // Степень. Для перевода из int в float
    struct {
      uint8_t type   :5;   // Тип параметра. Определяется в конструкторе. 0-Parameter, 1-Parameter2reg, 2-ParameterFlt, 3-резерв
      uint8_t rw     :1;   // Разрешение на запись
      uint8_t access :1;   // Доступ
    }_flags;            // Флаги дополнительных атрибутов                
                
};



//=================================================================================================
//  Декоратор по интерфейсу IVariable.
//  Содержит в себе декорируемый объект
//=================================================================================================
/*class Decorator : public IVariable {
public:
    // Конструктор без параметров
//    Decorator () { };

    // Конструктор с параметрами
    Decorator ( IVariable* variable )
    {
	    _variable = variable;
    }

protected:
  IVariable*  _variable;       // Декорируемый объект
};*/


//=================================================================================================
//  Декоратор-обертка по классу ParameterFlt
//  
//  Конкретная реализация Декоратора. Переопределение методов класса.
//  Изменяет поведение объекта под Функции калибровки
//  По записи "1" переходит в режим отображения кода
//  
//=================================================================================================
//class DecoratorCalibrated : public Decorator {
class DecoratorFltCalibrated : public IMenuItem, public IMbItem1Reg, public IVariable {
  public:

    // Конструктор с параметрами
    DecoratorFltCalibrated (
                uint16_t   id,
                char*      menu,
                char*      text,
                uint16_t   modbusAdr,
                int16_t    value,
                uint16_t   rw,
                int16_t    min,
                int16_t    max,
                uint16_t   access,
                uint16_t   def,
                int16_t    power
                );

    // Переопределение методов (декорирование)
    //----------------------------------------------------------------------------------------------------------
    // Методы интерфейса IMenuItem  ------------------------------------------------------------------------------
            void     putToMenu (IMenuItem* menuItem);                          // Положить в меню элемент (объект IMenuItem)
     virtual inline uint16_t getId     (void) { return _id; }                // Возвращает id параметра
     virtual inline char*    getMenu   (void) { return _parameter->getMenu(); }// Возвращает указатель на индекс меню
     inline char*    getText   (void) { return _parameter->getText(); }        // Возвращает указатель на текст меню
     inline uint8_t  getAccess (void) { return _parameter->getAccess(); }      // Возвращает уровень доступа

     
    //----------------------------------------------------------------------------------------------------------
    // Методы интерфейса IMbItem1Reg  ----------------------------------------------------------------------------
     virtual        void     putToMbMap  (IMbItem1Reg* mbItem1Reg);    // Положить объект в карту (объект IMbItem1Reg)
     virtual inline uint16_t com3Handler  (void) { return 0; }         // Возвращает значение на команду 3 (чтение регистра)
     virtual inline void     com6Handler  (void) {}                    // Возвращает результат на команду 6 (запись регистра)
     virtual inline void     com16Handler (void) {}                    // Возвращает результат на команду 16 (запись последовательности регистров)
     virtual inline uint16_t  getAdr 		 (void) { return _parameter->getAdr(); } // Возвращает модбас адрес объекта

    
    //----------------------------------------------------------------------------------------------------------
    // Методы интерфейса IVariable  ----------------------------------------------------------------------------
                    void     putToVarMap  (IVariable* variable);              // Положить в карту элемент (объект IVariable)
    // Методы, возвращающие значение параметра
     virtual inline int32_t  getValue    (void)          { if(_calibratingState==TRUE) { return _calibrationError / pow(10.0, (int)_parameter->getPower());} else { return _parameter->getValue(); }  }   // Возвращает значение параметра
     virtual inline float    getValueFlt (void)          { if(_calibratingState==TRUE) { return _calibrationError;} else { return _parameter->getValueFlt(); } } // Возвращает значение параметра.
     virtual        Bool     setValue    (int32_t value);                              // Обработчик ввода значения. Определяется в .cpp файле
    // Методы, возвращающие атрибуты
     virtual inline int32_t  getMin    (void) { return _parameter->getMin(); }         // Возвращает минимальное значение параметра
     virtual inline int32_t  getMax    (void) { return _parameter->getMax(); }         // Возвращает максимальное значение параметра
     virtual inline uint16_t getVarId  (void) { return _id; }                          // Возвращает id параметра
     virtual inline uint8_t  getRw 	   (void) { return _parameter->getRw(); }          // Возвращает разрешение на запись параметра

    //----------------------------------------------------------------------------------------------------------
    // Методы класса  ----------------------------------------------------------------------------
     inline Bool     getCalibratingState (void)           { return _calibratingState; }
     inline void     setAutocalibratingValue (float value){ _autocalibratingValue = value; }
     inline void     setCalibrationError (float error)    { _calibrationError = error; }
     inline float    getCalibrationError (void)           { return _calibrationError; }
     inline int16_t  getPower            (void)           { return _parameter->getPower(); } // Возвращает степень параметра
//     inline uint8_t getCalibratingValue (void)          { return _calibratingValue; }
//     inline void    setCalibratingValue ( float value ) { _calibratingValue = value; _parameter->setValue(value); }
     
protected:
  uint16_t  _id;        // Идентификатор параметра. Для привязки к картам

  // Методы класса DecoratorCalibrated
  inline void setCalibratingState(Bool state)            { _calibratingState = state; }    // Включить режим калибровки
//  inline void clrCalibratingState(void)                  { _calibratingState = FALSE; }   // Выключить режим калибровки

  // Данные класса DecoratorCalibrated
  float       _autocalibratingValue;    // Калибровочный коэффициент. Вычисленный автоматически.
  float         _calibrationError;          // Ошибка при автокалибровке. Используется для отображения в процессе автокалибровки.
//  float    _calibratingValue;          // Значение расчитанного калибровочного коэффициента
  Bool          _calibratingState;          // Статус калибровки. 0-нет, 1-включена
  ParameterFlt* _parameter;            // Декорируемый объект
};

}	// namespace src

#ifdef __cplusplus
};
#endif
#endif	//define PARAMETERS_H



