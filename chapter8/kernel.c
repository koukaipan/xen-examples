#include "console.h"
#include "xenstore.h"
#include <x86_mm.h>

/* Some static space for the stack */
char stack[8192];

/* Locations in the bootstrapping code */
extern shared_info_t * shared_info;
void hypervisor_callback(void);
void failsafe_callback(void);

/* Main kernel entry point, called by trampoline */
void start_kernel(start_info_t * start_info)
{
	/* Define hypervisor upcall entry points */
        HYPERVISOR_set_callbacks(
                FLAT_KERNEL_CS, (unsigned long)hypervisor_callback,
                FLAT_KERNEL_CS, (unsigned long)failsafe_callback);
	/* Map the shared info page */
	HYPERVISOR_update_va_mapping((unsigned long) shared_info, 
			__pte(start_info->shared_info),
			UVMF_INVLPG);
	/* Initialise the console */
	console_init(start_info);
	/* Write a message to check that it worked */
	console_write("Hello world!\n\r");
	console_write("Xen magic string: ");
	console_write(start_info->magic);
	console_write("\n\r");

	/* Set up the XenStore driver */
	xenstore_init(start_info);
	/* Test the store */
	xenstore_test();
	/* Flush the console buffer */
	console_flush();
	/* We cannot directly leave here */
	for(;;);
}
