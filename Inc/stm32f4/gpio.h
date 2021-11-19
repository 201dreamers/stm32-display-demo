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
    volatile uint32_t alternate_function_low;
    volatile uint32_t alternate_function_high;
} GPIOx_regs;

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
} GPIOx_addr;

typedef enum {
    GPIO_INPUT_MODE = 0x0,
    GPIO_OUTPUT_MODE = 0x1,
    GPIO_ALTERNATE_FUNCTION_MODE = 0x2,
    GPIO_ANALOG_MODE = 0x3
} GPIO_mode;

typedef enum {
    GPIO_NO_PULL_UP_DOWN = 0x0,
    GPIO_PULL_UP = 0x1,
    GPIO_PULL_DOWN = 0x2,
    // RESERVED = 0x3
} GPIO_pull_up_down_mode;

typedef volatile GPIOx_regs *const GPIOx;

extern GPIOx GPIOA;
extern GPIOx GPIOB;
extern GPIOx GPIOC;
extern GPIOx GPIOD;
extern GPIOx GPIOE;
extern GPIOx GPIOF;
extern GPIOx GPIOG;
extern GPIOx GPIOH;
extern GPIOx GPIOI;


void enable_GPIOx_clock(GPIOx port);
void reset_GPIOx(GPIOx port);
void set_GPIOx_pin_mode(GPIO_mode mode, uint8_t pin, GPIOx port);
void set_GPIOx_port_mode(GPIO_mode mode, GPIOx port);
void set_GPIOx_pin_pull_up_down_mode(GPIO_pull_up_down_mode mode, uint8_t pin, GPIOx port);
void set_GPIOx_port_pull_up_down_mode(GPIO_pull_up_down_mode mode, uint16_t pin, GPIOx port);
void write_into_GPIOx_pin(uint8_t data, uint8_t pin, GPIOx port);
void write_into_GPIOx_port(uint16_t data, GPIOx port);
uint8_t read_from_GPIOx_pin(uint8_t pin, GPIOx port);
uint16_t read_from_GPIOx_port(GPIOx port);

#endif /* _STM32F4_GPIO_H */
