#include "event.h"
#include "console.h"
#include <xen/features.h>

/* Some static space for the stack */
char stack[8192];

uint8_t xen_features[XENFEAT_NR_SUBMAPS * 32];

extern shared_info_t shared_info;
extern void handle_input(evtchn_port_t port, struct pt_regs * regs);

shared_info_t *HYPERVISOR_shared_info;

/* Main kernel entry point, called by trampoline */
void start_kernel(start_info_t * start_info)
{
	/* Map the shared info page */
	HYPERVISOR_update_va_mapping((unsigned long) &shared_info, 
			__pte(start_info->shared_info | 7),
			UVMF_INVLPG);
	/* Set the pointer used in the bootstrap for reenabling
	 * event delivery after an upcall */
	HYPERVISOR_shared_info = &shared_info;
	/* Set up and unmask events */
	init_events();
	/* Initialise the console */
	console_init(start_info);
	/* Write a message to check that it worked */
	console_write("Hello world!\r\n");
	/* Loop, handling events */
	while(1)
	{
		HYPERVISOR_sched_op(SCHEDOP_block,0);
	}
}
