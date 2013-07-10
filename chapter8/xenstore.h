#include <stdint.h>
#include <hypercall-x86_32.h>
#include <xen/xen.h>
#include <errno.h>
#include <xen/io/xs_wire.h>

int xenstore_init(start_info_t * start);
int xenstore_write(char * key, char * value);
int xenstore_read(char * key, char * value, int value_length);
