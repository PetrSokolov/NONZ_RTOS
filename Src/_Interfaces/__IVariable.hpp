//#####################################################################################
//	Файл				__IVARIABLE.hpp
//	Название:		Интерфейс класса, имеющего переменную величину Value
//	Автор:			Бабкин.П.А
//	Вер. | гггг-ммм-дд | Внес изменения | Описание изменений
//	=====|=============|================|==============================================
// 	 1.0 | 2015 май 29 | Бабкин П.А	 		| Первый релиз
//#####################################################################################


#ifndef IVARIABLE_H
#define IVARIABLE_H

#include "stdint.h"
#include "..\_Objects_Definitions\__ConstantsDefinitions.hpp"

#ifdef __cplusplus
 extern "C" {
#endif 

namespace src{	 

//=================================================================================================
//  Интерфейс класса, имеющего переменную величину Value
//=================================================================================================
class IVariable{
  public:
                   void     putToVarMap (IVariable* variable); // Положить в карту элемент (объект IVariable)

    // Методы, возвращающие значение параметра
    virtual inline int32_t  getValue   (void) =0;            // Возвращает значение параметра
    virtual inline float    getValueFlt(void) =0;            // Возвращает значение параметра в формате float
    virtual inline Bool     setValue   (int32_t value) =0;  // Устанавливает значение параметра
    // Методы, возвращающие атрибуты
    virtual inline int32_t  getMin     (void) =0;            // Возвращает минимальное значение параметра
    virtual inline int32_t  getMax     (void) =0;            // Возвращает максимальное значение параметра
    virtual inline uint16_t getVarId   (void) =0;            // Возвращает id объекта(параметра), если он есть
    virtual inline uint8_t  getRw 	   (void) =0;            // Возвращает разрешение на запись параметра

    // Методы, используемые при редактировании параметра через меню
/*    virtual inline void startEditing  (void) =0;            // Начало редактирования параметра
    virtual inline void endEditing    (void) =0;            // Завершение редактирования параметра. С последующей командой на сохранение.
    virtual inline void exitEditing   (void) =0;            // Выход из редактирования параметра (без сохранения результата)
    virtual        void incValueHandler(uint16_t x, uint8_t power) =0;  // Инкремент параметра
    virtual        void decValueHandler(uint16_t x, uint8_t power) =0;  // Декремент параметра
    virtual        void enterHandler  (void) =0;            // Обработчик ввода при редактировании параметра
    virtual inline int32_t getEditingValue(void) =0;       // Выход из редактирования параметра (без сохранения результата)
	*/
};  

} //src
#ifdef __cplusplus
}
#endif
#endif	//define IVARIABLE_H

