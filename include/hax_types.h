#ifndef HAX_TYPES_H_
#define HAX_TYPES_H_

#if defined(__i386__) || defined(_M_IX386)
#define HAX_ACH_X86_32
#define ASMCALL __cdecl
#elif defined(__x86_64__) || defined(_M_X64)
#define HAX_ARCH_X86_64
#define ASMCALL
#else
#error "Unsupported architecture"
#endif

#if defined(__clang__)
#define HAX_COMPILER_CLANG
#define PACKED		__attribute__ ((packed))
#define ALIGNED(x)	__attribute__ ((aligned(x)))
#elif defined(__GNUC__)
#define HAX_COMPILER_GCC
#define PACKED		__attribute__ ((packed))
#define ALIGNED(x)	__attribute__ ((aligned(x)))
#define __cdecl		__attribute__ ((__cdecl__,regparm(0)))
#define __stdcall	__attribute__ ((__stdcall__))
#endif

#ifndef HAX_TESTS
#if defined(__NetBSD__)
#define HAX_PLATFORM_NETBSD
#include "netbsd/hax_types_netbsd.h"
#endif
#else
#include <stdint.h>
#endif

#define HAX_PAGE_SIZE	4096
#define HAX_PAGE_SHIFT	12
#define HAX_PAGE_MASK	0xfff

typedef uint64_t hax_pa_t;
typedef uint64_t hax_pfn_t;
typedef uint64_t hax_paddr_t;
typedef uint64_t hax_vaddr_t;

#endif  // HAX_TYPES_H_
