#include <stdint.h>
#include <stdbool.h>
// #include <stdlib.h>

#include "wh1602b.h"
#include "stm32f4/gpio.h"
#include "bit_operations.h"
#include "misc.h"


static uint8_t DISPLAY_BUS_MODE;
static const struct disp_bus *DISPLAY_BUS;

static void send_enable_signal()
{
    sleep_us(1000);
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
    sleep_us(1000);
}

static void set_display_mode(uint8_t mode)
{
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

static void write_into_display_data_pins(uint8_t data)
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
    uint8_t last_bus_pin_num = (DISPLAY_BUS_MODE == DISP_4_BIT_BUS_MODE) ? 4 : 0;
    for (uint8_t pin_num = 7; pin_num >= last_bus_pin_num; pin_num--)
        write_into_GPIOx_pin(
            GET_BIT(data, pin_num - last_bus_pin_num),
            data_pins[pin_num].gpio_pin,
            data_pins[pin_num].gpio_port
        );
}

static void send_to_display(uint8_t data, uint8_t write_mode)
{
    set_display_mode(write_mode);

    if (DISPLAY_BUS_MODE == DISP_4_BIT_BUS_MODE) {
        write_into_display_data_pins(
            GET_4_HIGHEST_BITS_OF_8_BIT_NUM(data));
        send_enable_signal();

        write_into_display_data_pins(
            GET_4_LOWEST_BITS_OF_8_BIT_NUM(data));
        send_enable_signal();
    } else {
        write_into_display_data_pins(data);
        send_enable_signal();
    }
}

static void configure_display_gpio_pins()
{
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

    uint8_t last_bus_pin_num = (DISPLAY_BUS_MODE == DISP_4_BIT_BUS_MODE) ? 4 : 0;
    for (uint8_t pin_num = 10; pin_num >= last_bus_pin_num; pin_num--) {
        enable_GPIOx_clock(display_pins[pin_num].gpio_port);
        set_GPIOx_pin_mode(
            GPIO_OUTPUT_MODE,
            display_pins[pin_num].gpio_pin,
            display_pins[pin_num].gpio_port
        );
    }
}

static void make_4_bit_bus_pre_initialization()
{
    write_into_display_data_pins(0x2);
    send_enable_signal();
    write_into_display_data_pins(0x2);
    send_enable_signal();
    write_into_display_data_pins(0x2);
    send_enable_signal();
    write_into_display_data_pins(0x2);
    send_enable_signal();
}

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
)
{
    DISPLAY_BUS_MODE = bus_mode;
    DISPLAY_BUS = display_bus;

    configure_display_gpio_pins();

    sleep_us(20000);

    if (DISPLAY_BUS_MODE == DISP_4_BIT_BUS_MODE)
        make_4_bit_bus_pre_initialization();

    configure_function_set(bus_mode, line_mode, font_type);
    sleep_us(100);
    set_display_and_cursor_state(display_state, cursor_state, cursor_blink_state);
    sleep_us(100);
    clear_display();
    sleep_us(2000);
    set_entry_mode(direction, shift_display);
}

void send_string_to_display(char *string)
{
    for (; *string; string++)
        send_to_display((uint8_t)(*string), DISP_WRITE_DATA_MODE);
}

void clear_display()
{
    send_to_display(0x1, DISP_WRITE_COMMAND_MODE);
}

void return_home()
{
    send_to_display(0x2, DISP_WRITE_COMMAND_MODE);
}

void set_entry_mode(uint8_t direction, uint8_t shift_display)
{
    uint8_t command = 0x4 | (direction << 1) | (shift_display << 0);
    send_to_display(command, DISP_WRITE_COMMAND_MODE);
}

void set_display_and_cursor_state(uint8_t display_state, uint8_t cursor_state, uint8_t cursor_blink_state)
{
    uint8_t command = 0x8 | (display_state << 2) | (cursor_state << 1) | (cursor_blink_state << 0);
    send_to_display(command, DISP_WRITE_COMMAND_MODE);
}

void shift_cursor_or_display(uint8_t shift_type, uint8_t direction)
{
    uint8_t command = 0x10 | (direction << 3) | (shift_type << 2);
    send_to_display(command, DISP_WRITE_COMMAND_MODE);
}

void configure_function_set(uint8_t bus_mode, uint8_t line_mode, uint8_t font_type)
{
    DISPLAY_BUS_MODE = bus_mode;
    uint8_t command = 0x20 | (bus_mode << 4) | (line_mode << 3) | (font_type << 2);
    send_to_display(command, DISP_WRITE_COMMAND_MODE);
}

void set_CGRAM_address(uint8_t addr)
{
    uint8_t command = 0x40 | addr;
    send_to_display(command, DISP_WRITE_COMMAND_MODE);
}

void set_DDRAM_address(uint8_t addr)
{
    uint8_t command = 0x80 | addr;
    send_to_display(command, DISP_WRITE_COMMAND_MODE);
}
