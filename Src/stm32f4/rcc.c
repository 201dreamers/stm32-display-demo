#include <stdint.h>

#include "stm32f4/rcc.h"


volatile RCC_registers_t *const RCC = (RCC_registers_t *)RCC_BASEADDR;
