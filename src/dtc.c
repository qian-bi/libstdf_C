/* dtc.c
 * Copyright (C) 2004 Mike Frysinger <vapier@gmail.com>
 * Released under the BSD license.  For more information,
 * please see: http://opensource.org/licenses/bsd-license.php
 *
 * $Header$
 */

#include <libstdf.h>
#include "dtc.h"
#include "rec.h"

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
/* for bswap() functions */
#include <byteswap.h>

void __byte_order_change(int in_byte_order, int out_byte_order, byte_t *in, int len)
{
	if (in_byte_order == out_byte_order || len == 1)
		return;
	if (in_byte_order == PDP_ENDIAN) {
		fprintf(stderr, "__byte_order_change(): PDP_ENDIAN is not implemented\n");
		return;
	}

	switch (len) {
		case 2:	*((__uint16_t*)in) = bswap_16(*((__uint16_t*)in)); break;
		case 4:	*((__uint32_t*)in) = bswap_32(*((__uint32_t*)in)); break;
		case 8:	*((__uint64_t*)in) = bswap_64(*((__uint64_t*)in)); break;
		default:
			fprintf(stderr, "__byte_order_change(): byte len of %i has no implementation\n", len);
	}
}

void _stdf_read_dtc_Cn(stdf_file *f, dtc_Cn *Cn)
{
	unsigned char len;
	if (f->pos == f->rec_end) {
		(*Cn) = (dtc_Cn)malloc(sizeof(dtc_C1) * 1);
		(*Cn)[0] = 0;
		return;
	}
	f->pos += read(f->fd, &len, 1);
	(*Cn) = (dtc_Cn)malloc(sizeof(dtc_C1) * len + 2);
	(*Cn)[0] = len;
	f->pos += read(f->fd, (*Cn)+1, len);
	(*Cn)[len+1] = '\0';
}

void _stdf_read_dtc_Dn(stdf_file *f, dtc_Dn *Dn)
{
	dtc_U4 bit_cnt;
	unsigned int len;
	if (f->pos == f->rec_end) {
		(*Dn) = (dtc_Dn)malloc(2);
		((dtc_U4*)(*Dn))[0] = 0;
		return;
	}
	f->pos += read(f->fd, &bit_cnt, 2);
	len = bit_cnt / (sizeof(dtc_B1) * 8);
	if (bit_cnt % 8) ++len;
	(*Dn) = (dtc_Dn)malloc(len + 3);
	((dtc_U4*)(*Dn))[0] = bit_cnt;
	f->pos += read(f->fd, (*Dn)+2, len);
	(*Dn)[len+2] = 0x00;
}

void _stdf_read_dtc_xN1(stdf_file *f, dtc_xN1 *xN1, dtc_U2 cnt)
{
	unsigned int len = cnt / 2 + cnt % 2;
	(*xN1) = (dtc_xN1)malloc(len);
	f->pos += read(f->fd, (*xN1), len);
}

void __stdf_read_dtc_x(stdf_file *f, void *in, dtc_U2 cnt, int in_size)
{
	in = malloc(in_size * cnt);
	f->pos += read(f->fd, in, in_size * cnt);
}

void _stdf_read_dtc_xCn(stdf_file *f, dtc_xCn *xCn, dtc_U2 cnt)
{
	dtc_U2 i = 0;
	dtc_Cn Cn;
	(*xCn) = (dtc_xCn)malloc(sizeof(Cn) * cnt);
	while (i < cnt) {
		_stdf_read_dtc_Cn(f, &Cn);
		xCn[i++] = &Cn;
	}
}

void free_xCn(dtc_xCn xCn, dtc_U2 cnt)
{
	while (cnt-- > 0)
		free(xCn[cnt]);
	free(xCn);
}
