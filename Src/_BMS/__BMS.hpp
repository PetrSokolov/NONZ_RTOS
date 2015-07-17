//#####################################################################################
//	Файл				_BMS.hpp
//	Название:		Обработка BMS.
//	Автор:			Бабкин.П.А
//	Вер. | гггг-ммм-дд |      Кто       | Описание изменений
//	=====|=============|================|==============================================
// 	 1.0 | 2015 июнь 18 | Бабкин П.А	    | Первый релиз
// 	-----|-------------|----------------|----------------------------------------------
//#####################################################################################
#ifndef BMS_HPP
#define BMS_HPP


#include "stdint.h"
#include <map>
//#include "__RC_Decoder.hpp"
#include "..\_Interfaces\__ISpiMessage.hpp"
#include "..\_Interfaces\__ISpiDmaExchange.hpp"
#include "..\_SPI_Port_PIC\__SPI_Port_PIC.hpp"
#include "..\_Objects_Definitions\__ConstantsDefinitions.hpp"


using namespace std;


namespace src{	 

typedef struct {
// CFGR0
  uint32_t cdc:3;
  uint32_t cell_10:1;
  uint32_t lvlpl:1;
  uint32_t gpio1:1;
  uint32_t gpio2:1;
  uint32_t wdt:1;
// CFGR1
  uint32_t discharge_1:1;
  uint32_t discharge_2:1;
  uint32_t discharge_3:1;
  uint32_t discharge_4:1;
  uint32_t discharge_5:1;
  uint32_t discharge_6:1;
  uint32_t discharge_7:1;
  uint32_t discharge_8:1;
// CFGR2
  uint32_t discharge_9:1;
  uint32_t discharge_10:1;
  uint32_t discharge_11:1;
  uint32_t discharge_12:1;
  uint32_t maskCell_1_itr:1;
  uint32_t maskCell_2_itr:1;
  uint32_t maskCell_3_itr:1;
  uint32_t maskCell_4_itr:1;
// CFGR3
  uint32_t maskCell_5_itr:1;
  uint32_t maskCell_6_itr:1;
  uint32_t maskCell_7_itr:1;
  uint32_t maskCell_8_itr:1;
  uint32_t maskCell_9_itr:1;
  uint32_t maskCell_10_itr:1;
  uint32_t maskCell_11_itr:1;
  uint32_t maskCell_12_itr:1;
// CFGR4
  uint8_t  underVoltage;
// CFGR5
  uint8_t  overVoltage;

}LtcConfig_t;

//class SpiPortPic;

class Bms{// : public ISpiMessage{
  public:
    Bms(uint8_t  chipSelect, uint8_t chipAdres, SpiMode  spiModeLTC, SpiMode  spiModePort, uint16_t spiFrequency, uint16_t spiPortFrequency, ISpiDmaExchange* spiHandler, ISpiDmaExchange* spiPortHandler)
       { _chipSelect = chipSelect;
         _spiMode = spiModeLTC;
		     _chipAdres = chipAdres;
	    	 _spiFrequency = spiFrequency;
	       _spiHandler = spiHandler; 
		     _diagnosticMuxFail =0;
		     _diagnosticRefFail =0;
		     _diagnosticAdcTest1Fail =0;
		     _diagnosticAdcTest2Fail =0;
		     _diagnosticOpenWire =0;
         _configRegisterGroup.registers[4] = 0x87;  //2.496V
         _configRegisterGroup.registers[5] = 0x8F;  //2.688V
         _configRegisterGroup.bits.cdc =3;
         _configRegisterGroup.bits.wdt =1;

         spiPortPic = new SpiPortPic (_chipSelect, spiModePort, spiPortFrequency, spiPortHandler, 0xFF, 0xFF, 0x00, 0x00);
     }

    //  Методы класса
    inline uint8_t  getChipSelect               (void) { return _chipSelect; }
           void     writeConfigRegisterGroup    (void);  //  Write Configuration Register Group
           uint16_t readConfigRegisterGroup     (void);  //  Read Configuration Register Group
           uint16_t compareConfigRegisterGroup  (void);  //  Сравнение конфигурации прочитанной с текущей
           void     startCellVoltageMeasurement (void);  //  Function that starts Cell Voltage measurement
           uint16_t readCellVoltage             (void);  //  Read All Cell Voltage Group
           uint16_t readFlagRegisters           (void);  //  Read Flag Register Group
    inline float    getCellVoltage              (uint8_t cell)      { return _cell_voltages[cell]; }
           void     balanceControl              (uint16_t  data);  //  Управление балансировкой
    //  PIC
           void     dischargeControl            (uint16_t  data);  //  Управление разрядом
           void     closingControl              (uint16_t  data);  //  Управление шунтированием

	// Самодиагностика
    void     startDiagnosticRefMux  (void);  //  Start Diagnose and Poll Status
    uint16_t readDiagnosticRefMux   (void);  //  Read Diagnostic Register
    uint16_t diagnosticOpenWireAdc	(void);  //  Диагностика обрыва измеряющей цепи 
    void     startAdcTest1		      (void);  //  Старт "Тест 1" АЦП 
    void     startAdcTest2		      (void);  //  Старт "Тест 2" АЦП 
    uint16_t readAdcTest1		        (void);  //  Чтение "Тест 1" АЦП 
    uint16_t readAdcTest2		        (void);  //  Чтение "Тест 1" АЦП 

    // Результаты самодиагностики
    inline uint16_t getDiagnosticMuxFail(void)        { return _diagnosticMuxFail; }
    inline uint16_t getDiagnosticRefFail(void)        { return _diagnosticRefFail; }
    inline uint16_t getDiagnosticOpenWire(void)       { return _diagnosticOpenWire; }
    inline uint16_t getDiagnosticAdcTest1Fail(void)   { return _diagnosticAdcTest1Fail; }
    inline uint16_t getDiagnosticAdcTest2Fail(void)   { return _diagnosticAdcTest2Fail; }
    
	SpiPortPic*      spiPortPic;             //  SPI - Расширитель порта

  private:
    void             writeCommand (uint8_t command, uint8_t pec);  //  write command
    uint8_t          pec8_calc    (uint8_t len, uint8_t *data);
    ISpiDmaExchange* _spiHandler;    //  Агрегация обработчика задач на SPI
    uint8_t          _chipAdres;
    uint8_t          _chipSelect;
    SpiMode          _spiMode;
    uint16_t         _spiFrequency;
    float            _cell_voltages[12];        //  Напряжение на ячейках [В]
    uint16_t         _cellCodes[12];			      //  Код АЦП на ячейках
    uint16_t         _cellUnderVoltage;			    //  Регистр недостаточного напряжения на ячейках [C12,C11,C10,C9,...C1]
    uint16_t         _cellOverVoltage;			    //  Регистр превышения напряжения на ячейках [C12,C11,C10,C9,...C1]
    uint16_t         _diagnosticRegister;
    uint8_t          _diagnosticMuxFail : 1;    // Проверка мультиплексора не пройдена
    uint8_t          _diagnosticRefFail : 1;    // Проверка опрного напряжения не пройдена
	  uint16_t         _diagnosticOpenWire;       // Регистр, отображающий результат теста на обрыв измеряющией цепи
	  uint16_t         _diagnosticAdcTest1Fail;   // Не пройден тест 1 АЦП
    uint16_t         _diagnosticAdcTest2Fail;   // Не пройден тест 2 АЦП

    union{
      uint8_t          registers[6];
      LtcConfig_t      bits;
    }_configRegisterGroup, _configRegisterGroupRx;
};


//-----------------------------------------------------------------------------------------------------------------------
//  Класс, объединяющий все модули BMS

class BmsAssembly{
  public:
    void     addModule                   (Bms* module);               //  Добавить модуль в карту
    uint16_t getModule                   (uint16_t id, Bms*& module );//  Установить указатель на модуль из карты
//    uint16_t getModule                   (uint16_t id, Bms*& module );//  Установить указатель на модуль из карты

    void     portInit                    (void);  //  Инициализация всех PIC-портов
    void     writeConfigRegisterGroup    (void);  //  Запись регистров конфигурации во все модули BMS
    uint16_t readConfigRegisterGroup     (void);  //  Read Configuration Register Group
    uint16_t compareConfigRegisterGroup  (void);  //  Сравнение конфигурации прочитанной с текущей
    void     startCellVoltageMeasurement (void);  //  Function that starts Cell Voltage measurement
    uint16_t readCellVoltage             (void);  //  Read All Cell Voltage Group
    uint16_t readFlagRegisters           (void);  //  Read Flag Register Group
    float    getCellVoltage              (uint16_t module, uint8_t cell);

	// Самодиагностика
    void     startDiagnosticRefMux(void);         //  Запуск теста опоры и мультиплексора на всех модулях BMS
    void     readDiagnosticRefMux (void);         //  Чтение результатов теста опоры и мультиплексора на всех модулях BMS
    void     diagnosticOpenWireAdc(void);         //  Диагностика обрыва измеряющей цепи 
    void     startAdcTest1		    (void);         //  Старт "Тест 1" АЦП 
    void     startAdcTest2		    (void);         //  Старт "Тест 2" АЦП 
    uint16_t readAdcTest1		      (void);         //  Чтение "Тест 1" АЦП 
    uint16_t readAdcTest2		      (void);         //  Чтение "Тест 1" АЦП 

    // Результаты самодиагностики
    uint16_t  getDiagnosticMuxFail(void);
    uint16_t  getDiagnosticRefFail(void);
    uint16_t  getDiagnosticOpenWire(void);
    uint16_t  getDiagnosticAdcTest1Fail(void);
    uint16_t  getDiagnosticAdcTest2Fail(void);
    void      printDiagnostic      (void);

  private:
    map<uint16_t, Bms*> _bmsModulesMap;
    map<uint16_t, Bms*>::iterator _i;
    
};

}	// namespace src
#endif	//define RC_HPP
