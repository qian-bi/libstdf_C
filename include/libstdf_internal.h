/**
 * @file libstdf_internal.h
 * @brief Common internal functions
 * @internal
 */
/*
 * Copyright (C) 2005 Mike Frysinger <vapier@gmail.com>
 * Released under the BSD license.  For more information,
 * please see: http://opensource.org/licenses/bsd-license.php
 *
 * $Header$
 */

#ifndef _LIBSTDF_INTERNAL_H
#define _LIBSTDF_INTERNAL_H

#define _warn(fmt, args...)  fprintf(stderr, fmt , ## args)
#define warn(fmt, args...)   _warn("libstdf: " fmt "\n" , ## args)
#define warnf(fmt, args...)  warn("%s(): " fmt, __FUNCTION__ , ## args)
#define warnp(fmt, args...)  warn(fmt ": %s" , ## args , strerror(errno))
#define warnfp(fmt, args...) warnp("%s(): " fmt, __FUNCTION__ , ## args)

#define dumphex(buf, cnt) \
	do { \
		size_t i = 0; \
		_warn("dumphex: "); \
		while (i < cnt) \
			_warn("%2.2X ", buf[i++]); \
		_warn("\n"); \
	} while (0)

#if HAVE_LZW
# define __IN_LIBLZW
# include <lzw.h>
#endif

#endif /* _LIBSTDF_INTERNAL_H */
