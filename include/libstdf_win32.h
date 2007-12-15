/**
 * @file libstdf_win32.h
 * @brief Win32 specific definitions.
 */
/*
 * Copyright (C) 2004-2007 Mike Frysinger <vapier@gmail.com>
 * Released under the BSD license.  For more information,
 * please see: http://opensource.org/licenses/bsd-license.php
 */

#ifndef _LIBSTDF_WIN32_H
#define _LIBSTDF_WIN32_H

#include <sys/types.h>

typedef unsigned __int8    uint8_t;
typedef unsigned __int16   uint16_t;
typedef unsigned __int32   uint32_t;
typedef unsigned __int64   uint64_t;
typedef signed __int8      int8_t;
typedef signed __int16     int16_t;
typedef signed __int32     int32_t;
typedef signed __int64     int64_t;

/* pos windows compiler doesnt like 'far' as a variable name */
#define far _windows_is_a_pos_far

#ifdef __IN_LIBSTDF
# include <internal/win32.h>
#endif

#endif /* _LIBSTDF_WIN32_H */
