/* dtc.h
 * Copyright (C) 2004 Mike Frysinger <vapier@gmail.com>
 * Released under the BSD license.  For more information,
 * please see: http://opensource.org/licenses/bsd-license.php
 */

#include <libstdf.h>

extern void __byte_order_change(int in, int out, byte_t *in_buf, int len);
#define	__byte_order_to_host(b,p,l) __byte_order_change(b, __STDF_HOST_BYTE_ORDER, p, l)
#define	__byte_order_to_src(b,p,l) __byte_order_change(__STDF_HOST_BYTE_ORDER, b, p, l)
#define	_stdf_byte_order_to_host(f,v,l) __byte_order_to_host(f->byte_order, (byte_t*)(v), l);

extern void _stdf_read_dtc_Cn(stdf_file*, dtc_Cn*);
#define	_stdf_read_dtc(f, var, dtc, def) \
	do { \
		if (f->pos < f->rec_end) \
			f->pos += read(f->fd, var, sizeof(dtc)); \
		else \
			*var = def; \
	} while (0)

#define	_stdf_read_dtc_num(f, var, dtc) \
	_stdf_read_dtc(f, var, dtc, 0); \
	_stdf_byte_order_to_host(f, var, sizeof(dtc))
#define	_stdf_read_dtc_U1(f, var) _stdf_read_dtc_num(f, var, dtc_U1)
#define	_stdf_read_dtc_U2(f, var) _stdf_read_dtc_num(f, var, dtc_U2)
#define	_stdf_read_dtc_U4(f, var) _stdf_read_dtc_num(f, var, dtc_U4)
#define	_stdf_read_dtc_I1(f, var) _stdf_read_dtc_num(f, var, dtc_I1)
#define	_stdf_read_dtc_I2(f, var) _stdf_read_dtc_num(f, var, dtc_I2)
#define	_stdf_read_dtc_I4(f, var) _stdf_read_dtc_num(f, var, dtc_I4)
#define	_stdf_read_dtc_R4(f, var) _stdf_read_dtc_num(f, var, dtc_R4)

#define	_stdf_read_dtc_C1(f, var) _stdf_read_dtc(f, var, dtc_C1, ' ')
#define	_stdf_read_dtc_B1(f, var) _stdf_read_dtc(f, var, dtc_B1, 0x00)

#define	_stdf_read_dtc_Bn(f, Bn) _stdf_read_dtc_Cn(f, (dtc_Cn*)Bn)

extern void __stdf_read_dtc_x(stdf_file*, void*, dtc_U2, int);
#define	_stdf_read_dtc_x(f, var, cnt, dtc) __stdf_read_dtc_x(f, var, cnt, sizeof(dtc))
#define	_stdf_read_dtc_xU1(f, var, cnt) _stdf_read_dtc_x(f, var, cnt, dtc_U1)
#define	_stdf_read_dtc_xU2(f, var, cnt) _stdf_read_dtc_x(f, var, cnt, dtc_U2)
#define	_stdf_read_dtc_xR4(f, var, cnt) _stdf_read_dtc_x(f, var, cnt, dtc_R4)

#define	_stdf_read_dtc_xCn(f, var, cnt) fprintf(stderr, "xCn not implemented\n")
#define	free_xCn(xCn) fprintf(stderr, "xCn not implemented\n")
