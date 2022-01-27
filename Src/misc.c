#include <stdint.h>

#include "misc.h"


void sleep_us(uint32_t us)
{
   for (uint32_t i = 0; i < us; i++);
}
