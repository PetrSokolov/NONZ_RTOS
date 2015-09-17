

#include "..\_Objects_Definitions\__ObjectsDefinitions.hpp"
#include "..\_Objects_Definitions\__ConstantsDefinitions.hpp"

#include <stdio.h>

uint8_t picRegLatA, picRegLatB, picRegDirA, picRegDirB, picRegPortA, picRegPortB, onPwm=0, offPwm=0;

void StartTask500mls(void const * argument)
{
  
  //  Сборка модулей BMS
  bmsAssembly.addModule(&bms0);
  bmsAssembly.addModule(&bms1);
  bmsAssembly.addModule(&bms2);
  bmsAssembly.addModule(&bms3);
  bmsAssembly.addModule(&bms4);
  bmsAssembly.addModule(&bms5);
  bmsAssembly.addModule(&bms6);
  bmsAssembly.addModule(&bms7);
  bmsAssembly.addModule(&bms8);
  bmsAssembly.addModule(&bms9);
  bmsAssembly.addModule(&bms10);
  //------------------------------------------------------------------------
  //  Инициализация расширителей портов
    bmsAssembly.portInit();
  //------------------------------------------------------------------------
  //  Инициализация и тестирование BMS
 
  //  Запись конфигурационных регистров
    bmsAssembly.writeConfigRegisterGroup();
    osDelay(20);

  //  Диагностика опорного паряжения и мультиплексора
    bmsAssembly.startDiagnosticRefMux();
    osDelay(20);

    bmsAssembly.readDiagnosticRefMux();

  //  Проверка обрыва цепи измерения напряжения банок
    bmsAssembly.diagnosticOpenWireAdc();

  //  Проверка АЦП
    // Тест 1
    bmsAssembly.startAdcTest1();
    osDelay(20);
    bmsAssembly.readAdcTest1();

    // Тест 2
    bmsAssembly.startAdcTest2();
    osDelay(20);
    bmsAssembly.readAdcTest2();
    
    //  Результаты самодиагностики
    bmsAssembly.printDiagnostic();

    //  Окончание самодиагностики BMS

    //  Старт преобразования
    bmsAssembly.startCellVoltageMeasurement();
    // Задержка на преобразования ADC
    osDelay(20);


  for(;;)
  {
    osSemaphoreWait(binarySemTSK500mls, osWaitForever);
//    printf("TSK500 start\n");

    // Обработка принятых команд ПДУ
    rc.handler();
    // Обработка контроллера меню
    menuEngine.handler();

    // Обмен информацией с BMS
    bmsAssembly.readConfigRegisterGroup();
    bmsAssembly.readCellVoltage();
    bmsAssembly.readFlagRegisters();
    bmsAssembly.startCellVoltageMeasurement();
    
    // Инициализация параметров, отображающих в меню значения датчиков напряжения, тока
    vUDcBus.setValue( uDcBusSensor.getValue() );
    vUCharge.setValue( uChargeSensor.getValue() );
    vICharge.setValue( iChargeSensor.getValue() );

    // Обработка калибровки датчиков
    if (vUDcBusCodeUCal.getCalibratingState()==TRUE){
      vUDcBusCodeUCal.setAutocalibratingValue(uDcBusSensor.getMean());
    }
    if (vUChargeCodeUCal.getCalibratingState()==TRUE){
      vUChargeCodeUCal.setAutocalibratingValue(uChargeSensor.getMean());
    }
    if (vIChargeCodeICal.getCalibratingState()==TRUE){
      vIChargeCodeICal.setAutocalibratingValue(iChargeSensor.getMean());
    }
    
    if (vUDcBusCodeZero.getCalibratingState()==TRUE){
      vUDcBusCodeZero.setAutocalibratingValue(uDcBusSensor.getMean());
    }
    if (vUChargeCodeZero.getCalibratingState()==TRUE){
      vUChargeCodeZero.setAutocalibratingValue(uChargeSensor.getMean());
    }
    if (vIChargeCodeZero.getCalibratingState()==TRUE){
      vIChargeCodeZero.setAutocalibratingValue(iChargeSensor.getMean());
    }
    
//    if (onPwm) {  pwm.start(); onPwm =0; }
//    if (offPwm) {  pwm.stop(); offPwm =0; }
    
    // Балансировка
    printf("Balance Control %f\n", (bmsAssembly.getTotalVoltage()/(bmsAssembly.getsize()*NCELLS)));
    
    bmsAssembly.balanceControl(bmsAssembly.getTotalVoltage()/(bmsAssembly.getsize()*NCELLS), 0.05);
    
//    uint16_t* array;
//    array = new uint16_t [100];
//    array[0] = 100;
//    printf("Balance Control %d\n", array[0]);

//    SpiPacket packet((uint16_t)100, (uint16_t)0, (uint8_t)1, SPI_MODE_0, MODE_8BIT, (uint16_t)600);
//    packet.putTxByte(1);
    // Разряд
//    if (bmsAssembly.getTotalVoltage() < (float)310.0){
//      bmsAssembly.balanceIntermoduleControl(0.3);
//    }
//      else{
        bmsAssembly.dischargeControl(2.6);
//      }
/*    uint16_t bufer[10] = {0,1,2,3,4,5,6,7,8,9};
    ByteStoragePacket packet(bufer, 5, 0, ACTION_READ);
     osSemaphoreWait(packet.retSetmaphore(), osWaitForever);

    printf("Byte Storage Packet send in BSController\n");
    byteStorageController.transferPacket(&packet);
    printf("Byte Storage Packet waiting semaphore\n");
     osSemaphoreWait(packet.retSetmaphore(), osWaitForever);
*/
    osSemaphoreRelease(binarySemTSK1000mls);
//    printf("TSK500 stop\n\n");

  }
}

