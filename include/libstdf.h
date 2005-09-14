/**
 * @file libstdf.h
 * @brief Top level include and the main stdf structure definition.
 */
/*
 * Copyright (C) 2004-2005 Mike Frysinger <vapier@gmail.com>
 * Released under the BSD license.  For more information,
 * please see: http://opensource.org/licenses/bsd-license.php
 *
 * $Header$
 */

#ifndef _LIBSTDF_H
#define _LIBSTDF_H

#ifdef __cplusplus
extern "C" {
#endif

#include <libstdf_systems.h>
#include <libstdf_bswap.h>

#include <libstdf_const.h>
#include <libstdf_types.h>

/* STDF File structure */
#define	__STDF_HOST_BYTE_ORDER		BYTE_ORDER

typedef struct {
	int		(*open)(void*, int, uint32_t);
	int		(*read)(void*, void*, long);
	int		(*close)(void*);
} __stdf_fops;

/**
 * Compressed file format
 */
typedef enum {
	STDF_FORMAT_REG,			/**< Regular file */
	STDF_FORMAT_ZIP,			/**< Zipped file */
	STDF_FORMAT_GZIP,			/**< gzipped file */
	STDF_FORMAT_BZIP2			/**< bzipped file */
} stdf_format;

/**
 * Misc options to control libstdf behavior
 */
typedef enum {
	STDF_OPTS_DEFAULT  = 0x000,  /**< Default options */
	STDF_OPTS_FORCE    = 0x001,  /**< Force reading even if some sanity checks fail */
#ifdef STDF_VER3
	STDF_OPTS_FORCE_V3 = 0x002,  /**< Force STDFv3 behavior */
#endif
	STDF_OPTS_FORCE_V4 = 0x004,  /**< Force STDFv4 behavior */
	STDF_OPTS_ZIP      = 0x008,  /**< File is compressed with zip */
	STDF_OPTS_GZIP     = 0x010,  /**< File is compressed with gzip */
	STDF_OPTS_BZIP2    = 0x020,  /**< File is compressed with bzip2 */
	STDF_OPTS_READ     = 0x040,  /**< Allow reading from file */
	STDF_OPTS_WRITE    = 0x080,  /**< Allow writing to file */
	STDF_OPTS_CREATE   = 0x100   /**< Create file from scratch */
} stdf_options;

/**
 * @brief The main STDF file structure.
 */
typedef struct {
	rec_header	header;			/**< A processed version of the last record read */

	int			fd;				/**< Actual file descriptor for the backing file */
#if !HAVE_NO_COMPRESSION
	union {
# if HAVE_ZIP
	ZZIP_FILE	*zip;
#  define fd_zip __fd.zip
# endif
# if HAVE_GZIP
	gzFile		*gzip;
#  define fd_gzip __fd.gzip
# endif
# if HAVE_BZIP2
	BZFILE		*bzip2;
#  define fd_bzip2 __fd.bzip2
# endif
	} __fd;
#endif
	stdf_format	file_format;	/**< Compressed file format */
	char		*filename;		/**< Filename that was given to stdf_open() */
	__stdf_fops	*fops;			/**< Virtual file i/o functions to hide compression details */

	int			byte_order;		/**< Byte order of the file */
	uint32_t	opts;			/**< Misc options to control libstdf behavior */
	dtc_U1		ver;			/**< Spec version of the file */

	byte_t		*__data;
	byte_t		*rec_pos;
	byte_t		*rec_end;
} stdf_file;

#include <libstdf_funcs.h>

#ifdef __cplusplus
}
#endif

#endif /* _LIBSTDF_H */
