//#include "stm32f10x_gpio.h"
#include "_ADC.hpp"
//#include "..\main.hpp"
#include "stm32f4xx_hal.h"
//#include "IADC_Init.hpp"

/*
ADC1			#[]		#[]		#[]		#[]		#[]		#[]		#[]		#[]		
ADC3			#[]#[]#[]					#[]#[]#[]					#[]#[]#[]					#[]#[]#[]
ADC2
TIM4_CC4	|CC4	|CC4	|CC4	|CC4	|CC4	|CC4	|CC4	|CC4	|CC4	|CC4	|CC4	
TIM3_CC1	|CC1							|CC1							|CC1							|CC1							
Time(mks)	|0		|20		|40		|60		|80		|100	|120	|140	|160	|180	|200
*/

//-------------------------------------------------------------------------------------------------------
// Различные реализации методов. Из которых буду выбирать метод интерфейса
// Здесь пользователь может написать свою реализацию
//-------------------------------------------------------------------------------------------------------
namespace src{

// Конструктор Adc
Adc::Adc(uint16_t buferSize)
 {
   _buferSize = buferSize;
   _convertedBuf = new uint16_t[buferSize];
 }

 // DMAn:chx переносит результат в буфер _convertedBuf, который зациклен размером буфера
 // 
 
 
}//namespace


 

