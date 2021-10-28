#ifndef _WH1602B_DISPLAY_H
#define _WH1602B_DISPLAY_H

#include <stdbool.h>

#include "stm32f4/gpio.h"


/* To use this file, please configure defines in this block */
#define DISP_CONNECTED_PORT GPIOE
/* 0xf480 is a mask for pins DB -> [GPIOE_15, GPIOE_14, GPIOE_13, GPIOE_12]
 * RW -> [GPIOE_10], RS -> [GPIOE_7] without ENA pin
 * These bits are used to send data and commands to display
 */
#define DISP_DB_RS_RW_TO_GPIO_PINS_MASK 0xf480
/* 0x800 is a mask for ENA -> [GPIOE_11] */
#define DISP_ENA_TO_GPIO_PIN_MASK 0x800
/* ===== */

#define DISP_CONNECTED_GPIO_PINS_MASK ((DISP_DB_RS_RW_TO_GPIO_PINS_MASK) | (DISP_ENA_TO_GPIO_PIN_MASK))

#define DISP_LINE_LENGTH 16

typedef enum {
    LEFT,
    RIGHT
} direction_t;

typedef enum {
    DISP_WRITE_COMMAND_MODE = 0x0,
    DISP_WRITE_DATA_MODE = 0x1
} DISP_write_mode_t;

typedef enum {
    DISP_READ_BUSY_FLAG_AND_ADDR = 0x2,
    DISP_READ_DATA_MODE = 0x3
} DISP_read_mode_t;

typedef enum {
    DISP_MODE_4_BIT,
    DISP_MODE_8_BIT
} DISP_interface_mode_t;

typedef enum {
    DISP_FONT_5x8_DOTS,
    DISP_FONT_5x11_DOTS
} DISP_font_type_t;

typedef enum {
    DISP_DONT_SHIFT,
    DISP_DO_SHIFT
} DISP_shift_t;

typedef enum {
    DISP_STATE_OFF,
    DISP_STATE_ON
} DISP_state_t;

typedef enum {
    DISP_CURSOR_STATE_OFF,
    DISP_CURSOR_STATE_ON
} DISP_cursor_state_t;

typedef enum {
    DISP_CURSOR_BLINK_OFF,
    DISP_CURSOR_BLINK_ON
} DISP_cursor_blink_state_t;

typedef enum {
    DISP_CURSOR_SHIFT,
    DISP_SHIFT
} DISP_shift_type_t;

typedef enum {
    DISP_ONE_LINE_MODE,
    DISP_TWO_LINE_MODE
} DISP_line_mode_t;

extern DISP_interface_mode_t DISPLAY_MODE;

void clear_display();
void return_home();
void set_entry_mode(direction_t direction, DISP_shift_t shift_display);
void display_on_off(DISP_state_t display_state, DISP_cursor_state_t cursor_state, DISP_cursor_blink_state_t cursor_blink_state);
void shift_cursor_or_display(DISP_shift_type_t shift_type, direction_t direction);
void set_function(DISP_interface_mode_t mode, DISP_line_mode_t line_mode, DISP_font_type_t font_type);
void initialize_display(
    DISP_interface_mode_t mode, DISP_line_mode_t line_mode, DISP_font_type_t font_type,
    DISP_state_t display_state, DISP_cursor_state_t cursor_state, DISP_cursor_blink_state_t cursor_blink_state,
    direction_t direction, DISP_shift_t shift_display
);
void send_string_to_display(char *string);
void send_character_to_display(char character);

// Not implemented yet
void set_CGRAM_address();
void set_DDRAM_address();
void read_busy_flag_and_address();

#endif /* _WH1602B_DISPLAY_H */
