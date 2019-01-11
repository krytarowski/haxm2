#include "include/hax_driver.h"
#include "../include/hax.h"

struct hax_t *hax;

int hax_module_init(void)
{
	int ret = 0, cpu = 0;

	hax = (struct hax_t *)hax_vmalloc(sizeof(struct hax_t), HAX_MEM_NONPAGE);
	if (!hax)
		return -ENOMEM;

	hax->mem_quota = hax->mem_limit = hax_get_memory_threshold();
}

int hax_module_exit(void)
{
}
