#include "..\_Objects_Definitions\__ObjectsDefinitions.hpp"
#include "..\_Objects_Definitions\__ConstantsDefinitions.hpp"

void StartTaskSpi2(void const * argument)
{
  for(;;)
  {
   printf("Task SPI_2 begin\n");
   
    spi2Handler.handler();

   printf("Task SPI_2 end\n");
  }


}

