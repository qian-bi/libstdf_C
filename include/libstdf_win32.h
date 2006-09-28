/**
 * @file libstdf_win32.h
 * @brief Win32 specific definitions.
 */
/*
 * Copyright (C) 2004-2006 Mike Frysinger <vapier@gmail.com>
 * Released under the BSD license.  For more information,
 * please see: http://opensource.org/licenses/bsd-license.php
 */

#ifndef _LIBSTDF_WIN32_H
#define _LIBSTDF_WIN32_H

#include <libstdf_win32_options.h>

#include <sys/types.h>

#if !defined(__MINGW_H)
typedef unsigned __int8    uint8_t;
typedef unsigned __int16   uint16_t;
typedef unsigned __int32   uint32_t;
typedef unsigned __int64   uint64_t;
typedef signed __int8      int8_t;
typedef signed __int16     int16_t;
typedef signed __int32     int32_t;
typedef signed __int64     int64_t;
#endif

#ifndef PACKAGE_STRING
# ifndef __DATE__
#  define __DATE__
# endif
# define PACKAGE_STRING "libstdf win32 " __DATE__
#endif

/* pos windows compiler doesnt like 'far' as a variable name */
#define far _windows_is_a_pos_far

#include <io.h>
#define open _open

#if !defined(BYTE_ORDER)
# define LITTLE_ENDIAN     1234
# define BIG_ENDIAN        4321
# define BYTE_ORDER        LITTLE_ENDIAN
#endif

#endif /* _LIBSTDF_WIN32_H */
