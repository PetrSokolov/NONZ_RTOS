//#####################################################################################
//	Файл				__IControlCommands.h
//	Название:		Интерфейс обработки команд (ПМУ, ПДУ, дискр.входы, аналог.входы модбас)
//	Автор:			Бабкин.П.А
//	Вер. | гггг-ммм-дд | Внес изменения | Описание изменений
//	=====|=============|================|==============================================
// 	 1.0 | 2015 май 21 | Бабкин П.А	 		| Первый релиз
//#####################################################################################


//=====================================================================================
//  Интерфейс обработки команд (ПМУ, ПДУ, дискр.входы, аналог.входы модбас)
//=====================================================================================
#ifndef ICONTROLCOMMANDS_H
#define ICONTROLCOMMANDS_H

#include "stdint.h"
#include <stdio.h>
#include <vector>
#include <map>

#ifdef __cplusplus
 extern "C" {
#endif 

using namespace std;
   
namespace src{	 

  
//-------------------------------------------------------------------------------------  
  //  Интерфейс передачи команд с ПМУ
//-------------------------------------------------------------------------------------  
class IPmuCommands{
  public:
    virtual void pmuOpen(void);
    virtual void pmuClose(void);
    virtual void pmuNoDuMu(void);
    virtual void pmuDu(void);
    virtual void pmuMu(void);
};


//-------------------------------------------------------------------------------------  
//  Интерфейс передачи команд с ПДУ (Remote Control)
//-------------------------------------------------------------------------------------  
class IRcCommands{
  public:
    virtual inline void rcClear(void) {}
    virtual inline void rcOpen(void)  {}
    virtual inline void rcClose(void) {}
    virtual inline void rcPlus(void)  {}
    virtual inline void rcDown(void)  {}
    virtual inline void rcRight(void) {}
    virtual inline void rcMinus(void) {}
    virtual inline void rcEnter(void) {}
};


//-------------------------------------------------------------------------------------  
//  Интерфейс передачи команд с Дискретных входов
//-------------------------------------------------------------------------------------  
class IDinCommands{
  public:
    virtual void dinOpen();
    virtual void dinClose();
    virtual void dinStop();
    virtual void dinBlock();
    virtual void dinRemote();
};


//-------------------------------------------------------------------------------------  
//  Интерфейс передачи команд с Аналоговых входов
//-------------------------------------------------------------------------------------  
class IAinCommands{
  public:
    virtual void dinOpen();
    virtual void dinClose();
    virtual void dinStop();
    virtual void dinBlock();
};


//-------------------------------------------------------------------------------------  
//  Интерфейс передачи команд с Модбас
//-------------------------------------------------------------------------------------  
class IMbCommands{
  public:
    virtual void mbOpen();
    virtual void mbClose();
    virtual void mbStop();
    virtual void mbRemote();
    virtual void mbLocal();
};


//-------------------------------------------------------------------------------------  
//  Интерфейс передачи команд суммарный
//-------------------------------------------------------------------------------------  
class IControlCommands : //public IPmuCommands,
                         public IRcCommands
                         /*public IDinCommands,
                         public IAinCommands,
                         public IMbCommands*/ {
  public:
    virtual inline void rcPlus(void)  {}
    virtual inline void rcMinus(void) {}
    virtual inline void rcEnter(void) {}
    virtual inline void rcClear(void) {}
    virtual inline void rcOpen(void)  {}
    virtual inline void rcClose(void) {}
};

} //src
#ifdef __cplusplus
}
#endif
#endif	//define ICONTROLCOMMANDS_H

