#ifndef _WH1602B_H
#define _WH1602B_H

#include <stdint.h>

#include "stm32f4/gpio.h"


#define DISP_4_BIT_BUS_MODE 0x0
#define DISP_8_BIT_BUS_MODE 0x1

#define DISP_ONE_LINE_MODE 0x0
#define DISP_TWO_LINE_MODE 0x1

#define DISP_FONT_5x8_DOTS  0x0
#define DISP_FONT_5x11_DOTS 0x1

#define DISP_STATE_OFF 0x0
#define DISP_STATE_ON  0x1

#define DISP_CURSOR_STATE_OFF 0x0
#define DISP_CURSOR_STATE_ON  0x1

#define DISP_CURSOR_BLINK_OFF 0x0
#define DISP_CURSOR_BLINK_ON  0x1

#define LEFT  0x0
#define RIGHT 0x1

#define DISP_DONT_SHIFT 0x0
#define DISP_DO_SHIFT   0x1

#define DISP_CURSOR_SHIFT 0x0
#define DISP_SHIFT        0x1

#define DISP_WRITE_COMMAND_MODE      0x0
#define DISP_WRITE_DATA_MODE         0x2
#define DISP_READ_BUSY_FLAG_AND_ADDR 0x1
#define DISP_READ_DATA_MODE          0x3

struct disp_pin {
    const uint8_t gpio_pin;
    GPIOx gpio_port;
};

struct disp_bus {
    const struct disp_pin rs;
    const struct disp_pin rw;
    const struct disp_pin ena;
    const struct disp_pin db7;
    const struct disp_pin db6;
    const struct disp_pin db5;
    const struct disp_pin db4;
    const struct disp_pin db3;
    const struct disp_pin db2;
    const struct disp_pin db1;
    const struct disp_pin db0;
};

void initialize_display(
    const struct disp_bus *display_bus,
    uint8_t bus_mode,
    uint8_t line_mode,
    uint8_t font_type,
    uint8_t display_state,
    uint8_t cursor_state,
    uint8_t cursor_blink_state,
    uint8_t direction,
    uint8_t shift_display
);

void send_string_to_display(char *string);
void send_character_to_display(char character);

void clear_display();
void return_home();
void set_entry_mode(uint8_t direction, uint8_t shift_display);
void set_display_and_cursor_state(
    uint8_t display_state,
    uint8_t cursor_state,
    uint8_t cursor_blink_state
);
void shift_cursor_or_display(uint8_t shift_type, uint8_t direction);
void configure_function_set(
    uint8_t bus_mode,
    uint8_t line_mode,
    uint8_t font_type
);

void set_CGRAM_address(uint8_t addr);
void set_DDRAM_address(uint8_t addr);

#endif /* _WH1602B_H */
