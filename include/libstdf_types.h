/* libstdf_types.h
 * Copyright (C) 2004 Mike Frysinger <vapier@gmail.com>
 * Released under the BSD license.  For more information,
 * please see: http://opensource.org/licenses/bsd-license.php
 */

#ifndef _LIBSTDF_TYPES_H
#define _LIBSTDF_TYPES_H

/* Definitions for Record Types [page 7] */
#define	REC_TYP_INFO		0
#define	REC_TYP_PER_LOT		1
#define	REC_TYP_PER_WAFER	2
#define	REC_TYP_PER_PART	5
#define	REC_TYP_PER_TEST	10
#define	REC_TYP_PER_EXEC	15
#define	REC_TYP_PER_PROG	20
#define	REC_TYP_GENERIC		50
#define	REC_TYP_RESV_IMAGE	180
#define	REC_TYP_RESV_IG900	181
/* Definitions for Record Subtypes [page 7] */
#define	REC_SUB_FAR			10
#define	REC_SUB_ATR			20
#define	REC_SUB_MIR			10
#define	REC_SUB_MRR			20
#define	REC_SUB_PCR			30
#define	REC_SUB_HBR			40
#define	REC_SUB_SBR			50
#define	REC_SUB_PMR			60
#define	REC_SUB_PGR			62
#define	REC_SUB_PLR			63
#define	REC_SUB_RDR			70
#define	REC_SUB_SDR			80
#define	REC_SUB_WIR			10
#define	REC_SUB_WRR			20
#define	REC_SUB_WCR			30
#define	REC_SUB_PIR			10
#define	REC_SUB_PRR			20
#define	REC_SUB_TSR			30
#define	REC_SUB_PTR			10
#define	REC_SUB_MPR			15
#define	REC_SUB_FTR			20
#define	REC_SUB_BPS			10
#define	REC_SUB_EPS			20
#define	REC_SUB_GDR			10
#define	REC_SUB_DTR			30
/* Definitions that combine Record Types with Subtypes */
#define	MAKE_REC(typ,sub)	((typ << 8) + sub)
#define	REC_FAR				MAKE_REC(REC_TYP_INFO, REC_SUB_FAR)
#define	REC_ATR				MAKE_REC(REC_TYP_INFO, REC_SUB_ATR)
#define	REC_MIR				MAKE_REC(REC_TYP_PER_LOT, REC_SUB_MIR)
#define	REC_MRR				MAKE_REC(REC_TYP_PER_LOT, REC_SUB_MRR)
#define	REC_PCR				MAKE_REC(REC_TYP_PER_LOT, REC_SUB_PCR)
#define	REC_HBR				MAKE_REC(REC_TYP_PER_LOT, REC_SUB_HBR)
#define	REC_SBR				MAKE_REC(REC_TYP_PER_LOT, REC_SUB_SBR)
#define	REC_PMR				MAKE_REC(REC_TYP_PER_LOT, REC_SUB_PMR)
#define	REC_PGR				MAKE_REC(REC_TYP_PER_LOT, REC_SUB_PGR)
#define	REC_PLR				MAKE_REC(REC_TYP_PER_LOT, REC_SUB_PLR)
#define	REC_RDR				MAKE_REC(REC_TYP_PER_LOT, REC_SUB_RDR)
#define	REC_SDR				MAKE_REC(REC_TYP_PER_LOT, REC_SUB_SDR)
#define	REC_WIR				MAKE_REC(REC_TYP_PER_WAFER, REC_SUB_WIR)
#define	REC_WRR				MAKE_REC(REC_TYP_PER_WAFER, REC_SUB_WRR)
#define	REC_WCR				MAKE_REC(REC_TYP_PER_WAFER, REC_SUB_WCR)
#define	REC_PIR				MAKE_REC(REC_TYP_PER_PART, REC_SUB_PIR)
#define	REC_PRR				MAKE_REC(REC_TYP_PER_PART, REC_SUB_PRR)
#define	REC_TSR				MAKE_REC(REC_TYP_PER_TEST, REC_SUB_TSR)
#define	REC_PTR				MAKE_REC(REC_TYP_PER_EXEC, REC_SUB_PTR)
#define	REC_MPR				MAKE_REC(REC_TYP_PER_EXEC, REC_SUB_MPR)
#define	REC_FTR				MAKE_REC(REC_TYP_PER_EXEC, REC_SUB_FTR)
#define	REC_BPS				MAKE_REC(REC_TYP_PER_PROG, REC_SUB_BPS)
#define	REC_EPS				MAKE_REC(REC_TYP_PER_PROG, REC_SUB_EPS)
#define	REC_GDR				MAKE_REC(REC_TYP_GENERIC, REC_SUB_GDR)
#define	REC_DTR				MAKE_REC(REC_TYP_GENERIC, REC_SUB_DTR)

/* Definitions for Data Type Codes and Representation [page 8] */
#include <sys/types.h>
typedef	char*			dtc_Cn;
typedef	char*			dtc_Cf;
typedef	__uint8_t		dtc_U1;
typedef	__uint16_t		dtc_U2;
typedef	__uint64_t		dtc_U4;
typedef	__int8_t		dtc_I1;
typedef	__int16_t		dtc_I2;
typedef	__int64_t		dtc_I4;
typedef	float			dtc_R4;
typedef	double			dtc_R8;
typedef	char*			dtc_Vn;
typedef	char*			dtc_Bn;
typedef	char*			dtc_Dn;
typedef	char			dtc_N1;

#endif /* _LIBSTDF_TYPES_H */
