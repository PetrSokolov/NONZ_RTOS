//#####################################################################################
//	Файл				Editor.cpp
//	Название:		Редактор. Работает с объектами типа IVariable
//	Автор:			Бабкин.П.А
//	Вер. | гггг-ммм-дд | Внес изменения | Описание изменений
//	=====|=============|================|==============================================
// 	 1.0 | 2015 июнь 10 | Бабкин П.А	 		| Первый релиз
// 	-----|-------------|----------------|----------------------------------------------
//#####################################################################################


#include "stm32f4xx_hal.h"
#include "__Editor.hpp"
//#include "__Parameters.hpp"
#include "..\_Objects_Definitions\__ObjectsDefinitions.hpp"

using namespace src;

//---------------------------------------------------------------------------------------
  //  Методы интерфейса IControlCommands
//---------------------------------------------------------------------------------------
void Editor::rcPlus(void)
 {
   if(_variable->getRw() && !getModificationMode()) { setModificationMode(TRUE);}
   if(getModificationMode()) { incValueHandler(1,_power); }
 }
//---------------------------------------------------------------------------------------
void Editor::rcMinus(void)
 {
   if(_variable->getRw() && !getModificationMode()) { setModificationMode(TRUE);}
   if(getModificationMode()) { decValueHandler(1,_power); }
 }


//---------------------------------------------------------------------------------------
void Editor::rcEnter(void)
 {
   if( endEditing()==TRUE){
     rcDown();
//     setModificationMode(FALSE);
   }
   setModificationMode(FALSE);
 }


//---------------------------------------------------------------------------------------
void Editor::rcClear(void)
 {
   if(++_power>3) { _power =0; }
   if(editingValue<0 && _power==3) {_power =0;} // Дублируется в inc/dec методах
 }


//---------------------------------------------------------------------------------------
void Editor::rcOpen(void)
  {  }


//---------------------------------------------------------------------------------------
void Editor::rcClose(void)
 {  }


//---------------------------------------------------------------------------------------
void Editor::rcDown(void)
  { setViewerMode(FALSE); setModificationMode(FALSE); _variable->endOfCervice(); }


//---------------------------------------------------------------------------------------
void Editor::rcRight(void)
 {  }


//---------------------------------------------------------------------------------------
//  Методы интерфейса IDisplayed
//---------------------------------------------------------------------------------------
uint16_t tmpVar =0;
void Editor::getString(string& str, uint16_t row)
{
  char bufer[8];
  uint16_t lenth_;
  string str3 = "123";
  
  if (getModificationMode()){ //  Режим изменения значения
    snprintf(bufer, sizeof(bufer), "%d", getEditingValue());
  }
   else{                      //  Режим просмотра значения
    snprintf(bufer, sizeof(bufer), "%d", _variable->getValue());
   }

  // Выравнивание по правому краю. Для 4-зн 7-сегментного индикатора
  str = bufer;
  for(lenth_ = strlen(bufer); lenth_<4; lenth_++){
    str.insert(str.begin(), '0');
  }

  // Перенос знака "-" в левый край
  size_t found_ = str.find('-');
  if (found_ != string::npos){
//    printf("- found at %d\n", found_);
    str.erase(found_, 1);
    str.insert(0, "-");
  }
  
  // Мигание при редактировании
  lenth_ = str.size();
  if (getModificationMode() && ((lenth_-1-_power)>=0)){
    if (tmpVar){
      str.replace(lenth_-1-_power, 1, " ");
      tmpVar =0;
    } else { tmpVar =1; }
    
  }
  
}

//---------------------------------------------------------------------------------------
void Editor::getRow(uint16_t& row)
{
//  row = _im;
}


//---------------------------------------------------------------------------------------
//  Методы класса Editor
//---------------------------------------------------------------------------------------

// Инкремент параметра  -------------------------------------------------------------------------------------
void Editor::incValueHandler(uint16_t x, int power)
{
  int value_;
  value_ = x*pow(10.0, power);
  if(editingValue + value_ <= _variable->getMax() ) {editingValue += value_;} else { editingValue = _variable->getMin(); }
  if(editingValue<0 && _power==3) {_power =0;}
}


// Декремент параметра  -------------------------------------------------------------------------------------
void Editor::decValueHandler(uint16_t x, int power)
{
  int value_;
  value_ = x*pow(10.0, power);
  if(editingValue - value_ >= _variable->getMin()) {editingValue -= value_;} else { editingValue = _variable->getMax(); } 
  if(editingValue<0 && _power==3) {_power =0;}
}




