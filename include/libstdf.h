/* libstdf.h
 * Copyright (C) 2004 Mike Frysinger <vapier@gmail.com>
 * Released under the BSD license.  For more information,
 * please see: http://opensource.org/licenses/bsd-license.php
 *
 * $Header$
 */

#ifndef _LIBSTDF_H
#define _LIBSTDF_H

#include <libstdf_types.h>

/* Do we read one record at a time or a whole lot of data ? */
#define	__STDF_READ_ONE_RECORD		1
#define	__STDF_READ_SIZE			4096

#include <sys/types.h>
#define	__STDF_HOST_BYTE_ORDER		BYTE_ORDER
typedef	__uint8_t					byte_t;
typedef struct {
	int			fd;
	int			byte_order;
	rec_header	header;
	byte_t		*__data;
#ifndef __STDF_READ_ONE_RECORD
	byte_t		*__data_end;
#endif
	byte_t		*rec_pos;
	byte_t		*rec_end;
} stdf_file;

#include <libstdf_funcs.h>

#endif /* _LIBSTDF_H */
