/* libstdf.c
 * Copyright (C) 2004 Mike Frysinger <vapier@gmail.com>
 * Released under the BSD license.  For more information,
 * please see: http://opensource.org/licenses/bsd-license.php
 */

#include <libstdf.h>
#include "dtc.h"
#include "rec.h"

/* for open(2) */
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
/* for printf(3) */
#include <stdio.h>
/* for va args stuff */
#include <stdarg.h>
/* for close(2) / read(2) */
#include <unistd.h>
/* for malloc(3) */
#include <stdlib.h>
/* for memcpy(3) */
#include <string.h>
/* for BYTE_ORDER defines */
#include <endian.h>
/* for bswap() functions */
#include <byteswap.h>

void __stdf_init(stdf_file *f, char src_cpu)
{
	switch (src_cpu) {
		case CPU_TYPE_DEC:		f->byte_order = PDP_ENDIAN;
			fprintf(stderr, "byte_order: CPU_TYPE_DEC (PDP_ENDIAN) has no implementation\n");
			break;
		case CPU_TYPE_SPARC:	f->byte_order = BIG_ENDIAN; break;
		case CPU_TYPE_X86:		f->byte_order = LITTLE_ENDIAN; break;
		default:				f->byte_order = __STDF_HOST_BYTE_ORDER; break;
	}
}

stdf_file* stdf_open_ex(char *pathname, int flags)
{
	stdf_file *ret = (stdf_file*)malloc(sizeof(stdf_file));
	ret->fd = open(pathname, flags);
	if (ret->fd == -1) {
		free(ret);
		ret = NULL;
	} else {
		char temp[6];
		if (read(ret->fd, temp, 6) != 6) {
			close(ret->fd);
			free(ret);
			ret = NULL;
		} else {
			__stdf_init(ret, temp[4]);
			lseek(ret->fd, 0, SEEK_SET);
		}
	}
	return ret;
}
stdf_file* stdf_open(char *pathname)
{
	return stdf_open_ex(pathname, O_RDONLY);
}

int stdf_close(stdf_file *file)
{
	int ret = close(file->fd);
	free(file);
	return ret;
}

rec_unknown* stdf_read_record(stdf_file *file)
{
	rec_unknown *rec = NULL;

	if (read(file->fd, &(file->header), 4) != 4)
		return rec;
	_stdf_byte_order_to_host(file, &(file->header.REC_LEN), sizeof(dtc_U2));
	file->pos = lseek(file->fd, 0, SEEK_CUR);
	file->rec_end = file->pos + file->header.REC_LEN;

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
		case REC_TSR: rec = (rec_unknown*)stdf_read_rec_tsr(file); break;
		case REC_PTR: rec = (rec_unknown*)stdf_read_rec_ptr(file); break;
/*
		case REC_MPR:
		case REC_FTR:
*/
		case REC_BPS: rec = (rec_unknown*)stdf_read_rec_bps(file); break;
		case REC_EPS: rec = (rec_unknown*)stdf_read_rec_eps(file); break;
/*
		case REC_GDR:
*/
		case REC_DTR: rec = (rec_unknown*)stdf_read_rec_dtr(file); break;
		default:
			rec = stdf_read_rec_unknown(file, &(file->header));
			file->header.REC_TYP = REC_TYP_UNKNOWN;
			file->header.REC_SUB = REC_SUB_UNKNOWN;
			break;
	}
	memcpy(&(rec->header), &(file->header), sizeof(rec_header));

	return rec;
}
