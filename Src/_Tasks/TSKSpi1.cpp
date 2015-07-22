#include "..\_Objects_Definitions\__ObjectsDefinitions.hpp"
#include "..\_Objects_Definitions\__ConstantsDefinitions.hpp"

void StartTaskSpi1(void const * argument)
{
  for(;;)
  {

    spi1Handler.handler();


  }


}

