/**
 * @file libstdf_internal.h
 * @brief All the fun internal details no one needs to know about
 * @internal
 */
/*
 * Copyright (C) 2005-2007 Mike Frysinger <vapier@gmail.com>
 * Released under the BSD license.  For more information,
 * please see: http://opensource.org/licenses/bsd-license.php
 */

#ifndef _LIBSTDF_INTERNAL_H
#define _LIBSTDF_INTERNAL_H

#ifdef HAVE_ZIP
# define USE_ZIP(x) x
#else
# define USE_ZIP(x)
#endif
#ifdef HAVE_GZIP
# define USE_GZIP(x) x
#else
# define USE_GZIP(x)
#endif
#ifdef HAVE_BZIP2
# define USE_BZIP2(x) x
#else
# define USE_BZIP2(x)
#endif
#ifdef HAVE_LZW
# define USE_LZW(x) x
#else
# define USE_LZW(x)
#endif

/* STDF File structure */
#define	__STDF_HOST_BYTE_ORDER		BYTE_ORDER

typedef struct {
	int (*open)(void*, int, uint32_t);
	int (*read)(void*, void*, long);
	int (*close)(void*);
} __stdf_fops;

typedef	uint8_t byte_t;

/**
 * @brief The main STDF file structure.
 */
typedef struct {
	stdf_rec_header header;           /**< A processed version of the last record read */

	int fd;                           /**< Actual file descriptor for the backing file */
	union {
		USE_ZIP   (ZZIP_FILE *zip;)
		USE_GZIP  (gzFile gzip;)
		USE_BZIP2 (BZFILE *bzip2;)
		USE_LZW   (lzwFile *lzw;)
		/* leave space for 8 compression type ... */
		uintptr_t  padding[8];
	} __fd;
#define fd_zip    __fd.zip
#define fd_gzip   __fd.gzip
#define fd_bzip2  __fd.bzip2
#define fd_lzw    __fd.lzw

	stdf_format   file_format;        /**< Compressed file format */
	char         *filename;           /**< Filename that was given to stdf_open() */
	__stdf_fops  *fops;               /**< Virtual file i/o functions to hide compression details */

	int           byte_order;         /**< Byte order of the file */
	uint32_t      opts;               /**< Misc options to control libstdf behavior */
	stdf_dtc_U1   ver;                /**< Spec version of the file */

	byte_t       *__data;
	byte_t       *rec_pos;
	byte_t       *rec_end;

	byte_t       *__output;
	byte_t       *_write_pos;
	stdf_dtc_U2   _write_chunk_size;
} stdf_file;



/**
 * @brief Helpful debug macros.
 */
#define _warn(fmt, args...)  fprintf(stderr, fmt , ## args)
#define warn(fmt, args...)   _warn(PACKAGE_STRING ": " fmt "\n" , ## args)
#define warnf(fmt, args...)  warn("%s(): " fmt, __FUNCTION__ , ## args)
#define warnp(fmt, args...)  warn(fmt ": %s" , ## args , strerror(errno))
#define warnfp(fmt, args...) warnp("%s(): " fmt, __FUNCTION__ , ## args)

#define dumphex(buf, cnt) \
	do { \
		size_t i = 0; \
		_warn("dumphex: "); \
		while (i < cnt) \
			_warn("%2.2X ", buf[i++]); \
		_warn("\n"); \
	} while (0)


/**
 * @brief Misc cruft for debugging fun.
 */
#if HAVE_MCHECK_H
# include <mcheck.h>
# define _stdf_mtrace()   mtrace()
# define _stdf_muntrace() muntrace()
#else
# define _stdf_mtrace()
# define _stdf_muntrace()
#endif

#if HAVE_DMALLOC_H
# include <dmalloc.h>
#endif

#if HAVE_EFENCE_H
# include <efence.h>
#endif

#endif /* _LIBSTDF_INTERNAL_H */
