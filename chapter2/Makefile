XEN_SRC = ../../src/xen-4.2.1
CPPFLAGS += -I$(XEN_SRC)/xen/include/public
LDFLAGS  += -m elf_i386 -nostdlib -T example.lds
CFLAGS   += -m32 -std=c99
ASFLAGS   = -m32 -D__ASSEMBLY__

.PHONY: all clean run

all: testkernel

testkernel: bootstrap.x86_32.o kernel.o
	$(LD) $(LDFLAGS) $^ -o testkernel

clean:
	rm -f *.o 
	rm -f testkernel

run: testkernel
	sudo xm create -c domain_config
