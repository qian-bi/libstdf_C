/* libstdf.c
 * Copyright (C) 2004 Mike Frysinger <vapier@gmail.com>
 * Released under the BSD license.  For more information,
 * please see: http://opensource.org/licenses/bsd-license.php
 *
 * $Header$
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
/* for close(2) / read(2) */
#include <unistd.h>
/* for malloc(3) */
#include <stdlib.h>
/* for memcpy(3) */
#include <string.h>
/* for BYTE_ORDER defines */
#include <endian.h>

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
	f->__data = NULL;
#ifndef __STDF_READ_ONE_RECORD
	f->__data_end = NULL;
#endif
	f->rec_pos = NULL;
	f->rec_end = NULL;
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
#ifndef __STDF_READ_ONE_RECORD
	free(file->__data);
#endif
	free(file);
	return ret;
}

rec_unknown* stdf_read_record(stdf_file *file)
{
	rec_unknown *rec = NULL;

#ifdef __STDF_READ_ONE_RECORD
	/* read the record header to find out how big this next record is */
	if (read(file->fd, &(file->header), 4) != 4)
		return rec;
	_stdf_byte_order_to_host(file, &(file->header.REC_LEN), sizeof(dtc_U2));
	/* buffer the whole record in memory */
	file->__data = (void*)malloc(file->header.REC_LEN);
	read(file->fd, file->__data, file->header.REC_LEN);
	file->rec_pos = file->__data;
	file->rec_end = file->rec_pos + file->header.REC_LEN;
#else
	/* initialize the buffer since we haven't read anything yet */
	if (file->__data == NULL) {
		file->__data = (void*)malloc(__STDF_READ_SIZE);
		file->__data_end = file->__data + __STDF_READ_SIZE;
		file->rec_end = file->__data;
	}
	/* see if we've read everything there is to read */
	if (file->rec_pos >= file->__data_end)
		return rec;

	/* update the position to the end of the last record and grab the next
	   header ... this way we can screw up the reading of one record and
	   not have it affect any subsequent record reads */
	file->rec_pos = file->rec_end;
	memcpy(&(file->header), file->rec_pos, 4);
	_stdf_byte_order_to_host(file, &(file->header.REC_LEN), sizeof(dtc_U2));
	file->rec_pos += 4;
	file->rec_end = file->rec_pos + file->header.REC_LEN;

	/* make sure existing buffer contains at least the next record */
	if (file->__data_end < file->rec_end + 4) {
		void *tmp;
		long num_bytes_to_save, new_data_size;

		num_bytes_to_save = file->__data_end - file->rec_pos;
		new_data_size = __STDF_READ_SIZE + num_bytes_to_save;
		if (__STDF_READ_SIZE < file->header.REC_LEN)
			new_data_size += file->header.REC_LEN;

		tmp = (void*)malloc(new_data_size);
		memcpy(tmp, file->rec_pos, num_bytes_to_save);
		free(file->__data);
		file->__data = tmp;
		tmp += num_bytes_to_save;
		if (read(file->fd, tmp, (new_data_size - num_bytes_to_save)) == 0)
			return rec;

		file->__data_end = file->__data + new_data_size;
		file->rec_pos = file->__data;
		file->rec_end = file->rec_pos + file->header.REC_LEN;
	}
#endif

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
		case REC_MPR: rec = (rec_unknown*)stdf_read_rec_mpr(file); break;
		case REC_FTR: rec = (rec_unknown*)stdf_read_rec_ftr(file); break;
		case REC_BPS: rec = (rec_unknown*)stdf_read_rec_bps(file); break;
		case REC_EPS: rec = (rec_unknown*)stdf_read_rec_eps(file); break;
		case REC_GDR: rec = (rec_unknown*)stdf_read_rec_gdr(file); break;
		case REC_DTR: rec = (rec_unknown*)stdf_read_rec_dtr(file); break;
		default:
			rec = stdf_read_rec_unknown(file);
			file->header.REC_TYP = REC_TYP_UNKNOWN;
			file->header.REC_SUB = REC_SUB_UNKNOWN;
			break;
	}

	memcpy(&(rec->header), &(file->header), sizeof(rec_header));
#ifdef __STDF_READ_ONE_RECORD
	free(file->__data);
#endif

	return rec;
}
