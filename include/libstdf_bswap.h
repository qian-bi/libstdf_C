/**
 * @file libstdf_bswap.h
 * @brief Byte swapping functions.
 *
 * Copyright (C) 2004 Mike Frysinger <vapier@gmail.com>
 * Released under the BSD license.  For more information,
 * please see: http://opensource.org/licenses/bsd-license.php
 *
 * $Header$
 */

#ifndef _LIBSTDF_BSWAP_H
#define _LIBSTDF_BSWAP_H

#define stdf_bswap_16(x) \
	x = ((((x) & 0xff00) >> 8) | \
	     (((x) & 0x00ff) << 8))
#define stdf_bswap_32(x) \
	x = ((((x) & 0xff000000) >> 24) | \
	     (((x) & 0x00ff0000) >>  8) | \
	     (((x) & 0x0000ff00) <<  8) | \
	     (((x) & 0x000000ff) << 24))

/* gcc sometimes stupidly types these wrong */
#if defined(__GNUC__)
#define stdf_bswap_64(x) \
	x = ((((x) & 0xff00000000000000ull) >> 56) | \
	     (((x) & 0x00ff000000000000ull) >> 40) | \
	     (((x) & 0x0000ff0000000000ull) >> 24) | \
	     (((x) & 0x000000ff00000000ull) >>  8) | \
	     (((x) & 0x00000000ff000000ull) <<  8) | \
	     (((x) & 0x0000000000ff0000ull) << 24) | \
	     (((x) & 0x000000000000ff00ull) << 40) | \
	     (((x) & 0x00000000000000ffull) << 56))
#else
#define stdf_bswap_64(x) \
	x = ((((x) & 0xff00000000000000) >> 56) | \
	     (((x) & 0x00ff000000000000) >> 40) | \
	     (((x) & 0x0000ff0000000000) >> 24) | \
	     (((x) & 0x000000ff00000000) >>  8) | \
	     (((x) & 0x00000000ff000000) <<  8) | \
	     (((x) & 0x0000000000ff0000) << 24) | \
	     (((x) & 0x000000000000ff00) << 40) | \
	     (((x) & 0x00000000000000ff) << 56))
#endif

#endif /* _LIBSTDF_BSWAP_H */
