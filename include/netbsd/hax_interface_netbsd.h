#ifndef HAX_NETBSD_HAX_INTERFACE_NETBSD_H_
#define HAX_NETBSD_HAX_INTERFACE_NETBSD_H_

#ifdef _KERNEL
struct hax_vm_softc {
	device_t sc_dev;
	struct hax_vm_netbsd_t *vm;
};

struct hax_vcpu_softc {
	device_t sc_dev;
	struct hax_vcpu_netbsd_t *vcpu;
};

#define unit2vmmid(u)  (__SHIFTOUT(u, __BITS(4, 6)))
#define unit2vcpuid(u) (__SHIFTOUT(u, __BITS(0, 3)))
#endif

#endif  // HAX_NETBSD_HAX_INTERFACE_NETBSD_H_
