#ifndef HAX_CORE_CONFIG_H_
#define HAX_CORE_CONFIG_H_

#ifdef __NetBSD__
// TODO: Handle 64 VMs
#define HAX_MAX_VMS 8
#else
// Matches the number of bits in vm_mid_bits (see vm.c)
#define HAX_MAX_VMS 64
#endif

#endif // HAX_CORE_CONFIG_H_
