//#№####################################################################################
//	Файл				DcBus.hpp
//	Название:		
//	Автор:			Бабкин.П.А
//	Вер. | гггг-ммм-дд  |  Автор         | Описание изменений
//	=====|==============|================|==============================================
// 	 1.0 |2015 август 27| Бабкин П.А	 | Первый релиз
// 	-----|--------------|----------------|----------------------------------------------
//######################№###############################################################
#ifndef DCBUS_HPP
#define DCBUS_HPP


#include "stdint.h"
#include "cmsis_os.h"
#include "..\..\_AnalogSensors\__AnalogSensors.hpp"

using namespace std;

namespace src{	 

  /// Набор возможных команд DC-шине
  enum DcBusCommands { DISCONNECT,  ///<  Отключить DC-шину
                       CONNECT,     ///<  Подкючить DC-шину
                       COMMAND1 };  ///<  Команда 1
  
  //-------------------------------------------------------------------------------------
  //  Абстракция DC-шины
  //  Публичный метод activate() - подключить/отключить НО-НЗ к шине постоянного тока ( задействует алгоритмы заряда емкости, проверки шунтирования в приводе и тп )
                       //  Команды управления передаются через очередь. Обработка - в отдельной задаче.
  //-------------------------------------------------------------------------------------
class DcBus{
  public:
    DcBus(AnalogDcRmsSensor* uDcBusSensor, IVariable* dcBusLoadVoltageDifferent)
      {
        _uDcBusSensor = uDcBusSensor;
        _dcBusLoadVoltageDifferent = dcBusLoadVoltageDifferent;
        
        //osMessageQDef(QueueDcBus, 16, DcBusCommands);
//        _queueDcBus = osMessageCreate(osMessageQ(QueueDcBus), NULL);// Проверить корректность создания. Как соотносятся _queueDcBus и ##QueueDcBus
        _queueDcBus = xQueueCreate(16, sizeof(uint16_t));
      }

    ~DcBus()
      {
        vQueueDelete(_queueDcBus);// Проверить корректность удаления. Так как создавал через cmsis, а удаляю напрямую
      }

    bool                  activate      (bool state);
    inline xQueueHandle   retQueueDcBus (void)        { return _queueDcBus; }     ///< Возвращает очередь
    inline portBASE_TYPE  setCommand (DcBusCommands command)    { return xQueueSendToBack(retQueueDcBus(), &command, 0); }
    


  private:

    void setDcBusSwitch (bool state);     // Реле подключения DC-шины
    bool getDcBusSwitch (void);
    void setBypassSwitch (bool state);    // Реле шунтирования зарядного резистора
    bool getBypassSwitch (void);
    void setInverterSwitch (bool state);  // Подключение инвертора к DC-шине
    bool getInverterSwitch (void);
    
    void setHeaterState (bool state);     // Управление нагревателем
    bool getHeaterState (void);
    bool dcSourceBypassTest(void);        // Проверка шунтирования DC-шины источника
  
//    DcBusCommands      _command;
    xQueueHandle       _queueDcBus;
    AnalogDcRmsSensor* _uDcBusSensor;
    IVariable*         _dcBusLoadVoltageDifferent;
  
};
  
}	// namespace src

#endif	//define DCBUS_HPP
