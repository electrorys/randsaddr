#ifndef _RANDSADDR_H
#define _RANDSADDR_H

#include <stddef.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <limits.h>
#include <sys/mman.h>
#include <stdint.h>
#include <stdarg.h>
#include <errno.h>
#include <sys/syscall.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>

#include "randsaddr.h"
#include "tfdef.h"
#include "tfe.h"
#include "tfprng.h"

enum { NO, YES };
enum { RAT_NONE, RAT_IPV4, RAT_IPV6 };

typedef _Bool ras_yesno;
typedef short ras_atype;

#define NOSIZE ((size_t)-1)

#define STAT_ARRAY_SZ(x) (sizeof(x)/sizeof(*x))
#define CSTR_SZ(x) (sizeof(x)-1)

#define SADDRLEN INET6_ADDRSTRLEN+4
#define NADDRS 64

union s_addr {
	uint8_t ipa[16];
	struct sockaddr_in6 v6a;
	uint8_t v6b[16];
	struct sockaddr_in v4a;
	uint8_t v4b[4];
};

struct s_addrcfg {
	ras_atype atype;
	char s_addr[SADDRLEN];
	size_t s_pfx;
	union s_addr sa;
	ras_yesno eui64;
	ras_yesno whitelisted;
	ras_yesno dont_bind;
	ras_yesno fullbytes;
	ras_yesno remap;
	char d_addr[SADDRLEN];
	size_t d_pfx;
	union s_addr da;
};

struct s_envcfg {
	char s_cfg[sizeof(struct s_addrcfg)*NADDRS*2];

	ras_yesno initdone;
	ras_yesno disabled;

	ras_yesno do_socket; /* dangerous for servers! */
	ras_yesno do_bind; /* dangerous for servers! */
	ras_yesno do_connect;
	ras_yesno do_send;
	ras_yesno do_sendto;
	ras_yesno do_sendmsg;
	ras_yesno do_reuseaddr;
	ras_yesno do_eui64;
	ras_yesno do_fullbytes;
	ras_yesno do_clear_env;
};

extern const struct s_envcfg *randsaddr_config;

extern ras_yesno ras_mkrandaddr6(void *, const void *, size_t, ras_yesno);
extern void ras_mkeui64addr(void *, const void *);
extern ras_yesno ras_mkrandaddr4(void *, const void *, size_t, ras_yesno);

extern void ras_prng_init(void);
extern uint8_t ras_prng_getrandc(ras_yesno);
extern size_t ras_prng_index(size_t, size_t);

extern ras_atype ras_addr_type(const char *);
extern ras_yesno ras_compare_prefix(ras_atype, const void *, const void *, size_t);

extern size_t ras_strlcpy(char *, const char *, size_t);
extern size_t ras_strxstr(char *, const char *, const char *);

extern void ras_init(void);
extern ras_yesno ras_addr_bindable(int, const union s_addr *);
extern ras_yesno ras_addr_remapped(int, union s_addr *, const union s_addr *);
extern ras_yesno ras_bind_random(int, in_port_t, ras_yesno);

#endif
