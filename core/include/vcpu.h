#ifndef HAX_CORE_VCPU_H_
#define HAX_CORE_VCPU_H_

struct vcpu_t {
	uint16_t vcpu_id;
	uint16_t cpu_id;
	// Sometimes current thread might be migrated to other core.
	uint16_t prev_cpu_id;
	/*
	 * VPID: Virtual Processor Identifier
	 * VPIDs provide a way for software to identify to the processor
	 * the address spaces for different "virtual processors"
	 * The processor may use this identification to maintain concurrently
	 * information for multiple address spaces in its TLBs and paging-structure
	 * caches, even when non-zero PCIDs are not being used.
	 * Reference: SDM, Volume 3, Chapter 4.11.2 & Chapter 28.1.
	 */
	uint16_t vpid;
	uint32_t launched;
	/*
	 * This one should co-exist with the is_running and paused,
	 * but conisidering this needs atomic, don't trouble to clean it now
	 */
#define VCPU_STATE_FLAGS_OPENED	0x1
	uint64_t flags;
};

#endif // HAX_CORE_VCPU_H_
