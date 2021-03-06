/**
 * @file dtc.h
 * @brief Data type specific function prototypes.
 * @internal
 */
/*
 * Copyright (C) 2004-2007 Mike Frysinger <vapier@gmail.com>
 * Copyright (C) 2017 Stefan Brandner <stefan.brandner@gmx.at>
 * Released under the BSD license.  For more information,
 * please see: http://opensource.org/licenses/bsd-license.php
 */

#ifndef _LIBSTDF_DTC_H
#define _LIBSTDF_DTC_H

#include <libstdf.h>

extern void __byte_order_change(int, int, byte_t*, int) stdf_attribute_hidden;
#define	__byte_order_to_host(b,p,l) __byte_order_change(b, STDF_ENDIAN_HOST, p, l)
#define	__byte_order_to_src(b,p,l) __byte_order_change(STDF_ENDIAN_HOST, b, p, l)
#define	_stdf_byte_order_to_host(f,v,l) __byte_order_to_host(f->byte_order, (byte_t*)(v), l)
#define	_stdf_byte_order_to_src(f,v,l) __byte_order_to_src(f->byte_order, (byte_t*)(v), l)

#ifdef	STDF_VER3
extern void _stdf_read_dtc_Cx(stdf_file*, stdf_dtc_Cn*, int) stdf_attribute_hidden;
#endif
extern void _stdf_read_dtc_Cn(stdf_file*, stdf_dtc_Cn*) stdf_attribute_hidden;
extern void _stdf_read_dtc_Sn(stdf_file*, stdf_dtc_Sn*) stdf_attribute_hidden;
extern void _stdf_read_dtc_Bn(stdf_file*, stdf_dtc_Bn*) stdf_attribute_hidden;
extern void _stdf_read_dtc_Dn(stdf_file*, stdf_dtc_Dn*) stdf_attribute_hidden;

extern void _stdf_read_dtc_C1(stdf_file*, stdf_dtc_C1*) stdf_attribute_hidden;
extern void _stdf_read_dtc_B1(stdf_file*, stdf_dtc_B1*) stdf_attribute_hidden;
extern void _stdf_read_dtc_N1(stdf_file*, stdf_dtc_N1*) stdf_attribute_hidden;
extern void _stdf_read_dtc_U1(stdf_file*, stdf_dtc_U1*) stdf_attribute_hidden;
extern void _stdf_read_dtc_U2(stdf_file*, stdf_dtc_U2*) stdf_attribute_hidden;
extern void _stdf_read_dtc_U4(stdf_file*, stdf_dtc_U4*) stdf_attribute_hidden;
extern void _stdf_read_dtc_U8(stdf_file*, stdf_dtc_U8*) stdf_attribute_hidden;
extern void _stdf_read_dtc_I1(stdf_file*, stdf_dtc_I1*) stdf_attribute_hidden;
extern void _stdf_read_dtc_I2(stdf_file*, stdf_dtc_I2*) stdf_attribute_hidden;
extern void _stdf_read_dtc_I4(stdf_file*, stdf_dtc_I4*) stdf_attribute_hidden;
extern void _stdf_read_dtc_R4(stdf_file*, stdf_dtc_R4*) stdf_attribute_hidden;
extern void _stdf_read_dtc_R8(stdf_file*, stdf_dtc_R8*) stdf_attribute_hidden;

extern void _stdf_read_dtc_xN1(stdf_file*, stdf_dtc_xN1*, stdf_dtc_U2) stdf_attribute_hidden;
extern void _stdf_read_dtc_xU1(stdf_file*, stdf_dtc_xU1*, stdf_dtc_U2) stdf_attribute_hidden;
extern void _stdf_read_dtc_xU2(stdf_file*, stdf_dtc_xU2*, stdf_dtc_U2) stdf_attribute_hidden;
extern void _stdf_read_dtc_xU4(stdf_file*, stdf_dtc_xU4*, stdf_dtc_U2) stdf_attribute_hidden;
extern void _stdf_read_dtc_xU8(stdf_file*, stdf_dtc_xU8*, stdf_dtc_U2) stdf_attribute_hidden;
extern void _stdf_read_dtc_xUf(stdf_file*, stdf_dtc_xUf*, stdf_dtc_U2, stdf_dtc_U1) stdf_attribute_hidden;
extern void _stdf_read_dtc_xR4(stdf_file*, stdf_dtc_xR4*, stdf_dtc_U2) stdf_attribute_hidden;
#define	free_x(x) if (x) free(x)
#define	free_xU1(x) free_x(x)
#define	free_xU2(x) free_x(x)
#define	free_xU4(x) free_x(x)
#define	free_xU8(x) free_x(x)
#define	free_xUf(x) free_x(x)
#define	free_xR4(x) free_x(x)

extern void _stdf_read_dtc_xCn(stdf_file*, stdf_dtc_xCn*, stdf_dtc_U2) stdf_attribute_hidden;
extern void free_xCn(stdf_dtc_xCn, stdf_dtc_U2) stdf_attribute_hidden;

extern void _stdf_read_dtc_xSn(stdf_file*, stdf_dtc_xSn*, stdf_dtc_U2) stdf_attribute_hidden;
extern void free_xSn(stdf_dtc_xSn, stdf_dtc_U2) stdf_attribute_hidden;

extern void _stdf_read_dtc_Vn(stdf_file*, stdf_dtc_Vn*, stdf_dtc_U2) stdf_attribute_hidden;
extern void free_Vn(stdf_dtc_Vn, stdf_dtc_U2) stdf_attribute_hidden;

#define _stdf_write_byte(file, byte) \
	*file->_write_pos++ = byte
#define _stdf_write_nbytes(file, inbuf, cnt) \
	do { \
		memcpy(file->_write_pos, inbuf, cnt); \
		file->_write_pos += cnt; \
	} while (0)
#define _stdf_write_cnt_nbytes(file, inbuf, cnt) \
	do { \
		_stdf_write_byte(file, cnt); \
		if (cnt) _stdf_write_nbytes(file, inbuf, cnt); \
	} while (0)

#define __stdf_write_even_bytes(type, file, bytes, even) \
	do { \
		type mapme = bytes; \
		_stdf_byte_order_to_src(file, &mapme, even); \
		unsigned char *map = (unsigned char*)&mapme; \
		_stdf_write_nbytes(file, map, even); \
	} while (0)
#define _stdf_write_2bytes(file, bytes)       __stdf_write_even_bytes(uint16_t, file, bytes, 2)
#define _stdf_write_4bytes(file, bytes)       __stdf_write_even_bytes(uint32_t, file, bytes, 4)
#define _stdf_write_8bytes(file, bytes)       __stdf_write_even_bytes(uint64_t, file, bytes, 8)
#define _stdf_write_4bytes_float(file, bytes) __stdf_write_even_bytes(float, file, bytes, 4)
#define _stdf_write_8bytes_float(file, bytes) __stdf_write_even_bytes(double, file, bytes, 8)

#define _stdf_write_dtc_C1(file, C1) _stdf_write_byte(file, C1)
#define _stdf_write_dtc_B1(file, B1) _stdf_write_byte(file, B1)
#define _stdf_write_dtc_N1(file, N1) _stdf_write_byte(file, N1)
#define _stdf_write_dtc_U1(file, U1) _stdf_write_byte(file, U1)
#define _stdf_write_dtc_I1(file, I1) _stdf_write_byte(file, I1)

#define _stdf_write_dtc_U2(file, U2) _stdf_write_2bytes(file, U2)
#define _stdf_write_dtc_I2(file, I2) _stdf_write_2bytes(file, I2)

#define _stdf_write_dtc_U4(file, U4) _stdf_write_4bytes(file, U4)
#define _stdf_write_dtc_I4(file, I4) _stdf_write_4bytes(file, I4)
#define _stdf_write_dtc_R4(file, R4) _stdf_write_4bytes_float(file, R4)

#define _stdf_write_dtc_U8(file, U8) _stdf_write_8bytes(file, U8)
#define _stdf_write_dtc_R8(file, R8) _stdf_write_8bytes_float(file, R8)

#define _stdf_qwrite_dtc_header(file, REC_LEN, REC_TYP, REC_SUB) \
	do { \
		_stdf_write_dtc_U2(file, REC_LEN); \
		_stdf_write_dtc_U1(file, REC_TYP); \
		_stdf_write_dtc_U1(file, REC_SUB); \
	} while (0)
#define _stdf_write_dtc_header(file, header) \
	_stdf_qwrite_dtc_header(file, (header)->REC_LEN, (header)->REC_TYP, (header)->REC_SUB)

#define _stdf_write_dtc_Cn(file, Cn) _stdf_write_cnt_nbytes(file, Cn+1, (Cn ? (stdf_dtc_U1)*Cn : 0))
#define _stdf_write_dtc_Sn(file, Sn) \
	do { \
		if (Sn) {\
			stdf_dtc_U2 cnt = *((stdf_dtc_U2*)Sn); \
			_stdf_write_dtc_U2(file, cnt); \
 			_stdf_write_nbytes(file, Sn+2, cnt); \
		} \
	} while (0)
#define _stdf_write_dtc_Bn(file, Bn) _stdf_write_dtc_Cn(file, Bn)
#define _stdf_write_dtc_Dn(file, Dn) \
	do { \
		if (Dn) { \
			stdf_dtc_U2 cnt = *((stdf_dtc_U2*)Dn); \
			_stdf_write_dtc_U2(file, cnt); \
			cnt/=8; \
			if (*((stdf_dtc_U2*)Dn) % 8) ++cnt; \
			_stdf_write_nbytes(file, Dn+2, cnt); \
		} \
	} while (0)

static inline void _stdf_write_dtc_Vn(stdf_file *file, stdf_dtc_Vn Vn, stdf_dtc_U2 cnt)
{
#define DO_VN(DTC) _stdf_write_dtc_##DTC(file, *(stdf_dtc_##DTC*)Vn->data)
	while (cnt-- > 0) {
        	_stdf_write_byte(file, Vn->type);
		switch (Vn->type) {
			case STDF_GDR_B0: break;
			case STDF_GDR_U1: DO_VN(U1); break;
			case STDF_GDR_U2: DO_VN(U2); break;
			case STDF_GDR_U4: DO_VN(U4); break;
			case STDF_GDR_I1: DO_VN(I1); break;
			case STDF_GDR_I2: DO_VN(I2); break;
			case STDF_GDR_I4: DO_VN(I4); break;
			case STDF_GDR_R4: DO_VN(R4); break;
			case STDF_GDR_R8: DO_VN(R8); break;
			case STDF_GDR_Cn: DO_VN(Cn); break;
			case STDF_GDR_Bn: DO_VN(Bn); break;
			case STDF_GDR_Dn: DO_VN(Dn); break;
			case STDF_GDR_N1: DO_VN(N1); break;
			default:          warnf("unknown type '%i'", Vn->type); break;
		}
		++Vn;
	}
#undef DO_VN
}

#define _stdf_write_dtc_x(file, x, cnt, func) \
	do { \
		stdf_dtc_U2 i; \
		for (i=0; i<cnt; ++i) \
			func(file, x[i]); \
	} while (0)

#define _stdf_write_dtc_xU1(file, xU1, cnt) _stdf_write_nbytes(file, xU1, cnt)
#define _stdf_write_dtc_xU2(file, xU2, cnt) _stdf_write_dtc_x(file, xU2, cnt, _stdf_write_dtc_U2)
#define _stdf_write_dtc_xU4(file, xU4, cnt) _stdf_write_dtc_x(file, xU4, cnt, _stdf_write_dtc_U4)
#define _stdf_write_dtc_xU8(file, xU8, cnt) _stdf_write_dtc_x(file, xU8, cnt, _stdf_write_dtc_U8)
#define _stdf_write_dtc_xUf(file, xUf, cnt, type) \
do { \
         switch (type) { \
                case 0: break; \
		case 1: _stdf_write_nbytes(file, xUf, cnt); break; \
		case 2: { \
				stdf_dtc_U2 i; \
				for (i=0; i<cnt; ++i) \
					_stdf_write_dtc_U2(file, ((stdf_dtc_U2*)xUf)[i]); \
				break; \
			} \
                case 4: { \
				stdf_dtc_U2 i; \
				for (i=0; i<cnt; ++i) \
					_stdf_write_dtc_U4(file, ((stdf_dtc_U4*)xUf)[i]); \
				break; \
			} \
                case 8: { \
				stdf_dtc_U2 i; \
				for (i=0; i<cnt; ++i) \
					_stdf_write_dtc_U8(file, ((stdf_dtc_U8*)xUf)[i]); \
				break; \
			} \
                default:	warnf("write xUf unknown type '%i'", type); \
	} \
} while(0)
#define _stdf_write_dtc_xN1(file, xN1, cnt) _stdf_write_nbytes(file, xN1, ((cnt/2)+(cnt%2)))
#define _stdf_write_dtc_xR4(file, xR4, cnt) _stdf_write_dtc_x(file, xR4, cnt, _stdf_write_dtc_R4)
#define _stdf_write_dtc_xCn(file, xCn, cnt) _stdf_write_dtc_x(file, xCn, cnt, _stdf_write_dtc_Cn)
#define _stdf_write_dtc_xSn(file, xSn, cnt) _stdf_write_dtc_x(file, xSn, cnt, _stdf_write_dtc_Sn)
#define _stdf_write_dtc_xBn(file, xBn, cnt) _stdf_write_dtc_xCn(file, xBn, cnt)
#define _stdf_write_dtc_Cx(file, Cx, cnt)   _stdf_write_nbytes(file, Cx, cnt)

#endif /* _LIBSTDF_DTC_H */
