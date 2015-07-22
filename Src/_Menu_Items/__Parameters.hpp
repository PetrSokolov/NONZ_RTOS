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
  
    Parameter(
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
    // Переопределение методов под данный тип параметра
    virtual inline float getValueFlt (void)           { return _valueFlt; }                               // Возвращает значение параметра.
    virtual inline void  setValue    (int32_t value) { _value = value; _valueFlt = (float)value * pow(10.0, (int)_power); } // Устанавливает значение параметра.
  protected:
    float   _valueFlt;  // Значение в формате float
    int16_t _power;     // Степень. Для перевода из int в float
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
//  Декоратор-обертка по классу ParameterFlt - подумать, как перейти на наследование чистого интерфеса
//  наследовать класс с реализацией опасно.
//  Конкретная реализация Декоратора. Переопределение методов класса.
//  Изменяет поведение объекта под Функции калибровки по записи "1"
//  
//=================================================================================================
//class DecoratorCalibrated : public Decorator {
class DecoratorCalibrated : public ParameterFlt/*IMenuItem, public IMbItem1Reg, public IVariable*/ {
public:

  // Конструктор с параметрами
  DecoratorCalibrated (
                uint16_t   id,
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

// Переопределение методов (декорирование)
  // Методы, возвращающие значение параметра
  virtual inline int32_t  getValue  (void)           { return _parameterFlt->getValue(); } // Возвращает значение параметра
  virtual inline void     setValue  (int32_t value) { _parameterFlt->setValue  (value); }  // Устанавливает значение параметра

  // Методы, возвращающие атрибуты
  virtual inline int32_t  getMin    (void)           { return _parameterFlt->getMin(); }   // Возвращает минимальное значение параметра
  virtual inline int32_t  getMax    (void)           { return _parameterFlt->getMax(); }   // Возвращает максимальное значение параметра
  virtual inline uint16_t getVarId  (void)           { return _parameterFlt->getVarId(); } // Возвращает id параметра
  virtual inline uint8_t  getRw 	  (void)           { return _parameterFlt->getRw(); }    // Возвращает разрешение на запись параметра

  // Методы интерфейса IMenuItem  ------------------------------------------------------------------------------
     virtual inline uint16_t getId     (void) { return _id; }       // Возвращает id параметра
     virtual inline char*    getMenu   (void) { return _parameterFlt->getMenu(); }     // Возвращает указатель на индекс меню
     inline char*    getText   (void) { return _parameterFlt->getText(); }     // Возвращает указатель на текст меню
     inline uint8_t  getAccess (void) { return _parameterFlt->getAccess(); }   // Возвращает уровень доступа

protected:
  uint16_t  _id;        // Идентификатор параметра. Для привязки к картам

  // Методы класса DecoratorCalibrated
  inline void setCalibrating(void)                   { _calibrating = 1; }             // Включить режим калибровки
  inline void clrCalibrating(void)                   { _calibrating = 0; }             // Выключить режим калибровки

  // Данные класса DecoratorCalibrated
  float    _autocalibratingValue;                                                      // Калибровочный коэффициент. Вычисленный автоматически.
  float    _calibratingValue;                                                          // Значение расчитанного калибровочного коэффициента
  uint8_t  _calibrating;                                                               // Статус калибровки. 0-нет, 1-включена
  ParameterFlt* _parameterFlt;                                                         // Декорируемый объект
};

}	// namespace src

#ifdef __cplusplus
};
#endif
#endif	//define PARAMETERS_H



