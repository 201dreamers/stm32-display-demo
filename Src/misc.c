#include <stdint.h>

#include "misc.h"


void software_delay_us(uint32_t us)
{
    uint32_t iterations_in_1_us = 2;
    for (uint32_t i = 0; i < (iterations_in_1_us * us); i++);
}
