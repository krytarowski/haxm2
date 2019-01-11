#ifndef HAX_H_
#define HAX_H_

#include "hax_types.h"
#include "hax_list.h"

struct vcpu_t;

#define HAX_CUR_VERSION		0x0004
#define HAX_COMPAT_VERSION	0x0001

#define CONFIG_HAX_EPT2

#define for_each_vcpu(vcpu, vm)

#define HAX_MEM_NONPAGE		0x1
#define HAX_MEM_PAGABLE		0x2
#define HAX_MEM_LOW_4G		0x4
#define HAX_MEM_NONBLOCK	0x10

#define MAX_GMEM_G		5

#ifdef __cplusplus
extern "C" {
#endif

void *hax_vmalloc(uint32_t size, uint32_t flags);
void hax_vfree(void *va, uint32_t size);
void hax_vfree_flags(void *va, uint32_t size, uint32_t flags);
void *hax_vmalloc_aligned(uint32_t size, uint32_t flags, uint32_t alignment);
void hax_vfree_aligned(void *va, uint32_t size, uint32_t flags, uint32_t alignment);

#ifdef __cplusplus
}
#endif


extern hax_cpumap_t cpu_online_map;
extern int max_cpus;

/* Utilities */
#define HAX_NOLOG       0xff
#define HAX_LOGE        4
#define HAX_LOGW        3
#define HAX_LOGI        2
#define HAX_LOGD        1
#define HAX_LOG_DEFAULT 3

#ifdef HAX_PLATFORM_NETBSD
#include "netbsd/hax_netbsd.h"
#endif

#endif  // HAX_H_
