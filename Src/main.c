#include <stdint.h>
#include <stdio.h>

#include "stm32f4/gpio.h"
#include "wh1602b/display.h"
#include "bit_operations.h"
#include "misc.h"


#if !defined(__SOFT_FP__) && defined(__ARM_FP)
    #warning "FPU is not initialized, but the project is compiling for an FPU. Please initialize the FPU before use."
#endif

static void print_hello_screen()
{
    send_string_to_display("Demo: VDD->PD15");
    return_home();
}

static void print_all_symbols_pattern()
{
    clear_display();
    for(uint8_t character = 0; character <= 0xff;) {
        return_home();
        for (uint8_t i = 0; i < (DISP_LINE_LENGTH / 2); i++) {
            send_character_to_display(character++);
            send_character_to_display(' ');
            software_delay(200);
        }
    }
}

int main(void)
{
    enable_GPIOx_clock(DISP_CONNECTED_PORT);
    reset_GPIOx(DISP_CONNECTED_PORT);
    set_GPIOx_mode(DISP_CONNECTED_PORT, GPIO_OUTPUT_MODE, DISP_CONNECTED_GPIO_PINS_MASK);

    enable_GPIOx_clock(GPIOD);
    reset_GPIOx(GPIOD);
    set_GPIOx_mode(GPIOD, GPIO_INPUT_MODE, (1 << 15));

    initialize_display(
        DISP_MODE_4_BIT,
        DISP_TWO_LINE_MODE,
        DISP_FONT_5x8_DOTS,
        DISP_STATE_ON,
        DISP_CURSOR_STATE_ON,
        DISP_CURSOR_BLINK_ON,
        RIGHT,
        DISP_DONT_SHIFT
    );

    for(;;) {
        if (IS_BIT_SET(GPIOD->input_data, 15)) {
            print_all_symbols_pattern();
        } else {
            print_hello_screen();
        }
    }
}
