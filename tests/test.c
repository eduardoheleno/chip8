#include <stdio.h>

#include "cpu.h"

cpu_t cpu;
video_t video;

void test_0x1200();
void test_0x14ef();

int main(void)
{
	init_cpu(&cpu);
	init_video(&video);

	test_0x1200();
	test_0x14ef();

	return 0;
}

void test_0x1200()
{
	uint16_t opcode = 0x1200; /* 200 = 512 */
	opcode_decoder(opcode, &cpu, &video);

	if (cpu.pc == 512) {
		printf("test_0x1200................passed\n");
	} else {
		printf("test_0x1200................failed\n");
	}
}

void test_0x14ef()
{
	uint16_t opcode = 0x14EF; /* 4EF = 1263 */
	opcode_decoder(opcode, &cpu, &video);

	if (cpu.pc == 1263) {
		printf("test_0x14EF................passed\n");
	} else {
		printf("test_0x14EF................failed\n");
	}
}
