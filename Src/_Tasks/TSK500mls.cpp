

#include "..\_Objects_Definitions\__ObjectsDefinitions.hpp"
#include "..\_Objects_Definitions\__ConstantsDefinitions.hpp"

#include <stdio.h>

uint8_t picRegLatA, picRegLatB, picRegDirA, picRegDirB, picRegPortA, picRegPortB;

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

  uDcBusCalibrationCoeff.setValue((float)10);
  uChargeCalibrationCoeff.setValue((float)10);
  iChargeCalibrationCoeff.setValue((float)10);
  
  uDcSensor.setCalibration( uDcBusCalibrationCoeff.getValueFlt() );
  uChargeSensor.setCalibration( uChargeCalibrationCoeff.getValueFlt() );
  iChargeSensor.setCalibration( iChargeCalibrationCoeff.getValueFlt() );


  for(;;)
  {
    osSemaphoreWait(binarySemTSK500mls, osWaitForever);

    rc.handler();
    menuEngine.handler();


    bmsAssembly.readConfigRegisterGroup();
    bmsAssembly.readCellVoltage();
    bmsAssembly.readFlagRegisters();
    bmsAssembly.startCellVoltageMeasurement();
    
//    uDcBusCalibrationCoeff.setValue((float)10);
    
//    uDcSensor.setCalibration( uDcBusCalibrationCoeff.getValueFlt() );
//    printf ("uDcValue = %f\n", uDcSensor.getValue());
    uDcBus.setValue( uDcSensor.getValue() );
//    printf("DC Bus Voltage = %f\n", uDcBus.getValueFlt());
//    printf ("CalValue(int) =%d, (float) =%f\n", uDcBusCalibrationCoeff.getValue(), uDcBusCalibrationCoeff.getValueFlt());
//    printf ("CalValue(int) =%d, (float) =%f\n", uDcBusCalibrationCoeff.getValue(), uDcBusCalibrationCoeff.getValueFlt());

    if (uDcBusCalibrationCoeff.getCalibratingState()==TRUE){
      printf("U DC Bus AutoCal K = %f\n", uDcSensor.getCalibration());
      printf("CalVoltage    = %d\n", uDcBusCalibratingVoltage.getValue());
      printf("SensorVoltage = %f\n", uDcSensor.getValue());
      printf("Cal Err (int)   = %d\n", uDcBusCalibrationCoeff.getValue());
      printf("Cal Err (float) = %f\n", uDcBusCalibrationCoeff.getValueFlt());

      float error_ = uDcBusCalibratingVoltage.getValue()/uDcSensor.getValue();
      if (error_ < 0.1) {error_ = 0.1;}
      if (error_ > 2)   {error_ = 2;}

//      uDcBusCalibrationCoeff.setCalibrationError( 1 - (uDcBusCalibratingVoltage.getValue()/uDcSensor.getValue() ) );
      uDcBusCalibrationCoeff.setCalibrationError( 1 - error_ );
      uDcSensor.setCalibration( uDcSensor.getCalibration() * (1 - uDcBusCalibrationCoeff.getCalibrationError() ) );
      uDcBusCalibrationCoeff.setAutocalibratingValue(uDcSensor.getCalibration());
    }

    if (uChargeCalibrationCoeff.getCalibratingState()==TRUE){
      printf("uCharge AutoCal K = %f\n", uChargeSensor.getCalibration());
      printf("CalVoltage    = %d\n", uDcBusCalibratingVoltage.getValue());
      printf("SensorVoltage = %f\n", uChargeSensor.getValue());
      printf("Cal Err (int)   = %d\n", uChargeCalibrationCoeff.getValue());
      printf("Cal Err (float) = %f\n", uChargeCalibrationCoeff.getValueFlt());
      
      float error_ = uDcBusCalibratingVoltage.getValue()/uDcSensor.getValue();
      if (error_ < 0.1) {error_ = 0.1;}
      if (error_ > 2)   {error_ = 2;}

//      uChargeCalibrationCoeff.setCalibrationError( 1 - (uDcBusCalibratingVoltage.getValue()/ uChargeSensor.getValue() ) );
      uChargeCalibrationCoeff.setCalibrationError( 1 - error_ );
      uChargeSensor.setCalibration( uChargeSensor.getCalibration() * (1 - uChargeCalibrationCoeff.getCalibrationError() ) );
      uChargeCalibrationCoeff.setAutocalibratingValue(uChargeSensor.getCalibration());
    }
    
//    bms0.dischargeControl(0xAA);
//    bms0.closingControl(0xAA);
    
/*    mram.resetWriteProtect();
    mram.writeEnable();
    mram.readStatusRegister();
    mram.writeDataBytes();
    mram.readDataBytes();*/
/*    bms0.dischargeControl(0x0);
    bms1.dischargeControl(0);
    bms2.dischargeControl(0);
    bms3.dischargeControl(0);
    bms4.dischargeControl(0);
    bms5.dischargeControl(0);
    bms6.dischargeControl(0);
    bms7.dischargeControl(0);
    bms8.dischargeControl(0);
    bms9.dischargeControl(0);
    bms10.dischargeControl(0);

    bms0.closingControl(0);
    bms1.closingControl(0);
    bms2.closingControl(0);
    bms3.closingControl(0);
    bms4.closingControl(0);
    bms5.closingControl(0);
    bms6.closingControl(0);
    bms7.closingControl(0);
    bms8.closingControl(0);
    bms9.closingControl(0);
    bms10.closingControl(0);*/

/*    bms0.balanceControl(0xFFF);
    bms1.balanceControl(0xFFF);
    bms2.balanceControl(0xFFF);
    bms3.balanceControl(0xFFF);
    bms4.balanceControl(0xFFF);
    bms5.balanceControl(0xFFF);
    bms6.balanceControl(0xFFF);
    bms7.balanceControl(0xFFF);
    bms8.balanceControl(0xFFF);
    bms9.balanceControl(0xFFF);
    bms10.balanceControl(0xFFF);*/

  }
}

