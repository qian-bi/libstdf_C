/* is_valid_stdf.c
 * Copyright (C) 2004 Mike Frysinger <vapier@gmail.com>
 * Released under the BSD license.  For more information,
 * please see: http://opensource.org/licenses/bsd-license.php
 *
 * $Header$
 */

#include <libstdf.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define	print_msg(m) printf("\t" m "\n");

int main(int argc, char *argv[])
{
	stdf_file *f;
	char *recname;
	rec_unknown *rec;
	rec_header prev_rec;
	int i;

	if (argc <= 1) {
		printf("Need some files to open!\n");
		return EXIT_FAILURE;
	}

for (i=1; i<argc; ++i) {
	printf("Validating %s\n", argv[i]);
	f = stdf_open(argv[i]);
	if (!f) {
		perror("Could not open file");
		return EXIT_FAILURE;
	}

	rec = stdf_read_record(f);
	if (rec == NULL || HEAD_TO_REC(rec->header) != REC_FAR) {
		print_msg("First record is not FAR!");
		stdf_close(f);
		continue;
	}
	memcpy(&prev_rec, rec->header, sizeof(rec_header));
	while ((rec=stdf_read_record(f)) != NULL) {
		recname = stdf_get_rec_name(rec->header.REC_TYP, rec->header.REC_SUB);
		if (HEAD_TO_REC(rec->header) == REC_FAR) {
			
		}
		if (HEAD_TO_REC(rec->header) != REC_UNKNOWN)
			printf("Record %s:\n", recname);
		switch (HEAD_TO_REC(rec->header)) {
			case REC_FAR: {
				rec_far *far = (rec_far*)rec;
				print_int("CPU_TYPE", far->CPU_TYPE);
				print_int("STDF_VER", far->STDF_VER);
				break;
			}
			case REC_ATR: {
				rec_atr *atr = (rec_atr*)rec;
				print_tim("MOD_TIM", atr->MOD_TIM);
				print_str("CMD_LINE", atr->CMD_LINE);
				break;
			}
			case REC_MIR: {
				rec_mir *mir = (rec_mir*)rec;
				print_tim("SETUP_T", mir->SETUP_T);
				print_tim("START_T", mir->START_T);
				print_int("STAT_NUM", mir->STAT_NUM);
				print_chr("MODE_COD", mir->MODE_COD);
				print_chr("RTST_COD", mir->RTST_COD);
				print_chr("PROT_COD", mir->PROT_COD);
				print_int("BURN_TIM", mir->BURN_TIM);
				print_chr("CMOD_COD", mir->CMOD_COD);
				print_str("LOT_ID", mir->LOT_ID);
				print_str("PART_TYP", mir->PART_TYP);
				print_str("NODE_NAM", mir->NODE_NAM);
				print_str("TSTR_TYP", mir->TSTR_TYP);
				print_str("JOB_NAM", mir->JOB_NAM);
				print_str("JOB_REV", mir->JOB_REV);
				print_str("SBLOT_ID", mir->SBLOT_ID);
				print_str("OPER_NAM", mir->OPER_NAM);
				print_str("EXEC_TYP", mir->EXEC_TYP);
				print_str("EXEC_VER", mir->EXEC_VER);
				print_str("TEST_COD", mir->TEST_COD);
				print_str("TST_TEMP", mir->TST_TEMP);
				print_str("USER_TXT", mir->USER_TXT);
				print_str("AUX_FILE", mir->AUX_FILE);
				print_str("PKG_TYP", mir->PKG_TYP);
				print_str("FAMILY_ID", mir->FAMILY_ID);
				print_str("DATE_COD", mir->DATE_COD);
				print_str("FACIL_ID", mir->FACIL_ID);
				print_str("FLOOR_ID", mir->FLOOR_ID);
				print_str("PROC_ID", mir->PROC_ID);
				print_str("OPER_FRQ", mir->OPER_FRQ);
				print_str("SPEC_NAM", mir->SPEC_NAM);
				print_str("SPEC_VER", mir->SPEC_VER);
				print_str("FLOW_ID", mir->FLOW_ID);
				print_str("SETUP_ID", mir->SETUP_ID);
				print_str("DSGN_REV", mir->DSGN_REV);
				print_str("ENG_ID", mir->ENG_ID);
				print_str("ROM_COD", mir->ROM_COD);
				print_str("SERL_NUM", mir->SERL_NUM);
				print_str("SUPR_NAM", mir->SUPR_NAM);
				break;
			}
			case REC_MRR: {
				rec_mrr *mrr = (rec_mrr*)rec;
				print_tim("FINISH_T", mrr->FINISH_T);
				print_chr("DISP_COD", mrr->DISP_COD);
				print_str("USR_DESC", mrr->USR_DESC);
				print_str("EXC_DESC", mrr->EXC_DESC);
				break;
			}
			case REC_PCR: {
				rec_pcr *pcr = (rec_pcr*)rec;
				print_int("HEAD_NUM", pcr->HEAD_NUM);
				print_int("SITE_NUM", pcr->SITE_NUM);
				print_int("PART_CNT", pcr->PART_CNT);
				print_int("RTST_CNT", pcr->RTST_CNT);
				print_int("ABRT_CNT", pcr->ABRT_CNT);
				print_int("GOOD_CNT", pcr->GOOD_CNT);
				print_int("FUNC_CNT", pcr->FUNC_CNT);
				break;
			}
			case REC_HBR: {
				rec_hbr *hbr = (rec_hbr*)rec;
				print_int("HEAD_NUM", hbr->HEAD_NUM);
				print_int("SITE_NUM", hbr->SITE_NUM);
				print_int("HBIN_NUM", hbr->HBIN_NUM);
				print_int("HBIN_CNT", hbr->HBIN_CNT);
				print_chr("HBIN_PF", hbr->HBIN_PF);
				print_str("HBIN_NAM", hbr->HBIN_NAM);
				break;
			}
			case REC_SBR: {
				rec_sbr *sbr = (rec_sbr*)rec;
				print_int("HEAD_NUM", sbr->HEAD_NUM);
				print_int("SITE_NUM", sbr->SITE_NUM);
				print_int("SBIN_NUM", sbr->SBIN_NUM);
				print_int("SBIN_CNT", sbr->SBIN_CNT);
				print_chr("SBIN_PF", sbr->SBIN_PF);
				print_str("SBIN_NAM", sbr->SBIN_NAM);
				break;
			}
			case REC_PMR: {
				rec_pmr *pmr = (rec_pmr*)rec;
				print_int("PMR_INDX", pmr->PMR_INDX);
				print_int("CHAN_TYP", pmr->CHAN_TYP);
				print_str("CHAN_NAM", pmr->CHAN_NAM);
				print_str("PHY_NAM", pmr->PHY_NAM);
				print_str("LOG_NAM", pmr->LOG_NAM);
				print_int("HEAD_NUM", pmr->HEAD_NUM);
				print_int("SITE_NUM", pmr->SITE_NUM);
				break;
			}
			case REC_PGR: {
				rec_pgr *pgr = (rec_pgr*)rec;
				print_int("GRP_INDX", pgr->GRP_INDX);
				print_str("GRP_NAM", pgr->GRP_NAM);
				print_int("INDX_CNT", pgr->INDX_CNT);
				print_UNK("PMR_INDX");
				break;
			}
			case REC_PLR: {
				rec_plr *plr = (rec_plr*)rec;
				print_int("GRP_CNT", plr->GRP_CNT);
				print_UNK("GRP_INDX");
				print_UNK("GRP_MODE");
				print_UNK("GRP_RADX");
				print_UNK("PGM_CHAR");
				print_UNK("RTN_CHAR");
				print_UNK("PGM_CHAL");
				print_UNK("RTN_CHAL");
				break;
			}
			case REC_RDR: {
				rec_rdr *rdr = (rec_rdr*)rec;
				print_int("NUM_BINS", rdr->NUM_BINS);
				print_UNK("RTST_BIN");
				break;
			}
			case REC_SDR: {
				rec_sdr *sdr = (rec_sdr*)rec;
				print_int("HEAD_NUM", sdr->HEAD_NUM);
				print_int("SITE_GRP", sdr->SITE_GRP);
				print_int("SITE_CNT", sdr->SITE_CNT);
				print_UNK("SITE_NUM");
				print_str("HAND_TYP", sdr->HAND_TYP);
				print_str("HAND_ID", sdr->HAND_ID);
				print_str("CARD_TYP", sdr->CARD_TYP);
				print_str("CARD_ID", sdr->CARD_ID);
				print_str("LOAD_TYP", sdr->LOAD_TYP);
				print_str("LOAD_ID", sdr->LOAD_ID);
				print_str("DIB_TYP", sdr->DIB_TYP);
				print_str("DIB_ID", sdr->DIB_ID);
				print_str("CABL_TYP", sdr->CABL_TYP);
				print_str("CABL_ID", sdr->CABL_ID);
				print_str("CONT_TYP", sdr->CONT_TYP);
				print_str("CONT_ID", sdr->CONT_ID);
				print_str("LASR_TYP", sdr->LASR_TYP);
				print_str("LASR_ID", sdr->LASR_ID);
				print_str("EXTR_TYP", sdr->EXTR_TYP);
				print_str("EXTR_ID", sdr->EXTR_ID);
				break;
			}
			case REC_WIR: {
				rec_wir *wir = (rec_wir*)rec;
				print_int("HEAD_NUM", wir->HEAD_NUM);
				print_int("SITE_GRP", wir->SITE_GRP);
				print_tim("START_T", wir->START_T);
				print_str("WAFER_ID", wir->WAFER_ID);
				break;
			}
			case REC_WRR: {
				rec_wrr *wrr = (rec_wrr*)rec;
				print_int("HEAD_NUM", wrr->HEAD_NUM);
				print_int("SITE_GRP", wrr->SITE_GRP);
				print_tim("FINISH_T", wrr->FINISH_T);
				print_int("PART_CNT", wrr->PART_CNT);
				print_int("RTST_CNT", wrr->RTST_CNT);
				print_int("ABRT_CNT", wrr->ABRT_CNT);
				print_int("GOOD_CNT", wrr->GOOD_CNT);
				print_int("FUNC_CNT", wrr->FUNC_CNT);
				print_str("WAFER_ID", wrr->WAFER_ID);
				print_str("FABWF_ID", wrr->FABWF_ID);
				print_str("FRAME_ID", wrr->FRAME_ID);
				print_str("MASK_ID", wrr->MASK_ID);
				print_str("USR_DESC", wrr->USR_DESC);
				print_str("EXC_DESC", wrr->EXC_DESC);
				break;
			}
			case REC_WCR: {
				rec_wcr *wcr = (rec_wcr*)rec;
				print_rel("WAFR_SIZ", wcr->WAFR_SIZ);
				print_rel("DIE_HT", wcr->DIE_HT);
				print_rel("DIE_WID", wcr->DIE_WID);
				print_chr("WF_UNITS", wcr->WF_UNITS);
				print_chr("WF_FLAT", wcr->WF_FLAT);
				print_int("CENTER_X", wcr->CENTER_X);
				print_int("CENTER_Y", wcr->CENTER_Y);
				print_chr("POS_X", wcr->POS_X);
				print_chr("POS_Y", wcr->POS_Y);
				break;
			}
			case REC_PIR: {
				rec_pir *pir = (rec_pir*)rec;
				print_int("HEAD_NUM", pir->HEAD_NUM);
				print_int("SITE_NUM", pir->SITE_NUM);
				break;
			}
			case REC_PRR: {
				rec_prr *prr = (rec_prr*)rec;
				print_int("HEAD_NUM", prr->HEAD_NUM);
				print_int("SITE_NUM", prr->SITE_NUM);
				print_hex("PART_FLG", prr->PART_FLG);
				print_int("NUM_TEST", prr->NUM_TEST);
				print_int("HARD_BIN", prr->HARD_BIN);
				print_int("SOFT_BIN", prr->SOFT_BIN);
				print_int("X_COORD", prr->X_COORD);
				print_int("Y_COORD", prr->Y_COORD);
				print_tim("TEST_T", prr->TEST_T);
				print_str("PART_ID", prr->PART_ID);
				print_str("PART_TXT", prr->PART_TXT);
				print_UNK("PART_FIX");
				break;
			}
			case REC_TSR: {
				rec_tsr *tsr = (rec_tsr*)rec;
				print_int("HEAD_NUM", tsr->HEAD_NUM);
				print_int("SITE_NUM", tsr->SITE_NUM);
				print_chr("TEST_TYP", tsr->TEST_TYP);
				print_int("TEST_NUM", tsr->TEST_NUM);
				print_int("EXEC_CNT", tsr->EXEC_CNT);
				print_int("FAIL_CNT", tsr->FAIL_CNT);
				print_int("ALRM_CNT", tsr->ALRM_CNT);
				print_str("TEST_NAM", tsr->TEST_NAM);
				print_str("SEQ_NAME", tsr->SEQ_NAME);
				print_str("TEST_LBL", tsr->TEST_LBL);
				print_hex("OPT_FLAG", tsr->OPT_FLAG);
				print_rel("TEST_TIM", tsr->TEST_TIM);
				print_rel("TEST_MIN", tsr->TEST_MIN);
				print_rel("TEST_MAX", tsr->TEST_MAX);
				print_rel("TST_SUMS", tsr->TST_SUMS);
				print_rel("TST_SQRS", tsr->TST_SQRS);
				break;
			}
			case REC_PTR: {
				rec_ptr *ptr = (rec_ptr*)rec;
				print_int("TEST_NUM", ptr->TEST_NUM);
				print_int("HEAD_NUM", ptr->HEAD_NUM);
				print_int("SITE_NUM", ptr->SITE_NUM);
				print_hex("TEST_FLG", ptr->TEST_FLG);
				print_hex("PARM_FLG", ptr->PARM_FLG);
				print_rel("RESTULT", ptr->RESTULT);
				print_str("TEST_TXT", ptr->TEST_TXT);
				print_str("ALARM_ID", ptr->ALARM_ID);
				print_hex("OPT_FLAG", ptr->OPT_FLAG);
				print_int("RES_SCAL", ptr->RES_SCAL);
				print_int("LLM_SCAL", ptr->LLM_SCAL);
				print_int("HLM_SCAL", ptr->HLM_SCAL);
				print_rel("LO_LIMIT", ptr->LO_LIMIT);
				print_rel("HI_LIMIT", ptr->HI_LIMIT);
				print_str("UNITS", ptr->UNITS);
				print_str("C_RESFMT", ptr->C_RESFMT);
				print_str("C_LLMFMT", ptr->C_LLMFMT);
				print_str("C_HLMFMT", ptr->C_HLMFMT);
				print_rel("LO_SPEC", ptr->LO_SPEC);
				print_rel("HI_SPEC", ptr->HI_SPEC);
				break;
			}
			case REC_MPR: {
				rec_mpr *mpr = (rec_mpr*)rec;
				print_int("TEST_NUM", mpr->TEST_NUM);
				print_int("HEAD_NUM", mpr->HEAD_NUM);
				print_int("SITE_NUM", mpr->SITE_NUM);
				print_hex("TEST_FLG", mpr->TEST_FLG);
				print_hex("PARM_FLG", mpr->PARM_FLG);
				print_int("RTN_ICNT", mpr->RTN_ICNT);
				print_int("RSLT_CNT", mpr->RSLT_CNT);
				print_UNK("RTN_STAT");
				print_UNK("RTN_RSLT");
				print_str("TEST_TXT", mpr->TEST_TXT);
				print_str("ALARM_ID", mpr->ALARM_ID);
				print_hex("OPT_FLAG", mpr->OPT_FLAG);
				print_int("RES_SCAL", mpr->RES_SCAL);
				print_int("LLM_SCAL", mpr->LLM_SCAL);
				print_int("HLM_SCAL", mpr->HLM_SCAL);
				print_rel("LO_LIMIT", mpr->LO_LIMIT);
				print_rel("HI_LIMIT", mpr->HI_LIMIT);
				print_rel("START_IN", mpr->START_IN);
				print_rel("INCR_IN", mpr->INCR_IN);
				print_UNK("RTN_INDX");
				print_str("UNITS", mpr->UNITS);
				print_str("UNITS_IN", mpr->UNITS_IN);
				print_str("C_RESFMT", mpr->C_RESFMT);
				print_str("C_LLMFMT", mpr->C_LLMFMT);
				print_str("C_HLMFMT", mpr->C_HLMFMT);
				print_rel("LO_SPEC", mpr->LO_SPEC);
				print_rel("HI_SPEC", mpr->HI_SPEC);
				break;
			}
			case REC_FTR: {
				rec_ftr *ftr = (rec_ftr*)rec;
				print_int("TEST_NUM", ftr->TEST_NUM);
				print_int("HEAD_NUM", ftr->HEAD_NUM);
				print_int("SITE_NUM", ftr->SITE_NUM);
				print_hex("TEST_FLG", ftr->TEST_FLG);
				print_hex("PARM_FLG", ftr->PARM_FLG);
				print_int("CYCL_CNT", ftr->CYCL_CNT);
				print_int("REL_VADR", ftr->REL_VADR);
				print_int("REPT_CNT", ftr->REPT_CNT);
				print_int("NUM_FAIL", ftr->NUM_FAIL);
				print_int("XFAIL_AD", ftr->XFAIL_AD);
				print_int("YFAIL_AD", ftr->YFAIL_AD);
				print_int("VECT_OFF", ftr->VECT_OFF);
				print_int("RTN_ICNT", ftr->RTN_ICNT);
				print_int("PGM_ICNT", ftr->PGM_ICNT);
				print_UNK("RTN_INDX");
				print_UNK("RTN_STAT");
				print_UNK("PGM_INDX");
				print_UNK("PGM_STAT");
				print_UNK("FAIL_PIN");
				print_str("VECT_NAM", ftr->VECT_NAM);
				print_str("TIME_SET", ftr->TIME_SET);
				print_str("OP_CODE", ftr->OP_CODE);
				print_str("TEST_TXT", ftr->TEST_TXT);
				print_str("ALARM_ID", ftr->ALARM_ID);
				print_str("PROG_TXT", ftr->PROG_TXT);
				print_str("RSLT_TXT", ftr->RSLT_TXT);
				print_int("PATG_NUM", ftr->PATG_NUM);
				print_UNK("SPIN_MAP");
				break;
			}
			case REC_BPS: {
				rec_bps *bps = (rec_bps*)rec;
				print_str("SEQ_NAME", bps->SEQ_NAME);
				break;
			}
			case REC_EPS: {
				/*rec_eps *eps = (rec_eps*)rec;*/
				break;
			}
			case REC_GDR: {
				/*rec_gdr *gdr = (rec_gdr*)rec;*/
				print_UNK("GEN_DATA");
				break;
			}
			case REC_DTR: {
				rec_dtr *dtr = (rec_dtr*)rec;
				print_str("TEXT_DAT", dtr->TEXT_DAT);
				break;
			}
		}
		stdf_free_record(rec);
	}
	stdf_close(f);
}
	return EXIT_SUCCESS;
}
