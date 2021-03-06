//#####################################################################################
//	Файл				PWM.h
//	Название:		ШИМ для НО/НЗ.
//	Автор:			Бабкин.П.А
//	Вер. | гггг-ммм-дд |     	Кто		 		| Описание изменений
//	=====|=============|================|==============================================
// 	 1.0 | 2015 апр 22 | Бабкин П.А	 		| Первый релиз
// 	-----|-------------|----------------|----------------------------------------------
//#####################################################################################

#ifndef PWM_H
#define PWM_H

#include "stdint.h"
#include "..\_Interfaces\__IVariable.hpp"
#include "tim.h"  // TIM_HandleTypeDef

#ifdef __cplusplus
 extern "C" {
#endif 

namespace src{	 

//======================================================================================================================== 
// Pwm - генератор 3 фазного ШИМ
//======================================================================================================================== 
//	Методы:
//	setFrequency			- Установить частоту модуляции ШИМ
//	setValue					- Установить амплитуду сигнала (1-максамальный)
//	setBlock					- Установить/снять режим блокировки
//	computeDeathTime	- Установить мертвое время ШИМ. Имеет несколько реализаций под разное железо
//	init							- Инициализация. Имеет несколько реализаций под разное железо
	
class Pwm{
  public:
                 Pwm(IVariable* deathTime, IVariable* value)
                   {
                      _deathTime = deathTime;     //  Указатель на настроечный параметр. death time[мкс]
                      _value     = value;         //  Указатель на настроечный параметр. Амплитуда [%]
                   }
            void setFrequency (float frequency);
            void setBlock 		(float block);
    virtual	void setValue 		(float value) =0;
    virtual	void init			 		(void) = 0;
    virtual	void start		 		(void) = 0;
    virtual	void stop			 		(void) = 0;
    virtual	uint16_t computeDeathTime (float deathTime) = 0;
		
  protected:

                uint16_t  _frequency;     //  Частота модуляции ШИМ
//                float     _value;         //  Амплитуда
                uint16_t  _block;         //  Блокировка. Отключение ШИМ
                uint16_t  _timerPeriod;   //  Период таймера(ШИМ) [в отсчетах таймера]

                uint32_t  _clkTimer;      //  Частота синхронизации таймера 		[Гц]. Расчитывается при инициализации.
                uint32_t  _clkDeathTime;  //  Частота синхронизации death time	[Гц]. Расчитывается при инициализации.
                float     _dutyCycle1;    //  Скважность канала 1 (текущая)
                float     _dutyCycle2;    //  Скважность канала 2 (текущая)
                float     _dutyCycle3;    //  Скважность канала 3 (текущая)
                uint16_t  _channel1Pulse; //  Значение регистра сравнения CH1
                uint16_t  _channel2Pulse; //  Значение регистра сравнения CH2
                uint16_t  _channel3Pulse; //  Значение регистра сравнения CH3
      volatile  uint16_t* _compare1;      //  Указатель на регистр сравнения CH1 (У регистров идентификатор volatile, т.к. они изменяются аппаратно, чтоб оптимизатор не порезал )
      volatile  uint16_t* _compare2;      //  Указатель на регистр сравнения CH2
      volatile  uint16_t* _compare3;      //  Указатель на регистр сравнения CH3

  // Указатели на настроечные параметры
                IVariable* _deathTime;     //  Указатель на настроечный параметр. death time[мкс]
                IVariable* _value;         //  Указатель на настроечный параметр. Амплитуда [%]
};



//======================================================================================================================== 
// Pwm2phaseNONZ- генератор 2 фазного ШИМ для НО/НЗ
//======================================================================================================================== 
//	Методы:
//	init					- Инициализация. Реализация под железо НО/НЗ

class Pwm2phaseNONZ : public Pwm{
	public:
    Pwm2phaseNONZ(IVariable* deathTime, IVariable* value, TIM_HandleTypeDef *htim) : Pwm(deathTime, value) { _htim = htim; }
    virtual void setValue	(float value);
		virtual void init	    (void);
    virtual	void start		(void);
    virtual	void stop			(void);
		virtual uint16_t computeDeathTime (float deathTime);
  private:
    TIM_HandleTypeDef *_htim;   // Структура настроек модуля таймера
};

}	// namespace src
#ifdef __cplusplus
};
#endif
#endif	//define UART_HPP
