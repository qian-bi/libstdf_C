/* libstdf.h
 * Copyright (C) 2004 Mike Frysinger <vapier@gmail.com>
 * Released under the BSD license.  For more information,
 * please see: http://opensource.org/licenses/bsd-license.php
 */

#ifndef _LIBSTDF_H
#define _LIBSTDF_H

#include <libstdf_types.h>

#include <sys/types.h>
#define	__STDF_HOST_BYTE_ORDER		BYTE_ORDER
typedef	__uint8_t		byte_t;
typedef struct {
	int			fd;
	int			byte_order;
	rec_header	header;
	off_t		pos;
	off_t		rec_end;
} stdf_file;

#include <libstdf_funcs.h>

#endif /* _LIBSTDF_H */
