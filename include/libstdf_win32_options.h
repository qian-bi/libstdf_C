/**
 * @file libstdf_win32_options.h
 * @brief Enable/Disable libstdf features.
 *
 */
/*
 * Copyright (C) 2004 Mike Frysinger <vapier@gmail.com>
 * Released under the BSD license.  For more information,
 * please see: http://opensource.org/licenses/bsd-license.php
 *
 * $Header$
 */

#ifndef _LIBSTDF_WIN32_OPTIONS_H
#define _LIBSTDF_WIN32_OPTIONS_H

#define	HAVE_ZIP	0
#define	HAVE_GZIP	0
#define	HAVE_BZIP	0

/* you shouldn't have to touch anything below here */
#define	HAVE_SYS_TYPES_H	1
#define	HAVE_SYS_STAT_H		1
#define	HAVE_FCNTL_H		1
#define	HAVE_STDIO_H		1
#define	HAVE_STDLIB_H		1
#define	HAVE_STRING_H		1

#endif /* _LIBSTDF_WIN32_OPTIONS_H */
