#ifndef HAX_NETBSD_HAX_NETBSD_H_
#define HAX_NETBSD_HAX_NETBSD_H_

hax_mutex hax_mutex_alloc_init(void);
void hax_mutex_lock(hax_mutex lock);
void hax_mutex_unlock(hax_mutex lock);
void hax_mutex_free(hax_mutex lock);

extern int default_hax_log_level;

void hax_error(char *fmt, ...);
void hax_info(char *fmt, ...);

#endif  // HAX_NETBSD_HAX_NETBSD_H_
