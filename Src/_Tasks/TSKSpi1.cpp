#include "..\_Objects_Definitions\__ObjectsDefinitions.hpp"
#include "..\_Objects_Definitions\__ConstantsDefinitions.hpp"

void StartTaskSpi1(void const * argument)
{
  for(;;)
  {
    printf("Task SPI_1 begin\n");

    spi1Handler.handler();

    printf("Task SPI_1 end\n");

  }


}

