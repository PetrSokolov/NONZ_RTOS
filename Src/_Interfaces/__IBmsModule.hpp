//#####################################################################################
//	Файл				__IBmsModule.hpp
//	Название:		Интерфейс BMS модуля
//	Автор:			Бабкин.П.А
//	Вер. | гггг-ммм-дд | Внес изменения | Описание изменений
//	=====|=============|================|==============================================
// 	 1.0 | 2015 июль 13| Бабкин П.А	    | Первый релиз
//#####################################################################################


#ifndef IBMSMODULE_H
#define IBMSMODULE_H

#include "stdint.h"


namespace src{	 

//=================================================================================================
//  Интерфейс класса для работы с ModBus
//=================================================================================================

//-------------------------------------------------------------------------------------------------
//  Интерфейс класса, имеющего 1 регистр
//-------------------------------------------------------------------------------------------------
class IBmsModule{
  public:
    virtual        void     putToMbMap  (IMbItem1Reg* mbItem1Reg) = 0;               // Положить объект в карту (объект IMbItem1Reg)
    //  Методы обработки команд протокола
    virtual void allPortInit (void);                    //  Инициализация всех PIC-портов
    virtual void allWriteConfigRegisterGroup(void);     //  Запись регистров конфигурации во все модули BMS
    virtual void allStartDiagnosticRefMux(void);        //  Запуск теста опоры и мультиплексора на всех модулях BMS
    virtual void allReadDiagnosticRefMux(void);         //  Чтение результатов теста опоры и мультиплексора на всех модулях BMS
	startAdcTest1
	readAdcTest1
	startAdcTest2
	readAdcTest2

    virtual void     portInit  (void) = 0;           // Возвращает значение на команду 3 (чтение регистра)
    virtual void     com6Handler  (void) = 0;           // Возвращает результат на команду 6 (запись регистра)
    virtual void     com16Handler (void) = 0;           // Возвращает результат на команду 16 (запись последовательности регистров)
    virtual uint16_t  getAdr 		 (void) = 0;           // Возвращает модбас адрес объекта
//    virtual inline void     setAdr 		 (uint8_t adres) = 0;  // Устанавливает модбас адрес объекта
};  

} //src
#endif
