

#include "..\_Objects_Definitions\__ObjectsDefinitions.hpp"
#include "..\_Objects_Definitions\__ConstantsDefinitions.hpp"

#include <stdio.h>


__STATIC_INLINE uint32_t ITM_SendCharX (uint32_t Channel, uint32_t Data)
{
  if ((ITM->TCR & ITM_TCR_ITMENA_Msk) &&      /* ITM enabled */
      (ITM->TER & (1UL << Channel) ) )        /* ITM Port X enabled */
  {
    while (ITM->PORT[Channel].u32 == 0);
    ITM->PORT[Channel].u8 = (uint8_t) Data;
  }
  return (Data);
}


char transferChars[200];  // Возможно превышает кучу, сваливалось в хардфаулт
uint16_t timeCharge=0, size;

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
    printf("Task 1000 begin\n");

/*   ITM_SendCharX (1, 1);
   ITM_SendCharX (2, 2);
   ITM_SendCharX (3, 3);
   ITM_SendCharX (4, 4);
   ITM_SendCharX (5, 5);
   ITM_SendCharX (6, 6);
   ITM_SendCharX (7, 7);
   ITM_SendCharX (8, 8);
   ITM_SendCharX (9, 9);
   ITM_SendCharX (10, 10);
   ITM_SendCharX (11, 11);
   ITM_SendCharX (12, 12);*/

    //    printf("Periodic 1000mls\n");
    
    //ITMLOG 1 >D:\BMS3_2A.csv   
 //   size = sprintf(transferChars, "%d, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f", timeCharge++, bms6.getCellVoltage(0),bms6.getCellVoltage(1),bms6.getCellVoltage(2),bms6.getCellVoltage(3),bms6.getCellVoltage(4),bms6.getCellVoltage(5),bms6.getCellVoltage(6),bms6.getCellVoltage(7),bms6.getCellVoltage(8),bms6.getCellVoltage(9),bms6.getCellVoltage(10),bms6.getCellVoltage(11));
   timeCharge++;
   
   for (bmsNr =0; bmsNr<11; bmsNr++){
     switch(bmsNr){
       case 0:  {bms = &bms0;}break;
       case 1:  {bms = &bms1;}break;
       case 2:  {bms = &bms2;}break;
       case 3:  {bms = &bms3;}break;
       case 4:  {bms = &bms4;}break;
       case 5:  {bms = &bms5;}break;
       case 6:  {bms = &bms6;}break;
       case 7:  {bms = &bms7;}break;
       case 8:  {bms = &bms8;}break;
       case 9:  {bms = &bms9;}break;
       case 10: {bms = &bms10;}break;
     }
     size = sprintf(transferChars, "%d, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f", timeCharge, bms->getCellVoltage(0),bms->getCellVoltage(1),bms->getCellVoltage(2),bms->getCellVoltage(3),bms->getCellVoltage(4),bms->getCellVoltage(5),bms->getCellVoltage(6),bms->getCellVoltage(7),bms->getCellVoltage(8),bms->getCellVoltage(9),bms->getCellVoltage(10),bms->getCellVoltage(11));
     if (size>0){
       for(int i=0; i<size; i++){
         ITM_SendCharX (bmsNr+1, transferChars[i]);
       }
       ITM_SendCharX (bmsNr+1, '\n');
       osDelay(1);
     }
   }
   printf("Task 1000 end\n");
  }
}

