//#####################################################################################
//	Файл				BMS.cpp
//	Название:		Обработка BMS.
//	Автор:			Бабкин.П.А
//	Вер. | гггг-ммм-дд |     	Кто		 		| Описание изменений
//	=====|=============|================|==============================================
// 	 1.0 | 2015 июнь 18 | Бабкин П.А	 		| Первый релиз
// 	-----|-------------|----------------|----------------------------------------------
//#####################################################################################

#include "__BMS.hpp"
#include "stdint.h"
#include "stdio.h"


//#include "..\_Objects_Definitions\__ObjectsDefinitions.hpp"
//#include "..\_Interfaces\__IControlCommands.hpp"



using namespace src;

#define WRCFG            0x01
#define RDCFG            0x02
#define RDCV             0x04
#define RDFLG            0x0C
#define RDFLG_PEC        0xE4
#define STCVAD_ALL       0x10
#define STCVAD_ALL_PEC   0xB0
#define STCVAD_TEST1     0x1E
#define STCVAD_TEST1_PEC 0x9A
#define STCVAD_TEST2     0x1F
#define STCVAD_TEST2_PEC 0x9D
#define STOWAD_ALL       0x20
#define STOWAD_ALL_PEC   0x20
#define DAGN             0x52
#define DAGN_PEC         0x79
#define DAGN             0x52
#define DAGN_PEC         0x79
#define RDDGNR           0x54
#define RDDGNR_PEC       0x6B

#define REF_VOLTAGE_2_1V 0x778
#define REF_VOLTAGE_2_9V 0x98D


//---------------------------------------------------------------------------------------------
//  Write Configuration Register Group
void Bms::writeConfigRegisterGroup (void)
{
 uint16_t index_;
 uint8_t  pec_;
 SpiPacket packet(9, 0, _chipSelect, _spiMode, MODE_8BIT, _spiFrequency);
  
  osSemaphoreWait(packet.retSetmaphore(), osWaitForever);
  
/*   _configRegisterGroup.registers[0] =0;
   _configRegisterGroup.registers[1] =0;//0xFF;
   _configRegisterGroup.registers[2] =0;
   _configRegisterGroup.registers[3] =0;
   _configRegisterGroup.registers[4] =0;
   _configRegisterGroup.registers[5] =0;*/
// CFGR0
//   _configRegisterGroup.bits.cdc     =3;
//   _configRegisterGroup.bits.wdt     =1;
// CFGR4
//   _configRegisterGroup.bits.underVoltage = 0x48;
// CFGR5
//  _configRegisterGroup.bits.overVoltage   = 0x50;

  packet.putTxByte(WRCFG);
  packet.putTxByte(0xC7);
  
  pec_ = sizeof(_configRegisterGroup.registers);
  
  for(index_=0; index_<sizeof(_configRegisterGroup.registers); index_++){
    packet.putTxByte ( _configRegisterGroup.registers[index_] );
  }
  
  pec_ = pec8_calc(index_, _configRegisterGroup.registers);
  packet.putTxByte(pec_);


  _spiHandler->transferMessage(&packet);

  osSemaphoreWait(packet.retSetmaphore(), osWaitForever);

}


//---------------------------------------------------------------------------------------------
//  Read Configuration Register Group
uint16_t Bms::readConfigRegisterGroup (void)
{
 uint8_t  pec_, recievePec_;
 uint16_t size_, index_;
 uint8_t* pointerToRxBytes_;
  
 SpiPacket packet(4, 6+1, _chipSelect, _spiMode, MODE_8BIT, _spiFrequency);
  
  osSemaphoreWait(packet.retSetmaphore(), osWaitForever);
  
  packet.putTxByte(0x80 + _chipAdres);
  packet.putTxByte(pec8_calc(1,packet.getPointerToTransmit()));
  packet.putTxByte(RDCFG);    // read config
  packet.putTxByte(0xCE);     // PEC (read config)

  _spiHandler->transferMessage(&packet);

  osSemaphoreWait(packet.retSetmaphore(), osWaitForever);
  
  
  size_ = packet.getSizeOfRecieve()-1;
  pointerToRxBytes_ = packet.getPointerToRecieve() + packet.getSizeOfTransmit();
  
  pec_ = pec8_calc(size_,pointerToRxBytes_);
  recievePec_ = packet.getPointerToRecieve()[packet.getSizeOfTranscieve()-1];
  
  //  Проверка контрольной суммы
  if(pec_ == recievePec_){
    //  Копирования данных
    for(index_=0; index_< (packet.getSizeOfRecieve()-1); index_++){
      _configRegisterGroupRx.registers[index_] = pointerToRxBytes_[index_];
    }
    return 1;
  }
   else{ return 0; }
}   
   


//---------------------------------------------------------------------------------------------
//  Сравнение конфигурации прочитанной с текущей
uint16_t Bms::compareConfigRegisterGroup (void)
{
  uint8_t index;
  
  for (index=0; index<4; index++){
    if (_configRegisterGroupRx.registers[index] != _configRegisterGroup.registers[index]){
      return pdFALSE;
    }
  }
  return pdTRUE;
}
 
//---------------------------------------------------------------------------------------------
//  Read Flag Register Group
uint16_t  Bms::readFlagRegisters (void)
{
 uint8_t  pec_, recievePec_;
 uint8_t  byte_, bits_;
 uint16_t size_, index_;
 uint8_t* pointerToRxBytes_;
  
 SpiPacket packet(4, 3+1, _chipSelect, _spiMode, MODE_8BIT, _spiFrequency);
  
  osSemaphoreWait(packet.retSetmaphore(), osWaitForever);
  
  packet.putTxByte(0x80 + _chipAdres);
  packet.putTxByte(pec8_calc(1,packet.getPointerToTransmit()));
  packet.putTxByte(RDFLG);    // read config
  packet.putTxByte(RDFLG_PEC);     // PEC (read config)

  _spiHandler->transferMessage(&packet);

  osSemaphoreWait(packet.retSetmaphore(), osWaitForever);
  
  
  size_ = packet.getSizeOfRecieve()-1;  // Количество принятых байтов без PEC
  pointerToRxBytes_ = packet.getPointerToRecieve() + packet.getSizeOfTransmit();
  
  pec_ = pec8_calc(size_,pointerToRxBytes_);
  recievePec_ = packet.getPointerToRecieve()[packet.getSizeOfTranscieve()-1];
  
  //  Проверка контрольной суммы
  if(pec_ == recievePec_){
    //  Копирования данных
    _cellUnderVoltage = 0;
    _cellOverVoltage  = 0;

    for(index_=0; index_< size_; index_++){
      byte_ = pointerToRxBytes_[size_-1-index_];
      //  Заполняются с конца (C12UV..C1UV)
      for(bits_=0; bits_<8; bits_ +=2){
        _cellUnderVoltage <<=1;
        _cellOverVoltage <<=1;
        _cellOverVoltage  |= (byte_ & BIT_7)>>7;
          byte_ <<=1;
        _cellUnderVoltage |= (byte_ & BIT_7)>>7;
          byte_ <<=1;
      }

      
    }
    return 1;
  }
   else{ return 0; }
}
 
//---------------------------------------------------------------------------------------------
//  Старт Диагностики мультиплексора и источника опорного напряжения
void Bms::startDiagnosticRefMux()
{
  writeCommand(DAGN, DAGN_PEC);
}


//---------------------------------------------------------------------------------------------
//  Диагностика обрыва цепи, измеряющей напряжение банки
//  Выполняется 2 теста, их результаты сравниваются. При обрыве напряжение на банке изменяется
//  в соответствии с описанием на LTC
//  Возвращает TRUE в случае удачного прохождения теста
uint16_t Bms::diagnosticOpenWireAdc()
{
  float    cellVoltages_1[NCELLS];
  uint8_t  i; //pecErr_;

  _diagnosticOpenWire =0;

  //  Первый тест
  writeCommand(STOWAD_ALL, STOWAD_ALL_PEC);
  osDelay(50);
  if (readCellVoltage() == pdFALSE){
    _diagnosticOpenWire =0xFFF;
    return 0;
  }
  

  //  Копирование результата
  for (i=0; i<NCELLS; i++){
    cellVoltages_1[i] = _cellVoltage[i];
  }

  writeCommand(STOWAD_ALL, STOWAD_ALL_PEC);

  osDelay(50);

  //  Второй тест
  if (readCellVoltage() == pdFALSE){
    _diagnosticOpenWire =0xFFF;
    return 0;
  }
  
  //  Обработка результатов
  if(cellVoltages_1[0]<0 || _cellVoltage[0]<0){
    _diagnosticOpenWire |= BIT_0;
  }

  if(cellVoltages_1[11]<0 || _cellVoltage[11]<0){
    _diagnosticOpenWire |= BIT_11;
  }

  for (i=1; i<11; i++){
    if (((_cellVoltage[i+1] - cellVoltages_1[i+1])>(float)0.2) || (_cellCodes[i+1])==0xFFF){
     _diagnosticOpenWire |= 1<<i;
    }
  }

  return 1;
}



//---------------------------------------------------------------------------------------------
//  Самодиагностика АЦП
//  ячейки должны содержать код 0x555 или 0xAAA
void Bms::startAdcTest1()
{
  writeCommand(STCVAD_TEST1, STCVAD_TEST1_PEC);
}


void Bms::startAdcTest2()
{
  writeCommand(STCVAD_TEST2, STCVAD_TEST2_PEC);
}

//---------------------------------------------------------------------------------------------
// Чтение результата теста 1
uint16_t Bms::readAdcTest1()
{
  uint8_t i;
  
  if (readCellVoltage() == pdFALSE){
    _diagnosticAdcTest1Fail =1;
    return 0;
  }
   else{
     for(i=0; i<NCELLS; i++){
      if (_cellCodes[i] != 0x555){
        _diagnosticAdcTest1Fail =1;
      }
     }
     return 1;
   }
}


//---------------------------------------------------------------------------------------------
// Чтение результата теста 2
uint16_t Bms::readAdcTest2()
{
  uint8_t i;
  
  if (readCellVoltage() == pdFALSE){
    _diagnosticAdcTest2Fail =1;
    return 0;
  }
   else{
     for(i=0; i<NCELLS; i++){
       if (_cellCodes[i] != 0xAAA){
         _diagnosticAdcTest2Fail =1;
       }
     }
     return 1;
   }
}


//---------------------------------------------------------------------------------------------
//  Старт измерения напряжения на ячейках
void Bms::startCellVoltageMeasurement()
{
  writeCommand(STCVAD_ALL, STCVAD_ALL_PEC);
}


//---------------------------------------------------------------------------------------------
//  Read All Cell Voltage Group
uint16_t Bms::readCellVoltage (void)
{
 uint8_t  pec_, recievePec_;
 uint16_t size_;
 uint8_t* pointerToRxBytes_;
 uint16_t temp_, temp2_;

 SpiPacket packet(4, 18+1, _chipSelect, _spiMode, MODE_8BIT, _spiFrequency);
  
  osSemaphoreWait(packet.retSetmaphore(), osWaitForever);
  
  packet.putTxByte(0x80 + _chipAdres);
  packet.putTxByte(pec8_calc(1,packet.getPointerToTransmit()));
  packet.putTxByte(RDCV);
  packet.putTxByte(0xDC);

  _spiHandler->transferMessage(&packet);

  osSemaphoreWait(packet.retSetmaphore(), osWaitForever);  

  
  size_ = packet.getSizeOfRecieve()-1;
  pointerToRxBytes_ = packet.getPointerToRecieve() + packet.getSizeOfTransmit();// заменить на getPointerToRecievedBytes
  
  pec_ = pec8_calc(size_,pointerToRxBytes_);
  recievePec_ = packet.getPointerToRecieve()[packet.getSizeOfTranscieve()-1];
  
  //  Проверка контрольной суммы
  if(pec_ == recievePec_){
    //  Копирования данных
    uint8_t data_counter =0;
    float   totalVoltage_ =0;
    
          for (uint8_t k = 0; k<NCELLS; k=k+2)
          {
            temp_ = pointerToRxBytes_[data_counter++];
            temp2_ = (uint16_t)(pointerToRxBytes_[data_counter]&0x0F)<<8;
            _cellCodes[k] = temp_ + temp2_;
            _cellVoltage[k] = (_cellCodes[k] -512) * 1.5e-3;
            totalVoltage_ += _cellVoltage[k];
            temp2_ = (pointerToRxBytes_[data_counter++])>>4;
            temp_ =  (pointerToRxBytes_[data_counter++])<<4;
            _cellCodes[k+1] = temp_ + temp2_;
            _cellVoltage[k+1] = (_cellCodes[k+1] -512) * 1.5e-3;
            totalVoltage_ += _cellVoltage[k+1];
          }
    _totalVoltage = totalVoltage_;
    return 1;
  }
   else{ return 0; }
}



//-------------------------------------------------------------------------------------
//  Write command [0x80+ardes, PEC, Command, PEC]
void Bms::writeCommand(uint8_t command, uint8_t pec)
{
 SpiPacket packet(4, 0, _chipSelect, _spiMode, MODE_8BIT, _spiFrequency);
  
  osSemaphoreWait(packet.retSetmaphore(), osWaitForever);
  
  packet.putTxByte(0x80 + _chipAdres);
  packet.putTxByte(pec8_calc(1,packet.getPointerToTransmit()));
  packet.putTxByte(command);
  packet.putTxByte(pec);

  _spiHandler->transferMessage(&packet);

  osSemaphoreWait(packet.retSetmaphore(), osWaitForever);  
}


//-------------------------------------------------------------------------------------
//  Read Diagnostic Register
uint16_t Bms::readDiagnosticRefMux (void)
{
 uint8_t  pec_, recievePec_;
 uint16_t size_;
 uint8_t* pointerToRxBytes_;
 uint16_t refVoltage_;
 SpiPacket packet(4, 2+1, _chipSelect, _spiMode, MODE_8BIT, _spiFrequency);
  
  osSemaphoreWait(packet.retSetmaphore(), osWaitForever);
  
  packet.putTxByte(0x80 + _chipAdres);
  packet.putTxByte(pec8_calc(1,packet.getPointerToTransmit()));
  packet.putTxByte(RDDGNR);
  packet.putTxByte(RDDGNR_PEC);

  _spiHandler->transferMessage(&packet);

  osSemaphoreWait(packet.retSetmaphore(), osWaitForever);  
  
  //  Проверка PEC
  size_ = packet.getSizeOfRecieve()-1;
  pointerToRxBytes_ = packet.getPointerToRecieve() + packet.getSizeOfTransmit();
  
  pec_ = pec8_calc(size_,pointerToRxBytes_);
  recievePec_ = packet.getPointerToRecieve()[packet.getSizeOfTranscieve()-1];

  if(pec_ == recievePec_){
    _diagnosticRegister = ((pointerToRxBytes_[1] & 0xFF)<<8) | pointerToRxBytes_[0];
    refVoltage_ = ((pointerToRxBytes_[1] & 0x0F)<<8) | pointerToRxBytes_[0];

    if ((refVoltage_ < REF_VOLTAGE_2_1V)||(refVoltage_ > REF_VOLTAGE_2_9V)){
      _diagnosticRefFail =1;
    }

    _diagnosticMuxFail  = 1 && (pointerToRxBytes_[1] & BIT_5);
    return 1;
  }
   else{
     _diagnosticRefFail =1;
     _diagnosticMuxFail =1;
     return 0; 
   }
}


//-------------------------------------------------------------------------------------
//  Управление балансировкой
//  refVoltage - опорное напряжение на ячейке
//  cellDefference - разница напряжения, после которой включается балансировка ячейки
void Bms::balanceControl (float refVoltage, float cellDefference)
{
  uint16_t data_ =0;
  uint16_t i_;
  uint16_t size_;
  float meanCellVoltage;
//  float minCellVoltage =5;
  
  size_ = countof(_cellVoltage);

  // Находжение минимального напряжения в ячейках
  //
/*  for (i_=0; i_<size_; i_++){
   if (_cellVoltage[i_] < minCellVoltage) {
     minCellVoltage = _cellVoltage[i_]; 
   }
  }
  
  // Поиск ячеек, которые нуждаются в балансировке
  //
  for (i_=0; i_<size_; i_++){
   if (_cellVoltage[i_] - minCellVoltage > cellDefference) {
     data_ |= 1<<i_;
   }
  }*/


  // Поиск ячеек, которые нуждаются в балансировке
  //
  for (i_=0; i_<size_; i_++){
   if (_cellVoltage[i_] - refVoltage > cellDefference) {
     data_ |= 1<<i_;
   }
  }
  data_ = 0xFFF;
  _configRegisterGroup.registers[1]  = data_ & 0xFF;
  _configRegisterGroup.registers[2] &= 0xF0;
  _configRegisterGroup.registers[2] |= (data_ & 0xF00)>>8;
  
  writeConfigRegisterGroup();

  setBalanceByte (data_);            // Для отладки. Удалить
  if (data_ != 0){
//    printf ("Balance Cell %#X\n", data_);
  }

}


//-------------------------------------------------------------------------------------
//  Управление разрядом
void Bms::dischargeControl (float  maxCellVoltage)
{
  uint8_t  data_ =0xFF;
  uint16_t cells_ =0;
  uint16_t i_;
  //uint16_t j_;
  bool     cellPair1_;  // Первая ячейка разряжаемой пары
  bool     cellPair2_;  // Вторая ячейка разряжаемой пары
  uint16_t size_;
  
  size_ = countof(_cellVoltage);
  
  // Находжение ячеек, напряжение которых превышает величину maxCellVoltage
  //
  for (i_=0; i_<size_; i_++){
   if (_cellVoltage[i_] > maxCellVoltage) {
     cells_ |= 1<<i_; 
   }
  }
  
  // Установка пинов на разряд ячеек (по 2 ячейки на пин)
  //
  for (i_=0; i_<(size_>>1); i_++) {
    cellPair1_ = (bool) (cells_ & (1<<(i_ * 2)));
    cellPair2_ = (bool) (cells_ & (1<<(i_ * 2 + 1)));
    if ((cellPair1_==true) || (cellPair2_==true)) {
      data_ &= ~(1<<i_); 
    }
  }
  
  spiPortPic->writeRegister (data_, 0x14); // OLATA

  setDischargeByte (0xFF - data_);          // Для отладки. Удалить

  if (data_ != 0xFF){
//    printf ("Discharge Cell %#X\n", data_);
  }
}


//-------------------------------------------------------------------------------------
//  Управление шунтированием
void Bms::bypassControl (uint16_t  data)
{
  spiPortPic->writeRegister (data, 0x15); // OLATB
}


//-------------------------------------------------------------------------------------
//  Расчет контрольной суммы
//-------------------------------------------------------------------------------------
uint8_t Bms::pec8_calc(uint8_t len, uint8_t *data)
{
  uint8_t  remainder = 0x41;//PEC_SEED;
  /*
   * Perform modulo-2 division, a byte at a time.
   */
  for (int byte = 0; byte < len; ++byte)
  {
    /*
     * Bring the next byte into the remainder.
     */
    remainder ^= data[byte];

    /*
     * Perform modulo-2 division, a bit at a time.
     */
    for (uint8_t bit = 8; bit > 0; --bit)
    {
      /*
       * Try to divide the current data bit.
       */
      if (remainder & 128)
      {
        remainder = (remainder << 1) ^ 7;
      }
      else
      {
        remainder = (remainder << 1);
      }
    }
  }

  /*
   * The final remainder is the CRC result.
   */
  return (remainder);
}


//--------------------------------------------------------------------------------------------
//  Класс, объединяющий все модули BMS

//  Добавить модуль в карту
void BmsAssembly::addModule (Bms* module)
{
  _bmsModulesMap[module->getChipSelect()] = module;
  _size = _bmsModulesMap.size();
}

//  Устанавливает указатель на объект Bms с соответствующим идентификатором
//  Возвращает TRUE, если объект найден
uint16_t BmsAssembly::getModule (uint16_t id, Bms*& module )
{
  _i = _bmsModulesMap.find(id);
  if(_i == _bmsModulesMap.end()){
    // Такого адреса нет в таблице
    return pdFALSE;
  }
   else{ //return (*_i).second; 
     module = (*_i).second;
     return pdTRUE;
   }
}


//-------------------------------------------------------------------------------------
//  Инициализация всех PIC-портов
void BmsAssembly::portInit (void)
{
  for(_i=_bmsModulesMap.begin(); _i!=_bmsModulesMap.end(); _i++){
    (*_i).second->spiPortPic->init();
  }
}


//-------------------------------------------------------------------------------------
//  Управление балансировкой. refVoltage - опорное напряжение на ячейке. cellDefference - разница напряжения, после которой включается балансировка ячейки
void BmsAssembly::balanceControl(float refVoltage, float cellDefference)
{
  for(_i=_bmsModulesMap.begin(); _i!=_bmsModulesMap.end(); _i++){
    (*_i).second->balanceControl(refVoltage, cellDefference);
  }
}
//-------------------------------------------------------------------------------------
void BmsAssembly::dischargeControl (float maxCellVoltage)  // Управление разрядом. 
{
//  float maxCellVoltage_;
  
//  maxCellVoltage_ = maxTotalVoltage/(_bmsModulesMap.size() * NCELLS);
  for(_i=_bmsModulesMap.begin(); _i!=_bmsModulesMap.end(); _i++){
    (*_i).second->dischargeControl(maxCellVoltage);
  }
}
//-------------------------------------------------------------------------------------
void BmsAssembly::bypassControl (uint16_t  data)  // Управление шунтированием. 
{
//  for(_i=_bmsModulesMap.begin(); _i!=_bmsModulesMap.end(); _i++){
//    (*_i).second->bypassControl(maxCellVoltage);
//  }
}

//-------------------------------------------------------------------------------------
// Управление балансировкой. Значение разницы напряжений между модулями
// Модули, напряжение которых выше минимального на moduleDefference, разряжаются.
// Разряд осуществляется путем разряда ячеек в модуле до заданного(рассчитанного) напряжения ограничения.
void BmsAssembly::balanceIntermoduleControl (float moduleDefference)
{
  float    minimal_ =10000;
//  uint16_t module_;
  float cellVoltage_;  // Среднее напряжение на конденсаторе. Устанавливается в разряжаемых модулях.
  float moduleVoltage_;

  //  Поик минимального напряжения модуля
  for(_i=_bmsModulesMap.begin(); _i!=_bmsModulesMap.end(); _i++){
    if ( (*_i).second->getTotalVoltage() < minimal_){
      minimal_ = (*_i).second->getTotalVoltage();
    }
  }

  // Напряжение ограничения на ячейке
  cellVoltage_ = (minimal_ + moduleDefference)/NCELLS;
  
  // Поиск модулей, напряжение которых выше минимального на moduleDefference вольт. И включение их разряда.
  for(_i=_bmsModulesMap.begin(); _i!=_bmsModulesMap.end(); _i++){
    moduleVoltage_ = (*_i).second->getTotalVoltage();
    printf("BMS %d",(*_i).first);
    if ( (moduleVoltage_ - minimal_) > moduleDefference ){
      (*_i).second->dischargeControl(cellVoltage_);
      printf(" disch cell.    Module= %f, Different= %f, Cell Limit= %f\n",moduleVoltage_ ,(moduleVoltage_ - minimal_), cellVoltage_);
    }
      else{
        printf(" No disch cell. Module = %f V\n", moduleVoltage_); 
        (*_i).second->dischargeControl( (*_i).second->getMaxCellVoltage() );
      }
      
  }
}

//-------------------------------------------------------------------------------------
void BmsAssembly::writeConfigRegisterGroup(void)
{
  for(_i=_bmsModulesMap.begin(); _i!=_bmsModulesMap.end(); _i++){
    (*_i).second->writeConfigRegisterGroup();
  }
}


//-------------------------------------------------------------------------------------
//  Запуск теста опоры и мультиплексора на всех модулях BMS
void BmsAssembly::startDiagnosticRefMux(void)
{
  for(_i=_bmsModulesMap.begin(); _i!=_bmsModulesMap.end(); _i++){
    (*_i).second->startDiagnosticRefMux();
  }
}

//-------------------------------------------------------------------------------------
//  Чтение результатов теста опоры и мультиплексора на всех модулях BMS
void BmsAssembly::readDiagnosticRefMux(void)
{
  for(_i=_bmsModulesMap.begin(); _i!=_bmsModulesMap.end(); _i++){
    (*_i).second->readDiagnosticRefMux();
  }
}


//-------------------------------------------------------------------------------------
//  Диагностика обрыва измеряющей цепи 
void BmsAssembly::diagnosticOpenWireAdc(void)
{
  for(_i=_bmsModulesMap.begin(); _i!=_bmsModulesMap.end(); _i++){
    (*_i).second->diagnosticOpenWireAdc();
  }
}


//-------------------------------------------------------------------------------------
void BmsAssembly::startAdcTest1	(void)           //  Старт "Тест 1" АЦП 
{
  for(_i=_bmsModulesMap.begin(); _i!=_bmsModulesMap.end(); _i++){
    (*_i).second->startAdcTest1();
  }
}


//-------------------------------------------------------------------------------------
void BmsAssembly::startAdcTest2	(void)           //  Старт "Тест 2" АЦП 
{
  for(_i=_bmsModulesMap.begin(); _i!=_bmsModulesMap.end(); _i++){
    (*_i).second->startAdcTest2();
  }
}


//-------------------------------------------------------------------------------------
//  Возвращает ошибку CRC. Номер бита соответствует номеру неисправного модуля
uint16_t BmsAssembly::readAdcTest1	(void)           //  Чтение "Тест 1" АЦП 
{
  uint16_t result_ =0;
  for(_i=_bmsModulesMap.begin(); _i!=_bmsModulesMap.end(); _i++){
    if ((*_i).second->readAdcTest1() == pdFALSE){
     result_ |= 1<<(*_i).first;
    }
  }
  return result_;
}


//-------------------------------------------------------------------------------------
//  Возвращает ошибку CRC
//  Возвращает ошибку CRC. Номер бита соответствует номеру неисправного модуля
uint16_t BmsAssembly::readAdcTest2	(void)           //  Чтение "Тест 1" АЦП 
{
  uint16_t result_ =0;
  for(_i=_bmsModulesMap.begin(); _i!=_bmsModulesMap.end(); _i++){
    if ((*_i).second->readAdcTest2() == pdFALSE){
     result_ |= 1<<(*_i).first;
    }
  }
  return result_;
}


//-------------------------------------------------------------------------------------
// Результаты самодиагностики
//  Возвращает байт ошибок. Номер бита соответствует номеру неисправного модуля
uint16_t BmsAssembly::getDiagnosticMuxFail (void)
{
  uint16_t result_ =0;
  for(_i=_bmsModulesMap.begin(); _i!=_bmsModulesMap.end(); _i++){
    if ((*_i).second->getDiagnosticMuxFail() == pdTRUE){  //  Мультиплексор не исправен
      result_ |= 1<<(*_i).first;
    }
  }
  return result_;
}


//-------------------------------------------------------------------------------------
//  Возвращает байт ошибок. Номер бита соответствует номеру неисправного модуля
uint16_t BmsAssembly::getDiagnosticRefFail (void)
{
  uint16_t result_ =0;
  for(_i=_bmsModulesMap.begin(); _i!=_bmsModulesMap.end(); _i++){
    if ((*_i).second->getDiagnosticRefFail() == pdTRUE){  //  Опора не исправна
      result_ |= 1<<(*_i).first;
    }
  }
  return result_;
}


//-------------------------------------------------------------------------------------
//  Возвращает байт ошибок. Номер бита соответствует номеру неисправного модуля
uint16_t BmsAssembly::getDiagnosticOpenWire (void)
{
  uint16_t result_ =0;
  for(_i=_bmsModulesMap.begin(); _i!=_bmsModulesMap.end(); _i++){
    if ((*_i).second->getDiagnosticOpenWire() == pdTRUE){
      result_ |= 1<<(*_i).first;
    }
  }
  return result_;
}


//-------------------------------------------------------------------------------------
//  Возвращает байт ошибок. Номер бита соответствует номеру неисправного модуля
uint16_t BmsAssembly::getDiagnosticAdcTest1Fail (void)
{
  uint16_t result_ =0;
  for(_i=_bmsModulesMap.begin(); _i!=_bmsModulesMap.end(); _i++){
    if ((*_i).second->getDiagnosticAdcTest1Fail() == pdTRUE){
      result_ |= 1<<(*_i).first;
    }
  }
  return result_;
}


//-------------------------------------------------------------------------------------
//  Возвращает байт ошибок. Номер бита соответствует номеру неисправного модуля
uint16_t BmsAssembly::getDiagnosticAdcTest2Fail (void)
{
  uint16_t result_ =0;
  for(_i=_bmsModulesMap.begin(); _i!=_bmsModulesMap.end(); _i++){
    if ((*_i).second->getDiagnosticAdcTest2Fail() == pdTRUE){
      result_ |= 1<<(*_i).first;
    }
  }
  return result_;
}


//-------------------------------------------------------------------------------------
//  Read Configuration Register Group
//  Возвращает байт ошибок. Номер бита соответствует номеру неисправного модуля
uint16_t BmsAssembly::readConfigRegisterGroup (void)
{
  uint16_t result_ =0;
  for(_i=_bmsModulesMap.begin(); _i!=_bmsModulesMap.end(); _i++){
    if ((*_i).second->readConfigRegisterGroup() == pdFALSE){
      result_ |= 1<<(*_i).first;
    }
  }
  return result_;
}

//-------------------------------------------------------------------------------------
//  Сравнение конфигурации прочитанной с текущей
//  Возвращает байт несоответствия регистров. Номер бита соответствует номеру модуля
uint16_t BmsAssembly::compareConfigRegisterGroup (void)
{
  uint16_t result_ =0;
  for(_i=_bmsModulesMap.begin(); _i!=_bmsModulesMap.end(); _i++){
    if ((*_i).second->compareConfigRegisterGroup() == pdFALSE){
      result_ |= 1<<(*_i).first;
    }
  }
  return result_;
}


//-------------------------------------------------------------------------------------
//  Function that starts Cell Voltage measurement
void BmsAssembly::startCellVoltageMeasurement (void)
{
  for(_i=_bmsModulesMap.begin(); _i!=_bmsModulesMap.end(); _i++){
    (*_i).second->startCellVoltageMeasurement();
  }
}


//-------------------------------------------------------------------------------------
//  Read All Cell Voltage Group
uint16_t BmsAssembly::readCellVoltage (void)
{
  uint16_t result_ =0;
  float totalVoltage_ =0;

  for(_i=_bmsModulesMap.begin(); _i!=_bmsModulesMap.end(); _i++){
    if ((*_i).second->readCellVoltage() == pdFALSE){
      result_ |= 1<<(*_i).first;
    }
    totalVoltage_ += (*_i).second->getTotalVoltage();
  }
  _totalVoltage = totalVoltage_;
  return result_;
}


//-------------------------------------------------------------------------------------
//  Read Flag Register Group
uint16_t BmsAssembly::readFlagRegisters (void)
{
  uint16_t result_ =0;
  for(_i=_bmsModulesMap.begin(); _i!=_bmsModulesMap.end(); _i++){
    if ((*_i).second->readFlagRegisters() == pdFALSE){
      result_ |= 1<<(*_i).first;
    }
  }
  return result_;
}
//-------------------------------------------------------------------------------------

void BmsAssembly::printDiagnostic (void)
{
  for(_i=_bmsModulesMap.begin(); _i!=_bmsModulesMap.end(); _i++){
    printf("BMS-%d Mux       ", (*_i).second->getChipSelect());
      if ((*_i).second->getDiagnosticMuxFail() == pdTRUE)     { printf("-ERROR\n"); } else { printf("-OK\n"); }
    printf("BMS-%d Ref       ", (*_i).second->getChipSelect());
      if ((*_i).second->getDiagnosticRefFail() == pdTRUE)     { printf("-ERROR\n"); } else { printf("-OK\n"); }
    printf("BMS-%d Open Wire ", (*_i).second->getChipSelect()); 
      if ((*_i).second->getDiagnosticOpenWire())              { printf("-ERROR\n"); } else { printf("-OK\n"); }
    printf("BMS-%d ADC Test1 ", (*_i).second->getChipSelect());
      if ((*_i).second->getDiagnosticAdcTest1Fail() == pdTRUE){ printf("-ERROR\n"); } else { printf("-OK\n"); }
    printf("BMS-%d ADC Test2 ", (*_i).second->getChipSelect());
      if ((*_i).second->getDiagnosticAdcTest2Fail() == pdTRUE){ printf("-ERROR\n"); } else { printf("-OK\n\n"); }
  }
}

//-------------------------------------------------------------------------------------
float BmsAssembly::getCellVoltage (uint16_t module, uint8_t cell)
{
  return _bmsModulesMap[module]->getCellVoltage(cell);
}
//-------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------
/*uint8_t check_pec (ISpiMessage* packet)
{
}*/

