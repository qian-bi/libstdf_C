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

typedef unsigned __int8		uint8_t;
typedef unsigned __int16	uint16_t;
typedef unsigned __int32	uint32_t;
typedef unsigned __int64	uint64_t;
typedef signed __int8		int8_t;
typedef signed __int16		int16_t;
typedef signed __int32		int32_t;
typedef signed __int64		int64_t;

/* pos windows compiler doesnt like 'far' as a variable name */
#define	far			_windows_is_a_pos_far

#include <io.h>
#define	open	_open

#if !defined(BYTE_ORDER)
#define	LITTLE_ENDIAN	1234
#define	BIG_ENDIAN		4321
#define	BYTE_ORDER		LITTLE_ENDIAN
#endif

#if !defined(bswap_16)
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
