/**
 * @file win32.h
 * @brief Win32 specific internal definitions.
 * @internal
 */
/*
 * Copyright (C) 2004-2007 Mike Frysinger <vapier@gmail.com>
 * Released under the BSD license.  For more information,
 * please see: http://opensource.org/licenses/bsd-license.php
 */

#ifndef _LIBSTDF_INTERNAL_WIN32_H
#define _LIBSTDF_INTERNAL_WIN32_H

#include <win32_options.h>

#ifndef PACKAGE_STRING
# ifndef __DATE__
#  define __DATE__
# endif
# define PACKAGE_STRING "libstdf win32 " __DATE__
#endif

#include <io.h>
#define open _open

#define STDF_ENDIAN_FORCE STDF_ENDIAN_LITTLE

#endif /* _LIBSTDF_INTERNAL_WIN32_H */
