//#####################################################################################
//	Файл				_Parameters.cpp
//	Название:		Методы настроечных параметров.
//	Автор:			Бабкин.П.А
//	Вер. | гггг-ммм-дд | Внес изменения | Описание изменений
//	=====|=============|================|==============================================
// 	 1.0 | 2015 апр 27 | Бабкин П.А	 		| Первый релиз
// 	-----|-------------|----------------|----------------------------------------------
//#####################################################################################

#include <stdio.h>
//#include <algorithm>
#include "__Parameters.hpp"
#include "..\_Objects_Definitions\__ObjectsDefinitions.hpp"


using namespace src;
using namespace std;


//=================================================================================================
//  Класс, содержащий объекты - переменные
//  В контейнер буду класть объекты - настроечные параметры
//=================================================================================================
ContainerOfVariables::ContainerOfVariables()
 {
 }

void  ContainerOfVariables::putContent (IVariable* variable)
{
  uint16_t id;
//  _variablesMap[variable->getVarId()] = variable;
  id = variable->getVarId();
  _variablesMap[id] = variable;
}

//-------------------------------------------------------------------------------------------------
//  Возвращает указатель на объект типа IVariable, соответствующий заданному id
//-------------------------------------------------------------------------------------------------
IVariable* ContainerOfVariables::getContent (uint16_t id)
{
  _i = _variablesMap.find(id);
  if(_i == _variablesMap.end()){
    // Такого адреса нет в таблице
    return 0;
  }
   else{ return (*_i).second; }
}
    



//-------------------------------------------------------------------------------------------------
//  Метод возвращает количество объектов в контейнере
//-------------------------------------------------------------------------------------------------
uint16_t  ContainerOfVariables::getCountOfContents(void)
{
  return _variablesMap.size();
}




//=================================================================================================
//                                     Класс GroupParameter
//  Класс, определяющий группу настроечных параметров
//  Пустой элемент меню, работает только на отображение
//=================================================================================================
  // Конструктор с параметрами    
  GroupParameter::GroupParameter( uint16_t   id,
                    char*      menu,
                    char*      text
//                    IMenuItem& iMenuItem
                  )
                   { _id = id;
                     _menu = menu;
                     _text = text;
                     putToMenu(this);
                   }

                   
// Методы интерфейса IMenuItem  ------------------------------------------------------------------------------
void GroupParameter::putToMenu (IMenuItem* menuItem)   // Положить в меню элемент (объект IMenuItem)
{
  menuEngine.putItem(menuItem);
}

//========================================================================================================
//                                        Класс Parameter
//========================================================================================================
  
//--------------------------------------------------------------------------------------------------------
// Parameter - Конструктор с параметрами
//--------------------------------------------------------------------------------------------------------
Parameter::Parameter(  uint16_t   id,
                char*      menu,
                char*      text,
                uint16_t    modbusAdr,
                int16_t    value,
                uint16_t   rw,
                int16_t    min,
                int16_t    max,
                uint16_t   access,
                uint16_t   def
                 )
                    { 
                      _id          = id;
                      _menu        = menu;
                      _text        = text;
                      _modbusAdr   = modbusAdr;
                      _value       = value;
                      _min         = min;
                      _max         = max;
                      _def         = def;

                      _flags.rw    = rw;
                      _flags.access  = access;
                      putToMenu(this);
                      putToMbMap(this);
                      putToVarMap(this);
                    }

// Без записи в карты
Parameter::Parameter(
                char*      menu,
                char*      text,
                uint16_t    modbusAdr,
                int16_t    value,
                uint16_t   rw,
                int16_t    min,
                int16_t    max,
                uint16_t   access,
                uint16_t   def
                 )
                    { 
                      _menu        = menu;
                      _text        = text;
                      _modbusAdr   = modbusAdr;
                      _value       = value;
                      _min         = min;
                      _max         = max;
                      _def         = def;

                      _flags.rw    = rw;
                      _flags.access  = access;
                    }

// Методы интерфейса IMenuItem  ------------------------------------------------------------------------------
void Parameter::putToMenu (IMenuItem* menuItem)   // Положить в меню элемент (объект IMenuItem)
{
  menuEngine.putItem(menuItem);
}

// Методы интерфейса IMbItem1Reg  ----------------------------------------------------------------------------
void  Parameter::putToMbMap  (IMbItem1Reg* mbItem1Reg) // Положить объект в карту (объект IMbItem1Reg)
{
  // Контейнер будет в MbEngine
}

// Методы интерфейса IVariable  ----------------------------------------------------------------------------
void  Parameter::putToVarMap  (IVariable* variable)  // Положить в карту элемент (объект IVariable)
{
 containerOfVariables.putContent(variable);
}



//--------------------------------------------------------------------------------------------------------
// ParameterFlt - Конструктор с параметрами. С запиью в карты
//--------------------------------------------------------------------------------------------------------
ParameterFlt::ParameterFlt(  uint16_t   id,
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
                ):
    Parameter(     id,
                   menu,
                   text,
                   modbusAdr,
                   value,
                   rw,
                   min,
                   max,
                   user,
                   def
                )
                { _flags.type = TYPE_FLOAT;
                  _power = power;
                }

//--------------------------------------------------------------------------------------------------------
// ParameterFlt - Конструктор с параметрами. Без записи в карты
//--------------------------------------------------------------------------------------------------------
ParameterFlt::ParameterFlt(
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
                ):
    Parameter(     
                   menu,
                   text,
                   modbusAdr,
                   value,
                   rw,
                   min,
                   max,
                   user,
                   def
                )
                { _flags.type = TYPE_FLOAT;
                  _power = power;
                }


				
//========================================================================================================
//                                        Декоратор класса DecoratorCalibrated
//  Изменяет поведение объекта под Функция калибровки по записи "1"
//========================================================================================================
 
  // Конструктор с параметрами
// DecoratorCalibrated::DecoratorCalibrated ( ParameterFlt* parameterFlt )
DecoratorCalibrated::DecoratorCalibrated (/* ParameterFlt* parameterFlt,*/
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
                int16_t    power)
  {

    _parameterFlt = new ParameterFlt ( menu,
                                text,
                                modbusAdr,
                                value,
                                rw,
                                min,
                                max,
                                user,
                                def,
                                power);
    _id = id;

    putToMenu(this);
    putToMbMap(this);
    putToVarMap(this);
  }

