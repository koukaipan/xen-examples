/*
 * gettimeofday() example implementation.
 * Requires the shared info page to be mapped and stored in a global variable
 *
 */


#include <time.h>
#include <xen/xen.h> 

extern shared_info_t * shared_info;


#define NANOSECONDS(tsc) (tsc << shared_info->cpu_info[0].time.tsc_shift)\
   	* shared_info->cpu_info[0].time.tsc_to_system_mul

#define RDTSC(x)     asm volatile ("RDTSC":"=A"(tsc))

int gettimeofday(struct timeval *tp, struct timezone *tzp)
{
	uint64_t tsc;
	/* Get the time values from the shared info page */
	uint32_t version, wc_version;
	uint32_t seconds, nanoseconds, system_time;
	uint64_t old_tsc;
	/* Loop until we can read all required values from the same update */
	do
	{
		/* Spin if the time value is being updated */
		do
		{
			wc_version = shared_info->wc_version;
			version = shared_info->cpu_info[0].time.version;
		} while(
				version & 1 == 1
				||
				wc_version & 1 == 1);
		/* Read the values */
		seconds = shared_info->wc_sec;
		nanoseconds = shared_info->wc_nsec;
		system_time = shared_info->cpu_info[0].time.system_time;
		old_tsc = shared_info->cpu_info[0].time.tsc_timestamp;
	} while(
			version != shared_info->cpu_info[0].time.version
			||
			wc_version != shared_info->wc_version
			);
	/* Get the current TSC value */
	RDTSC(tsc);
	/* Get the number of elapsed cycles */
	tsc -= old_tsc;
	/* Update the system time */
	system_time += NANOSECONDS(tsc);
	/* Update the nanosecond time */
	nanoseconds += system_time;
	/* Move complete seconds to the second counter */
	seconds += nanoseconds / 1000000000;
	nanoseconds = nanoseconds % 1000000000;
	/* Return second and millisecond values */
	tp->tv_sec = seconds;
	tp->tv_usec = nanoseconds * 1000;
	return 0;
}
