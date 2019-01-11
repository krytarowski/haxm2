#include <sys/param.h>
#include <sys/types.h>
#include <sys/kmem.h>
#include <uvm/uvm.h>

#include "../../include/hax.h"

void *
hax_vmalloc(uint32_t size, uint32_t flags)
{
	vaddr_t kva;
	uvm_flag_t flag;

	if (size == 0)
		return NULL;

	flag = UVM_KMF_WIRED | UVM_KMF_ZERO;

	flag |= UVM_KMF_WAITVA;

	kva = uvm_km_alloc(kernel_map, size, PAGE_SIZE, flag);

	return (void *)kva;
}

void
hax_vfree_flags(void *va, uint32_t size, uint32_t flags)
{
	uvm_flag_t flag;

	flag = UVM_KMF_WIRED;

	uvm_km_free(kernel_map, (vaddr_t)va, size, flag);
}

void
hax_vfree(void *va, uint32_t size)
{
	uint32_t flags = HAX_MEM_NONPAGE;

	hax_vfree_flags(va, size, flags);
}

void
hax_vfree_aligned(void *va, uint32_t size, uint32_t alignment, uint32_t flags)
{
	hax_vfree_flags(va, size, flags);
}
