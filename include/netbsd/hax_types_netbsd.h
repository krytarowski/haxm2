#ifndef HAX_NETBSD_HAX_TYPES_NETBSD_H_
#define HAX_NETBSD_HAX_TYPES_NETBSD_H_

#include <sys/param.h>
#include <sys/types.h>

// Signed Types
typedef int8_t  int8;
typedef int16_t int16;
typedef int32_t int32;
typedef int64_t int64;

// Unsigned Types
typedef uint8_t  uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;
typedef uint64_t uint64;

typedef unsigned int  uint;
typedef unsigned long ulong;
typedef unsigned long ulong_t;

#if defined(__i386__)
typedef uint32_t mword;
#endif
#if defined(__x86_64__)
typedef uint64_t mword;
#endif
typedef mword HAX_VADDR_T;

#include "../hax_list.h"
struct hax_page {
	void *kva;
	struct vm_page *page;
	struct pglist *pglist;
	uint64_t pa;
	uint32_t order;
	uint32_t flags;
	struct hax_link_list list;
	size_t size;
};

typedef int hax_cpumap_t;

typedef kmutex_t *hax_mutex;

#endif  // HAX_NETBSD_HAX_TYPES_NETBSD_H_
