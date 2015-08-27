//#####################################################################################
//	Файл				_AnalogSensors.h
//	Название:		Аналоговые датчики.
//	Автор:			Бабкин.П.А
//	Вер. | гггг-ммм-дд | Внес изменения | Описание изменений
//	=====|=============|================|==============================================
// 	 1.0 | 2015 апр 21 | Бабкин П.А	 		| Первый релиз
// 	-----|-------------|----------------|----------------------------------------------
//#####################################################################################


#ifndef AV_SENSORS_HPP
#define AV_SENSORS_HPP
#include "stdint.h"
#include "stdio.h"

#include "..\_DSP_tools\__Filters.hpp"
#include "..\_Interfaces\__IVariable.hpp"

#ifdef __cplusplus
 extern "C" {
#endif 


namespace src{	 

//-------------------------------------------------------------------------------------------------------------------------
//	AnalogMeanSensor - класс аналоговых датчиков, вычисляющих среднее Mean значение
//-------------------------------------------------------------------------------------------------------------------------
//	Методы:
//	PutSample				- Положить в фильтр отсчет АЦП и обработать. Аргумент adc_sample - значение АЦП. Выполняется каждый период дискретизации
//	GetMean					- Получить значение в [PU]. Аргументу присваивается значение.
//	GetValue				- Получить значение в единицах измерения (В, А, ...). Аргументу присваивается значение.
//	SetTsTf					- Установить период дискретизации (ts) и постоянную времени фильтра (tf). ts и tf задаются в секундах (0.0002 сек и тп)
//	SetCalibration	- Установить калибровочный коэффициент
	class AnalogMeanSensor : public MeanFilter {
	public:
		// Конструкторы
		AnalogMeanSensor();
		AnalogMeanSensor(float tsMean, float tfMean);

		// Методы
		inline	void  getValue				(float &v);
		inline	float getValue				();
		inline	void  setCalibration	(float value) { _calibrationValue = value; }
		inline	float getCalibration	(void)        { return _calibrationValue; }

	protected:
		float	_calibrationValue;	// Калибровочный коэффициент
};
	

//-------------------------------------------------------------------------------------------------------------------------
//	AnalogDсRmsSensor - класс аналоговых датчиков, вычисляющих среднеквадратическое DC RMS значение
//-------------------------------------------------------------------------------------------------------------------------
//	Методы:
//	PutSample				- Положить в фильтр отсчет АЦП и обработать. Аргумент adc_sample - значение АЦП. Выполняется каждый период дискретизации
//	GetMean					- Получить значение в [PU]. Аргументу присваивается значение.
//	GetValue				- Получить значение в единицах измерения (В, А, ...). Аргументу присваивается значение.
//	SetTsTf					- Установить период дискретизации (ts) и постоянную времени фильтра (tf). ts и tf задаются в секундах (0.0002 сек и тп)
//	SetCalibration	- Установить калибровочный коэффициент
//	class AnalogRmsSensor : public RmsFilter {
	class AnalogDcRmsSensor {
	public:
		// Конструкторы
//		AnalogDcRmsSensor();
		AnalogDcRmsSensor(float tsRms, float tfRms, IVariable* _codeUcal, IVariable* _codeZero, IVariable* _uCal);

		// Методы
		inline	void  putSample			 (uint16_t sample);
		inline	void  getValue			 (float &value);
		inline	float getValue			 (void);
		inline	float getMean 			 (void)        { return _rmsFilter.getMean(); }
//		inline  void  setCalibration (float value) { _calibrationValue = value; };
//		inline	float getCalibration (void)        { return _calibrationValue; }

	protected:
//		float	_calibrationValue;	// Калибровочный коэффициент
    RmsFilter  _rmsFilter;
    IVariable*  _codeUcal;    // Код, соответствующий калибровочному напряжению
    IVariable*  _codeZero;    // Код нуля
    IVariable*  _uCal;        // Напряжение калибровки
};


//======================================================================================================================== 
//	Inline методы
//======================================================================================================================== 

//-------------------------------------------------------------------------------------------------------------------
//	AnalogDcRmsSensor - класс аналоговых датчиков, вычисляющих среднеквадратическое RMS значение
//-------------------------------------------------------------------------------------------------------------------

inline	void  AnalogDcRmsSensor::putSample  (uint16_t sample)
{
  _rmsFilter.putSample(sample);
}

//	GetValue				- Получить значение в единицах измерения (В, А, ...). Аргументу присваивается значение.
inline	void AnalogDcRmsSensor::getValue  (float &value)
{
	value = ((_rmsFilter.getMean() - _codeZero->getValueFlt())*_uCal->getValueFlt())/_codeUcal->getValueFlt();
}


//	GetValue				- Получить значение в единицах измерения (В, А, ...). Аргументу присваивается значение.
inline	float AnalogDcRmsSensor::getValue  ()
{
//  printf("mean      = %f\n", _rmsFilter.getMean());
//  printf("code zero = %f\n", _codeZero->getValueFlt());
//  printf("uCal      = %f\n", _uCal->getValueFlt());
//  printf("code uCal = %f\n\n", _codeUcal->getValueFlt());
  return ((_rmsFilter.getMean() - _codeZero->getValueFlt())*_uCal->getValueFlt())/_codeUcal->getValueFlt();
}

//-------------------------------------------------------------------------------------------------------------------------
//	AnalogMeanSensor - класс аналоговых датчиков, вычисляющих среднее Mean значение
//-------------------------------------------------------------------------------------------------------------------------
//	GetValue				- Получить значение в единицах измерения (В, А, ...). Аргументу присваивается значение.
inline	void AnalogMeanSensor::getValue  (float &value)
{
	value = getMean() * _calibrationValue;
}


//	GetValue				- Получить значение в единицах измерения (В, А, ...). Аргументу присваивается значение.
inline	float AnalogMeanSensor::getValue		()
{
	return (getMean() * _calibrationValue);
}

//-------------------------------------------------------------------------------------------------------------------

}
#ifdef __cplusplus
};
#endif
#endif	//define UART_HPP
