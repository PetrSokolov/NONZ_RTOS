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
bool DcBus::activate (bool state)
{
  float    dcBusVoltage_;       // Текущее напряжение на шине
  float    dcBusVoltage_1_ =0;  // Предыдущее напряжение на шине
  float    deltaVoltage_;       // Приращение напряжения на шине 
  uint16_t connectTimer_ = 600; // Таймер подключения DC-шины. На подключение дается 1 минута, иначе отключение.
  bool     dcSourceBypass_;     // Результат проверки шунтирования DC-шины источника
  
  if(state == true){
    // Подключение шины
    setInverterSwitch(true);
    setBypassSwitch(false);
    setDcBusSwitch(true);
    setDcBusSwitch(true);
     osDelay(80);  // Задержка на включение реле

    dcBusVoltage_ = _uDcBusSensor->getValue();
    deltaVoltage_ = dcBusVoltage_ - dcBusVoltage_1_;
    
    // Ожинание заряда конденсатора инвертора
    while((dcBusVoltage_ < (float)100.0 || deltaVoltage_ > (float)2.0) && connectTimer_ != 0 ){
      osDelay(100);
      dcBusVoltage_ = _uDcBusSensor->getValue();
      deltaVoltage_ = dcBusVoltage_ - dcBusVoltage_1_;
      dcBusVoltage_1_ = dcBusVoltage_;
      connectTimer_--;
    }
    
    // Проверка шунтирования DC-шины источника
    dcSourceBypass_ = dcSourceBypassTest();
    
    if (connectTimer_ == 0 || dcSourceBypass_ == false){
      // Отключение DC-шины
//      setCommand(DISCONNECT);
      setInverterSwitch(false);
      setBypassSwitch(false);
      setDcBusSwitch(false);
      return false;
    }
      else{
        // Конденсатор заряжен. Шунтирование зарядного резистора
        setBypassSwitch(true);
        return true;
      }

    
  }
    // Отключение шины
    else{
      setInverterSwitch(false);
      setBypassSwitch(false);
      setDcBusSwitch(false);
      return false;
    }
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
// Проверка шунтирования DC-шины источника
bool DcBus::dcSourceBypassTest(void)
{
  float dcBusVoltage_;
  float dcBusVoltage_1_;
  
  dcBusVoltage_1_ = _uDcBusSensor->getValue();
  setHeaterState(true);
  osDelay(2000);
  dcBusVoltage_ = _uDcBusSensor->getValue();
  setHeaterState(false);
  if((dcBusVoltage_1_ - dcBusVoltage_) > _dcBusLoadVoltageDifferent->getValueFlt()){
    return false;
  }
    else return true;
}

