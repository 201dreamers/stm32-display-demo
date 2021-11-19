#include <stdint.h>
#include <stdlib.h>

#include "stm32f4/gpio.h"
#include "stm32f4/rcc.h"
#include "bit_operations.h"


GPIOx GPIOA = (GPIOx)GPIOA_BASEADDR;
GPIOx GPIOB = (GPIOx)GPIOB_BASEADDR;
GPIOx GPIOC = (GPIOx)GPIOC_BASEADDR;
GPIOx GPIOD = (GPIOx)GPIOD_BASEADDR;
GPIOx GPIOE = (GPIOx)GPIOE_BASEADDR;
GPIOx GPIOF = (GPIOx)GPIOF_BASEADDR;
GPIOx GPIOG = (GPIOx)GPIOG_BASEADDR;
GPIOx GPIOH = (GPIOx)GPIOH_BASEADDR;
GPIOx GPIOI = (GPIOx)GPIOI_BASEADDR;

static uint8_t get_GPIOx_offset_in_RCC_AHB1_register(GPIOx port)
{
    uint8_t offset;
    if (port == GPIOA)
        offset = 0;
    else if (port == GPIOB)
        offset = 1;
    else if (port == GPIOC)
        offset = 2;
    else if (port == GPIOD)
        offset = 3;
    else if (port == GPIOE)
        offset = 4;
    else if (port == GPIOF)
        offset = 5;
    else if (port == GPIOG)
        offset = 6;
    else if (port == GPIOH)
        offset = 7;
    else if (port == GPIOI)
        offset = 8;
    else
        exit(EXIT_FAILURE);  /* No such GPIO port */

    return offset;
}

void enable_GPIOx_clock(GPIOx port)
{
    uint8_t offset = get_GPIOx_offset_in_RCC_AHB1_register(port);
    SET_BIT(RCC->AHB1ENR, offset);
}

void reset_GPIOx(GPIOx port)
{
    uint8_t offset = get_GPIOx_offset_in_RCC_AHB1_register(port);
    CLEAR_BIT(RCC->AHB1RSTR, offset);
}

void set_GPIOx_pin_mode(GPIO_mode mode, uint8_t pin, GPIOx port)
{
    port->mode &= ~(0x3 << (pin * 2));
    port->mode |= mode << (pin * 2);
}

void set_GPIOx_port_mode(GPIO_mode mode, GPIOx port)
{
    // 0x55555555 means combination `01` 16 times that fits 32bits register
    // if we multiply this num by `mode`, we will get the same mode value for every pin
    port->mode &= ~((uint32_t)0x55555555 * 0x3);
    port->mode |= 0x55555555 * mode;
}

void set_GPIOx_pin_pull_up_down_mode(GPIO_pull_up_down_mode mode, uint8_t pin, GPIOx port)
{
    port->pull_up_down &= ~(0x3 << (pin * 2));
    port->pull_up_down |= mode << (pin * 2);
}

void set_GPIOx_port_pull_up_down_mode(GPIO_pull_up_down_mode mode, uint16_t pin, GPIOx port)
{
    // 0x55555555 means combination `01` 16 times that fits 32bits register
    // if we multiply this num by `mode`, we will get the same mode value for every pin
    port->pull_up_down &= ~((uint32_t)0x55555555 * 0x3);
    port->pull_up_down |= 0x55555555 * mode;
}

void write_into_GPIOx_pin(uint8_t data, uint8_t pin, GPIOx port)
{
    if (GET_BIT(data, 0))
        SET_BIT(port->output_data, pin);
    else
        CLEAR_BIT(port->output_data, pin);
}

void write_into_GPIOx_port(uint16_t data, GPIOx port)
{
    port->output_data &= ~(0xf);
    port->output_data |= data;
}

uint8_t read_from_GPIOx_pin(uint8_t pin, GPIOx port)
{
    return (uint8_t)GET_BIT(port->input_data, pin);
}

uint16_t read_from_GPIOx_port(GPIOx port)
{
    return port->input_data;
}
