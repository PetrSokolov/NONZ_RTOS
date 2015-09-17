

#include "..\_Objects_Definitions\__ObjectsDefinitions.hpp"
#include "..\_Objects_Definitions\__ConstantsDefinitions.hpp"

#include <stdio.h>


__STATIC_INLINE uint32_t ITM_SendCharX (uint32_t Channel, uint32_t Data)
{
  if ((ITM->TCR & ITM_TCR_ITMENA_Msk) &&      // ITM enabled
      (ITM->TER & (1UL << Channel) ) )        // ITM Port X enabled
  {
    while (ITM->PORT[Channel].u32 == 0);
    ITM->PORT[Channel].u8 = (uint8_t) Data;
  }
  return (Data);
}


char transferChars[250];  // Возможно превышает кучу, сваливалось в хардфаулт
uint16_t timeCharge=0, size;

float pwmval =0;
float bms1Voltage,bms2Voltage,bms3Voltage,bms4Voltage,bms5Voltage,bms6Voltage,bms7Voltage,bms8Voltage,bms9Voltage,bms10Voltage,bms11Voltage,bms12Voltage;

void StartTask1000mls(void const * argument)
{

 uint8_t bmsNr;
 Bms* bms;
  
  for(;;)
  {
    
    osSemaphoreWait(binarySemTSK1000mls, osWaitForever);
//     printf("TSK1000 start\n");
   
    pwmval =0.5;
    pwm.setValue(pwmval);

    //ITMLOG 1 >D:\BMS3_2A.csv   
//    printf("FreeHeapSize = %d\n", xPortGetFreeHeapSize());
//    printf("MinimumEverFreeHeapSize = %d\n", xPortGetMinimumEverFreeHeapSize());
//    printf("%s High Watermark = %d\n", pcTaskGetTaskName(Task1000mlsHandle), uxTaskGetStackHighWaterMark(Task1000mlsHandle));
//    printf("Process...\n");
    

   timeCharge++;
   bms = &bms1;
   bmsNr =0;
     

// Глобальные переменные для отладки    
  bms1Voltage = bms0.getTotalVoltage();
  bms2Voltage = bms1.getTotalVoltage();
  bms3Voltage = bms2.getTotalVoltage();
  bms4Voltage = bms3.getTotalVoltage();
  bms5Voltage = bms4.getTotalVoltage();
  bms6Voltage = bms5.getTotalVoltage();
  bms7Voltage = bms6.getTotalVoltage();
  bms8Voltage = bms7.getTotalVoltage();
  bms9Voltage = bms8.getTotalVoltage();
  bms10Voltage = bms9.getTotalVoltage();
  bms11Voltage = bms10.getTotalVoltage();
  bms12Voltage = 0;

   size = sprintf(transferChars, "%d, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f", timeCharge, bms1Voltage, bms2Voltage, bms3Voltage, bms4Voltage, bms5Voltage, bms6Voltage, bms7Voltage, bms8Voltage, bms9Voltage, bms10Voltage, bms11Voltage, bms12Voltage);
     if (size>0){
       for(int i=0; i<size; i++){
         ITM_SendCharX (bmsNr+1, transferChars[i]);
       }
       ITM_SendCharX (bmsNr+1, '\n');
     }
       osDelay(10);

     size = sprintf(transferChars, "%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d",
            timeCharge, 1&&(bms->getBalanceByte()&BIT_0), 1&&(bms->getBalanceByte()&BIT_1), 1&&(bms->getBalanceByte()&BIT_2), 1&&(bms->getBalanceByte()&BIT_3), 1&&(bms->getBalanceByte()&BIT_4), 1&&(bms->getBalanceByte()&BIT_5), 1&&(bms->getBalanceByte()&BIT_6), 1&&(bms->getBalanceByte()&BIT_7), 1&&(bms->getBalanceByte()&BIT_8), 1&&(bms->getBalanceByte()&BIT_9), 1&&(bms->getBalanceByte()&BIT_10), 1&&(bms->getBalanceByte()&BIT_11), 
                        1&&(bms->getDischargeByte()&BIT_0), 1&&(bms->getDischargeByte()&BIT_1), 1&&(bms->getDischargeByte()&BIT_2), 1&&(bms->getDischargeByte()&BIT_3), 1&&(bms->getDischargeByte()&BIT_4), 1&&(bms->getDischargeByte()&BIT_5) );
     if (size>0){
       for(int i=0; i<size; i++){
         ITM_SendCharX (bmsNr+2, transferChars[i]);
       }
       ITM_SendCharX (bmsNr+2, '\n');
     }
//    printf("TSK1000 stop\n\n");

  }
}

