#include <xen/lib.h>
#include <xen/sched.h>
#include <xen/sched-if.h>


/* CPU Run Queue */
static struct vcpu * vcpu_list_head = NULL;
static struct vcpu * vcpu_list_tail = NULL;
unsigned int vcpus = 0;
#define VCPU_NEXT(_vcpu) ((struct vcpu*)_vcpu->sched_priv)

/* Add a VCPU */
int trivial_init_vcpu(struct vcpu * v)
{
	if (vcpu_list_head == NULL)
	{
		vcpu_list_head = vcpu_list_tail = v;
	}
	else
	{
		vcpu_list_tail->sched_priv = vcpu_list_tail = v;
	}
	v->sched_priv = NULL;
	return 0;
}

/* Remove a VCPU */
void trivial_destroy_vcpu(struct vcpu * v)
{
	if(v == vcpu_list_head)
	{
		vcpu_list_head = VCPU_NEXT(v);
	}
	else
	{
		struct vcpu * last = NULL;
		struct vcpu * current = vcpu_list_head;
		while(current != v && current != NULL)
		{
			last = current;
			current = VCPU_NEXT(current);
		}
		if(current != NULL)
		{
			last->sched_priv = VCPU_NEXT(current);
		}
	}
}

/* Move the front VCPU to the back */
static inline void increment_run_queue(void)
{
	vcpu_list_tail->sched_priv = vcpu_list_head;
	vcpu_list_tail = vcpu_list_head;
	vcpu_list_head = VCPU_NEXT(vcpu_list_tail);
	vcpu_list_tail->sched_priv = NULL;
}

/* Pick a VCPU to run */
struct task_slice trivial_do_schedule(s_time_t)
{
	struct task_slice ret;
	/* Fixed-size quantum */
	ret.time = MILLISECS(10);
	struct * vcpu head = vcpu_list_head;
	do
	{
		/* Find a runnable VCPU */
		increment_run_queue();
		if(vcpu_runnable(vcpu_list_head))
		{
			ret.task = vcpu_list_head;
		}
	} while(head != vcpu_list_head);
	/* Return the idle task if there isn't one */
	ret.task = ((struct vcpu*)__get_per_cpu(schedule_data)).idle);
	return ret;
}

struct scheduler sched_trivial_def = {
    .name     = "Trivial Round Robin Scheduler",
    .opt_name = "trivial",
    .sched_id = XEN_SCHEDULER_SEDF,
    
    .init_vcpu      = trivial_init_vcpu,
    .destroy_vcpu   = trivial_destroy_vcpu,

    .do_schedule    = trivial_do_schedule,
};

