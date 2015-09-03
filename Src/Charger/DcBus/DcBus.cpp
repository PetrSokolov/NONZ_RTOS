//#####################################################################################
//	Файл				RC.cpp
//	Название:		Обработка ПДУ.
//	Автор:			Бабкин.П.А
//	Вер. | гггг-ммм-дд |     	Кто		 		| Описание изменений
//	=====|=============|================|==============================================
// 	 1.0 | 2015 июнь 4 | Бабкин П.А	 		| Первый релиз
// 	-----|-------------|----------------|----------------------------------------------
//#####################################################################################

#include "__DcBus.hpp"
#include "stdint.h"
#include "..\..\_Objects_Definitions\__ObjectsDefinitions.hpp"
#include "..\..\_Interfaces\__IControlCommands.hpp"



using namespace src;

//  Привязка к железу
#define DC_BUS_SWITCH_PIN    GPIO_PIN_6
#define DC_BUS_SWITCH_PORT   GPIOC
#define BYPASS_SWITCH_PIN    GPIO_PIN_7
#define BYPASS_SWITCH_PORT   GPIOC
#define INVERTER_SWITCH_PIN  GPIO_PIN_8
#define INVERTER_SWITCH_PORT GPIOC
#define HEATER_PIN           GPIO_PIN_13
#define HEATER_PORT          GPIOC




//-------------------------------------------------------------------------------------
//  Публичные методы
//-------------------------------------------------------------------------------------
void DcBus::connect (void)
{
  setInverterSwitch(true);
  setBypassSwitch(false);
  setDcBusSwitch(true);
  
  osDelay(80);  // Задержка на включение реле
  
  
}

void DcBus::disconnect (void)
{
  setInverterSwitch(false);
  setBypassSwitch(false);
  setDcBusSwitch(false);
}

//-------------------------------------------------------------------------------------
//  Приватные методы
//-------------------------------------------------------------------------------------

//--------------------------------------------------------------------------
// Реле подключения DC-шины
void DcBus::setDcBusSwitch (bool state)
{
  if (state == true){
    HAL_GPIO_WritePin(DC_BUS_SWITCH_PORT, DC_BUS_SWITCH_PIN, GPIO_PIN_RESET);
  }
    else{
      HAL_GPIO_WritePin(DC_BUS_SWITCH_PORT, DC_BUS_SWITCH_PIN, GPIO_PIN_SET);
    }
}

bool DcBus::getDcBusSwitch (void)
{
  if(HAL_GPIO_ReadPin(DC_BUS_SWITCH_PORT, DC_BUS_SWITCH_PIN) == GPIO_PIN_RESET){
    return true;
  }
    else{ return false; }
}

//--------------------------------------------------------------------------
// Реле шунтирования зарядного резистора
void DcBus::setBypassSwitch (bool state)
{
  if (state == true){
    HAL_GPIO_WritePin(BYPASS_SWITCH_PORT, BYPASS_SWITCH_PIN, GPIO_PIN_RESET);
  }
    else{
      HAL_GPIO_WritePin(BYPASS_SWITCH_PORT, BYPASS_SWITCH_PIN, GPIO_PIN_SET);
    }
}

bool DcBus::getBypassSwitch (void)
{
  if(HAL_GPIO_ReadPin(BYPASS_SWITCH_PORT, BYPASS_SWITCH_PIN) == GPIO_PIN_RESET){
    return true;
  }
    else{ return false; }
}

//--------------------------------------------------------------------------
// Подключение инвертора к DC-шине
void DcBus::setInverterSwitch (bool state)
{
  if (state == true){
    HAL_GPIO_WritePin(INVERTER_SWITCH_PORT, INVERTER_SWITCH_PIN, GPIO_PIN_RESET);
  }
    else{
      HAL_GPIO_WritePin(INVERTER_SWITCH_PORT, INVERTER_SWITCH_PIN, GPIO_PIN_SET);
    }
}
bool DcBus::getInverterSwitch (void)
{
  if(HAL_GPIO_ReadPin(INVERTER_SWITCH_PORT, INVERTER_SWITCH_PIN) == GPIO_PIN_RESET){
    return true;
  }
    else{ return false; }
}
//--------------------------------------------------------------------------
//  Управление нагревателем
void DcBus::setHeaterState (bool state)
{
  if (state == true){
    HAL_GPIO_WritePin(HEATER_PORT, HEATER_PIN, GPIO_PIN_RESET);
  }
    else{
      HAL_GPIO_WritePin(HEATER_PORT, HEATER_PIN, GPIO_PIN_SET);
    }
}
bool DcBus::getHeaterState (void)
{
  if(HAL_GPIO_ReadPin(HEATER_PORT, HEATER_PIN) == GPIO_PIN_RESET){
    return true;
  }
    else{ return false; }
}
//-------------------------------------------------------------------------------------

