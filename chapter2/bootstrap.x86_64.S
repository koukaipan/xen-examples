#include <arch-x86_64.h>

.section __xen_guest
	.ascii	"GUEST_OS=Mini-OS"
	.ascii	",XEN_VER=xen-3.0"
	.ascii	",VIRT_BASE=0x0" /* &_text from minios_x86_64.lds */
	.ascii	",ELF_PADDR_OFFSET=0x0"
	.ascii	",HYPERCALL_PAGE=0x2"
	.ascii	",LOADER=generic"
	.byte	0
.text

#define ENTRY(X) .globl X ; X :
.globl _start, shared_info, hypercall_page


_start:
        cld
        movq stack_start(%rip),%rsp
        andq $(~(8192-1)), %rsp
        movq %rsi,%rdi
        call start_kernel

stack_start:
        .quad stack+(2*8192)

        /* Unpleasant -- the PTE that maps this page is actually overwritten */
        /* to map the real shared-info page! :-)                             */
        .org 0x1000
shared_info:
        .org 0x2000

hypercall_page:
        .org 0x3000
