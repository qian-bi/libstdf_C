/**
 * @file dtc.c
 * @brief Data type specific function implementations.
 * @internal
 */
/*
 * Copyright (C) 2004-2007 Mike Frysinger <vapier@gmail.com>
 * Released under the BSD license.  For more information,
 * please see: http://opensource.org/licenses/bsd-license.php
 */

#include <libstdf.h>
#include "dtc.h"
#include "rec.h"

void __byte_order_change(int in_byte_order, int out_byte_order, byte_t *in, int len)
{
	if (in_byte_order == out_byte_order || len == 1)
		return;
	if (in_byte_order != STDF_ENDIAN_LITTLE && in_byte_order != STDF_ENDIAN_BIG) {
		warnf("byte order %i is not implemented", in_byte_order);
		return;
	}

	switch (len) {
		case 2:	stdf_bswap_16(*((uint16_t*)in)); break;
		case 4:	stdf_bswap_32(*((uint32_t*)in)); break;
		case 8:	stdf_bswap_64(*((uint64_t*)in)); break;
		default:
			warnf("byte len of %i has no implementation", len);
	}
}

#define MAKE_BYTE_FUNC(DTC, DEFAULT) \
void _stdf_read_ ## DTC(stdf_file *f, stdf_##DTC *dtc) \
{ \
	if (f->rec_pos < f->rec_end) \
		*dtc = *f->rec_pos++; \
	else \
		*dtc = DEFAULT; \
}
MAKE_BYTE_FUNC(dtc_U1, 0)
MAKE_BYTE_FUNC(dtc_I1, 0)
MAKE_BYTE_FUNC(dtc_C1, ' ')
MAKE_BYTE_FUNC(dtc_B1, 0x00)
MAKE_BYTE_FUNC(dtc_N1, 0x00)
#undef MAKE_BYTE_FUNC

#define MAKE_NUM_FUNC(DTC) \
void _stdf_read_ ## DTC(stdf_file *f, stdf_##DTC *dtc) \
{ \
	if (f->rec_pos < f->rec_end) { \
		memcpy(dtc, f->rec_pos, sizeof(stdf_##DTC)); \
		f->rec_pos += sizeof(stdf_##DTC); \
	} else \
		*dtc = 0; \
	_stdf_byte_order_to_host(f, dtc, sizeof(stdf_##DTC)); \
}
MAKE_NUM_FUNC(dtc_U2)
MAKE_NUM_FUNC(dtc_U4)
MAKE_NUM_FUNC(dtc_I2)
MAKE_NUM_FUNC(dtc_I4)
MAKE_NUM_FUNC(dtc_R4)
MAKE_NUM_FUNC(dtc_R8)
#undef MAKE_NUM_FUNC

#ifdef STDF_VER3
void _stdf_read_dtc_Cx(stdf_file *f, stdf_dtc_Cn *Cn, int len)
{
	/* does this even work ?
	   need a file with a PLR record in it to test ... */
	(*Cn) = (stdf_dtc_Cn)calloc(len + 1, sizeof(stdf_dtc_C1));
	(*Cn)[0] = len;
	memset((*Cn)+1, 0x00, 1 + len + 1);
	if (f->rec_pos == f->rec_end) {
		return;
	}
	memcpy((*Cn)+1, f->rec_pos, len);
	f->rec_pos += len;
}
#endif

void _stdf_read_dtc_Cn(stdf_file *f, stdf_dtc_Cn *Cn)
{
	unsigned char len;
	if (f->rec_pos == f->rec_end) {
		(*Cn) = malloc(sizeof(stdf_dtc_C1));
		(*Cn)[0] = 0;
		return;
	}
	len = f->rec_pos[0];
	f->rec_pos++;
	(*Cn) = (stdf_dtc_Cn)calloc(len + 2, sizeof(stdf_dtc_C1));
	(*Cn)[0] = len;
	memcpy((*Cn)+1, f->rec_pos, len);
	f->rec_pos += len;
	(*Cn)[len+1] = '\0';
}

void _stdf_read_dtc_Bn(stdf_file *f, stdf_dtc_Bn *Bn)
{
	return _stdf_read_dtc_Cn(f, (stdf_dtc_Cn*)Bn);
}

void _stdf_read_dtc_Dn(stdf_file *f, stdf_dtc_Dn *Dn)
{
	stdf_dtc_U2 bit_cnt;
	unsigned int len;

	if (f->rec_pos == f->rec_end) {
		(*Dn) = malloc(2);
		((stdf_dtc_U2*)(*Dn))[0] = 0;
		return;
	}

	memcpy(&bit_cnt, f->rec_pos, 2);
	f->rec_pos += 2;
	_stdf_byte_order_to_host(f, &bit_cnt, 2);

	len = bit_cnt / (sizeof(stdf_dtc_B1) * 8);
	if (bit_cnt % 8) ++len;
	(*Dn) = malloc(len + 3);
	((stdf_dtc_U2*)(*Dn))[0] = bit_cnt;
	memcpy(((byte_t*)(*Dn))+2, f->rec_pos, len);
	f->rec_pos += len;
	((byte_t*)(*Dn))[len+2] = 0x00;
}

void _stdf_read_dtc_xN1(stdf_file *f, stdf_dtc_xN1 *xN1, stdf_dtc_U2 cnt)
{
	stdf_dtc_U2 len = cnt / 2 + cnt % 2;
	(*xN1) = malloc(len);
	memcpy((*xN1), f->rec_pos, len);
	f->rec_pos += len;
}

#define MAKE_X_FUNC(DTC) \
void _stdf_read_dtc_x ## DTC(stdf_file *f, stdf_dtc_x ## DTC *x, stdf_dtc_U2 cnt) \
{ \
	int i; \
	if (cnt == 0) { \
		(*x) = NULL; \
		return; \
	} \
	(*x) = (stdf_dtc_x ## DTC)calloc(cnt, sizeof(stdf_dtc_ ## DTC)); \
	for (i=0; i<cnt; ++i) \
		_stdf_read_dtc_ ## DTC(f, &((*x)[i])); \
}
MAKE_X_FUNC(U1)
MAKE_X_FUNC(U2)
MAKE_X_FUNC(R4)
#undef MAKE_X_FUNC

void _stdf_read_dtc_xCn(stdf_file *f, stdf_dtc_xCn *xCn, stdf_dtc_U2 cnt)
{
	stdf_dtc_U2 i = 0;
	stdf_dtc_Cn Cn;
	(*xCn) = (stdf_dtc_xCn)calloc(cnt, sizeof(Cn));
	while (i < cnt) {
		_stdf_read_dtc_Cn(f, &Cn);
		(*xCn)[i++] = Cn;
	}
}

void free_xCn(stdf_dtc_xCn xCn, stdf_dtc_U2 cnt)
{
	while (cnt-- > 0)
		free(xCn[cnt]);
	free(xCn);
}

void stdf_get_Vn_name_r(int type, char *buf)
{
	switch (type) {
		case STDF_GDR_B0: memcpy(buf, "B0", 2); break;
		case STDF_GDR_U1: memcpy(buf, "U1", 2); break;
		case STDF_GDR_U2: memcpy(buf, "U2", 2); break;
		case STDF_GDR_U4: memcpy(buf, "U4", 2); break;
		case STDF_GDR_I1: memcpy(buf, "I1", 2); break;
		case STDF_GDR_I2: memcpy(buf, "I2", 2); break;
		case STDF_GDR_I4: memcpy(buf, "I4", 2); break;
		case STDF_GDR_R4: memcpy(buf, "R4", 2); break;
		case STDF_GDR_R8: memcpy(buf, "R8", 2); break;
		case STDF_GDR_Cn: memcpy(buf, "Cn", 2); break;
		case STDF_GDR_Bn: memcpy(buf, "Bn", 2); break;
		case STDF_GDR_Dn: memcpy(buf, "Dn", 2); break;
		case STDF_GDR_N1: memcpy(buf, "N1", 2); break;
	}
	buf[2] = '\0';
}
char* stdf_get_Vn_name(int type)
{
	static char name[3];
	stdf_get_Vn_name_r(type, name);
	return name;
}

void _stdf_read_dtc_Vn(stdf_file *f, stdf_dtc_Vn *pVn, stdf_dtc_U2 cnt)
{
	stdf_dtc_Vn Vn;

	if (cnt == 0) {
		pVn = NULL;
		return;
	}

#define DO_VN(DTC) \
		Vn->data = malloc(sizeof(stdf_##DTC)); \
		_stdf_read_ ## DTC(f, ((stdf_##DTC*)Vn->data));

	(*pVn) = (stdf_dtc_Vn)calloc(cnt, sizeof(stdf_dtc_Vn_ele));
	Vn = *pVn;
	while (cnt-- > 0) {
		Vn->type = *(f->rec_pos);
		f->rec_pos++;
		switch (Vn->type) {
			case STDF_GDR_B0: DO_VN(dtc_B1); break;
			case STDF_GDR_U1: DO_VN(dtc_U1); break;
			case STDF_GDR_U2: DO_VN(dtc_U2); break;
			case STDF_GDR_U4: DO_VN(dtc_U4); break;
			case STDF_GDR_I1: DO_VN(dtc_I1); break;
			case STDF_GDR_I2: DO_VN(dtc_I2); break;
			case STDF_GDR_I4: DO_VN(dtc_I4); break;
			case STDF_GDR_R4: DO_VN(dtc_R4); break;
			case STDF_GDR_R8: DO_VN(dtc_R8); break;
			case STDF_GDR_Cn: DO_VN(dtc_Cn); break;
			case STDF_GDR_Bn: DO_VN(dtc_Bn); break;
			case STDF_GDR_Dn: DO_VN(dtc_Dn); break;
			case STDF_GDR_N1: DO_VN(dtc_N1); break;
			default: {
				warnf("unknown type '%i'", Vn->type);
				Vn->data = NULL;
			}
		}
		++Vn;
	}
#undef DO_VN
}

void free_Vn(stdf_dtc_Vn Vn, stdf_dtc_U2 cnt)
{
#define DO_FREE(DTC) free(*((DTC*)Vn[cnt].data))
	while (cnt-- > 0) {
		switch (Vn[cnt].type) {
			case STDF_GDR_Cn: DO_FREE(stdf_dtc_Cn); break;
			case STDF_GDR_Bn: DO_FREE(stdf_dtc_Bn); break;
			case STDF_GDR_Dn: DO_FREE(stdf_dtc_Dn); break;
			default:;
		}
		free(Vn[cnt].data);
	}
	free(Vn);
#undef DO_FREE
}
