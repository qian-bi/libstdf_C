#include <libstdf.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define print_fmt(n,f,v) printf("\t" n ": " f, v)
#define print_int(n,i) print_fmt(n, "%i\n", i)
#define print_str(n,s) print_fmt(n, "%s\n", (*s ? s+1 : NULL))
#define print_chr(n,c) print_fmt(n, "%c\n", c)
#define print_tim(n,d) print_fmt(n, "%s", ctime((time_t*)&d))
#define print_hex(n,h) print_fmt(n, "%X\n", h)
#define print_rel(n,r) print_fmt(n, "%f\n", r)

int main(int argc, char *argv[])
{
	stdf_file *f;
	char *recname;
	rec_unknown *rec;

	if (argc != 2) {
		printf("Need a file to open!\n");
		return -1;
	}

	f = stdf_open(argv[1]);
	if (!f) {
		perror("Could not open file");
		return 1;
	}

	printf("Dumping [%s]\n", argv[1]);
	while ((rec=stdf_read_record(f)) != NULL) {
		recname = stdf_get_rec_name(rec->header.REC_TYP, rec->header.REC_SUB);
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
/*
rec_pgr
rec_plr
rec_rdr
rec_sdr
*/
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
				/*print_hxs("PART_FIX", prr->*/
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
/*
rec_mpr
rec_ftr
*/
			case REC_BPS: {
				rec_bps *bps = (rec_bps*)rec;
				print_str("SEQ_NAME", bps->SEQ_NAME);
				break;
			}
			case REC_EPS: {
				/*rec_eps *eps = (rec_eps*)rec;*/
				break;
			}
/*
rec_gdr
*/
			case REC_DTR: {
				rec_dtr *dtr = (rec_dtr*)rec;
				print_str("TEXT_DAT", dtr->TEXT_DAT);
				break;
			}
		}
		stdf_free_record(rec);
	}
	stdf_close(f);
	return 0;
}
