#ifndef CONSTANTSDEFINITIONS_HPP
#define CONSTANTSDEFINITIONS_HPP

//using namespace std;

  
#define BIT_0   0x0001
#define BIT_1   0x0002
#define BIT_2   0x0004
#define BIT_3   0x0008
#define BIT_4   0x0010
#define BIT_5   0x0020
#define BIT_6   0x0040
#define BIT_7   0x0080
#define BIT_8   0x0100
#define BIT_9   0x0200
#define BIT_10  0x0400
#define BIT_11  0x0800
#define BIT_12  0x1000
#define BIT_13  0x2000
#define BIT_14  0x4000
#define BIT_15  0x8000


namespace src{

/// Определение логических значений
enum Bool    {FALSE,
              TRUE };

/// Набор возможных режимов работы SPI
enum SpiMode { SPI_MODE_0,  ///<  режим 0 (CPOL = 0, CPHA = 0)
               SPI_MODE_1,  ///<  режим 1 (CPOL = 0, CPHA = 1)
               SPI_MODE_2,  ///<  режим 2 (CPOL = 1, CPHA = 0)
               SPI_MODE_3 };///<  режим 3 (CPOL = 1, CPHA = 1)


///  Выбор устройств на шине SPI 1
enum Spi1CsDevice { CS_LTC_0,  ///<  Выбор устройства LTC 0
                    CS_LTC_1,  ///<  Выбор устройства LTC 1
                    CS_LTC_2,  ///<  Выбор устройства LTC 2
                    CS_LTC_3,  ///<  Выбор устройства LTC 3
                    CS_LTC_4,  ///<  Выбор устройства LTC 4
                    CS_LTC_5,  ///<  Выбор устройства LTC 5
                    CS_LTC_6,  ///<  Выбор устройства LTC 6
                    CS_LTC_7,  ///<  Выбор устройства LTC 7
                    CS_LTC_8,  ///<  Выбор устройства LTC 8
                    CS_LTC_9,   ///<  Выбор устройства LTC 9
                    CS_LTC_10};///<  Выбор устройства LTC 10

///  Выбор устройств на шине SPI 2
enum Spi2CsDevice { CS_PORT_0,  ///<  Выбор устройства - расширитель порта 0
                    CS_PORT_1,  ///<  Выбор устройства - расширитель порта 1
                    CS_PORT_2,  ///<  Выбор устройства - расширитель порта 2
                    CS_PORT_3,  ///<  Выбор устройства - расширитель порта 3
                    CS_PORT_4,  ///<  Выбор устройства - расширитель порта 4
                    CS_PORT_5,  ///<  Выбор устройства - расширитель порта 5
                    CS_PORT_6,  ///<  Выбор устройства - расширитель порта 6
                    CS_PORT_7,  ///<  Выбор устройства - расширитель порта 7
                    CS_PORT_8,  ///<  Выбор устройства - расширитель порта 8
                    CS_PORT_9,   ///<  Выбор устройства - расширитель порта 9
                    CS_PORT_10,  ///<  Выбор устройства - расширитель порта 10
                    CS_MRAM,     ///<  Выбор устройства - энергонезависимая память MRAM
                    CS_FLASH};  ///<  Выбор устройства - энергонезависимая память FLASH

}

#define countof(a)   (sizeof(a) / sizeof(*(a)))	

#endif	//define CONSTANTSDEFINITIONS_HPP
