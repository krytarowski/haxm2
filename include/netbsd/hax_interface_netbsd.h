#ifndef HAX_NETBSD_HAX_INTERFACE_NETBSD_H_
#define HAX_NETBSD_HAX_INTERFACE_NETBSD_H_

#ifdef _KERNEL
struct hax_vm_softc {
    device_t sc_dev;
    struct hax_vm_netbsd_t *vm;
};
#endif

#endif  // HAX_NETBSD_HAX_INTERFACE_NETBSD_H_
