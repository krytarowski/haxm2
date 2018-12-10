#include <sys/param.h>
#include <sys/types.h>
#include <sys/conf.h>
#include <sys/device.h>
#include <sys/module.h>

#include "../../include/hax.h"

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
		return 0;

init_err1:
		return ENXIO;
	}
	case MODULE_CMD_FINI:
		return 0;
	default:
		return ENOTTY;
	}
}
