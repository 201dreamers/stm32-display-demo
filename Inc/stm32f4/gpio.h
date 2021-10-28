#ifndef _STM32F4_GPIO_H
#define _STM32F4_GPIO_H

#include <stdint.h>

#include "stm32f4/general.h"
#include "stm32f4/rcc.h"


#define GPIOx_PIN_COUNT 16

typedef struct {
    volatile uint32_t mode;
    volatile uint32_t output_type;
    volatile uint32_t output_speed;
    volatile uint32_t pull_up_down;
    volatile uint32_t input_data;
    volatile uint32_t output_data;
    volatile uint32_t bit_set_reset;
    volatile uint32_t config_lock;
    volatile uint16_t alternate_function_low;
    volatile uint16_t alternate_function_high;
} GPIOx_t;

typedef enum {
    GPIOA_BASEADDR = AHB1PERIPH_BASEADDR + GPIOA_ADDR_OFFSET,
    GPIOB_BASEADDR = AHB1PERIPH_BASEADDR + GPIOB_ADDR_OFFSET,
    GPIOC_BASEADDR = AHB1PERIPH_BASEADDR + GPIOC_ADDR_OFFSET,
    GPIOD_BASEADDR = AHB1PERIPH_BASEADDR + GPIOD_ADDR_OFFSET,
    GPIOE_BASEADDR = AHB1PERIPH_BASEADDR + GPIOE_ADDR_OFFSET,
    GPIOF_BASEADDR = AHB1PERIPH_BASEADDR + GPIOF_ADDR_OFFSET,
    GPIOG_BASEADDR = AHB1PERIPH_BASEADDR + GPIOG_ADDR_OFFSET,
    GPIOH_BASEADDR = AHB1PERIPH_BASEADDR + GPIOH_ADDR_OFFSET,
    GPIOI_BASEADDR = AHB1PERIPH_BASEADDR + GPIOI_ADDR_OFFSET
} GPIOx_addr_t;

typedef enum {
    GPIO_INPUT_MODE = 0x0,
    GPIO_OUTPUT_MODE = 0x1,
    GPIO_ALTERNATE_FUNCTION_MODE = 0x2,
    GPIO_ANALOG_MODE = 0x3
} GPIO_mode_t;

typedef enum {
    GPIO_NO_PULL_UP_DOWN = 0x0,
    GPIO_PULL_UP = 0x1,
    GPIO_PULL_DOWN = 0x2,
    // RESERVED = 0x3
} GPIO_pull_up_down_config_t;

extern volatile GPIOx_t *const GPIOA;
extern volatile GPIOx_t *const GPIOB;
extern volatile GPIOx_t *const GPIOC;
extern volatile GPIOx_t *const GPIOD;
extern volatile GPIOx_t *const GPIOE;
extern volatile GPIOx_t *const GPIOF;
extern volatile GPIOx_t *const GPIOG;
extern volatile GPIOx_t *const GPIOH;
extern volatile GPIOx_t *const GPIOI;

void enable_GPIOx_clock(volatile GPIOx_t *const port);
void reset_GPIOx(volatile GPIOx_t *const port);
void set_GPIOx_mode(volatile GPIOx_t *const port, GPIO_mode_t mode, uint16_t pin_mask);
void set_GPIOx_pull_up_down_configuration(volatile GPIOx_t *const port, GPIO_pull_up_down_config_t configuration, uint16_t pin_mask);
void write_into_GPIOx_output_register(volatile GPIOx_t *const port, uint16_t data, uint16_t pin_mask);
uint16_t read_from_GPIOx_input_register(volatile GPIOx_t *const port, uint16_t pin_mask);

#endif /* _STM32F4_GPIO_H */
