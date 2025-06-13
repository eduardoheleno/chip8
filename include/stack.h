#ifndef STACK_H_
#define STACK_H_

#include <stdint.h>

void stack_up(uint16_t *stack, uint8_t *sp, uint16_t value, uint16_t opcode);
uint16_t stack_down(uint16_t *stack, uint8_t *sp, uint16_t opcode);

#endif // STACK_H_
