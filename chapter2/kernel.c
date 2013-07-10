#include <stdint.h> 
#include <xen.h>
#include "debug.h"

/* Some static space for the stack */
char stack[8192];

/* Main kernel entry point, called by trampoline */
void start_kernel(start_info_t * start_info)
{
	HYPERVISOR_console_io(CONSOLEIO_write,12,"Hello World\n");
	while(1);
}
