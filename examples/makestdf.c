/**
 * @file makestdf.c
 */
/*
 * Copyright (C) 2004 Mike Frysinger <vapier@gmail.com>
 * Released under the BSD license.  For more information,
 * please see: http://opensource.org/licenses/bsd-license.php
 *
 * $Header$
 */

#include <libstdf.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>

#define	write_head(typ, sub) \
	do { \
		*((dtc_U2*)buff) = rec - buff - 4; \
		buff[2] = typ; \
		buff[3] = sub; \
	} while (0)
#define	write_byte(b) *rec++ = b;
#define	write_bytes(b) \
	do { \
		memcpy(rec, &b, sizeof(b)); \
		rec += sizeof(b); \
	} while (0)
#define	write_string(s) \
	do { \
		U2 = strlen(s); \
		write_byte(U2); \
		memcpy(rec, s, U2); \
		rec += U2; \
	} while (0)
#define	write_rec(out, b) fwrite(b, 1, (*((dtc_U2*)b) + 4), out)

int main(int argc, char *argv[])
{
	FILE *out;
	char buff[65536], *rec;
	dtc_U2 U2;
	dtc_U4 U4;

	if (argc != 2) {
		printf("Usage: %s <stdf file>\n", argv[0]);
		return EXIT_FAILURE;
	}

	out = fopen(argv[1], "w");
	if (!out) {
		perror("Could not open output");
		return EXIT_FAILURE;
	}

	/* create FAR */
	rec = buff+4;
	write_byte(CPU_TYPE_X86);
	write_byte(4);
	write_head(REC_TYP_INFO, REC_SUB_FAR);
	write_rec(out, buff);

	/* create ATR */
	rec = buff+4;
	U4 = 3; write_bytes(U4);	/* MOD_TIM */
	write_string("CMD_LINE");	/* CMD_LINE */
	write_head(REC_TYP_INFO, REC_SUB_ATR);
	write_rec(out, buff);

	/* create MIR */
	rec = buff+4;
	U4 = 0; write_bytes(U4);	/* SETUP_T */
	U4 = 1; write_bytes(U4);	/* START_T */
	write_byte(2);				/* STAT_NUM */
	write_byte('D');			/* MODE_COD */
	write_byte(' ');			/* RTST_COD */
	write_byte(' ');			/* PROT_COD */
	U2 = 9; write_bytes(U2);	/* BURN_TIM */
	write_byte(' ');			/* CMOD_COD */
	write_string("LOT_ID");		/* LOT_ID */
	write_string("PART_TYP");	/* PART_TYP */
	write_string("NODE_NAM");	/* NODE_NAM */
	write_string("TSTR_TYP");	/* TSTR_TYP */
	write_string("JOB_NAM");	/* JOB_NAM */
	write_string("JOB_REV");	/* JOB_REV */
	write_string("SBLOT_ID");	/* SBLOT_ID */
	write_string("OPER_NAM");	/* OPER_NAM */
	write_string("EXEC_TYP");	/* EXEC_TYP */
	write_string("EXEC_VER");	/* EXEC_VER */
	write_string("TEST_COD");	/* TEST_COD */
	write_string("TST_TEMP");	/* TST_TEMP */
	write_string("USER_TXT");	/* USER_TXT */
	write_string("AUX_FILE");	/* AUX_FILE */
	write_string("PKG_TYP");	/* PKG_TYP */
	write_string("FAMILY_ID");	/* FAMILY_ID */
	write_string("DATE_COD");	/* DATE_COD */
	write_string("FACIL_ID");	/* FACIL_ID */
	write_string("FLOOR_ID");	/* FLOOR_ID */
	write_string("PROC_ID");	/* PROC_ID */
	write_string("OPER_FRQ");	/* OPER_FRQ */
	write_string("SPEC_NAM");	/* SPEC_NAM */
	write_string("SPEC_VER");	/* SPEC_VER */
	write_string("FLOW_ID");	/* FLOW_ID */
	write_string("SETUP_ID");	/* SETUP_ID */
	write_string("DSGN_REV");	/* DSGN_REV */
	write_string("ENG_ID");		/* ENG_ID */
	write_string("ROM_COD");	/* ROM_COD */
	write_string("SERL_NUM");	/* SERL_NUM */
	write_string("SUPR_NAM");	/* SUPR_NAM */
	write_head(REC_TYP_PER_LOT, REC_SUB_MIR);
	write_rec(out, buff);

	/* create MRR */
	rec = buff+4;
	U4 = 4; write_bytes(U4);	/* FINISH_T */
	write_byte(' ');
	write_string("USR_DESC");	/* USR_DESC */
	write_string("EXC_DESC");	/* EXC_DESC */
	write_head(REC_TYP_PER_LOT, REC_SUB_MRR);
	write_rec(out, buff);

	/* create PCR */
	rec = buff+4;
	write_byte(1);				/* HEAD_NUM */
	write_byte(2);				/* SITE_NUM */
	U4 = 5; write_bytes(U4);	/* PART_CNT */
	U4 = 6; write_bytes(U4);	/* RTST_CNT */
	U4 = 7; write_bytes(U4);	/* ABRT_CNT */
	U4 = 8; write_bytes(U4);	/* GOOD_CNT */
	U4 = 9; write_bytes(U4);	/* FUNC_CNT */
	write_head(REC_TYP_PER_LOT, REC_SUB_PCR);
	write_rec(out, buff);

	/* create HBR */
	rec = buff+4;
	write_byte(1);				/* HEAD_NUM */
	write_byte(2);				/* SITE_NUM */
	U2 = 6; write_bytes(U2);	/* HBIN_NUM */
	U4 = 8; write_bytes(U4);	/* HBIN_CNT */
	write_byte('F');			/* HBIN_PF */
	write_string("HBIN_NAM");	/* HBIN_NAM */
	write_head(REC_TYP_PER_LOT, REC_SUB_HBR);
	write_rec(out, buff);

	/* create SBR */
	rec = buff+4;
	write_byte(1);				/* HEAD_NUM */
	write_byte(2);				/* SITE_NUM */
	U2 = 0; write_bytes(U2);	/* SBIN_NUM */
	U4 = 6; write_bytes(U4);	/* SBIN_CNT */
	write_byte('P');			/* SBIN_PF */
	write_string("SBIN_NAM");	/* SBIN_NAM */
	write_head(REC_TYP_PER_LOT, REC_SUB_SBR);
	write_rec(out, buff);

	fclose(out);

	return EXIT_SUCCESS;
}
