#include <stdint.h>

#include "stm32f4/rcc.h"


volatile RCC_regs *const RCC = (RCC_regs *)RCC_BASEADDR;
