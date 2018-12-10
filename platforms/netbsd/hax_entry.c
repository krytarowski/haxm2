#include <sys/param.h>
#include <sys/types.h>
#include <sys/module.h>

#include "../../include/hax.h"

MODULE(MODULE_CLASS_MISC, haxm, NULL);

static int
haxm_modcmd(modcmd_t cmd, void *arg __unused)
{
	switch (cmd) {
	case MODULE_CMD_INIT: {
		// Initialization
		max_cpus = 0;
		return 0;
	}
	case MODULE_CMD_FINI:
		return 0;
	default:
		return ENOTTY;
	}
}
