/**
 * @file stdf2xtdf.c
 */
/*
 * Copyright (C) 2004-2007 Mike Frysinger <vapier@gmail.com>
 * Copyright (C) 2017 Stefan Brandner <stefan.brandner@gmx.at>
 * Released under the BSD license.  For more information,
 * please see: http://opensource.org/licenses/bsd-license.php
 */

#include <config.h>
#include <libstdf.h>

#include <internal/headers.h>

#define print_fmt(n,f,v) printf("\t<" n " value=" f "/>\n", v)
#define print_int(n,i) print_fmt(n, "%i", i)
#define print_Cn(n,s) \
	do { \
		if (*s) \
			print_fmt(n, "\"%s\"", s+1); \
	} while (0)
#define print_Sn(n,s) \
        do { \
                stdf_dtc_U2 c = ((stdf_dtc_U2*)s)[0]; \
                if (*s && c) \
			print_fmt(n, "\"%s\"", s+2); \
		else \
			print_fmt(n, "%s", "(null)"); \
        } while (0) 
#define print_chr(n,c) print_fmt(n, "'%c'", c)
#define print_tim(n,d) print_int(n,d)
#define print_hex(n,h) print_fmt(n, "%X", h)
#define print_rel(n,r) print_fmt(n, "%f", r)

#define print_x(xFunc, xType, format) \
	void xFunc(char *n, xType u, int c) \
	{ \
		int i; \
		printf("\t<%s value='", n); \
		for (i=0; i<c; ++i) { \
			printf(format, u[i]); \
			if (i+1 < c) printf(","); \
		} \
		printf("'/>\n"); \
	}
print_x(print_xU1, stdf_dtc_xU1, "%i")
print_x(print_xU2, stdf_dtc_xU2, "%i")
print_x(print_xU4, stdf_dtc_xU4, "%i")
print_x(print_xU8, stdf_dtc_xU8, "%i")
print_x(print_xR4, stdf_dtc_xR4, "%f")

#define print_xCn(n,s,c) \
{ \
	int i; \
        printf("\t<%s value=\"", n); \
        if (*s) \
		for (i=0; i<c; ++i) { \
			printf("%s", s[i]+1); \
			if (i+1 < c) printf(","); \
		} \
	printf("\"/>\n"); \
}

void print_xSn(char *member, stdf_dtc_xSn xSn, stdf_dtc_U2 cnt)
{
        stdf_dtc_U2 i,c;
	printf("\t<%s value=\"", member);
        for ( i=0; i < cnt; i++) {
                stdf_dtc_U2 c = ((stdf_dtc_U2*)xSn[i])[0];
                if (*xSn[i] && c) 
			printf("%s", xSn[i]+2);
		else
			printf("(null)");		
		if (i+1 < cnt) printf(",");
	}
	printf("\"/>\n");
}

void print_xUf(char *member, stdf_dtc_xUf xUf, stdf_dtc_U2 cnt, stdf_dtc_U1 size)
{
       stdf_dtc_U2 i;

       switch (size) {
                case 1: print_xU1(member,((stdf_dtc_U1*)xUf),cnt); break;
                case 2: print_xU2(member,((stdf_dtc_U2*)xUf),cnt); break;
                case 4: print_xU4(member,((stdf_dtc_U4*)xUf),cnt); break;
        }

}

void print_xN1(char *member, stdf_dtc_xN1 xN1, stdf_dtc_U2 c)
{
	stdf_dtc_N1 *n = xN1;
	printf("\t<%s value='", member);
	while (c > 0) {
		if (c > 1) {
			printf("0x%X,0x%X", (*n) & 0x0F, ((*n) & 0xF0) >> 4);
			c -= 2;
			if (c) printf(",");
		} else {
			printf("0x%X", (*n) & 0x0F);
			break;
		}
		++n;
	}
	printf("'/>\n");
}

static unsigned char lookup[16] = {
0x0, 0x8, 0x4, 0xc, 0x2, 0xa, 0x6, 0xe,
0x1, 0x9, 0x5, 0xd, 0x3, 0xb, 0x7, 0xf };

uint8_t reverse(uint8_t n) {
   // Reverse the top and bottom nibble then swap them.
   return (lookup[n&0xf] << 4) | lookup[n>>4];
}

void print_Vn(char *n, stdf_dtc_Vn v, int c)
{
	stdf_dtc_U2 i,j;
	--c;
	printf("\t<%s>\n", n);
	for (i=0; i<=c; ++i) {
		printf("\t\t<%s value=", stdf_get_Vn_name(v[i].type));
		switch (v[i].type) {
			case STDF_GDR_B0: printf("''"); break;
			case STDF_GDR_U1: printf("%i", *((stdf_dtc_U1*)v[i].data)); break;
			case STDF_GDR_U2: printf("%i", *((stdf_dtc_U2*)v[i].data)); break;
			case STDF_GDR_U4: printf("%i", *((stdf_dtc_U4*)v[i].data)); break;
			case STDF_GDR_I1: printf("%i", *((stdf_dtc_I1*)v[i].data)); break;
			case STDF_GDR_I2: printf("%i", *((stdf_dtc_I2*)v[i].data)); break;
			case STDF_GDR_I4: printf("%i", *((stdf_dtc_I4*)v[i].data)); break;
			case STDF_GDR_R4: printf("%f", *((stdf_dtc_R4*)v[i].data)); break;
			case STDF_GDR_R8: printf("%f", *((stdf_dtc_R8*)v[i].data)); break;
			case STDF_GDR_Cn: {
				stdf_dtc_Cn Cn = *((stdf_dtc_Cn*)v[i].data);
				printf("\"%s\"", (*Cn ? Cn+1 : "(null"));
				break;
			}
			case STDF_GDR_Bn: {
                                stdf_dtc_Bn Bn = *((stdf_dtc_Bn*)v[i].data);
				if (*Bn) {
					printf("0x", *Bn);
					for ( j=1; j<=*Bn; ++j)
						printf("%02X", reverse(*(Bn+j)));
				}
				else
					printf("''");
				break;
                        }
			case STDF_GDR_Dn: {
                                stdf_dtc_Dn Dn = *((stdf_dtc_Dn*)v[i].data);
				stdf_dtc_U2 *num_bits = (stdf_dtc_U2*)Dn, len;
				len = *num_bits / 8;
        			if (*num_bits % 8) ++len;
				if (len) {
					printf("0x", *num_bits);
					for ( j=0; j<len; ++j)
						printf("%02X", reverse(*(Dn+j+2)));
				}
				else
					printf("''");
				break;
			}
			case STDF_GDR_N1: printf("0x%X", *((stdf_dtc_N1*)v[i].data)); break;
		}
		printf("/>\n");
	}
	printf("\t</%s>\n", n);
}

void print_Bn(char *n, stdf_dtc_Bn b)
{
	stdf_dtc_U1 i;
	if (*b) {
		printf("\t<%s value=0x", n);
		for (i=1; i<=*b; ++i)
			printf("%02X", reverse(*(b+i)));
	}
	else 
		printf("\t<%s value=''", n);
	printf("/>\n");
}

void print_Dn(char *n, stdf_dtc_Dn d)
{
	int i;
	stdf_dtc_U2 *num_bits = (stdf_dtc_U2*)d, len;
	len = *num_bits / 8;
	if (*num_bits % 8) ++len;
	if (len) {
		printf("\t<%s value=0x", n);
		for (i=0; i<len; ++i)
			printf("%02X", reverse(*(d+i+2)));
	}
	else
		printf("\t<%s value=''", n);
	printf("/>\n");
}

#define print_UNK(n) \
	do { \
		fprintf(stderr, "******************************************\n"); \
		fprintf(stderr, "This field (" n ") has not been tested!\n"); \
		fprintf(stderr, "Please consider sending this file to\n"); \
		fprintf(stderr, "vapier@gmail.com to help out the\n"); \
		fprintf(stderr, "FreeSTDF project and make sure this code\n"); \
		fprintf(stderr, "works exactly the way it should!\n"); \
		fprintf(stderr, "******************************************\n"); \
	} while (0)

int main(int argc, char *argv[])
{
	stdf_file *f;
	char *recname;
	stdf_rec_unknown *rec;
	int i;
	stdf_dtc_U4 stdf_ver;

	if (argc <= 1) {
		printf("Need some files to open!\n");
		return EXIT_FAILURE;
	}

printf("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
       "<!DOCTYPE guide SYSTEM \"http://freestdf.sourceforge.net/specs/xtdf.dtd\">\n");

for (i=1; i<argc; ++i) {
	f = stdf_open(argv[i]);
	if (!f) {
		perror("Could not open file");
		continue;
	}
	stdf_get_setting(f, STDF_SETTING_VERSION, &stdf_ver);

	printf("<stdf source=\"%s\">\n", argv[i]);
	while ((rec=stdf_read_record(f)) != NULL) {
		recname = stdf_get_rec_name(rec->header.REC_TYP, rec->header.REC_SUB);
		if (HEAD_TO_REC(rec->header) != STDF_REC_UNKNOWN)
			printf("<%s>\n", recname);
		switch (HEAD_TO_REC(rec->header)) {
			case STDF_REC_FAR: {
				stdf_rec_far *far = (stdf_rec_far*)rec;
				print_int("CPU_TYPE", far->CPU_TYPE);
				print_int("STDF_VER", far->STDF_VER);
				break;
			}
			case STDF_REC_ATR: {
				stdf_rec_atr *atr = (stdf_rec_atr*)rec;
				print_tim("MOD_TIM", atr->MOD_TIM);
				print_Cn("CMD_LINE", atr->CMD_LINE);
				break;
			}
                        case STDF_REC_VUR: {
                                stdf_rec_vur *vur = (stdf_rec_vur*)rec;
                                print_Cn("UPD_NAM", vur->UPD_NAM);
                                break;
                        }
			case STDF_REC_MIR: {
				stdf_rec_mir *mir = (stdf_rec_mir*)rec;
#ifdef STDF_VER3
				if (stdf_ver == 4) {
#endif
				print_tim("SETUP_T", mir->SETUP_T);
				print_tim("START_T", mir->START_T);
				print_int("STAT_NUM", mir->STAT_NUM);
				print_chr("MODE_COD", mir->MODE_COD);
				print_chr("RTST_COD", mir->RTST_COD);
				print_chr("PROT_COD", mir->PROT_COD);
				print_int("BURN_TIM", mir->BURN_TIM);
				print_chr("CMOD_COD", mir->CMOD_COD);
				print_Cn("LOT_ID", mir->LOT_ID);
				print_Cn("PART_TYP", mir->PART_TYP);
				print_Cn("NODE_NAM", mir->NODE_NAM);
				print_Cn("TSTR_TYP", mir->TSTR_TYP);
				print_Cn("JOB_NAM", mir->JOB_NAM);
				print_Cn("JOB_REV", mir->JOB_REV);
				print_Cn("SBLOT_ID", mir->SBLOT_ID);
				print_Cn("OPER_NAM", mir->OPER_NAM);
				print_Cn("EXEC_TYP", mir->EXEC_TYP);
				print_Cn("EXEC_VER", mir->EXEC_VER);
				print_Cn("TEST_COD", mir->TEST_COD);
				print_Cn("TST_TEMP", mir->TST_TEMP);
				print_Cn("USER_TXT", mir->USER_TXT);
				print_Cn("AUX_FILE", mir->AUX_FILE);
				print_Cn("PKG_TYP", mir->PKG_TYP);
				print_Cn("FAMILY_ID", mir->FAMILY_ID);
				print_Cn("DATE_COD", mir->DATE_COD);
				print_Cn("FACIL_ID", mir->FACIL_ID);
				print_Cn("FLOOR_ID", mir->FLOOR_ID);
				print_Cn("PROC_ID", mir->PROC_ID);
				print_Cn("OPER_FRQ", mir->OPER_FRQ);
				print_Cn("SPEC_NAM", mir->SPEC_NAM);
				print_Cn("SPEC_VER", mir->SPEC_VER);
				print_Cn("FLOW_ID", mir->FLOW_ID);
				print_Cn("SETUP_ID", mir->SETUP_ID);
				print_Cn("DSGN_REV", mir->DSGN_REV);
				print_Cn("ENG_ID", mir->ENG_ID);
				print_Cn("ROM_COD", mir->ROM_COD);
				print_Cn("SERL_NUM", mir->SERL_NUM);
				print_Cn("SUPR_NAM", mir->SUPR_NAM);
#ifdef STDF_VER3
				} else {
				print_int("CPU_TYPE", mir->CPU_TYPE);
				print_int("STDF_VER", mir->STDF_VER);
				print_chr("MODE_COD", mir->MODE_COD);
				print_int("STAT_NUM", mir->STAT_NUM);
				print_Cn("TEST_COD", mir->TEST_COD);
				print_chr("RTST_COD", mir->RTST_COD);
				print_chr("PROT_COD", mir->PROT_COD);
				print_chr("CMOD_COD", mir->CMOD_COD);
				print_tim("SETUP_T", mir->SETUP_T);
				print_tim("START_T", mir->START_T);
				print_Cn("LOT_ID", mir->LOT_ID);
				print_Cn("PART_TYP", mir->PART_TYP);
				print_Cn("JOB_NAM", mir->JOB_NAM);
				print_Cn("OPER_NAM", mir->OPER_NAM);
				print_Cn("NODE_NAM", mir->NODE_NAM);
				print_Cn("TSTR_TYP", mir->TSTR_TYP);
				print_Cn("EXEC_TYP", mir->EXEC_TYP);
				print_Cn("SUPR_NAM", mir->SUPR_NAM);
				print_Cn("HAND_ID", mir->HAND_ID);
				print_Cn("SBLOT_ID", mir->SBLOT_ID);
				print_Cn("JOB_REV", mir->JOB_REV);
				print_Cn("PROC_ID", mir->PROC_ID);
				print_Cn("PRB_CARD", mir->PRB_CARD);
				}
#endif
				break;
			}
			case STDF_REC_MRR: {
				stdf_rec_mrr *mrr = (stdf_rec_mrr*)rec;
				print_tim("FINISH_T", mrr->FINISH_T);
#ifdef STDF_VER3
				if (stdf_ver == 3) {
				print_int("PART_CNT", mrr->PART_CNT);
				print_int("RTST_CNT", mrr->RTST_CNT);
				print_int("ABRT_CNT", mrr->ABRT_CNT);
				print_int("GOOD_CNT", mrr->GOOD_CNT);
				print_int("FUNC_CNT", mrr->FUNC_CNT);
				}
#endif
				print_chr("DISP_COD", mrr->DISP_COD);
				print_Cn("USR_DESC", mrr->USR_DESC);
				print_Cn("EXC_DESC", mrr->EXC_DESC);
				break;
			}
			case STDF_REC_PCR: {
				stdf_rec_pcr *pcr = (stdf_rec_pcr*)rec;
				print_int("HEAD_NUM", pcr->HEAD_NUM);
				print_int("SITE_NUM", pcr->SITE_NUM);
				print_int("PART_CNT", pcr->PART_CNT);
				print_int("RTST_CNT", pcr->RTST_CNT);
				print_int("ABRT_CNT", pcr->ABRT_CNT);
				print_int("GOOD_CNT", pcr->GOOD_CNT);
				print_int("FUNC_CNT", pcr->FUNC_CNT);
				break;
			}
			case STDF_REC_HBR: {
				stdf_rec_hbr *hbr = (stdf_rec_hbr*)rec;
				print_int("HEAD_NUM", hbr->HEAD_NUM);
				print_int("SITE_NUM", hbr->SITE_NUM);
				print_int("HBIN_NUM", hbr->HBIN_NUM);
				print_int("HBIN_CNT", hbr->HBIN_CNT);
				print_chr("HBIN_PF", hbr->HBIN_PF);
				print_Cn("HBIN_NAM", hbr->HBIN_NAM);
				break;
			}
			case STDF_REC_SBR: {
				stdf_rec_sbr *sbr = (stdf_rec_sbr*)rec;
				print_int("HEAD_NUM", sbr->HEAD_NUM);
				print_int("SITE_NUM", sbr->SITE_NUM);
				print_int("SBIN_NUM", sbr->SBIN_NUM);
				print_int("SBIN_CNT", sbr->SBIN_CNT);
				print_chr("SBIN_PF", sbr->SBIN_PF);
				print_Cn("SBIN_NAM", sbr->SBIN_NAM);
				break;
			}
			case STDF_REC_PMR: {
				stdf_rec_pmr *pmr = (stdf_rec_pmr*)rec;
				print_int("PMR_INDX", pmr->PMR_INDX);
				print_int("CHAN_TYP", pmr->CHAN_TYP);
				print_Cn("CHAN_NAM", pmr->CHAN_NAM);
				print_Cn("PHY_NAM", pmr->PHY_NAM);
				print_Cn("LOG_NAM", pmr->LOG_NAM);
				print_int("HEAD_NUM", pmr->HEAD_NUM);
				print_int("SITE_NUM", pmr->SITE_NUM);
				break;
			}
			case STDF_REC_PGR: {
				stdf_rec_pgr *pgr = (stdf_rec_pgr*)rec;
				print_int("GRP_INDX", pgr->GRP_INDX);
				print_Cn("GRP_NAM", pgr->GRP_NAM);
				print_int("INDX_CNT", pgr->INDX_CNT);
				print_xU2("PMR_INDX", pgr->PMR_INDX, pgr->INDX_CNT);
				break;
			}
			case STDF_REC_PLR: {
				stdf_rec_plr *plr = (stdf_rec_plr*)rec;
				print_int("GRP_CNT", plr->GRP_CNT);
				print_xU2("GRP_INDX", plr->GRP_INDX, plr->GRP_CNT);
				print_xU2("GRP_MODE", plr->GRP_MODE, plr->GRP_CNT);
				print_xU1("GRP_RADX", plr->GRP_RADX, plr->GRP_CNT);
				print_xCn("PGM_CHAR", plr->PGM_CHAR, plr->GRP_CNT);
				print_xCn("RTN_CHAR", plr->RTN_CHAR, plr->GRP_CNT);
				print_xCn("PGM_CHAL", plr->PGM_CHAL, plr->GRP_CNT);
				print_xCn("RTN_CHAL", plr->RTN_CHAL, plr->GRP_CNT);
				break;
			}
			case STDF_REC_RDR: {
				stdf_rec_rdr *rdr = (stdf_rec_rdr*)rec;
				print_int("NUM_BINS", rdr->NUM_BINS);
				print_xU2("RTST_BIN", rdr->RTST_BIN, rdr->NUM_BINS);
				break;
			}
			case STDF_REC_SDR: {
				stdf_rec_sdr *sdr = (stdf_rec_sdr*)rec;
				print_int("HEAD_NUM", sdr->HEAD_NUM);
				print_int("SITE_GRP", sdr->SITE_GRP);
				print_int("SITE_CNT", sdr->SITE_CNT);
				print_xU1("SITE_NUM", sdr->SITE_NUM, sdr->SITE_CNT);
				print_Cn("HAND_TYP", sdr->HAND_TYP);
				print_Cn("HAND_ID", sdr->HAND_ID);
				print_Cn("CARD_TYP", sdr->CARD_TYP);
				print_Cn("CARD_ID", sdr->CARD_ID);
				print_Cn("LOAD_TYP", sdr->LOAD_TYP);
				print_Cn("LOAD_ID", sdr->LOAD_ID);
				print_Cn("DIB_TYP", sdr->DIB_TYP);
				print_Cn("DIB_ID", sdr->DIB_ID);
				print_Cn("CABL_TYP", sdr->CABL_TYP);
				print_Cn("CABL_ID", sdr->CABL_ID);
				print_Cn("CONT_TYP", sdr->CONT_TYP);
				print_Cn("CONT_ID", sdr->CONT_ID);
				print_Cn("LASR_TYP", sdr->LASR_TYP);
				print_Cn("LASR_ID", sdr->LASR_ID);
				print_Cn("EXTR_TYP", sdr->EXTR_TYP);
				print_Cn("EXTR_ID", sdr->EXTR_ID);
				break;
			}
			case STDF_REC_PSR: {
				stdf_rec_psr *psr = (stdf_rec_psr*)rec;
				print_int("CONT_FLG", psr->CONT_FLG);
				print_int("PSR_INDX", psr->PSR_INDX);
				print_Cn("PSR_NAM", psr->PSR_NAM);
				print_int("OPT_FLG", psr->OPT_FLG);
				print_int("TOTP_CNT", psr->TOTP_CNT);
				print_int("LOCP_CNT", psr->LOCP_CNT);
				print_xU8("PAT_BGN", psr->PAT_BGN, psr->LOCP_CNT);
				print_xU8("PAT_END", psr->PAT_END, psr->LOCP_CNT);
				print_xCn("PAT_FILE", psr->PAT_FILE, psr->LOCP_CNT);
				print_xCn("PAT_LBL", psr->PAT_LBL, psr->LOCP_CNT);
				print_xCn("FILE_UID", psr->FILE_UID, psr->LOCP_CNT);
				print_xCn("ATPG_DSC", psr->ATPG_DSC, psr->LOCP_CNT);
				print_xCn("SRC_ID", psr->SRC_ID, psr->LOCP_CNT);
				break;
			}
			case STDF_REC_NMR: {
				stdf_rec_nmr *nmr = (stdf_rec_nmr*)rec;
				print_int("CONT_FLG", nmr->CONT_FLG);
				print_int("TOTM_CNT", nmr->TOTM_CNT);
				print_int("LOCM_CNT", nmr->LOCM_CNT);
				print_xU2("PMR_INDX", nmr->PMR_INDX, nmr->LOCM_CNT);
				print_xCn("ATPG_NAM", nmr->ATPG_NAM, nmr->LOCM_CNT);
				break;
			}
			case STDF_REC_CNR: {
				stdf_rec_cnr *cnr = (stdf_rec_cnr*)rec;
				print_int("CHN_NUM", cnr->CHN_NUM);
				print_int("BIT_POS", cnr->BIT_POS);
				print_Sn("CELL_NAM", cnr->CELL_NAM);
				break;
			}
			case STDF_REC_SSR: {
				stdf_rec_ssr *ssr = (stdf_rec_ssr*)rec;
				print_Cn("SSR_NAM", ssr->SSR_NAM);
				print_int("CHN_CNT", ssr->CHN_CNT);
				print_xU2("CHN_LIST", ssr->CHN_LIST, ssr->CHN_CNT);
				break;
			}
			case STDF_REC_CDR: {
				stdf_rec_cdr *cdr = (stdf_rec_cdr*)rec;
				print_int("CONT_FLG", cdr->CONT_FLG);
				print_int("CDR_INDX", cdr->CDR_INDX);
				print_Cn("CHN_NAM", cdr->CHN_NAM);
				print_int("CHN_LEN", cdr->CHN_LEN);
				print_int("SIN_PIN", cdr->SIN_PIN);
				print_int("SOUT_PIN", cdr->SOUT_PIN);
				print_int("MSTR_CNT", cdr->MSTR_CNT);
				print_xU2("M_CLKS", cdr->M_CLKS, cdr->MSTR_CNT);
				print_int("SLAV_CNT", cdr->SLAV_CNT);
				print_xU2("S_CLKS", cdr->S_CLKS, cdr->SLAV_CNT);
				print_int("INV_VAL", cdr->INV_VAL);
				print_int("LST_CNT", cdr->LST_CNT);
                                print_xSn("CELL_LST", cdr->CELL_LST, cdr->LST_CNT);
				break;
			}
			case STDF_REC_WIR: {
				stdf_rec_wir *wir = (stdf_rec_wir*)rec;
				print_int("HEAD_NUM", wir->HEAD_NUM);
#ifdef STDF_VER3
				if (stdf_ver == 3)
				print_hex("PAD_BYTE", wir->PAD_BYTE);
				else
#endif
				print_int("SITE_GRP", wir->SITE_GRP);
				print_tim("START_T", wir->START_T);
				print_Cn("WAFER_ID", wir->WAFER_ID);
				break;
			}
			case STDF_REC_WRR: {
				stdf_rec_wrr *wrr = (stdf_rec_wrr*)rec;
#ifdef STDF_VER3
				if (stdf_ver == 4) {
#endif
				print_int("HEAD_NUM", wrr->HEAD_NUM);
				print_int("SITE_GRP", wrr->SITE_GRP);
				print_tim("FINISH_T", wrr->FINISH_T);
#ifdef STDF_VER3
				} else {
				print_tim("FINISH_T", wrr->FINISH_T);
				print_int("HEAD_NUM", wrr->HEAD_NUM);
				print_hex("PAD_BYTE", wrr->PAD_BYTE);
				}
#endif
				print_int("PART_CNT", wrr->PART_CNT);
				print_int("RTST_CNT", wrr->RTST_CNT);
				print_int("ABRT_CNT", wrr->ABRT_CNT);
				print_int("GOOD_CNT", wrr->GOOD_CNT);
				print_int("FUNC_CNT", wrr->FUNC_CNT);
				print_Cn("WAFER_ID", wrr->WAFER_ID);
#ifdef STDF_VER3
				if (stdf_ver == 4) {
#endif
				print_Cn("FABWF_ID", wrr->FABWF_ID);
				print_Cn("FRAME_ID", wrr->FRAME_ID);
				print_Cn("MASK_ID", wrr->MASK_ID);
#ifdef STDF_VER3
				} else {
				print_Cn("HAND_ID", wrr->HAND_ID);
				print_Cn("PRB_CARD", wrr->PRB_CARD);
				}
#endif
				print_Cn("USR_DESC", wrr->USR_DESC);
				print_Cn("EXC_DESC", wrr->EXC_DESC);
				break;
			}
			case STDF_REC_WCR: {
				stdf_rec_wcr *wcr = (stdf_rec_wcr*)rec;
				print_rel("WAFR_SIZ", wcr->WAFR_SIZ);
				print_rel("DIE_HT", wcr->DIE_HT);
				print_rel("DIE_WID", wcr->DIE_WID);
				print_int("WF_UNITS", wcr->WF_UNITS);
				print_chr("WF_FLAT", wcr->WF_FLAT);
				print_int("CENTER_X", wcr->CENTER_X);
				print_int("CENTER_Y", wcr->CENTER_Y);
				print_chr("POS_X", wcr->POS_X);
				print_chr("POS_Y", wcr->POS_Y);
				break;
			}
			case STDF_REC_PIR: {
				stdf_rec_pir *pir = (stdf_rec_pir*)rec;
				print_int("HEAD_NUM", pir->HEAD_NUM);
				print_int("SITE_NUM", pir->SITE_NUM);
#ifdef STDF_VER3
				if (stdf_ver == 3) {
				print_int("X_COORD", pir->X_COORD);
				print_int("Y_COORD", pir->Y_COORD);
				print_Cn("PART_ID", pir->PART_ID);
				}
#endif
				break;
			}
			case STDF_REC_PRR: {
				stdf_rec_prr *prr = (stdf_rec_prr*)rec;
				print_int("HEAD_NUM", prr->HEAD_NUM);
				print_int("SITE_NUM", prr->SITE_NUM);
#ifdef STDF_VER3
				if (stdf_ver == 4)
#endif
				print_hex("PART_FLG", prr->PART_FLG);
				print_int("NUM_TEST", prr->NUM_TEST);
				print_int("HARD_BIN", prr->HARD_BIN);
				print_int("SOFT_BIN", prr->SOFT_BIN);
#ifdef STDF_VER3
				if (stdf_ver == 3) {
				print_hex("PART_FLG", prr->PART_FLG);
				print_hex("PAD_BYTE", prr->PAD_BYTE);
				}
#endif
				print_int("X_COORD", prr->X_COORD);
				print_int("Y_COORD", prr->Y_COORD);
#ifdef STDF_VER3
				if (stdf_ver == 4)
#endif
				print_tim("TEST_T", prr->TEST_T);
				print_Cn("PART_ID", prr->PART_ID);
				print_Cn("PART_TXT", prr->PART_TXT);
				print_Bn("PART_FIX", prr->PART_FIX);
				break;
			}
#ifdef STDF_VER3
			case STDF_REC_PDR: {
				stdf_rec_pdr *pdr = (stdf_rec_pdr*)rec;
				print_int("TEST_NUM", pdr->TEST_NUM);
				print_hex("DESC_FLG", pdr->DESC_FLG);
				print_hex("OPT_FLAG", pdr->OPT_FLAG);
				print_int("RES_SCAL", pdr->RES_SCAL);
				print_Cn("UNITS", pdr->UNITS);
				print_int("RES_LDIG", pdr->RES_LDIG);
				print_int("RES_RDIG", pdr->RES_RDIG);
				print_int("LLM_SCAL", pdr->LLM_SCAL);
				print_int("HLM_SCAL", pdr->HLM_SCAL);
				print_int("LLM_LDIG", pdr->LLM_LDIG);
				print_int("LLM_RDIG", pdr->LLM_RDIG);
				print_int("HLM_LDIG", pdr->HLM_LDIG);
				print_int("HLM_RDIG", pdr->HLM_RDIG);
				print_rel("LO_LIMIT", pdr->LO_LIMIT);
				print_rel("HI_LIMIT", pdr->HI_LIMIT);
				print_Cn("TEST_NAM", pdr->TEST_NAM);
				print_Cn("SEQ_NAME", pdr->SEQ_NAME);
				break;
			}
			case STDF_REC_FDR: {
				stdf_rec_fdr *fdr = (stdf_rec_fdr*)rec;
				print_int("TEST_NUM", fdr->TEST_NUM);
				print_hex("DESC_FLG", fdr->DESC_FLG);
				print_Cn("TEST_NAM", fdr->TEST_NAM);
				print_Cn("SEQ_NAME", fdr->SEQ_NAME);
				break;
			}
#endif
			case STDF_REC_TSR: {
				stdf_rec_tsr *tsr = (stdf_rec_tsr*)rec;
				print_int("HEAD_NUM", tsr->HEAD_NUM);
				print_int("SITE_NUM", tsr->SITE_NUM);
#ifdef STDF_VER3
				if (stdf_ver == 4)
#endif
				print_chr("TEST_TYP", tsr->TEST_TYP);
				print_int("TEST_NUM", tsr->TEST_NUM);
				print_int("EXEC_CNT", tsr->EXEC_CNT);
				print_int("FAIL_CNT", tsr->FAIL_CNT);
				print_int("ALRM_CNT", tsr->ALRM_CNT);
#ifdef STDF_VER3
				if (stdf_ver == 4) {
#endif
				print_Cn("TEST_NAM", tsr->TEST_NAM);
				print_Cn("SEQ_NAME", tsr->SEQ_NAME);
				print_Cn("TEST_LBL", tsr->TEST_LBL);
				print_hex("OPT_FLAG", tsr->OPT_FLAG);
				print_rel("TEST_TIM", tsr->TEST_TIM);
				print_rel("TEST_MIN", tsr->TEST_MIN);
				print_rel("TEST_MAX", tsr->TEST_MAX);
				print_rel("TST_SUMS", tsr->TST_SUMS);
				print_rel("TST_SQRS", tsr->TST_SQRS);
#ifdef STDF_VER3
				} else {
				print_hex("OPT_FLAG", tsr->OPT_FLAG);
				print_hex("PAD_BYTE", tsr->PAD_BYTE);
				print_rel("TEST_MIN", tsr->TEST_MIN);
				print_rel("TEST_MAX", tsr->TEST_MAX);
				print_rel("TST_MEAN", tsr->TST_MEAN);
				print_rel("TST_SDEV", tsr->TST_SDEV);
				print_rel("TST_SUMS", tsr->TST_SUMS);
				print_rel("TST_SQRS", tsr->TST_SQRS);
				print_Cn("TEST_NAM", tsr->TEST_NAM);
				print_Cn("SEQ_NAME", tsr->SEQ_NAME);
				}
#endif
				break;
			}
			case STDF_REC_PTR: {
				stdf_rec_ptr *ptr = (stdf_rec_ptr*)rec;
				print_int("TEST_NUM", ptr->TEST_NUM);
				print_int("HEAD_NUM", ptr->HEAD_NUM);
				print_int("SITE_NUM", ptr->SITE_NUM);
				print_hex("TEST_FLG", ptr->TEST_FLG);
				print_hex("PARM_FLG", ptr->PARM_FLG);
				print_rel("RESULT", ptr->RESULT);
				print_Cn("TEST_TXT", ptr->TEST_TXT);
				print_Cn("ALARM_ID", ptr->ALARM_ID);
				print_hex("OPT_FLAG", ptr->OPT_FLAG);
				print_int("RES_SCAL", ptr->RES_SCAL);
				print_int("LLM_SCAL", ptr->LLM_SCAL);
				print_int("HLM_SCAL", ptr->HLM_SCAL);
				print_rel("LO_LIMIT", ptr->LO_LIMIT);
				print_rel("HI_LIMIT", ptr->HI_LIMIT);
				print_Cn("UNITS", ptr->UNITS);
				print_Cn("C_RESFMT", ptr->C_RESFMT);
				print_Cn("C_LLMFMT", ptr->C_LLMFMT);
				print_Cn("C_HLMFMT", ptr->C_HLMFMT);
				print_rel("LO_SPEC", ptr->LO_SPEC);
				print_rel("HI_SPEC", ptr->HI_SPEC);
				break;
			}
			case STDF_REC_MPR: {
				stdf_rec_mpr *mpr = (stdf_rec_mpr*)rec;
				print_int("TEST_NUM", mpr->TEST_NUM);
				print_int("HEAD_NUM", mpr->HEAD_NUM);
				print_int("SITE_NUM", mpr->SITE_NUM);
				print_hex("TEST_FLG", mpr->TEST_FLG);
				print_hex("PARM_FLG", mpr->PARM_FLG);
				print_int("RTN_ICNT", mpr->RTN_ICNT);
				print_int("RSLT_CNT", mpr->RSLT_CNT);
				print_xN1("RTN_STAT", mpr->RTN_STAT, mpr->RTN_ICNT);
				print_xR4("RTN_RSLT", mpr->RTN_RSLT, mpr->RSLT_CNT);
				print_Cn("TEST_TXT", mpr->TEST_TXT);
				print_Cn("ALARM_ID", mpr->ALARM_ID);
				print_hex("OPT_FLAG", mpr->OPT_FLAG);
				print_int("RES_SCAL", mpr->RES_SCAL);
				print_int("LLM_SCAL", mpr->LLM_SCAL);
				print_int("HLM_SCAL", mpr->HLM_SCAL);
				print_rel("LO_LIMIT", mpr->LO_LIMIT);
				print_rel("HI_LIMIT", mpr->HI_LIMIT);
				print_rel("START_IN", mpr->START_IN);
				print_rel("INCR_IN", mpr->INCR_IN);
				print_xU2("RTN_INDX", mpr->RTN_INDX, mpr->RTN_ICNT);
				print_Cn("UNITS", mpr->UNITS);
				print_Cn("UNITS_IN", mpr->UNITS_IN);
				print_Cn("C_RESFMT", mpr->C_RESFMT);
				print_Cn("C_LLMFMT", mpr->C_LLMFMT);
				print_Cn("C_HLMFMT", mpr->C_HLMFMT);
				print_rel("LO_SPEC", mpr->LO_SPEC);
				print_rel("HI_SPEC", mpr->HI_SPEC);
				break;
			}
			case STDF_REC_FTR: {
				stdf_rec_ftr *ftr = (stdf_rec_ftr*)rec;
				print_int("TEST_NUM", ftr->TEST_NUM);
				print_int("HEAD_NUM", ftr->HEAD_NUM);
				print_int("SITE_NUM", ftr->SITE_NUM);
				print_hex("TEST_FLG", ftr->TEST_FLG);
				print_hex("OPT_FLAG", ftr->OPT_FLAG);
				print_int("CYCL_CNT", ftr->CYCL_CNT);
				print_int("REL_VADR", ftr->REL_VADR);
				print_int("REPT_CNT", ftr->REPT_CNT);
				print_int("NUM_FAIL", ftr->NUM_FAIL);
				print_int("XFAIL_AD", ftr->XFAIL_AD);
				print_int("YFAIL_AD", ftr->YFAIL_AD);
				print_int("VECT_OFF", ftr->VECT_OFF);
				print_int("RTN_ICNT", ftr->RTN_ICNT);
				print_int("PGM_ICNT", ftr->PGM_ICNT);
				print_xU2("RTN_INDX", ftr->RTN_INDX, ftr->RTN_ICNT);
				print_xN1("RTN_STAT", ftr->RTN_STAT, ftr->RTN_ICNT);
				print_xU2("PGM_INDX", ftr->PGM_INDX, ftr->PGM_ICNT);
				print_xN1("PGM_STAT", ftr->PGM_STAT, ftr->PGM_ICNT);
				print_Dn("FAIL_PIN", ftr->FAIL_PIN);
				print_Cn("VECT_NAM", ftr->VECT_NAM);
				print_Cn("TIME_SET", ftr->TIME_SET);
				print_Cn("OP_CODE", ftr->OP_CODE);
				print_Cn("TEST_TXT", ftr->TEST_TXT);
				print_Cn("ALARM_ID", ftr->ALARM_ID);
				print_Cn("PROG_TXT", ftr->PROG_TXT);
				print_Cn("RSLT_TXT", ftr->RSLT_TXT);
				print_int("PATG_NUM", ftr->PATG_NUM);
				print_Dn("SPIN_MAP", ftr->SPIN_MAP);
				break;
			}
			case STDF_REC_STR: {
				stdf_rec_str *str = (stdf_rec_str*)rec;
				print_hex("CONT_FLG", str->CONT_FLG);
				print_int("TEST_NUM", str->TEST_NUM);
				print_int("HEAD_NUM", str->HEAD_NUM);
				print_int("SITE_NUM", str->SITE_NUM);
				print_int("PSR_REF",  str->PSR_REF);
				print_hex("TEST_FLG", str->TEST_FLG);
				print_Cn("LOG_TYP",  str->LOG_TYP);
				print_Cn("TEST_TXT", str->TEST_TXT);
				print_Cn("ALARM_ID", str->ALARM_ID);
				print_Cn("PROG_TXT", str->PROG_TXT);
				print_Cn("RSLT_TXT", str->RSLT_TXT);
				print_hex("Z_VAL",    str->Z_VAL);
				print_hex("FMU_FLG",  str->FMU_FLG);
				if (str->FMU_FLG & 1)
					print_Dn("MASK_MAP",  str->MASK_MAP);
				else
					printf("\t<MASK_MAP value=''/>\n");
				if ((str->FMU_FLG & 4) == 4)
					print_Dn("FAL_MAP",   str->FAL_MAP);
				else
					printf("\t<FAL_MAP value=''/>\n");
				print_int("CYCL_CNT", str->CYCL_CNT);
				print_int("TOTF_CNT", str->TOTF_CNT);
				print_int("TOTL_CNT", str->TOTL_CNT);
				print_int("CYC_BASE", str->CYC_BASE);
				print_int("BIT_BASE", str->BIT_BASE);
				print_int("COND_CNT", str->COND_CNT);
				print_int("LIM_CNT",  str->LIM_CNT);
				print_int("CYCL_SIZE", str->CYCL_SIZE);
				print_int("PMR_SIZE", str->PMR_SIZE);
				print_int("CHN_SIZE", str->CHN_SIZE);
				print_int("PAT_SIZE", str->PAT_SIZE);
				print_int("BIT_SIZE", str->BIT_SIZE);
				print_int("U1_SIZE",  str->U1_SIZE);
				print_int("U2_SIZE",  str->U2_SIZE);
				print_int("U3_SIZE",  str->U3_SIZE);
				print_int("UTX_SIZE", str->UTX_SIZE);
				print_int("CAP_BGN",  str->CAP_BGN);
				print_xU2("LIM_INDX", str->LIM_INDX, str->LIM_CNT);
				print_xU4("LIM_SPEC", str->LIM_SPEC, str->LIM_CNT);
				print_xCn("COND_LST", str->COND_LST, str->COND_CNT);
				print_int("CYC_CNT", str->CYC_CNT);
				print_xUf("CYC_OFST", str->CYC_OFST, str->CYC_CNT, str->CYCL_SIZE);
				print_int("PMR_CNT", str->PMR_CNT);
				print_xUf("PMR_INDX", str->PMR_INDX, str->PMR_CNT, str->PMR_SIZE);
				print_int("CHN_CNT", str->CHN_CNT);
				print_xUf("CHN_NUM", str->CHN_NUM, str->CHN_CNT, str->CHN_SIZE);
				print_int("EXP_CNT", str->EXP_CNT);
				print_xU1("EXP_DATA", str->EXP_DATA, str->EXP_CNT);
				print_int("CAP_CNT", str->CAP_CNT);
				print_xU1("CAP_DATA", str->CAP_DATA, str->CAP_CNT);
				print_int("NEW_CNT", str->NEW_CNT);
				print_xU1("NEW_DATA", str->NEW_DATA, str->NEW_CNT);
				print_int("PAT_CNT", str->PAT_CNT);
				print_xUf("PAT_NUM", str->PAT_NUM, str->PAT_CNT, str->PAT_SIZE);
				print_int("BPOS_CNT", str->BPOS_CNT);
				print_xUf("BIT_POS", str->BIT_POS, str->BPOS_CNT, str->BIT_SIZE);
				print_int("USR1_CNT", str->USR1_CNT);
				print_xUf("USR1", str->USR1, str->USR1_CNT, str->U1_SIZE);
				print_int("USR2_CNT", str->USR2_CNT);
				print_xUf("USR2", str->USR2, str->USR2_CNT, str->U2_SIZE);
				print_int("USR3_CNT", str->USR3_CNT);
				print_xUf("USR3", str->USR3, str->USR3_CNT, str->U3_SIZE);
				print_int("TXT_CNT", str->TXT_CNT);
				print_xCn("USER_TXT", str->USER_TXT, str->TXT_CNT);
				break;
			}
			case STDF_REC_BPS: {
				stdf_rec_bps *bps = (stdf_rec_bps*)rec;
				print_Cn("SEQ_NAME", bps->SEQ_NAME);
				break;
			}
			case STDF_REC_EPS: {
				/*stdf_rec_eps *eps = (stdf_rec_eps*)rec;*/
				break;
			}
#ifdef STDF_VER3
			case STDF_REC_SHB: {
				stdf_rec_shb *shb = (stdf_rec_shb*)rec;
				print_int("HEAD_NUM", shb->HEAD_NUM);
				print_int("SITE_NUM", shb->SITE_NUM);
				print_int("HBIN_NUM", shb->HBIN_NUM);
				print_int("HBIN_CNT", shb->HBIN_CNT);
				print_Cn("HBIN_NAM", shb->HBIN_NAM);
				break;
			}
			case STDF_REC_SSB: {
				stdf_rec_ssb *ssb = (stdf_rec_ssb*)rec;
				print_int("HEAD_NUM", ssb->HEAD_NUM);
				print_int("SITE_NUM", ssb->SITE_NUM);
				print_int("SBIN_NUM", ssb->SBIN_NUM);
				print_int("SBIN_CNT", ssb->SBIN_CNT);
				print_Cn("SBIN_NAM", ssb->SBIN_NAM);
				break;
			}
			case STDF_REC_STS: {
				stdf_rec_sts *sts = (stdf_rec_sts*)rec;
				print_int("HEAD_NUM", sts->HEAD_NUM);
				print_int("SITE_NUM", sts->SITE_NUM);
				print_int("TEST_NUM", sts->TEST_NUM);
				print_int("EXEC_CNT", sts->EXEC_CNT);
				print_int("FAIL_CNT", sts->FAIL_CNT);
				print_int("ALRM_CNT", sts->ALRM_CNT);
				print_hex("OPT_FLAG", sts->OPT_FLAG);
				print_hex("PAD_BYTE", sts->PAD_BYTE);
				print_rel("TEST_MIN", sts->TEST_MIN);
				print_rel("TEST_MAX", sts->TEST_MAX);
				print_rel("TST_MEAN", sts->TST_MEAN);
				print_rel("TST_SDEV", sts->TST_SDEV);
				print_rel("TST_SUMS", sts->TST_SUMS);
				print_rel("TST_SQRS", sts->TST_SQRS);
				print_Cn("TEST_NAM", sts->TEST_NAM);
				print_Cn("SEQ_NAME", sts->SEQ_NAME);
				print_Cn("TEST_LBL", sts->TEST_LBL);
				break;
			}
			case STDF_REC_SCR: {
				stdf_rec_scr *scr = (stdf_rec_scr*)rec;
				print_int("HEAD_NUM", scr->HEAD_NUM);
				print_int("SITE_NUM", scr->SITE_NUM);
				print_int("FINISH_T", scr->FINISH_T);
				print_int("PART_CNT", scr->PART_CNT);
				print_int("RTST_CNT", scr->RTST_CNT);
				print_int("ABRT_CNT", scr->ABRT_CNT);
				print_int("GOOD_CNT", scr->GOOD_CNT);
				print_int("FUNC_CNT", scr->FUNC_CNT);
				break;
			}
#endif
			case STDF_REC_GDR: {
				stdf_rec_gdr *gdr = (stdf_rec_gdr*)rec;
				print_int("FLD_CNT", gdr->FLD_CNT);
				print_Vn("GEN_DATA", gdr->GEN_DATA, gdr->FLD_CNT);
				break;
			}
			case STDF_REC_DTR: {
				stdf_rec_dtr *dtr = (stdf_rec_dtr*)rec;
				print_Cn("TEXT_DAT", dtr->TEXT_DAT);
				break;
			}
		}
		if (HEAD_TO_REC(rec->header) != STDF_REC_UNKNOWN)
			printf("</%s>\n", recname);
		stdf_free_record(rec);
	}
	printf("</stdf>\n");
	stdf_close(f);
}
	return EXIT_SUCCESS;
}
