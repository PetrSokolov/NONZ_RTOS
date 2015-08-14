//#####################################################################################
//	Файл				DisplayLedDecoder.cpp
//	Название:		Декодер на LED индикатор
//	Автор:			Бабкин.П.А
//	Вер. | гггг-ммм-дд | Внес изменения | Описание изменений
//	=====|=============|================|==============================================
// 	 1.0 | 2015 июнь 5 | Бабкин П.А	 		| Первый релиз
// 	-----|-------------|----------------|----------------------------------------------
//#####################################################################################


//#include "stm32f10x.h"
#include "__DisplayLedDecoder.hpp"
#include "..\_Objects_Definitions\__ConstantsDefinitions.hpp"
//#include "_Parameters.h"

using namespace src;


//-------------------------------------------------------------------------------------
//  Декодитрование символа на битовые составляющие индикатора
//-------------------------------------------------------------------------------------
void LedDecoder::decode (char symbol)
{
  if (symbol<32){ // Единичные индикаторы
    _segments.all =0;
    if (symbol & BIT_0) {_segments.all |= BIT_0;}
    if (symbol & BIT_1) {_segments.all |= BIT_6;}
  }
   else{
    switch(symbol){
    case 'a':
    case 'A': {_segments.all = 0x77; } break;
    case 'b':
    case 'B': {_segments.all = 0x7C; } break;
    case 'c':
    case 'C': {_segments.all = 0x39; } break;
    case 'd':
    case 'D': {_segments.all = 0x5E; } break;
    case 'e':
    case 'E': {_segments.all = 0x79; } break;
    case 'f':
    case 'F': {_segments.all = 0x71; } break;
    case 'g':
    case 'G': {_segments.all = 0x6F; } break;
    case 'h':
    case 'H': {_segments.all = 0x74; } break;
    case 'i':
    case 'I': {_segments.all = 0x06; } break;
    case 'j':
    case 'J': {_segments.all = 0x0E; } break;
    case 'l':
    case 'L': {_segments.all = 0x38; } break;
    case 'o':
    case 'O': {_segments.all = 0x3F; } break;
    case 'p':
    case 'P': {_segments.all = 0x73; } break;
    case 's':
    case 'S': {_segments.all = 0x6D; } break;
    case 'u':
    case 'U': {_segments.all = 0x3E; } break;
    case 'y':
    case 'Y': {_segments.all = 0x6E; } break;
    case '0': {_segments.all = 0x3F; } break;
    case '1': {_segments.all = 0x06; } break;
    case '2': {_segments.all = 0x5B; } break;
    case '3': {_segments.all = 0x4F; } break;
    case '4': {_segments.all = 0x66; } break;
    case '5': {_segments.all = 0x6D; } break;
    case '6': {_segments.all = 0x7D; } break;
    case '7': {_segments.all = 0x07; } break;
    case '8': {_segments.all = 0x7F; } break;
    case '9': {_segments.all = 0x6F; } break;
    case '.': {_segments.seg.dp = 1; } break;
    case '-': {_segments.all = 0x40; } break;
    default:  {_segments.all = 0x00; } break;
    }
   
   }

}



