/* libstdf_systems.h
 * Copyright (C) 2004 Mike Frysinger <vapier@gmail.com>
 * Released under the BSD license.  For more information,
 * please see: http://opensource.org/licenses/bsd-license.php
 *
 * $Header$
 */

#ifndef _LIBSTDF_SYSTEMS_H
#define _LIBSTDF_SYSTEMS_H

#include <sys/types.h>

typedef unsigned char	uint8_t;
typedef unsigned short	uint16_t;
typedef unsigned int	uint32_t;
typedef unsigned long	uint64_t;
typedef signed char		int8_t;
typedef signed short	int16_t;
typedef signed int		int32_t;
typedef unsigned long	int64_t;

/* pos windows compiler doesnt like 'far' as a variable name */
#define	far			_windows_is_a_pos_far

#include <io.h>
#define	open	_open

#if !defined(BYTE_ORDER)
#define	LITTLE_ENDIAN	1234
#define	BIG_ENDIAN		4321
#define	PDP_ENDIAN		3412
#define	BYTE_ORDER		LITTLE_ENDIAN
#endif

#if !defined(bswap_16)
#define	bswap_16(x)		((((x) >> 8) & 0xff) | (((x) & 0xff) << 8))
#define	bswap_32(x)		((((x) & 0xff000000) >> 24) | (((x) & 0x00ff0000) >>  8) | (((x) & 0x0000ff00) <<  8) | (((x) & 0x000000ff) << 24))
#define	bswap_64(x)		(x)
#endif

#endif /* _LIBSTDF_SYSTEMS_H */
