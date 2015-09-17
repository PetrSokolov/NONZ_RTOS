#include "..\_Objects_Definitions\__ObjectsDefinitions.hpp"
#include "..\_Objects_Definitions\__ConstantsDefinitions.hpp"

void StartTaskSpi2(void const * argument)
{
  for(;;)
  {
    spi2Handler.handler();
  }


}

