/**
 * @file libstdf_const.h
 * @brief Constants for specific records/members.
 */
/*
 * Copyright (C) 2004-2007 Mike Frysinger <vapier@gmail.com>
 * Released under the BSD license.  For more information,
 * please see: http://opensource.org/licenses/bsd-license.php
 */

#ifndef _LIBSTDF_CONST_H
#define _LIBSTDF_CONST_H

/* FAR: File Attributes Record [page 18] */
typedef enum {
	STDF_CPU_TYPE_DEC        = 0,                    /**< PDP-11 / VAX */
	STDF_CPU_TYPE_SPARC      = 1,                    /**< sparc [Solaris] */
	STDF_CPU_TYPE_X86        = 2,                    /**< x86 [Sun386i / Windows / DOS] */
	STDF_CPU_TYPE_UNKNOWN    = 123,                  /**< Unknown kind of CPU */
#ifdef STDF_VER3
	STDF_CPU_TYPE_SUN_680XX  = STDF_CPU_TYPE_SPARC,  /**< sparc [Solaris] */
	STDF_CPU_TYPE_SUN_80386  = STDF_CPU_TYPE_X86,    /**< x86 [Sun386i] */
	STDF_CPU_TYPE_LTX        = 3,                    /**< WTF is LTX ? */
	STDF_CPU_TYPE_APOLLO     = 4                   	 /**< WTF is Apollo ? */
#endif
} stdf_cpu_types;

/* MIR: Master Information Record [page 20] */
#define	MODE_AEL			'A'
#define	MODE_CHECKER		'C'
#define	MODE_DEBUG			'D'
#define	MODE_ENG			'E'
#define	MODE_MAINTENANCE	'M'
#define	MODE_PRODUCTION		'P'
#define	MODE_QUALITY		'Q'
#define	RETEST_YES			'Y'
#define	RETEST_NO			'N'
#define	RETEST_UNKNOWN		' '

/* HBR: Hardware Bin Record [page 25] */
/* SBR: Software Bin Record [page 27] */
#define	BIN_PASS			'P'
#define	BIN_FAIL			'F'
#define	BIN_UNKNOWN			' '

/* PLR: Pin List Record [page 32] */
#define	PIN_GRP_UNKNOWN		00
#define	PIN_GRP_NORMAL		10
#define	PIN_GRP_SCIO		20
#define	PIN_GRP_SCIO_MID	21
#define	PIN_GRP_SCIO_VALID	22
#define	PIN_GRP_SCIO_SUS	23
#define	PIN_GRP_DUAL		30
#define	PIN_GRP_DUAL_MID	31
#define	PIN_GRP_DUAL_VALID	32
#define	PIN_GRP_DUAL_SUS	33
#define	RADIX_DEFAULT		0
#define	RADIX_BINARY		2
#define	RADIX_OCTAL			8
#define	RADIX_DECIMAL		10
#define	RADIX_HEX			16
#define	RADIX_SYMBOLIC		20

/* WCR: Wafer Configuration Record [page 40] */
#define	UNIT_UNKNOWN		0
#define	UNIT_INCHES			1
#define	UNIT_CENTIMETERS	2
#define	UNIT_MILLIMETERS	3
#define	UNIT_MILS			4
#define	FLAT_UP				'U'
#define	FLAT_DOWN			'D'
#define	FLAT_LEFT			'L'
#define	FLAT_RIGHT			'R'
#define	FLAT_UNKNOWN		' '
#define	POS_LEFT			'L'
#define	POS_RIGHT			'R'
#define	POS_UNKNOWN			' '
#define	POS_UP				'U'
#define	POS_DOWN			'D'

/* PRR: Part Results Record [page 43] */
#define	PART_NEW_ID			0x01
#define	PART_NEW_COORD		0x02
#define	PART_TEST_ABORTED	0x04
#define	PART_FAILED			0x08
#define	PART_NO_INDICATION	0x10

/* TSR: Test Synopsis Record [page 45] */
#define	TEST_PARAMETRIC		'P'
#define	TEST_FUNCTIONAL		'F'
#define	TEST_MULTIPLE		'M'
#define	TEST_UNKNOWN		' '
#define	TEST_MIN_INVALID	0x01
#define	TEST_MAX_INVALID	0x02
#define	TEST_TIM_INVALID	0x04
#define	TEST_SUMS_INVALID	0x10
#define	TEST_SQRS_INVALID	0x20

/* PTR: Parametric Test Record [page 47] */


/* MPR: Multiple-Result Parametric Record [page 53] */


/* FTR: Functional Test Record [page 57] */


/* GDR: Generic Data Record [page 64] */
typedef enum {
	STDF_GDR_B0				= 0,
	STDF_GDR_U1				= 1,
	STDF_GDR_U2				= 2,
	STDF_GDR_U4				= 3,
	STDF_GDR_I1				= 4,
	STDF_GDR_I2				= 5,
	STDF_GDR_I4				= 6,
	STDF_GDR_R4				= 7,
	STDF_GDR_R8				= 8,
	STDF_GDR_Cn				= 10,
	STDF_GDR_Bn				= 11,
	STDF_GDR_Dn				= 12,
	STDF_GDR_N1				= 13
} stdf_dtc_Vn_type;

#endif /* _LIBSTDF_CONST_H */
