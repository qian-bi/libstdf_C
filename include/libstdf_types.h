/**
 * @file libstdf_types.h
 * @brief Public types.
 */
/*
 * Copyright (C) 2004-2007 Mike Frysinger <vapier@gmail.com>
 * Released under the BSD license.  For more information,
 * please see: http://opensource.org/licenses/bsd-license.php
 */

#ifndef _LIBSTDF_TYPES_H
#define _LIBSTDF_TYPES_H

/* Definitions for Record Types [page 7] */
typedef enum {
	STDF_REC_TYP_INFO         = 0,
	STDF_REC_TYP_PER_LOT      = 1,
	STDF_REC_TYP_PER_WAFER    = 2,
	STDF_REC_TYP_PER_PART     = 5,
	STDF_REC_TYP_PER_TEST     = 10,
	STDF_REC_TYP_PER_EXEC     = 15,
	STDF_REC_TYP_PER_PROG     = 20,
	STDF_REC_TYP_PER_SITE     = 25,
	STDF_REC_TYP_GENERIC      = 50,
	STDF_REC_TYP_RESV_IMAGE   = 180,
	STDF_REC_TYP_RESV_IG900   = 181,
	STDF_REC_TYP_UNKNOWN      = 0xFF
} stdf_rec_typ;
/* Definitions for Record Subtypes [page 7] */
typedef enum {
	STDF_REC_SUB_FAR          = 10,
	STDF_REC_SUB_ATR          = 20,
	STDF_REC_SUB_MIR          = 10,
	STDF_REC_SUB_MRR          = 20,
	STDF_REC_SUB_PCR          = 30,
	STDF_REC_SUB_HBR          = 40,
	STDF_REC_SUB_SBR          = 50,
	STDF_REC_SUB_PMR          = 60,
	STDF_REC_SUB_PGR          = 62,
	STDF_REC_SUB_PLR          = 63,
	STDF_REC_SUB_RDR          = 70,
	STDF_REC_SUB_SDR          = 80,
	STDF_REC_SUB_WIR          = 10,
	STDF_REC_SUB_WRR          = 20,
	STDF_REC_SUB_WCR          = 30,
	STDF_REC_SUB_PIR          = 10,
	STDF_REC_SUB_PRR          = 20,
#ifdef STDF_VER3
	STDF_REC_SUB_PDR          = 10,
	STDF_REC_SUB_FDR          = 20,
#endif
	STDF_REC_SUB_TSR          = 30,
	STDF_REC_SUB_PTR          = 10,
	STDF_REC_SUB_MPR          = 15,
	STDF_REC_SUB_FTR          = 20,
	STDF_REC_SUB_BPS          = 10,
	STDF_REC_SUB_EPS          = 20,
#ifdef STDF_VER3
	STDF_REC_SUB_SHB          = 10,
	STDF_REC_SUB_SSB          = 20,
	STDF_REC_SUB_STS          = 30,
	STDF_REC_SUB_SCR          = 40,
#endif
	STDF_REC_SUB_GDR          = 10,
	STDF_REC_SUB_DTR          = 30,
	STDF_REC_SUB_UNKNOWN      = 0xFF
} stdf_rec_sub;
/* Definitions that combine Record Types with Subtypes */
#define	MAKE_REC(typ,sub)	((typ << 8) + sub)
#define	BREAK_REC(h,t)		do { h.REC_TYP = (stdf_rec_typ)(t >> 8); h.REC_SUB = (stdf_rec_sub)(t & 0xFF); } while (0)
#define	SET_HEADER(h,t,l)	do { BREAK_REC(h, t); h.REC_LEN = l; } while (0)
#define	INIT_HEADER(h,t)	do { SET_HEADER(h, t, 0); h.state = STDF_REC_STATE_PARSED; } while (0)
#define	HEAD_TO_REC(h)		MAKE_REC(h.REC_TYP,h.REC_SUB)
#define	STDF_REC_FAR		MAKE_REC(STDF_REC_TYP_INFO, STDF_REC_SUB_FAR)
#define	STDF_REC_ATR		MAKE_REC(STDF_REC_TYP_INFO, STDF_REC_SUB_ATR)
#define	STDF_REC_MIR		MAKE_REC(STDF_REC_TYP_PER_LOT, STDF_REC_SUB_MIR)
#define	STDF_REC_MRR		MAKE_REC(STDF_REC_TYP_PER_LOT, STDF_REC_SUB_MRR)
#define	STDF_REC_PCR		MAKE_REC(STDF_REC_TYP_PER_LOT, STDF_REC_SUB_PCR)
#define	STDF_REC_HBR		MAKE_REC(STDF_REC_TYP_PER_LOT, STDF_REC_SUB_HBR)
#define	STDF_REC_SBR		MAKE_REC(STDF_REC_TYP_PER_LOT, STDF_REC_SUB_SBR)
#define	STDF_REC_PMR		MAKE_REC(STDF_REC_TYP_PER_LOT, STDF_REC_SUB_PMR)
#define	STDF_REC_PGR		MAKE_REC(STDF_REC_TYP_PER_LOT, STDF_REC_SUB_PGR)
#define	STDF_REC_PLR		MAKE_REC(STDF_REC_TYP_PER_LOT, STDF_REC_SUB_PLR)
#define	STDF_REC_RDR		MAKE_REC(STDF_REC_TYP_PER_LOT, STDF_REC_SUB_RDR)
#define	STDF_REC_SDR		MAKE_REC(STDF_REC_TYP_PER_LOT, STDF_REC_SUB_SDR)
#define	STDF_REC_WIR		MAKE_REC(STDF_REC_TYP_PER_WAFER, STDF_REC_SUB_WIR)
#define	STDF_REC_WRR		MAKE_REC(STDF_REC_TYP_PER_WAFER, STDF_REC_SUB_WRR)
#define	STDF_REC_WCR		MAKE_REC(STDF_REC_TYP_PER_WAFER, STDF_REC_SUB_WCR)
#define	STDF_REC_PIR		MAKE_REC(STDF_REC_TYP_PER_PART, STDF_REC_SUB_PIR)
#define	STDF_REC_PRR		MAKE_REC(STDF_REC_TYP_PER_PART, STDF_REC_SUB_PRR)
#ifdef	STDF_VER3
#define	STDF_REC_PDR		MAKE_REC(STDF_REC_TYP_PER_TEST, STDF_REC_SUB_PDR)
#define	STDF_REC_FDR		MAKE_REC(STDF_REC_TYP_PER_TEST, STDF_REC_SUB_FDR)
#endif
#define	STDF_REC_TSR		MAKE_REC(STDF_REC_TYP_PER_TEST, STDF_REC_SUB_TSR)
#define	STDF_REC_PTR		MAKE_REC(STDF_REC_TYP_PER_EXEC, STDF_REC_SUB_PTR)
#define	STDF_REC_MPR		MAKE_REC(STDF_REC_TYP_PER_EXEC, STDF_REC_SUB_MPR)
#define	STDF_REC_FTR		MAKE_REC(STDF_REC_TYP_PER_EXEC, STDF_REC_SUB_FTR)
#define	STDF_REC_BPS		MAKE_REC(STDF_REC_TYP_PER_PROG, STDF_REC_SUB_BPS)
#define	STDF_REC_EPS		MAKE_REC(STDF_REC_TYP_PER_PROG, STDF_REC_SUB_EPS)
#ifdef	STDF_VER3
#define	STDF_REC_SHB		MAKE_REC(STDF_REC_TYP_PER_SITE, STDF_REC_SUB_SHB)
#define	STDF_REC_SSB		MAKE_REC(STDF_REC_TYP_PER_SITE, STDF_REC_SUB_SSB)
#define	STDF_REC_STS		MAKE_REC(STDF_REC_TYP_PER_SITE, STDF_REC_SUB_STS)
#define	STDF_REC_SCR		MAKE_REC(STDF_REC_TYP_PER_SITE, STDF_REC_SUB_SCR)
#endif
#define	STDF_REC_GDR		MAKE_REC(STDF_REC_TYP_GENERIC, STDF_REC_SUB_GDR)
#define	STDF_REC_DTR		MAKE_REC(STDF_REC_TYP_GENERIC, STDF_REC_SUB_DTR)
#define	STDF_REC_UNKNOWN			MAKE_REC(STDF_REC_TYP_UNKNOWN, STDF_REC_SUB_UNKNOWN)

/* Definitions for Data Type Codes and Representation [page 8] */
typedef	char*			stdf_dtc_Cn;
typedef	char			stdf_dtc_C1;
typedef	uint8_t			stdf_dtc_U1;
typedef	uint16_t		stdf_dtc_U2;
typedef	uint32_t		stdf_dtc_U4;
typedef	int8_t			stdf_dtc_I1;
typedef	int16_t			stdf_dtc_I2;
typedef	int32_t			stdf_dtc_I4;
typedef	float			stdf_dtc_R4;
typedef	double			stdf_dtc_R8;
typedef	unsigned char*	stdf_dtc_Bn;
typedef	unsigned char	stdf_dtc_B1;
typedef	unsigned char*	stdf_dtc_Dn;
typedef	unsigned char	stdf_dtc_N1;
typedef	stdf_dtc_Cn*	stdf_dtc_xCn;
typedef	stdf_dtc_U1*	stdf_dtc_xU1;
typedef	stdf_dtc_U2*	stdf_dtc_xU2;
typedef	stdf_dtc_R4*	stdf_dtc_xR4;
typedef	stdf_dtc_N1*	stdf_dtc_xN1;
typedef struct {
	stdf_dtc_Vn_type     type;
	void                *data;
} stdf_dtc_Vn_ele;
typedef	stdf_dtc_Vn_ele* stdf_dtc_Vn;

typedef enum {
	STDF_REC_STATE_RAW,
	STDF_REC_STATE_PARSED
} stdf_rec_state;
/**
 * @brief The header found with every record in a STDF file.
 *
 * See the Definitions for Record Types [page 15+] of the spec
 * file for more information
 */
typedef struct {
	void                *stdf_file;
	stdf_rec_state       state;
	stdf_dtc_U2          REC_LEN;		/**< Record length (in bytes) */
	stdf_rec_typ         REC_TYP;		/**< The 'type' of record (class) */
	stdf_rec_sub         REC_SUB;		/**< The 'sub type' of record */
} stdf_rec_header;
/**
 * @brief The basic record type.
 *
 * You figure out what kind of record this really represents
 * by looking at the header.  Then simply cast it as the
 * appropriate record type.
 */
typedef struct {
	stdf_rec_header      header;
	void                *data;
} stdf_rec_unknown;
/* FAR: File Attributes Record [page 18] */
typedef struct {
	stdf_rec_header      header;
	stdf_dtc_U1          CPU_TYPE;
	stdf_dtc_U1          STDF_VER;
} stdf_rec_far;
/* ATR: Audit Trail Record [page 19] */
typedef struct {	/* V4 */
	stdf_rec_header	header;
	stdf_dtc_U4		MOD_TIM;
	stdf_dtc_Cn		CMD_LINE;
} stdf_rec_atr;
/* MIR: Master Information Record [page 20] */
typedef struct {
	stdf_rec_header	header;
#ifdef STDF_VER3
	stdf_dtc_U1		CPU_TYPE;	/* V3 */
	stdf_dtc_U1		STDF_VER;	/* V3 */
#endif
	stdf_dtc_U4		SETUP_T;
	stdf_dtc_U4		START_T;
	stdf_dtc_U1		STAT_NUM;
	stdf_dtc_C1		MODE_COD;
	stdf_dtc_C1		RTST_COD;
	stdf_dtc_C1		PROT_COD;
	stdf_dtc_U2		BURN_TIM;	/* V4 */
	stdf_dtc_C1		CMOD_COD;
	stdf_dtc_Cn		LOT_ID;
	stdf_dtc_Cn		PART_TYP;
	stdf_dtc_Cn		NODE_NAM;
	stdf_dtc_Cn		TSTR_TYP;
#ifdef STDF_VER3
	stdf_dtc_Cn		HAND_ID;	/* V3 */
	stdf_dtc_Cn		PRB_CARD;	/* V3 */
#endif
	stdf_dtc_Cn		JOB_NAM;
	stdf_dtc_Cn		JOB_REV;
	stdf_dtc_Cn		SBLOT_ID;
	stdf_dtc_Cn		OPER_NAM;
	stdf_dtc_Cn		EXEC_TYP;
	stdf_dtc_Cn		EXEC_VER;	/* V4 */
	stdf_dtc_Cn		TEST_COD;	/* V3:C3 V4:Cn */
	stdf_dtc_Cn		TST_TEMP;	/* V4 */
	stdf_dtc_Cn		USER_TXT;	/* V4 */
	stdf_dtc_Cn		AUX_FILE;	/* V4 */
	stdf_dtc_Cn		PKG_TYP;	/* V4 */
	stdf_dtc_Cn		FAMILY_ID;	/* V4 */
	stdf_dtc_Cn		DATE_COD;	/* V4 */
	stdf_dtc_Cn		FACIL_ID;	/* V4 */
	stdf_dtc_Cn		FLOOR_ID;	/* V4 */
	stdf_dtc_Cn		PROC_ID;
	stdf_dtc_Cn		OPER_FRQ;	/* V4 */
	stdf_dtc_Cn		SPEC_NAM;	/* V4 */
	stdf_dtc_Cn		SPEC_VER;	/* V4 */
	stdf_dtc_Cn		FLOW_ID;	/* V4 */
	stdf_dtc_Cn		SETUP_ID;	/* V4 */
	stdf_dtc_Cn		DSGN_REV;	/* V4 */
	stdf_dtc_Cn		ENG_ID;		/* V4 */
	stdf_dtc_Cn		ROM_COD;	/* V4 */
	stdf_dtc_Cn		SERL_NUM;	/* V4 */
	stdf_dtc_Cn		SUPR_NAM;
} stdf_rec_mir;
/* MRR: Master Results Record [page 23] */
typedef struct {
	stdf_rec_header	header;
	stdf_dtc_U4		FINISH_T;
#ifdef STDF_VER3
	stdf_dtc_U4		PART_CNT;	/* V3 */
	stdf_dtc_U4		RTST_CNT;	/* V3 */
	stdf_dtc_U4		ABRT_CNT;	/* V3 */
	stdf_dtc_U4		GOOD_CNT;	/* V3 */
	stdf_dtc_U4		FUNC_CNT;	/* V3 */
#endif
	stdf_dtc_C1		DISP_COD;
	stdf_dtc_Cn		USR_DESC;
	stdf_dtc_Cn		EXC_DESC;
} stdf_rec_mrr;
/* PCR: Part Count Record [page 24] */
typedef struct {	/* V4 */
	stdf_rec_header	header;
	stdf_dtc_U1		HEAD_NUM;
	stdf_dtc_U1		SITE_NUM;
	stdf_dtc_U4		PART_CNT;
	stdf_dtc_U4		RTST_CNT;
	stdf_dtc_U4		ABRT_CNT;
	stdf_dtc_U4		GOOD_CNT;
	stdf_dtc_U4		FUNC_CNT;
} stdf_rec_pcr;
/* HBR: Hardware Bin Record [page 25] */
typedef struct {
	stdf_rec_header	header;
	stdf_dtc_U1		HEAD_NUM;
	stdf_dtc_U1		SITE_NUM;
	stdf_dtc_U2		HBIN_NUM;
	stdf_dtc_U4		HBIN_CNT;
	stdf_dtc_C1		HBIN_PF;
	stdf_dtc_Cn		HBIN_NAM;
} stdf_rec_hbr;
/* SBR: Software Bin Record [page 27] */
typedef struct {
	stdf_rec_header	header;
	stdf_dtc_U1		HEAD_NUM;
	stdf_dtc_U1		SITE_NUM;
	stdf_dtc_U2		SBIN_NUM;
	stdf_dtc_U4		SBIN_CNT;
	stdf_dtc_C1		SBIN_PF;
	stdf_dtc_Cn		SBIN_NAM;
} stdf_rec_sbr;
/* PMR: Pin Map Record [page 29] */
typedef struct {
	stdf_rec_header	header;
	stdf_dtc_U2		PMR_INDX;
	stdf_dtc_U2		CHAN_TYP;
	stdf_dtc_Cn		CHAN_NAM;
	stdf_dtc_Cn		PHY_NAM;
	stdf_dtc_Cn		LOG_NAM;
	stdf_dtc_U1		HEAD_NUM;
	stdf_dtc_U1		SITE_NUM;
} stdf_rec_pmr;
/* PGR: Pin Group Record [page 31] */
typedef struct {	/* V4 */
	stdf_rec_header	header;
	stdf_dtc_U2		GRP_INDX;
	stdf_dtc_Cn		GRP_NAM;
	stdf_dtc_U2		INDX_CNT;
	stdf_dtc_xU2		PMR_INDX;
} stdf_rec_pgr;
/* PLR: Pin List Record [page 32] */
typedef struct {	/* V4 */
	stdf_rec_header	header;
	stdf_dtc_U2		GRP_CNT;
	stdf_dtc_xU2		GRP_INDX;
	stdf_dtc_xU2		GRP_MODE;
	stdf_dtc_xU1		GRP_RADX;
	stdf_dtc_xCn		PGM_CHAR;
	stdf_dtc_xCn		RTN_CHAR;
	stdf_dtc_xCn		PGM_CHAL;
	stdf_dtc_xCn		RTN_CHAL;
} stdf_rec_plr;
/* RDR: Retest Data Record [page 34] */
typedef struct {	/* V4 */
	stdf_rec_header	header;
	stdf_dtc_U2		NUM_BINS;
	stdf_dtc_xU2		RTST_BIN;
} stdf_rec_rdr;
/* SDR: Site Description Record [page 35] */
typedef struct {	/* V4 */
	stdf_rec_header	header;
	stdf_dtc_U1		HEAD_NUM;
	stdf_dtc_U1		SITE_GRP;
	stdf_dtc_U1		SITE_CNT;
	stdf_dtc_xU1		SITE_NUM;
	stdf_dtc_Cn		HAND_TYP;
	stdf_dtc_Cn		HAND_ID;
	stdf_dtc_Cn		CARD_TYP;
	stdf_dtc_Cn		CARD_ID;
	stdf_dtc_Cn		LOAD_TYP;
	stdf_dtc_Cn		LOAD_ID;
	stdf_dtc_Cn		DIB_TYP;
	stdf_dtc_Cn		DIB_ID;
	stdf_dtc_Cn		CABL_TYP;
	stdf_dtc_Cn		CABL_ID;
	stdf_dtc_Cn		CONT_TYP;
	stdf_dtc_Cn		CONT_ID;
	stdf_dtc_Cn		LASR_TYP;
	stdf_dtc_Cn		LASR_ID;
	stdf_dtc_Cn		EXTR_TYP;
	stdf_dtc_Cn		EXTR_ID;
} stdf_rec_sdr;
/* WIR: Wafer Information Record [page 37] */
typedef struct {
	stdf_rec_header	header;
	stdf_dtc_U1		HEAD_NUM;
	stdf_dtc_U1		SITE_GRP;	/* V4 */
#ifdef STDF_VER3
	stdf_dtc_B1		PAD_BYTE;	/* V3 */
#endif
	stdf_dtc_U4		START_T;
	stdf_dtc_Cn		WAFER_ID;
} stdf_rec_wir;
/* WRR: Wafer Results Record [page 38] */
typedef struct {
	stdf_rec_header	header;
	stdf_dtc_U1		HEAD_NUM;
	stdf_dtc_U1		SITE_GRP;	/* V4 */
#ifdef STDF_VER3
	stdf_dtc_B1		PAD_BYTE;	/* V3 */
#endif
	stdf_dtc_U4		FINISH_T;
	stdf_dtc_U4		PART_CNT;
	stdf_dtc_U4		RTST_CNT;
	stdf_dtc_U4		ABRT_CNT;
	stdf_dtc_U4		GOOD_CNT;
	stdf_dtc_U4		FUNC_CNT;
#ifdef STDF_VER3
	stdf_dtc_Cn		HAND_ID;	/* V3 */
	stdf_dtc_Cn		PRB_CARD;	/* V3 */
#endif
	stdf_dtc_Cn		WAFER_ID;
	stdf_dtc_Cn		FABWF_ID;	/* V4 */
	stdf_dtc_Cn		FRAME_ID;	/* V4 */
	stdf_dtc_Cn		MASK_ID;	/* V4 */
	stdf_dtc_Cn		USR_DESC;
	stdf_dtc_Cn		EXC_DESC;
} stdf_rec_wrr;
/* WCR: Wafer Configuration Record [page 40] */
typedef struct {
	stdf_rec_header	header;
	stdf_dtc_R4		WAFR_SIZ;
	stdf_dtc_R4		DIE_HT;
	stdf_dtc_R4		DIE_WID;
	stdf_dtc_U1		WF_UNITS;
	stdf_dtc_C1		WF_FLAT;
	stdf_dtc_I2		CENTER_X;
	stdf_dtc_I2		CENTER_Y;
	stdf_dtc_C1		POS_X;
	stdf_dtc_C1		POS_Y;
} stdf_rec_wcr;
/* PIR: Part Information Record [page 42] */
typedef struct {
	stdf_rec_header	header;
	stdf_dtc_U1		HEAD_NUM;
	stdf_dtc_U1		SITE_NUM;
#ifdef STDF_VER3
	stdf_dtc_I2		X_COORD;	/* V3 */
	stdf_dtc_I2		Y_COORD;	/* V3 */
	stdf_dtc_Cn		PART_ID;	/* V3 */
#endif
} stdf_rec_pir;
/* PRR: Part Results Record [page 43] */
typedef struct {
	stdf_rec_header	header;
	stdf_dtc_U1		HEAD_NUM;
	stdf_dtc_U1		SITE_NUM;
#ifdef STDF_VER3
	stdf_dtc_B1		PAD_BYTE;	/* V3 */
#endif
	stdf_dtc_B1		PART_FLG;
	stdf_dtc_U2		NUM_TEST;
	stdf_dtc_U2		HARD_BIN;
	stdf_dtc_U2		SOFT_BIN;
	stdf_dtc_I2		X_COORD;
	stdf_dtc_I2		Y_COORD;
	stdf_dtc_U4		TEST_T;		/* V4 */
	stdf_dtc_Cn		PART_ID;
	stdf_dtc_Cn		PART_TXT;
	stdf_dtc_Bn		PART_FIX;
} stdf_rec_prr;
#ifdef	STDF_VER3
/* PDR: Parametric Test Description */
typedef struct {	/* V3 */
	stdf_rec_header	header;
	stdf_dtc_U4		TEST_NUM;
	stdf_dtc_B1		DESC_FLG;
	stdf_dtc_B1		OPT_FLAG;
	stdf_dtc_I1		RES_SCAL;
	stdf_dtc_Cn		UNITS;	/*stdf_dtc_C7*/
	stdf_dtc_U1		RES_LDIG;
	stdf_dtc_U1		RES_RDIG;
	stdf_dtc_I1		LLM_SCAL;
	stdf_dtc_I1		HLM_SCAL;
	stdf_dtc_U1		LLM_LDIG;
	stdf_dtc_U1		LLM_RDIG;
	stdf_dtc_U1		HLM_LDIG;
	stdf_dtc_U1		HLM_RDIG;
	stdf_dtc_R4		LO_LIMIT;
	stdf_dtc_R4		HI_LIMIT;
	stdf_dtc_Cn		TEST_NAM;
	stdf_dtc_Cn		SEQ_NAME;
} stdf_rec_pdr;
#endif
#ifdef	STDF_VER3
/* FDR: Functional Test Description */
typedef struct {	/* V3 */
	stdf_rec_header	header;
	stdf_dtc_U4		TEST_NUM;
	stdf_dtc_B1		DESC_FLG;
	stdf_dtc_Cn		TEST_NAM;
	stdf_dtc_Cn		SEQ_NAME;
} stdf_rec_fdr;
#endif
/* TSR: Test Synopsis Record [page 45] */
typedef struct {
	stdf_rec_header	header;
	stdf_dtc_U1		HEAD_NUM;	/* V4 */
	stdf_dtc_U1		SITE_NUM;	/* V4 */
#ifdef STDF_VER3
	stdf_dtc_B1		PAD_BYTE;	/* V3 */
	stdf_dtc_R4		TST_MEAN;	/* V3 */
	stdf_dtc_R4		TST_SDEV;	/* V3 */
#endif
	stdf_dtc_C1		TEST_TYP;	/* V4 */
	stdf_dtc_U4		TEST_NUM;
	stdf_dtc_U4		EXEC_CNT;	/* V3:I4 V4:U4 */
	stdf_dtc_U4		FAIL_CNT;	/* V3:I4 V4:U4 */
	stdf_dtc_U4		ALRM_CNT;	/* V3:I4 V4:U4 */
	stdf_dtc_Cn		TEST_NAM;
	stdf_dtc_Cn		SEQ_NAME;
	stdf_dtc_Cn		TEST_LBL;	/* V4 */
	stdf_dtc_B1		OPT_FLAG;
	stdf_dtc_R4		TEST_TIM;	/* V4 */
	stdf_dtc_R4		TEST_MIN;
	stdf_dtc_R4		TEST_MAX;
	stdf_dtc_R4		TST_SUMS;
	stdf_dtc_R4		TST_SQRS;
} stdf_rec_tsr;
/* PTR: Parametric Test Record [page 47] */
typedef struct {
	stdf_rec_header	header;
	stdf_dtc_U4		TEST_NUM;
	stdf_dtc_U1		HEAD_NUM;
	stdf_dtc_U1		SITE_NUM;
	stdf_dtc_B1		TEST_FLG;
	stdf_dtc_B1		PARM_FLG;
	stdf_dtc_R4		RESULT;
	stdf_dtc_Cn		TEST_TXT;
	stdf_dtc_Cn		ALARM_ID;	/* V4 */
	stdf_dtc_B1		OPT_FLAG;
	stdf_dtc_I1		RES_SCAL;
#ifdef STDF_VER3
	stdf_dtc_U1		RES_LDIG;	/* V3 */
	stdf_dtc_U1		RES_RDIG;	/* V3 */
	stdf_dtc_B1		DESC_FLG;	/* V3 */
#endif
	stdf_dtc_I1		LLM_SCAL;
	stdf_dtc_I1		HLM_SCAL;
#ifdef STDF_VER3
	stdf_dtc_U1		HLM_LDIG;	/* V3 */
	stdf_dtc_U1		HLM_RDIG;	/* V3 */
	stdf_dtc_U1		LLM_LDIG;	/* V3 */
	stdf_dtc_U1		LLM_RDIG;	/* V3 */
#endif
	stdf_dtc_R4		LO_LIMIT;
	stdf_dtc_R4		HI_LIMIT;
	stdf_dtc_Cn		UNITS;		/* V3:C7 V4:Cn */
	stdf_dtc_Cn		C_RESFMT;
	stdf_dtc_Cn		C_LLMFMT;
	stdf_dtc_Cn		C_HLMFMT;
	stdf_dtc_R4		LO_SPEC;	/* V4 */
	stdf_dtc_R4		HI_SPEC;	/* V4 */
#ifdef STDF_VER3
	stdf_dtc_Cn		TEST_NAM;
	stdf_dtc_Cn		SEQ_NAME;
#endif
} stdf_rec_ptr;
/* MPR: Multiple-Result Parametric Record [page 53] */
typedef struct {	/* V4 */
	stdf_rec_header	header;
	stdf_dtc_U4		TEST_NUM;
	stdf_dtc_U1		HEAD_NUM;
	stdf_dtc_U1		SITE_NUM;
	stdf_dtc_B1		TEST_FLG;
	stdf_dtc_B1		PARM_FLG;
	stdf_dtc_U2		RTN_ICNT;
	stdf_dtc_U2		RSLT_CNT;
	stdf_dtc_xN1		RTN_STAT;
	stdf_dtc_xR4		RTN_RSLT;
	stdf_dtc_Cn		TEST_TXT;
	stdf_dtc_Cn		ALARM_ID;
	stdf_dtc_B1		OPT_FLAG;
	stdf_dtc_I1		RES_SCAL;
	stdf_dtc_I1		LLM_SCAL;
	stdf_dtc_I1		HLM_SCAL;
	stdf_dtc_R4		LO_LIMIT;
	stdf_dtc_R4		HI_LIMIT;
	stdf_dtc_R4		START_IN;
	stdf_dtc_R4		INCR_IN;
	stdf_dtc_xU2		RTN_INDX;
	stdf_dtc_Cn		UNITS;
	stdf_dtc_Cn		UNITS_IN;
	stdf_dtc_Cn		C_RESFMT;
	stdf_dtc_Cn		C_LLMFMT;
	stdf_dtc_Cn		C_HLMFMT;
	stdf_dtc_R4		LO_SPEC;
	stdf_dtc_R4		HI_SPEC;
} stdf_rec_mpr;
/* FTR: Functional Test Record [page 57] */
typedef struct {
	stdf_rec_header header;
	stdf_dtc_U4		TEST_NUM;
	stdf_dtc_U1		HEAD_NUM;
	stdf_dtc_U1		SITE_NUM;
	stdf_dtc_B1		TEST_FLG;
#ifdef STDF_VER3
	stdf_dtc_B1		DESC_FLG;	/* V3 */
#endif
	stdf_dtc_B1		OPT_FLAG;
	stdf_dtc_U4		CYCL_CNT;
	stdf_dtc_U4		REL_VADR;	/* V4 */
	stdf_dtc_U4		REPT_CNT;	/* V3:U2 V4:U4 */
	stdf_dtc_U4		NUM_FAIL;
	stdf_dtc_I4		XFAIL_AD;	/* V4 */
	stdf_dtc_I4		YFAIL_AD;	/* V4 */
	stdf_dtc_I2		VECT_OFF;	/* V4 */
	stdf_dtc_U2		RTN_ICNT;	/* V4 */
	stdf_dtc_U2		PGM_ICNT;	/* V4 */
	stdf_dtc_xU2		RTN_INDX;	/* V4 */
	stdf_dtc_xN1		RTN_STAT;	/* V4 */
	stdf_dtc_xU2		PGM_INDX;	/* V4 */
	stdf_dtc_xN1		PGM_STAT;	/* V4 */
	stdf_dtc_Dn		FAIL_PIN;	/* V3:Bn V4:Dn */
	stdf_dtc_Cn		VECT_NAM;	/* V4 */
	stdf_dtc_Cn		TIME_SET;	/* V3:U1 V4:Cn */
#ifdef STDF_VER3
	stdf_dtc_U4		VECT_ADR;	/* V3 */
	stdf_dtc_U2		PCP_ADDR;	/* V3 */
	stdf_dtc_Bn		VECT_DAT;	/* V3 */
	stdf_dtc_Bn		DEV_DAT;	/* V3 */
	stdf_dtc_Bn		RPIN_MAP;	/* V3 */
	stdf_dtc_Cn		TEST_NAM;	/* V3 */
	stdf_dtc_Cn		SEQ_NAME;	/* V3 */
#endif
	stdf_dtc_Cn		OP_CODE;	/* V4 */
	stdf_dtc_Cn		TEST_TXT;
	stdf_dtc_Cn		ALARM_ID;	/* V4 */
	stdf_dtc_Cn		PROG_TXT;	/* V4 */
	stdf_dtc_Cn		RSLT_TXT;	/* V4 */
	stdf_dtc_U1		PATG_NUM;	/* V4 */
	stdf_dtc_Dn		SPIN_MAP;	/* V4 */
} stdf_rec_ftr;
/* BPS: Begin Program Section [page 62] */
typedef struct {
	stdf_rec_header	header;
	stdf_dtc_Cn		SEQ_NAME;
} stdf_rec_bps;
/* EPS: End Program Section [page 63] */
typedef struct {
	stdf_rec_header	header;
} stdf_rec_eps;
#ifdef	STDF_VER3
/* SHB: Site-Specific Hardware Bin Record */
typedef struct {	/* V3 */
	stdf_rec_header	header;
	stdf_dtc_U1		HEAD_NUM;
	stdf_dtc_U1		SITE_NUM;
	stdf_dtc_U2		HBIN_NUM;
	stdf_dtc_U4		HBIN_CNT;
	stdf_dtc_Cn		HBIN_NAM;
} stdf_rec_shb;
/* SSB: Site-Specific Software Bin Record */
typedef struct {	/* V3 */
	stdf_rec_header	header;
	stdf_dtc_U1		HEAD_NUM;
	stdf_dtc_U1		SITE_NUM;
	stdf_dtc_U2		SBIN_NUM;
	stdf_dtc_U4		SBIN_CNT;
	stdf_dtc_Cn		SBIN_NAM;
} stdf_rec_ssb;
/* STS: Site-Specific Test Synopsis Record */
typedef struct {	/* V3 */
	stdf_rec_header	header;
	stdf_dtc_U1		HEAD_NUM;
	stdf_dtc_U1		SITE_NUM;
	stdf_dtc_U4		TEST_NUM;
	stdf_dtc_I4		EXEC_CNT;
	stdf_dtc_I4		FAIL_CNT;
	stdf_dtc_I4		ALRM_CNT;
	stdf_dtc_B1		OPT_FLAG;
	stdf_dtc_B1		PAD_BYTE;
	stdf_dtc_R4		TEST_MIN;
	stdf_dtc_R4		TEST_MAX;
	stdf_dtc_R4		TST_MEAN;
	stdf_dtc_R4		TST_SDEV;
	stdf_dtc_R4		TST_SUMS;
	stdf_dtc_R4		TST_SQRS;
	stdf_dtc_Cn		TEST_NAM;
	stdf_dtc_Cn		SEQ_NAME;
	stdf_dtc_Cn		TEST_LBL;
} stdf_rec_sts;
/* SCR: Site-Specific Part Count Record */
typedef struct {	/* V3 */
	stdf_rec_header	header;
	stdf_dtc_U1		HEAD_NUM;
	stdf_dtc_U1		SITE_NUM;
	stdf_dtc_U4		FINISH_T;
	stdf_dtc_U4		PART_CNT;
	stdf_dtc_I4		RTST_CNT;
	stdf_dtc_I4		ABRT_CNT;
	stdf_dtc_I4		GOOD_CNT;
	stdf_dtc_I4		FUNC_CNT;
} stdf_rec_scr;
#endif
/* GDR: Generic Data Record [page 64] */
typedef struct {
	stdf_rec_header	header;
	stdf_dtc_U2		FLD_CNT;
	stdf_dtc_Vn		GEN_DATA;
} stdf_rec_gdr;
/* DTR: Datalog Text Record [page 66] */
typedef struct {
	stdf_rec_header	header;
	stdf_dtc_Cn		TEXT_DAT;
} stdf_rec_dtr;

#endif /* _LIBSTDF_TYPES_H */
