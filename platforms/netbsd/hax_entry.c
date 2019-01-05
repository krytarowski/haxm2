#include <sys/param.h>
#include <sys/types.h>
#include <sys/conf.h>
#include <sys/device.h>
#include <sys/module.h>

#include "../../include/hax.h"
#include "../../include/hax_interface.h"

static int hax_vm_match(device_t, cfdata_t, void *);
static void hax_vm_attach(device_t, device_t, void *);
static int hax_vm_detach(device_t, int);

CFATTACH_DECL_NEW(hax_vm, sizeof(struct hax_vm_softc),
                  hax_vm_match, hax_vm_attach, hax_vm_detach, NULL);

static int
hax_vm_match(device_t parent, cfdata_t match, void *aux)
{
    return 1;
}

static void
hax_vm_attach(device_t parent, device_t self, void *aux)
{
    struct hax_vm_softc *sc;
    int unit;

    sc = device_private(self);
    if (sc == NULL) {
        hax_error("device_private() for hax_vm failed\n");
        return;
    }

    unit = device_unit(self);

    sc->sc_dev = self;
    sc->vm = NULL;

    snprintf(self->dv_xname, sizeof self->dv_xname, "hax_vm/vm%02d", unit);

    if (!pmf_device_register(self, NULL, NULL))
        aprint_error_dev(self, "couldn't establish power handler\n");
}

static int
hax_vm_detach(device_t self, int flags)
{
}

static const struct cfiattrdata haxbus_iattrdata = {
    "haxbus", 0, { { NULL, NULL, 0 },}
};

static const struct cfiattrdata *const hax_vm_attrs[] = {
    &haxbus_iattrdata, NULL
};

CFDRIVER_DECL(hax_vm, DV_DULL, hax_vm_attrs);

MODULE(MODULE_CLASS_MISC, haxm, NULL);

static int
haxm_modcmd(modcmd_t cmd, void *arg __unused)
{
	struct cpu_info *ci;
	CPU_INFO_ITERATOR cii;
	int err;

	switch (cmd) {
	case MODULE_CMD_INIT: {
		// Initialization
		max_cpus = 0;

		ci = NULL;

		for (CPU_INFO_FOREACH(cii, ci)) {
			++max_cpus;
			if (!ISSET(ci->ci_schedstate.spc_flags, SPCF_OFFLINE)) {
				cpu_online_map |= __BIT(ci->ci_cpuid);
			}
		}

		// Register hax_vm
		err = config_cfdriver_attach(&hax_vm_cd);
		if (err) {
			hax_error("Unable to register cfdriver hax_vm\n");
			goto init_err1;
		}

		err = config_cfattach_attach(hax_vm_cd.cd_name, &hax_vm_ca);
		if (err) {
			hax_error("Unable to register cfattach hax_vm\n");
			goto init_err2;
		}

		return 0;
init_err2:
		config_cfdriver_detach(&hax_vm_cd);
init_err1:
		return ENXIO;
	}
	case MODULE_CMD_FINI:
		return 0;
	default:
		return ENOTTY;
	}
}
