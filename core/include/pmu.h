#ifndef HAX_CORE_PMU_H_
#define HAX_CORE_PMU_H_

#include "types.h"

/*
 * Information about a logical processor's performance monitoring units (PMU),
 * particularly its support for architectural performance monitoring (APM). See
 * IA SDM Vol. 2A Table 3-8 ("Information Returned by CPUID Instruction"),
 * Initial EAX Value = 0AH ("Architectural Performance Monitoring Leaf").
 */

/*	Architectural Performance Monitoring Leaf

Initial EAX Value: 0AH

EAX	Bits 07 - 00: Version ID of architectural performance monitoring.
	Bits 15 - 08: Number of general-purpose performance monitoring counter per logical processor.
	Bits 23 - 16: Bit width of general-purpose, performance monitoring counter.
	Bits 31 - 24: Length of EBX bit vector to enumerate architectural performance monitoring events.
EBX	Bit 00: Core cycle event not available if 1.
	Bit 01: Instruction retired event not available if 1.
	Bit 02: Reference cycles event not available if 1.
	Bit 03: Last-level cache reference event not available if 1.
	Bit 04: Last-level cache misses event not available if 1.
	Bit 05: Branch instruction retired event not available if 1.
	Bit 06: Branch mispredict retired event not available if 1.
	Bits 31 - 07: Reserved = 0.
ECX	Reserved = 0.
EDX	Bits 04 - 00: Number of fixed-function performance counters (if Version ID > 1).
	Bits 12 - 05: Bit width of fixed-function performance counters (if Version ID > 1).
	Reserved = 0.
*/

struct cpu_pmu_info {
	union {
		uint32_t cpuid_eax;
		struct {
			// Version ID of architectural performance monitoring (APM)
			uint32_t apm_version		: 8;
			// Number of general-purpose performance monitoring counters
			uint32_t apm_general_count	: 8;
			// Bit width of general-purpose performance monitoring counters
			uint32_t apm_general_bitlen	: 8;
			// Length of EBX bit vector
			uint32_t apm_event_count	: 8;
		};
	};
	union {
		uint32_t cpuid_ebx;
		// Bit vector to enumerate APM events
		uint32_t apm_event_unavailability;
	};
	union {
		uint32_t cpuid_edx;
		struct {
			// Number of fixed-function performance monitoring counters
			uint32_t apm_fixed_count	: 5;
			// Bit width of fixed-function performance monitoring counters
			uint32_t apm_fixed_bitlen	: 8;
			// Reserved
			uint32_t			: 19;
		};
	};
} PACKED;

// Maximum number of general-purpose performance monitoring counters per
// processor: IA SDM Val. 3C 35.1 defines IA32_PERFEVTSELx, x = 0..3
#define APM_MAX_GENERAL_COUNT	4
// Maximum number of APM events: IA SDM Vol. 2 Table 3-8 lists 7 APM events
#define APM_MAX_EVENT_COUNT	7
// Maximum number of fixed-function performance monitoring counters per
// processor: IA SDM Vol. 3C 35.1 defines IA32_FIXED_CTRx, x = 0..2
#define APM_MAX_FIXED_COUNT	3

#endif // HAX_CORE_PMU_H_
