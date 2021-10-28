#include <stdint.h>
#include <stdbool.h>

#include "wh1602b/display.h"
#include "stm32f4/gpio.h"
#include "bit_operations.h"
#include "misc.h"


DISP_interface_mode_t DISPLAY_MODE = DISP_MODE_4_BIT;

static void send_enable_signal()
{
    software_delay(1);
    // Set ENA pin of display to high and then to low
    write_into_GPIOx_output_register(DISP_CONNECTED_PORT, 0x800, DISP_ENA_TO_GPIO_PIN_MASK);
    write_into_GPIOx_output_register(DISP_CONNECTED_PORT, 0x0, DISP_ENA_TO_GPIO_PIN_MASK);
    software_delay(1);
}

static uint16_t convert_display_instruction_into_GPIO_pins_representation(uint16_t instruction)
{
    uint16_t converted_instruction = 0x0;

    uint8_t current_bit = 0;
    for (uint16_t pin = 0; pin < GPIOx_PIN_COUNT; pin++) {
        if (IS_BIT_SET(DISP_DB_RS_RW_TO_GPIO_PINS_MASK, pin)) {
            if (IS_BIT_SET(instruction, current_bit))
                SET_BIT(converted_instruction, pin);
            else
                CLEAR_BIT(converted_instruction, pin);
            current_bit++;
        }
    }

    return converted_instruction;
}

static void send_to_display(DISP_write_mode_t mode, uint8_t data)
{
    if (DISPLAY_MODE == DISP_MODE_8_BIT) {
        // Make full instruction with RS and RW bits at lowest positions of the data
        uint16_t full_instruction = (data << 2) | mode;
        uint16_t converted_instruction = convert_display_instruction_into_GPIO_pins_representation(full_instruction);
        write_into_GPIOx_output_register(DISP_CONNECTED_PORT, converted_instruction, DISP_DB_RS_RW_TO_GPIO_PINS_MASK);
        send_enable_signal();
    } else {
        // Make first part of instruction with RS and RW bits at lowest positions of the data
        uint8_t part_1_of_instruction = (GET_4_HIGHEST_BITS_OF_8_BIT_NUM(data) << 2) | mode;
        uint16_t converted_part_1_of_instruction = convert_display_instruction_into_GPIO_pins_representation(part_1_of_instruction);
        write_into_GPIOx_output_register(DISP_CONNECTED_PORT, converted_part_1_of_instruction, DISP_DB_RS_RW_TO_GPIO_PINS_MASK);
        send_enable_signal();

        // Make first second of instruction with RS and RW bits at lowest positions of the data
        uint8_t part_2_of_instruction = (GET_4_LOWEST_BITS_OF_8_BIT_NUM(data) << 2) | mode;
        uint16_t converted_part_2_of_instruction = convert_display_instruction_into_GPIO_pins_representation(part_2_of_instruction);
        write_into_GPIOx_output_register(DISP_CONNECTED_PORT, converted_part_2_of_instruction, DISP_DB_RS_RW_TO_GPIO_PINS_MASK);
        send_enable_signal();
    }
}

void initialize_display(
    DISP_interface_mode_t mode, DISP_line_mode_t line_mode, DISP_font_type_t font_type,
    DISP_state_t display_state, DISP_cursor_state_t cursor_state, DISP_cursor_blink_state_t cursor_blink_state,
    direction_t direction, DISP_shift_t shift_display
)
{
    software_delay(20);
    set_function(mode, line_mode, font_type);
    display_on_off(display_state, cursor_state, cursor_blink_state);
    clear_display();
    software_delay(15);
    set_entry_mode(direction, shift_display);
}

void send_string_to_display(char *string)
{
    for (; *string; string++)
        send_to_display(DISP_WRITE_DATA_MODE, (uint8_t)(*string));
}

void send_character_to_display(char character)
{
    send_to_display(DISP_WRITE_DATA_MODE, (uint8_t)character);
}

void clear_display()
{
    send_to_display(DISP_WRITE_COMMAND_MODE, 0x1);
}

void return_home()
{
    send_to_display(DISP_WRITE_COMMAND_MODE, 0x2);
}

void set_entry_mode(direction_t direction, DISP_shift_t shift_display)
{
    uint8_t command = 0x4 + (direction << 1) + (shift_display << 0);
    send_to_display(DISP_WRITE_COMMAND_MODE, command);
}

void display_on_off(DISP_state_t display_state, DISP_cursor_state_t cursor_state, DISP_cursor_blink_state_t cursor_blink_state)
{
    uint8_t command = 0x8 + (display_state << 2) + (cursor_state << 1) + (cursor_blink_state << 0);
    send_to_display(DISP_WRITE_COMMAND_MODE, command);
}

void shift_cursor_or_display(DISP_shift_type_t shift_type, direction_t direction)
{
    uint8_t command = 0x10 + (direction << 3) + (shift_type << 2);
    send_to_display(DISP_WRITE_COMMAND_MODE, command);
}

void set_function(DISP_interface_mode_t mode, DISP_line_mode_t line_mode, DISP_font_type_t font_type)
{
    DISPLAY_MODE = mode;
    uint8_t command = 0x20 + (mode << 4) + (line_mode << 3) + (font_type << 2);
    send_to_display(DISP_WRITE_COMMAND_MODE, command);
}
