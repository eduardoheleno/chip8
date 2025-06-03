#include "cpu.h"

#include <string.h>

void init_cpu(cpu_t *c)
{
	memset(c->sys_ram, 0, sizeof(c->sys_ram));
	memset(c->sys_reg, 0, sizeof(c->sys_reg));

	c->I = 0;
	c->pc = 0x200;
}
