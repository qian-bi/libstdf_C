/**
 * @file dtc.h
 * @brief Data type specific function prototypes.
 * @internal
 */
/*
 * Copyright (C) 2004-2005 Mike Frysinger <vapier@gmail.com>
 * Released under the BSD license.  For more information,
 * please see: http://opensource.org/licenses/bsd-license.php
 *
 * $Header$
 */

#ifndef _LIBSTDF_DTC_H
#define _LIBSTDF_DTC_H

#include <libstdf.h>

extern void __byte_order_change(int, int, byte_t*, int);
#define	__byte_order_to_host(b,p,l) __byte_order_change(b, __STDF_HOST_BYTE_ORDER, p, l)
#define	__byte_order_to_src(b,p,l) __byte_order_change(__STDF_HOST_BYTE_ORDER, b, p, l)
#define	_stdf_byte_order_to_host(f,v,l) __byte_order_to_host(f->byte_order, (byte_t*)(v), l)
#define	_stdf_byte_order_to_src(f,v,l) __byte_order_to_src(f->byte_order, (byte_t*)(v), l)

#ifdef	STDF_VER3
extern void _stdf_read_dtc_Cx(stdf_file*, dtc_Cn*, int);
#endif
extern void _stdf_read_dtc_Cn(stdf_file*, dtc_Cn*);
extern void _stdf_read_dtc_Bn(stdf_file*, dtc_Bn*);
extern void _stdf_read_dtc_Dn(stdf_file*, dtc_Dn*);

extern void _stdf_read_dtc_C1(stdf_file*, dtc_C1*);
extern void _stdf_read_dtc_B1(stdf_file*, dtc_B1*);
extern void _stdf_read_dtc_N1(stdf_file*, dtc_N1*);
extern void _stdf_read_dtc_U1(stdf_file*, dtc_U1*);
extern void _stdf_read_dtc_U2(stdf_file*, dtc_U2*);
extern void _stdf_read_dtc_U4(stdf_file*, dtc_U4*);
extern void _stdf_read_dtc_I1(stdf_file*, dtc_I1*);
extern void _stdf_read_dtc_I2(stdf_file*, dtc_I2*);
extern void _stdf_read_dtc_I4(stdf_file*, dtc_I4*);
extern void _stdf_read_dtc_R4(stdf_file*, dtc_R4*);
extern void _stdf_read_dtc_R8(stdf_file*, dtc_R8*);

extern void _stdf_read_dtc_xN1(stdf_file*, dtc_xN1*, dtc_U2);
extern void _stdf_read_dtc_xU1(stdf_file*, dtc_xU1*, dtc_U2);
extern void _stdf_read_dtc_xU2(stdf_file*, dtc_xU2*, dtc_U2);
extern void _stdf_read_dtc_xR4(stdf_file*, dtc_xR4*, dtc_U2);
#define	free_x(x) if (x) free(x)
#define	free_xU1(x) free_x(x)
#define	free_xU2(x) free_x(x)
#define	free_xR4(x) free_x(x)

extern void _stdf_read_dtc_xCn(stdf_file*, dtc_xCn*, dtc_U2);
extern void free_xCn(dtc_xCn, dtc_U2);

extern void _stdf_read_dtc_Vn(stdf_file*, dtc_Vn*, dtc_U2);
extern void free_Vn(dtc_Vn, dtc_U2);

#define _stdf_write_nbytes(file, inbuf, cnt, outbuf) \
	do { \
		memcpy(outbuf, inbuf, cnt); \
		outbuf += cnt; \
	} while (0)
#define _stdf_write_cnt_nbytes(file, inbuf, cnt, outbuf) \
	do { \
		*outbuf++ = cnt; \
		_stdf_write_nbytes(file, inbuf, cnt, outbuf); \
	} while (0)

#define _stdf_write_byte(file, byte, buffer) \
	*buffer++ = byte
#define _stdf_write_2bytes(file, bytes, buffer) \
	do { \
		uint16_t mapme = bytes; \
		_stdf_byte_order_to_src(file, &mapme, 2); \
		uchar *map = (uchar*)&mapme; \
		_stdf_write_byte(file, map[0], buffer); \
		_stdf_write_byte(file, map[1], buffer); \
	} while (0)

#define __stdf_write_4bytes(type, file, bytes, buffer) \
	do { \
		type mapme = bytes; \
		_stdf_byte_order_to_src(file, &mapme, 4); \
		uchar *map = (uchar*)&mapme; \
		_stdf_write_byte(file, map[0], buffer); \
		_stdf_write_byte(file, map[1], buffer); \
		_stdf_write_byte(file, map[2], buffer); \
		_stdf_write_byte(file, map[3], buffer); \
	} while (0)
#define _stdf_write_4bytes(file, bytes, buffer) \
	__stdf_write_4bytes(uint32_t, file, bytes, buffer)
#define _stdf_write_4bytes_float(file, bytes, buffer) \
	__stdf_write_4bytes(float, file, bytes, buffer)

#define _stdf_qwrite_dtc_header(file, buffer, REC_LEN, REC_TYP, REC_SUB) \
	do { \
		_stdf_write_dtc_U2(file, REC_LEN, buffer); \
		_stdf_write_dtc_U1(file, REC_TYP, buffer); \
		_stdf_write_dtc_U1(file, REC_SUB, buffer); \
	} while (0)
#define _stdf_write_dtc_header(file, header, buffer) \
	_stdf_qwrite_dtc_header(file, buffer, (header)->REC_LEN, (header)->REC_TYP, (header)->REC_SUB)

#define _stdf_write_dtc_C1(file, C1, buffer) _stdf_write_byte(file, C1, buffer)
#define _stdf_write_dtc_B1(file, B1, buffer) _stdf_write_byte(file, B1, buffer)
#define _stdf_write_dtc_N1(file, N1, buffer) _stdf_write_byte(file, N1, buffer)
#define _stdf_write_dtc_U1(file, U1, buffer) _stdf_write_byte(file, U1, buffer)
#define _stdf_write_dtc_I1(file, I1, buffer) _stdf_write_byte(file, I1, buffer)

#define _stdf_write_dtc_U2(file, U2, buffer) _stdf_write_2bytes(file, U2, buffer)
#define _stdf_write_dtc_I2(file, I2, buffer) _stdf_write_2bytes(file, I2, buffer)

#define _stdf_write_dtc_U4(file, U4, buffer) _stdf_write_4bytes(file, U4, buffer)
#define _stdf_write_dtc_I4(file, I4, buffer) _stdf_write_4bytes(file, I4, buffer)
#define _stdf_write_dtc_R4(file, R4, buffer) _stdf_write_4bytes_float(file, R4, buffer)

#define _stdf_write_dtc_Cn(file, Cn, buffer) _stdf_write_cnt_nbytes(file, Cn+1, *Cn, buffer)
#define _stdf_write_dtc_Bn(file, Bn, buffer) _stdf_write_cnt_nbytes(file, Bn+1, *Bn, buffer)
#define _stdf_write_dtc_Dn(file, Dn, buffer) \
	do { \
		dtc_U2 cnt = *((dtc_U2*)Dn); \
		_stdf_write_dtc_U2(file, cnt, buffer); \
		_stdf_write_nbytes(file, Dn+2, cnt, buffer); \
	} while (0)

#define _stdf_write_dtc_Vn(file, Vn, cnt, buffer) warnf("writing dtc_Vn is not implemented")

#define _stdf_write_dtc_x(file, x, cnt, buffer, func) \
	do { \
		dtc_U2 i; \
		for (i=0; i<cnt; ++i) \
			func(file, x[i], buffer); \
	} while (0)

#define _stdf_write_dtc_xU1(file, xU1, cnt, buffer) _stdf_write_nbytes(file, xU1, cnt, buffer)
#define _stdf_write_dtc_xU2(file, xU2, cnt, buffer) _stdf_write_dtc_x(file, xU2, cnt, buffer, _stdf_write_dtc_U2)
#define _stdf_write_dtc_xN1(file, xN1, cnt, buffer) _stdf_write_nbytes(file, xN1, ((cnt/2)+(cnt%1)), buffer)
#define _stdf_write_dtc_xR4(file, xR4, cnt, buffer) _stdf_write_dtc_x(file, xR4, cnt, buffer, _stdf_write_dtc_R4)
#define _stdf_write_dtc_xCn(file, xCn, cnt, buffer) _stdf_write_dtc_x(file, xCn, cnt, buffer, _stdf_write_dtc_Cn)
#define _stdf_write_dtc_xBn(file, xBn, cnt, buffer) _stdf_write_dtc_xCn(file, xBn, cnt, buffer)

#ifdef STDF_VER3
#define _stdf_write_dtc_Cx(file, Cx, cnt, buffer) warnf("writing dtc_Cx is not implemented");
#endif

#endif /* _LIBSTDF_DTC_H */
