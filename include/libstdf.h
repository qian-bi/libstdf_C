/**
 * @file libstdf.h
 * @brief Top level include and the main stdf structure definition.
 *
 * Copyright (C) 2004 Mike Frysinger <vapier@gmail.com>
 * Released under the BSD license.  For more information,
 * please see: http://opensource.org/licenses/bsd-license.php
 *
 * $Header$
 */

#ifndef _LIBSTDF_H
#define _LIBSTDF_H

#include <libstdf_types.h>
#include <libstdf_const.h>

/* Do we read one record at a time or a whole lot of data ? */
#define	__STDF_READ_ONE_RECORD		1
#define	__STDF_READ_SIZE			4096

/* STDF File structure */
#define	__STDF_HOST_BYTE_ORDER		BYTE_ORDER
typedef	uint8_t						byte_t;

typedef struct {
	int		(*open)(void*);
	int		(*read)(void*, void*, long);
	int		(*close)(void*);
	int		(*reopen)(void*);
} __stdf_fops;

typedef struct {
	rec_header	header;

	int			fd;
	union {
#if HAVE_ZIP
	ZZIP_FILE	*fd_zip;
#endif
#if HAVE_GZIP
	gzFile		fd_gzip;
#endif
#if HAVE_BZIP2
	BZFILE		*fd_bzip2;
#endif
	};
	char		*filename;
	byte_t		file_format;
	__stdf_fops	*fops;

	int			byte_order;
	uint32_t	opts;
	dtc_U1		ver;

	byte_t		*__data;
#ifndef __STDF_READ_ONE_RECORD
	byte_t		*__data_end;
#endif
	byte_t		*rec_pos;
	byte_t		*rec_end;
} stdf_file;

/* the format of the stdf file */
#define	STDF_FILE_REG				0x01
#define	STDF_FILE_ZIP				0x02
#define	STDF_FILE_GZIP				0x03
#define	STDF_FILE_BZIP2				0x04

/* options for input behavior */
#define	STDF_OPTS_FORCE				0x01
#ifdef STDF_VER3
#define	STDF_OPTS_FORCE_V3			0x02
#endif
#define	STDF_OPTS_FORCE_V4			0x04
#define	STDF_OPTS_DEFAULT			0x00
#define	STDF_OPTS_ZIP				0x08
#define	STDF_OPTS_GZIP				0x10
#define	STDF_OPTS_BZIP2				0x20

#include <libstdf_funcs.h>

#endif /* _LIBSTDF_H */
