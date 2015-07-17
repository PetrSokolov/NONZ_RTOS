#ifndef ADC_HPP
#define ADC_HPP
#include "stdint.h"
//#include "IADC_GetSample.hpp"

#ifdef __cplusplus
 extern "C" {
#endif 
	 
namespace src{	 
	 
#define countof(a)   (sizeof(a) / sizeof(*(a)))	

// АЦП
class Adc{
	public:
    Adc(uint16_t buferSize);                                                               // Конструктор с параметром - размер буфера АЦП
		inline void			getSample(uint16_t n, uint16_t &sample) {sample = _convertedBuf[n];}  // Возвращает n-ый отсчет
		inline uint16_t	getSample() {return _convertedBuf[0];}                                // Возвращает n-ый отсчет
		inline uint16_t	getBuferSize() {return _buferSize;}                                // Возвращает размер буфера
		inline uint16_t*	getBufer() {return _convertedBuf;}                                // Возвращает адрес буфера
//		inline void     EOC(void);                                                            // Процедура обработки в конце преобразований АЦП

	protected:
		uint16_t	_buferSize;	// Количество конвертированных данных
		uint16_t*	_convertedBuf;	// Буфер для конвертированных данных
};

							
}; // namespace
#ifdef __cplusplus
};
#endif
#endif	//define ADC_HPP
