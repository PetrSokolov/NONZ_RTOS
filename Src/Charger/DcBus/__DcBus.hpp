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

using namespace std;

namespace src{	 

  /// Набор возможных команд DC-шине
  enum DcBusCommands { DISCONNECT,  ///<  Отключить DC-шину
                       CONNECT,     ///<  Подкючить DC-шину
                       COMMAND1 };  ///<  Команда 1
  
  //-------------------------------------------------------------------------------------
  //  Абстракция DC-шины
  //  Публичный метод connect() - подключить НО-НЗ к шине постоянного тока ( задействует алгоритмы заряда емкости, проверки шунтирования в приводе и тп )
  //  Публичный метод disconnect() - отключить НО-НЗ от шины постоянного тока
  //-------------------------------------------------------------------------------------
class DcBus{
  public:
    DcBus()
      {
        osMessageQDef(QueueDcBus, 16, DcBusCommands);
        _queueDcBus = osMessageCreate(osMessageQ(QueueDcBus), NULL);
      }

    ~DcBus()
      {
        vQueueDelete(_queueDcBus);
      }

    void           activate      (bool state);
    inline osMessageQId   retQueueDcBus (void)        { return _queueDcBus; }     ///< Возвращает очередь


  private:

    void setDcBusSwitch (bool state);     // Реле подключения DC-шины
    bool getDcBusSwitch (void);
    void setBypassSwitch (bool state);    // Реле шунтирования зарядного резистора
    bool getBypassSwitch (void);
    void setInverterSwitch (bool state);  // Подключение инвертора к DC-шине
    bool getInverterSwitch (void);
    
    void setHeaterState (bool state); // Управление нагревателем
    bool getHeaterState (void);
  
    osMessageQId _queueDcBus;

  
};
  
}	// namespace src

#endif	//define DCBUS_HPP
