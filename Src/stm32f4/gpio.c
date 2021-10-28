#include <stdint.h>
#include <stdlib.h>

#include "stm32f4/gpio.h"
#include "stm32f4/rcc.h"
#include "bit_operations.h"


volatile GPIOx_t *const GPIOA = (GPIOx_t *)GPIOA_BASEADDR;
volatile GPIOx_t *const GPIOB = (GPIOx_t *)GPIOB_BASEADDR;
volatile GPIOx_t *const GPIOC = (GPIOx_t *)GPIOC_BASEADDR;
volatile GPIOx_t *const GPIOD = (GPIOx_t *)GPIOD_BASEADDR;
volatile GPIOx_t *const GPIOE = (GPIOx_t *)GPIOE_BASEADDR;
volatile GPIOx_t *const GPIOF = (GPIOx_t *)GPIOF_BASEADDR;
volatile GPIOx_t *const GPIOG = (GPIOx_t *)GPIOG_BASEADDR;
volatile GPIOx_t *const GPIOH = (GPIOx_t *)GPIOH_BASEADDR;
volatile GPIOx_t *const GPIOI = (GPIOx_t *)GPIOI_BASEADDR;

static uint8_t get_GPIOx_offset_in_RCC_register(volatile GPIOx_t *const port)
{
    uint8_t offset;
    if (port == GPIOA) {
        offset = 0;
    } else if (port == GPIOB) {
        offset = 1;
    } else if (port == GPIOC) {
        offset = 2;
    } else if (port == GPIOD) {
        offset = 3;
    } else if (port == GPIOE) {
        offset = 4;
    } else if (port == GPIOF) {
        offset = 5;
    } else if (port == GPIOG) {
        offset = 6;
    } else if (port == GPIOH) {
        offset = 7;
    } else if (port == GPIOI) {
        offset = 8;
    } else {
        /* No such GPIO port */
        exit(EXIT_FAILURE);
    }
    return offset;
}

void enable_GPIOx_clock(volatile GPIOx_t *const port)
{
    uint8_t offset = get_GPIOx_offset_in_RCC_register(port);
    SET_BIT(RCC->AHB1ENR, offset);
}

void reset_GPIOx(volatile GPIOx_t *const port)
{
    uint8_t offset = get_GPIOx_offset_in_RCC_register(port);
    CLEAR_BIT(RCC->AHB1RSTR, offset);
}

void set_GPIOx_mode(volatile GPIOx_t *const port, GPIO_mode_t mode, uint16_t pin_mask)
{
    for (uint8_t pin = 0; pin < GPIOx_PIN_COUNT; pin++) {
        if (IS_BIT_SET(pin_mask, pin)) {
            port->mode &= ~(0x3 << (pin * 2));
            port->mode |= mode << (pin * 2);
        }
    }
}

void set_GPIOx_pull_up_down_configuration(volatile GPIOx_t *const port, GPIO_pull_up_down_config_t configuration, uint16_t pin_mask)
{
    for (uint8_t pin = 0; pin < GPIOx_PIN_COUNT; pin++) {
        if (IS_BIT_SET(pin_mask, pin)) {
            port->pull_up_down &= ~(0x3 << (pin * 2));
            port->pull_up_down |= configuration << (pin * 2);
        }
    }
}

void write_into_GPIOx_output_register(volatile GPIOx_t *const port, uint16_t data, uint16_t pin_mask)
{
    for (uint8_t pin = 0; pin < GPIOx_PIN_COUNT; pin++) {
        if (IS_BIT_SET(pin_mask, pin)) {
            if (IS_BIT_SET(data, pin))
                SET_BIT(port->output_data, pin);
            else
                CLEAR_BIT(port->output_data, pin);
        }
    }
}

uint16_t read_from_GPIOx_input_register(volatile GPIOx_t *const port, uint16_t pin_mask)
{
    uint16_t data = 0x0;
    for (uint8_t pin = 0; pin < GPIOx_PIN_COUNT; pin++) {
        if (IS_BIT_SET(pin_mask, pin)) {
            if (IS_BIT_SET(port->input_data, pin))
                data += 0x1;
            data <<= 1;
        }
    }

    return data;
}