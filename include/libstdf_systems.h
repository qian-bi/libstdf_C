/**
 * @file libstdf_systems.h
 * @brief System specific include files / features.
 *
 */
/*
 * Copyright (C) 2004 Mike Frysinger <vapier@gmail.com>
 * Released under the BSD license.  For more information,
 * please see: http://opensource.org/licenses/bsd-license.php
 *
 * $Header$
 */

#ifndef _LIBSTDF_SYSTEMS_H
#define _LIBSTDF_SYSTEMS_H

#ifdef WIN32
# include <libstdf_win32.h>
#else
# include <libstdf_sys_config.h>
#endif

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
#if defined(HAVE_STRING_H)
# include <string.h>
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
#if !HAVE_ZIP && !HAVE_GZIP && !HAVE_BZIP2
# define HAVE_NO_COMPRESSION	1
#endif

#endif /* _LIBSTDF_SYSTEMS_H */
