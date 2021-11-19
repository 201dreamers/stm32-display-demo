#ifndef _WH1602B_DISPLAY_H
#define _WH1602B_DISPLAY_H

#include <stdbool.h>

#include "stm32f4/gpio.h"


#define DATA_PINS_NUMBER 8

struct disp_pin {
    const uint16_t gpio_pin;
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

typedef enum {
    LEFT,
    RIGHT
} direction_type;

typedef enum {
    DISP_WRITE_COMMAND_MODE = 0x0,
    DISP_WRITE_DATA_MODE = 0x2
} disp_write_mode;

typedef enum {
    DISP_READ_BUSY_FLAG_AND_ADDR = 0x1,
    DISP_READ_DATA_MODE = 0x3
} disp_read_mode;

typedef enum {
    DISP_4_BITS_BUS_MODE = 0x0,
    DISP_8_BITS_BUS_MODE = 0x1
} disp_bus_mode;

typedef enum {
    DISP_FONT_5x8_DOTS,
    DISP_FONT_5x11_DOTS
} disp_font_size;

typedef enum {
    DISP_DONT_SHIFT,
    DISP_DO_SHIFT
} disp_shift;

typedef enum {
    DISP_STATE_OFF,
    DISP_STATE_ON
} disp_state;

typedef enum {
    DISP_CURSOR_STATE_OFF,
    DISP_CURSOR_STATE_ON
} disp_cursor_state;

typedef enum {
    DISP_CURSOR_BLINK_OFF,
    DISP_CURSOR_BLINK_ON
} disp_cursor_blink_state;

typedef enum {
    DISP_CURSOR_SHIFT,
    DISP_SHIFT
} disp_shift_type;

typedef enum {
    DISP_ONE_LINE_MODE,
    DISP_TWO_LINE_MODE
} disp_line_mode;

void initialize_display(
    const struct disp_bus *display_bus, disp_bus_mode bus_mode,
    disp_line_mode line_mode, disp_font_size font_type,
    disp_state display_state, disp_cursor_state cursor_state,
    disp_cursor_blink_state cursor_blink_state,
    direction_type direction, disp_shift shift_display
);
void send_string_to_display(char *string);
void send_character_to_display(char character);
void clear_display();
void return_home();
void set_entry_mode(direction_type direction, disp_shift shift_display);
void display_on_off(disp_state display_state, disp_cursor_state cursor_state, disp_cursor_blink_state cursor_blink_state);
void shift_cursor_or_display(disp_shift_type shift_type, direction_type direction);
void set_function(disp_bus_mode bus_mode, disp_line_mode line_mode, disp_font_size font_type);

void set_CGRAM_address(uint8_t addr);
void set_DDRAM_address(uint8_t addr);
uint8_t read_busy_flag();
uint8_t read_cusor_address();

#endif /* _WH1602B_DISPLAY_H */
