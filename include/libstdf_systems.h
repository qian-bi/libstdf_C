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
#if defined(HAVE_INTTYPES_H)
# include <inttypes.h>
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

#if HAVE_ZIP
# include <zzip/zzip.h>
#endif
#if HAVE_GZIP
# include <zlib.h>
#endif
#if HAVE_BZIP2
# include <bzlib.h>
#endif

#endif /* _LIBSTDF_SYSTEMS_H */
