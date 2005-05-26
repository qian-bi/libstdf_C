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

#define warn(fmt, args...)  fprintf(stderr, "libstdf: " fmt "\n", ## args)
#define warnf(fmt, args...) warn("%s(): " fmt, __FUNCTION__, ## args)

#endif /* _LIBSTDF_INTERNAL_H */
