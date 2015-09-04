

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

//int16_t numbers=0, pages=0, p1=0, p2=0, p3=0;
//char names[20];
float pwmval =0;
float bms1Voltage,bms2Voltage,bms3Voltage,bms4Voltage,bms5Voltage,bms6Voltage,bms7Voltage,bms8Voltage,bms9Voltage,bms10Voltage,bms11Voltage,bms12Voltage;

void StartTask1000mls(void const * argument)
{

//  char transferChars[128];  // Возможно превышает кучу, сваливалось в хардфаулт
//  uint16_t timeCharge=0, size;
//  float valueCharge = 1.234;
 uint8_t bmsNr;
 Bms* bms;
  
  
  for(;;)
  {
    
    osSemaphoreWait(binarySemTSK1000mls, osWaitForever);
    
    //pwmval += 0.01;
    pwmval =0.5;
//    if (pwmval>0.8) {pwmval = 0.1;}
    pwm.setValue(pwmval);
//    printf("1000mls\n");
//    printf("Size of variables %d\n", containerOfVariables.getCountOfContents());
//    printf("id 202 =  %d\n", containerOfVariables.getContent(202)->getValue());

    //ITMLOG 1 >D:\BMS3_2A.csv   
/*    p3 = (numbers-6-90*2)/3;       if (p3<0) p3=0;
    p2 = (numbers - p3*3 - 6)/2;   if (p2<0) p2=0;
    p1 = (numbers - p3*3 - p2*2);

    printf("numbers of Digits = %d,\n", numbers);
    printf("p1 = %d, p2 = %d, p3 = %d\n", p1, p2, p3);
    printf("Totel Pages = %d\n\n", 3+p1+p2+p3);
*/    
   timeCharge++;
   bms = &bms1;
   bmsNr =0;
     
    
//   for (bmsNr =0; bmsNr<11; bmsNr++){
//     switch(bmsNr){
//       case 0:  {bms = &bms0;}break;
//       case 1:  {bms = &bms1;}break;
//       case 2:  {bms = &bms2;}break;
//       case 3:  {bms = &bms3;}break;
//       case 4:  {bms = &bms4;}break;
//       case 5:  {bms = &bms5;}break;
//       case 6:  {bms = &bms6;}break;
//       case 7:  {bms = &bms7;}break;
//       case 8:  {bms = &bms8;}break;
//       case 9:  {bms = &bms9;}break;
//       case 10: {bms = &bms10;}break;
//     }

//     size = sprintf(transferChars, "%d, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f", timeCharge, bms->getCellVoltage(0),bms->getCellVoltage(1),bms->getCellVoltage(2),bms->getCellVoltage(3),bms->getCellVoltage(4),bms->getCellVoltage(5),bms->getCellVoltage(6),bms->getCellVoltage(7),bms->getCellVoltage(8),bms->getCellVoltage(9),bms->getCellVoltage(10),bms->getCellVoltage(11));
//   size = sprintf(transferChars, "%d, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f", timeCharge, bms0->getModuleVoltage(),bms1->getModuleVoltage(),bms2->getModuleVoltage(),bms3->getModuleVoltage(),bms4->getModuleVoltage(),bms5->getModuleVoltage(),bms6->getModuleVoltage(),bms7->getModuleVoltage(),bms8->getModuleVoltage(),bms9->getModuleVoltage(),bms10->getModuleVoltage(),bms11->getModuleVoltage());
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
//       osDelay(1);
     }
  //   }
       osDelay(10);

     size = sprintf(transferChars, "%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d",
            timeCharge, 1&&(bms->getBalanceByte()&BIT_0), 1&&(bms->getBalanceByte()&BIT_1), 1&&(bms->getBalanceByte()&BIT_2), 1&&(bms->getBalanceByte()&BIT_3), 1&&(bms->getBalanceByte()&BIT_4), 1&&(bms->getBalanceByte()&BIT_5), 1&&(bms->getBalanceByte()&BIT_6), 1&&(bms->getBalanceByte()&BIT_7), 1&&(bms->getBalanceByte()&BIT_8), 1&&(bms->getBalanceByte()&BIT_9), 1&&(bms->getBalanceByte()&BIT_10), 1&&(bms->getBalanceByte()&BIT_11), 
                        1&&(bms->getDischargeByte()&BIT_0), 1&&(bms->getDischargeByte()&BIT_1), 1&&(bms->getDischargeByte()&BIT_2), 1&&(bms->getDischargeByte()&BIT_3), 1&&(bms->getDischargeByte()&BIT_4), 1&&(bms->getDischargeByte()&BIT_5) );
//     bms->getDischargeByte()
     if (size>0){
       for(int i=0; i<size; i++){
         ITM_SendCharX (bmsNr+2, transferChars[i]);
       }
       ITM_SendCharX (bmsNr+2, '\n');
//       osDelay(1);
     }

  }
}

