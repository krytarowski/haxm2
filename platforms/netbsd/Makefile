S?=     /usr/src/sys

KMOD=   haxm

SRCS+=	hax_entry.c
SRCS+=	hax_wrapper.c
SRCS+=	hax_mem_alloc.c
SRCS+=	hax_mm.c

.PATH: ../../core
SRCS+=	hax.c

.include <bsd.kmodule.mk>
