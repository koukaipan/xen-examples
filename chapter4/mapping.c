#include <public/xen.h>

grant_handle_t map(domid_t friend, 
		unsigned int entry, 
		void * shared_page, 
		grant_handle_t * handle)
{
	/* Set up the mapping operation */
	gnttab_map_grant_ref_t map_op;
	map_op.host_addr = shared_page;
	map_op.flags = GNTMAP_host_map;
	map_op.ref = entry;
	map_op.dom = friend;
	/* Perform the map */
	HYPERVISOR_grant_table_op(GNTTABOP_map_grant_ref, &op,1);
	/* Check if it worked */
	if(map_op.status != GNTST_okay)
	{
		return -1;
	}
	else 
	{
		/* Return the handle */
		*handle = map_op.handle;
		return 0;
	}
}
