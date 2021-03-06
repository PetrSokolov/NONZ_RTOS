#ifndef OBJECTSDEFINITIONS_HPP
#define OBJECTSDEFINITIONS_HPP

#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os.h"
#include "stm32f4xx_hal.h"

#include "stdint.h"

#include "..\_ADC\_ADC.hpp"
#include "..\_PWM\__PWM.hpp"
#include "..\_AnalogSensors\__AnalogSensors.hpp"
#include "..\_Menu_Items\__MainMenu.hpp"
#include "..\_Menu_Items\__Engine.hpp"
#include "..\_Menu_Items\__Parameters.hpp"
#include "..\_Menu_Items\__DisplayLed4Digit.hpp"
#include "..\_RC\__RC.hpp"
#include "..\_SPI_Handler\__SPI_Handler.hpp"
#include "..\_SPI_Port_PIC\__SPI_Port_PIC.hpp"
#include "..\_BMS\__BMS.hpp"
#include "..\_MRAM\__Mram.hpp"
#include "..\Charger\DcBus\__DcBus.hpp"
#include "..\ByteStorage\__ByteStorageController.hpp"
#include "__ConstantsDefinitions.hpp"


using namespace src;


  //  АЦП1, АЦП3
  extern Adc adc1, adc3;

  // ШИМ
  extern Pwm2phaseNONZ pwm;

   //  Датчик напряжения на шине постоянного тока. Напряжение  и ток на заряжаемой батарее
  extern AnalogDcRmsSensor uDcBusSensor, uChargeSensor, iChargeSensor;

  //  Обработчик отображения на индикаторе. Реализует интерфейс IDisplay
  extern DisplayLed4Digit displayLed4Digit;

   //  Менеджер меню
  extern MainMenu mainMenu;
   
  //  Движок меню
  extern MenuEngine menuEngine;

  // ПДУ
  extern Rc rc;

  //  Обработчик SPI
  extern SpiHandler  spi1Handler;
  extern Spi2Handler spi2Handler;

  // Энергонезависимая память MRAM
  extern Mram mram;
  
  // Хранилище. Может включать MRAM, FLASH , SD и тп
  extern ByteStorageController byteStorageController;

  // Модули БМС (0-10)
  extern Bms bms0, bms1, bms2, bms3, bms4, bms5, bms6, bms7, bms8, bms9, bms10;
  extern BmsAssembly bmsAssembly;  // Объект-контейнер, объединяющий все модули BMS

  // SPI порты на микроконтроллере PIC
//  extern SpiPortPic spiPortPic6, spiPortPic8;

  // Абстракция Charger
  // Dc-Шина
  extern DcBus dcBus;

//------------------------------------------------------------------------
//  Контейнер настроечных объектов - параметров
  extern ContainerOfVariables containerOfVariables;

// Настроечные параметры ШИМ
  extern ParameterFlt  pwmDeathTime, pwmValue;

  // Напряжение на шине постоянного тока
  extern ParameterFlt vUDcBus;
  extern ParameterFlt vUCharge;
  extern ParameterFlt vICharge;

  // Напряжение и ток калибровки
  extern ParameterInt vUCalibrating;
  extern ParameterFlt vICalibrating;
  
  extern ParameterFlt vDcBusLoadVoltageDifferent;

  // Калибровочный коэффициент датчика напряжения на шине постоянного тока
  extern DecoratorFltCalibrated vUDcBusCodeUCal;
  extern DecoratorFltCalibrated vUDcBusCodeZero;

  // Калибровочный коэффициент датчика напряжения и тока заряда
  extern DecoratorFltCalibrated vUChargeCodeUCal;
  extern DecoratorFltCalibrated vIChargeCodeICal;
  extern DecoratorFltCalibrated vUChargeCodeZero;
  extern DecoratorFltCalibrated vIChargeCodeZero;
   
//----------------------------------------------------------------------------------------------------------
//  Объекты FreeRTOS
  extern osMessageQId queueRc; /* bugfix - queueRc сделать членом объекта*/  //, queuePicDelay;
//  extern osMessageQId queueDcBus; /* bugfix - queueDcBus сделать членом объекта*/
//  extern osSemaphoreId binarySemSpiDelayOlegMode;
  extern osSemaphoreId binarySemTSK5mls;
  extern osSemaphoreId binarySemTSK500mls;
  extern osSemaphoreId binarySemTSK1000mls;



#endif	//define OBJECTSDEFINITIONS_HPP
