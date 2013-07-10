
#define WORD_BOUNDARY (sizeof(long) - 1)

inline void zero(char * addr, unsigned int length)
{
	/* Fill to word boundary */
	while(length > 0 && (((long)addr) & WORD_BOUNDARY))
	{
		*addr = 0;
		length--;
	}
	long * addrl = (long*)addr;
	/* Zero a word at a time */
	while(length > sizeof(long))
	{
		*addrl = 0;
		addrl++;
		length -= 4;
	}
	addr = (char*)addrl;
	/* Zero anything else */
	while(length > 0)
	{
		*addr = 0;
		length--;
	}
}
