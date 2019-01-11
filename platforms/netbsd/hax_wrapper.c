#include <sys/param.h>
#include <sys/types.h>
#include <sys/kmem.h>
#include <sys/mutex.h>
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

/* Mutex */
hax_mutex
hax_mutex_alloc_init(void)
{
	kmutex_t *lock;

	lock = kmem_alloc(sizeof(kmutex_t), KM_SLEEP);
	if (!lock) {
		hax_error("Could not allocate mutex\n");
		return NULL;
	}

	mutex_init(lock, MUTEX_DEFAULT, IPL_NONE);

	return lock;
}

void
hax_mutex_lock(hax_mutex lock)
{
	if (!lock)
		return;

	mutex_enter(lock);
}

void
hax_mutex_unlock(hax_mutex lock)
{
	if (!lock)
		return;

	mutex_exit(lock);
}

void
hax_mutex_free(hax_mutex lock)
{
	if (!lock)
		return;

	mutex_destroy(lock);
	kmem_free(lock, sizeof(kmutex_t));
}
