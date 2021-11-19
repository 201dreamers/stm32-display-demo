#ifndef _BIT_OPERATIONS_H
#define _BIT_OPERATIONS_H

#define GET_BIT(number, position) (((number) & (1 << (position))) >> (position))

#define SET_BIT(destination, position) ((destination) |= (1 << (position)))
#define CLEAR_BIT(destination, position) ((destination) &= ~(1 << (position)))
#define TOGGLE_BIT(destination, position) ((destination) ^= (1 << (position)))
#define GET_4_HIGHEST_BITS_OF_8_BIT_NUM(number) (((number) >> 4) & 0xf)
#define GET_4_LOWEST_BITS_OF_8_BIT_NUM(number) (((number) >> 0) & 0xf)

#endif /*_BIT_OPERATIONS_H */
