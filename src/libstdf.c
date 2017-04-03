/**
 * @file libstdf.c
 * @brief Implementation of all the public libstdf functions.
 * @internal
 */
/*
 * Copyright (C) 2004-2007 Mike Frysinger <vapier@gmail.com>
 * Copyright (C) 2017 Stefan Brandner <stefan.brandner@gmx.at>
 * Released under the BSD license.  For more information,
 * please see: http://opensource.org/licenses/bsd-license.php
 */

#include <libstdf.h>
#include "dtc.h"
#include "rec.h"



static int __stdf_init(stdf_file *f, stdf_dtc_U1 cpu_type, stdf_dtc_U1 stdf_ver, uint32_t opts)
{
	switch (cpu_type) {
		case STDF_CPU_TYPE_DEC:
			f->byte_order = 0xBEEF;
			warnf("STDF_CPU_TYPE_DEC (PDP_ENDIAN) has no implementation");
			break;

		/*case STDF_CPU_TYPE_SUN_680XX:*/
		case STDF_CPU_TYPE_SPARC:
			f->byte_order = STDF_ENDIAN_BIG;
			break;

		/*case STDF_CPU_TYPE_SUN_80386:*/
		case STDF_CPU_TYPE_X86:
			f->byte_order = STDF_ENDIAN_LITTLE;
			break;

#ifdef STDF_VER3
		case STDF_CPU_TYPE_LTX:
			if (stdf_ver == 3) {
				warnf("STDF_CPU_TYPE_LTX (???_ENDIAN) has no implementation");
				break;
			}
		case STDF_CPU_TYPE_APOLLO:
			if (stdf_ver == 3) {
				warnf("STDF_CPU_TYPE_APOLLO (???_ENDIAN) has no implementation");
				break;
			}
#endif
		default:
			f->byte_order = STDF_ENDIAN_HOST;
			if (!(opts & STDF_OPTS_CREATE))
				warnf("Unknown STDF_CPU type 0x%X; you may have incorrect output", cpu_type);
			break;
	}

	if ((opts & STDF_OPTS_FORCE_V4) || (opts & STDF_OPTS_FORCE) || stdf_ver == 4) {
		f->ver = 4;
#ifdef STDF_VER3
	} else if ((opts & STDF_OPTS_FORCE_V3) || stdf_ver == 3) {
		f->ver = 3;
#endif
	} else {
		warnf("Unable to handle STDF ver%i !", stdf_ver);
		return 1;
	}

	f->opts = opts;

	f->rec_pos = NULL;
	f->rec_end = NULL;

	if (f->opts & STDF_OPTS_WRITE) {
		/* make the buffer big enough to hold the largest record possible */
		f->__output = malloc(sizeof(byte_t) * (stdf_dtc_U2)-1);
		if (f->__output == NULL)
			return 1;
	} else
		f->__output = NULL;
	f->_write_pos = f->__output;
	f->_write_chunk_size = 4096;

	return 0;
}

int stdf_set_setting(stdf_file *f, uint32_t option, ...)
{
	va_list ap;
	stdf_dtc_U4 input;

	va_start(ap, option);
	input = va_arg(ap, stdf_dtc_U4);
	va_end(ap);

	switch (option) {
		case STDF_SETTING_WRITE_SIZE: f->_write_chunk_size = input; break;
		case STDF_SETTING_VERSION:    f->ver               = input; break;
		case STDF_SETTING_BYTE_ORDER: f->byte_order        = input; break;
	}

	return 0;
}
void stdf_get_setting(stdf_file *f, uint32_t option, ...)
{
	va_list ap;
	stdf_dtc_U4 *ret;

	va_start(ap, option);
	ret = va_arg(ap, stdf_dtc_U4*);
	va_end(ap);

	switch (option) {
		case STDF_SETTING_WRITE_SIZE: *ret = f->_write_chunk_size; break;
		case STDF_SETTING_VERSION:    *ret = f->ver;               break;
		case STDF_SETTING_BYTE_ORDER: *ret = f->byte_order;        break;
	}
}


/*
 * UNCOMPRESSED SUPPORT
 */
static int __stdf_open_reg(void *data, int flags, uint32_t mode)
{
	stdf_file *stdf = (stdf_file*)data;

	/* if filename is NULL we can assume that the fd is already set ... */
	if (stdf->filename) {
		if (stdf->filename[0] == '-' && stdf->filename[1] == '\0')
			stdf->fd = 0;
		else
			stdf->fd = open(stdf->filename, flags, mode);
	}

	return stdf->fd;
}
static int __stdf_read_reg(void *data, void *buf, long count)
{
	return read(((stdf_file*)data)->fd, buf, count);
}
static int __stdf_close_reg(void *data)
{
	stdf_file *stdf = (stdf_file*)data;
	if (stdf->__data) {
		free(stdf->__data);
		stdf->__data = NULL;
	}
	return close(stdf->fd);
}
static __stdf_fops __stdf_fops_reg = {
	__stdf_open_reg,
	__stdf_read_reg,
	__stdf_close_reg
};



/*
 * ZIP SUPPORT
 */
#ifdef HAVE_ZIP
static int __stdf_open_zip(void *data, int flags, uint32_t mode)
{
	stdf_file *stdf = (stdf_file*)data;
	ZZIP_DIR *d;
	ZZIP_DIRENT *de;
	zzip_error_t err;

	stdf->fd_zip = NULL;

	if (__stdf_open_reg(data, flags, mode) == -1)
		return -1;

	d = zzip_dir_fdopen(stdf->fd, &err);
	if (d == NULL)
		return -1;
	de = zzip_readdir(d);
	if (de == NULL) {
		zzip_dir_close(d);
		return -1;
	}
	stdf->fd_zip = zzip_file_open(d, de->d_name, O_RDONLY);
	zzip_dir_close(d);
	if (stdf->fd_zip == NULL)
		return -1;

	return 0;
}
static int __stdf_read_zip(void *data, void *buf, long count)
{
	return zzip_read(((stdf_file*)data)->fd_zip, buf, count);
}
static int __stdf_close_zip(void *data)
{
	stdf_file *stdf = (stdf_file*)data;
	return (stdf->fd_zip==NULL) ? -1 : zzip_close(stdf->fd_zip);
}
static __stdf_fops __stdf_fops_zip = {
	__stdf_open_zip,
	__stdf_read_zip,
	__stdf_close_zip
};
#endif



/*
 * GZIP SUPPORT
 */
#ifdef HAVE_GZIP
static int __stdf_open_gzip(void *data, int flags, uint32_t mode)
{
	stdf_file *stdf = (stdf_file*)data;
	stdf->fd_gzip = NULL;

	if (__stdf_open_reg(data, flags, mode) == -1)
		return -1;

	stdf->fd_gzip = gzdopen(stdf->fd, "rb");
	if (stdf->fd_gzip == NULL)
		return -1;

	return stdf->fd;
}
static int __stdf_read_gzip(void *data, void *buf, long count)
{
	return gzread(((stdf_file*)data)->fd_gzip, buf, count);
}
static int __stdf_close_gzip(void *data)
{
	stdf_file *stdf = (stdf_file*)data;
	if (stdf->fd_gzip != NULL)
		gzclose(stdf->fd_gzip);
	return __stdf_close_reg(data);
}
static __stdf_fops __stdf_fops_gzip = {
	__stdf_open_gzip,
	__stdf_read_gzip,
	__stdf_close_gzip
};
#endif



/*
 * BZIP2 SUPPORT
 */
#ifdef HAVE_BZIP2
static int __stdf_open_bzip2(void *data, int flags, uint32_t mode)
{
	stdf_file *stdf = (stdf_file*)data;
	stdf->fd_bzip2 = NULL;

	if (__stdf_open_reg(data, flags, mode) == -1)
		return -1;

	stdf->fd_bzip2 = BZ2_bzdopen(stdf->fd, "rb");
	if (stdf->fd_bzip2 == NULL)
		return -1;

	return stdf->fd;
}
static int __stdf_read_bzip2(void *data, void *buf, long count)
{
	return BZ2_bzread(((stdf_file*)data)->fd_bzip2, buf, count);
}
static int __stdf_close_bzip2(void *data)
{
	stdf_file *stdf = (stdf_file*)data;
	if (stdf->fd_bzip2 != NULL)
		BZ2_bzclose(stdf->fd_bzip2);
	return __stdf_close_reg(data);
}
static __stdf_fops __stdf_fops_bzip2 = {
	__stdf_open_bzip2,
	__stdf_read_bzip2,
	__stdf_close_bzip2
};
#endif



/*
 * LZW SUPPORT
 */
#ifdef HAVE_LZW
static int __stdf_open_lzw(void *data, int flags, uint32_t mode)
{
	stdf_file *stdf = (stdf_file*)data;
	stdf->fd_lzw = NULL;

	if (__stdf_open_reg(data, flags, mode) == -1)
		return -1;

	stdf->fd_lzw = lzw_fdopen(stdf->fd);
	if (stdf->fd_lzw == NULL)
		return -1;

	return stdf->fd;
}
static int __stdf_read_lzw(void *data, void *buf, long count)
{
	return lzw_read(((stdf_file*)data)->fd_lzw, buf, count);
}
static int __stdf_close_lzw(void *data)
{
	return lzw_close(((stdf_file*)data)->fd_lzw);
}
static __stdf_fops __stdf_fops_lzw = {
	__stdf_open_lzw,
	__stdf_read_lzw,
	__stdf_close_lzw
};
#endif



static stdf_file* _stdf_open(char *pathname, int fd, uint32_t opts, uint32_t mode)
{
	int flags, ret_errno = EINVAL;
	stdf_file *ret;

	_stdf_mtrace();
	ret = malloc(sizeof(*ret));

	if (!pathname || pathname[0] == '\0') {
		if (fd == -1) {
			free(ret);
			goto set_errno_and_ret;
		}
		ret->filename = NULL;
		ret->fd = fd;
	} else
		ret->filename = strdup(pathname);
	ret->fops = NULL;
	ret->__data = NULL;

	if (opts == STDF_OPTS_DEFAULT)
		opts = STDF_OPTS_READ;

	if (opts & STDF_OPTS_ZIP)
		ret->file_format = STDF_FORMAT_ZIP;
	else if (opts & STDF_OPTS_GZIP)
		ret->file_format = STDF_FORMAT_GZIP;
	else if (opts & STDF_OPTS_BZIP2)
		ret->file_format = STDF_FORMAT_BZIP2;
	else if (opts & STDF_OPTS_LZW)
		ret->file_format = STDF_FORMAT_LZW;
	else if (ret->filename) {
		/* try to guess from the filename if it's compressed */
		if (strrchr(ret->filename, '.') != NULL) {
			struct {
				char *filetype;
				stdf_format fmt;
			} guesses[] = {
				{ ".zip", STDF_FORMAT_ZIP   },
				{ ".gz",  STDF_FORMAT_GZIP  },
				{ ".Z",   STDF_FORMAT_LZW   },
				{ ".bz",  STDF_FORMAT_BZIP2 },
				{ ".bz2", STDF_FORMAT_BZIP2 },
			};
			size_t i;
			ret->file_format = STDF_FORMAT_REG;
			for (i = 0; i < ARRAY_SIZE(guesses); ++i)
				if (strstr(ret->filename, guesses[i].filetype) != NULL) {
					ret->file_format = guesses[i].fmt;
					break;
				}
		} else
			ret->file_format = STDF_FORMAT_REG;
	}
        else if (ret->fd) 
                ret->file_format = STDF_FORMAT_REG;  

	switch (ret->file_format) {
#ifdef HAVE_ZIP
		case STDF_FORMAT_ZIP:
			ret->fops = &__stdf_fops_zip;
			break;
#endif
#ifdef HAVE_GZIP
		case STDF_FORMAT_GZIP:
			ret->fops = &__stdf_fops_gzip;
			break;
#endif
#ifdef HAVE_BZIP2
		case STDF_FORMAT_BZIP2:
			ret->fops = &__stdf_fops_bzip2;
			break;
#endif
#ifdef HAVE_LZW
		case STDF_FORMAT_LZW:
			ret->fops = &__stdf_fops_lzw;
			break;
#endif
		default:
			warn("format not supported");
			goto out_err;
		case STDF_FORMAT_REG:
			ret->fops = &__stdf_fops_reg;
	}

	flags = O_BINARY;
	if ((opts & STDF_OPTS_READ) && (opts & STDF_OPTS_WRITE))
		flags |= O_RDWR;
	else if (opts & STDF_OPTS_WRITE)
		flags |= O_WRONLY;
	else
		flags |= O_RDONLY;
	if (opts & STDF_OPTS_CREATE)
		flags |= O_CREAT | O_TRUNC;

	if (ret->fops->open(ret, flags, mode) == -1) {
		ret_errno = ENOENT;
		goto out_err;
	}

	if (!(opts & STDF_OPTS_WRITE)) {
		/* try to peek at the FAR record to figure out the CPU type/STDF ver */
		ret->__data = malloc(6);
		if (ret->fops->read(ret, ret->__data, 6) != 6)
			goto out_err;
		if ((MAKE_REC(ret->__data[2], ret->__data[3]) != STDF_REC_FAR)
#ifdef STDF_VER3
		    /* STDF v3 can have either a FAR or a MIR record */
		    && (MAKE_REC(ret->__data[2], ret->__data[3]) != STDF_REC_MIR)
#endif
		   )
			goto out_err;
		if (__stdf_init(ret, ret->__data[4], ret->__data[5], opts))
			goto out_err;
	} else {
		if (__stdf_init(ret, -1, 4, opts))
			goto out_err;
	}

	_stdf_muntrace();
	errno = 0;
	return ret;

out_err:
	if (ret->fops)
		ret->fops->close(ret);
	free(ret->filename);
	free(ret);

set_errno_and_ret:
	errno = ret_errno;
	_stdf_muntrace();
	return NULL;
}
stdf_file* stdf_open_ex(char *pathname, uint32_t opts, ...)
{
	uint32_t mode = 0;
	if (opts & STDF_OPTS_CREATE) {
		va_list ap;
		va_start(ap, opts);
		mode = va_arg(ap, uint32_t);
		va_end(ap);
	}
	return _stdf_open(pathname, -1, opts, mode);
}
stdf_file* stdf_open(char *pathname)
{
	return _stdf_open(pathname, -1, STDF_OPTS_DEFAULT, 0);
}
stdf_file* stdf_dopen(int fd)
{
	return _stdf_open(NULL, fd, STDF_OPTS_DEFAULT, 0);
}
stdf_file* stdf_dopen_ex(int fd, uint32_t opts, ...)
{
	uint32_t mode = 0;
	if (opts & STDF_OPTS_CREATE) {
		va_list ap;
		va_start(ap, opts);
		mode = va_arg(ap, uint32_t);
		va_end(ap);
	}
	return _stdf_open(NULL, fd, opts, mode);
}

int stdf_close(stdf_file *file)
{
	int ret, ret_errno;
	_stdf_mtrace();
	if (file->__output) {
		_stdf_write_flush(file, (size_t)-1);
		free(file->__output);
	}
	ret = file->fops->close(file);
	ret_errno = errno;
	if (file->filename) free(file->filename);
	free(file);
	_stdf_muntrace();
	errno = ret_errno;
	return ret;
}

stdf_rec_unknown* stdf_read_record_raw(stdf_file *file)
{
	stdf_rec_unknown *raw_rec = NULL;
	char header[6];
	int cheated=-1;

	_stdf_mtrace();

	if (!file->__data) {
		/* read the record header to find out how big this next record is */
		if (file->fops->read(file, header, 4) != 4)
			goto ret_null;
		cheated = 0;
	} else {
		memcpy(header, file->__data, 6);
		free(file->__data);
		file->__data = NULL;
		cheated = 1;
	}
	raw_rec = malloc(sizeof(*raw_rec));
	if (raw_rec == NULL) {
		warnfp("malloc.1");
		goto ret_null;
	}
	raw_rec->header.stdf_file = (void*)file;
	raw_rec->header.state = STDF_REC_STATE_RAW;
	memcpy(&(raw_rec->header.REC_LEN), header, 2);
	raw_rec->header.REC_TYP = (stdf_rec_typ)header[2];
	raw_rec->header.REC_SUB = (stdf_rec_sub)header[3];
	memcpy(&(file->header), &(raw_rec->header), sizeof(stdf_rec_header));
	_stdf_byte_order_to_host(file, &(file->header.REC_LEN), sizeof(stdf_dtc_U2));

	/* buffer the whole record in memory */
	raw_rec->data = malloc(file->header.REC_LEN+4);
	if (raw_rec->data == NULL) {
		warnfp("malloc.2");
		free(raw_rec);
		goto ret_null;
	}
	if (cheated) {
		file->fops->read(file, ((byte_t*)raw_rec->data)+6, file->header.REC_LEN-2);
		memcpy(raw_rec->data, header, 6);
	} else {
		file->fops->read(file, ((byte_t*)raw_rec->data)+4, file->header.REC_LEN);
		memcpy(raw_rec->data, header, 4);
	}

	_stdf_muntrace();
	return raw_rec;
ret_null:
	_stdf_muntrace();
	return NULL;
}

stdf_rec_unknown* stdf_parse_raw_record(stdf_rec_unknown *raw_rec)
{
	stdf_rec_unknown *rec;
	stdf_file *file;

	if (!raw_rec)
		return NULL;

	_stdf_mtrace();

	file = (stdf_file*)(raw_rec->header.stdf_file);

	/* setup the buffer so the rest of the library can parse it if need be */
	file->__data = (byte_t*)raw_rec->data;
	file->rec_pos = file->__data + 4;
	file->rec_end = file->rec_pos + file->header.REC_LEN;

	/* record order is based on frequency in 'standard' files
	 * Note: keep order in sync with rec.c
	 */
	switch (HEAD_TO_REC(file->header)) {
		/* STDF_REC_TYP_PER_EXEC */
		case STDF_REC_PTR: rec = (stdf_rec_unknown*)stdf_read_rec_ptr(file); break;
		case STDF_REC_MPR: rec = (stdf_rec_unknown*)stdf_read_rec_mpr(file); break;
		case STDF_REC_FTR: rec = (stdf_rec_unknown*)stdf_read_rec_ftr(file); break;
		case STDF_REC_STR: rec = (stdf_rec_unknown*)stdf_read_rec_str(file); break;

		/* STDF_REC_TYP_PER_PART */
		case STDF_REC_PIR: rec = (stdf_rec_unknown*)stdf_read_rec_pir(file); break;
		case STDF_REC_PRR: rec = (stdf_rec_unknown*)stdf_read_rec_prr(file); break;

		/* STDF_REC_TYP_PER_TEST */
		case STDF_REC_TSR: rec = (stdf_rec_unknown*)stdf_read_rec_tsr(file); break;
#ifdef STDF_VER3
		case STDF_REC_PDR: rec = (stdf_rec_unknown*)stdf_read_rec_pdr(file); break;
		case STDF_REC_FDR: rec = (stdf_rec_unknown*)stdf_read_rec_fdr(file); break;
#endif

		/* STDF_REC_TYP_GENERIC */
		case STDF_REC_DTR: rec = (stdf_rec_unknown*)stdf_read_rec_dtr(file); break;
		case STDF_REC_GDR: rec = (stdf_rec_unknown*)stdf_read_rec_gdr(file); break;

		/* STDF_REC_TYP_PER_PROG */
		case STDF_REC_BPS: rec = (stdf_rec_unknown*)stdf_read_rec_bps(file); break;
		case STDF_REC_EPS: rec = (stdf_rec_unknown*)stdf_read_rec_eps(file); break;

		/* STDF_REC_TYP_PER_SITE */
#ifdef STDF_VER3
		case STDF_REC_SHB: rec = (stdf_rec_unknown*)stdf_read_rec_shb(file); break;
		case STDF_REC_SSB: rec = (stdf_rec_unknown*)stdf_read_rec_ssb(file); break;
		case STDF_REC_STS: rec = (stdf_rec_unknown*)stdf_read_rec_sts(file); break;
		case STDF_REC_SCR: rec = (stdf_rec_unknown*)stdf_read_rec_scr(file); break;
#endif

		/* STDF_REC_TYP_PER_LOT */
		case STDF_REC_MIR: rec = (stdf_rec_unknown*)stdf_read_rec_mir(file); break;
		case STDF_REC_MRR: rec = (stdf_rec_unknown*)stdf_read_rec_mrr(file); break;
		case STDF_REC_PCR: rec = (stdf_rec_unknown*)stdf_read_rec_pcr(file); break;
		case STDF_REC_PMR: rec = (stdf_rec_unknown*)stdf_read_rec_pmr(file); break;
		case STDF_REC_PGR: rec = (stdf_rec_unknown*)stdf_read_rec_pgr(file); break;
		case STDF_REC_HBR: rec = (stdf_rec_unknown*)stdf_read_rec_hbr(file); break;
		case STDF_REC_SBR: rec = (stdf_rec_unknown*)stdf_read_rec_sbr(file); break;
		case STDF_REC_PLR: rec = (stdf_rec_unknown*)stdf_read_rec_plr(file); break;
		case STDF_REC_RDR: rec = (stdf_rec_unknown*)stdf_read_rec_rdr(file); break;
		case STDF_REC_SDR: rec = (stdf_rec_unknown*)stdf_read_rec_sdr(file); break;
		case STDF_REC_PSR: rec = (stdf_rec_unknown*)stdf_read_rec_psr(file); break;
		case STDF_REC_NMR: rec = (stdf_rec_unknown*)stdf_read_rec_nmr(file); break;
		case STDF_REC_CNR: rec = (stdf_rec_unknown*)stdf_read_rec_cnr(file); break;
		case STDF_REC_SSR: rec = (stdf_rec_unknown*)stdf_read_rec_ssr(file); break;
		case STDF_REC_CDR: rec = (stdf_rec_unknown*)stdf_read_rec_cdr(file); break;

		/* STDF_REC_TYP_PER_WAFER */
		case STDF_REC_WIR: rec = (stdf_rec_unknown*)stdf_read_rec_wir(file); break;
		case STDF_REC_WRR: rec = (stdf_rec_unknown*)stdf_read_rec_wrr(file); break;
		case STDF_REC_WCR: rec = (stdf_rec_unknown*)stdf_read_rec_wcr(file); break;

		/* STDF_REC_TYP_INFO */
		case STDF_REC_FAR: rec = (stdf_rec_unknown*)stdf_read_rec_far(file); break;
		case STDF_REC_ATR: rec = (stdf_rec_unknown*)stdf_read_rec_atr(file); break;
                case STDF_REC_VUR: rec = (stdf_rec_unknown*)stdf_read_rec_vur(file); break;

		default:
			rec = stdf_read_rec_unknown(file);
			file->header.REC_TYP = STDF_REC_TYP_UNKNOWN;
			file->header.REC_SUB = STDF_REC_SUB_UNKNOWN;
			break;
	}
	file->header.state = STDF_REC_STATE_PARSED;
	memcpy(&(rec->header), &(file->header), sizeof(stdf_rec_header));

	file->__data = NULL;

	_stdf_muntrace();
	return rec;
}

stdf_rec_unknown* stdf_read_record(stdf_file *file)
{
	stdf_rec_unknown *raw_rec = NULL,
	            *rec = NULL;

	raw_rec = stdf_read_record_raw(file);
	if (raw_rec == NULL)
		return NULL;
	rec = stdf_parse_raw_record(raw_rec);
	stdf_free_record(raw_rec);

	return rec;
}

ssize_t stdf_write_record(stdf_file *file, void *stdf_rec_void)
{
	stdf_rec_unknown *rec = (stdf_rec_unknown*)stdf_rec_void;
	/* record order is based on frequency in 'standard' files
	 * Note: keep order in sync with rec.c
	 */
	switch (HEAD_TO_REC(rec->header)) {
		/* STDF_REC_TYP_PER_EXEC */
		case STDF_REC_PTR: return stdf_write_rec_ptr(file, (stdf_rec_ptr*)rec);
		case STDF_REC_MPR: return stdf_write_rec_mpr(file, (stdf_rec_mpr*)rec);
		case STDF_REC_FTR: return stdf_write_rec_ftr(file, (stdf_rec_ftr*)rec);
		case STDF_REC_STR: return stdf_write_rec_str(file, (stdf_rec_str*)rec);

		/* STDF_REC_TYP_PER_PART */
		case STDF_REC_PIR: return stdf_write_rec_pir(file, (stdf_rec_pir*)rec);
		case STDF_REC_PRR: return stdf_write_rec_prr(file, (stdf_rec_prr*)rec);

		/* STDF_REC_TYP_PER_TEST */
		case STDF_REC_TSR: return stdf_write_rec_tsr(file, (stdf_rec_tsr*)rec);
#ifdef STDF_VER3
		case STDF_REC_PDR: return stdf_write_rec_pdr(file, (stdf_rec_pdr*)rec);
		case STDF_REC_FDR: return stdf_write_rec_fdr(file, (stdf_rec_fdr*)rec);
#endif

		/* STDF_REC_TYP_GENERIC */
		case STDF_REC_DTR: return stdf_write_rec_dtr(file, (stdf_rec_dtr*)rec);
		case STDF_REC_GDR: return stdf_write_rec_gdr(file, (stdf_rec_gdr*)rec);

		/* STDF_REC_TYP_PER_PROG */
		case STDF_REC_BPS: return stdf_write_rec_bps(file, (stdf_rec_bps*)rec);
		case STDF_REC_EPS: return stdf_write_rec_eps(file, (stdf_rec_eps*)rec);

		/* STDF_REC_TYP_PER_SITE */
#ifdef STDF_VER3
		case STDF_REC_SHB: return stdf_write_rec_shb(file, (stdf_rec_shb*)rec);
		case STDF_REC_SSB: return stdf_write_rec_ssb(file, (stdf_rec_ssb*)rec);
		case STDF_REC_STS: return stdf_write_rec_sts(file, (stdf_rec_sts*)rec);
		case STDF_REC_SCR: return stdf_write_rec_scr(file, (stdf_rec_scr*)rec);
#endif

		/* STDF_REC_TYP_PER_LOT */
		case STDF_REC_PMR: return stdf_write_rec_pmr(file, (stdf_rec_pmr*)rec);
		case STDF_REC_PGR: return stdf_write_rec_pgr(file, (stdf_rec_pgr*)rec);
		case STDF_REC_HBR: return stdf_write_rec_hbr(file, (stdf_rec_hbr*)rec);
		case STDF_REC_SBR: return stdf_write_rec_sbr(file, (stdf_rec_sbr*)rec);
		case STDF_REC_PLR: return stdf_write_rec_plr(file, (stdf_rec_plr*)rec);
		case STDF_REC_RDR: return stdf_write_rec_rdr(file, (stdf_rec_rdr*)rec);
		case STDF_REC_SDR: return stdf_write_rec_sdr(file, (stdf_rec_sdr*)rec);
		case STDF_REC_MIR: return stdf_write_rec_mir(file, (stdf_rec_mir*)rec);
		case STDF_REC_MRR: return stdf_write_rec_mrr(file, (stdf_rec_mrr*)rec);
		case STDF_REC_PCR: return stdf_write_rec_pcr(file, (stdf_rec_pcr*)rec);
		case STDF_REC_PSR: return stdf_write_rec_psr(file, (stdf_rec_psr*)rec);
		case STDF_REC_NMR: return stdf_write_rec_nmr(file, (stdf_rec_nmr*)rec);
		case STDF_REC_CNR: return stdf_write_rec_cnr(file, (stdf_rec_cnr*)rec);
		case STDF_REC_SSR: return stdf_write_rec_ssr(file, (stdf_rec_ssr*)rec);
		case STDF_REC_CDR: return stdf_write_rec_cdr(file, (stdf_rec_cdr*)rec);

		/* STDF_REC_TYP_PER_WAFER */
		case STDF_REC_WIR: return stdf_write_rec_wir(file, (stdf_rec_wir*)rec);
		case STDF_REC_WRR: return stdf_write_rec_wrr(file, (stdf_rec_wrr*)rec);
		case STDF_REC_WCR: return stdf_write_rec_wcr(file, (stdf_rec_wcr*)rec);

		/* STDF_REC_TYP_INFO */
		case STDF_REC_FAR: return stdf_write_rec_far(file, (stdf_rec_far*)rec);
		case STDF_REC_ATR: return stdf_write_rec_atr(file, (stdf_rec_atr*)rec);
		case STDF_REC_VUR: return stdf_write_rec_vur(file, (stdf_rec_vur*)rec);

		default:      return -1;
	}
}

ssize_t stdf_write_record_raw(stdf_file *file, stdf_rec_unknown *rec)
{
	return stdf_write_rec_raw(file,rec);
}
