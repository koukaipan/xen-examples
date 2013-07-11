XEN_SRC = ../../src/xen-4.2.1

#ARCH dependent
XEN_TARGET_ARCH = x86_32
ifeq ($(XEN_TARGET_ARCH),x86_32)
ARCH_CFLAGS  := -m32 -march=i686
ARCH_LDFLAGS := -m elf_i386 -T x86_32.lds
ARCH_ASFLAGS := -m32
ARCH_OBJS := bootstrap.x86_32.o
endif

CPPFLAGS += -I$(XEN_SRC)/xen/include/public $(ARCH_CPPFLAGS)
LDFLAGS  += -nostdlib $(ARCH_LDFLAGS)
CFLAGS   += -Wall -Werror -std=c99 $(ARCH_CFLAGS)
ASFLAGS   = -D__ASSEMBLY__ $(ARCH_ASFLAGS)

.PHONY: all clean run

all: testkernel

testkernel: $(ARCH_OBJS) kernel.o
	$(LD) $(LDFLAGS) $^ -o testkernel

clean:
	rm -f *.o 
	rm -f testkernel

run: testkernel
	sudo xm create -c domain_config
