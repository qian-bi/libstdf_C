/**
 * @file dtc.c
 * @brief Data type specific function implementations.
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

void __byte_order_change(int in_byte_order, int out_byte_order, byte_t *in, int len)
{
	if (in_byte_order == out_byte_order || len == 1)
		return;
	if (in_byte_order == STDF_ENDIAN_HOST)
		if (out_byte_order != STDF_ENDIAN_LITTLE && out_byte_order != STDF_ENDIAN_BIG) {
			warnf("byte order %i is not implemented", out_byte_order);
            	return;
		}
		else
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
MAKE_NUM_FUNC(dtc_U8)
MAKE_NUM_FUNC(dtc_I2)
MAKE_NUM_FUNC(dtc_I4)
MAKE_NUM_FUNC(dtc_R4)
MAKE_NUM_FUNC(dtc_R8)
#undef MAKE_NUM_FUNC

#ifdef STDF_VER3
void _stdf_read_dtc_Cx(stdf_file *f, stdf_dtc_Cn *Cn, int len)
{
	(*Cn) = calloc(len + 1, sizeof(stdf_dtc_C1));
	(*Cn)[0] = len;
	memset((*Cn)+1, 0x00, 1 + len + 1);
	if (f->rec_pos == f->rec_end)
		return;
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
	(*Cn) = calloc(len + 2, sizeof(stdf_dtc_C1));
	(*Cn)[0] = len;
	memcpy((*Cn)+1, f->rec_pos, len);
	f->rec_pos += len;
	(*Cn)[len+1] = '\0';
}

void _stdf_read_dtc_Sn(stdf_file *f, stdf_dtc_Sn *Sn)
{
	unsigned int len;
	if ((f->rec_pos)+1 == f->rec_end) {
		(*Sn) = malloc(2);
		((stdf_dtc_U2*)(*Sn))[0] = 0;
		return;
	}

	memcpy(&len, f->rec_pos, 2);
	f->rec_pos += 2;
	_stdf_byte_order_to_host(f, &len, 2);

	(*Sn) = calloc(len + 3, sizeof(stdf_dtc_C1));
        ((stdf_dtc_U2*)(*Sn))[0] = len;
	memcpy((*Sn)+2, f->rec_pos, len);
	f->rec_pos += len;
	(*Sn)[len+2] = '\0';
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
	(*Dn) = malloc(len + 2);
	((stdf_dtc_U2*)(*Dn))[0] = bit_cnt;
	memcpy(((byte_t*)(*Dn))+2, f->rec_pos, len);
	f->rec_pos += len;
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
	(*x) = calloc(cnt, sizeof(stdf_dtc_ ## DTC)); \
	for (i = 0; i < cnt; ++i) \
		_stdf_read_dtc_ ## DTC(f, &((*x)[i])); \
}
MAKE_X_FUNC(U1)
MAKE_X_FUNC(U2)
MAKE_X_FUNC(U4)
MAKE_X_FUNC(U8)
MAKE_X_FUNC(R4)
#undef MAKE_X_FUNC

void _stdf_read_dtc_xUf(stdf_file *f, stdf_dtc_xUf *xUf, stdf_dtc_U2 cnt, stdf_dtc_U1 size)
{
        stdf_dtc_U2 i=0;

        if (cnt == 0 || size == 0) {
		(*xUf) = NULL;
                return;
        }
        switch (size) {
        	case 1: { 
                        (*xUf) = (void*)calloc(cnt, sizeof(stdf_dtc_U1));
                        for ( i = 0; i < cnt; i++)
				_stdf_read_dtc_U1(f, &((stdf_dtc_U1*)(*xUf))[i]);
			break;
		} 
        	case 2: { 
                        (*xUf) = calloc(cnt, sizeof(stdf_dtc_U2));
                        for ( i = 0; i < cnt; i++)
				_stdf_read_dtc_U2(f, &((stdf_dtc_U2*)(*xUf))[i]);
			break;
		} 
        	case 4: { 
                        (*xUf) = calloc(cnt, sizeof(stdf_dtc_U4));
                        for ( i = 0; i < cnt; i++)
				_stdf_read_dtc_U4(f, &((stdf_dtc_U4*)(*xUf))[i]);
			break;
		} 
        	case 8: { 
                        (*xUf) = calloc(cnt, sizeof(stdf_dtc_U8));
                        for ( i = 0; i < cnt; i++)
				_stdf_read_dtc_U8(f, &((stdf_dtc_U8*)(*xUf))[i]);
			break;
		} 
		default:
			warnf("illegal Uf size '%u'", size);
	}
}

void _stdf_read_dtc_xCn(stdf_file *f, stdf_dtc_xCn *xCn, stdf_dtc_U2 cnt)
{
	stdf_dtc_U2 i = 0;
	stdf_dtc_Cn Cn;
	(*xCn) = calloc(cnt, sizeof(Cn));
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

void _stdf_read_dtc_xSn(stdf_file *f, stdf_dtc_xSn *xSn, stdf_dtc_U2 cnt)
{
	stdf_dtc_U2 i = 0;
	stdf_dtc_Sn Sn;
	(*xSn) = calloc(cnt, sizeof(Sn));
	while (i < cnt) {
		_stdf_read_dtc_Sn(f, &Sn);
		(*xSn)[i++] = Sn;
	}
}

void free_xSn(stdf_dtc_xSn xSn, stdf_dtc_U2 cnt)
{
	while (cnt-- > 0)
		free(xSn[cnt]);
	free(xSn);
}

char* stdf_get_Vn_name(int type)
{
	static char name[3];

	switch (type) {
		case STDF_GDR_B0: memcpy(name, "B0", 2); break;
		case STDF_GDR_U1: memcpy(name, "U1", 2); break;
		case STDF_GDR_U2: memcpy(name, "U2", 2); break;
		case STDF_GDR_U4: memcpy(name, "U4", 2); break;
		case STDF_GDR_I1: memcpy(name, "I1", 2); break;
		case STDF_GDR_I2: memcpy(name, "I2", 2); break;
		case STDF_GDR_I4: memcpy(name, "I4", 2); break;
		case STDF_GDR_R4: memcpy(name, "R4", 2); break;
		case STDF_GDR_R8: memcpy(name, "R8", 2); break;
		case STDF_GDR_Cn: memcpy(name, "Cn", 2); break;
		case STDF_GDR_Bn: memcpy(name, "Bn", 2); break;
		case STDF_GDR_Dn: memcpy(name, "Dn", 2); break;
		case STDF_GDR_N1: memcpy(name, "N1", 2); break;
	}
	name[2] = '\0';
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

	(*pVn) = calloc(cnt, sizeof(stdf_dtc_Vn_ele));
	Vn = *pVn;
	while (cnt-- > 0) {
		Vn->type = *(f->rec_pos);
		f->rec_pos++;
		switch (Vn->type) {
			case STDF_GDR_B0: break;
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
