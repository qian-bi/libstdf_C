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

/*** WIN32 CRUFT ***/
#if defined(WIN32)

typedef unsigned char	__uint8_t;
typedef unsigned short	__uint16_t;
typedef unsigned int	__uint32_t;
typedef signed char		__int8_t;
typedef signed short	__int16_t;
typedef signed int		__int32_t;

/* pos windows compiler doesnt like 'far' as a variable name */
#define	far			_windows_is_a_pos_far

#include <io.h>
#define	open	_open

/*** *NIX OPERATING SYSTEMS ***/
#else

#include <unistd.h>

#if defined(SOLARIS)
typedef	uint8_t			__uint8_t;
typedef	uint16_t		__uint16_t;
typedef	uint32_t		__uint32_t;
typedef	int8_t			__int8_t;
typedef	int16_t			__int16_t;
typedef	int32_t			__int32_t;

#else

#include <endian.h>
#include <byteswap.h>

#endif

#endif

/* stolen from linux headers */
#if !defined(BYTE_ORDER)
#define	LITTLE_ENDIAN	1234
#define	BIG_ENDIAN		4321
#define	PDP_ENDIAN		3412
#define	BYTE_ORDER		LITTLE_ENDIAN
#endif

#if defined(LITTLE_ENDIAN) && !defined(bswap_16)
#define	bswap_16(x)		((((x) >> 8) & 0xff) | (((x) & 0xff) << 8))
#define	bswap_32(x)		((((x) & 0xff000000) >> 24) | (((x) & 0x00ff0000) >>  8) | (((x) & 0x0000ff00) <<  8) | (((x) & 0x000000ff) << 24))
#endif

#endif /* _LIBSTDF_SYSTEMS_H */
