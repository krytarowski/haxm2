#include <sys/param.h>
#include <sys/types.h>
#include <sys/kmem.h>
#include <uvm/uvm.h>

#include "../../include/hax.h"

uint64_t
hax_get_memory_threshold(void)
{
	// Since there is no memory cap, just return a sufficently large value
	return 1ULL << 48; // PHYSADDR_MAX + 1
}
