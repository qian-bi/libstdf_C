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
	if (in_byte_order != LITTLE_ENDIAN && in_byte_order != BIG_ENDIAN) {
		fprintf(stderr, "__byte_order_change(): byte order %i is not implemented\n", in_byte_order);
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
	/* does this even work ?
	   need a file with a PLR record in it to test ... */
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
	/* does this even work ?
	   need a file with a MPR or FTR record in it to test ... */
	unsigned int len = cnt / 2 + cnt % 2;
	(*xN1) = (dtc_xN1)malloc(len);
	memcpy((*xN1), f->rec_pos, len);
	f->rec_pos += len;
}

/*
TODO: is there any way i can make this not frickin ugly ?

void _stdf_read_dtc_xU1(stdf_file *f, dtc_xU1 *xU1, dtc_U2 cnt)
{
	int i;
	if (cnt == 0) {
		xU1 = NULL;
		return;
	}

	(*xU1) = (dtc_xU1)malloc(sizeof(dtc_U1) * cnt);
	for (i=0; i<cnt; ++i)
		_stdf_read_dtc_U1(f, &((*xU1)[i]));
}
*/
#define	__stdf_read_dtc_x(bFunc, xFunc, bType, xType) \
	void bFunc(stdf_file *f, xType *x, dtc_U2 cnt) \
	{ \
		int i; \
		if (cnt == 0) { \
			(*x) = NULL; \
			return; \
		} \
		(*x) = (xType)malloc(sizeof(bType) * cnt); \
		for (i=0; i<cnt; ++i) \
			xFunc(f, &((*x)[i])); \
	}
__stdf_read_dtc_x(_stdf_read_dtc_xU1,  _stdf_read_dtc_U1, dtc_U1, dtc_xU1)
__stdf_read_dtc_x(_stdf_read_dtc_xU2,  _stdf_read_dtc_U2, dtc_U2, dtc_xU2)
__stdf_read_dtc_x(_stdf_read_dtc_xR4,  _stdf_read_dtc_R4, dtc_R4, dtc_xR4)

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

char* stdf_get_Vn_name(int type)
{
	static char name[3];
	switch (type) {
		case GDR_B0: memcpy(name, "B0", 2); break;
		case GDR_U1: memcpy(name, "U1", 2); break;
		case GDR_U2: memcpy(name, "U2", 2); break;
		case GDR_U4: memcpy(name, "U4", 2); break;
		case GDR_I1: memcpy(name, "I1", 2); break;
		case GDR_I2: memcpy(name, "I2", 2); break;
		case GDR_I4: memcpy(name, "I4", 2); break;
		case GDR_R4: memcpy(name, "R4", 2); break;
		case GDR_R8: memcpy(name, "R8", 2); break;
		case GDR_Cn: memcpy(name, "Cn", 2); break;
		case GDR_Bn: memcpy(name, "Bn", 2); break;
		case GDR_Dn: memcpy(name, "Dn", 2); break;
		case GDR_N1: memcpy(name, "N1", 2); break;
	}
	name[2] = '\0';
	return name;
}

void _stdf_read_dtc_Vn(stdf_file *f, dtc_Vn *pVn, dtc_U2 cnt)
{
	dtc_Vn Vn;

	if (cnt == 0) {
		pVn = NULL;
		return;
	}

	(*pVn) = (dtc_Vn)malloc(sizeof(dtc_Vn_ele) * cnt);
	Vn = *pVn;
	while (cnt-- > 0) {
		Vn->type = *(f->rec_pos);
		f->rec_pos++;
		switch (Vn->type) {
			case GDR_B0: {
				Vn->data = (void*)malloc(1);
				memset(Vn->data, 0x00, 1);
				break;
			}
			case GDR_U1: {
				Vn->data = (void*)malloc(sizeof(dtc_U1));
				_stdf_read_dtc_U1(f, ((dtc_U1*)Vn->data));
				break;
			}
			case GDR_U2: {
				Vn->data = (void*)malloc(sizeof(dtc_U2));
				_stdf_read_dtc_U2(f, ((dtc_U2*)Vn->data));
				break;
			}
			case GDR_U4: {
				Vn->data = (void*)malloc(sizeof(dtc_U4));
				_stdf_read_dtc_U4(f, ((dtc_U4*)Vn->data));
				break;
			}
			case GDR_I1: {
				Vn->data = (void*)malloc(sizeof(dtc_I1));
				_stdf_read_dtc_I1(f, ((dtc_I1*)Vn->data));
				break;
			}
			case GDR_I2: {
				Vn->data = (void*)malloc(sizeof(dtc_I2));
				_stdf_read_dtc_I2(f, ((dtc_I2*)Vn->data));
				break;
			}
			case GDR_I4: {
				Vn->data = (void*)malloc(sizeof(dtc_I4));
				_stdf_read_dtc_I4(f, ((dtc_I4*)Vn->data));
				break;
			}
			case GDR_R4: {
				Vn->data = (void*)malloc(sizeof(dtc_R4));
				_stdf_read_dtc_R4(f, ((dtc_R4*)Vn->data));
				break;
			}
			case GDR_R8: {
				Vn->data = (void*)malloc(sizeof(dtc_R8));
				_stdf_read_dtc_R8(f, ((dtc_R8*)Vn->data));
				break;
			}
			case GDR_Cn: {
				Vn->data = (void*)malloc(sizeof(dtc_Cn));
				_stdf_read_dtc_Cn(f, ((dtc_Cn*)Vn->data));
				break;
			}
			case GDR_Bn: {
				Vn->data = (void*)malloc(sizeof(dtc_Bn));
				_stdf_read_dtc_Bn(f, ((dtc_Bn*)Vn->data));
				break;
			}
			case GDR_Dn: {
				Vn->data = (void*)malloc(sizeof(dtc_Dn));
				_stdf_read_dtc_Dn(f, ((dtc_Dn*)Vn->data));
				break;
			}
			case GDR_N1: {
				Vn->data = (void*)malloc(sizeof(dtc_N1));
				_stdf_read_dtc_N1(f, ((dtc_N1*)Vn->data));
				break;
			}
			default: {
				fprintf(stderr, "_stdf_read_dtc_Vn(): unknown type '%i'\n", Vn->type);
				Vn->data = NULL;
			}
		}
		++Vn;
	}
}

void free_Vn(dtc_Vn Vn, dtc_U2 cnt)
{
	while (cnt-- > 0)
		free(Vn[cnt].data);
	free(Vn);
}
