/* dtc.c
 * Copyright (C) 2004 Mike Frysinger <vapier@gmail.com>
 * Released under the BSD license.  For more information,
 * please see: http://opensource.org/licenses/bsd-license.php
 */

#include <libstdf.h>

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
		(*Cn) = (dtc_Cn)malloc(sizeof(char) * 1);
		(*Cn)[0] = 0;
		return;
	}
	f->pos += read(f->fd, &len, 1);
	(*Cn) = (dtc_Cn)malloc(sizeof(char) * len + 2);
	(*Cn)[0] = len;
	f->pos += read(f->fd, (*Cn)+1, len);
	(*Cn)[len+1] = '\0';
}

void __stdf_read_dtc_x(stdf_file *f, void *in, dtc_U2 cnt, int in_size)
{
	in = malloc(in_size * cnt);
	f->pos += read(f->fd, in, in_size * cnt);
}
