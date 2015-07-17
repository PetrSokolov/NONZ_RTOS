//#####################################################################################
//	Файл				ObjectsDefinitions.cpp
//	Название:		Определение глобальных объектов
//	Автор:			Бабкин.П.А
//	Вер. | гггг-ммм-дд | Внес изменения | Описание изменений
//	=====|=============|================|==============================================
// 	 1.0 | 2015 май 28 | Бабкин П.А	 		| Первый релиз
// 	-----|-------------|----------------|----------------------------------------------
//#####################################################################################

#include "__ObjectsDefinitions.hpp"

using namespace src;

//---------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------
extern SPI_HandleTypeDef hspi1, hspi2;
extern DMA_HandleTypeDef hdma_spi1_tx;
extern DMA_HandleTypeDef hdma_spi1_rx;
extern DMA_HandleTypeDef hdma_spi2_tx;
extern DMA_HandleTypeDef hdma_spi2_rx;


//---------------------------------------------------------------------------------------------------------
//  Создание глобальных объектов
//---------------------------------------------------------------------------------------------------------
  //  АЦП1, АЦП3
  Adc adc1(5), adc3(3);
  
  // ШИМ
  Pwm2phaseNONZ pwm;

  //  Датчик напряжения на шине постоянного тока. Напряжение  и ток на заряжаемой батарее
  AnalogRmsSensor uDC(60e-6, 0.01), uCharge(60e-6, 0.01), iCharge(20e-6, 0.001);
  
  //  Контейнер настроечных объектов - параметров
  ContainerOfVariables containerOfVariables;

  //  Менеджер меню
  MainMenu mainMenu;
  
  //  Движок меню.
  MenuEngine menuEngine(&containerOfVariables);

  //  Обработчик отображения на индикаторе. Агрегирует по интерфейсу IDisplayed объект, который надо отображать
  DisplayLed4Digit displayLed4Digit (&menuEngine, &mainMenu);

  // ПДУ
  Rc rc( &mainMenu );

  //  Обработчик SPI
  SpiHandler  spi1Handler(&hspi1, &hdma_spi1_tx, &hdma_spi1_rx, 84000);
  Spi2Handler spi2Handler(&hspi2, &hdma_spi2_tx, &hdma_spi2_rx, 42000);
  
  // MRAM память
//  Mram(uint8_t chipSelect, SpiMode spiMode, uint16_t spiFrequency, ISpiDmaExchange* spiHandler)
  Mram mram(CS_MRAM, SPI_MODE_0, 1000, &spi2Handler);
  
  // Модули БМС (0-10)
  //Bms(chipSelect, chipAdres, spiModeLTC, spiModePort, spiFrequency, spiPortFrequency, *spiHandler, *spiPortHandler)

  Bms bms0(CS_LTC_0, 0, SPI_MODE_0, SPI_MODE_0, 700, 700, &spi1Handler, &spi2Handler),
      bms1(CS_LTC_1, 1, SPI_MODE_0, SPI_MODE_0, 700, 700, &spi1Handler, &spi2Handler),
      bms2(CS_LTC_2, 2, SPI_MODE_0, SPI_MODE_0, 700, 700, &spi1Handler, &spi2Handler),
      bms3(CS_LTC_3, 3, SPI_MODE_0, SPI_MODE_0, 700, 700, &spi1Handler, &spi2Handler),
      bms4(CS_LTC_4, 4, SPI_MODE_0, SPI_MODE_0, 700, 700, &spi1Handler, &spi2Handler),
      bms5(CS_LTC_5, 5, SPI_MODE_0, SPI_MODE_0, 700, 700, &spi1Handler, &spi2Handler),
      bms6(CS_LTC_6, 6, SPI_MODE_0, SPI_MODE_0, 700, 700, &spi1Handler, &spi2Handler),
      bms7(CS_LTC_7, 7, SPI_MODE_0, SPI_MODE_0, 700, 700, &spi1Handler, &spi2Handler),
      bms8(CS_LTC_8, 8, SPI_MODE_0, SPI_MODE_0, 700, 700, &spi1Handler, &spi2Handler),
      bms9(CS_LTC_9, 9, SPI_MODE_0, SPI_MODE_0, 700, 700, &spi1Handler, &spi2Handler),
      bms10(CS_LTC_10, 10, SPI_MODE_0, SPI_MODE_0, 700, 700, &spi1Handler, &spi2Handler);

  BmsAssembly bmsAssembly;  // Объект-контейнер, объединяющий все модули BMS
  
  // SPI порты на микроконтроллере PIC
//  SpiPortPic spiPortPic6(6, 0, 700, &spi2Handler, 0xFF, 0xFF, 0x00, 0x00),
//             spiPortPic8(8, 0, 700, &spi2Handler, 0xFF, 0xFF, 0x00, 0x00);

//---------------------------------------------------------------------------------------------------------
//  Создание глобальных объектов-параметров
//---------------------------------------------------------------------------------------------------------
GroupParameter grA  (1,                         // _id        - Идентификатор параметра. Для привязки к FRAM
                      (char *)"A",              // _menu      - Идентификатор меню.
                      (char *)"Группа A меню"   //_text      - Тестовая информация
                    ),

               grB  (2,                         // _id        - Идентификатор параметра. Для привязки к FRAM
                      (char *)"B",              // _menu      - Идентификатор меню.
                      (char *)"Группа B меню"   //_text      - Тестовая информация
                    );


GroupParameter grA0     ( 100,                  // _id        - Идентификатор параметра. Для привязки к FRAM
                      (char *)"A.0",              // _menu      - Идентификатор меню.
                      (char *)"Элемент меню 1"   //_text      - Тестовая информация
                      );                 // _def       - Значение по умолчанию
 
Parameter  par2      ( 101,                  // _id        - Идентификатор параметра. Для привязки к FRAM
                      (char *)"A.0.0",              // _menu      - Идентификатор меню.
                      (char *)"Элемент меню 2",   //_text      - Тестовая информация
                      255,                // _modbusAdr	- Адрес модбас
                      21,                 // _value     - Значение параметра
                      1,                  // _rw        - Разрешение на запись
                      -100,               // _min       - Минимальное значение
                      9999,                // _max       - Максимальное значение
                      1,                  // _user      - Доступ в режиме пользователя
                      21),                 // _def       - Значение по умолчанию
          par3      ( 102,                  // _id        - Идентификатор параметра. Для привязки к FRAM
                      (char *)"A.0.1",              // _menu      - Идентификатор меню.
                      (char *)"Элемент меню 3",   //_text      - Тестовая информация
                      256,                // _modbusAdr	- Адрес модбас
                      321,                // _value     - Значение параметра
                      1,                  // _rw        - Разрешение на запись
                      0,                  // _min       - Минимальное значение
                      1000,                // _max       - Максимальное значение
                      1,                  // _user      - Доступ в режиме пользователя
                      21),                 // _def       - Значение по умолчанию
          par4      ( 103,                  // _id        - Идентификатор параметра. Для привязки к FRAM
                      (char *)"A.1",              // _menu      - Идентификатор меню.
                      (char *)"Элемент меню 4",   //_text      - Тестовая информация
                      257,                // _modbusAdr	- Адрес модбас
                      321,                // _value     - Значение параметра
                      1,                  // _rw        - Разрешение на запись
                      0,                  // _min       - Минимальное значение
                      100,                // _max       - Максимальное значение
                      1,                  // _user      - Доступ в режиме пользователя
                      21),                 // _def       - Значение по умолчанию
          par5      ( 104,                  // _id        - Идентификатор параметра. Для привязки к FRAM
                      (char *)"A.2",              // _menu      - Идентификатор меню.
                      (char *)"Элемент меню 5",   //_text      - Тестовая информация
                      257,                // _modbusAdr	- Адрес модбас
                      321,                // _value     - Значение параметра
                      1,                  // _rw        - Разрешение на запись
                      0,                  // _min       - Минимальное значение
                      100,                // _max       - Максимальное значение
                      1,                  // _user      - Доступ в режиме пользователя
                      21);                 // _def       - Значение по умолчанию
                      
  // Настроечные параметры ШИМ
ParameterFlt  pwmDeathTime( 200,                    // _id        - Идентификатор параметра. Для привязки к FRAM
                      (char *)"B.0",              // _menu      - Идентификатор меню.
                      (char *)"Элемент меню 3",   //_text      - Тестовая информация
                      255,                // _modbusAdr	- Адрес модбас
                      321,                // _value     - Значение параметра
                      1,                  // _rw        - Разрешение на запись
                      0,                  // _min       - Минимальное значение
                      100,                // _max       - Максимальное значение
                      1,                  // _user      - Доступ в режиме пользователя
                      21,                 // _def       - Значение по умолчанию
                      -7),                // _power     - Степень
              pwmValue( 201,                        // _id        - Идентификатор параметра. Для привязки к FRAM
                      (char *)"B.1",              // _menu      - Идентификатор меню.
                      (char *)"Элемент меню 4",   //_text      - Тестовая информация
                      255,                // _modbusAdr	- Адрес модбас
                      321,                // _value     - Значение параметра
                      1,                  // _rw        - Разрешение на запись
                      0,                  // _min       - Минимальное значение
                      100,                // _max       - Максимальное значение
                      1,                  // _user      - Доступ в режиме пользователя
                      100,                // _def       - Значение по умолчанию
                      -2);                // _power     - Степень

