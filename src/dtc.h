/* dtc.h
 * Copyright (C) 2004 Mike Frysinger <vapier@gmail.com>
 * Released under the BSD license.  For more information,
 * please see: http://opensource.org/licenses/bsd-license.php
 *
 * $Header$
 */

#ifndef _LIBSTDF_DTC_H
#define _LIBSTDF_DTC_H

#include <libstdf.h>

extern void __byte_order_change(int in, int out, byte_t *in_buf, int len);
#define	__byte_order_to_host(b,p,l) __byte_order_change(b, __STDF_HOST_BYTE_ORDER, p, l)
#define	__byte_order_to_src(b,p,l) __byte_order_change(__STDF_HOST_BYTE_ORDER, b, p, l)
#define	_stdf_byte_order_to_host(f,v,l) __byte_order_to_host(f->byte_order, (byte_t*)(v), l);

#ifdef	STDF_VER3
extern void _stdf_read_dtc_Cx(stdf_file*, dtc_Cn*, int);
#endif
extern void _stdf_read_dtc_Cn(stdf_file*, dtc_Cn*);
#define	_stdf_read_dtc_Bn(f, Bn) _stdf_read_dtc_Cn(f, (dtc_Cn*)Bn)
extern void _stdf_read_dtc_Dn(stdf_file*, dtc_Dn*);

#define	_stdf_read_dtc(f, var, dtc, def) \
	do { \
		if (f->rec_pos < f->rec_end) { \
			memcpy(var, f->rec_pos, sizeof(dtc)); \
			f->rec_pos += sizeof(dtc); \
		} else \
			*var = def; \
	} while (0)

#define	_stdf_read_dtc_num(f, var, dtc) \
	do { \
		_stdf_read_dtc(f, var, dtc, 0); \
		_stdf_byte_order_to_host(f, var, sizeof(dtc)); \
	} while (0)
#define	_stdf_read_dtc_U1(f, var) _stdf_read_dtc_num(f, var, dtc_U1)
#define	_stdf_read_dtc_U2(f, var) _stdf_read_dtc_num(f, var, dtc_U2)
#define	_stdf_read_dtc_U4(f, var) _stdf_read_dtc_num(f, var, dtc_U4)
#define	_stdf_read_dtc_I1(f, var) _stdf_read_dtc_num(f, var, dtc_I1)
#define	_stdf_read_dtc_I2(f, var) _stdf_read_dtc_num(f, var, dtc_I2)
#define	_stdf_read_dtc_I4(f, var) _stdf_read_dtc_num(f, var, dtc_I4)
#define	_stdf_read_dtc_R4(f, var) _stdf_read_dtc_num(f, var, dtc_R4)
#define	_stdf_read_dtc_R8(f, var) _stdf_read_dtc_num(f, var, dtc_R8)

#define	_stdf_read_dtc_C1(f, var) _stdf_read_dtc(f, var, dtc_C1, ' ')
#define	_stdf_read_dtc_B1(f, var) _stdf_read_dtc(f, var, dtc_B1, 0x00)
#define	_stdf_read_dtc_N1(f, var) _stdf_read_dtc(f, var, dtc_N1, 0x00)

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

#endif /* _LIBSTDF_DTC_H */
