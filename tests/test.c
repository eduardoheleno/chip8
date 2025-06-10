#include <stdio.h>

#include "cpu.h"

cpu_t cpu;
video_t video;

void test_0x00ee();
void test_0x1200();
void test_0x14ef();
void test_0x211e();
void test_0x8540();
void test_0x80d1();
void test_0x88e2();
void test_0x84b3();
void test_0x8564();
void test_0x8785();
void test_0x8236();

int main(void)
{
	init_cpu(&cpu);
	init_video(&video);

	test_0x00ee();
	test_0x1200();
	test_0x14ef();
	test_0x211e();
	test_0x8540();
	test_0x80d1();
	test_0x88e2();
	test_0x84b3();
	test_0x8564();
	test_0x8785();
	test_0x8236();

	return 0;
}

void test_0x00ee()
{
	uint16_t opcode = 0x211E; /* 11E = 286 */
	cpu.pc = 0x45C; /* 45C = 1116 */
	opcode_decoder(opcode, &cpu, &video);

	opcode = 0x00EE;
	opcode_decoder(opcode, &cpu, &video);

	if (cpu.pc == 1116 && cpu.sp == 0) {
		printf("test_0x00ee................passed\n");
	} else {
		printf("test_0x00ee................failed\n");
	}

	init_cpu(&cpu);
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

	init_cpu(&cpu);
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

	init_cpu(&cpu);
}

void test_0x211e()
{
	uint16_t opcode = 0x211E; /* 11E = 286 */
	cpu.pc = 0x45C; /* 45C = 1116 */
	opcode_decoder(opcode, &cpu, &video);

	if (cpu.pc == 286 && cpu.stack[cpu.sp - 1] == 1116) {
		printf("test_0x211E................passed\n");
	} else {
		printf("test_0x211E................failed\n");
	}

	init_cpu(&cpu);
}

void test_0x8540()
{
	uint16_t opcode = 0x8540; /* v[5] v[4] */
	cpu.sys_reg[4] = 10;
	opcode_decoder(opcode, &cpu, &video);

	if (cpu.sys_reg[5] == 10) {
		printf("test_0x8540................passed\n");
	} else {
		printf("test_0x8540................failed\n");
	}

	init_cpu(&cpu);
}

void test_0x80d1()
{
	uint16_t opcode = 0x80D1; /* v[0] v[13] */
	cpu.sys_reg[0] = 6;
	cpu.sys_reg[13] = 8;  /* 6 | 8 */

	opcode_decoder(opcode, &cpu, &video);

	if (cpu.sys_reg[0] == 14) {
		printf("test_0x80D1................passed\n");
	} else {
		printf("test_0x80D1................failed\n");
	}

	init_cpu(&cpu);
}

void test_0x88e2()
{
	uint16_t opcode = 0x88E2; /* v[8] v[14] */
	cpu.sys_reg[8] = 10;
	cpu.sys_reg[14] = 20;

	opcode_decoder(opcode, &cpu, &video);

	if (cpu.sys_reg[8] == 0) {
		printf("test_0x88E2................passed\n");
	} else {
		printf("test_0x88E2................failed\n");
	}

	init_cpu(&cpu);
}

void test_0x84b3()
{
	uint16_t opcode = 0x84B3; /* v[4] v[11] */
	cpu.sys_reg[4] = 11;
	cpu.sys_reg[11] = 15;

	opcode_decoder(opcode, &cpu, &video);

	if (cpu.sys_reg[4] == 4) {
		printf("test_0x84B3................passed\n");
	} else {
		printf("test_0x84B3................failed\n");
	}

	init_cpu(&cpu);
}

void test_0x8564()
{
	uint16_t opcode = 0x8564;
	cpu.sys_reg[5] = 200;
	cpu.sys_reg[6] = 100;

	opcode_decoder(opcode, &cpu, &video);

	if (cpu.sys_reg[0xF] == 1 && cpu.sys_reg[5] == 44) {
		printf("test_0x8564................passed\n");
	} else {
		printf("test_0x8564................failed\n");
	}

	init_cpu(&cpu);

	cpu.sys_reg[5] = 10;
	cpu.sys_reg[6] = 30;

	opcode_decoder(opcode, &cpu, &video);

	if (cpu.sys_reg[0xF] == 0 && cpu.sys_reg[5] == 40) {
		printf("test_0x8564(2).............passed\n");
	} else {
		printf("test_0x8564(2).............failed\n");
	}

	init_cpu(&cpu);
}

void test_0x8785()
{
	uint16_t opcode = 0x8785;
	cpu.sys_reg[7] = 23;
	cpu.sys_reg[8] = 32;

	opcode_decoder(opcode, &cpu, &video);

	if (cpu.sys_reg[0xF] == 0 && (cpu.sys_reg[7] & 0xFF) == 247) {
		printf("test_0x8785................passed\n");
	} else {
		printf("test_0x8785................failed\n");
	}

	init_cpu(&cpu);

	cpu.sys_reg[7] = 32;
	cpu.sys_reg[8] = 23;

	opcode_decoder(opcode, &cpu, &video);

	if (cpu.sys_reg[0xF] == 1 && cpu.sys_reg[7] == 9) {
		printf("test_0x8785(2).............passed\n");
	} else {
		printf("test_0x8785(2).............failed\n");
	}

	init_cpu(&cpu);
}

void test_0x8236()
{
	uint16_t opcode = 0x8236;
	cpu.sys_reg[2] = 21;

	opcode_decoder(opcode, &cpu, &video);

	init_cpu(&cpu);
}
