/**
 * @file rec.c
 * @brief Record specific function implementations.
 * @internal
 */
/*
 * Copyright (C) 2004 Mike Frysinger <vapier@gmail.com>
 * Released under the BSD license.  For more information,
 * please see: http://opensource.org/licenses/bsd-license.php
 *
 * $Header$
 */

#include <libstdf.h>
#include "dtc.h"
#include "rec.h"

char *stdf_rec_names[] = {
	"???",
	"FAR", "ATR", "MIR", "MRR", "PCR", "HBR",
	"SBR", "PMR", "PGR", "PLR", "RDR", "SDR",
	"WIR", "WRR", "WCR", "PIR", "PRR",
	"PDR", "FDR",
	"TSR", "PTR", "MPR", "FTR", "BPS", "EPS",
	"SHB", "SSB", "STS", "SCR",
	"GDR", "DTR"
};

static int _stdf_header_to_idx(rec_typ type, rec_sub sub)
{
	switch (MAKE_REC(type, sub)) {
		case REC_FAR:	return 1;
		case REC_ATR:	return 2;
		case REC_MIR:	return 3;
		case REC_MRR:	return 4;
		case REC_PCR:	return 5;
		case REC_HBR:	return 6;
		case REC_SBR:	return 7;
		case REC_PMR:	return 8;
		case REC_PGR:	return 9;
		case REC_PLR:	return 10;
		case REC_RDR:	return 11;
		case REC_SDR:	return 12;
		case REC_WIR:	return 13;
		case REC_WRR:	return 14;
		case REC_WCR:	return 15;
		case REC_PIR:	return 16;
		case REC_PRR:	return 17;
#ifdef STDF_VER3
		case REC_PDR:	return 18;
		case REC_FDR:	return 19;
#endif
		case REC_TSR:	return 20;
		case REC_PTR:	return 21;
		case REC_MPR:	return 22;
		case REC_FTR:	return 23;
		case REC_BPS:	return 24;
		case REC_EPS:	return 25;
#ifdef STDF_VER3
		case REC_SHB:	return 26;
		case REC_SSB:	return 27;
		case REC_STS:	return 28;
		case REC_SCR:	return 29;
#endif
		case REC_GDR:	return 30;
		case REC_DTR:	return 31;
		case REC_UNKNOWN:
		default:		return 0;
	}
}
int stdf_rec_to_idx_count()
{
	return 32;
}
int stdf_rec_name_to_idx(char *rec_name)
{
	int i = stdf_rec_to_idx_count();
	while (--i > 0)
		if (strncmp(rec_name, stdf_rec_names[i], 3) == 0)
			return i;
	return 0;
}
int stdf_rec_to_idx(rec_unknown *rec)
{
	return _stdf_header_to_idx(rec->header.REC_TYP, rec->header.REC_SUB);
}
char* stdf_get_rec_name(rec_typ type, rec_sub sub)
{
	return stdf_rec_names[_stdf_header_to_idx(type, sub)];
}

#if WARN_UNTESTED
#define	warn_untested(type) \
	do { \
		fprintf(stderr, "*********************************************\n"); \
		fprintf(stderr, "This record type (" type ") has not been tested!\n"); \
		fprintf(stderr, "Please consider sending this file to\n"); \
		fprintf(stderr, "vapier@gmail.com to help out the\n"); \
		fprintf(stderr, "FreeSTDF project and make sure this code\n"); \
		fprintf(stderr, "works exactly the way it should!\n"); \
		fprintf(stderr, "*********************************************\n"); \
	} while (0)
#else
#define warn_untested(type)
#endif

#define __malloc_rec(r) ((r*)malloc(sizeof(r)))
rec_unknown* stdf_read_rec_unknown(stdf_file *file)
{
	rec_unknown *rec = __malloc_rec(rec_unknown);
#if 0
	rec->data = (void*)malloc(file->header.REC_LEN);
	memcpy(rec->data, file->rec_data, file->header.REC_LEN);
#else
	rec->data = NULL;
#endif
	file->rec_pos += file->header.REC_LEN;
	return rec;
}
rec_far* stdf_read_rec_far(stdf_file *file)
{
	rec_far *far = __malloc_rec(rec_far);
	_stdf_read_dtc_U1(file, &(far->CPU_TYPE));
	_stdf_read_dtc_U1(file, &(far->STDF_VER));
	return far;
}
rec_atr* stdf_read_rec_atr(stdf_file *file)
{
	rec_atr *atr = __malloc_rec(rec_atr);
	_stdf_read_dtc_U4(file, &(atr->MOD_TIM));
	_stdf_read_dtc_Cn(file, &(atr->CMD_LINE));
	return atr;
}
rec_mir* stdf_read_rec_mir(stdf_file *file)
{
	rec_mir *mir = __malloc_rec(rec_mir);
#ifdef STDF_VER3
	if (file->ver == 4) {
#endif
	_stdf_read_dtc_U4(file, &(mir->SETUP_T));
	_stdf_read_dtc_U4(file, &(mir->START_T));
	_stdf_read_dtc_U1(file, &(mir->STAT_NUM));
	_stdf_read_dtc_C1(file, &(mir->MODE_COD));
	_stdf_read_dtc_C1(file, &(mir->RTST_COD));
	_stdf_read_dtc_C1(file, &(mir->PROT_COD));
	_stdf_read_dtc_U2(file, &(mir->BURN_TIM));
	_stdf_read_dtc_C1(file, &(mir->CMOD_COD));
	_stdf_read_dtc_Cn(file, &(mir->LOT_ID));
	_stdf_read_dtc_Cn(file, &(mir->PART_TYP));
	_stdf_read_dtc_Cn(file, &(mir->NODE_NAM));
	_stdf_read_dtc_Cn(file, &(mir->TSTR_TYP));
	_stdf_read_dtc_Cn(file, &(mir->JOB_NAM));
	_stdf_read_dtc_Cn(file, &(mir->JOB_REV));
	_stdf_read_dtc_Cn(file, &(mir->SBLOT_ID));
	_stdf_read_dtc_Cn(file, &(mir->OPER_NAM));
	_stdf_read_dtc_Cn(file, &(mir->EXEC_TYP));
	_stdf_read_dtc_Cn(file, &(mir->EXEC_VER));
	_stdf_read_dtc_Cn(file, &(mir->TEST_COD));
	_stdf_read_dtc_Cn(file, &(mir->TST_TEMP));
	_stdf_read_dtc_Cn(file, &(mir->USER_TXT));
	_stdf_read_dtc_Cn(file, &(mir->AUX_FILE));
	_stdf_read_dtc_Cn(file, &(mir->PKG_TYP));
	_stdf_read_dtc_Cn(file, &(mir->FAMILY_ID));
	_stdf_read_dtc_Cn(file, &(mir->DATE_COD));
	_stdf_read_dtc_Cn(file, &(mir->FACIL_ID));
	_stdf_read_dtc_Cn(file, &(mir->FLOOR_ID));
	_stdf_read_dtc_Cn(file, &(mir->PROC_ID));
	_stdf_read_dtc_Cn(file, &(mir->OPER_FRQ));
	_stdf_read_dtc_Cn(file, &(mir->SPEC_NAM));
	_stdf_read_dtc_Cn(file, &(mir->SPEC_VER));
	_stdf_read_dtc_Cn(file, &(mir->FLOW_ID));
	_stdf_read_dtc_Cn(file, &(mir->SETUP_ID));
	_stdf_read_dtc_Cn(file, &(mir->DSGN_REV));
	_stdf_read_dtc_Cn(file, &(mir->ENG_ID));
	_stdf_read_dtc_Cn(file, &(mir->ROM_COD));
	_stdf_read_dtc_Cn(file, &(mir->SERL_NUM));
	_stdf_read_dtc_Cn(file, &(mir->SUPR_NAM));
#ifdef STDF_VER3
	} else {
	_stdf_read_dtc_U1(file, &(mir->CPU_TYPE));
	_stdf_read_dtc_U1(file, &(mir->STDF_VER));
	_stdf_read_dtc_C1(file, &(mir->MODE_COD));
	_stdf_read_dtc_U1(file, &(mir->STAT_NUM));
	_stdf_read_dtc_Cx(file, &(mir->TEST_COD), 3);
	_stdf_read_dtc_C1(file, &(mir->RTST_COD));
	_stdf_read_dtc_C1(file, &(mir->PROT_COD));
	_stdf_read_dtc_C1(file, &(mir->CMOD_COD));
	_stdf_read_dtc_U4(file, &(mir->SETUP_T));
	_stdf_read_dtc_U4(file, &(mir->START_T));
	_stdf_read_dtc_Cn(file, &(mir->LOT_ID));
	_stdf_read_dtc_Cn(file, &(mir->PART_TYP));
	_stdf_read_dtc_Cn(file, &(mir->JOB_NAM));
	_stdf_read_dtc_Cn(file, &(mir->OPER_NAM));
	_stdf_read_dtc_Cn(file, &(mir->NODE_NAM));
	_stdf_read_dtc_Cn(file, &(mir->TSTR_TYP));
	_stdf_read_dtc_Cn(file, &(mir->EXEC_TYP));
	_stdf_read_dtc_Cn(file, &(mir->SUPR_NAM));
	_stdf_read_dtc_Cn(file, &(mir->HAND_ID));
	_stdf_read_dtc_Cn(file, &(mir->SBLOT_ID));
	_stdf_read_dtc_Cn(file, &(mir->JOB_REV));
	_stdf_read_dtc_Cn(file, &(mir->PROC_ID));
	_stdf_read_dtc_Cn(file, &(mir->PRB_CARD));
	}
#endif
	return mir;
}
rec_mrr* stdf_read_rec_mrr(stdf_file *file)
{
	rec_mrr *mrr = __malloc_rec(rec_mrr);
	_stdf_read_dtc_U4(file, &(mrr->FINISH_T));
#ifdef STDF_VER3
	if (file->ver == 3) {
	_stdf_read_dtc_U4(file, &(mrr->PART_CNT));
	_stdf_read_dtc_I4(file, &(mrr->RTST_CNT));
	_stdf_read_dtc_I4(file, &(mrr->ABRT_CNT));
	_stdf_read_dtc_I4(file, &(mrr->GOOD_CNT));
	_stdf_read_dtc_I4(file, &(mrr->FUNC_CNT));
	}
#endif
	_stdf_read_dtc_C1(file, &(mrr->DISP_COD));
	_stdf_read_dtc_Cn(file, &(mrr->USR_DESC));
	_stdf_read_dtc_Cn(file, &(mrr->EXC_DESC));
	return mrr;
}
rec_pcr* stdf_read_rec_pcr(stdf_file *file)
{
	rec_pcr *pcr = __malloc_rec(rec_pcr);
	_stdf_read_dtc_U1(file, &(pcr->HEAD_NUM));
	_stdf_read_dtc_U1(file, &(pcr->SITE_NUM));
	_stdf_read_dtc_U4(file, &(pcr->PART_CNT));
	_stdf_read_dtc_U4(file, &(pcr->RTST_CNT));
	_stdf_read_dtc_U4(file, &(pcr->ABRT_CNT));
	_stdf_read_dtc_U4(file, &(pcr->GOOD_CNT));
	_stdf_read_dtc_U4(file, &(pcr->FUNC_CNT));
	return pcr;
}
rec_hbr* stdf_read_rec_hbr(stdf_file *file)
{
	rec_hbr *hbr = __malloc_rec(rec_hbr);
	_stdf_read_dtc_U1(file, &(hbr->HEAD_NUM));
	_stdf_read_dtc_U1(file, &(hbr->SITE_NUM));
	_stdf_read_dtc_U2(file, &(hbr->HBIN_NUM));
	_stdf_read_dtc_U4(file, &(hbr->HBIN_CNT));
	_stdf_read_dtc_C1(file, &(hbr->HBIN_PF));
	_stdf_read_dtc_Cn(file, &(hbr->HBIN_NAM));
	return hbr;
}
rec_sbr* stdf_read_rec_sbr(stdf_file *file)
{
	rec_sbr *sbr = __malloc_rec(rec_sbr);
	_stdf_read_dtc_U1(file, &(sbr->HEAD_NUM));
	_stdf_read_dtc_U1(file, &(sbr->SITE_NUM));
	_stdf_read_dtc_U2(file, &(sbr->SBIN_NUM));
	_stdf_read_dtc_U4(file, &(sbr->SBIN_CNT));
	_stdf_read_dtc_C1(file, &(sbr->SBIN_PF));
	_stdf_read_dtc_Cn(file, &(sbr->SBIN_NAM));
	return sbr;
}
rec_pmr* stdf_read_rec_pmr(stdf_file *file)
{
	rec_pmr *pmr = __malloc_rec(rec_pmr);
	_stdf_read_dtc_U2(file, &(pmr->PMR_INDX));
	_stdf_read_dtc_U2(file, &(pmr->CHAN_TYP));
	_stdf_read_dtc_Cn(file, &(pmr->CHAN_NAM));
	_stdf_read_dtc_Cn(file, &(pmr->PHY_NAM));
	_stdf_read_dtc_Cn(file, &(pmr->LOG_NAM));
	_stdf_read_dtc_U1(file, &(pmr->HEAD_NUM));
	_stdf_read_dtc_U1(file, &(pmr->SITE_NUM));
	return pmr;
}
rec_pgr* stdf_read_rec_pgr(stdf_file *file)
{
	rec_pgr *pgr = __malloc_rec(rec_pgr);
	_stdf_read_dtc_U2(file, &(pgr->GRP_INDX));
	_stdf_read_dtc_Cn(file, &(pgr->GRP_NAM));
	_stdf_read_dtc_U2(file, &(pgr->INDX_CNT));
	_stdf_read_dtc_xU2(file, &(pgr->PMR_INDX), pgr->INDX_CNT);
	return pgr;
}
rec_plr* stdf_read_rec_plr(stdf_file *file)
{
	rec_plr *plr = __malloc_rec(rec_plr);
	warn_untested("PLR");
	_stdf_read_dtc_U2(file, &(plr->GRP_CNT));
	_stdf_read_dtc_xU2(file, &(plr->GRP_INDX), plr->GRP_CNT);
	_stdf_read_dtc_xU2(file, &(plr->GRP_MODE), plr->GRP_CNT);
	_stdf_read_dtc_xU1(file, &(plr->GRP_RADX), plr->GRP_CNT);
	_stdf_read_dtc_xCn(file, &(plr->PGM_CHAR), plr->GRP_CNT);
	_stdf_read_dtc_xCn(file, &(plr->RTN_CHAR), plr->GRP_CNT);
	_stdf_read_dtc_xCn(file, &(plr->PGM_CHAL), plr->GRP_CNT);
	_stdf_read_dtc_xCn(file, &(plr->RTN_CHAL), plr->GRP_CNT);
	return plr;
}
rec_rdr* stdf_read_rec_rdr(stdf_file *file)
{
	rec_rdr *rdr = __malloc_rec(rec_rdr);
	_stdf_read_dtc_U2(file, &(rdr->NUM_BINS));
	_stdf_read_dtc_xU2(file, &(rdr->RTST_BIN), rdr->NUM_BINS);
	return rdr;
}
rec_sdr* stdf_read_rec_sdr(stdf_file *file)
{
	rec_sdr *sdr = __malloc_rec(rec_sdr);
	_stdf_read_dtc_U1(file, &(sdr->HEAD_NUM));
	_stdf_read_dtc_U1(file, &(sdr->SITE_GRP));
	_stdf_read_dtc_U1(file, &(sdr->SITE_CNT));
	_stdf_read_dtc_xU1(file, &(sdr->SITE_NUM), sdr->SITE_CNT);
	_stdf_read_dtc_Cn(file, &(sdr->HAND_TYP));
	_stdf_read_dtc_Cn(file, &(sdr->HAND_ID));
	_stdf_read_dtc_Cn(file, &(sdr->CARD_TYP));
	_stdf_read_dtc_Cn(file, &(sdr->CARD_ID));
	_stdf_read_dtc_Cn(file, &(sdr->LOAD_TYP));
	_stdf_read_dtc_Cn(file, &(sdr->LOAD_ID));
	_stdf_read_dtc_Cn(file, &(sdr->DIB_TYP));
	_stdf_read_dtc_Cn(file, &(sdr->DIB_ID));
	_stdf_read_dtc_Cn(file, &(sdr->CABL_TYP));
	_stdf_read_dtc_Cn(file, &(sdr->CABL_ID));
	_stdf_read_dtc_Cn(file, &(sdr->CONT_TYP));
	_stdf_read_dtc_Cn(file, &(sdr->CONT_ID));
	_stdf_read_dtc_Cn(file, &(sdr->LASR_TYP));
	_stdf_read_dtc_Cn(file, &(sdr->LASR_ID));
	_stdf_read_dtc_Cn(file, &(sdr->EXTR_TYP));
	_stdf_read_dtc_Cn(file, &(sdr->EXTR_ID));
	return sdr;
}
rec_wir* stdf_read_rec_wir(stdf_file *file)
{
	rec_wir *wir = __malloc_rec(rec_wir);
	_stdf_read_dtc_U1(file, &(wir->HEAD_NUM));
#ifdef STDF_VER3
	if (file->ver == 3)
	_stdf_read_dtc_B1(file, &(wir->PAD_BYTE));
	else
#endif
	_stdf_read_dtc_U1(file, &(wir->SITE_GRP));
	_stdf_read_dtc_U4(file, &(wir->START_T));
	_stdf_read_dtc_Cn(file, &(wir->WAFER_ID));
	return wir;
}
rec_wrr* stdf_read_rec_wrr(stdf_file *file)
{
	rec_wrr *wrr = __malloc_rec(rec_wrr);
#ifdef STDF_VER3
	if (file->ver == 4) {
#endif
	_stdf_read_dtc_U1(file, &(wrr->HEAD_NUM));
	_stdf_read_dtc_U1(file, &(wrr->SITE_GRP));
	_stdf_read_dtc_U4(file, &(wrr->FINISH_T));
#ifdef STDF_VER3
	} else {
	_stdf_read_dtc_U4(file, &(wrr->FINISH_T));
	_stdf_read_dtc_U1(file, &(wrr->HEAD_NUM));
	_stdf_read_dtc_B1(file, &(wrr->PAD_BYTE));
	}
#endif
	_stdf_read_dtc_U4(file, &(wrr->PART_CNT));
	_stdf_read_dtc_U4(file, &(wrr->RTST_CNT));
	_stdf_read_dtc_U4(file, &(wrr->ABRT_CNT));
	_stdf_read_dtc_U4(file, &(wrr->GOOD_CNT));
	_stdf_read_dtc_U4(file, &(wrr->FUNC_CNT));
	_stdf_read_dtc_Cn(file, &(wrr->WAFER_ID));
#ifdef STDF_VER3
	if (file->ver == 4) {
#endif
	_stdf_read_dtc_Cn(file, &(wrr->FABWF_ID));
	_stdf_read_dtc_Cn(file, &(wrr->FRAME_ID));
	_stdf_read_dtc_Cn(file, &(wrr->MASK_ID));
#ifdef STDF_VER3
	} else {
	_stdf_read_dtc_Cn(file, &(wrr->HAND_ID));
	_stdf_read_dtc_Cn(file, &(wrr->PRB_CARD));
	}
#endif
	_stdf_read_dtc_Cn(file, &(wrr->USR_DESC));
	_stdf_read_dtc_Cn(file, &(wrr->EXC_DESC));
	return wrr;
}
rec_wcr* stdf_read_rec_wcr(stdf_file *file)
{
	rec_wcr *wcr = __malloc_rec(rec_wcr);
	_stdf_read_dtc_R4(file, &(wcr->WAFR_SIZ));
	_stdf_read_dtc_R4(file, &(wcr->DIE_HT));
	_stdf_read_dtc_R4(file, &(wcr->DIE_WID));
	_stdf_read_dtc_U1(file, &(wcr->WF_UNITS));
	_stdf_read_dtc_C1(file, &(wcr->WF_FLAT));
	_stdf_read_dtc_I2(file, &(wcr->CENTER_X));
	_stdf_read_dtc_I2(file, &(wcr->CENTER_Y));
	_stdf_read_dtc_C1(file, &(wcr->POS_X));
	_stdf_read_dtc_C1(file, &(wcr->POS_Y));
	return wcr;
}
rec_pir* stdf_read_rec_pir(stdf_file *file)
{
	rec_pir *pir = __malloc_rec(rec_pir);
	_stdf_read_dtc_U1(file, &(pir->HEAD_NUM));
	_stdf_read_dtc_U1(file, &(pir->SITE_NUM));
#ifdef STDF_VER3
	if (file->ver == 3) {
	_stdf_read_dtc_I2(file, &(pir->X_COORD));
	_stdf_read_dtc_I2(file, &(pir->Y_COORD));
	_stdf_read_dtc_Cn(file, &(pir->PART_ID));
	}
#endif
	return pir;
}
rec_prr* stdf_read_rec_prr(stdf_file *file)
{
	rec_prr *prr = __malloc_rec(rec_prr);
	_stdf_read_dtc_U1(file, &(prr->HEAD_NUM));
	_stdf_read_dtc_U1(file, &(prr->SITE_NUM));
#ifdef STDF_VER3
	if (file->ver == 4)
#endif
	_stdf_read_dtc_B1(file, &(prr->PART_FLG));
	_stdf_read_dtc_U2(file, &(prr->NUM_TEST));
	_stdf_read_dtc_U2(file, &(prr->HARD_BIN));
	_stdf_read_dtc_U2(file, &(prr->SOFT_BIN));
#ifdef STDF_VER3
	if (file->ver == 3) {
	_stdf_read_dtc_B1(file, &(prr->PART_FLG));
	_stdf_read_dtc_B1(file, &(prr->PAD_BYTE));
	}
#endif
	_stdf_read_dtc_I2(file, &(prr->X_COORD));
	_stdf_read_dtc_I2(file, &(prr->Y_COORD));
#ifdef STDF_VER3
	if (file->ver == 4)
#endif
	_stdf_read_dtc_U4(file, &(prr->TEST_T));
	_stdf_read_dtc_Cn(file, &(prr->PART_ID));
	_stdf_read_dtc_Cn(file, &(prr->PART_TXT));
	_stdf_read_dtc_Bn(file, &(prr->PART_FIX));
	return prr;
}
#ifdef STDF_VER3
rec_pdr* stdf_read_rec_pdr(stdf_file *file)
{
	rec_pdr *pdr = __malloc_rec(rec_pdr);
	_stdf_read_dtc_U4(file, &(pdr->TEST_NUM));
	_stdf_read_dtc_B1(file, &(pdr->DESC_FLG));
	_stdf_read_dtc_B1(file, &(pdr->OPT_FLAG));
	_stdf_read_dtc_I1(file, &(pdr->RES_SCAL));
	_stdf_read_dtc_Cx(file, &(pdr->UNITS), 7);
	_stdf_read_dtc_U1(file, &(pdr->RES_LDIG));
	_stdf_read_dtc_U1(file, &(pdr->RES_RDIG));
	_stdf_read_dtc_I1(file, &(pdr->LLM_SCAL));
	_stdf_read_dtc_I1(file, &(pdr->HLM_SCAL));
	_stdf_read_dtc_U1(file, &(pdr->LLM_LDIG));
	_stdf_read_dtc_U1(file, &(pdr->LLM_RDIG));
	_stdf_read_dtc_U1(file, &(pdr->HLM_LDIG));
	_stdf_read_dtc_U1(file, &(pdr->HLM_RDIG));
	_stdf_read_dtc_R4(file, &(pdr->LO_LIMIT));
	_stdf_read_dtc_R4(file, &(pdr->HI_LIMIT));
	_stdf_read_dtc_Cn(file, &(pdr->TEST_NAM));
	_stdf_read_dtc_Cn(file, &(pdr->SEQ_NAME));
	return pdr;
}
rec_fdr* stdf_read_rec_fdr(stdf_file *file)
{
	rec_fdr *fdr = __malloc_rec(rec_fdr);
	_stdf_read_dtc_U4(file, &(fdr->TEST_NUM));
	_stdf_read_dtc_B1(file, &(fdr->DESC_FLG));
	_stdf_read_dtc_Cn(file, &(fdr->TEST_NAM));
	_stdf_read_dtc_Cn(file, &(fdr->SEQ_NAME));
	return fdr;
}
#endif
rec_tsr* stdf_read_rec_tsr(stdf_file *file)
{
	rec_tsr *tsr = __malloc_rec(rec_tsr);
	_stdf_read_dtc_U1(file, &(tsr->HEAD_NUM));
	_stdf_read_dtc_U1(file, &(tsr->SITE_NUM));
#ifdef STDF_VER3
	if (file->ver == 4)
#endif
	_stdf_read_dtc_C1(file, &(tsr->TEST_TYP));
	_stdf_read_dtc_U4(file, &(tsr->TEST_NUM));
	_stdf_read_dtc_U4(file, &(tsr->EXEC_CNT));
	_stdf_read_dtc_U4(file, &(tsr->FAIL_CNT));
	_stdf_read_dtc_U4(file, &(tsr->ALRM_CNT));
#ifdef STDF_VER3
	if (file->ver == 4) {
#endif
	_stdf_read_dtc_Cn(file, &(tsr->TEST_NAM));
	_stdf_read_dtc_Cn(file, &(tsr->SEQ_NAME));
	_stdf_read_dtc_Cn(file, &(tsr->TEST_LBL));
	_stdf_read_dtc_B1(file, &(tsr->OPT_FLAG));
	_stdf_read_dtc_R4(file, &(tsr->TEST_TIM));
	_stdf_read_dtc_R4(file, &(tsr->TEST_MIN));
	_stdf_read_dtc_R4(file, &(tsr->TEST_MAX));
	_stdf_read_dtc_R4(file, &(tsr->TST_SUMS));
	_stdf_read_dtc_R4(file, &(tsr->TST_SQRS));
#ifdef STDF_VER3
	} else {
	_stdf_read_dtc_B1(file, &(tsr->OPT_FLAG));
	_stdf_read_dtc_B1(file, &(tsr->PAD_BYTE));
	_stdf_read_dtc_R4(file, &(tsr->TEST_MIN));
	_stdf_read_dtc_R4(file, &(tsr->TEST_MAX));
	_stdf_read_dtc_R4(file, &(tsr->TST_MEAN));
	_stdf_read_dtc_R4(file, &(tsr->TST_SDEV));
	_stdf_read_dtc_R4(file, &(tsr->TST_SUMS));
	_stdf_read_dtc_R4(file, &(tsr->TST_SQRS));
	_stdf_read_dtc_Cn(file, &(tsr->TEST_NAM));
	_stdf_read_dtc_Cn(file, &(tsr->SEQ_NAME));
	}
#endif
	return tsr;
}
rec_ptr* stdf_read_rec_ptr(stdf_file *file)
{
	rec_ptr *ptr = __malloc_rec(rec_ptr);
	_stdf_read_dtc_U4(file, &(ptr->TEST_NUM));
	_stdf_read_dtc_U1(file, &(ptr->HEAD_NUM));
	_stdf_read_dtc_U1(file, &(ptr->SITE_NUM));
	_stdf_read_dtc_B1(file, &(ptr->TEST_FLG));
	_stdf_read_dtc_B1(file, &(ptr->PARM_FLG));
	_stdf_read_dtc_R4(file, &(ptr->RESULT));
	_stdf_read_dtc_Cn(file, &(ptr->TEST_TXT));
	_stdf_read_dtc_Cn(file, &(ptr->ALARM_ID));
	_stdf_read_dtc_B1(file, &(ptr->OPT_FLAG));
	_stdf_read_dtc_I1(file, &(ptr->RES_SCAL));
	_stdf_read_dtc_I1(file, &(ptr->LLM_SCAL));
	_stdf_read_dtc_I1(file, &(ptr->HLM_SCAL));
	_stdf_read_dtc_R4(file, &(ptr->LO_LIMIT));
	_stdf_read_dtc_R4(file, &(ptr->HI_LIMIT));
	_stdf_read_dtc_Cn(file, &(ptr->UNITS));
	_stdf_read_dtc_Cn(file, &(ptr->C_RESFMT));
	_stdf_read_dtc_Cn(file, &(ptr->C_LLMFMT));
	_stdf_read_dtc_Cn(file, &(ptr->C_HLMFMT));
	_stdf_read_dtc_R4(file, &(ptr->LO_SPEC));
	_stdf_read_dtc_R4(file, &(ptr->HI_SPEC));
	return ptr;
}
rec_mpr* stdf_read_rec_mpr(stdf_file *file)
{
	rec_mpr *mpr = __malloc_rec(rec_mpr);
	warn_untested("MPR");
	_stdf_read_dtc_U4(file, &(mpr->TEST_NUM));
	_stdf_read_dtc_U1(file, &(mpr->HEAD_NUM));
	_stdf_read_dtc_U1(file, &(mpr->SITE_NUM));
	_stdf_read_dtc_B1(file, &(mpr->TEST_FLG));
	_stdf_read_dtc_B1(file, &(mpr->PARM_FLG));
	_stdf_read_dtc_U2(file, &(mpr->RTN_ICNT));
	_stdf_read_dtc_U2(file, &(mpr->RSLT_CNT));
	_stdf_read_dtc_xN1(file, &(mpr->RTN_STAT), mpr->RTN_ICNT);
	_stdf_read_dtc_xR4(file, &(mpr->RTN_RSLT), mpr->RSLT_CNT);
	_stdf_read_dtc_Cn(file, &(mpr->TEST_TXT));
	_stdf_read_dtc_Cn(file, &(mpr->ALARM_ID));
	_stdf_read_dtc_B1(file, &(mpr->OPT_FLAG));
	_stdf_read_dtc_I1(file, &(mpr->RES_SCAL));
	_stdf_read_dtc_I1(file, &(mpr->LLM_SCAL));
	_stdf_read_dtc_I1(file, &(mpr->HLM_SCAL));
	_stdf_read_dtc_R4(file, &(mpr->LO_LIMIT));
	_stdf_read_dtc_R4(file, &(mpr->HI_LIMIT));
	_stdf_read_dtc_R4(file, &(mpr->START_IN));
	_stdf_read_dtc_R4(file, &(mpr->INCR_IN));
	_stdf_read_dtc_xU2(file, &(mpr->RTN_INDX), mpr->RTN_ICNT);
	_stdf_read_dtc_Cn(file, &(mpr->UNITS));
	_stdf_read_dtc_Cn(file, &(mpr->UNITS_IN));
	_stdf_read_dtc_Cn(file, &(mpr->C_RESFMT));
	_stdf_read_dtc_Cn(file, &(mpr->C_LLMFMT));
	_stdf_read_dtc_Cn(file, &(mpr->C_HLMFMT));
	_stdf_read_dtc_R4(file, &(mpr->LO_SPEC));
	_stdf_read_dtc_R4(file, &(mpr->HI_SPEC));
	return mpr;
}
rec_ftr* stdf_read_rec_ftr(stdf_file *file)
{
	rec_ftr *ftr = __malloc_rec(rec_ftr);
	warn_untested("FTR");
	_stdf_read_dtc_U4(file, &(ftr->TEST_NUM));
	_stdf_read_dtc_U1(file, &(ftr->HEAD_NUM));
	_stdf_read_dtc_U1(file, &(ftr->SITE_NUM));
	_stdf_read_dtc_B1(file, &(ftr->TEST_FLG));
	_stdf_read_dtc_B1(file, &(ftr->OPT_FLG));
	_stdf_read_dtc_U4(file, &(ftr->CYCL_CNT));
	_stdf_read_dtc_U4(file, &(ftr->REL_VADR));
	_stdf_read_dtc_U4(file, &(ftr->REPT_CNT));
	_stdf_read_dtc_U4(file, &(ftr->NUM_FAIL));
	_stdf_read_dtc_I4(file, &(ftr->XFAIL_AD));
	_stdf_read_dtc_I4(file, &(ftr->YFAIL_AD));
	_stdf_read_dtc_I2(file, &(ftr->VECT_OFF));
	_stdf_read_dtc_U2(file, &(ftr->RTN_ICNT));
	_stdf_read_dtc_U2(file, &(ftr->PGM_ICNT));
	_stdf_read_dtc_xU2(file, &(ftr->RTN_INDX), ftr->RTN_ICNT);
	_stdf_read_dtc_xN1(file, &(ftr->RTN_STAT), ftr->RTN_ICNT);
	_stdf_read_dtc_xU2(file, &(ftr->PGM_INDX), ftr->PGM_ICNT);
	_stdf_read_dtc_xN1(file, &(ftr->PGM_STAT), ftr->PGM_ICNT);
	_stdf_read_dtc_Dn(file, &(ftr->FAIL_PIN));
	_stdf_read_dtc_Cn(file, &(ftr->VECT_NAM));
	_stdf_read_dtc_Cn(file, &(ftr->TIME_SET));
	_stdf_read_dtc_Cn(file, &(ftr->OP_CODE));
	_stdf_read_dtc_Cn(file, &(ftr->TEST_TXT));
	_stdf_read_dtc_Cn(file, &(ftr->ALARM_ID));
	_stdf_read_dtc_Cn(file, &(ftr->PROG_TXT));
	_stdf_read_dtc_Cn(file, &(ftr->RSLT_TXT));
	_stdf_read_dtc_U1(file, &(ftr->PATG_NUM));
	_stdf_read_dtc_Dn(file, &(ftr->SPIN_MAP));
	return ftr;
}
rec_bps* stdf_read_rec_bps(stdf_file *file)
{
	rec_bps *bps = __malloc_rec(rec_bps);
	_stdf_read_dtc_Cn(file, &(bps->SEQ_NAME));
	return bps;
}
rec_eps* stdf_read_rec_eps(stdf_file *file)
{
	rec_eps *eps = __malloc_rec(rec_eps);
	return eps;
}
#ifdef STDF_VER3
rec_shb* stdf_read_rec_shb(stdf_file *file)
{
	rec_shb *shb = __malloc_rec(rec_shb);
	_stdf_read_dtc_U1(file, &(shb->HEAD_NUM));
	_stdf_read_dtc_U1(file, &(shb->SITE_NUM));
	_stdf_read_dtc_U2(file, &(shb->HBIN_NUM));
	_stdf_read_dtc_U4(file, &(shb->HBIN_CNT));
	_stdf_read_dtc_Cn(file, &(shb->HBIN_NAM));
	return shb;
}
rec_ssb* stdf_read_rec_ssb(stdf_file *file)
{
	rec_ssb *ssb = __malloc_rec(rec_ssb);
	_stdf_read_dtc_U1(file, &(ssb->HEAD_NUM));
	_stdf_read_dtc_U1(file, &(ssb->SITE_NUM));
	_stdf_read_dtc_U2(file, &(ssb->SBIN_NUM));
	_stdf_read_dtc_U4(file, &(ssb->SBIN_CNT));
	_stdf_read_dtc_Cn(file, &(ssb->SBIN_NAM));
	return ssb;
}
rec_sts* stdf_read_rec_sts(stdf_file *file)
{
	rec_sts *sts = __malloc_rec(rec_sts);
	_stdf_read_dtc_U1(file, &(sts->HEAD_NUM));
	_stdf_read_dtc_U1(file, &(sts->SITE_NUM));
	_stdf_read_dtc_U4(file, &(sts->TEST_NUM));
	_stdf_read_dtc_I4(file, &(sts->EXEC_CNT));
	_stdf_read_dtc_I4(file, &(sts->FAIL_CNT));
	_stdf_read_dtc_I4(file, &(sts->ALRM_CNT));
	_stdf_read_dtc_B1(file, &(sts->OPT_FLAG));
	_stdf_read_dtc_B1(file, &(sts->PAD_BYTE));
	_stdf_read_dtc_R4(file, &(sts->TEST_MIN));
	_stdf_read_dtc_R4(file, &(sts->TEST_MAX));
	_stdf_read_dtc_R4(file, &(sts->TST_MEAN));
	_stdf_read_dtc_R4(file, &(sts->TST_SDEV));
	_stdf_read_dtc_R4(file, &(sts->TST_SUMS));
	_stdf_read_dtc_R4(file, &(sts->TST_SQRS));
	_stdf_read_dtc_Cn(file, &(sts->TEST_NAM));
	_stdf_read_dtc_Cn(file, &(sts->SEQ_NAME));
	_stdf_read_dtc_Cn(file, &(sts->TEST_LBL));
	return sts;
}
rec_scr* stdf_read_rec_scr(stdf_file *file)
{
	rec_scr *scr = __malloc_rec(rec_scr);
	_stdf_read_dtc_U1(file, &(scr->HEAD_NUM));
	_stdf_read_dtc_U1(file, &(scr->SITE_NUM));
	_stdf_read_dtc_U4(file, &(scr->FINISH_T));
	_stdf_read_dtc_U4(file, &(scr->PART_CNT));
	_stdf_read_dtc_I4(file, &(scr->RTST_CNT));
	_stdf_read_dtc_I4(file, &(scr->ABRT_CNT));
	_stdf_read_dtc_I4(file, &(scr->GOOD_CNT));
	_stdf_read_dtc_I4(file, &(scr->FUNC_CNT));
	return scr;
}
#endif
rec_gdr* stdf_read_rec_gdr(stdf_file *file)
{
	rec_gdr *gdr = __malloc_rec(rec_gdr);
	_stdf_read_dtc_U2(file, &(gdr->FLD_CNT));
	_stdf_read_dtc_Vn(file, &(gdr->GEN_DATA), gdr->FLD_CNT);
	return gdr;
}
rec_dtr* stdf_read_rec_dtr(stdf_file *file)
{
	rec_dtr *dtr = __malloc_rec(rec_dtr);
	_stdf_read_dtc_Cn(file, &(dtr->TEXT_DAT));
	return dtr;
}

void stdf_free_record(rec_unknown *rec)
{
	stdf_file *file;
	if (!rec)
		return;
	file = (stdf_file*)(rec->header.stdf_file);

	if (rec->header.state == REC_STATE_RAW) {
		free(rec->data);
		free(rec);
		return;
	}

	switch (HEAD_TO_REC(rec->header)) {
		case REC_FAR: {
			free(rec);
			break;
		}
		case REC_ATR: {
			rec_atr *atr = (rec_atr*)rec;
			free(atr->CMD_LINE);
			free(atr);
			break;
		}
		case REC_MIR: {
			rec_mir *mir = (rec_mir*)rec;
#ifdef STDF_VER3
			if (file->ver == 4) {
#endif
			free(mir->LOT_ID);
			free(mir->PART_TYP);
			free(mir->NODE_NAM);
			free(mir->TSTR_TYP);
			free(mir->JOB_NAM);
			free(mir->JOB_REV);
			free(mir->SBLOT_ID);
			free(mir->OPER_NAM);
			free(mir->EXEC_TYP);
			free(mir->EXEC_VER);
			free(mir->TEST_COD);
			free(mir->TST_TEMP);
			free(mir->USER_TXT);
			free(mir->AUX_FILE);
			free(mir->PKG_TYP);
			free(mir->FAMILY_ID);
			free(mir->DATE_COD);
			free(mir->FACIL_ID);
			free(mir->FLOOR_ID);
			free(mir->PROC_ID);
			free(mir->OPER_FRQ);
			free(mir->SPEC_NAM);
			free(mir->SPEC_VER);
			free(mir->FLOW_ID);
			free(mir->SETUP_ID);
			free(mir->DSGN_REV);
			free(mir->ENG_ID);
			free(mir->ROM_COD);
			free(mir->SERL_NUM);
			free(mir->SUPR_NAM);
#ifdef STDF_VER3
			} else {
			free(mir->TEST_COD);
			free(mir->LOT_ID);
			free(mir->PART_TYP);
			free(mir->JOB_NAM);
			free(mir->OPER_NAM);
			free(mir->NODE_NAM);
			free(mir->TSTR_TYP);
			free(mir->EXEC_TYP);
			free(mir->SUPR_NAM);
			free(mir->HAND_ID);
			free(mir->SBLOT_ID);
			free(mir->JOB_REV);
			free(mir->PROC_ID);
			free(mir->PRB_CARD);
			}
#endif
			free(mir);
			break;
		}
		case REC_MRR: {
			rec_mrr *mrr = (rec_mrr*)rec;
			free(mrr->USR_DESC);
			free(mrr->EXC_DESC);
			free(rec);
			break;
		}
		case REC_PCR: {
			free(rec);
			break;
		}
		case REC_HBR: {
			rec_hbr *hbr = (rec_hbr*)rec;
			free(hbr->HBIN_NAM);
			free(rec);
			break;
		}
		case REC_SBR: {
			rec_sbr *sbr = (rec_sbr*)rec;
			free(sbr->SBIN_NAM);
			free(rec);
			break;
		}
		case REC_PMR: {
			rec_pmr *pmr = (rec_pmr*)rec;
			free(pmr->CHAN_NAM);
			free(pmr->PHY_NAM);
			free(pmr->LOG_NAM);
			free(rec);
			break;
		}
		case REC_PGR: {
			rec_pgr *pgr = (rec_pgr*)rec;
			free(pgr->GRP_NAM);
			free_xU2(pgr->PMR_INDX);
			free(rec);
			break;
		}
		case REC_PLR: {
			rec_plr *plr = (rec_plr*)rec;
			free_xU2(plr->GRP_INDX);
			free_xU2(plr->GRP_MODE);
			free_xU1(plr->GRP_RADX);
			free_xCn(plr->PGM_CHAR, plr->GRP_CNT);
			free_xCn(plr->RTN_CHAR, plr->GRP_CNT);
			free_xCn(plr->PGM_CHAL, plr->GRP_CNT);
			free_xCn(plr->RTN_CHAL, plr->GRP_CNT);
			free(rec);
			break;
		}
		case REC_RDR: {
			rec_rdr *rdr = (rec_rdr*)rec;
			free_xU2(rdr->RTST_BIN);
			free(rec);
			break;
		}
		case REC_SDR: {
			rec_sdr *sdr = (rec_sdr*)rec;
			free_xU1(sdr->SITE_NUM);
			free(sdr->HAND_TYP);
			free(sdr->HAND_ID);
			free(sdr->CARD_TYP);
			free(sdr->CARD_ID);
			free(sdr->LOAD_TYP);
			free(sdr->LOAD_ID);
			free(sdr->DIB_TYP);
			free(sdr->DIB_ID);
			free(sdr->CABL_TYP);
			free(sdr->CABL_ID);
			free(sdr->CONT_TYP);
			free(sdr->CONT_ID);
			free(sdr->LASR_TYP);
			free(sdr->LASR_ID);
			free(sdr->EXTR_TYP);
			free(sdr->EXTR_ID);
			free(rec);
			break;
		}
		case REC_WIR: {
			rec_wir *wir = (rec_wir*)rec;
			free(wir->WAFER_ID);
			free(rec);
			break;
		}
		case REC_WRR: {
			rec_wrr *wrr = (rec_wrr*)rec;
			free(wrr->WAFER_ID);
#ifdef STDF_VER3
			if (file->ver == 4) {
#endif
			free(wrr->FABWF_ID);
			free(wrr->FRAME_ID);
			free(wrr->MASK_ID);
#ifdef STDF_VER3
			} else {
			free(wrr->HAND_ID);
			free(wrr->PRB_CARD);
			}
#endif
			free(wrr->USR_DESC);
			free(wrr->EXC_DESC);
			free(rec);
			break;
		}
		case REC_WCR: {
			free(rec);
			break;
		}
		case REC_PIR: {
#ifdef STDF_VER3
			rec_pir *pir = (rec_pir*)rec;
			if (file->ver == 3)
			free(pir->PART_ID);
#endif
			free(rec);
			break;
		}
		case REC_PRR: {
			rec_prr *prr = (rec_prr*)rec;
			free(prr->PART_ID);
			free(prr->PART_TXT);
			free(prr->PART_FIX);
			free(rec);
			break;
		}
#ifdef STDF_VER3
		case REC_PDR: {
			rec_pdr *pdr = (rec_pdr*)rec;
			free(pdr->UNITS);
			free(pdr->TEST_NAM);
			free(pdr->SEQ_NAME);
			free(rec);
			break;
		}
		case REC_FDR: {
			rec_fdr *fdr = (rec_fdr*)rec;
			free(fdr->TEST_NAM);
			free(fdr->SEQ_NAME);
			free(rec);
			break;
		}
#endif
		case REC_TSR: {
			rec_tsr *tsr = (rec_tsr*)rec;
			free(tsr->TEST_NAM);
			free(tsr->SEQ_NAME);
#ifdef STDF_VER3
			if (file->ver == 4)
#endif
			free(tsr->TEST_LBL);
			free(rec);
			break;
		}
		case REC_PTR: {
			rec_ptr *ptr = (rec_ptr*)rec;
			free(ptr->TEST_TXT);
			free(ptr->ALARM_ID);
			free(ptr->UNITS);
			free(ptr->C_RESFMT);
			free(ptr->C_LLMFMT);
			free(ptr->C_HLMFMT);
			free(rec);
			break;
		}
		case REC_MPR: {
			rec_mpr *mpr = (rec_mpr*)rec;
			free(mpr->RTN_STAT);
			free_xR4(mpr->RTN_RSLT);
			free(mpr->TEST_TXT);
			free(mpr->ALARM_ID);
			free_xU2(mpr->RTN_INDX);
			free(mpr->UNITS);
			free(mpr->UNITS_IN);
			free(mpr->C_RESFMT);
			free(mpr->C_LLMFMT);
			free(mpr->C_HLMFMT);
			free(rec);
			break;
		}
		case REC_FTR: {
			rec_ftr *ftr = (rec_ftr*)rec;
			free_xU2(ftr->RTN_INDX);
			free(ftr->RTN_STAT);
			free_xU2(ftr->PGM_INDX);
			free(ftr->PGM_STAT);
			free(ftr->FAIL_PIN);
			free(ftr->VECT_NAM);
			free(ftr->TIME_SET);
			free(ftr->OP_CODE);
			free(ftr->TEST_TXT);
			free(ftr->ALARM_ID);
			free(ftr->PROG_TXT);
			free(ftr->RSLT_TXT);
			free(ftr->SPIN_MAP);
			free(rec);
			break;
		}
		case REC_BPS: {
			rec_bps *bps = (rec_bps*)rec;
			free(bps->SEQ_NAME);
			free(rec);
			break;
		}
		case REC_EPS: {
			free(rec);
			break;
		}
#ifdef STDF_VER3
		case REC_SHB: {
			rec_shb *shb = (rec_shb*)rec;
			free(shb->HBIN_NAM);
			free(rec);
			break;
		}
		case REC_SSB: {
			rec_ssb *ssb = (rec_ssb*)rec;
			free(ssb->SBIN_NAM);
			free(rec);
			break;
		}
		case REC_STS: {
			rec_sts *sts = (rec_sts*)rec;
			free(sts->TEST_NAM);
			free(sts->SEQ_NAME);
			free(sts->TEST_LBL);
			free(rec);
			break;
		}
		case REC_SCR: {
			free(rec);
			break;
		}
#endif
		case REC_GDR: {
			rec_gdr *gdr = (rec_gdr*)rec;
			free_Vn(gdr->GEN_DATA, gdr->FLD_CNT);
			free(rec);
			break;
		}
		case REC_DTR: {
			rec_dtr *dtr = (rec_dtr*)rec;
			free(dtr->TEXT_DAT);
			free(rec);
			break;
		}
		case REC_UNKNOWN: {
			if (rec->data) free(rec->data);
			free(rec);
			break;
		}
		default: {
			fprintf(stderr, "Memory leak: unhandled record type (%s) in stdf_free_record()!\n",
			       stdf_get_rec_name(rec->header.REC_TYP, rec->header.REC_SUB));
			break;
		}
	}

	rec = NULL;
}
