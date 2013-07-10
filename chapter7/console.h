#include <stdint.h>
#include <hypercall-x86_32.h>
#include <xen/xen.h>
#include <xen/io/console.h>

int console_init(start_info_t * start);
int console_write(char * message);
void console_flush(void);
