//#####################################################################################
//	Файл				DisplayLedDecoder.hpp
//	Название:		Декодер на LED индикатор
//	Автор:			Бабкин.П.А
//	Вер. | гггг-ммм-дд | Внес изменения | Описание изменений
//	=====|=============|================|==============================================
// 	 1.0 | 2015 июнь 5 | Бабкин П.А	 		| Первый релиз
// 	-----|-------------|----------------|----------------------------------------------
//#####################################################################################

#ifndef DISPLAYLEDDECODER_HPP
#define DISPLAYLEDDECODER_HPP

#include "stdint.h"
#include <stdio.h>


#ifdef __cplusplus
 extern "C" {
#endif 

using namespace std;
   
namespace src{	 

class LedDecoder{
  public:
    void decode (char symbol);
    inline uint8_t getA   (void)  { return  _segments.seg.a; }
    inline uint8_t getB   (void)  { return  _segments.seg.b; }
    inline uint8_t getC   (void)  { return  _segments.seg.c; }
    inline uint8_t getD   (void)  { return  _segments.seg.d; }
    inline uint8_t getE   (void)  { return  _segments.seg.e; }
    inline uint8_t getF   (void)  { return  _segments.seg.f; }
    inline uint8_t getG   (void)  { return  _segments.seg.g; }
    inline uint8_t getDP  (void)  { return  _segments.seg.dp; }
    inline uint8_t getAll (void)  { return  _segments.all; }

  private:
    struct _segments_t{
      uint8_t a  :1;
      uint8_t b  :1;
      uint8_t c  :1;
      uint8_t d  :1;
      uint8_t e  :1;
      uint8_t f  :1;
      uint8_t g  :1;
      uint8_t dp :1;
    };
  
  union {
   uint8_t      all;  //  Все сегменты
   _segments_t  seg;  //  Отдельный сегмент
  }_segments;

};

}	// namespace src


#ifdef __cplusplus
}
#endif
#endif




