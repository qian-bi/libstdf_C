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
/* for malloc(3) */
#include <stdlib.h>
/* for memcpy(3) */
#include <string.h>

void __byte_order_change(int in_byte_order, int out_byte_order, byte_t *in, int len)
{
	if (in_byte_order == out_byte_order || len == 1)
		return;
	if (in_byte_order == PDP_ENDIAN) {
		fprintf(stderr, "__byte_order_change(): PDP_ENDIAN is not implemented\n");
		return;
	}

	switch (len) {
		case 2:	*((uint16_t*)in) = bswap_16(*((uint16_t*)in)); break;
		case 4:	*((uint32_t*)in) = bswap_32(*((uint32_t*)in)); break;
		case 8:	*((uint64_t*)in) = bswap_64(*((uint64_t*)in)); break;
		default:
			fprintf(stderr, "__byte_order_change(): byte len of %i has no implementation\n", len);
	}
}

void _stdf_read_dtc_Cx(stdf_file *f, dtc_Cn *Cn, int len)
{
	(*Cn) = (dtc_Cn)malloc(sizeof(dtc_C1) * 1 + len + 1);
	(*Cn)[0] = len;
	memset((*Cn)+1, 0x00, 1 + len + 1);
	if (f->rec_pos == f->rec_end) {
		return;
	}
	memcpy((*Cn)+1, f->rec_pos, len);
	f->rec_pos += len;
}

void _stdf_read_dtc_Cn(stdf_file *f, dtc_Cn *Cn)
{
	unsigned char len;
	if (f->rec_pos == f->rec_end) {
		(*Cn) = (dtc_Cn)malloc(sizeof(dtc_C1) * 1);
		(*Cn)[0] = 0;
		return;
	}
	len = f->rec_pos[0];
	f->rec_pos++;
	(*Cn) = (dtc_Cn)malloc(sizeof(dtc_C1) * len + 2);
	(*Cn)[0] = len;
	memcpy((*Cn)+1, f->rec_pos, len);
	f->rec_pos += len;
	(*Cn)[len+1] = '\0';
}

void _stdf_read_dtc_Dn(stdf_file *f, dtc_Dn *Dn)
{
	dtc_U2 bit_cnt;
	unsigned int len;

	if (f->rec_pos == f->rec_end) {
		(*Dn) = (dtc_Dn)malloc(2);
		((dtc_U2*)(*Dn))[0] = 0;
		return;
	}

	memcpy(&bit_cnt, f->rec_pos, 2);
	f->rec_pos += 2;
	_stdf_byte_order_to_host(f, &bit_cnt, 2);

	len = bit_cnt / (sizeof(dtc_B1) * 8);
	if (bit_cnt % 8) ++len;
	(*Dn) = (dtc_Dn)malloc(len + 3);
	((dtc_U2*)(*Dn))[0] = bit_cnt;
	memcpy(((byte_t*)(*Dn))+2, f->rec_pos, len);
	f->rec_pos += len;
	((byte_t*)(*Dn))[len+2] = 0x00;
}

void _stdf_read_dtc_xN1(stdf_file *f, dtc_xN1 *xN1, dtc_U2 cnt)
{
	unsigned int len = cnt / 2 + cnt % 2;
	(*xN1) = (dtc_xN1)malloc(len);
	memcpy((*xN1), f->rec_pos, len);
	f->rec_pos += len;
}

void __stdf_read_dtc_x(stdf_file *f, void *in, dtc_U2 cnt, int in_size)
{
	/* maybe be rewritten to work in a for loop so we can do byte swapping ? */
/*
	in = malloc(in_size * cnt);
	memcpy(in, f->rec_pos, in_size * cnt);
*/
	f->rec_pos += in_size * cnt;
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
