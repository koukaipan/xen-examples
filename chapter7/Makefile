#ARCH dependent
XEN_TARGET_ARCH = x86_32
ifeq ($(XEN_TARGET_ARCH),x86_32)
ARCH_CFLAGS  := -m32 -march=i686
ARCH_LDFLAGS := -m elf_i386 -T loader.lds
ARCH_ASFLAGS := -m32
ARCH_OBJS := bootstrap.x86_32.o
endif

CPPFLAGS += -Iinclude -DCONFIG_X86_PAE -D__XEN_INTERFACE_VERSION__=0x00030203 $(ARCH_CPPFLAGS)
LDFLAGS  += -nostdlib -g $(ARCH_LDFLAGS)
CFLAGS   += -Wall -std=c99 -g $(ARCH_CFLAGS) -fno-stack-protector
ASFLAGS   = -D__ASSEMBLY__ $(ARCH_ASFLAGS)

.PHONY: all clean run

all: testkernel

testkernel: $(ARCH_OBJS) kernel.o console.o event.o traps.o
	$(LD) $(LDFLAGS) $^ -o testkernel

clean:
	rm -f *.o 
	rm -f testkernel

run: testkernel
	sudo xm create -c domain_config
