#include <stdio.h>
#include <sys/types.h>
#include <string.h>

typedef union 
{
	uint32_t r[3];
	char string[12];
} cpuid_t;

#define CPUID(command, result) \
	__asm __volatile(\
			"CPUID"\
			: "=b" (result.r[0]), "=c" (cpu.r[1]), "=d" (cpu.r[2])\
			: "a" (command));

int main(void)
{
	cpuid_t cpu;
	CPUID(0,cpu);
	if(strncmp(cpu.string, "XenVMMXenVMM", 12) == 0)
	{
		printf("Running as a Xen HVM guest\n");
	}
	else
	{
		printf("Running on native hardware or a non-Xen hypervisor.\n");
	}
	return 0;
}
