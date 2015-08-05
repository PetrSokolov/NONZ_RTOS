//########################################################################################
//	Файл				PWM.cpp
//	Название:		ШИМ для НО/НЗ.
//	Автор:			Бабкин.П.А
//	Вер. | гггг-ммм-дд |     	Кто		 		| Описание изменений
//	=====|=============|================|==============================================
// 	 1.0 | 2015 апр 23 | Бабкин П.А	 		| Первый релиз
// 	-----|-------------|----------------|----------------------------------------------
// Примечание:  Методы общего класса, реализация которых привязанна к конкретному проекту.
//########################################################################################

#include "__PWM.hpp"
#include "..\_Objects_Definitions\__ObjectsDefinitions.hpp"
#include "tim.h"
#include "stdint.h"
//#include "math.h"
#include <cmath>




using namespace src;

//======================================================================================================================== 
// Pwm2phaseNONZ- генератор 2 фазного ШИМ для НО/НЗ
//======================================================================================================================== 
//	Методы:
//	setFrequency			- Установить частоту модуляции ШИМ
//	setValue					- Установить амплитуду сигнала (1-максамальный)
//	setBlock					- Установить/снять режим блокировки
//	computeDeathTime	- Установить мертвое время ШИМ. Имеет несколько реализаций под разное железо
//	init							- Инициализация. Имеет несколько реализаций под разное железо


void Pwm2phaseNONZ::setValue (float value)
{
	_channel1Pulse = (uint16_t) (value * (_timerPeriod - 1));
	_channel2Pulse = (uint16_t) ((1-value) * (_timerPeriod - 1));
  *_compare1 = _channel1Pulse;
  *_compare2 = _channel2Pulse;
}


//	Установка Death Time. Задается в микросекундах deathtime[мкс]
//  Возвращает значение, записываемое в регистр
uint16_t Pwm2phaseNONZ::computeDeathTime (float deathTime)
{
	// Расчет death time заключается в выборе множителя периода дискретизации и значения счетчика death time

	uint16_t	reg;						// временный контейнер регистра
	uint32_t	n1, n2, n3, n4;	// счетчик death time
	float			td;							// Период дискретизации таймера

	// Пересчет микросекунд в число периодов синхронизации
	td = (float)1/_clkDeathTime;
	
	n1	= deathTime  / td;
	n2	= (deathTime - 2*64*td) / (2*td);
	n3	= (deathTime - 8*32*td)	/ (8*td);
	n4 	= (deathTime - 16*32*td) / (16*td);
	reg =0;
	if( n1 <= 0x7F ){
		reg = 0x00 + n1;
	}
	 else{
	  if( n2 <= 0x3F ){
			reg = 0x80 + n2;
		}
		 else{
			 if( n3 <= 0x1F ){
				reg = 0xC0 + n3;
			}
			 else{
				if( n4 <= 0x1F ){
					reg = 0xE0 + n4;
				}
				else{ reg = 11; }	// Значение в случае исключения
			 }
	   }
	 }
	return reg;
}




//---------------------------------------------------------------------------------------------
//	init	- Инициализация. Реализация под железо НО/НЗ. На базе TIM1
//---------------------------------------------------------------------------------------------

void Pwm2phaseNONZ::init (void)
{
  //  Инициализация:
  //  1) указатель на регистр сравнения
  //  2) указатели на настроечные параметры _deathTime и _value
  //  3) установка deathTime

  // Инизиализация указателей на регистры перифирии
	TIM_TypeDef*	tim = TIM1;   //	Указатель на таймер TIMx
  _compare1 = (uint16_t*)&TIM1->CCR1;    //  Указатель на регистр сравнения CH1
  _compare2 = (uint16_t*)&TIM1->CCR2;    //  Указатель на регистр сравнения CH2
  

  // Инизиализация указателей на настроечные параметры
/* bugfix    _deathTime = &pwmDeathTime;     //  Указатель на настроечный параметр. death time[мкс]
  _value     = &pwmValue;         //  Указатель на настроечный параметр. Амплитуда [%]
*/
  _clkTimer = HAL_RCC_GetPCLK2Freq() *2;
		// Расчет частоты семплирования dead-time (dead-time and sampling clock)
		switch(tim->CR1 & TIM_CR1_CKD){
			case TIM_CR1_CKD_0:		{_clkDeathTime = _clkTimer>>1;}	break;
			case TIM_CR1_CKD_1:		{_clkDeathTime = _clkTimer>>2;}	break;
			case 0:
			default: 							{_clkDeathTime = _clkTimer; }		break;
		}
		
	_timerPeriod = TIM1->ARR;
	_dutyCycle1 = 0.5;
	_dutyCycle2 = 0.5;
	_dutyCycle3 = 0.5;
	// Compute CCR1 value to generate a duty cycle for channel 1,2,3
	_channel1Pulse = (uint16_t) (_dutyCycle1 * (_timerPeriod - 1));
	_channel2Pulse = (uint16_t) (_dutyCycle2 * (_timerPeriod - 1));
	_channel3Pulse = (uint16_t) (_dutyCycle3 * (_timerPeriod - 1));


}



