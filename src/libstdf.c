/**
 * @file libstdf.c
 * @brief Implementation of all the public libstdf functions.
 * @internal
 */
/*
 * Copyright (C) 2004 Mike Frysinger <vapier@gmail.com>
 * Released under the BSD license.  For more information,
 * please see: http://opensource.org/licenses/bsd-license.php
 *
 * $Header$
 */

#include <libstdf.h>
#include "dtc.h"
#include "rec.h"

int __stdf_init(stdf_file *f, dtc_U1 cpu_type, dtc_U1 stdf_ver, uint32_t opts)
{
	switch (cpu_type) {
		case CPU_TYPE_DEC:
			f->byte_order = 0xBEEF;
			fprintf(stderr, "byte_order: CPU_TYPE_DEC (PDP_ENDIAN) has no implementation\n");
			break;

		/*case CPU_TYPE_SUN_680XX:*/
		case CPU_TYPE_SPARC:
			f->byte_order = BIG_ENDIAN;
			break;

		/*case CPU_TYPE_SUN_80386:*/
		case CPU_TYPE_X86:
			f->byte_order = LITTLE_ENDIAN;
			break;

#ifdef STDF_VER3
		case CPU_TYPE_LTX:
			if (stdf_ver == 3) {
				fprintf(stderr, "byte_order: CPU_TYPE_LTX (???_ENDIAN) has no implementation\n");
				break;
			}
		case CPU_TYPE_APOLLO:
			if (stdf_ver == 3) {
				fprintf(stderr, "byte_order: CPU_TYPE_APOLLO (???_ENDIAN) has no implementation\n");
				break;
			}
#endif
		default:
			f->byte_order = __STDF_HOST_BYTE_ORDER;
			break;
	}
	if ((opts & STDF_OPTS_FORCE_V4) || (opts & STDF_OPTS_FORCE) || stdf_ver == 4) {
		f->ver = 4;
#ifdef STDF_VER3
	} else if ((opts & STDF_OPTS_FORCE_V3) || stdf_ver == 3) {
		f->ver = 3;
#endif
	} else {
		fprintf(stderr, "Unable to handle STDF ver%i !\n", stdf_ver);
		return 1;
	}
	f->opts = opts;

	f->rec_pos = NULL;
	f->rec_end = NULL;

	return 0;
}

static int __stdf_open_reg(void *data)
{
	stdf_file *stdf = (stdf_file*)data;

	/* if filename is NULL we can assume that the fd is already set ... */
	if (stdf->filename) {
		if (stdf->filename[0] == '-' && stdf->filename[1] == '\0')
			stdf->fd = 0;
		else
			stdf->fd = open(stdf->filename, O_RDONLY | O_BINARY);
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

#if HAVE_ZIP
static int __stdf_open_zip(void *data)
{
	stdf_file *stdf = (stdf_file*)data;

	/* am i doing something wrong or does
	   zziplib really suck this much ? */
	ZZIP_DIR *d;
	ZZIP_DIRENT *de;
	zzip_error_t err;

	stdf->fd_zip = NULL;

	if (__stdf_open_reg(data) == -1)
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

#if HAVE_GZIP
static int __stdf_open_gzip(void *data)
{
	stdf_file *stdf = (stdf_file*)data;
	stdf->fd_gzip = NULL;

	if (__stdf_open_reg(data) == -1)
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

#if HAVE_BZIP2
static int __stdf_open_bzip2(void *data)
{
	stdf_file *stdf = (stdf_file*)data;
	stdf->fd_bzip2 = NULL;

	if (__stdf_open_reg(data) == -1)
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

static stdf_file* _stdf_open(char *pathname, int fd, uint32_t opts)
{
	stdf_file *ret = (stdf_file*)malloc(sizeof(stdf_file));

	if (!pathname || pathname[0] == '\0') {
		if (fd == -1) {
			free(ret);
			return NULL;
		}
		ret->filename = NULL;
		ret->fd = fd;
	} else
		ret->filename = strdup(pathname);
	ret->fops = NULL;

	if (opts & STDF_OPTS_ZIP)
		ret->file_format = STDF_FORMAT_ZIP;
	else if (opts & STDF_OPTS_GZIP)
		ret->file_format = STDF_FORMAT_GZIP;
	else if (opts & STDF_OPTS_BZIP2)
		ret->file_format = STDF_FORMAT_BZIP2;
	else if (ret->filename) {
		/* try to guess from the filename if it's compressed */
		if (strrchr(ret->filename, '.') != NULL)
			if (strstr(ret->filename, ".zip") != NULL)
				ret->file_format = STDF_FORMAT_ZIP;
			else if (strstr(ret->filename, ".gz") != NULL)
				ret->file_format = STDF_FORMAT_GZIP;
			else if (strstr(ret->filename, ".bz") != NULL || strstr(ret->filename, ".bz2") != NULL)
				ret->file_format = STDF_FORMAT_BZIP2;
			else
				ret->file_format = STDF_FORMAT_REG;
		else
			ret->file_format = STDF_FORMAT_REG;
	}

	switch (ret->file_format) {
		case STDF_FORMAT_ZIP:
#if HAVE_ZIP
			ret->fops = &__stdf_fops_zip;
			break;
#else
			fprintf(stderr, "stdf_open(): zip support was disabled!\n");
			goto out_err;
#endif
		case STDF_FORMAT_GZIP:
#if HAVE_GZIP
			ret->fops = &__stdf_fops_gzip;
			break;
#else
			fprintf(stderr, "stdf_open(): gzip support was disabled!\n");
			goto out_err;
#endif
		case STDF_FORMAT_BZIP2:
#if HAVE_BZIP2
			ret->fops = &__stdf_fops_bzip2;
			break;
#else
			fprintf(stderr, "stdf_open(): bzip2 compression was disabled!\n");
			goto out_err;
#endif
		default:
		case STDF_FORMAT_REG:
			ret->fops = &__stdf_fops_reg;
	}

	if (ret->fops->open(ret) == -1)
		goto out_err;

	/* try to peek at the FAR record to figure out the CPU type/STDF ver */
	ret->__data = (byte_t*)malloc(6);
	if (ret->fops->read(ret, ret->__data, 6) != 6)
		goto out_err;
	else {
		if ((MAKE_REC(ret->__data[2], ret->__data[3]) != REC_FAR)
#ifdef STDF_VER3
		    /* STDF v3 can have either a FAR or a MIR record */
		    && (MAKE_REC(ret->__data[2], ret->__data[3]) != REC_MIR)
#endif
		   )
			goto out_err;
		if (__stdf_init(ret, ret->__data[4], ret->__data[5], opts))
			goto out_err;
	}

	return ret;
out_err:
	if (ret->fops)
		ret->fops->close(ret);
	free(ret->filename);
	free(ret);
	return NULL;
}
stdf_file* stdf_open_ex(char *pathname, uint32_t opts)
{
	return _stdf_open(pathname, -1, opts);
}
stdf_file* stdf_open(char *pathname)
{
	return _stdf_open(pathname, -1, STDF_OPTS_DEFAULT);
}
stdf_file* stdf_dopen(int fd)
{
	return _stdf_open(NULL, fd, STDF_OPTS_DEFAULT);
}
stdf_file* stdf_dopen_ex(int fd, uint32_t opts)
{
	return _stdf_open(NULL, fd, opts);
}

int stdf_close(stdf_file *file)
{
	int ret = file->fops->close(file);
	if (file->filename) free(file->filename);
	free(file);
	return ret;
}

rec_unknown* stdf_read_record_raw(stdf_file *file)
{
	rec_unknown *raw_rec = NULL;
	char header[6];
	int cheated;

	if (!file->__data) {
		/* read the record header to find out how big this next record is */
		if (file->fops->read(file, header, 4) != 4)
			return NULL;
		cheated = 0;
	} else {
		memcpy(header, file->__data, 6);
		free(file->__data);
		file->__data = NULL;
		cheated = 1;
	}
	raw_rec = (rec_unknown*)malloc(sizeof(rec_unknown));
	if (raw_rec == NULL) {
		perror("stdf_read_record_raw():malloc.1");
		return NULL;
	}
	raw_rec->header.stdf_file = (void*)file;
	raw_rec->header.state = REC_STATE_RAW;
	memcpy(&(raw_rec->header.REC_LEN), header, 2);
	raw_rec->header.REC_TYP = header[2];
	raw_rec->header.REC_SUB = header[3];
	memcpy(&(file->header), &(raw_rec->header), sizeof(rec_header));
	_stdf_byte_order_to_host(file, &(file->header.REC_LEN), sizeof(dtc_U2));

	/* buffer the whole record in memory */
	raw_rec->data = (void*)malloc(file->header.REC_LEN+4);
	if (raw_rec->data == NULL) {
		perror("stdf_read_record_raw():malloc.2");
		free(raw_rec);
		return NULL;
	}
	if (cheated) {
		file->fops->read(file, ((byte_t*)raw_rec->data)+6, file->header.REC_LEN-2);
		memcpy(raw_rec->data, header, 6);
	} else {
		file->fops->read(file, ((byte_t*)raw_rec->data)+4, file->header.REC_LEN);
		memcpy(raw_rec->data, header, 4);
	}

	return raw_rec;
}

rec_unknown* stdf_parse_raw_record(rec_unknown *raw_rec)
{
	rec_unknown *rec;
	stdf_file *file;

	if (!raw_rec)
		return NULL;

	file = (stdf_file*)(raw_rec->header.stdf_file);

	/* setup the buffer so the rest of the library can parse it if need be */
	file->__data = (byte_t*)raw_rec->data;
	file->rec_pos = file->__data + 4;
	file->rec_end = file->rec_pos + file->header.REC_LEN;

	switch (HEAD_TO_REC(file->header)) {
		case REC_FAR: rec = (rec_unknown*)stdf_read_rec_far(file); break;
		case REC_ATR: rec = (rec_unknown*)stdf_read_rec_atr(file); break;
		case REC_MIR: rec = (rec_unknown*)stdf_read_rec_mir(file); break;
		case REC_MRR: rec = (rec_unknown*)stdf_read_rec_mrr(file); break;
		case REC_PCR: rec = (rec_unknown*)stdf_read_rec_pcr(file); break;
		case REC_HBR: rec = (rec_unknown*)stdf_read_rec_hbr(file); break;
		case REC_SBR: rec = (rec_unknown*)stdf_read_rec_sbr(file); break;
		case REC_PMR: rec = (rec_unknown*)stdf_read_rec_pmr(file); break;
		case REC_PGR: rec = (rec_unknown*)stdf_read_rec_pgr(file); break;
		case REC_PLR: rec = (rec_unknown*)stdf_read_rec_plr(file); break;
		case REC_RDR: rec = (rec_unknown*)stdf_read_rec_rdr(file); break;
		case REC_SDR: rec = (rec_unknown*)stdf_read_rec_sdr(file); break;
		case REC_WIR: rec = (rec_unknown*)stdf_read_rec_wir(file); break;
		case REC_WRR: rec = (rec_unknown*)stdf_read_rec_wrr(file); break;
		case REC_WCR: rec = (rec_unknown*)stdf_read_rec_wcr(file); break;
		case REC_PIR: rec = (rec_unknown*)stdf_read_rec_pir(file); break;
		case REC_PRR: rec = (rec_unknown*)stdf_read_rec_prr(file); break;
#ifdef STDF_VER3
		case REC_PDR: rec = (rec_unknown*)stdf_read_rec_pdr(file); break;
		case REC_FDR: rec = (rec_unknown*)stdf_read_rec_fdr(file); break;
#endif
		case REC_TSR: rec = (rec_unknown*)stdf_read_rec_tsr(file); break;
		case REC_PTR: rec = (rec_unknown*)stdf_read_rec_ptr(file); break;
		case REC_MPR: rec = (rec_unknown*)stdf_read_rec_mpr(file); break;
		case REC_FTR: rec = (rec_unknown*)stdf_read_rec_ftr(file); break;
		case REC_BPS: rec = (rec_unknown*)stdf_read_rec_bps(file); break;
		case REC_EPS: rec = (rec_unknown*)stdf_read_rec_eps(file); break;
#ifdef STDF_VER3
		case REC_SHB: rec = (rec_unknown*)stdf_read_rec_shb(file); break;
		case REC_SSB: rec = (rec_unknown*)stdf_read_rec_ssb(file); break;
		case REC_STS: rec = (rec_unknown*)stdf_read_rec_sts(file); break;
		case REC_SCR: rec = (rec_unknown*)stdf_read_rec_scr(file); break;
#endif
		case REC_GDR: rec = (rec_unknown*)stdf_read_rec_gdr(file); break;
		case REC_DTR: rec = (rec_unknown*)stdf_read_rec_dtr(file); break;
		default:
			rec = stdf_read_rec_unknown(file);
			file->header.REC_TYP = REC_TYP_UNKNOWN;
			file->header.REC_SUB = REC_SUB_UNKNOWN;
			break;
	}
	file->header.state = REC_STATE_PARSED;
	memcpy(&(rec->header), &(file->header), sizeof(rec_header));

	file->__data = NULL;

	return rec;
}

rec_unknown* stdf_read_record(stdf_file *file)
{
	rec_unknown *raw_rec = NULL,
	            *rec = NULL;

	raw_rec = stdf_read_record_raw(file);
	if (raw_rec == NULL)
		return NULL;
	rec = stdf_parse_raw_record(raw_rec);
	stdf_free_record(raw_rec);

	return rec;
}
