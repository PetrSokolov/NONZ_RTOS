//#####################################################################################
//	Файл				__MainMenu.cpp
//	Название:		Менеджер меню.
//	Автор:			Бабкин.П.А
//	Вер. | гггг-ммм-дд | Внес изменения | Описание изменений
//	=====|=============|================|==============================================
// 	 1.0 | 2015 июнь 4 | Бабкин П.А	 		| Первый релиз
// 	-----|-------------|----------------|----------------------------------------------
//#####################################################################################

#include "__MainMenu.hpp"

using namespace src;


//-------------------------------------------------------------------------------------
//  Методы интерфейса IObservable
//-------------------------------------------------------------------------------------

//  Добавление наблюдателя в список рассылки
void  MainMenu::addObserver (IControlCommands* observer)
{
  _observers.push_back(observer);
}

//  Удаление наблюдателя из списка рассылки
void  MainMenu::removeObserver (IControlCommands* observer)
{
  _observers.remove(observer);
}

void MainMenu::getString (string& str, uint16_t row)
{
  switch (row){
    case 0: // Отображение ДУ/МУ
      {
        if (getRemote()){
         str = "Remote";
        }
         else {str = "";}
      }break;
    case 1: // Отображение "Программирование"
      {
        if (getProgramming()){
          str = "Progr.";
        }
         else {str = "";}
      }break;
  }
}

//  Оповещение наблюдателей
/*void  MainMenu::notyfyObservers (void)
{
  for (list<IObserver*>::iterator it=_observers.begin(); it!=_observers.end(); it++){
    (*it)->handleEvent(_message);
  }
}*/

