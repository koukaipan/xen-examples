#include "console.h"
#include <xen/sched.h>
#include <barrier.h>
#include "event.h"

static evtchn_port_t console_evt;
extern char _text;
struct xencons_interface * console;

/* Event received on console event channel */
void handle_input(evtchn_port_t port, struct pt_regs * regs)
{
	XENCONS_RING_IDX cons = console->in_cons;
	XENCONS_RING_IDX prod = console->in_prod;
	int length = prod - cons;
	if(length > 0)
	{
		char buffer[10];
		console_read(buffer, ++length);
		console_write(buffer);
	}
}

/* Initialise the console */
int console_init(start_info_t * start)
{
	console = (struct xencons_interface*)
		((machine_to_phys_mapping[start->console.domU.mfn] << 12)
		 +
		((unsigned long)&_text));
	console_evt = start->console.domU.evtchn;
	/* Set up the event channel */
	register_event(console_evt, handle_input);
	return 0;
}

/* Write a NULL-terminated string */
int console_write(char * message)
{
	struct evtchn_send event;
	event.port = console_evt;
	int length = 0;
	while(*message != '\0')
	{
		/* Wait for the back end to clear enough space in the buffer */
		XENCONS_RING_IDX data;
		do
		{
			data = console->out_prod - console->out_cons;
			HYPERVISOR_event_channel_op(EVTCHNOP_send, &event);
			mb();
		} while (data >= sizeof(console->out));
		/* Copy the byte */
		int ring_index = MASK_XENCONS_IDX(console->out_prod, console->out);
		console->out[ring_index] = *message;
		/* Ensure that the data really is in the ring before continuing */
		wmb();
		/* Increment input and output pointers */
		console->out_prod++;
		length++;
		message++;
	}
	HYPERVISOR_event_channel_op(EVTCHNOP_send, &event);
	return length;
}

/* Read up to length characters from the console into buffer */
int console_read(char * buffer, int n)
{
	int length = 0;
	while(n > length  && console->in_cons < console->in_prod)
	{
		/* Copy the character */
		int ring_index = MASK_XENCONS_IDX(console->in_cons, console->in);
		*buffer = console->in[ring_index];
		buffer++;
		length++;
		console->in_cons++;
		wmb();
	}
	/* NULL-terminate the string */
	*buffer = '\0';
	return length;
}

/* Block while data is in the out buffer */
void console_flush(void)
{
	/* While there is data in the out channel */
	while(console->out_cons < console->out_prod)
	{
		/* Let other processes run */
		HYPERVISOR_sched_op(SCHEDOP_yield, 0);
		mb();
	}
}

char atoc(int n)
{
	switch(n)
	{
		case 0: return '0';		
		case 1: return '1';		
		case 2: return '2';		
		case 3: return '3';		
		case 4: return '4';		
		case 5: return '5';		
		case 6: return '6';		
		case 7: return '7';		
		case 8: return '8';		
		case 9: return '9';		
		default: return 'x';
	}
}

void console_write_int(unsigned int n)
{
	char buffer[11];
	int digits = 0;
	unsigned int digit = 1000000000;
	int mid = 0;
	for(int i=0 ; i<10 ; i++)
	{
		if(n >= digit || mid != 0)
		{
			char digit_value = n / digit;
			buffer[digits] = atoc(digit_value);
			n -= digit_value * digit;
			digits++;
			mid = 1;
		}
		digit /= 10;
	}
	if(digits == 0)
	{
		buffer[digits++] = '0';
	}
	buffer[digits] = '\0';
	console_write(buffer);
}
