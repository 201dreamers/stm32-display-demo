#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#include "wh1602b/display.h"
#include "stm32f4/gpio.h"
#include "bit_operations.h"
#include "misc.h"


static disp_bus_mode DISPLAY_BUS_MODE = DISP_4_BITS_BUS_MODE;
static const struct disp_bus *DISPLAY_BUS;

static void send_enable_signal()
{
    software_delay_us(1000);
    // Set ENA pin of display to high
    write_into_GPIOx_pin(
        1,
        DISPLAY_BUS->ena.gpio_pin,
        DISPLAY_BUS->ena.gpio_port
    );
    // Set ENA pin of display to low
    write_into_GPIOx_pin(
        0,
        DISPLAY_BUS->ena.gpio_pin,
        DISPLAY_BUS->ena.gpio_port
    );
    software_delay_us(1000);
}

static void set_display_mode(uint8_t mode)
{
   if (
       mode != DISP_WRITE_COMMAND_MODE
       && mode != DISP_WRITE_DATA_MODE
       && mode != DISP_READ_BUSY_FLAG_AND_ADDR
       && mode != DISP_READ_DATA_MODE
   )
       exit(EXIT_FAILURE);  /* No such display mode */

    write_into_GPIOx_pin(
        GET_BIT(mode, 1),
        DISPLAY_BUS->rs.gpio_pin,
        DISPLAY_BUS->rs.gpio_port
    );
    write_into_GPIOx_pin(
        GET_BIT(mode, 0),
        DISPLAY_BUS->rw.gpio_pin,
        DISPLAY_BUS->rw.gpio_port
    );
}

static void write_into_display_data_pins(uint8_t data, disp_bus_mode bus_mode)
{
    struct disp_pin data_pins[8] = {
        DISPLAY_BUS->db0,
        DISPLAY_BUS->db1,
        DISPLAY_BUS->db2,
        DISPLAY_BUS->db3,
        DISPLAY_BUS->db4,
        DISPLAY_BUS->db5,
        DISPLAY_BUS->db6,
        DISPLAY_BUS->db7
    };

    uint8_t last_bus_pin_num = (bus_mode == DISP_4_BITS_BUS_MODE) ? 4 : 0;
    for (uint8_t pin_num = 7; pin_num >= last_bus_pin_num; pin_num--)
        write_into_GPIOx_pin(
            GET_BIT(data, pin_num - last_bus_pin_num),
            data_pins[pin_num].gpio_pin,
            data_pins[pin_num].gpio_port
        );
}

static uint8_t read_from_display_data_pins(disp_bus_mode bus_mode)
{
    struct disp_pin data_pins[8] = {
        DISPLAY_BUS->db0,
        DISPLAY_BUS->db1,
        DISPLAY_BUS->db2,
        DISPLAY_BUS->db3,
        DISPLAY_BUS->db4,
        DISPLAY_BUS->db5,
        DISPLAY_BUS->db6,
        DISPLAY_BUS->db7
    };
    uint8_t result = 0;

    uint8_t last_bus_pin_num = (bus_mode == DISP_4_BITS_BUS_MODE) ? 4 : 0;
    for (uint8_t pin_num = 7; pin_num >= last_bus_pin_num; pin_num--) {
        result <<= 1;
        result += read_from_GPIOx_pin(
            data_pins[pin_num].gpio_pin,
            data_pins[pin_num].gpio_port
        );
    }

    return result;
}

static void send_to_display(uint8_t data, disp_write_mode write_mode)
{
    set_display_mode(write_mode);

    if (DISPLAY_BUS_MODE == DISP_4_BITS_BUS_MODE) {
        write_into_display_data_pins(
            GET_4_HIGHEST_BITS_OF_8_BIT_NUM(data),
            DISP_4_BITS_BUS_MODE
        );
        send_enable_signal();

        write_into_display_data_pins(
            GET_4_LOWEST_BITS_OF_8_BIT_NUM(data),
            DISP_4_BITS_BUS_MODE
        );
        send_enable_signal();
    } else {
        write_into_display_data_pins(
            data,
            DISP_8_BITS_BUS_MODE
        );
        send_enable_signal();
    }
}

static uint8_t read_from_display(disp_read_mode read_mode)
{
    set_display_mode(read_mode);

    uint8_t result = 0;
    if (DISPLAY_BUS_MODE == DISP_4_BITS_BUS_MODE) {
        send_enable_signal();
        result = read_from_display_data_pins(DISP_4_BITS_BUS_MODE);

        result <<= 4;

        send_enable_signal();
        result += read_from_display_data_pins(DISP_4_BITS_BUS_MODE);
    } else {
        send_enable_signal();
        result = read_from_display_data_pins(DISP_8_BITS_BUS_MODE);
    }

    return result;
}

void configure_display_gpio_pins(const struct disp_bus *display_bus)
{
    DISPLAY_BUS = display_bus;
    struct disp_pin display_pins[11] = {
        DISPLAY_BUS->db0,
        DISPLAY_BUS->db1,
        DISPLAY_BUS->db2,
        DISPLAY_BUS->db3,
        DISPLAY_BUS->db4,
        DISPLAY_BUS->db5,
        DISPLAY_BUS->db6,
        DISPLAY_BUS->db7,
        DISPLAY_BUS->ena,
        DISPLAY_BUS->rw,
        DISPLAY_BUS->rs
    };

    uint8_t last_bus_pin_num = (DISPLAY_BUS_MODE == DISP_4_BITS_BUS_MODE) ? 4 : 0;
    for (uint8_t pin_num = 10; pin_num >= last_bus_pin_num; pin_num--) {
        enable_GPIOx_clock(display_pins[pin_num].gpio_port);
        set_GPIOx_pin_mode(
            GPIO_OUTPUT_MODE,
            display_pins[pin_num].gpio_pin,
            display_pins[pin_num].gpio_port
        );
    }
}

static void make_4_bits_pre_initialization()
{
    write_into_display_data_pins(
        0x2 | DISP_4_BITS_BUS_MODE,
        DISP_4_BITS_BUS_MODE
    );
    send_enable_signal();
    write_into_display_data_pins(
        0x2 | DISP_4_BITS_BUS_MODE,
        DISP_4_BITS_BUS_MODE
    );
    send_enable_signal();
    write_into_display_data_pins(
        0x2 | DISP_4_BITS_BUS_MODE,
        DISP_4_BITS_BUS_MODE
    );
    send_enable_signal();
    write_into_display_data_pins(
        0x2,
        DISP_4_BITS_BUS_MODE
    );
    send_enable_signal();
}

void initialize_display(
    const struct disp_bus *display_bus, disp_bus_mode bus_mode,
    disp_line_mode line_mode, disp_font_size font_type,
    disp_state display_state, disp_cursor_state cursor_state,
    disp_cursor_blink_state cursor_blink_state,
    direction_type direction, disp_shift shift_display
)
{
    configure_display_gpio_pins(display_bus);

    software_delay_us(40000);

    if (bus_mode == DISP_4_BITS_BUS_MODE)
        make_4_bits_pre_initialization();

    set_function(bus_mode, line_mode, font_type);
    display_on_off(display_state, cursor_state, cursor_blink_state);
    clear_display();
    software_delay_us(2000);
    set_entry_mode(direction, shift_display);
}

void send_string_to_display(char *string)
{
    for (; *string; string++)
        send_character_to_display((uint8_t)(*string));
}

void send_character_to_display(char character)
{
//    if (character == '\n')
//        move_cursor_to_another_line();
//    else
    send_to_display(character, DISP_WRITE_DATA_MODE);
}

void clear_display()
{
    send_to_display(0x1, DISP_WRITE_COMMAND_MODE);
}

void return_home()
{
    send_to_display(0x2, DISP_WRITE_COMMAND_MODE);
}

void set_entry_mode(direction_type direction, disp_shift shift_display)
{
    uint8_t command = 0x4 + (direction << 1) + (shift_display << 0);
    send_to_display(command, DISP_WRITE_COMMAND_MODE);
}

void display_on_off(disp_state display_state, disp_cursor_state cursor_state, disp_cursor_blink_state cursor_blink_state)
{
    uint8_t command = 0x8 + (display_state << 2) + (cursor_state << 1) + (cursor_blink_state << 0);
    send_to_display(command, DISP_WRITE_COMMAND_MODE);
}

void shift_cursor_or_display(disp_shift_type shift_type, direction_type direction)
{
    uint8_t command = 0x10 + (direction << 3) + (shift_type << 2);
    send_to_display(command, DISP_WRITE_COMMAND_MODE);
}

void set_function(disp_bus_mode bus_mode, disp_line_mode line_mode, disp_font_size font_type)
{
    DISPLAY_BUS_MODE = bus_mode;
    uint8_t command = 0x20 + (bus_mode << 4) + (line_mode << 3) + (font_type << 2);
    send_to_display(command, DISP_WRITE_COMMAND_MODE);
}

void set_CGRAM_address(uint8_t addr)
{
    if (addr > 0x3f)
        exit(EXIT_FAILURE);  /* Too big address */

    uint8_t command = 0x40 + addr;
    send_to_display(command, DISP_WRITE_COMMAND_MODE);
}

void set_DDRAM_address(uint8_t addr)
{
    if (addr > 0x7f)
        exit(EXIT_FAILURE);  /* Too big address */

    uint8_t command = 0x80 + addr;
    send_to_display(command, DISP_WRITE_COMMAND_MODE);
}

uint8_t read_busy_flag()
{
    uint8_t data = read_from_display(DISP_READ_BUSY_FLAG_AND_ADDR);
    uint8_t busy_flag = GET_BIT(data, 7);
    return busy_flag;
}

uint8_t read_cusor_address()
{
    uint8_t data = read_from_display(DISP_READ_BUSY_FLAG_AND_ADDR);
    uint8_t cursor_address = CLEAR_BIT(data, 7);
    return cursor_address;
}

void move_cursor_to_position(uint8_t x, uint8_t y)
{

}
