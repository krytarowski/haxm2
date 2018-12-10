#include <sys/param.h>
#include <sys/types.h>
#include <sys/module.h>

#include "../../include/hax.h"

MODULE(MODULE_CLASS_MISC, haxm, NULL);

static int
haxm_modcmd(modcmd_t cmd, void *arg __unused)
{
	struct cpu_info *ci;
	CPU_INFO_ITERATOR cii;

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
		return 0;
	}
	case MODULE_CMD_FINI:
		return 0;
	default:
		return ENOTTY;
	}
}
