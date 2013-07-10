#include <arch-x86_32.h>


.section __xen_guest
	.ascii	"GUEST_OS=Hacking_Xen_Example"
	.ascii	",XEN_VER=xen-3.0"
	.ascii	",VIRT_BASE=0x0"
	.ascii	",ELF_PADDR_OFFSET=0x0"
	.ascii	",HYPERCALL_PAGE=0x2"
	.ascii	",PAE=yes"
	.ascii	",LOADER=generic"
	.byte	0
.text

.globl _start, shared_info, hypercall_page
                        
_start:
        cld
        lss stack_start,%esp
        push %esi 
        call start_kernel

stack_start:
	.long stack+8192, FLAT_KERNEL_SS

        /* Unpleasant -- the PTE that maps this page is actually overwritten */
        /* to map the real shared-info page! :-)                             */
        .org 0x1000
shared_info:
        .org 0x2000

hypercall_page:
        .org 0x3000

ES		= 0x20
ORIG_EAX	= 0x24
EIP		= 0x28
CS		= 0x2C
