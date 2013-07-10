#include <public/xen.h>

extern void * shared_page;
extern grant_entry_t * grant_table;

void offer_page()
{
	uint16_t flags;
	/* Create the grant table */
	gnttab_setup_table_t setup_op;
	
	setup_op.dom = DOMID_SELF;
	setup_op.nr_frames = 1;
	setup_op.frame_list = grant_table;

	HYPERVISOR_grant_table_op(GNTTABOP_setup_table, &setup_op, 1);

	/* Offer the grant */
	grant_table[0].domid = DOMID_FRIEND;
	grant_table[0].frame = shared_page >> 12;
	flags = GTF_permit_access & GTF_reading & GTF_writing;
	grant_table[0].flags = flags;
}
