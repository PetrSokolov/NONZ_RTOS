//#####################################################################################
//	Файл				_AnalogSensors.cpp
//	Название:		Аналоговые датчики.
//	Автор:			Бабкин.П.А
//	Вер. | гггг-ммм-дд | Внес изменения | Описание изменений
//	=====|=============|================|==============================================
// 	 1.0 | 2015 апр 21 | Бабкин П.А	 		| Первый релиз
// 	-----|-------------|----------------|----------------------------------------------
//#####################################################################################

#include "__AnalogSensors.hpp"
//#include "stm32f10x.h"

namespace src{


//======================================================================================================================== 
// Class AnalogRmsSensor
//======================================================================================================================== 
 
// Конструктор без параметров. Устанавливаются значения по умолчанию ts = 200mks, tf = 0.1sec
AnalogRmsSensor::AnalogRmsSensor()
 {
	_meanFilter.putTsTf	(0.0002, 0.5);
	_rmsFilter.putTsTf	(0.0002, 0.1);
 }


// Конструктор с параметрами
AnalogRmsSensor::AnalogRmsSensor (float tsMean, float tfMean, float tsRms, float tfRms, IVariable* scaleFactor, IVariable* scaleZero)
{
	_scaleFactor = scaleFactor;
  _scaleZero   = scaleZero;
  _meanFilter.putTsTf		(tsMean, tfMean);
	_rmsFilter.putTsTf		(tsRms, tfRms);
}

// Установить калибровочный коэффициент
/*void AnalogRmsSensor::setCalibration	(uint16_t value)
{
	_calibration_value = (float)value;
	_calibration_value *= (float)0.0001;
}*/

//======================================================================================================================== 
// Class AnalogMeanSensor
//======================================================================================================================== 
 
// Конструктор без параметров. Устанавливаются значения по умолчанию ts = 200mks, tf = 0.1sec
AnalogMeanSensor::AnalogMeanSensor()
 {
	putTsTf	(0.0002, 0.1);
 }


// Конструктор с параметрами
AnalogMeanSensor::AnalogMeanSensor (float tsMean, float tfMean)// : AnalogSensor(ts, tf)
{
	putTsTf		(tsMean, tfMean);
}

// Установить калибровочный коэффициент
/*void AnalogMeanSensor::setCalibration	(uint16_t value)
{
	_calibration_value = (float)value;
	_calibration_value *= (float)0.0001;
}*/

}

