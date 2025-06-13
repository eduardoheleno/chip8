#include "stack.h"

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#define MAX_STACK_SIZE 15

void stack_up(uint16_t *stack, uint8_t *sp, uint16_t value, uint16_t opcode)
{
	if (*sp >= MAX_STACK_SIZE) {
		printf("Stack overflow - opcode: %04x - %s\n", opcode, strerror(errno));
		exit(EXIT_FAILURE);
	}

	stack[*sp] = value;
	(*sp)++;
}

uint16_t stack_down(uint16_t *stack, uint8_t *sp, uint16_t opcode)
{
	if (*sp <= 0) {
		printf("Stack overflow - opcode: %04x - %s\n", opcode, strerror(errno));
		exit(EXIT_FAILURE);
	}

	(*sp)--;
	return stack[*sp];
}
