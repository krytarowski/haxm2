#ifndef HAX_CORE_HAX_DRIVER_H_
#define HAX_CORE_HAX_DRIVER_H_

#include "vm.h"
#include "pmu.h"

struct hax_t {
	int vmx_enable_flag;
	int nx_enable_flag;
	int em64t_enable_flag;
	int ug_enable_flag;

	/*
	 * Common architectural performance monitoring (APM) parameters
	 * (version ID, etc.) supported by all logical processors of
	 * the host CPU.
	 */
	uint apm_version;
	uint apm_general_count;
	uint64_t apm_general_mask;
	uint apm_event_count;
	uint32_t apm_event_unavailability;
	uint apm_fixed_count;
	uint64_t apm_fixed_mask;

	// Unparsed CPUID leaf 0xa output for CPUID virtualization
	struct cpu_pmu_info apm_cpuid_0xa;
};

#endif // HAX_CORE_HAX_DRIVER_H_
