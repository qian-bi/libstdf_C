/* libstdf.c
 * Copyright (C) 2004 Mike Frysinger <vapier@gmail.com>
 * Released under the BSD license.  For more information,
 * please see: http://opensource.org/licenses/bsd-license.php
 */

#include <libstdf.h>

/* for open(2) */
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
/* for printf(3) */
#include <stdio.h>
/* for va args stuff */
#include <stdarg.h>
/* for close(2) / read(2) */
#include <unistd.h>
/* for malloc(3) */
#include <stdlib.h>
/* for memcpy(3) */
#include <string.h>
/* for BYTE_ORDER defines */
#include <endian.h>
/* for bswap() functions */
#include <byteswap.h>

void __stdf_init(stdf_file *f, char src_cpu)
{
	switch (src_cpu) {
		case CPU_TYPE_DEC:		f->byte_order = PDP_ENDIAN;
			fprintf(stderr, "byte_order: CPU_TYPE_DEC (PDP_ENDIAN) has no implementation\n");
			break;
		case CPU_TYPE_SPARC:	f->byte_order = BIG_ENDIAN; break;
		case CPU_TYPE_X86:		f->byte_order = LITTLE_ENDIAN; break;
		default:				f->byte_order = __STDF_HOST_BYTE_ORDER; break;
	}
}

stdf_file* stdf_open_ex(char *pathname, int flags)
{
	stdf_file *ret = (stdf_file*)malloc(sizeof(stdf_file));
	ret->fd = open(pathname, flags);
	if (ret->fd == -1) {
		free(ret);
		ret = NULL;
	} else {
		char temp[6];
		if (read(ret->fd, temp, 6) != 6) {
			close(ret->fd);
			free(ret);
			ret = NULL;
		} else {
			__stdf_init(ret, temp[4]);
			lseek(ret->fd, 0, SEEK_SET);
		}
	}
	return ret;
}
stdf_file* stdf_open(char *pathname)
{
	return stdf_open_ex(pathname, O_RDONLY);
}

int stdf_close(stdf_file *file)
{
	int ret = close(file->fd);
	free(file);
	return ret;
}

char* stdf_get_rec_name(int type, int subtype)
{
	static char name[4];
	switch (MAKE_REC(type, subtype)) {
		case REC_FAR: memcpy(name, "FAR", 3); break;
		case REC_ATR: memcpy(name, "ATR", 3); break;
		case REC_MIR: memcpy(name, "MIR", 3); break;
		case REC_MRR: memcpy(name, "MRR", 3); break;
		case REC_PCR: memcpy(name, "PCR", 3); break;
		case REC_HBR: memcpy(name, "HBR", 3); break;
		case REC_SBR: memcpy(name, "SBR", 3); break;
		case REC_PMR: memcpy(name, "PMR", 3); break;
		case REC_PGR: memcpy(name, "PGR", 3); break;
		case REC_PLR: memcpy(name, "PLR", 3); break;
		case REC_RDR: memcpy(name, "RDR", 3); break;
		case REC_SDR: memcpy(name, "SDR", 3); break;
		case REC_WIR: memcpy(name, "WIR", 3); break;
		case REC_WRR: memcpy(name, "WRR", 3); break;
		case REC_WCR: memcpy(name, "WCR", 3); break;
		case REC_PIR: memcpy(name, "PIR", 3); break;
		case REC_PRR: memcpy(name, "PRR", 3); break;
		case REC_TSR: memcpy(name, "TSR", 3); break;
		case REC_PTR: memcpy(name, "PTR", 3); break;
		case REC_MPR: memcpy(name, "MPR", 3); break;
		case REC_FTR: memcpy(name, "FTR", 3); break;
		case REC_BPS: memcpy(name, "BPS", 3); break;
		case REC_EPS: memcpy(name, "EPS", 3); break;
		case REC_GDR: memcpy(name, "GDR", 3); break;
		case REC_DTR: memcpy(name, "DTR", 3); break;
		default:      memcpy(name, "???", 3); break;
	}
	name[3] = '\0';
	return name;
}

void __byte_order_change(int in_byte_order, int out_byte_order, byte_t *in, int len)
{
	if (in_byte_order == out_byte_order || len == 1)
		return;
	if (in_byte_order == PDP_ENDIAN) {
		fprintf(stderr, "__byte_order_change(): PDP_ENDIAN is not implemented\n");
		return;
	}

	switch (len) {
		case 2:	*((__uint16_t*)in) = bswap_16(*((__uint16_t*)in)); break;
		case 4:	*((__uint32_t*)in) = bswap_32(*((__uint32_t*)in)); break;
		case 8:	*((__uint64_t*)in) = bswap_64(*((__uint64_t*)in)); break;
		default:
			fprintf(stderr, "__byte_order_change(): byte len of %i has no implementation\n", len);
	}
}
#define __byte_order_to_host(b,p,l) __byte_order_change(b, __STDF_HOST_BYTE_ORDER, p, l)
#define __byte_order_to_src(b,p,l) __byte_order_change(__STDF_HOST_BYTE_ORDER, b, p, l)
#define _stdf_byte_order_to_host(f,v,l) __byte_order_to_host(f->byte_order, (byte_t*)(v), l);

#define	_stdf_read_dtc(f, var, dtc, def) \
	if (f->pos < f->rec_end) \
		f->pos += read(f->fd, var, sizeof(dtc)); \
	else \
		*var = def
#define _stdf_read_dtc_num(f, var, dtc) \
	_stdf_read_dtc(f, var, dtc, 0); \
	_stdf_byte_order_to_host(f, var, sizeof(dtc))
#define	_stdf_read_dtc_U1(f, var) _stdf_read_dtc_num(f, var, dtc_U1)
#define	_stdf_read_dtc_U2(f, var) _stdf_read_dtc_num(f, var, dtc_U2)
#define	_stdf_read_dtc_U4(f, var) _stdf_read_dtc_num(f, var, dtc_U4)
#define	_stdf_read_dtc_I1(f, var) _stdf_read_dtc_num(f, var, dtc_I1)
#define	_stdf_read_dtc_I2(f, var) _stdf_read_dtc_num(f, var, dtc_I2)
#define	_stdf_read_dtc_R4(f, var) _stdf_read_dtc_num(f, var, dtc_R4)
#define	_stdf_read_dtc_C1(f, var) _stdf_read_dtc(f, var, dtc_C1, ' ')
#define	_stdf_read_dtc_B1(f, var) _stdf_read_dtc(f, var, dtc_B1, 0x00)
void _stdf_read_dtc_Cn(stdf_file *f, dtc_Cn *Cn)
{
	unsigned char len;
	if (f->pos == f->rec_end) {
		(*Cn) = (dtc_Cn)malloc(sizeof(char) * 1);
		(*Cn)[0] = 0;
		return;
	}
	f->pos += read(f->fd, &len, 1);
	(*Cn) = (dtc_Cn)malloc(sizeof(char) * len + 2);
	(*Cn)[0] = len;
	f->pos += read(f->fd, (*Cn)+1, len);
	(*Cn)[len+1] = '\0';
}
#define	_stdf_read_dtc_Bn(f, Bn) _stdf_read_dtc_Cn(f, (dtc_Cn*)Bn)

#define __malloc_rec(r) ((r*)malloc(sizeof(r)))
rec_unknown* stdf_read_rec_unknown(stdf_file *file, rec_header *h)
{
	rec_unknown *rec = __malloc_rec(rec_unknown);
#if 0
	rec->data = (void*)malloc(h->REC_LEN);
	read(file->fd, rec->data, h->REC_LEN);
#else
	rec->data = NULL;
	lseek(file->fd, h->REC_LEN, SEEK_CUR);
#endif
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
	return mir;
}
rec_mrr* stdf_read_rec_mrr(stdf_file *file)
{
	rec_mrr *mrr = __malloc_rec(rec_mrr);
	_stdf_read_dtc_U4(file, &(mrr->FINISH_T));
	_stdf_read_dtc_C1(file, &(mrr->DISP_COD));
	_stdf_read_dtc_Cn(file, &(mrr->USR_DESC));
	_stdf_read_dtc_Cn(file, &(mrr->EXC_DESC));
	return mrr;
}
rec_pcr *stdf_read_rec_pcr(stdf_file *file)
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
rec_hbr *stdf_read_rec_hbr(stdf_file *file)
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
rec_sbr *stdf_read_rec_sbr(stdf_file *file)
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
rec_pmr *stdf_read_rec_pmr(stdf_file *file)
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
/*
		case REC_PGR:
		case REC_PLR:
		case REC_RDR:
		case REC_SDR:
*/
rec_wir *stdf_read_rec_wir(stdf_file *file)
{
	rec_wir *wir = __malloc_rec(rec_wir);
	_stdf_read_dtc_U1(file, &(wir->HEAD_NUM));
	_stdf_read_dtc_U1(file, &(wir->SITE_GRP));
	_stdf_read_dtc_U4(file, &(wir->START_T));
	_stdf_read_dtc_Cn(file, &(wir->WAFER_ID));
	return wir;
}
rec_wrr *stdf_read_rec_wrr(stdf_file *file)
{
	rec_wrr *wrr = __malloc_rec(rec_wrr);
	_stdf_read_dtc_U1(file, &(wrr->HEAD_NUM));
	_stdf_read_dtc_U1(file, &(wrr->SITE_GRP));
	_stdf_read_dtc_U4(file, &(wrr->FINISH_T));
	_stdf_read_dtc_U4(file, &(wrr->PART_CNT));
	_stdf_read_dtc_U4(file, &(wrr->RTST_CNT));
	_stdf_read_dtc_U4(file, &(wrr->ABRT_CNT));
	_stdf_read_dtc_U4(file, &(wrr->GOOD_CNT));
	_stdf_read_dtc_U4(file, &(wrr->FUNC_CNT));
	_stdf_read_dtc_Cn(file, &(wrr->WAFER_ID));
	_stdf_read_dtc_Cn(file, &(wrr->FABWF_ID));
	_stdf_read_dtc_Cn(file, &(wrr->FRAME_ID));
	_stdf_read_dtc_Cn(file, &(wrr->MASK_ID));
	_stdf_read_dtc_Cn(file, &(wrr->USR_DESC));
	_stdf_read_dtc_Cn(file, &(wrr->EXC_DESC));
	return wrr;
}
rec_wcr *stdf_read_rec_wcr(stdf_file *file)
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
rec_pir *stdf_read_rec_pir(stdf_file *file)
{
	rec_pir *pir = __malloc_rec(rec_pir);
	_stdf_read_dtc_U1(file, &(pir->HEAD_NUM));
	_stdf_read_dtc_U1(file, &(pir->SITE_NUM));
	return pir;
}
rec_prr *stdf_read_rec_prr(stdf_file *file)
{
	rec_prr *prr = __malloc_rec(rec_prr);
	_stdf_read_dtc_U1(file, &(prr->HEAD_NUM));
	_stdf_read_dtc_U1(file, &(prr->SITE_NUM));
	_stdf_read_dtc_B1(file, &(prr->PART_FLG));
	_stdf_read_dtc_U2(file, &(prr->NUM_TEST));
	_stdf_read_dtc_U2(file, &(prr->HARD_BIN));
	_stdf_read_dtc_U2(file, &(prr->SOFT_BIN));
	_stdf_read_dtc_I2(file, &(prr->X_COORD));
	_stdf_read_dtc_I2(file, &(prr->Y_COORD));
	_stdf_read_dtc_U4(file, &(prr->TEST_T));
	_stdf_read_dtc_Cn(file, &(prr->PART_ID));
	_stdf_read_dtc_Cn(file, &(prr->PART_TXT));
	_stdf_read_dtc_Bn(file, &(prr->PART_FIX));
	return prr;
}
rec_tsr *stdf_read_rec_tsr(stdf_file *file)
{
	rec_tsr *tsr = __malloc_rec(rec_tsr);
	_stdf_read_dtc_U1(file, &(tsr->HEAD_NUM));
	_stdf_read_dtc_U1(file, &(tsr->SITE_NUM));
	_stdf_read_dtc_C1(file, &(tsr->TEST_TYP));
	_stdf_read_dtc_U4(file, &(tsr->TEST_NUM));
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
	return tsr;
}
rec_ptr *stdf_read_rec_ptr(stdf_file *file)
{
	rec_ptr *ptr = __malloc_rec(rec_ptr);
	_stdf_read_dtc_U4(file, &(ptr->TEST_NUM));
	_stdf_read_dtc_U1(file, &(ptr->HEAD_NUM));
	_stdf_read_dtc_U1(file, &(ptr->SITE_NUM));
	_stdf_read_dtc_B1(file, &(ptr->TEST_FLG));
	_stdf_read_dtc_B1(file, &(ptr->PARM_FLG));
	_stdf_read_dtc_R4(file, &(ptr->RESTULT));
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
rec_bps *stdf_read_rec_bps(stdf_file *file)
{
	rec_bps *bps = __malloc_rec(rec_bps);
	_stdf_read_dtc_Cn(file, &(bps->SEQ_NAME));
	return bps;
}
rec_eps *stdf_read_rec_eps(stdf_file *file)
{
	rec_eps *eps = __malloc_rec(rec_eps);
	return eps;
}
rec_dtr *stdf_read_rec_dtr(stdf_file *file)
{
	rec_dtr *dtr = __malloc_rec(rec_dtr);
	_stdf_read_dtc_Cn(file, &(dtr->TEXT_DAT));
	return dtr;
}

rec_unknown* stdf_read_record(stdf_file *file)
{
	rec_unknown *rec = NULL;

	if (read(file->fd, &(file->header), 4) != 4)
		return rec;
	_stdf_byte_order_to_host(file, &(file->header.REC_LEN), sizeof(dtc_U2));
	file->pos = lseek(file->fd, 0, SEEK_CUR);
	file->rec_end = file->pos + file->header.REC_LEN;

	switch (HEAD_TO_REC(file->header)) {
		case REC_FAR: rec = (rec_unknown*)stdf_read_rec_far(file); break;
		case REC_ATR: rec = (rec_unknown*)stdf_read_rec_atr(file); break;
		case REC_MIR: rec = (rec_unknown*)stdf_read_rec_mir(file); break;
		case REC_MRR: rec = (rec_unknown*)stdf_read_rec_mrr(file); break;
		case REC_PCR: rec = (rec_unknown*)stdf_read_rec_pcr(file); break;
		case REC_HBR: rec = (rec_unknown*)stdf_read_rec_hbr(file); break;
		case REC_SBR: rec = (rec_unknown*)stdf_read_rec_sbr(file); break;
		case REC_PMR: rec = (rec_unknown*)stdf_read_rec_pmr(file); break;
/* i want to cry so many kx??
		case REC_PGR:
		case REC_PLR:
		case REC_RDR:
		case REC_SDR:
*/
		case REC_WIR: rec = (rec_unknown*)stdf_read_rec_wir(file); break;
		case REC_WRR: rec = (rec_unknown*)stdf_read_rec_wrr(file); break;
		case REC_WCR: rec = (rec_unknown*)stdf_read_rec_wcr(file); break;
		case REC_PIR: rec = (rec_unknown*)stdf_read_rec_pir(file); break;
		case REC_PRR: rec = (rec_unknown*)stdf_read_rec_prr(file); break;
		case REC_TSR: rec = (rec_unknown*)stdf_read_rec_tsr(file); break;
		case REC_PTR: rec = (rec_unknown*)stdf_read_rec_ptr(file); break;
/*
		case REC_MPR:
		case REC_FTR:
*/
		case REC_BPS: rec = (rec_unknown*)stdf_read_rec_bps(file); break;
		case REC_EPS: rec = (rec_unknown*)stdf_read_rec_eps(file); break;
/*
		case REC_GDR:
*/
		case REC_DTR: rec = (rec_unknown*)stdf_read_rec_dtr(file); break;
		default:
			rec = stdf_read_rec_unknown(file, &(file->header));
			file->header.REC_TYP = REC_TYP_UNKNOWN;
			file->header.REC_SUB = REC_SUB_UNKNOWN;
			break;
	}
	memcpy(&(rec->header), &(file->header), sizeof(rec_header));

	return rec;
}

//#define	_free_Cn(Cn) if (Cn) free(Cn)
void stdf_free_record(rec_unknown *rec)
{
	if (!rec)
		return;

	switch (HEAD_TO_REC(rec->header)) {
		case REC_FAR:
			free(rec);
			break;
		case REC_ATR: {
			rec_atr *atr = (rec_atr*)rec;
			free(atr->CMD_LINE);
			free(atr);
			break;
		}
		case REC_MIR: {
			rec_mir *mir = (rec_mir*)rec;
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
		case REC_PCR:
			free(rec);
			break;
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
/* i want to cry so many kx??
		case REC_PGR:
		case REC_PLR:
		case REC_RDR:
		case REC_SDR:
*/
		case REC_WIR: {
			rec_wir *wir = (rec_wir*)rec;
			free(wir->WAFER_ID);
			free(rec);
			break;
		}
		case REC_WRR: {
			rec_wrr *wrr = (rec_wrr*)rec;
			free(wrr->WAFER_ID);
			free(wrr->FABWF_ID);
			free(wrr->FRAME_ID);
			free(wrr->MASK_ID);
			free(wrr->USR_DESC);
			free(wrr->EXC_DESC);
			free(rec);
			break;
		}
		case REC_WCR:
			free(rec);
			break;
		case REC_PIR:
			free(rec);
			break;
		case REC_PRR: {
			rec_prr *prr = (rec_prr*)rec;
			free(prr->PART_ID);
			free(prr->PART_TXT);
			free(prr->PART_FIX);
			free(rec);
			break;
		}
		case REC_TSR: {
			rec_tsr *tsr = (rec_tsr*)rec;
			free(tsr->TEST_NAM);
			free(tsr->SEQ_NAME);
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
/*
		case REC_MPR:
		case REC_FTR:
*/
		case REC_BPS: {
			rec_bps *bps = (rec_bps*)rec;
			free(bps->SEQ_NAME);
			free(rec);
			break;
		}
		case REC_EPS:
			free(rec);
			break;
/*
		case REC_GDR:
*/
		case REC_DTR: {
			rec_dtr *dtr = (rec_dtr*)rec;
			free(dtr->TEXT_DAT);
			free(rec);
			break;
		}
		case REC_UNKNOWN:
			if (rec->data) free(rec->data);
			free(rec);
			break;
		default:
			fprintf(stderr, "Memory leak: unhandled record type (%s) in stdf_free_record()!\n",
			       stdf_get_rec_name(rec->header.REC_TYP, rec->header.REC_SUB));
			break;
	}
}
