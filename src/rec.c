/**
 * @file rec.c
 * @brief Record specific function implementations.
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


/*
 * Misc utility functions
 */


static char *stdf_rec_names[] = {
	"???",
	"FAR", "ATR", "MIR", "MRR", "PCR", "HBR",
	"SBR", "PMR", "PGR", "PLR", "RDR", "SDR",
	"WIR", "WRR", "WCR", "PIR", "PRR",
	"PDR", "FDR",
	"TSR", "PTR", "MPR", "FTR", "BPS", "EPS",
	"SHB", "SSB", "STS", "SCR",
	"GDR", "DTR",
	"VUR", "PSR", "STR", "NMR", "CNR", "SSR",
	"CDR"
};

static int _stdf_header_to_idx(stdf_rec_typ type, stdf_rec_sub sub)
{
	switch (MAKE_REC(type, sub)) {
		case STDF_REC_FAR:	return 1;
		case STDF_REC_ATR:	return 2;
		case STDF_REC_MIR:	return 3;
		case STDF_REC_MRR:	return 4;
		case STDF_REC_PCR:	return 5;
		case STDF_REC_HBR:	return 6;
		case STDF_REC_SBR:	return 7;
		case STDF_REC_PMR:	return 8;
		case STDF_REC_PGR:	return 9;
		case STDF_REC_PLR:	return 10;
		case STDF_REC_RDR:	return 11;
		case STDF_REC_SDR:	return 12;
		case STDF_REC_WIR:	return 13;
		case STDF_REC_WRR:	return 14;
		case STDF_REC_WCR:	return 15;
		case STDF_REC_PIR:	return 16;
		case STDF_REC_PRR:	return 17;
#ifdef STDF_VER3
		case STDF_REC_PDR:	return 18;
		case STDF_REC_FDR:	return 19;
#endif
		case STDF_REC_TSR:	return 20;
		case STDF_REC_PTR:	return 21;
		case STDF_REC_MPR:	return 22;
		case STDF_REC_FTR:	return 23;
		case STDF_REC_BPS:	return 24;
		case STDF_REC_EPS:	return 25;
#ifdef STDF_VER3
		case STDF_REC_SHB:	return 26;
		case STDF_REC_SSB:	return 27;
		case STDF_REC_STS:	return 28;
		case STDF_REC_SCR:	return 29;
#endif
		case STDF_REC_GDR:	return 30;
		case STDF_REC_DTR:	return 31;
                case STDF_REC_VUR:      return 32;
                case STDF_REC_PSR:      return 33;
                case STDF_REC_STR:      return 34;
                case STDF_REC_NMR:      return 35;
                case STDF_REC_CNR:      return 36;
                case STDF_REC_SSR:      return 37;
                case STDF_REC_CDR:      return 38;
		case STDF_REC_UNKNOWN:
		default:		return 0;
	}
}
int stdf_rec_to_idx_count(void)
{
	return 39;
}
int stdf_rec_name_to_idx(char *stdf_rec_name)
{
	int i = stdf_rec_to_idx_count();
	while (--i > 0)
		if (strncmp(stdf_rec_name, stdf_rec_names[i], 3) == 0)
			return i;
	return 0;
}
int stdf_rec_to_idx(void *stdf_rec_void)
{
	stdf_rec_unknown *rec = (stdf_rec_unknown*)stdf_rec_void;
	return _stdf_header_to_idx(rec->header.REC_TYP, rec->header.REC_SUB);
}
char* stdf_get_rec_name(stdf_rec_typ type, stdf_rec_sub sub)
{
	return stdf_rec_names[_stdf_header_to_idx(type, sub)];
}

#ifdef STDF_VER3
/* The V4 spec changed types of some fields from V3.  Use an
 * union to get warning free access to the pointer conversion.
 */
# define _stdf_read_dtc_as(file, ptr, type1, type2) \
	do { \
		union { \
			stdf_dtc_##type1 *t1; \
			stdf_dtc_##type2 *t2; \
		} u; \
		u.t1 = ptr; \
		_stdf_read_dtc_##type2(file, u.t2); \
	} while (0)
#endif


/*
 * Memory handling functions
 */


void stdf_free_record(stdf_rec_unknown *rec)
{
#ifdef STDF_VER3
	stdf_file *file;
#endif

	if (!rec)
		return;

#ifdef STDF_VER3
	file = (stdf_file*)(rec->header.stdf_file);
#endif

	if (rec->header.state == STDF_REC_STATE_RAW) {
		free(rec->data);
		free(rec);
		return;
	}

	/* record order is based on frequency in 'standard' files
	 * Note: keep order in sync with libstdf.c
	 */
	switch (HEAD_TO_REC(rec->header)) {
		/* STDF_REC_TYP_PER_EXEC */
		case STDF_REC_PTR: {
			stdf_rec_ptr *ptr = (stdf_rec_ptr*)rec;
			free(ptr->TEST_TXT);
			free(ptr->ALARM_ID);
			free(ptr->UNITS);
			free(ptr->C_RESFMT);
			free(ptr->C_LLMFMT);
			free(ptr->C_HLMFMT);
			free(rec);
			break;
		}
		case STDF_REC_MPR: {
			stdf_rec_mpr *mpr = (stdf_rec_mpr*)rec;
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
		case STDF_REC_FTR: {
			stdf_rec_ftr *ftr = (stdf_rec_ftr*)rec;
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
                case STDF_REC_STR: {
                        stdf_rec_str *str = (stdf_rec_str*)rec;
			free(str->LOG_TYP);
			free(str->TEST_TXT);
			free(str->ALARM_ID);
			free(str->PROG_TXT);
			free(str->RSLT_TXT);
			free(str->MASK_MAP);
			free(str->FAL_MAP);
			free_xU2(str->LIM_INDX);
			free_xU4(str->LIM_SPEC);
                        free_xCn(str->COND_LST, str->COND_CNT);
                        free_xUf(str->CYC_OFST);
                        free_xUf(str->PMR_INDX);
                        free_xUf(str->CHN_NUM);
                        free_xU1(str->EXP_DATA);
                        free_xU1(str->CAP_DATA);
                        free_xU1(str->NEW_DATA);
                        free_xUf(str->PAT_NUM);
                        free_xUf(str->BIT_POS);
                        free_xUf(str->USR1);
                        free_xUf(str->USR2);
                        free_xUf(str->USR3);
                        free_xCn(str->USER_TXT, str->TXT_CNT);
			free(rec);
			break;
		}

		/* STDF_REC_TYP_PER_PART */
		case STDF_REC_PIR: {
#ifdef STDF_VER3
			stdf_rec_pir *pir = (stdf_rec_pir*)rec;
			if (file->ver == 3)
			free(pir->PART_ID);
#endif
			free(rec);
			break;
		}
		case STDF_REC_PRR: {
			stdf_rec_prr *prr = (stdf_rec_prr*)rec;
			free(prr->PART_ID);
			free(prr->PART_TXT);
			free(prr->PART_FIX);
			free(rec);
			break;
		}

		/* STDF_REC_TYP_PER_TEST */
		case STDF_REC_TSR: {
			stdf_rec_tsr *tsr = (stdf_rec_tsr*)rec;
			free(tsr->TEST_NAM);
			free(tsr->SEQ_NAME);
#ifdef STDF_VER3
			if (file->ver == 4)
#endif
			free(tsr->TEST_LBL);
			free(rec);
			break;
		}
#ifdef STDF_VER3
		case STDF_REC_PDR: {
			stdf_rec_pdr *pdr = (stdf_rec_pdr*)rec;
			free(pdr->UNITS);
			free(pdr->TEST_NAM);
			free(pdr->SEQ_NAME);
			free(rec);
			break;
		}
		case STDF_REC_FDR: {
			stdf_rec_fdr *fdr = (stdf_rec_fdr*)rec;
			free(fdr->TEST_NAM);
			free(fdr->SEQ_NAME);
			free(rec);
			break;
		}
#endif

		/* STDF_REC_TYP_GENERIC */
		case STDF_REC_DTR: {
			stdf_rec_dtr *dtr = (stdf_rec_dtr*)rec;
			free(dtr->TEXT_DAT);
			free(rec);
			break;
		}
		case STDF_REC_GDR: {
			stdf_rec_gdr *gdr = (stdf_rec_gdr*)rec;
			free_Vn(gdr->GEN_DATA, gdr->FLD_CNT);
			free(rec);
			break;
		}

		/* STDF_REC_TYP_PER_PROG */
		case STDF_REC_BPS: {
			stdf_rec_bps *bps = (stdf_rec_bps*)rec;
			free(bps->SEQ_NAME);
			free(rec);
			break;
		}
		case STDF_REC_EPS: {
			free(rec);
			break;
		}

		/* STDF_REC_TYP_PER_SITE */
#ifdef STDF_VER3
		case STDF_REC_SHB: {
			stdf_rec_shb *shb = (stdf_rec_shb*)rec;
			free(shb->HBIN_NAM);
			free(rec);
			break;
		}
		case STDF_REC_SSB: {
			stdf_rec_ssb *ssb = (stdf_rec_ssb*)rec;
			free(ssb->SBIN_NAM);
			free(rec);
			break;
		}
		case STDF_REC_STS: {
			stdf_rec_sts *sts = (stdf_rec_sts*)rec;
			free(sts->TEST_NAM);
			free(sts->SEQ_NAME);
			free(sts->TEST_LBL);
			free(rec);
			break;
		}
		case STDF_REC_SCR: {
			free(rec);
			break;
		}
#endif

		/* STDF_REC_TYP_PER_LOT */
		case STDF_REC_PMR: {
			stdf_rec_pmr *pmr = (stdf_rec_pmr*)rec;
			free(pmr->CHAN_NAM);
			free(pmr->PHY_NAM);
			free(pmr->LOG_NAM);
			free(rec);
			break;
		}
		case STDF_REC_PGR: {
			stdf_rec_pgr *pgr = (stdf_rec_pgr*)rec;
			free(pgr->GRP_NAM);
			free_xU2(pgr->PMR_INDX);
			free(rec);
			break;
		}
		case STDF_REC_HBR: {
			stdf_rec_hbr *hbr = (stdf_rec_hbr*)rec;
			free(hbr->HBIN_NAM);
			free(rec);
			break;
		}
		case STDF_REC_SBR: {
			stdf_rec_sbr *sbr = (stdf_rec_sbr*)rec;
			free(sbr->SBIN_NAM);
			free(rec);
			break;
		}
		case STDF_REC_PLR: {
			stdf_rec_plr *plr = (stdf_rec_plr*)rec;
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
		case STDF_REC_RDR: {
			stdf_rec_rdr *rdr = (stdf_rec_rdr*)rec;
			free_xU2(rdr->RTST_BIN);
			free(rec);
			break;
		}
		case STDF_REC_SDR: {
			stdf_rec_sdr *sdr = (stdf_rec_sdr*)rec;
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
		case STDF_REC_MIR: {
			stdf_rec_mir *mir = (stdf_rec_mir*)rec;
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
		case STDF_REC_MRR: {
			stdf_rec_mrr *mrr = (stdf_rec_mrr*)rec;
			free(mrr->USR_DESC);
			free(mrr->EXC_DESC);
			free(rec);
			break;
		}
		case STDF_REC_PCR: {
			free(rec);
			break;
		}
		case STDF_REC_PSR: {
                        stdf_rec_psr *psr = (stdf_rec_psr*)rec;
                        free(psr->PSR_NAM);
                        free_xU8(psr->PAT_BGN);
                        free_xU8(psr->PAT_END);
                        free_xCn(psr->PAT_FILE,psr->LOCP_CNT);
                        free_xCn(psr->PAT_LBL,psr->LOCP_CNT);
                        free_xCn(psr->FILE_UID,psr->LOCP_CNT);
                        free_xCn(psr->ATPG_DSC,psr->LOCP_CNT);
                        free_xCn(psr->SRC_ID,psr->LOCP_CNT);
			free(rec);
			break;
		}
		case STDF_REC_NMR: {
                        stdf_rec_nmr *nmr = (stdf_rec_nmr*)rec;
                        free_xU2(nmr->PMR_INDX);
                        free_xCn(nmr->ATPG_NAM,nmr->LOCM_CNT);
			free(rec);
			break;
		}
		case STDF_REC_CNR: {
                        stdf_rec_cnr *cnr = (stdf_rec_cnr*)rec;
                        free(cnr->CELL_NAM);
			free(rec);
			break;
		}
		case STDF_REC_SSR: {
                        stdf_rec_ssr *ssr = (stdf_rec_ssr*)rec;
                        free(ssr->SSR_NAM);
                        free_xU2(ssr->CHN_LIST);
			free(rec);
			break;
		}
		case STDF_REC_CDR: {
                        stdf_rec_cdr *cdr = (stdf_rec_cdr*)rec;
                        free(cdr->CHN_NAM);
                        free_xU2(cdr->M_CLKS);
                        free_xU2(cdr->S_CLKS);
                        free_xSn(cdr->CELL_LST,cdr->LST_CNT);
			free(rec);
			break;
		}

		/* STDF_REC_TYP_PER_WAFER */
		case STDF_REC_WIR: {
			stdf_rec_wir *wir = (stdf_rec_wir*)rec;
			free(wir->WAFER_ID);
			free(rec);
			break;
		}
		case STDF_REC_WRR: {
			stdf_rec_wrr *wrr = (stdf_rec_wrr*)rec;
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
		case STDF_REC_WCR: {
			free(rec);
			break;
		}

		/* STDF_REC_TYP_INFO */
		case STDF_REC_FAR: {
			free(rec);
			break;
		}
		case STDF_REC_ATR: {
			stdf_rec_atr *atr = (stdf_rec_atr*)rec;
			free(atr->CMD_LINE);
			free(atr);
			break;
		}
                case STDF_REC_VUR: {
                        stdf_rec_vur *vur = (stdf_rec_vur*)rec;
                        free(vur->UPD_NAM);
                        free(vur);
                        break;
                }

		/* all the rest */
		case STDF_REC_UNKNOWN: {
			if (rec->data) free(rec->data);
			free(rec);
			break;
		}
		default: {
			warnf("Memory leak: unhandled record type (%s) !",
			       stdf_get_rec_name(rec->header.REC_TYP, rec->header.REC_SUB));
			break;
		}
	}

	rec = NULL;
}


/*
 * Reading records
 */


stdf_rec_unknown* stdf_read_rec_unknown(stdf_file *file)
{
	stdf_rec_unknown *rec = malloc(sizeof(*rec));
#if 0
	rec->data = malloc(file->header.REC_LEN);
	memcpy(rec->data, file->stdf_rec_data, file->header.REC_LEN);
#else
	rec->data = NULL;
#endif
	file->rec_pos += file->header.REC_LEN;
	return rec;
}

stdf_rec_far* stdf_read_rec_far(stdf_file *file)
{
	stdf_rec_far *far = malloc(sizeof(*far));

	_stdf_read_dtc_U1(file, &(far->CPU_TYPE));
	_stdf_read_dtc_U1(file, &(far->STDF_VER));
	return far;
}

stdf_rec_atr* stdf_read_rec_atr(stdf_file *file)
{
	stdf_rec_atr *atr = malloc(sizeof(*atr));
	_stdf_read_dtc_U4(file, &(atr->MOD_TIM));
	_stdf_read_dtc_Cn(file, &(atr->CMD_LINE));
	return atr;
}

stdf_rec_vur* stdf_read_rec_vur(stdf_file *file)
{
	stdf_rec_vur *vur = malloc(sizeof(*vur));
	_stdf_read_dtc_Cn(file, &(vur->UPD_NAM));
	return vur;
}

stdf_rec_mir* stdf_read_rec_mir(stdf_file *file)
{
	stdf_rec_mir *mir = malloc(sizeof(*mir));
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

stdf_rec_mrr* stdf_read_rec_mrr(stdf_file *file)
{
	stdf_rec_mrr *mrr = malloc(sizeof(*mrr));
	_stdf_read_dtc_U4(file, &(mrr->FINISH_T));
#ifdef STDF_VER3
	if (file->ver == 3) {
	_stdf_read_dtc_U4(file, &(mrr->PART_CNT));
	_stdf_read_dtc_U4(file, &(mrr->RTST_CNT));
	_stdf_read_dtc_U4(file, &(mrr->ABRT_CNT));
	_stdf_read_dtc_U4(file, &(mrr->GOOD_CNT));
	_stdf_read_dtc_U4(file, &(mrr->FUNC_CNT));
	}
#endif
	_stdf_read_dtc_C1(file, &(mrr->DISP_COD));
	_stdf_read_dtc_Cn(file, &(mrr->USR_DESC));
	_stdf_read_dtc_Cn(file, &(mrr->EXC_DESC));
	return mrr;
}

stdf_rec_pcr* stdf_read_rec_pcr(stdf_file *file)
{
	stdf_rec_pcr *pcr = malloc(sizeof(*pcr));
	_stdf_read_dtc_U1(file, &(pcr->HEAD_NUM));
	_stdf_read_dtc_U1(file, &(pcr->SITE_NUM));
	_stdf_read_dtc_U4(file, &(pcr->PART_CNT));
	_stdf_read_dtc_U4(file, &(pcr->RTST_CNT));
	_stdf_read_dtc_U4(file, &(pcr->ABRT_CNT));
	_stdf_read_dtc_U4(file, &(pcr->GOOD_CNT));
	_stdf_read_dtc_U4(file, &(pcr->FUNC_CNT));
	return pcr;
}

stdf_rec_hbr* stdf_read_rec_hbr(stdf_file *file)
{
	stdf_rec_hbr *hbr = malloc(sizeof(*hbr));
	_stdf_read_dtc_U1(file, &(hbr->HEAD_NUM));
	_stdf_read_dtc_U1(file, &(hbr->SITE_NUM));
	_stdf_read_dtc_U2(file, &(hbr->HBIN_NUM));
	_stdf_read_dtc_U4(file, &(hbr->HBIN_CNT));
	_stdf_read_dtc_C1(file, &(hbr->HBIN_PF));
	_stdf_read_dtc_Cn(file, &(hbr->HBIN_NAM));
	return hbr;
}

stdf_rec_sbr* stdf_read_rec_sbr(stdf_file *file)
{
	stdf_rec_sbr *sbr = malloc(sizeof(*sbr));
	_stdf_read_dtc_U1(file, &(sbr->HEAD_NUM));
	_stdf_read_dtc_U1(file, &(sbr->SITE_NUM));
	_stdf_read_dtc_U2(file, &(sbr->SBIN_NUM));
	_stdf_read_dtc_U4(file, &(sbr->SBIN_CNT));
	_stdf_read_dtc_C1(file, &(sbr->SBIN_PF));
	_stdf_read_dtc_Cn(file, &(sbr->SBIN_NAM));
	return sbr;
}

stdf_rec_pmr* stdf_read_rec_pmr(stdf_file *file)
{
	stdf_rec_pmr *pmr = malloc(sizeof(*pmr));
	_stdf_read_dtc_U2(file, &(pmr->PMR_INDX));
	_stdf_read_dtc_U2(file, &(pmr->CHAN_TYP));
	_stdf_read_dtc_Cn(file, &(pmr->CHAN_NAM));
	_stdf_read_dtc_Cn(file, &(pmr->PHY_NAM));
	_stdf_read_dtc_Cn(file, &(pmr->LOG_NAM));
	_stdf_read_dtc_U1(file, &(pmr->HEAD_NUM));
	_stdf_read_dtc_U1(file, &(pmr->SITE_NUM));
	return pmr;
}

stdf_rec_pgr* stdf_read_rec_pgr(stdf_file *file)
{
	stdf_rec_pgr *pgr = malloc(sizeof(*pgr));
	_stdf_read_dtc_U2(file, &(pgr->GRP_INDX));
	_stdf_read_dtc_Cn(file, &(pgr->GRP_NAM));
	_stdf_read_dtc_U2(file, &(pgr->INDX_CNT));
	_stdf_read_dtc_xU2(file, &(pgr->PMR_INDX), pgr->INDX_CNT);
	return pgr;
}

stdf_rec_plr* stdf_read_rec_plr(stdf_file *file)
{
	stdf_rec_plr *plr = malloc(sizeof(*plr));
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

stdf_rec_rdr* stdf_read_rec_rdr(stdf_file *file)
{
	stdf_rec_rdr *rdr = malloc(sizeof(*rdr));
	_stdf_read_dtc_U2(file, &(rdr->NUM_BINS));
	_stdf_read_dtc_xU2(file, &(rdr->RTST_BIN), rdr->NUM_BINS);
	return rdr;
}

stdf_rec_sdr* stdf_read_rec_sdr(stdf_file *file)
{
	stdf_rec_sdr *sdr = malloc(sizeof(*sdr));
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

stdf_rec_psr* stdf_read_rec_psr(stdf_file *file)
{
	stdf_rec_psr *psr = malloc(sizeof(*psr));
	_stdf_read_dtc_B1(file, &(psr->CONT_FLG));
	_stdf_read_dtc_U2(file, &(psr->PSR_INDX));
	_stdf_read_dtc_Cn(file, &(psr->PSR_NAM));
	_stdf_read_dtc_B1(file, &(psr->OPT_FLG));
	_stdf_read_dtc_U2(file, &(psr->TOTP_CNT));
	_stdf_read_dtc_U2(file, &(psr->LOCP_CNT));
	_stdf_read_dtc_xU8(file, &(psr->PAT_BGN), psr->LOCP_CNT);
	_stdf_read_dtc_xU8(file, &(psr->PAT_END), psr->LOCP_CNT);
	_stdf_read_dtc_xCn(file, &(psr->PAT_FILE), psr->LOCP_CNT);
	_stdf_read_dtc_xCn(file, &(psr->PAT_LBL), psr->LOCP_CNT);
	_stdf_read_dtc_xCn(file, &(psr->FILE_UID), psr->LOCP_CNT);
	_stdf_read_dtc_xCn(file, &(psr->ATPG_DSC), psr->LOCP_CNT);
	_stdf_read_dtc_xCn(file, &(psr->SRC_ID), psr->LOCP_CNT);
	return psr;
}

stdf_rec_nmr* stdf_read_rec_nmr(stdf_file *file)
{
	stdf_rec_nmr *nmr = malloc(sizeof(*nmr));
	_stdf_read_dtc_B1(file, &(nmr->CONT_FLG));
	_stdf_read_dtc_U2(file, &(nmr->TOTM_CNT));
	_stdf_read_dtc_U2(file, &(nmr->LOCM_CNT));
	_stdf_read_dtc_xU2(file, &(nmr->PMR_INDX), nmr->LOCM_CNT);
	_stdf_read_dtc_xCn(file, &(nmr->ATPG_NAM), nmr->LOCM_CNT);
	return nmr;
}

stdf_rec_cnr* stdf_read_rec_cnr(stdf_file *file)
{
	stdf_rec_cnr *cnr = malloc(sizeof(*cnr));
	_stdf_read_dtc_U2(file, &(cnr->CHN_NUM));
	_stdf_read_dtc_U4(file, &(cnr->BIT_POS));
	_stdf_read_dtc_Sn(file, &(cnr->CELL_NAM));
	return cnr;
}

stdf_rec_ssr* stdf_read_rec_ssr(stdf_file *file)
{
	stdf_rec_ssr *ssr = malloc(sizeof(*ssr));
	_stdf_read_dtc_Cn(file, &(ssr->SSR_NAM));
	_stdf_read_dtc_U2(file, &(ssr->CHN_CNT));
	_stdf_read_dtc_xU2(file, &(ssr->CHN_LIST), ssr->CHN_CNT);
	return ssr;
}

stdf_rec_cdr* stdf_read_rec_cdr(stdf_file *file)
{
	stdf_rec_cdr *cdr = malloc(sizeof(*cdr));
	_stdf_read_dtc_B1(file, &(cdr->CONT_FLG));
	_stdf_read_dtc_U2(file, &(cdr->CDR_INDX));
	_stdf_read_dtc_Cn(file, &(cdr->CHN_NAM));
	_stdf_read_dtc_U4(file, &(cdr->CHN_LEN));
	_stdf_read_dtc_U2(file, &(cdr->SIN_PIN));
	_stdf_read_dtc_U2(file, &(cdr->SOUT_PIN));
	_stdf_read_dtc_U1(file, &(cdr->MSTR_CNT));
	_stdf_read_dtc_xU2(file, &(cdr->M_CLKS), cdr->MSTR_CNT);
	_stdf_read_dtc_U1(file, &(cdr->SLAV_CNT));
	_stdf_read_dtc_xU2(file, &(cdr->S_CLKS), cdr->SLAV_CNT);
	_stdf_read_dtc_U1(file, &(cdr->INV_VAL));
	_stdf_read_dtc_U2(file, &(cdr->LST_CNT));
        _stdf_read_dtc_xSn(file, &(cdr->CELL_LST), cdr->LST_CNT);
	return cdr;
}

stdf_rec_wir* stdf_read_rec_wir(stdf_file *file)
{
	stdf_rec_wir *wir = malloc(sizeof(*wir));
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

stdf_rec_wrr* stdf_read_rec_wrr(stdf_file *file)
{
	stdf_rec_wrr *wrr = malloc(sizeof(*wrr));
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

stdf_rec_wcr* stdf_read_rec_wcr(stdf_file *file)
{
	stdf_rec_wcr *wcr = malloc(sizeof(*wcr));
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

stdf_rec_pir* stdf_read_rec_pir(stdf_file *file)
{
	stdf_rec_pir *pir = malloc(sizeof(*pir));
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

stdf_rec_prr* stdf_read_rec_prr(stdf_file *file)
{
	stdf_rec_prr *prr = malloc(sizeof(*prr));
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

stdf_rec_pdr* stdf_read_rec_pdr(stdf_file *file)
{
	stdf_rec_pdr *pdr = malloc(sizeof(*pdr));
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

stdf_rec_fdr* stdf_read_rec_fdr(stdf_file *file)
{
	stdf_rec_fdr *fdr = malloc(sizeof(*fdr));
	_stdf_read_dtc_U4(file, &(fdr->TEST_NUM));
	_stdf_read_dtc_B1(file, &(fdr->DESC_FLG));
	_stdf_read_dtc_Cn(file, &(fdr->TEST_NAM));
	_stdf_read_dtc_Cn(file, &(fdr->SEQ_NAME));
	return fdr;
}

#endif

stdf_rec_tsr* stdf_read_rec_tsr(stdf_file *file)
{
	stdf_rec_tsr *tsr = malloc(sizeof(*tsr));
	_stdf_read_dtc_U1(file, &(tsr->HEAD_NUM));
	_stdf_read_dtc_U1(file, &(tsr->SITE_NUM));
#ifdef STDF_VER3
	if (file->ver == 4)
#endif
	_stdf_read_dtc_C1(file, &(tsr->TEST_TYP));
	_stdf_read_dtc_U4(file, &(tsr->TEST_NUM));
#ifdef STDF_VER3
	if (file->ver == 4) {
#endif
	_stdf_read_dtc_U4(file, &(tsr->EXEC_CNT));
	_stdf_read_dtc_U4(file, &(tsr->FAIL_CNT));
	_stdf_read_dtc_U4(file, &(tsr->ALRM_CNT));
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
	_stdf_read_dtc_as(file, &(tsr->EXEC_CNT), U4, I4);
	_stdf_read_dtc_as(file, &(tsr->FAIL_CNT), U4, I4);
	_stdf_read_dtc_as(file, &(tsr->ALRM_CNT), U4, I4);
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

stdf_rec_ptr* stdf_read_rec_ptr(stdf_file *file)
{
	stdf_rec_ptr *ptr = malloc(sizeof(*ptr));
	_stdf_read_dtc_U4(file, &(ptr->TEST_NUM));
	_stdf_read_dtc_U1(file, &(ptr->HEAD_NUM));
	_stdf_read_dtc_U1(file, &(ptr->SITE_NUM));
	_stdf_read_dtc_B1(file, &(ptr->TEST_FLG));
	_stdf_read_dtc_B1(file, &(ptr->PARM_FLG));
	_stdf_read_dtc_R4(file, &(ptr->RESULT));
#ifdef STDF_VER3
	if (file->ver == 4) {
#endif
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
#ifdef STDF_VER3
	} else {
	_stdf_read_dtc_B1(file, &(ptr->OPT_FLAG));
	_stdf_read_dtc_I1(file, &(ptr->RES_SCAL));
	_stdf_read_dtc_U1(file, &(ptr->RES_LDIG));
	_stdf_read_dtc_U1(file, &(ptr->RES_RDIG));
	_stdf_read_dtc_B1(file, &(ptr->DESC_FLG));
	_stdf_read_dtc_Cx(file, &(ptr->UNITS), 7);
	_stdf_read_dtc_I1(file, &(ptr->LLM_SCAL));
	_stdf_read_dtc_I1(file, &(ptr->HLM_SCAL));
	_stdf_read_dtc_U1(file, &(ptr->LLM_LDIG));
	_stdf_read_dtc_U1(file, &(ptr->LLM_RDIG));
	_stdf_read_dtc_U1(file, &(ptr->HLM_LDIG));
	_stdf_read_dtc_U1(file, &(ptr->HLM_RDIG));
	_stdf_read_dtc_R4(file, &(ptr->LO_LIMIT));
	_stdf_read_dtc_R4(file, &(ptr->HI_LIMIT));
	_stdf_read_dtc_Cn(file, &(ptr->TEST_NAM));
	_stdf_read_dtc_Cn(file, &(ptr->SEQ_NAME));
	_stdf_read_dtc_Cn(file, &(ptr->TEST_TXT));
	}
#endif
	return ptr;
}

stdf_rec_mpr* stdf_read_rec_mpr(stdf_file *file)
{
	stdf_rec_mpr *mpr = malloc(sizeof(*mpr));
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

stdf_rec_ftr* stdf_read_rec_ftr(stdf_file *file)
{
	stdf_rec_ftr *ftr = malloc(sizeof(*ftr));
	_stdf_read_dtc_U4(file, &(ftr->TEST_NUM));
	_stdf_read_dtc_U1(file, &(ftr->HEAD_NUM));
	_stdf_read_dtc_U1(file, &(ftr->SITE_NUM));
	_stdf_read_dtc_B1(file, &(ftr->TEST_FLG));
#ifdef STDF_VER3
	if (file->ver == 4) {
#endif
	_stdf_read_dtc_B1(file, &(ftr->OPT_FLAG));
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
#ifdef STDF_VER3
	} else {
	_stdf_read_dtc_B1(file, &(ftr->DESC_FLG));
	_stdf_read_dtc_B1(file, &(ftr->OPT_FLAG));
	_stdf_read_dtc_as(file, &(ftr->TIME_SET), Cn, U1);
	_stdf_read_dtc_U4(file, &(ftr->VECT_ADR));
	_stdf_read_dtc_U4(file, &(ftr->CYCL_CNT));
	_stdf_read_dtc_as(file, &(ftr->REPT_CNT), U4, U2);
	_stdf_read_dtc_U2(file, &(ftr->PCP_ADDR));
	_stdf_read_dtc_U4(file, &(ftr->NUM_FAIL));
	_stdf_read_dtc_Bn(file, &(ftr->FAIL_PIN));
	_stdf_read_dtc_Bn(file, &(ftr->VECT_DAT));
	_stdf_read_dtc_Bn(file, &(ftr->DEV_DAT));
	_stdf_read_dtc_Bn(file, &(ftr->RPIN_MAP));
	_stdf_read_dtc_Cn(file, &(ftr->TEST_NAM));
	_stdf_read_dtc_Cn(file, &(ftr->SEQ_NAME));
	_stdf_read_dtc_Cn(file, &(ftr->TEST_TXT));
	}
#endif
	return ftr;
}

stdf_rec_str* stdf_read_rec_str(stdf_file *file)
{
	stdf_rec_str *str = malloc(sizeof(*str));
	_stdf_read_dtc_B1(file, &(str->CONT_FLG));
	_stdf_read_dtc_U4(file, &(str->TEST_NUM));
	_stdf_read_dtc_U1(file, &(str->HEAD_NUM));
	_stdf_read_dtc_U1(file, &(str->SITE_NUM));
	_stdf_read_dtc_U2(file, &(str->PSR_REF));
	_stdf_read_dtc_B1(file, &(str->TEST_FLG));
	_stdf_read_dtc_Cn(file, &(str->LOG_TYP));
	_stdf_read_dtc_Cn(file, &(str->TEST_TXT));
	_stdf_read_dtc_Cn(file, &(str->ALARM_ID));
	_stdf_read_dtc_Cn(file, &(str->PROG_TXT));
	_stdf_read_dtc_Cn(file, &(str->RSLT_TXT));
	_stdf_read_dtc_U1(file, &(str->Z_VAL));
	_stdf_read_dtc_B1(file, &(str->FMU_FLG));
	if (str->FMU_FLG & 1) 
		_stdf_read_dtc_Dn(file, &(str->MASK_MAP));
	else
	{
		str->MASK_MAP = malloc(2);
		str->MASK_MAP[0]=0;
	}
	if ((str->FMU_FLG & 4) == 4) 
		_stdf_read_dtc_Dn(file, &(str->FAL_MAP));
	else
	{
		str->FAL_MAP = malloc(2);
		str->FAL_MAP[0]=0;
	}
	_stdf_read_dtc_U8(file, &(str->CYCL_CNT));
	_stdf_read_dtc_U4(file, &(str->TOTF_CNT));
	_stdf_read_dtc_U4(file, &(str->TOTL_CNT));
	_stdf_read_dtc_U8(file, &(str->CYC_BASE));
	_stdf_read_dtc_U4(file, &(str->BIT_BASE));
	_stdf_read_dtc_U2(file, &(str->COND_CNT));
	_stdf_read_dtc_U2(file, &(str->LIM_CNT));
	_stdf_read_dtc_U1(file, &(str->CYCL_SIZE));
	_stdf_read_dtc_U1(file, &(str->PMR_SIZE));
	_stdf_read_dtc_U1(file, &(str->CHN_SIZE));
	_stdf_read_dtc_U1(file, &(str->PAT_SIZE));
	_stdf_read_dtc_U1(file, &(str->BIT_SIZE));
	_stdf_read_dtc_U1(file, &(str->U1_SIZE));
	_stdf_read_dtc_U1(file, &(str->U2_SIZE));
	_stdf_read_dtc_U1(file, &(str->U3_SIZE));
	_stdf_read_dtc_U1(file, &(str->UTX_SIZE));
	_stdf_read_dtc_U2(file, &(str->CAP_BGN));
	_stdf_read_dtc_xU2(file, &(str->LIM_INDX), str->LIM_CNT);
	_stdf_read_dtc_xU4(file, &(str->LIM_SPEC), str->LIM_CNT);
	_stdf_read_dtc_xCn(file, &(str->COND_LST), str->COND_CNT);
	_stdf_read_dtc_U2(file, &(str->CYC_CNT));
	_stdf_read_dtc_xUf(file, &(str->CYC_OFST), str->CYC_CNT, str->CYCL_SIZE);
	_stdf_read_dtc_U2(file, &(str->PMR_CNT));
	_stdf_read_dtc_xUf(file, &(str->PMR_INDX), str->PMR_CNT, str->PMR_SIZE);
	_stdf_read_dtc_U2(file, &(str->CHN_CNT));
	_stdf_read_dtc_xUf(file, &(str->CHN_NUM), str->CHN_CNT, str->CHN_SIZE);
	_stdf_read_dtc_U2(file, &(str->EXP_CNT));
	_stdf_read_dtc_xU1(file, &(str->EXP_DATA), str->EXP_CNT);
	_stdf_read_dtc_U2(file, &(str->CAP_CNT));
	_stdf_read_dtc_xU1(file, &(str->CAP_DATA), str->CAP_CNT);
	_stdf_read_dtc_U2(file, &(str->NEW_CNT));
	_stdf_read_dtc_xU1(file, &(str->NEW_DATA), str->NEW_CNT);
	_stdf_read_dtc_U2(file, &(str->PAT_CNT));
	_stdf_read_dtc_xUf(file, &(str->PAT_NUM), str->PAT_CNT, str->PAT_SIZE);
	_stdf_read_dtc_U2(file, &(str->BPOS_CNT));
	_stdf_read_dtc_xUf(file, &(str->BIT_POS), str->BPOS_CNT, str->BIT_SIZE);
	_stdf_read_dtc_U2(file, &(str->USR1_CNT));
	_stdf_read_dtc_xUf(file, &(str->USR1), str->USR1_CNT, str->U1_SIZE);
	_stdf_read_dtc_U2(file, &(str->USR2_CNT));
	_stdf_read_dtc_xUf(file, &(str->USR2), str->USR2_CNT, str->U2_SIZE);
	_stdf_read_dtc_U2(file, &(str->USR3_CNT));
	_stdf_read_dtc_xUf(file, &(str->USR3), str->USR3_CNT, str->U3_SIZE);
	_stdf_read_dtc_U2(file, &(str->TXT_CNT));
	_stdf_read_dtc_xCn(file, &(str->USER_TXT), str->TXT_CNT);
	return str;
}

stdf_rec_bps* stdf_read_rec_bps(stdf_file *file)
{
	stdf_rec_bps *bps = malloc(sizeof(*bps));
	_stdf_read_dtc_Cn(file, &(bps->SEQ_NAME));
	return bps;
}

stdf_rec_eps* stdf_read_rec_eps(stdf_attribute_unused stdf_file *file)
{
	stdf_rec_eps *eps = malloc(sizeof(*eps));
	return eps;
}

#ifdef STDF_VER3

stdf_rec_shb* stdf_read_rec_shb(stdf_file *file)
{
	stdf_rec_shb *shb = malloc(sizeof(*shb));
	_stdf_read_dtc_U1(file, &(shb->HEAD_NUM));
	_stdf_read_dtc_U1(file, &(shb->SITE_NUM));
	_stdf_read_dtc_U2(file, &(shb->HBIN_NUM));
	_stdf_read_dtc_U4(file, &(shb->HBIN_CNT));
	_stdf_read_dtc_Cn(file, &(shb->HBIN_NAM));
	return shb;
}

stdf_rec_ssb* stdf_read_rec_ssb(stdf_file *file)
{
	stdf_rec_ssb *ssb = malloc(sizeof(*ssb));
	_stdf_read_dtc_U1(file, &(ssb->HEAD_NUM));
	_stdf_read_dtc_U1(file, &(ssb->SITE_NUM));
	_stdf_read_dtc_U2(file, &(ssb->SBIN_NUM));
	_stdf_read_dtc_U4(file, &(ssb->SBIN_CNT));
	_stdf_read_dtc_Cn(file, &(ssb->SBIN_NAM));
	return ssb;
}

stdf_rec_sts* stdf_read_rec_sts(stdf_file *file)
{
	stdf_rec_sts *sts = malloc(sizeof(*sts));
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

stdf_rec_scr* stdf_read_rec_scr(stdf_file *file)
{
	stdf_rec_scr *scr = malloc(sizeof(*scr));
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

stdf_rec_gdr* stdf_read_rec_gdr(stdf_file *file)
{
	stdf_rec_gdr *gdr = malloc(sizeof(*gdr));
	_stdf_read_dtc_U2(file, &(gdr->FLD_CNT));
	_stdf_read_dtc_Vn(file, &(gdr->GEN_DATA), gdr->FLD_CNT);
	return gdr;
}

stdf_rec_dtr* stdf_read_rec_dtr(stdf_file *file)
{
	stdf_rec_dtr *dtr = malloc(sizeof(*dtr));
	_stdf_read_dtc_Cn(file, &(dtr->TEXT_DAT));
	return dtr;
}


/*
 * Record utility funcs
 */


static inline size_t _lenCn(stdf_dtc_Cn Cn)
{
	return (Cn && Cn[0] ? strlen(Cn) : 1);
}
static inline size_t _lenSn(stdf_dtc_Sn Sn)
{
        return (Sn && (*(stdf_dtc_U2*)Sn) ? (*(stdf_dtc_U2*)Sn)+2 : 2);
}
static inline size_t _lenDn(stdf_dtc_Dn Dn)
{
        stdf_dtc_U2 len;
        if (Dn && (*(stdf_dtc_U2*)Dn))
        {
                len=(*(stdf_dtc_U2*)Dn)/8;
                if ((*(stdf_dtc_U2*)Dn) % 8) ++len;
                len+=2;
                return len;
        }
        else
                return 2;
}
static inline size_t _len_dtcXCn(stdf_dtc_xCn xCn, stdf_dtc_U2 cnt)
{
	size_t ret = 0;
	while (cnt--)
		ret += _lenCn(xCn[cnt]);
	return ret;
}
static inline size_t _len_dtcXSn(stdf_dtc_xSn xSn, stdf_dtc_U2 cnt)
{
	size_t ret = 0;
	while (cnt--)
		ret += _lenSn(xSn[cnt]);
	return ret;
}
static inline size_t _lenBn(stdf_dtc_Bn Bn)
{
        unsigned short len;
        // using _lenCn with strlen doesn't work here since 0x0 is legal value
        if (Bn && Bn[0]) 
        {    len=Bn[0];
             len++;
             return len;
        }
        else
             return 1;
}
#define _lenCx(Cx, x) (x)
#define _len_dtcX(x, cnt) (sizeof(*(x)) * cnt)
#define _len_dtcXN(xn, cnt) (sizeof(*(xn)) * (cnt / 2 + cnt % 2))
#define _len_dtcXf(cnt, size) (cnt * size)
static inline size_t _lenVn(stdf_dtc_Vn Vn, stdf_dtc_U2 cnt)
{
	size_t ret = 0;
	while (cnt--) {
        	ret++;
		switch (Vn->type) {
			case STDF_GDR_B0: break;
			case STDF_GDR_U1: ret += sizeof(stdf_dtc_U1); break;
			case STDF_GDR_U2: ret += sizeof(stdf_dtc_U2); break;
			case STDF_GDR_U4: ret += sizeof(stdf_dtc_U4); break;
			case STDF_GDR_I1: ret += sizeof(stdf_dtc_I1); break;
			case STDF_GDR_I2: ret += sizeof(stdf_dtc_I2); break;
			case STDF_GDR_I4: ret += sizeof(stdf_dtc_I4); break;
			case STDF_GDR_R4: ret += sizeof(stdf_dtc_R4); break;
			case STDF_GDR_R8: ret += sizeof(stdf_dtc_R8); break;
			case STDF_GDR_Cn: ret += _lenCn(*((stdf_dtc_Cn*)Vn->data)); break;
			case STDF_GDR_Bn: ret += _lenBn(*((stdf_dtc_Bn*)Vn->data)); break;
			case STDF_GDR_Dn: ret += _lenDn(*((stdf_dtc_Dn*)Vn->data)); break;
			case STDF_GDR_N1: ret += sizeof(stdf_dtc_B1); break;
		}
		++Vn;
	}
	return ret;
}

static inline size_t _calc_rec_len_far(stdf_attribute_unused stdf_file *f, stdf_rec_far *r)
{
	return sizeof(r->CPU_TYPE) + sizeof(r->STDF_VER);
}

static inline size_t _calc_rec_len_atr(stdf_attribute_unused stdf_file *f, stdf_rec_atr *r)
{
	return sizeof(r->MOD_TIM) + _lenCn(r->CMD_LINE);
}

static inline size_t _calc_rec_len_vur(stdf_attribute_unused stdf_file *f, stdf_rec_vur *r)
{
	return _lenCn(r->UPD_NAM);
}

static inline size_t _calc_rec_len_mir(stdf_file *f, stdf_rec_mir *r)
{
#define _CALC_REC_LEN_MIR_v3(r) \
	( \
	sizeof(r->CPU_TYPE) + sizeof(r->STDF_VER) + sizeof(r->MODE_COD) + \
	sizeof(r->STAT_NUM) + _lenCx(r->TEST_COD, 3) + sizeof(r->RTST_COD) + \
	sizeof(r->PROT_COD) + sizeof(r->CMOD_COD) + sizeof(r->SETUP_T) + \
	sizeof(r->START_T) + _lenCn(r->LOT_ID) + _lenCn(r->PART_TYP) + \
	_lenCn(r->JOB_NAM) + _lenCn(r->OPER_NAM) + _lenCn(r->NODE_NAM) + \
	_lenCn(r->TSTR_TYP) + _lenCn(r->EXEC_TYP) + _lenCn(r->SUPR_NAM) + \
	_lenCn(r->HAND_ID) + _lenCn(r->SBLOT_ID) + _lenCn(r->JOB_REV) + \
	_lenCn(r->PROC_ID) + _lenCn(r->PRB_CARD) \
	)
#define _CALC_REC_LEN_MIR_v4(r) \
	( \
	sizeof(r->SETUP_T) + sizeof(r->START_T) + sizeof(r->STAT_NUM) + \
	sizeof(r->MODE_COD) + sizeof(r->RTST_COD) + sizeof(r->PROT_COD) + \
	sizeof(r->BURN_TIM) + sizeof(r->CMOD_COD) + \
	_lenCn(r->LOT_ID) + _lenCn(r->PART_TYP) + _lenCn(r->NODE_NAM) + \
	_lenCn(r->TSTR_TYP) + _lenCn(r->JOB_NAM) + _lenCn(r->JOB_REV) + \
	_lenCn(r->SBLOT_ID) + _lenCn(r->OPER_NAM) + _lenCn(r->EXEC_TYP) + \
	_lenCn(r->EXEC_VER) + _lenCn(r->TEST_COD) + _lenCn(r->TST_TEMP) + \
	_lenCn(r->USER_TXT) + _lenCn(r->AUX_FILE) + _lenCn(r->PKG_TYP) + \
	_lenCn(r->FAMILY_ID) + _lenCn(r->DATE_COD) + _lenCn(r->FACIL_ID) + \
	_lenCn(r->FLOOR_ID) + _lenCn(r->PROC_ID) + _lenCn(r->OPER_FRQ) + \
	_lenCn(r->SPEC_NAM) + _lenCn(r->SPEC_VER) + _lenCn(r->FLOW_ID) + \
	_lenCn(r->SETUP_ID) + _lenCn(r->DSGN_REV) + _lenCn(r->ENG_ID) + \
	_lenCn(r->ROM_COD) + _lenCn(r->SERL_NUM) + _lenCn(r->SUPR_NAM) \
	)
#ifndef STDF_VER3
# undef _CALC_REC_LEN_MIR_v3
# define _CALC_REC_LEN_MIR_v3(r) 0
#endif
	return (f->ver == 3 ? _CALC_REC_LEN_MIR_v3(r) : _CALC_REC_LEN_MIR_v4(r));
}

static inline size_t _calc_rec_len_mrr(stdf_file *f, stdf_rec_mrr *r)
{
#define _CALC_REC_LEN_MRR_v3(r) \
	( \
	sizeof(r->FINISH_T) + sizeof(r->DISP_COD) + \
	sizeof(r->PART_CNT) + sizeof(r->RTST_CNT) + sizeof(r->ABRT_CNT) + \
	sizeof(r->GOOD_CNT) + sizeof(r->FUNC_CNT) + \
	_lenCn(r->USR_DESC) + _lenCn(r->EXC_DESC) \
	)
#define _CALC_REC_LEN_MRR_v4(r) \
	( \
	sizeof(r->FINISH_T) + sizeof(r->DISP_COD) + \
	_lenCn(r->USR_DESC) + _lenCn(r->EXC_DESC) \
	)
#ifndef STDF_VER3
# undef _CALC_REC_LEN_MRR_v3
# define _CALC_REC_LEN_MRR_v3(r) 0
#endif
	return (f->ver == 3 ? _CALC_REC_LEN_MRR_v3(r) : _CALC_REC_LEN_MRR_v4(r));
}

static inline size_t _calc_rec_len_pcr(stdf_attribute_unused stdf_file *f, stdf_rec_pcr *r)
{
	return
		sizeof(r->HEAD_NUM) + sizeof(r->SITE_NUM) + sizeof(r->PART_CNT) +
		sizeof(r->RTST_CNT) + sizeof(r->ABRT_CNT) + sizeof(r->GOOD_CNT) +
		sizeof(r->FUNC_CNT);
}

static inline size_t _calc_rec_len_hbr(stdf_attribute_unused stdf_file *f, stdf_rec_hbr *r)
{
	return
		sizeof(r->HEAD_NUM) + sizeof(r->SITE_NUM) + sizeof(r->HBIN_NUM) +
		sizeof(r->HBIN_CNT) + sizeof(r->HBIN_PF) + _lenCn(r->HBIN_NAM);
}

static inline size_t _calc_rec_len_sbr(stdf_attribute_unused stdf_file *f, stdf_rec_sbr *r)
{
	return
		sizeof(r->HEAD_NUM) + sizeof(r->SITE_NUM) + sizeof(r->SBIN_NUM) +
		sizeof(r->SBIN_CNT) + sizeof(r->SBIN_PF) + _lenCn(r->SBIN_NAM);
}

static inline size_t _calc_rec_len_pmr(stdf_attribute_unused stdf_file *f, stdf_rec_pmr *r)
{
	return
		sizeof(r->PMR_INDX) + sizeof(r->CHAN_TYP) + _lenCn(r->CHAN_NAM) +
		_lenCn(r->PHY_NAM) + _lenCn(r->LOG_NAM) + sizeof(r->HEAD_NUM) +
		sizeof(r->SITE_NUM);
}

static inline size_t _calc_rec_len_pgr(stdf_attribute_unused stdf_file *f, stdf_rec_pgr *r)
{
	return
		sizeof(r->GRP_INDX) + _lenCn(r->GRP_NAM) +
		sizeof(r->INDX_CNT) + _len_dtcX(r->PMR_INDX, r->INDX_CNT);
}

static inline size_t _calc_rec_len_plr(stdf_attribute_unused stdf_file *f, stdf_rec_plr *r)
{
	return
		sizeof(r->GRP_CNT) +
		_len_dtcX(r->GRP_INDX, r->GRP_CNT) +
		_len_dtcX(r->GRP_MODE, r->GRP_CNT) + _len_dtcX(r->GRP_RADX, r->GRP_CNT) +
		_len_dtcXCn(r->PGM_CHAR, r->GRP_CNT) + _len_dtcXCn(r->RTN_CHAR, r->GRP_CNT) +
		_len_dtcXCn(r->PGM_CHAL, r->GRP_CNT) + _len_dtcXCn(r->RTN_CHAL, r->GRP_CNT);
}

static inline size_t _calc_rec_len_rdr(stdf_attribute_unused stdf_file *f, stdf_rec_rdr *r)
{
	return sizeof(r->NUM_BINS) + _len_dtcX(r->RTST_BIN, r->NUM_BINS);
}

static inline size_t _calc_rec_len_sdr(stdf_attribute_unused stdf_file *f, stdf_rec_sdr *r)
{
	return
		sizeof(r->HEAD_NUM) + sizeof(r->SITE_GRP) + sizeof(r->SITE_CNT) +
		_len_dtcX(r->SITE_NUM, r->SITE_CNT) +
		_lenCn(r->HAND_TYP) + _lenCn(r->HAND_ID) + _lenCn(r->CARD_TYP) +
		_lenCn(r->CARD_ID) + _lenCn(r->LOAD_TYP) + _lenCn(r->LOAD_ID) +
		_lenCn(r->DIB_TYP) + _lenCn(r->DIB_ID) + _lenCn(r->CABL_TYP) +
		_lenCn(r->CABL_ID) + _lenCn(r->CONT_TYP) + _lenCn(r->CONT_ID) +
		_lenCn(r->LASR_TYP) + _lenCn(r->LASR_ID) + _lenCn(r->EXTR_TYP) +
		_lenCn(r->EXTR_ID);
}

static inline size_t _calc_rec_len_psr(stdf_attribute_unused stdf_file *f, stdf_rec_psr *r)
{
	return
		sizeof(r->CONT_FLG) + sizeof(r->PSR_INDX) + _lenCn(r->PSR_NAM) +
         	sizeof(r->OPT_FLG) + sizeof(r->TOTP_CNT) +  sizeof(r->LOCP_CNT) +
                _len_dtcX(r->PAT_BGN, r->LOCP_CNT) +
                _len_dtcX(r->PAT_END, r->LOCP_CNT) +
                _len_dtcXCn(r->PAT_FILE, r->LOCP_CNT) +
                _len_dtcXCn(r->PAT_LBL, r->LOCP_CNT) +
                _len_dtcXCn(r->FILE_UID, r->LOCP_CNT) +
                _len_dtcXCn(r->ATPG_DSC, r->LOCP_CNT) +
                _len_dtcXCn(r->SRC_ID, r->LOCP_CNT);
}

static inline size_t _calc_rec_len_nmr(stdf_attribute_unused stdf_file *f, stdf_rec_nmr *r)
{
	return
		sizeof(r->CONT_FLG) + sizeof(r->TOTM_CNT) + sizeof(r->LOCM_CNT) +
                _len_dtcX(r->PMR_INDX, r->LOCM_CNT) +
                _len_dtcXCn(r->ATPG_NAM, r->LOCM_CNT);
}

static inline size_t _calc_rec_len_cnr(stdf_attribute_unused stdf_file *f, stdf_rec_cnr *r)
{
	return
		sizeof(r->CHN_NUM) + sizeof(r->BIT_POS) + _lenSn(r->CELL_NAM);
}

static inline size_t _calc_rec_len_ssr(stdf_attribute_unused stdf_file *f, stdf_rec_ssr *r)
{
	return
		_lenCn(r->SSR_NAM) + sizeof(r->CHN_CNT) +
                _len_dtcX(r->CHN_LIST, r->CHN_CNT);
}

static inline size_t _calc_rec_len_cdr(stdf_attribute_unused stdf_file *f, stdf_rec_cdr *r)
{
	return
		sizeof(r->CONT_FLG) + sizeof(r->CDR_INDX) + _lenCn(r->CHN_NAM) +
                sizeof(r->CHN_LEN) + sizeof(r->SIN_PIN) + sizeof(r->SOUT_PIN) +
                sizeof(r->MSTR_CNT) + _len_dtcX(r->M_CLKS, r->MSTR_CNT) +
                sizeof(r->SLAV_CNT) + _len_dtcX(r->S_CLKS, r->SLAV_CNT) +
                sizeof(r->INV_VAL) + sizeof(r->LST_CNT) +
                _len_dtcXSn(r->CELL_LST, r->LST_CNT);
}

static inline size_t _calc_rec_len_wir(stdf_file *f, stdf_rec_wir *r)
{
#define _CALC_REC_LEN_WIR_v3(r) \
	( \
	sizeof(r->HEAD_NUM) + sizeof(r->PAD_BYTE) + \
	sizeof(r->START_T) + _lenCn(r->WAFER_ID) \
	)
#define _CALC_REC_LEN_WIR_v4(r) \
	( \
	sizeof(r->HEAD_NUM) + sizeof(r->SITE_GRP) + \
	sizeof(r->START_T) + _lenCn(r->WAFER_ID) \
	)
#ifndef STDF_VER3
# undef _CALC_REC_LEN_WIR_v3
# define _CALC_REC_LEN_WIR_v3(r) 0
#endif
	return (f->ver == 3 ? _CALC_REC_LEN_WIR_v3(r) : _CALC_REC_LEN_WIR_v4(r));
}

static inline size_t _calc_rec_len_wrr(stdf_file *f, stdf_rec_wrr *r)
{
#define _CALC_REC_LEN_WRR_v3(r) \
	( \
	sizeof(r->FINISH_T) + sizeof(r->HEAD_NUM) + sizeof(r->PAD_BYTE) + \
	sizeof(r->PART_CNT) + sizeof(r->RTST_CNT) + sizeof(r->ABRT_CNT) + \
	sizeof(r->GOOD_CNT) + sizeof(r->FUNC_CNT) + _lenCn(r->WAFER_ID) + \
	_lenCn(r->HAND_ID) + _lenCn(r->PRB_CARD) + \
	_lenCn(r->USR_DESC) + _lenCn(r->EXC_DESC) \
	)
#define _CALC_REC_LEN_WRR_v4(r) \
	( \
	sizeof(r->HEAD_NUM) + sizeof(r->SITE_GRP) + sizeof(r->FINISH_T) + \
	sizeof(r->PART_CNT) + sizeof(r->RTST_CNT) + sizeof(r->ABRT_CNT) + \
	sizeof(r->GOOD_CNT) + sizeof(r->FUNC_CNT) + _lenCn(r->WAFER_ID) + \
	_lenCn(r->FABWF_ID) + _lenCn(r->FRAME_ID) + _lenCn(r->MASK_ID) + \
	_lenCn(r->USR_DESC) + _lenCn(r->EXC_DESC) \
	)
#ifndef STDF_VER3
# undef _CALC_REC_LEN_WRR_v3
# define _CALC_REC_LEN_WRR_v3(r) 0
#endif
	return (f->ver == 3 ? _CALC_REC_LEN_WRR_v3(r) : _CALC_REC_LEN_WRR_v4(r));
}

static inline size_t _calc_rec_len_wcr(stdf_attribute_unused stdf_file *f, stdf_rec_wcr *r)
{
	return
		sizeof(r->WAFR_SIZ) + sizeof(r->DIE_HT) + sizeof(r->DIE_WID) +
		sizeof(r->WF_UNITS) + sizeof(r->WF_FLAT) + sizeof(r->CENTER_X) +
		sizeof(r->CENTER_Y) + sizeof(r->POS_X) + sizeof(r->POS_Y);
}

static inline size_t _calc_rec_len_pir(stdf_file *f, stdf_rec_pir *r)
{
#define _CALC_REC_LEN_PIR_v3(r) \
	( \
	sizeof(r->HEAD_NUM) + sizeof(r->SITE_NUM) + \
	sizeof(r->X_COORD) + sizeof(r->Y_COORD) + sizeof(r->PART_ID) \
	)
#define _CALC_REC_LEN_PIR_v4(r) \
	( \
	sizeof(r->HEAD_NUM) + sizeof(r->SITE_NUM) \
	)
#ifndef STDF_VER3
# undef _CALC_REC_LEN_PIR_v3
# define _CALC_REC_LEN_PIR_v3(r) 0
#endif
	return (f->ver == 3 ? _CALC_REC_LEN_PIR_v3(r) : _CALC_REC_LEN_PIR_v4(r));
}

static inline size_t _calc_rec_len_prr(stdf_file *f, stdf_rec_prr *r)
{
#define _CALC_REC_LEN_PRR_v3(r) \
	( \
	sizeof(r->HEAD_NUM) + sizeof(r->SITE_NUM) + \
	sizeof(r->NUM_TEST) + sizeof(r->HARD_BIN) + sizeof(r->SOFT_BIN) + \
	sizeof(r->PART_FLG) + sizeof(r->PAD_BYTE) + \
	sizeof(r->X_COORD) + sizeof(r->Y_COORD) + \
	_lenCn(r->PART_ID) + _lenCn(r->PART_TXT) + _lenBn(r->PART_FIX) \
	)
#define _CALC_REC_LEN_PRR_v4(r) \
	( \
	sizeof(r->HEAD_NUM) + sizeof(r->SITE_NUM) + \
	sizeof(r->PART_FLG) + \
	sizeof(r->NUM_TEST) + sizeof(r->HARD_BIN) + sizeof(r->SOFT_BIN) + \
	sizeof(r->X_COORD) + sizeof(r->Y_COORD) + \
	sizeof(r->TEST_T) + \
	_lenCn(r->PART_ID) + _lenCn(r->PART_TXT) + _lenBn(r->PART_FIX) \
	)
#ifndef STDF_VER3
# undef _CALC_REC_LEN_PRR_v3
# define _CALC_REC_LEN_PRR_v3(r) 0
#endif
	return (f->ver == 3 ? _CALC_REC_LEN_PRR_v3(r) : _CALC_REC_LEN_PRR_v4(r));
}

#ifdef STDF_VER3

static inline size_t _calc_rec_len_pdr(stdf_attribute_unused stdf_file *f, stdf_rec_pdr *r)
{
	return
		sizeof(r->TEST_NUM) + sizeof(r->DESC_FLG) + sizeof(r->OPT_FLAG) +
		sizeof(r->RES_SCAL) + _lenCx(r->UNITS, 7) +
		sizeof(r->RES_LDIG) + sizeof(r->RES_RDIG) + sizeof(r->LLM_SCAL) +
		sizeof(r->HLM_SCAL) + sizeof(r->LLM_LDIG) + sizeof(r->LLM_RDIG) +
		sizeof(r->HLM_LDIG) + sizeof(r->HLM_RDIG) + sizeof(r->LO_LIMIT) +
		sizeof(r->HI_LIMIT) + _lenCn(r->TEST_NAM) + _lenCn(r->SEQ_NAME);
}

static inline size_t _calc_rec_len_fdr(stdf_attribute_unused stdf_file *f, stdf_rec_fdr *r)
{
	return
		sizeof(r->TEST_NUM) + sizeof(r->DESC_FLG) +
		_lenCn(r->TEST_NAM) + _lenCn(r->SEQ_NAME);
}

#endif

static inline size_t _calc_rec_len_tsr(stdf_file *f, stdf_rec_tsr *r)
{
#define _CALC_REC_LEN_TSR_v3(r) \
	( \
	sizeof(r->HEAD_NUM) + sizeof(r->SITE_NUM) + \
	sizeof(r->TEST_NUM) + sizeof(r->EXEC_CNT) + sizeof(r->FAIL_CNT) + \
	sizeof(r->ALRM_CNT) + \
	sizeof(r->OPT_FLAG) + sizeof(r->PAD_BYTE) + sizeof(r->TEST_MIN) + \
	sizeof(r->TEST_MAX) + sizeof(r->TST_MEAN) + sizeof(r->TST_SDEV) + \
	sizeof(r->TST_SUMS) + sizeof(r->TST_SQRS) + _lenCn(r->TEST_NAM) + \
	_lenCn(r->SEQ_NAME) \
	)
#define _CALC_REC_LEN_TSR_v4(r) \
	( \
	sizeof(r->HEAD_NUM) + sizeof(r->SITE_NUM) + \
	sizeof(r->TEST_TYP) + \
	sizeof(r->TEST_NUM) + sizeof(r->EXEC_CNT) + sizeof(r->FAIL_CNT) + \
	sizeof(r->ALRM_CNT) + \
	_lenCn(r->TEST_NAM) + _lenCn(r->SEQ_NAME) + _lenCn(r->TEST_LBL) + \
	sizeof(r->OPT_FLAG) + sizeof(r->TEST_TIM) + sizeof(r->TEST_MIN) + \
	sizeof(r->TEST_MAX) + sizeof(r->TST_SUMS) + sizeof(r->TST_SQRS) \
	)
#ifndef STDF_VER3
# undef _CALC_REC_LEN_TSR_v3
# define _CALC_REC_LEN_TSR_v3(r) 0
#endif
	return (f->ver == 3 ? _CALC_REC_LEN_TSR_v3(r) : _CALC_REC_LEN_TSR_v4(r));
}

static inline size_t _calc_rec_len_ptr(stdf_attribute_unused stdf_file *f, stdf_rec_ptr *r)
{
#define _CALC_REC_LEN_PTR_v3(r) \
	( \
	sizeof(r->TEST_NUM) + sizeof(r->HEAD_NUM) + sizeof(r->SITE_NUM) + \
	sizeof(r->TEST_FLG) + sizeof(r->PARM_FLG) + sizeof(r->RESULT) + \
	sizeof(r->OPT_FLAG) + sizeof(r->RES_SCAL) + sizeof(r->RES_LDIG) + \
	sizeof(r->RES_RDIG) + sizeof(r->DESC_FLG) + _lenCx(r->UNITS, 7) + \
	sizeof(r->LLM_SCAL) + sizeof(r->HLM_SCAL) + sizeof(r->LLM_LDIG) + \
	sizeof(r->LLM_RDIG) + sizeof(r->HLM_LDIG) + sizeof(r->HLM_RDIG) + \
	sizeof(r->LO_LIMIT) + sizeof(r->HI_LIMIT) + \
	_lenCn(r->TEST_NAM) + _lenCn(r->SEQ_NAME) + _lenCn(r->TEST_TXT) \
	)
#define _CALC_REC_LEN_PTR_v4(r) \
	( \
	sizeof(r->TEST_NUM) + sizeof(r->HEAD_NUM) + sizeof(r->SITE_NUM) + \
	sizeof(r->TEST_FLG) + sizeof(r->PARM_FLG) + sizeof(r->RESULT) + \
	_lenCn(r->TEST_TXT) + _lenCn(r->ALARM_ID) + \
	sizeof(r->OPT_FLAG) + sizeof(r->RES_SCAL) + sizeof(r->LLM_SCAL) + \
	sizeof(r->HLM_SCAL) + sizeof(r->LO_LIMIT) + sizeof(r->HI_LIMIT) + \
	_lenCn(r->UNITS) + _lenCn(r->C_RESFMT) + _lenCn(r->C_LLMFMT) + \
	_lenCn(r->C_HLMFMT) + sizeof(r->LO_SPEC) + sizeof(r->HI_SPEC) \
	)
#ifndef STDF_VER3
# undef _CALC_REC_LEN_PTR_v3
# define _CALC_REC_LEN_PTR_v3(r) 0
#endif
	return (f->ver == 3 ? _CALC_REC_LEN_PTR_v3(r) : _CALC_REC_LEN_PTR_v4(r));
}

static inline size_t _calc_rec_len_mpr(stdf_attribute_unused stdf_file *f, stdf_rec_mpr *r)
{
	return
		sizeof(r->TEST_NUM) + sizeof(r->HEAD_NUM) + sizeof(r->SITE_NUM) +
		sizeof(r->TEST_FLG) + sizeof(r->PARM_FLG) + sizeof(r->RTN_ICNT) +
		sizeof(r->RSLT_CNT) +
		_len_dtcXN(r->RTN_STAT, r->RTN_ICNT) + _len_dtcX(r->RTN_RSLT, r->RSLT_CNT) +
		_lenCn(r->TEST_TXT) + _lenCn(r->ALARM_ID) + sizeof(r->OPT_FLAG) +
		sizeof(r->RES_SCAL) + sizeof(r->LLM_SCAL) + sizeof(r->HLM_SCAL) +
		sizeof(r->LO_LIMIT) + sizeof(r->HI_LIMIT) + sizeof(r->START_IN) +
		sizeof(r->INCR_IN) + _len_dtcX(r->RTN_INDX, r->RTN_ICNT) +
		_lenCn(r->UNITS) + _lenCn(r->UNITS_IN) + _lenCn(r->C_RESFMT) +
		_lenCn(r->C_LLMFMT) + _lenCn(r->C_HLMFMT) + sizeof(r->LO_SPEC) +
		sizeof(r->HI_SPEC);
}

static inline size_t _calc_rec_len_ftr(stdf_attribute_unused stdf_file *f, stdf_rec_ftr *r)
{
#define _CALC_REC_LEN_FTR_v3(r) \
	( \
	sizeof(r->TEST_NUM) + sizeof(r->HEAD_NUM) + sizeof(r->SITE_NUM) + \
	sizeof(r->TEST_FLG) + sizeof(r->DESC_FLG) + sizeof(r->OPT_FLAG) + \
	sizeof(r->TIME_SET) + sizeof(r->VECT_ADR) + sizeof(r->CYCL_CNT) + \
	sizeof(r->REPT_CNT) + sizeof(r->PCP_ADDR) + sizeof(r->NUM_FAIL) + \
	_lenBn(r->FAIL_PIN) + _lenBn(r->VECT_DAT) + _lenBn(r->DEV_DAT) + \
	_lenBn(r->RPIN_MAP) + _lenCn(r->TEST_NAM) + _lenCn(r->SEQ_NAME) + \
	_lenCn(r->TEST_TXT) \
	)
#define _CALC_REC_LEN_FTR_v4(r) \
	( \
	sizeof(r->TEST_NUM) + sizeof(r->HEAD_NUM) + sizeof(r->SITE_NUM) + \
	sizeof(r->TEST_FLG) + sizeof(r->OPT_FLAG) + sizeof(r->CYCL_CNT) + \
	sizeof(r->REL_VADR) + sizeof(r->REPT_CNT) + sizeof(r->NUM_FAIL) + \
	sizeof(r->XFAIL_AD) + sizeof(r->YFAIL_AD) + sizeof(r->VECT_OFF) + \
	sizeof(r->RTN_ICNT) + sizeof(r->PGM_ICNT) + \
	_len_dtcX(r->RTN_INDX, r->RTN_ICNT) + _len_dtcXN(r->RTN_STAT, r->RTN_ICNT) + \
	_len_dtcX(r->PGM_INDX, r->PGM_ICNT) + _len_dtcXN(r->PGM_STAT, r->PGM_ICNT) + \
	_lenDn(r->FAIL_PIN) + _lenCn(r->VECT_NAM) + _lenCn(r->TIME_SET) + \
	_lenCn(r->OP_CODE) + _lenCn(r->TEST_TXT) + _lenCn(r->ALARM_ID) + \
	_lenCn(r->PROG_TXT) + _lenCn(r->RSLT_TXT) + sizeof(r->PATG_NUM) + \
	_lenDn(r->SPIN_MAP) \
	)
#ifndef STDF_VER3
# undef _CALC_REC_LEN_FTR_v3
# define _CALC_REC_LEN_FTR_v3(r) 0
#endif
	return (f->ver == 3 ? _CALC_REC_LEN_FTR_v3(r) : _CALC_REC_LEN_FTR_v4(r));
}

static inline size_t _calc_rec_len_str(stdf_attribute_unused stdf_file *f, stdf_rec_str *r)
{
        stdf_dtc_U2 len = sizeof(r->CONT_FLG) + sizeof(r->TEST_NUM) + sizeof(r->HEAD_NUM) + sizeof(r->SITE_NUM) +
		sizeof(r->PSR_REF) + sizeof(r->TEST_FLG) + _lenCn(r->LOG_TYP) + _lenCn(r->TEST_TXT) + _lenCn(r->ALARM_ID) +
		_lenCn(r->PROG_TXT) + _lenCn(r->RSLT_TXT) + sizeof(r->Z_VAL) + sizeof(r->FMU_FLG);

    	if (r->FMU_FLG & 1)
                len +=  _lenDn(r->MASK_MAP);
	if ((r->FMU_FLG & 4) == 4)
                len += _lenDn(r->FAL_MAP);

        len +=  sizeof(r->CYCL_CNT) + sizeof(r->TOTF_CNT) +
		sizeof(r->TOTL_CNT) + sizeof(r->CYC_BASE) + sizeof(r->BIT_BASE) + sizeof(r->COND_CNT) +
		sizeof(r->LIM_CNT) + sizeof(r->CYCL_SIZE) + sizeof(r->PMR_SIZE) + sizeof(r->CHN_SIZE) +
		sizeof(r->PAT_SIZE) + sizeof(r->BIT_SIZE) + sizeof(r->U1_SIZE) + sizeof(r->U2_SIZE) +
                sizeof(r->U3_SIZE) + sizeof(r->UTX_SIZE) + sizeof(r->CAP_BGN) + _len_dtcX(r->LIM_INDX, r->LIM_CNT) +
		_len_dtcX(r->LIM_SPEC, r->LIM_CNT) + _len_dtcXCn(r->COND_LST, r->COND_CNT) + sizeof(r->CYC_CNT) +
		_len_dtcXf(r->CYC_CNT, r->CYCL_SIZE) + sizeof(r->PMR_CNT) +  _len_dtcXf(r->PMR_CNT, r->PMR_SIZE) +
		sizeof(r->CHN_CNT) +_len_dtcXf(r->CHN_CNT, r->CHN_SIZE) + sizeof(r->EXP_CNT) +
                _len_dtcX(r->EXP_DATA, r->EXP_CNT) + sizeof(r->CAP_CNT) +  _len_dtcXN(r->CAP_DATA, r->CAP_CNT) +
                sizeof(r->NEW_CNT) +  _len_dtcXN(r->NEW_DATA, r->NEW_CNT) + sizeof(r->PAT_CNT) +
                _len_dtcXf(r->PAT_CNT, r->PAT_SIZE) + sizeof(r->BPOS_CNT) +  _len_dtcXf(r->BPOS_CNT, r->BIT_SIZE) +
                sizeof(r->USR1_CNT) +  _len_dtcXf(r->USR1_CNT, r->U1_SIZE) + sizeof(r->USR2_CNT) +
                _len_dtcXf(r->USR2_CNT, r->U2_SIZE) + sizeof(r->USR3_CNT) +  _len_dtcXf(r->USR3_CNT, r->U3_SIZE) +
                sizeof(r->TXT_CNT) +  _len_dtcXCn(r->USER_TXT, r->TXT_CNT);
        return len;
}

static inline size_t _calc_rec_len_bps(stdf_attribute_unused stdf_file *f, stdf_attribute_unused stdf_rec_bps *r)
{
	return _lenCn(r->SEQ_NAME);
}

static inline size_t _calc_rec_len_eps(stdf_attribute_unused stdf_file *f, stdf_attribute_unused stdf_rec_eps *r)
{
	return 0;
}

#ifdef STDF_VER3

static inline size_t _calc_rec_len_shb(stdf_attribute_unused stdf_file *f, stdf_rec_shb *r)
{
	return
		sizeof(r->HEAD_NUM) + sizeof(r->SITE_NUM) + sizeof(r->HBIN_NUM) +
		sizeof(r->HBIN_CNT) + _lenCn(r->HBIN_NAM);
}

static inline size_t _calc_rec_len_ssb(stdf_attribute_unused stdf_file *f, stdf_rec_ssb *r)
{
	return
		sizeof(r->HEAD_NUM) + sizeof(r->SITE_NUM) + sizeof(r->SBIN_NUM) +
		sizeof(r->SBIN_CNT) + _lenCn(r->SBIN_NAM);
}

static inline size_t _calc_rec_len_sts(stdf_attribute_unused stdf_file *f, stdf_rec_sts *r)
{
	return
		sizeof(r->HEAD_NUM) + sizeof(r->SITE_NUM) + sizeof(r->TEST_NUM) +
		sizeof(r->EXEC_CNT) + sizeof(r->FAIL_CNT) + sizeof(r->ALRM_CNT) +
		sizeof(r->OPT_FLAG) + sizeof(r->PAD_BYTE) + sizeof(r->TEST_MIN) +
		sizeof(r->TEST_MAX) + sizeof(r->TST_MEAN) + sizeof(r->TST_SDEV) +
		sizeof(r->TST_SUMS) + sizeof(r->TST_SQRS) + _lenCn(r->TEST_NAM) +
		_lenCn(r->SEQ_NAME) + _lenCn(r->TEST_LBL);
}

static inline size_t _calc_rec_len_scr(stdf_attribute_unused stdf_file *f, stdf_rec_scr *r)
{
	return
		sizeof(r->HEAD_NUM) + sizeof(r->SITE_NUM) + sizeof(r->FINISH_T) +
		sizeof(r->PART_CNT) + sizeof(r->RTST_CNT) + sizeof(r->ABRT_CNT) +
		sizeof(r->GOOD_CNT) + sizeof(r->FUNC_CNT);
}

#endif

static inline size_t _calc_rec_len_gdr(stdf_attribute_unused stdf_file *f, stdf_attribute_unused stdf_rec_gdr *r)
{
	return sizeof(r->FLD_CNT) + _lenVn(r->GEN_DATA, r->FLD_CNT);
}

static inline size_t _calc_rec_len_dtr(stdf_attribute_unused stdf_file *f, stdf_rec_dtr *r)
{
	return _lenCn(r->TEXT_DAT);
}


/*
 * Writing records
 */


ssize_t _stdf_write_flush(stdf_file *file, size_t count)
{
	size_t write_ret = 1;

	if (count == (size_t)-1) {
		count = file->_write_pos - file->__output;
		if (count == 0)
			return 0;
	} else {
		if (count < file->_write_chunk_size)
			return 0;
		count = file->_write_pos - file->__output;
	}

	file->_write_pos = file->__output;
#if 0
	{
		/* debug */
		ssize_t ret;
		ret = write(file->fd, file->__output, count);
		memset(file->__output, '|', (stdf_dtc_U2)-1);
		return ret;
	}
#else
	while (count > 0 && write_ret > 0) {
		write_ret = write(file->fd, file->_write_pos, count);
		if (write_ret > 0) {
			count -= write_ret;
			file->_write_pos += write_ret;
		}
	}

	/* everything was written, happy happy joy joy */
	if (count == 0) {
		errno = 0;
		count = file->_write_pos - file->__output;
		file->_write_pos = file->__output;
		return count;
	}

	/* did we have a partial write ?  if so, report how many bytes we
	 * managed to write, but keep the rest in the buffer for next time */
	if (file->_write_pos != file->__output) {
		memmove(file->__output, file->_write_pos, count);
		count = file->_write_pos - file->__output;
		file->_write_pos = file->__output;
		return count;
	}

	/* did we totally fail ? */
	return -1;
#endif
}

static inline ssize_t _stdf_check_write_buffer(stdf_file *file, size_t count)
{
	if ((file->_write_pos - file->__output) + count >= (stdf_dtc_U2)-1)
		return _stdf_write_flush(file, (size_t)-1);
	else
		return 0;
}

ssize_t stdf_write_rec_raw(stdf_file *file, stdf_rec_unknown *rec)
{
	_stdf_byte_order_to_src(file,&(rec->header.REC_LEN),sizeof(stdf_dtc_U2));
	_stdf_check_write_buffer(file, rec->header.REC_LEN+4);
	_stdf_write_nbytes(file, rec->data, rec->header.REC_LEN+4);
	return _stdf_write_flush(file, rec->header.REC_LEN+4);
}

ssize_t stdf_write_rec_far(stdf_file *file, stdf_rec_far *far)
{
	if (!far->header.REC_LEN)
		SET_HEADER(far->header, STDF_REC_FAR, _calc_rec_len_far(file, far));
	_stdf_check_write_buffer(file, far->header.REC_LEN);
	_stdf_write_dtc_header(file, &(far->header));
	_stdf_write_dtc_U1(file, far->CPU_TYPE);
	_stdf_write_dtc_U1(file, far->STDF_VER);
	return _stdf_write_flush(file, far->header.REC_LEN);
}

ssize_t stdf_write_rec_atr(stdf_file *file, stdf_rec_atr *atr)
{
	if (!atr->header.REC_LEN)
		SET_HEADER(atr->header, STDF_REC_ATR, _calc_rec_len_atr(file, atr));
	_stdf_check_write_buffer(file, atr->header.REC_LEN);
	_stdf_write_dtc_header(file, &(atr->header));
	_stdf_write_dtc_U4(file, atr->MOD_TIM);
	_stdf_write_dtc_Cn(file, atr->CMD_LINE);
	return _stdf_write_flush(file, atr->header.REC_LEN);
}

ssize_t stdf_write_rec_vur(stdf_file *file, stdf_rec_vur *vur)
{
	if (!vur->header.REC_LEN)
		SET_HEADER(vur->header, STDF_REC_VUR, _calc_rec_len_vur(file, vur));
	_stdf_check_write_buffer(file, vur->header.REC_LEN);
	_stdf_write_dtc_header(file, &(vur->header));
	_stdf_write_dtc_Cn(file, vur->UPD_NAM);
	return _stdf_write_flush(file, vur->header.REC_LEN);
}

ssize_t stdf_write_rec_mir(stdf_file *file, stdf_rec_mir *mir)
{
	if (!mir->header.REC_LEN)
		SET_HEADER(mir->header, STDF_REC_MIR, _calc_rec_len_mir(file, mir));
	_stdf_check_write_buffer(file, mir->header.REC_LEN);
	_stdf_write_dtc_header(file, &(mir->header));
#ifdef STDF_VER3
	if (file->ver == 4) {
#endif
	_stdf_write_dtc_U4(file, mir->SETUP_T);
	_stdf_write_dtc_U4(file, mir->START_T);
	_stdf_write_dtc_U1(file, mir->STAT_NUM);
	_stdf_write_dtc_C1(file, mir->MODE_COD);
	_stdf_write_dtc_C1(file, mir->RTST_COD);
	_stdf_write_dtc_C1(file, mir->PROT_COD);
	_stdf_write_dtc_U2(file, mir->BURN_TIM);
	_stdf_write_dtc_C1(file, mir->CMOD_COD);
	_stdf_write_dtc_Cn(file, mir->LOT_ID);
	_stdf_write_dtc_Cn(file, mir->PART_TYP);
	_stdf_write_dtc_Cn(file, mir->NODE_NAM);
	_stdf_write_dtc_Cn(file, mir->TSTR_TYP);
	_stdf_write_dtc_Cn(file, mir->JOB_NAM);
	_stdf_write_dtc_Cn(file, mir->JOB_REV);
	_stdf_write_dtc_Cn(file, mir->SBLOT_ID);
	_stdf_write_dtc_Cn(file, mir->OPER_NAM);
	_stdf_write_dtc_Cn(file, mir->EXEC_TYP);
	_stdf_write_dtc_Cn(file, mir->EXEC_VER);
	_stdf_write_dtc_Cn(file, mir->TEST_COD);
	_stdf_write_dtc_Cn(file, mir->TST_TEMP);
	_stdf_write_dtc_Cn(file, mir->USER_TXT);
	_stdf_write_dtc_Cn(file, mir->AUX_FILE);
	_stdf_write_dtc_Cn(file, mir->PKG_TYP);
	_stdf_write_dtc_Cn(file, mir->FAMILY_ID);
	_stdf_write_dtc_Cn(file, mir->DATE_COD);
	_stdf_write_dtc_Cn(file, mir->FACIL_ID);
	_stdf_write_dtc_Cn(file, mir->FLOOR_ID);
	_stdf_write_dtc_Cn(file, mir->PROC_ID);
	_stdf_write_dtc_Cn(file, mir->OPER_FRQ);
	_stdf_write_dtc_Cn(file, mir->SPEC_NAM);
	_stdf_write_dtc_Cn(file, mir->SPEC_VER);
	_stdf_write_dtc_Cn(file, mir->FLOW_ID);
	_stdf_write_dtc_Cn(file, mir->SETUP_ID);
	_stdf_write_dtc_Cn(file, mir->DSGN_REV);
	_stdf_write_dtc_Cn(file, mir->ENG_ID);
	_stdf_write_dtc_Cn(file, mir->ROM_COD);
	_stdf_write_dtc_Cn(file, mir->SERL_NUM);
	_stdf_write_dtc_Cn(file, mir->SUPR_NAM);
#ifdef STDF_VER3
	} else {
	_stdf_write_dtc_U1(file, mir->CPU_TYPE);
	_stdf_write_dtc_U1(file, mir->STDF_VER);
	_stdf_write_dtc_C1(file, mir->MODE_COD);
	_stdf_write_dtc_U1(file, mir->STAT_NUM);
	_stdf_write_dtc_Cx(file, mir->TEST_COD, 3);
	_stdf_write_dtc_C1(file, mir->RTST_COD);
	_stdf_write_dtc_C1(file, mir->PROT_COD);
	_stdf_write_dtc_C1(file, mir->CMOD_COD);
	_stdf_write_dtc_U4(file, mir->SETUP_T);
	_stdf_write_dtc_U4(file, mir->START_T);
	_stdf_write_dtc_Cn(file, mir->LOT_ID);
	_stdf_write_dtc_Cn(file, mir->PART_TYP);
	_stdf_write_dtc_Cn(file, mir->JOB_NAM);
	_stdf_write_dtc_Cn(file, mir->OPER_NAM);
	_stdf_write_dtc_Cn(file, mir->NODE_NAM);
	_stdf_write_dtc_Cn(file, mir->TSTR_TYP);
	_stdf_write_dtc_Cn(file, mir->EXEC_TYP);
	_stdf_write_dtc_Cn(file, mir->SUPR_NAM);
	_stdf_write_dtc_Cn(file, mir->HAND_ID);
	_stdf_write_dtc_Cn(file, mir->SBLOT_ID);
	_stdf_write_dtc_Cn(file, mir->JOB_REV);
	_stdf_write_dtc_Cn(file, mir->PROC_ID);
	_stdf_write_dtc_Cn(file, mir->PRB_CARD);
	}
#endif
	return _stdf_write_flush(file, mir->header.REC_LEN);
}

ssize_t stdf_write_rec_mrr(stdf_file *file, stdf_rec_mrr *mrr)
{
	if (!mrr->header.REC_LEN)
		SET_HEADER(mrr->header, STDF_REC_MRR, _calc_rec_len_mrr(file, mrr));
	_stdf_check_write_buffer(file, mrr->header.REC_LEN);
	_stdf_write_dtc_header(file, &(mrr->header));
	_stdf_write_dtc_U4(file, mrr->FINISH_T);
#ifdef STDF_VER3
	if (file->ver == 3) {
	_stdf_write_dtc_U4(file, mrr->PART_CNT);
	_stdf_write_dtc_U4(file, mrr->RTST_CNT);
	_stdf_write_dtc_U4(file, mrr->ABRT_CNT);
	_stdf_write_dtc_U4(file, mrr->GOOD_CNT);
	_stdf_write_dtc_U4(file, mrr->FUNC_CNT);
	}
#endif
	_stdf_write_dtc_C1(file, mrr->DISP_COD);
	_stdf_write_dtc_Cn(file, mrr->USR_DESC);
	_stdf_write_dtc_Cn(file, mrr->EXC_DESC);
	return _stdf_write_flush(file, mrr->header.REC_LEN);
}

ssize_t stdf_write_rec_pcr(stdf_file *file, stdf_rec_pcr *pcr)
{
	if (!pcr->header.REC_LEN)
		SET_HEADER(pcr->header, STDF_REC_PCR, _calc_rec_len_pcr(file, pcr));
	_stdf_check_write_buffer(file, pcr->header.REC_LEN);
	_stdf_write_dtc_header(file, &(pcr->header));
	_stdf_write_dtc_U1(file, pcr->HEAD_NUM);
	_stdf_write_dtc_U1(file, pcr->SITE_NUM);
	_stdf_write_dtc_U4(file, pcr->PART_CNT);
	_stdf_write_dtc_U4(file, pcr->RTST_CNT);
	_stdf_write_dtc_U4(file, pcr->ABRT_CNT);
	_stdf_write_dtc_U4(file, pcr->GOOD_CNT);
	_stdf_write_dtc_U4(file, pcr->FUNC_CNT);
	return _stdf_write_flush(file, pcr->header.REC_LEN);
}

ssize_t stdf_write_rec_hbr(stdf_file *file, stdf_rec_hbr *hbr)
{
	if (!hbr->header.REC_LEN)
		SET_HEADER(hbr->header, STDF_REC_HBR, _calc_rec_len_hbr(file, hbr));
	_stdf_check_write_buffer(file, hbr->header.REC_LEN);
	_stdf_write_dtc_header(file, &(hbr->header));
	_stdf_write_dtc_U1(file, hbr->HEAD_NUM);
	_stdf_write_dtc_U1(file, hbr->SITE_NUM);
	_stdf_write_dtc_U2(file, hbr->HBIN_NUM);
	_stdf_write_dtc_U4(file, hbr->HBIN_CNT);
	_stdf_write_dtc_C1(file, hbr->HBIN_PF);
	_stdf_write_dtc_Cn(file, hbr->HBIN_NAM);
	return _stdf_write_flush(file, hbr->header.REC_LEN);
}

ssize_t stdf_write_rec_sbr(stdf_file *file, stdf_rec_sbr *sbr)
{
	if (!sbr->header.REC_LEN)
		SET_HEADER(sbr->header, STDF_REC_SBR, _calc_rec_len_sbr(file, sbr));
	_stdf_check_write_buffer(file, sbr->header.REC_LEN);
	_stdf_write_dtc_header(file, &(sbr->header));
	_stdf_write_dtc_U1(file, sbr->HEAD_NUM);
	_stdf_write_dtc_U1(file, sbr->SITE_NUM);
	_stdf_write_dtc_U2(file, sbr->SBIN_NUM);
	_stdf_write_dtc_U4(file, sbr->SBIN_CNT);
	_stdf_write_dtc_C1(file, sbr->SBIN_PF);
	_stdf_write_dtc_Cn(file, sbr->SBIN_NAM);
	return _stdf_write_flush(file, sbr->header.REC_LEN);
}

ssize_t stdf_write_rec_pmr(stdf_file *file, stdf_rec_pmr *pmr)
{
	if (!pmr->header.REC_LEN)
		SET_HEADER(pmr->header, STDF_REC_PMR, _calc_rec_len_pmr(file, pmr));
	_stdf_check_write_buffer(file, pmr->header.REC_LEN);
	_stdf_write_dtc_header(file, &(pmr->header));
	_stdf_write_dtc_U2(file, pmr->PMR_INDX);
	_stdf_write_dtc_U2(file, pmr->CHAN_TYP);
	_stdf_write_dtc_Cn(file, pmr->CHAN_NAM);
	_stdf_write_dtc_Cn(file, pmr->PHY_NAM);
	_stdf_write_dtc_Cn(file, pmr->LOG_NAM);
	_stdf_write_dtc_U1(file, pmr->HEAD_NUM);
	_stdf_write_dtc_U1(file, pmr->SITE_NUM);
	return _stdf_write_flush(file, pmr->header.REC_LEN);
}

ssize_t stdf_write_rec_pgr(stdf_file *file, stdf_rec_pgr *pgr)
{
	if (!pgr->header.REC_LEN)
		SET_HEADER(pgr->header, STDF_REC_PGR, _calc_rec_len_pgr(file, pgr));
	_stdf_check_write_buffer(file, pgr->header.REC_LEN);
	_stdf_write_dtc_header(file, &(pgr->header));
	_stdf_write_dtc_U2(file, pgr->GRP_INDX);
	_stdf_write_dtc_Cn(file, pgr->GRP_NAM);
	_stdf_write_dtc_U2(file, pgr->INDX_CNT);
	_stdf_write_dtc_xU2(file, pgr->PMR_INDX, pgr->INDX_CNT);
	return _stdf_write_flush(file, pgr->header.REC_LEN);
}

ssize_t stdf_write_rec_plr(stdf_file *file, stdf_rec_plr *plr)
{
	if (!plr->header.REC_LEN)
		SET_HEADER(plr->header, STDF_REC_PLR, _calc_rec_len_plr(file, plr));
	_stdf_check_write_buffer(file, plr->header.REC_LEN);
	_stdf_write_dtc_header(file, &(plr->header));
	_stdf_write_dtc_U2(file, plr->GRP_CNT);
	_stdf_write_dtc_xU2(file, plr->GRP_INDX, plr->GRP_CNT);
	_stdf_write_dtc_xU2(file, plr->GRP_MODE, plr->GRP_CNT);
	_stdf_write_dtc_xU1(file, plr->GRP_RADX, plr->GRP_CNT);
	_stdf_write_dtc_xCn(file, plr->PGM_CHAR, plr->GRP_CNT);
	_stdf_write_dtc_xCn(file, plr->RTN_CHAR, plr->GRP_CNT);
	_stdf_write_dtc_xCn(file, plr->PGM_CHAL, plr->GRP_CNT);
	_stdf_write_dtc_xCn(file, plr->RTN_CHAL, plr->GRP_CNT);
	return _stdf_write_flush(file, plr->header.REC_LEN);
}

ssize_t stdf_write_rec_rdr(stdf_file *file, stdf_rec_rdr *rdr)
{
	if (!rdr->header.REC_LEN)
		SET_HEADER(rdr->header, STDF_REC_RDR, _calc_rec_len_rdr(file, rdr));
	_stdf_check_write_buffer(file, rdr->header.REC_LEN);
	_stdf_write_dtc_header(file, &(rdr->header));
	_stdf_write_dtc_U2(file, rdr->NUM_BINS);
	_stdf_write_dtc_xU2(file, rdr->RTST_BIN, rdr->NUM_BINS);
	return _stdf_write_flush(file, rdr->header.REC_LEN);
}

ssize_t stdf_write_rec_sdr(stdf_file *file, stdf_rec_sdr *sdr)
{
	if (!sdr->header.REC_LEN)
		SET_HEADER(sdr->header, STDF_REC_SDR, _calc_rec_len_sdr(file, sdr));
	_stdf_check_write_buffer(file, sdr->header.REC_LEN);
	_stdf_write_dtc_header(file, &(sdr->header));
	_stdf_write_dtc_U1(file, sdr->HEAD_NUM);
	_stdf_write_dtc_U1(file, sdr->SITE_GRP);
	_stdf_write_dtc_U1(file, sdr->SITE_CNT);
	_stdf_write_dtc_xU1(file, sdr->SITE_NUM, sdr->SITE_CNT);
	_stdf_write_dtc_Cn(file, sdr->HAND_TYP);
	_stdf_write_dtc_Cn(file, sdr->HAND_ID);
	_stdf_write_dtc_Cn(file, sdr->CARD_TYP);
	_stdf_write_dtc_Cn(file, sdr->CARD_ID);
	_stdf_write_dtc_Cn(file, sdr->LOAD_TYP);
	_stdf_write_dtc_Cn(file, sdr->LOAD_ID);
	_stdf_write_dtc_Cn(file, sdr->DIB_TYP);
	_stdf_write_dtc_Cn(file, sdr->DIB_ID);
	_stdf_write_dtc_Cn(file, sdr->CABL_TYP);
	_stdf_write_dtc_Cn(file, sdr->CABL_ID);
	_stdf_write_dtc_Cn(file, sdr->CONT_TYP);
	_stdf_write_dtc_Cn(file, sdr->CONT_ID);
	_stdf_write_dtc_Cn(file, sdr->LASR_TYP);
	_stdf_write_dtc_Cn(file, sdr->LASR_ID);
	_stdf_write_dtc_Cn(file, sdr->EXTR_TYP);
	_stdf_write_dtc_Cn(file, sdr->EXTR_ID);
	return _stdf_write_flush(file, sdr->header.REC_LEN);
}

ssize_t stdf_write_rec_psr(stdf_file *file, stdf_rec_psr *psr)
{
	if (!psr->header.REC_LEN)
		SET_HEADER(psr->header, STDF_REC_PSR, _calc_rec_len_psr(file, psr));
	_stdf_check_write_buffer(file, psr->header.REC_LEN);
	_stdf_write_dtc_header(file, &(psr->header));
	_stdf_write_dtc_B1(file, psr->CONT_FLG);
	_stdf_write_dtc_U2(file, psr->PSR_INDX);
	_stdf_write_dtc_Cn(file, psr->PSR_NAM);
	_stdf_write_dtc_B1(file, psr->OPT_FLG);
	_stdf_write_dtc_U2(file, psr->TOTP_CNT);
	_stdf_write_dtc_U2(file, psr->LOCP_CNT);
	_stdf_write_dtc_xU8(file, psr->PAT_BGN, psr->LOCP_CNT);
	_stdf_write_dtc_xU8(file, psr->PAT_END, psr->LOCP_CNT);
	_stdf_write_dtc_xCn(file, psr->PAT_FILE, psr->LOCP_CNT);
	_stdf_write_dtc_xCn(file, psr->PAT_LBL, psr->LOCP_CNT);
	_stdf_write_dtc_xCn(file, psr->FILE_UID, psr->LOCP_CNT);
	_stdf_write_dtc_xCn(file, psr->ATPG_DSC, psr->LOCP_CNT);
	_stdf_write_dtc_xCn(file, psr->SRC_ID, psr->LOCP_CNT);
	return _stdf_write_flush(file, psr->header.REC_LEN);
}

ssize_t stdf_write_rec_nmr(stdf_file *file, stdf_rec_nmr *nmr)
{
	if (!nmr->header.REC_LEN)
		SET_HEADER(nmr->header, STDF_REC_NMR, _calc_rec_len_nmr(file, nmr));
	_stdf_check_write_buffer(file, nmr->header.REC_LEN);
	_stdf_write_dtc_header(file, &(nmr->header));
	_stdf_write_dtc_B1(file, nmr->CONT_FLG);
	_stdf_write_dtc_U2(file, nmr->TOTM_CNT);
	_stdf_write_dtc_U2(file, nmr->LOCM_CNT);
	_stdf_write_dtc_xU2(file, nmr->PMR_INDX, nmr->LOCM_CNT);
	_stdf_write_dtc_xCn(file, nmr->ATPG_NAM, nmr->LOCM_CNT);
	return _stdf_write_flush(file, nmr->header.REC_LEN);
}

ssize_t stdf_write_rec_cnr(stdf_file *file, stdf_rec_cnr *cnr)
{
	if (!cnr->header.REC_LEN)
		SET_HEADER(cnr->header, STDF_REC_CNR, _calc_rec_len_cnr(file, cnr));
	_stdf_check_write_buffer(file, cnr->header.REC_LEN);
	_stdf_write_dtc_header(file, &(cnr->header));
	_stdf_write_dtc_U2(file, cnr->CHN_NUM);
	_stdf_write_dtc_U4(file, cnr->BIT_POS);
	_stdf_write_dtc_Sn(file, cnr->CELL_NAM);
	return _stdf_write_flush(file, cnr->header.REC_LEN);
}

ssize_t stdf_write_rec_ssr(stdf_file *file, stdf_rec_ssr *ssr)
{
	if (!ssr->header.REC_LEN)
		SET_HEADER(ssr->header, STDF_REC_SSR, _calc_rec_len_ssr(file, ssr));
	_stdf_check_write_buffer(file, ssr->header.REC_LEN);
	_stdf_write_dtc_header(file, &(ssr->header));
	_stdf_write_dtc_Cn(file, ssr->SSR_NAM);
	_stdf_write_dtc_U2(file, ssr->CHN_CNT);
	_stdf_write_dtc_xU2(file, ssr->CHN_LIST, ssr->CHN_CNT);
	return _stdf_write_flush(file, ssr->header.REC_LEN);
}

ssize_t stdf_write_rec_cdr(stdf_file *file, stdf_rec_cdr *cdr)
{
	if (!cdr->header.REC_LEN)
		SET_HEADER(cdr->header, STDF_REC_CDR, _calc_rec_len_cdr(file, cdr));
	_stdf_check_write_buffer(file, cdr->header.REC_LEN);
	_stdf_write_dtc_header(file, &(cdr->header));
        _stdf_write_dtc_B1(file, cdr->CONT_FLG);
	_stdf_write_dtc_U2(file, cdr->CDR_INDX);
	_stdf_write_dtc_Cn(file, cdr->CHN_NAM);
	_stdf_write_dtc_U4(file, cdr->CHN_LEN);
	_stdf_write_dtc_U2(file, cdr->SIN_PIN);
	_stdf_write_dtc_U2(file, cdr->SOUT_PIN);
	_stdf_write_dtc_U1(file, cdr->MSTR_CNT);
	_stdf_write_dtc_xU2(file, cdr->M_CLKS, cdr->MSTR_CNT);
	_stdf_write_dtc_U1(file, cdr->SLAV_CNT);
	_stdf_write_dtc_xU2(file, cdr->S_CLKS, cdr->SLAV_CNT);
	_stdf_write_dtc_U1(file, cdr->INV_VAL);
	_stdf_write_dtc_U2(file, cdr->LST_CNT);
        _stdf_write_dtc_xSn(file, cdr->CELL_LST, cdr->LST_CNT);
	return _stdf_write_flush(file, cdr->header.REC_LEN);
}

ssize_t stdf_write_rec_wir(stdf_file *file, stdf_rec_wir *wir)
{
	if (!wir->header.REC_LEN)
		SET_HEADER(wir->header, STDF_REC_WIR, _calc_rec_len_wir(file, wir));
	_stdf_check_write_buffer(file, wir->header.REC_LEN);
	_stdf_write_dtc_header(file, &(wir->header));
	_stdf_write_dtc_U1(file, wir->HEAD_NUM);
#ifdef STDF_VER3
	if (file->ver == 3)
	_stdf_write_dtc_B1(file, wir->PAD_BYTE);
	else
#endif
	_stdf_write_dtc_U1(file, wir->SITE_GRP);
	_stdf_write_dtc_U4(file, wir->START_T);
	_stdf_write_dtc_Cn(file, wir->WAFER_ID);
	return _stdf_write_flush(file, wir->header.REC_LEN);
}

ssize_t stdf_write_rec_wrr(stdf_file *file, stdf_rec_wrr *wrr)
{
	if (!wrr->header.REC_LEN)
		SET_HEADER(wrr->header, STDF_REC_WRR, _calc_rec_len_wrr(file, wrr));
	_stdf_check_write_buffer(file, wrr->header.REC_LEN);
	_stdf_write_dtc_header(file, &(wrr->header));
#ifdef STDF_VER3
	if (file->ver == 4) {
#endif
	_stdf_write_dtc_U1(file, wrr->HEAD_NUM);
	_stdf_write_dtc_U1(file, wrr->SITE_GRP);
	_stdf_write_dtc_U4(file, wrr->FINISH_T);
#ifdef STDF_VER3
	} else {
	_stdf_write_dtc_U4(file, wrr->FINISH_T);
	_stdf_write_dtc_U1(file, wrr->HEAD_NUM);
	_stdf_write_dtc_B1(file, wrr->PAD_BYTE);
	}
#endif
	_stdf_write_dtc_U4(file, wrr->PART_CNT);
	_stdf_write_dtc_U4(file, wrr->RTST_CNT);
	_stdf_write_dtc_U4(file, wrr->ABRT_CNT);
	_stdf_write_dtc_U4(file, wrr->GOOD_CNT);
	_stdf_write_dtc_U4(file, wrr->FUNC_CNT);
	_stdf_write_dtc_Cn(file, wrr->WAFER_ID);
#ifdef STDF_VER3
	if (file->ver == 4) {
#endif
	_stdf_write_dtc_Cn(file, wrr->FABWF_ID);
	_stdf_write_dtc_Cn(file, wrr->FRAME_ID);
	_stdf_write_dtc_Cn(file, wrr->MASK_ID);
#ifdef STDF_VER3
	} else {
	_stdf_write_dtc_Cn(file, wrr->HAND_ID);
	_stdf_write_dtc_Cn(file, wrr->PRB_CARD);
	}
#endif
	_stdf_write_dtc_Cn(file, wrr->USR_DESC);
	_stdf_write_dtc_Cn(file, wrr->EXC_DESC);
	return _stdf_write_flush(file, wrr->header.REC_LEN);
}

ssize_t stdf_write_rec_wcr(stdf_file *file, stdf_rec_wcr *wcr)
{
	if (!wcr->header.REC_LEN)
		SET_HEADER(wcr->header, STDF_REC_WCR, _calc_rec_len_wcr(file, wcr));
	_stdf_check_write_buffer(file, wcr->header.REC_LEN);
	_stdf_write_dtc_header(file, &(wcr->header));
	_stdf_write_dtc_R4(file, wcr->WAFR_SIZ);
	_stdf_write_dtc_R4(file, wcr->DIE_HT);
	_stdf_write_dtc_R4(file, wcr->DIE_WID);
	_stdf_write_dtc_U1(file, wcr->WF_UNITS);
	_stdf_write_dtc_C1(file, wcr->WF_FLAT);
	_stdf_write_dtc_I2(file, wcr->CENTER_X);
	_stdf_write_dtc_I2(file, wcr->CENTER_Y);
	_stdf_write_dtc_C1(file, wcr->POS_X);
	_stdf_write_dtc_C1(file, wcr->POS_Y);
	return _stdf_write_flush(file, wcr->header.REC_LEN);
}

ssize_t stdf_write_rec_pir(stdf_file *file, stdf_rec_pir *pir)
{
	if (!pir->header.REC_LEN)
		SET_HEADER(pir->header, STDF_REC_PIR, _calc_rec_len_pir(file, pir));
	_stdf_check_write_buffer(file, pir->header.REC_LEN);
	_stdf_write_dtc_header(file, &(pir->header));
	_stdf_write_dtc_U1(file, pir->HEAD_NUM);
	_stdf_write_dtc_U1(file, pir->SITE_NUM);
#ifdef STDF_VER3
	if (file->ver == 3) {
	_stdf_write_dtc_I2(file, pir->X_COORD);
	_stdf_write_dtc_I2(file, pir->Y_COORD);
	_stdf_write_dtc_Cn(file, pir->PART_ID);
	}
#endif
	return _stdf_write_flush(file, pir->header.REC_LEN);
}

ssize_t stdf_write_rec_prr(stdf_file *file, stdf_rec_prr *prr)
{
	if (!prr->header.REC_LEN)
		SET_HEADER(prr->header, STDF_REC_PRR, _calc_rec_len_prr(file, prr));
	_stdf_check_write_buffer(file, prr->header.REC_LEN);
	_stdf_write_dtc_header(file, &(prr->header));
	_stdf_write_dtc_U1(file, prr->HEAD_NUM);
	_stdf_write_dtc_U1(file, prr->SITE_NUM);
#ifdef STDF_VER3
	if (file->ver == 4)
#endif
	_stdf_write_dtc_B1(file, prr->PART_FLG);
	_stdf_write_dtc_U2(file, prr->NUM_TEST);
	_stdf_write_dtc_U2(file, prr->HARD_BIN);
	_stdf_write_dtc_U2(file, prr->SOFT_BIN);
#ifdef STDF_VER3
	if (file->ver == 3) {
	_stdf_write_dtc_B1(file, prr->PART_FLG);
	_stdf_write_dtc_B1(file, prr->PAD_BYTE);
	}
#endif
	_stdf_write_dtc_I2(file, prr->X_COORD);
	_stdf_write_dtc_I2(file, prr->Y_COORD);
#ifdef STDF_VER3
	if (file->ver == 4)
#endif
	_stdf_write_dtc_U4(file, prr->TEST_T);
	_stdf_write_dtc_Cn(file, prr->PART_ID);
	_stdf_write_dtc_Cn(file, prr->PART_TXT);
	_stdf_write_dtc_Bn(file, prr->PART_FIX);
	return _stdf_write_flush(file, prr->header.REC_LEN);
}

#ifdef STDF_VER3

ssize_t stdf_write_rec_pdr(stdf_file *file, stdf_rec_pdr *pdr)
{
	if (!pdr->header.REC_LEN)
		SET_HEADER(pdr->header, STDF_REC_PDR, _calc_rec_len_pdr(file, pdr));
	_stdf_check_write_buffer(file, pdr->header.REC_LEN);
	_stdf_write_dtc_header(file, &(pdr->header));
	_stdf_write_dtc_U4(file, pdr->TEST_NUM);
	_stdf_write_dtc_B1(file, pdr->DESC_FLG);
	_stdf_write_dtc_B1(file, pdr->OPT_FLAG);
	_stdf_write_dtc_I1(file, pdr->RES_SCAL);
	_stdf_write_dtc_Cx(file, pdr->UNITS, 7);
	_stdf_write_dtc_U1(file, pdr->RES_LDIG);
	_stdf_write_dtc_U1(file, pdr->RES_RDIG);
	_stdf_write_dtc_I1(file, pdr->LLM_SCAL);
	_stdf_write_dtc_I1(file, pdr->HLM_SCAL);
	_stdf_write_dtc_U1(file, pdr->LLM_LDIG);
	_stdf_write_dtc_U1(file, pdr->LLM_RDIG);
	_stdf_write_dtc_U1(file, pdr->HLM_LDIG);
	_stdf_write_dtc_U1(file, pdr->HLM_RDIG);
	_stdf_write_dtc_R4(file, pdr->LO_LIMIT);
	_stdf_write_dtc_R4(file, pdr->HI_LIMIT);
	_stdf_write_dtc_Cn(file, pdr->TEST_NAM);
	_stdf_write_dtc_Cn(file, pdr->SEQ_NAME);
	return _stdf_write_flush(file, pdr->header.REC_LEN);
}

ssize_t stdf_write_rec_fdr(stdf_file *file, stdf_rec_fdr *fdr)
{
	if (!fdr->header.REC_LEN)
		SET_HEADER(fdr->header, STDF_REC_FDR, _calc_rec_len_fdr(file, fdr));
	_stdf_check_write_buffer(file, fdr->header.REC_LEN);
	_stdf_write_dtc_header(file, &(fdr->header));
	_stdf_write_dtc_U4(file, fdr->TEST_NUM);
	_stdf_write_dtc_B1(file, fdr->DESC_FLG);
	_stdf_write_dtc_Cn(file, fdr->TEST_NAM);
	_stdf_write_dtc_Cn(file, fdr->SEQ_NAME);
	return _stdf_write_flush(file, fdr->header.REC_LEN);
}

#endif

ssize_t stdf_write_rec_tsr(stdf_file *file, stdf_rec_tsr *tsr)
{
	if (!tsr->header.REC_LEN)
		SET_HEADER(tsr->header, STDF_REC_TSR, _calc_rec_len_tsr(file, tsr));
	_stdf_check_write_buffer(file, tsr->header.REC_LEN);
	_stdf_write_dtc_header(file, &(tsr->header));
	_stdf_write_dtc_U1(file, tsr->HEAD_NUM);
	_stdf_write_dtc_U1(file, tsr->SITE_NUM);
#ifdef STDF_VER3
	if (file->ver == 4)
#endif
	_stdf_write_dtc_C1(file, tsr->TEST_TYP);
	_stdf_write_dtc_U4(file, tsr->TEST_NUM);
#ifdef STDF_VER3
	if (file->ver == 4) {
#endif
	_stdf_write_dtc_U4(file, tsr->EXEC_CNT);
	_stdf_write_dtc_U4(file, tsr->FAIL_CNT);
	_stdf_write_dtc_U4(file, tsr->ALRM_CNT);
	_stdf_write_dtc_Cn(file, tsr->TEST_NAM);
	_stdf_write_dtc_Cn(file, tsr->SEQ_NAME);
	_stdf_write_dtc_Cn(file, tsr->TEST_LBL);
	_stdf_write_dtc_B1(file, tsr->OPT_FLAG);
	_stdf_write_dtc_R4(file, tsr->TEST_TIM);
	_stdf_write_dtc_R4(file, tsr->TEST_MIN);
	_stdf_write_dtc_R4(file, tsr->TEST_MAX);
	_stdf_write_dtc_R4(file, tsr->TST_SUMS);
	_stdf_write_dtc_R4(file, tsr->TST_SQRS);
#ifdef STDF_VER3
	} else {
	_stdf_write_dtc_I4(file, tsr->EXEC_CNT);
	_stdf_write_dtc_I4(file, tsr->FAIL_CNT);
	_stdf_write_dtc_I4(file, tsr->ALRM_CNT);
	_stdf_write_dtc_B1(file, tsr->OPT_FLAG);
	_stdf_write_dtc_B1(file, tsr->PAD_BYTE);
	_stdf_write_dtc_R4(file, tsr->TEST_MIN);
	_stdf_write_dtc_R4(file, tsr->TEST_MAX);
	_stdf_write_dtc_R4(file, tsr->TST_MEAN);
	_stdf_write_dtc_R4(file, tsr->TST_SDEV);
	_stdf_write_dtc_R4(file, tsr->TST_SUMS);
	_stdf_write_dtc_R4(file, tsr->TST_SQRS);
	_stdf_write_dtc_Cn(file, tsr->TEST_NAM);
	_stdf_write_dtc_Cn(file, tsr->SEQ_NAME);
	}
#endif
	return _stdf_write_flush(file, tsr->header.REC_LEN);
}

ssize_t stdf_write_rec_ptr(stdf_file *file, stdf_rec_ptr *ptr)
{
	if (!ptr->header.REC_LEN)
		SET_HEADER(ptr->header, STDF_REC_PTR, _calc_rec_len_ptr(file, ptr));
	_stdf_check_write_buffer(file, ptr->header.REC_LEN);
	_stdf_write_dtc_header(file, &(ptr->header));
	_stdf_write_dtc_U4(file, ptr->TEST_NUM);
	_stdf_write_dtc_U1(file, ptr->HEAD_NUM);
	_stdf_write_dtc_U1(file, ptr->SITE_NUM);
	_stdf_write_dtc_B1(file, ptr->TEST_FLG);
	_stdf_write_dtc_B1(file, ptr->PARM_FLG);
	_stdf_write_dtc_R4(file, ptr->RESULT);
#ifdef STDF_VER3
	if (file->ver == 4) {
#endif
	_stdf_write_dtc_Cn(file, ptr->TEST_TXT);
	_stdf_write_dtc_Cn(file, ptr->ALARM_ID);
	_stdf_write_dtc_B1(file, ptr->OPT_FLAG);
	_stdf_write_dtc_I1(file, ptr->RES_SCAL);
	_stdf_write_dtc_I1(file, ptr->LLM_SCAL);
	_stdf_write_dtc_I1(file, ptr->HLM_SCAL);
	_stdf_write_dtc_R4(file, ptr->LO_LIMIT);
	_stdf_write_dtc_R4(file, ptr->HI_LIMIT);
	_stdf_write_dtc_Cn(file, ptr->UNITS);
	_stdf_write_dtc_Cn(file, ptr->C_RESFMT);
	_stdf_write_dtc_Cn(file, ptr->C_LLMFMT);
	_stdf_write_dtc_Cn(file, ptr->C_HLMFMT);
	_stdf_write_dtc_R4(file, ptr->LO_SPEC);
	_stdf_write_dtc_R4(file, ptr->HI_SPEC);
#ifdef STDF_VER3
	} else {
	_stdf_write_dtc_B1(file, ptr->OPT_FLAG);
	_stdf_write_dtc_I1(file, ptr->RES_SCAL);
	_stdf_write_dtc_U1(file, ptr->RES_LDIG);
	_stdf_write_dtc_U1(file, ptr->RES_RDIG);
	_stdf_write_dtc_B1(file, ptr->DESC_FLG);
	_stdf_write_dtc_Cx(file, ptr->UNITS, 7);
	_stdf_write_dtc_I1(file, ptr->LLM_SCAL);
	_stdf_write_dtc_I1(file, ptr->HLM_SCAL);
	_stdf_write_dtc_U1(file, ptr->LLM_LDIG);
	_stdf_write_dtc_U1(file, ptr->LLM_RDIG);
	_stdf_write_dtc_U1(file, ptr->HLM_LDIG);
	_stdf_write_dtc_U1(file, ptr->HLM_RDIG);
	_stdf_write_dtc_R4(file, ptr->LO_LIMIT);
	_stdf_write_dtc_R4(file, ptr->HI_LIMIT);
	_stdf_write_dtc_Cn(file, ptr->TEST_NAM);
	_stdf_write_dtc_Cn(file, ptr->SEQ_NAME);
	_stdf_write_dtc_Cn(file, ptr->TEST_TXT);
	}
#endif
	return _stdf_write_flush(file, ptr->header.REC_LEN);
}

ssize_t stdf_write_rec_mpr(stdf_file *file, stdf_rec_mpr *mpr)
{
	if (!mpr->header.REC_LEN)
		SET_HEADER(mpr->header, STDF_REC_MPR, _calc_rec_len_mpr(file, mpr));
	_stdf_check_write_buffer(file, mpr->header.REC_LEN);
	_stdf_write_dtc_header(file, &(mpr->header));
	_stdf_write_dtc_U4(file, mpr->TEST_NUM);
	_stdf_write_dtc_U1(file, mpr->HEAD_NUM);
	_stdf_write_dtc_U1(file, mpr->SITE_NUM);
	_stdf_write_dtc_B1(file, mpr->TEST_FLG);
	_stdf_write_dtc_B1(file, mpr->PARM_FLG);
	_stdf_write_dtc_U2(file, mpr->RTN_ICNT);
	_stdf_write_dtc_U2(file, mpr->RSLT_CNT);
	_stdf_write_dtc_xN1(file, mpr->RTN_STAT, mpr->RTN_ICNT);
	_stdf_write_dtc_xR4(file, mpr->RTN_RSLT, mpr->RSLT_CNT);
	_stdf_write_dtc_Cn(file, mpr->TEST_TXT);
	_stdf_write_dtc_Cn(file, mpr->ALARM_ID);
	_stdf_write_dtc_B1(file, mpr->OPT_FLAG);
	_stdf_write_dtc_I1(file, mpr->RES_SCAL);
	_stdf_write_dtc_I1(file, mpr->LLM_SCAL);
	_stdf_write_dtc_I1(file, mpr->HLM_SCAL);
	_stdf_write_dtc_R4(file, mpr->LO_LIMIT);
	_stdf_write_dtc_R4(file, mpr->HI_LIMIT);
	_stdf_write_dtc_R4(file, mpr->START_IN);
	_stdf_write_dtc_R4(file, mpr->INCR_IN);
	_stdf_write_dtc_xU2(file, mpr->RTN_INDX, mpr->RTN_ICNT);
	_stdf_write_dtc_Cn(file, mpr->UNITS);
	_stdf_write_dtc_Cn(file, mpr->UNITS_IN);
	_stdf_write_dtc_Cn(file, mpr->C_RESFMT);
	_stdf_write_dtc_Cn(file, mpr->C_LLMFMT);
	_stdf_write_dtc_Cn(file, mpr->C_HLMFMT);
	_stdf_write_dtc_R4(file, mpr->LO_SPEC);
	_stdf_write_dtc_R4(file, mpr->HI_SPEC);
	return _stdf_write_flush(file, mpr->header.REC_LEN);
}

ssize_t stdf_write_rec_ftr(stdf_file *file, stdf_rec_ftr *ftr)
{
	if (!ftr->header.REC_LEN)
		SET_HEADER(ftr->header, STDF_REC_FTR, _calc_rec_len_ftr(file, ftr));
	_stdf_check_write_buffer(file, ftr->header.REC_LEN);
	_stdf_write_dtc_header(file, &(ftr->header));
	_stdf_write_dtc_U4(file, ftr->TEST_NUM);
	_stdf_write_dtc_U1(file, ftr->HEAD_NUM);
	_stdf_write_dtc_U1(file, ftr->SITE_NUM);
	_stdf_write_dtc_B1(file, ftr->TEST_FLG);
#ifdef STDF_VER3
	if (file->ver == 4) {
#endif
	_stdf_write_dtc_B1(file, ftr->OPT_FLAG);
	_stdf_write_dtc_U4(file, ftr->CYCL_CNT);
	_stdf_write_dtc_U4(file, ftr->REL_VADR);
	_stdf_write_dtc_U4(file, ftr->REPT_CNT);
	_stdf_write_dtc_U4(file, ftr->NUM_FAIL);
	_stdf_write_dtc_I4(file, ftr->XFAIL_AD);
	_stdf_write_dtc_I4(file, ftr->YFAIL_AD);
	_stdf_write_dtc_I2(file, ftr->VECT_OFF);
	_stdf_write_dtc_U2(file, ftr->RTN_ICNT);
	_stdf_write_dtc_U2(file, ftr->PGM_ICNT);
	_stdf_write_dtc_xU2(file, ftr->RTN_INDX, ftr->RTN_ICNT);
	_stdf_write_dtc_xN1(file, ftr->RTN_STAT, ftr->RTN_ICNT);
	_stdf_write_dtc_xU2(file, ftr->PGM_INDX, ftr->PGM_ICNT);
	_stdf_write_dtc_xN1(file, ftr->PGM_STAT, ftr->PGM_ICNT);
	_stdf_write_dtc_Dn(file, ftr->FAIL_PIN);
	_stdf_write_dtc_Cn(file, ftr->VECT_NAM);
	_stdf_write_dtc_Cn(file, ftr->TIME_SET);
	_stdf_write_dtc_Cn(file, ftr->OP_CODE);
	_stdf_write_dtc_Cn(file, ftr->TEST_TXT);
	_stdf_write_dtc_Cn(file, ftr->ALARM_ID);
	_stdf_write_dtc_Cn(file, ftr->PROG_TXT);
	_stdf_write_dtc_Cn(file, ftr->RSLT_TXT);
	_stdf_write_dtc_U1(file, ftr->PATG_NUM);
	_stdf_write_dtc_Dn(file, ftr->SPIN_MAP);
#ifdef STDF_VER3
	} else {
	_stdf_write_dtc_B1(file, ftr->DESC_FLG);
	_stdf_write_dtc_B1(file, ftr->OPT_FLAG);
	/* _stdf_write_dtc_U1(file, ftr->TIME_SET); */
	_stdf_write_dtc_U4(file, ftr->VECT_ADR);
	_stdf_write_dtc_U4(file, ftr->CYCL_CNT);
	_stdf_write_dtc_U2(file, ftr->REPT_CNT);
	_stdf_write_dtc_U2(file, ftr->PCP_ADDR);
	_stdf_write_dtc_U4(file, ftr->NUM_FAIL);
	_stdf_write_dtc_Bn(file, ftr->FAIL_PIN);
	_stdf_write_dtc_Bn(file, ftr->VECT_DAT);
	_stdf_write_dtc_Bn(file, ftr->DEV_DAT);
	_stdf_write_dtc_Bn(file, ftr->RPIN_MAP);
	_stdf_write_dtc_Cn(file, ftr->TEST_NAM);
	_stdf_write_dtc_Cn(file, ftr->SEQ_NAME);
	_stdf_write_dtc_Cn(file, ftr->TEST_TXT);
	}
#endif
	return _stdf_write_flush(file, ftr->header.REC_LEN);
}

ssize_t stdf_write_rec_str(stdf_file *file, stdf_rec_str *str)
{
	if (!str->header.REC_LEN)
		SET_HEADER(str->header, STDF_REC_STR, _calc_rec_len_str(file, str));
	_stdf_check_write_buffer(file, str->header.REC_LEN);
	_stdf_write_dtc_header(file, &(str->header));
	_stdf_write_dtc_B1(file, str->CONT_FLG);
	_stdf_write_dtc_U4(file, str->TEST_NUM);
	_stdf_write_dtc_U1(file, str->HEAD_NUM);
	_stdf_write_dtc_U1(file, str->SITE_NUM);
	_stdf_write_dtc_U2(file, str->PSR_REF);
	_stdf_write_dtc_B1(file, str->TEST_FLG);
	_stdf_write_dtc_Cn(file, str->LOG_TYP);
	_stdf_write_dtc_Cn(file, str->TEST_TXT);
	_stdf_write_dtc_Cn(file, str->ALARM_ID);
	_stdf_write_dtc_Cn(file, str->PROG_TXT);
	_stdf_write_dtc_Cn(file, str->RSLT_TXT);
	_stdf_write_dtc_U1(file, str->Z_VAL);
	_stdf_write_dtc_B1(file, str->FMU_FLG);
	if (str->FMU_FLG & 1) 
		_stdf_write_dtc_Dn(file, str->MASK_MAP);
	if ((str->FMU_FLG & 4) == 4) 
		_stdf_write_dtc_Dn(file, str->FAL_MAP);
	_stdf_write_dtc_U8(file, str->CYCL_CNT);
	_stdf_write_dtc_U4(file, str->TOTF_CNT);
	_stdf_write_dtc_U4(file, str->TOTL_CNT);
	_stdf_write_dtc_U8(file, str->CYC_BASE);
	_stdf_write_dtc_U4(file, str->BIT_BASE);
	_stdf_write_dtc_U2(file, str->COND_CNT);
	_stdf_write_dtc_U2(file, str->LIM_CNT);
	_stdf_write_dtc_U1(file, str->CYCL_SIZE);
	_stdf_write_dtc_U1(file, str->PMR_SIZE);
	_stdf_write_dtc_U1(file, str->CHN_SIZE);
	_stdf_write_dtc_U1(file, str->PAT_SIZE);
	_stdf_write_dtc_U1(file, str->BIT_SIZE);
	_stdf_write_dtc_U1(file, str->U1_SIZE);
	_stdf_write_dtc_U1(file, str->U2_SIZE);
	_stdf_write_dtc_U1(file, str->U3_SIZE);
	_stdf_write_dtc_U1(file, str->UTX_SIZE);
	_stdf_write_dtc_U2(file, str->CAP_BGN);
	_stdf_write_dtc_xU2(file,str->LIM_INDX, str->LIM_CNT);
	_stdf_write_dtc_xU4(file,str->LIM_SPEC, str->LIM_CNT);
	_stdf_write_dtc_xCn(file,str->COND_LST, str->COND_CNT);
	_stdf_write_dtc_U2(file, str->CYC_CNT);
	_stdf_write_dtc_xUf(file,str->CYC_OFST, str->CYC_CNT, str->CYCL_SIZE);
	_stdf_write_dtc_U2(file, str->PMR_CNT);
	_stdf_write_dtc_xUf(file,str->PMR_INDX, str->PMR_CNT, str->PMR_SIZE);
	_stdf_write_dtc_U2(file, str->CHN_CNT);
	_stdf_write_dtc_xUf(file,str->CHN_NUM, str->CHN_CNT, str->CHN_SIZE);
	_stdf_write_dtc_U2(file, str->EXP_CNT);
	_stdf_write_dtc_xU1(file,str->EXP_DATA, str->EXP_CNT);
	_stdf_write_dtc_U2(file, str->CAP_CNT);
	_stdf_write_dtc_xU1(file,str->CAP_DATA, str->CAP_CNT);
	_stdf_write_dtc_U2(file, str->NEW_CNT);
	_stdf_write_dtc_xU1(file,str->NEW_DATA, str->NEW_CNT);
	_stdf_write_dtc_U2(file, str->PAT_CNT);
	_stdf_write_dtc_xUf(file,str->PAT_NUM, str->PAT_CNT, str->PAT_SIZE);
	_stdf_write_dtc_U2(file, str->BPOS_CNT);
	_stdf_write_dtc_xUf(file,str->BIT_POS, str->BPOS_CNT, str->BIT_SIZE);
	_stdf_write_dtc_U2(file, str->USR1_CNT);
	_stdf_write_dtc_xUf(file,str->USR1, str->USR1_CNT, str->U1_SIZE);
	_stdf_write_dtc_U2(file, str->USR2_CNT);
	_stdf_write_dtc_xUf(file,str->USR2, str->USR2_CNT, str->U2_SIZE);
	_stdf_write_dtc_U2(file, str->USR3_CNT);
	_stdf_write_dtc_xUf(file,str->USR3, str->USR3_CNT, str->U3_SIZE);
	_stdf_write_dtc_U2(file, str->TXT_CNT);
	_stdf_write_dtc_xCn(file,str->USER_TXT, str->TXT_CNT);
	return _stdf_write_flush(file, str->header.REC_LEN);
}

ssize_t stdf_write_rec_bps(stdf_file *file, stdf_rec_bps *bps)
{
	if (!bps->header.REC_LEN)
		SET_HEADER(bps->header, STDF_REC_BPS, _calc_rec_len_bps(file, bps));
	_stdf_check_write_buffer(file, bps->header.REC_LEN);
	_stdf_write_dtc_header(file, &(bps->header));
	_stdf_write_dtc_Cn(file, bps->SEQ_NAME);
	return _stdf_write_flush(file, bps->header.REC_LEN);
}

ssize_t stdf_write_rec_eps(stdf_file *file, stdf_rec_eps *eps)
{
	if (!eps->header.REC_LEN)
		SET_HEADER(eps->header, STDF_REC_EPS, _calc_rec_len_eps(file, eps));
	_stdf_check_write_buffer(file, eps->header.REC_LEN);
	_stdf_write_dtc_header(file, &(eps->header));
	return _stdf_write_flush(file, eps->header.REC_LEN);
}

#ifdef STDF_VER3

ssize_t stdf_write_rec_shb(stdf_file *file, stdf_rec_shb *shb)
{
	if (!shb->header.REC_LEN)
		SET_HEADER(shb->header, STDF_REC_SHB, _calc_rec_len_shb(file, shb));
	_stdf_check_write_buffer(file, shb->header.REC_LEN);
	_stdf_write_dtc_header(file, &(shb->header));
	_stdf_write_dtc_U1(file, shb->HEAD_NUM);
	_stdf_write_dtc_U1(file, shb->SITE_NUM);
	_stdf_write_dtc_U2(file, shb->HBIN_NUM);
	_stdf_write_dtc_U4(file, shb->HBIN_CNT);
	_stdf_write_dtc_Cn(file, shb->HBIN_NAM);
	return _stdf_write_flush(file, shb->header.REC_LEN);
}

ssize_t stdf_write_rec_ssb(stdf_file *file, stdf_rec_ssb *ssb)
{
	if (!ssb->header.REC_LEN)
		SET_HEADER(ssb->header, STDF_REC_SSB, _calc_rec_len_ssb(file, ssb));
	_stdf_check_write_buffer(file, ssb->header.REC_LEN);
	_stdf_write_dtc_header(file, &(ssb->header));
	_stdf_write_dtc_U1(file, ssb->HEAD_NUM);
	_stdf_write_dtc_U1(file, ssb->SITE_NUM);
	_stdf_write_dtc_U2(file, ssb->SBIN_NUM);
	_stdf_write_dtc_U4(file, ssb->SBIN_CNT);
	_stdf_write_dtc_Cn(file, ssb->SBIN_NAM);
	return _stdf_write_flush(file, ssb->header.REC_LEN);
}

ssize_t stdf_write_rec_sts(stdf_file *file, stdf_rec_sts *sts)
{
	if (!sts->header.REC_LEN)
		SET_HEADER(sts->header, STDF_REC_STS, _calc_rec_len_sts(file, sts));
	_stdf_check_write_buffer(file, sts->header.REC_LEN);
	_stdf_write_dtc_header(file, &(sts->header));
	_stdf_write_dtc_U1(file, sts->HEAD_NUM);
	_stdf_write_dtc_U1(file, sts->SITE_NUM);
	_stdf_write_dtc_U4(file, sts->TEST_NUM);
	_stdf_write_dtc_I4(file, sts->EXEC_CNT);
	_stdf_write_dtc_I4(file, sts->FAIL_CNT);
	_stdf_write_dtc_I4(file, sts->ALRM_CNT);
	_stdf_write_dtc_B1(file, sts->OPT_FLAG);
	_stdf_write_dtc_B1(file, sts->PAD_BYTE);
	_stdf_write_dtc_R4(file, sts->TEST_MIN);
	_stdf_write_dtc_R4(file, sts->TEST_MAX);
	_stdf_write_dtc_R4(file, sts->TST_MEAN);
	_stdf_write_dtc_R4(file, sts->TST_SDEV);
	_stdf_write_dtc_R4(file, sts->TST_SUMS);
	_stdf_write_dtc_R4(file, sts->TST_SQRS);
	_stdf_write_dtc_Cn(file, sts->TEST_NAM);
	_stdf_write_dtc_Cn(file, sts->SEQ_NAME);
	_stdf_write_dtc_Cn(file, sts->TEST_LBL);
	return _stdf_write_flush(file, sts->header.REC_LEN);
}

ssize_t stdf_write_rec_scr(stdf_file *file, stdf_rec_scr *scr)
{
	if (!scr->header.REC_LEN)
		SET_HEADER(scr->header, STDF_REC_SCR, _calc_rec_len_scr(file, scr));
	_stdf_check_write_buffer(file, scr->header.REC_LEN);
	_stdf_write_dtc_header(file, &(scr->header));
	_stdf_write_dtc_U1(file, scr->HEAD_NUM);
	_stdf_write_dtc_U1(file, scr->SITE_NUM);
	_stdf_write_dtc_U4(file, scr->FINISH_T);
	_stdf_write_dtc_U4(file, scr->PART_CNT);
	_stdf_write_dtc_I4(file, scr->RTST_CNT);
	_stdf_write_dtc_I4(file, scr->ABRT_CNT);
	_stdf_write_dtc_I4(file, scr->GOOD_CNT);
	_stdf_write_dtc_I4(file, scr->FUNC_CNT);
	return _stdf_write_flush(file, scr->header.REC_LEN);
}

#endif

ssize_t stdf_write_rec_gdr(stdf_file *file, stdf_rec_gdr *gdr)
{
	if (!gdr->header.REC_LEN)
		SET_HEADER(gdr->header, STDF_REC_GDR, _calc_rec_len_gdr(file, gdr));
	_stdf_check_write_buffer(file, gdr->header.REC_LEN);
	_stdf_write_dtc_header(file, &(gdr->header));
	_stdf_write_dtc_U2(file, gdr->FLD_CNT);
	_stdf_write_dtc_Vn(file, gdr->GEN_DATA, gdr->FLD_CNT);
	return _stdf_write_flush(file, gdr->header.REC_LEN);
}

ssize_t stdf_write_rec_dtr(stdf_file *file, stdf_rec_dtr *dtr)
{
	if (!dtr->header.REC_LEN)
		SET_HEADER(dtr->header, STDF_REC_DTR, _calc_rec_len_dtr(file, dtr));
	_stdf_check_write_buffer(file, dtr->header.REC_LEN);
	_stdf_write_dtc_header(file, &(dtr->header));
	_stdf_write_dtc_Cn(file, dtr->TEXT_DAT);
	return _stdf_write_flush(file, dtr->header.REC_LEN);
}
