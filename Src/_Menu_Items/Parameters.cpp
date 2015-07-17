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
  _variablesMap[variable->getVarId()] = variable;
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
//                      _flags.type  = TYPE_SINGLE_REGISTER;
                      _flags.access  = access;
                      putToMenu(this);
                      putToMbMap(this);
                      putToVarMap(this);
                    }
//

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


// Инкремент параметра  -------------------------------------------------------------------------------------
/*void Parameter::incValueHandler(uint16_t x, uint8_t power)
{
  if(editingValue + x <= getMax() ) {editingValue += x;} else { editingValue = getMin(); }
}


// Декремент параметра  -------------------------------------------------------------------------------------
void Parameter::decValueHandler(uint16_t x, uint8_t power)
{
  if(editingValue - x >= getMin()) {editingValue -= x;} else { editingValue = getMax(); } 
}
*/

/*
//--------------------------------------------------------------------------------------------------------
// Parameter - Конструктор с параметрами. Без записи в карты
//--------------------------------------------------------------------------------------------------------
Parameter::Parameter(  uint16_t   id,
                char*      menu,
                char*      text,
                uint16_t   modbusAdr,
                uint16_t   value,
                uint16_t   rw,
                uint16_t   min,
                uint16_t   max,
                uint16_t   user,
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
                      _flags.type  = TYPE_SINGLE_REGISTER;
                      _flags.user  = user;
                    }
//--------------------------------------------------------------------------------------------------------
// Обработчик ввода при редактировании параметра
// Определяется в файле Parameters_User.cpp
//--------------------------------------------------------------------------------------------------------

//void Parameter::enterHandler (void)
//{
//}

//========================================================================================================
//                                        Класс Parameter2reg
//========================================================================================================

// Конструктор без параметров
Parameter2reg::Parameter2reg() { _flags.type  = TYPE_DOUBLE_REGISTER; }
                    
//--------------------------------------------------------------------------------------------------------
// Parameter2reg - Конструктор с параметрами
//--------------------------------------------------------------------------------------------------------
Parameter2reg::Parameter2reg(  uint16_t   id,
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
                uint16_t   def2,
                MapsOfParameters& mapsOfParameters,
                MenuEngine& menuEngine
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
                { _flags.type = TYPE_DOUBLE_REGISTER;
                  _modbusAdr2 = modbusAdr2;
                  _value2 = value2;
                  _min2 = min2;
                  _max2 = max2;
                  _def2 = def2;
                  menuEngine.putToMenu(this);
                  mapsOfParameters.putToMaps(this);
                }


//--------------------------------------------------------------------------------------------------------
// Parameter2reg - Конструктор с параметрами. Без записи в карты
//--------------------------------------------------------------------------------------------------------
Parameter2reg::Parameter2reg(  uint16_t   id,
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
                { _flags.type = TYPE_DOUBLE_REGISTER;
                  _modbusAdr2 = modbusAdr2;
                  _value2 = value2;
                  _min2 = min2;
                  _max2 = max2;
                  _def2 = def2;
                }


//========================================================================================================
//                                        Класс ParameterFlt
//========================================================================================================

// Конструктор с параметрами
ParameterFlt::ParameterFlt(  uint16_t   id,
                char*      menu,
                char*      text,
                uint16_t   modbusAdr,
                uint16_t   value,
                int16_t    power,
                uint16_t   rw,
                uint16_t   min,
                uint16_t   max,
                uint16_t   user,
                uint16_t   def,
                MapsOfParameters& mapsOfParameters,
                MenuEngine& menuEngine
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
                  mapsOfParameters.putToMaps(this);
                  menuEngine.putToMenu(this);
                }
                
*/
//--------------------------------------------------------------------------------------------------------
// ParameterFlt - Конструктор с параметрами. Без записи в карты
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


				
//========================================================================================================
//                                        Декоратор класса DecoratorCalibrated
//  Изменяет поведение объекта под Функция калибровки по записи "1"
//========================================================================================================
 
  // Конструктор с параметрами
 DecoratorCalibrated::DecoratorCalibrated ( IVariable* variable ) : Decorator( variable )
  {
//    mapsOfParameters.putToMaps(this);
//    menuEngine.putToMenu(this);
  }

