

#include "..\_Objects_Definitions\__ObjectsDefinitions.hpp"
#include "..\_Objects_Definitions\__ConstantsDefinitions.hpp"

#include <stdio.h>


/*__STATIC_INLINE uint32_t ITM_SendCharX (uint32_t Channel, uint32_t Data)
{
  if ((ITM->TCR & ITM_TCR_ITMENA_Msk) &&      // ITM enabled
      (ITM->TER & (1UL << Channel) ) )        // ITM Port X enabled
  {
    while (ITM->PORT[Channel].u32 == 0);
    ITM->PORT[Channel].u8 = (uint8_t) Data;
  }
  return (Data);
}*/


char transferChars[200];  // Возможно превышает кучу, сваливалось в хардфаулт
uint16_t timeCharge=0, size;

//int16_t numbers=0, pages=0, p1=0, p2=0, p3=0;
//char names[20];
float pwmval =0;

void StartTask1000mls(void const * argument)
{

//  char transferChars[128];  // Возможно превышает кучу, сваливалось в хардфаулт
//  uint16_t timeCharge=0, size;
//  float valueCharge = 1.234;
// uint8_t bmsNr;
// Bms* bms;
  
  
  for(;;)
  {
    
    osSemaphoreWait(binarySemTSK1000mls, osWaitForever);
    
    //pwmval += 0.01;
    pwmval =0.5;
    if (pwmval>0.8) {pwmval = 0.1;}
    pwm.setValue(pwmval);
    printf("1000mls\n");
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
/*   timeCharge++;
      
    
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
   }*/
  }
}

