#ifndef HAX_H_
#define HAX_H_

#include "hax_types.h"

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
