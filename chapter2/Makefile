CPPFLAGS += -I../xen/xen/include/public
LDFLAGS  += -nostdlib -T example.lds
CFLAGS   += -std=c99
ASFLAGS   = -D__ASSEMBLY__ 

.PHONY: all

all: testkernel

testkernel: bootstrap.x86_32.o kernel.o
	$(CC) $(LDFLAGS) $^ -o testkernel

clean:
	rm -f *.o 
	rm -f testkernel
