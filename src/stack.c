#include "stack.h"
#include <stdio.h>
#include <stdlib.h>

#define MAX_STACK_SIZE 15

void stack_up(uint16_t *stack, uint8_t *sp, uint16_t value)
{
	if (*sp >= MAX_STACK_SIZE) {
		perror("Stack overflow!");
		exit(EXIT_FAILURE);
	}

	stack[*sp] = value;
	(*sp)++;
}

uint16_t stack_down(uint16_t *stack, uint8_t *sp)
{
	if (*sp <= 0) {
		perror("Stack overflow!");
		exit(EXIT_FAILURE);
	}

	(*sp)--;
	return stack[*sp];
}
