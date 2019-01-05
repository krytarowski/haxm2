#include <sys/param.h>
#include <sys/types.h>
#include <sys/systm.h>


#include "../../include/hax.h"

int default_hax_log_level = HAX_LOG_DEFAULT;
int max_cpus;

void hax_error(char *fmt, ...)
{
	va_list args;

	if (HAX_LOGE < default_hax_log_level)
		return;

	va_start(args, fmt);
	printf("haxm_error: ");
	vprintf(fmt, args);
	va_end(args);
}

void hax_info(char *fmt, ...)
{
	va_list args;

	if (HAX_LOGI < default_hax_log_level)
		return;

	va_start(args, fmt);
	printf("haxm_info: ");
	vprintf(fmt, args);
	va_end(args);
}
