#include <stdint.h>
#include <stdio.h>

#include "stm32f4/gpio.h"
#include "wh1602b.h"
#include "bit_operations.h"
#include "misc.h"


#if !defined(__SOFT_FP__) && defined(__ARM_FP)
    #warning "FPU is not initialized, but the project is compiling for an FPU. Please initialize the FPU before use."
#endif

static void print_hello_screen()
{
    send_string_to_display("Test");
    return_home();
}

int main(void)
{
    struct disp_bus display_bus = {
        .db4 = {.gpio_pin = 12, .gpio_port = GPIOE},
        .db5 = {.gpio_pin = 13, .gpio_port = GPIOE},
        .db6 = {.gpio_pin = 14, .gpio_port = GPIOE},
        .db7 = {.gpio_pin = 15, .gpio_port = GPIOE},
        .ena = {.gpio_pin = 11, .gpio_port = GPIOE},
        .rw = {.gpio_pin = 10, .gpio_port = GPIOE},
        .rs = {.gpio_pin = 7, .gpio_port = GPIOE},
    };

    initialize_display(
        &display_bus,
        DISP_4_BIT_BUS_MODE,
        DISP_TWO_LINE_MODE,
        DISP_FONT_5x11_DOTS,
        DISP_STATE_ON,
        DISP_CURSOR_STATE_ON,
        DISP_CURSOR_BLINK_ON,
        RIGHT,
        DISP_DONT_SHIFT
    );

    // TODO: fix read_busy_flag and read_cursor_address (merge)

    print_hello_screen();
    for(;;);
}
