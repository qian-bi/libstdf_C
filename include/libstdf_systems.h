/* libstdf_systems.h
 * Copyright (C) 2004 Mike Frysinger <vapier@gmail.com>
 * Released under the BSD license.  For more information,
 * please see: http://opensource.org/licenses/bsd-license.php
 *
 * $Header$
 */

#ifndef _LIBSTDF_SYSTEMS_H
#define _LIBSTDF_SYSTEMS_H

#include <libstdf_sys_config.h>

#if defined(HAVE_STDINT_H)
# include <stdint.h>
#endif
#if defined(HAVE_SYS_TYPES_H)
# include <sys/types.h>
#endif
#if defined(HAVE_UNISTD_H)
# include <unistd.h>
#endif

#if defined(HAVE_ENDIAN_H)
# include <endian.h>
#endif
#if defined(HAVE_SYS_ENDIAN_H)
# include <sys/endian.h>
#endif

#if defined(HAVE_BYTESWAP_H)
# include <byteswap.h>
#elif defined(HAVE_BSWAP_H)
# include <bswap.h>
#else
#define bswap_16(x) \
	x = ((((x) & 0xff00) >> 8) | \
	     (((x) & 0x00ff) << 8))
#define bswap_32(x) \
	x = ((((x) & 0xff000000) >> 24) | \
	     (((x) & 0x00ff0000) >>  8) | \
	     (((x) & 0x0000ff00) <<  8) | \
	     (((x) & 0x000000ff) << 24))
#define bswap_64(x) \
	x = ((((x) & 0xff00000000000000) >> 56) | \
	     (((x) & 0x00ff000000000000) >> 40) | \
	     (((x) & 0x0000ff0000000000) >> 24) | \
	     (((x) & 0x000000ff00000000) >>  8) | \
	     (((x) & 0x00000000ff000000) <<  8) | \
	     (((x) & 0x0000000000ff0000) << 24) | \
	     (((x) & 0x000000000000ff00) << 40) | \
	     (((x) & 0x00000000000000ff) << 56))
#endif

#endif /* _LIBSTDF_SYSTEMS_H */
