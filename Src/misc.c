#include <stdint.h>

#include "misc.h"


void software_delay(uint32_t ms)
{
    uint16_t iterations_in_1_ms = 2000;
    for (uint32_t i = 0; i < (iterations_in_1_ms * ms); i++);
}
