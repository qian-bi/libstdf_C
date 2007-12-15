/**
 * @file libstdf_systems.h
 * @brief System specific include files / features.
 */
/*
 * Copyright (C) 2004-2007 Mike Frysinger <vapier@gmail.com>
 * Released under the BSD license.  For more information,
 * please see: http://opensource.org/licenses/bsd-license.php
 */

#ifndef _LIBSTDF_INTERNAL_HEADERS_H
#define _LIBSTDF_INTERNAL_HEADERS_H

#if defined(HAVE_STDINT_H)
# include <stdint.h>
#endif
#if defined(HAVE_INTTYPES_H)
# include <inttypes.h>
#endif
#if defined(HAVE_SYS_TYPES_H)
# include <sys/types.h>
#endif
#if defined(HAVE_SYS_STAT_H)
# include <sys/stat.h>
#endif
#if defined(HAVE_CTYPE_H)
# include <ctype.h>
#endif
#if defined(HAVE_ERRNO_H)
# include <errno.h>
#else
extern int errno;
#endif
#if defined(HAVE_UNISTD_H)
# include <unistd.h>
#endif
#if defined(HAVE_FCNTL_H)
# include <fcntl.h>
#endif
#if defined(HAVE_STDIO_H)
# include <stdio.h>
#endif
#if defined(HAVE_STDLIB_H)
# include <stdlib.h>
#endif
#if defined(HAVE_TIME_H)
# include <time.h>
#endif
#if defined(HAVE_STRING_H)
# include <string.h>
#endif
#if defined(HAVE_FEATURES_H)
# include <features.h>
#endif
#if defined(HAVE_STDARG_H)
# include <stdarg.h>
#endif
#if defined(HAVE_SYS_CDEFS_H)
# include <sys/cdefs.h>
#endif
#if defined(HAVE_ASSERT_H)
# include <assert.h>
#endif

#if defined(HAVE_ENDIAN_H)
# include <endian.h>
#endif
#if defined(HAVE_SYS_ENDIAN_H)
# include <sys/endian.h>
#endif
#if defined(HAVE_MACHINE_ENDIAN_H)
# include <machine/endian.h>
#endif
#if defined(HAVE_SYS_ISA_DEFS_H)
# include <sys/isa_defs.h>
#endif
#if defined(HAVE_SYS_PARAM_H)
# include <sys/param.h>
#endif

#if defined(HAVE_BYTESWAP_H)
# include <byteswap.h>
#endif

#if defined(HAVE_ZIP)
# include <zzip/zzip.h>
#endif
#if defined(HAVE_GZIP)
# include <zlib.h>
#endif
#if defined(HAVE_BZIP2)
# include <bzlib.h>
#endif
#if defined(HAVE_LZW)
# define __IN_LIBLZW
# include <lzw.h>
#endif

#if defined(HAVE_CMPLRS_HOST_H)
# include <cmplrs/host.h>
#endif

/* Setup endian type. */
#define STDF_ENDIAN_BIG    4321
#define STDF_ENDIAN_LITTLE 1234
#if !defined(STDF_ENDIAN_FORCE)
# ifdef WORDS_BIGENDIAN
#  define STDF_ENDIAN_FORCE STDF_ENDIAN_BIG
# else
#  define STDF_ENDIAN_FORCE STDF_ENDIAN_LITTLE
# endif
#endif
#if (STDF_ENDIAN_FORCE == STDF_ENDIAN_BIG)
# define STDF_ENDIAN_HOST STDF_ENDIAN_BIG
#elif (STDF_ENDIAN_FORCE == STDF_ENDIAN_LITTLE)
# define STDF_ENDIAN_HOST STDF_ENDIAN_LITTLE
#else
# error Unable to detect appropriate endian settings for your system.
# error Please send a bug report to the freestdf-devel@lists.sourceforge.net
# error mailing list.  You can work around the problem by re-running
# error the configure script with the --enable-endian option.
#endif

/* i'll see you in hell windows */
#if !defined(O_BINARY)
# define O_BINARY 0x00
#endif

/**
 * @brief Internal macros for fun ELF tricks.
 */
#ifdef STDF_HAVE_GCC_ATTRIBUTE_UNUSED
# define stdf_attribute_unused __attribute__ ((__unused__))
#else
# define stdf_attribute_unused
#endif
#ifdef STDF_HAVE_GCC_ATTRIBUTE_VISIBILITY
# define stdf_attribute_hidden __attribute__ ((__visibility__("hidden")))
#else
# define stdf_attribute_hidden
#endif
#ifdef STDF_HAVE_GCC_ATTRIBUTE_STRONG_ALIAS
# define stdf_attribute_strong_alias(name, aliasname) \
	extern __typeof (name) aliasname __attribute__ ((__alias__ (#name)))
#else
# define stdf_attribute_strong_alias(name, aliasname)
#endif
#ifdef STDF_HAVE_GCC_ATTRIBUTE_WEAK_ALIAS
# define stdf_attribute_weak_alias(name, aliasname) \
	extern __typeof (name) aliasname __attribute__ ((__weak__, __alias__ (#name)))
#else
# define stdf_attribute_weak_alias(name, aliasname)
#endif

#endif /* _LIBSTDF_INTERNAL_HEADERS_H */
