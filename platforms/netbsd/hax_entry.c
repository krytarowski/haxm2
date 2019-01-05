#include <sys/param.h>
#include <sys/types.h>
#include <sys/conf.h>
#include <sys/device.h>
#include <sys/module.h>

#include "../../core/include/config.h"
#include "../../core/include/hax_core_interface.h"
#include "../../include/hax.h"
#include "../../include/hax_interface.h"

#define HAX_DEVICE_NAME "HAX"
#define HAX_VM_DEVICE_NAME "hax_vm"
#define HAX_VCPU_DEVICE_NAME "hax_vcpu"

static int hax_cmajor = 220, hax_bmajor = -1;
static int hax_vm_cmajor = 222, hax_vm_bmajor = -1;
static int hax_vcpu_cmajor = 221, hax_vcpu_bmajor = -1;

extern struct cdevsw hax_cdevsw;
extern struct cdevsw hax_vm_cdevsw;
extern struct cdevsw hax_vcpu_cdevsw;

static int hax_vm_match(device_t, cfdata_t, void *);
static void hax_vm_attach(device_t, device_t, void *);
static int hax_vm_detach(device_t, int);

CFATTACH_DECL_NEW(hax_vm, sizeof(struct hax_vm_softc),
                  hax_vm_match, hax_vm_attach, hax_vm_detach, NULL);

static int hax_vcpu_match(device_t, cfdata_t, void *);
static void hax_vcpu_attach(device_t, device_t, void *);
static int hax_vcpu_detach(device_t, int);

CFATTACH_DECL_NEW(hax_vcpu, sizeof(struct hax_vcpu_softc),
                  hax_vcpu_match, hax_vcpu_attach, hax_vcpu_detach, NULL);

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
	struct hax_vm_softc *sc;

	sc = device_private(self);
	if (sc == NULL) {
		hax_error("device_private() for hax_vm failed\n");
		return -ENODEV;
	}
	pmf_device_deregister(self);

	return 0;
}

static int
hax_vcpu_match(device_t parent, cfdata_t match, void *aux)
{
	return 1;
}

static void
hax_vcpu_attach(device_t parent, device_t self, void *aux)
{
}

static int
hax_vcpu_detach(device_t self, int flags)
{
}

static const struct cfiattrdata haxbus_iattrdata = {
	"haxbus", 0, { { NULL, NULL, 0 },}
};

static const struct cfiattrdata *const hax_vm_attrs[] = {
	&haxbus_iattrdata, NULL
};

CFDRIVER_DECL(hax_vm, DV_DULL, hax_vm_attrs);
static int hax_vmloc[] = {
	-1,
	-1,
	-1
};

static struct cfdata hax_vm_cfdata[] = {
	{
		.cf_name = "hax_vm",
		.cf_atname = "hax_vm",
		.cf_unit = 0,
		.cf_fstate = FSTATE_STAR,
		.cf_loc = hax_vmloc,
		.cf_flags = 0,
		.cf_pspec = NULL,
	},
	{ NULL, NULL, 0, FSTATE_NOTFOUND, NULL, 0, NULL }
};

static const struct cfiattrdata *const hax_vcpu_attrs[] = {
	&haxbus_iattrdata, NULL
};

CFDRIVER_DECL(hax_vcpu, DV_DULL, hax_vcpu_attrs);
static int hax_vcpuloc[] = {
	-1,
	-1,
	-1
};

static struct cfdata hax_vcpu_cfdata[] = {
	{
		.cf_name = "hax_vcpu",
		.cf_atname = "hax_vcpu",
		.cf_unit = 0,
		.cf_fstate = FSTATE_STAR,
		.cf_loc = hax_vcpuloc,
		.cf_flags = 0,
		.cf_pspec = NULL,
	},
	{ NULL, NULL, 0, FSTATE_NOTFOUND, NULL, 0, NULL }
};

MODULE(MODULE_CLASS_MISC, haxm, NULL);

static int
haxm_modcmd(modcmd_t cmd, void *arg __unused)
{
	struct cpu_info *ci;
	CPU_INFO_ITERATOR cii;
	int err;
	size_t i;

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

		err = config_cfdata_attach(hax_vm_cfdata, 1);
		if (err) {
			hax_error("Unable to register cfdata hax_vm\n");
			goto init_err3;
		}

		// Register hax_vcpu
		err = config_cfdriver_attach(&hax_vcpu_cd);
		if (err) {
			hax_error("Unable to register cfdriver hax_vcpu\n");
			goto init_err4;
		}

		err = config_cfattach_attach(hax_vcpu_cd.cd_name, &hax_vcpu_ca);
		if (err) {
			hax_error("Unable to register cfattch hax_vcpu\n");
			goto init_err5;
		}

		err = config_cfdata_attach(hax_vcpu_cfdata, 1);
		if (err) {
			hax_error("Unable to register cfdata hax_vcpu\n");
			goto init_err6;
		}

		// Register device entries
		err = devsw_attach(HAX_DEVICE_NAME, NULL, &hax_bmajor, &hax_cdevsw,
                       &hax_cmajor);
		if (err) {
			hax_error("Failed to register HAXM device\n");
			goto init_err7;
		}

		err = devsw_attach(HAX_VM_DEVICE_NAME, NULL, &hax_vm_bmajor, &hax_vm_cdevsw,
		       &hax_vm_cmajor);
		if (err) {
			hax_error("Failed to register HAXM VM device\n");
			goto init_err8;
		}

		err = devsw_attach(HAX_VCPU_DEVICE_NAME, NULL, &hax_vcpu_bmajor, &hax_vcpu_cdevsw,
		       &hax_vcpu_cmajor);
		if (err) {
			hax_error("Failed to register HAXM VCPU device\n");
			goto init_err9;
		}

		for (i = 0; i < HAX_MAX_VMS; i++)
			config_attach_pseudo(hax_vm_cfdata);

		for (i = 0; i < (HAX_MAX_VMS * HAX_MAX_VCPUS); i++)
			config_attach_pseudo(hax_vcpu_cfdata);

		// Initialize HAXM
		if (hax_module_init() < 0) {
			hax_error("Failed to initialize HAXM module\n");
			goto init_err10;
		}

		hax_info("Created HAXM device\n");
		return 0;
init_err10:
		devsw_detach(NULL, &hax_vcpu_cdevsw);
init_err9:
		devsw_detach(NULL, &hax_vm_cdevsw);
init_err8:
		devsw_detach(NULL, &hax_cdevsw);
init_err7:
		config_cfdata_detach(hax_vcpu_cfdata);
init_err6:
		config_cfattach_detach(hax_vcpu_cd.cd_name, &hax_vcpu_ca);
init_err5:
		config_cfdriver_detach(&hax_vcpu_cd);
init_err4:
		config_cfdata_detach(hax_vm_cfdata);
init_err3:
		config_cfattach_detach(hax_vm_cd.cd_name, &hax_vm_ca);
init_err2:
		config_cfdriver_detach(&hax_vm_cd);
init_err1:
		return ENXIO;
	}
	case MODULE_CMD_FINI:
		if (hax_module_exit() < 0) {
			hax_error("Failed to finalize HAXM module\n");
			return EBUSY;
		}

		devsw_detach(NULL, &hax_vcpu_cdevsw);
		devsw_detach(NULL, &hax_vm_cdevsw);
		devsw_detach(NULL, &hax_cdevsw);

		config_cfdata_detach(hax_vcpu_cfdata);
		config_cfattach_detach(hax_vcpu_cd.cd_name, &hax_vcpu_ca);
		config_cfdriver_detach(&hax_vcpu_cd);

		config_cfdata_detach(hax_vm_cfdata);
		config_cfattach_detach(hax_vm_cd.cd_name, &hax_vm_ca);
		config_cfdriver_detach(&hax_vm_cd);

		return 0;
	default:
		return ENOTTY;
	}
}
