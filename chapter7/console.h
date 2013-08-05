#include <stdint.h>
#include <xen/xen.h>
#include <xen/io/console.h>

#if defined(__i386__)
#include <hypercall-x86_32.h>
#elif defined(__x86_64__)
#include <hypercall-x86_64.h>
#else
#error "Unsupported architecture"
#endif

int console_init(start_info_t * start);
int console_write(char * message);
void console_flush(void);
