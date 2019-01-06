#ifndef HAX_CORE_HAX_DRIVER_H_
#define HAX_CORE_HAX_DRIVER_H_

#include "vm.h"

struct hax_t {
	int vmx_enable_flag;
	int nx_enable_flag;
	int em64t_enable_flag;
	int ug_enable_flag;

	uint apm_version;
	uint apm_general_count;
	uint64_t apm_general_mask;
	uint apm_event_count;
	uint32_t apm_event_unavailability;
	uint apm_fixed_count;
	uint64_t apm_fixed_mask;
};

#endif // HAX_CORE_HAX_DRIVER_H_
