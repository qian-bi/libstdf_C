/**
 * @file makestdf.c
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

int main(int argc, char *argv[])
{
	stdf_file *f;

	if (argc != 2) {
		printf("Usage: %s <stdf file>\n", argv[0]);
		return EXIT_FAILURE;
	}

	f = stdf_open_ex(argv[1], STDF_OPTS_WRITE | STDF_OPTS_CREATE, 0644);
	if (!f) {
		perror("Could not open output");
		return EXIT_FAILURE;
	}

	{
		// use Far instead of far which is used in Windef.h when compiling for windows
		stdf_rec_far Far = {
			.CPU_TYPE = STDF_CPU_TYPE_X86,
			.STDF_VER = 4
		};
        	stdf_set_setting(f, STDF_SETTING_BYTE_ORDER, STDF_ENDIAN_LITTLE);
		stdf_init_header(Far.header, STDF_REC_FAR);
		stdf_write_record(f, &Far);
	}

	{
		stdf_rec_atr atr = {
			.MOD_TIM = 3,
			.CMD_LINE = "\010CMD_LINE"
		};
		stdf_init_header(atr.header, STDF_REC_ATR);
		stdf_write_record(f, &atr);
	}

	{
		stdf_rec_vur vur = {
			.UPD_NAM = "\007V4-2007"
		};
		stdf_init_header(vur.header, STDF_REC_VUR);
		stdf_write_record(f, &vur);
	}

	{
		stdf_rec_mir mir = {
			.SETUP_T = 0,
			.START_T = 1,
			.STAT_NUM = 2,
			.MODE_COD = 'D',
			.RTST_COD = ' ',
			.PROT_COD = ' ',
			.BURN_TIM = 9,
			.CMOD_COD = ' ',
			.LOT_ID   = "\006LOT_ID",
			.PART_TYP = "\010PART_TYP",
			.NODE_NAM = "\010NODE_NAM",
			.TSTR_TYP = "\010TSTR_TYP",
			.JOB_NAM  = "\007JOB_NAM",
			.JOB_REV  = "\007JOB_REV",
			.SBLOT_ID = "\010SBLOT_ID",
			.OPER_NAM = "\010OPER_NAM",
			.EXEC_TYP = "\010EXEC_TYP",
			.EXEC_VER = "\010EXEC_VER",
			.TEST_COD = "\010TEST_COD",
			.TST_TEMP = "\010TST_TEMP",
			.USER_TXT = "\010USER_TXT",
			.AUX_FILE = "\010AUX_FILE",
			.PKG_TYP  = "\007PKG_TYP",
			.FAMILY_ID = "\011FAMILY_ID",
			.DATE_COD = "\010DATE_COD",
			.FACIL_ID = "\010FACIL_ID",
			.FLOOR_ID = "\010FLOOR_ID",
			.PROC_ID  = "\007PROC_ID",
			.OPER_FRQ = "\010OPER_FRQ",
			.SPEC_NAM = "\010SPEC_NAM",
			.SPEC_VER = "\010SPEC_VER",
			.FLOW_ID  = "\007FLOW_ID",
			.SETUP_ID = "\010SETUP_ID",
			.DSGN_REV = "\010DSGN_REV",
			.ENG_ID   = "\006ENG_ID",
			.ROM_COD  = "\007ROM_COD",
			.SERL_NUM = "\010SERL_NUM",
			.SUPR_NAM = "\010SUPR_NAM"
		};
		stdf_init_header(mir.header, STDF_REC_MIR);
		stdf_write_record(f, &mir);
	}

	{
		stdf_dtc_U2 rtst_bin[10] = { 2, 4, 6, 8, 10, 12, 14, 16, 18, 20 };
		stdf_rec_rdr rdr = {
			.NUM_BINS = 10,
			.RTST_BIN = rtst_bin
		};
		stdf_init_header(rdr.header, STDF_REC_RDR);
		stdf_write_record(f, &rdr);
	}

	{
		stdf_dtc_U1 site_num[3] = { 1, 2, 3 };
		stdf_rec_sdr sdr = {
			.HEAD_NUM = 2,
			.SITE_GRP = 3,
			.SITE_CNT = 3,
			.SITE_NUM = site_num,
			.HAND_TYP = "\010HAND_TYP",
			.HAND_ID  = "\007HAND_ID",
			.CARD_TYP = "\010CARD_TYP",
			.CARD_ID  = "\007CARD_ID",
			.LOAD_TYP = "\010LOAD_TYP",
			.LOAD_ID  = "\007LOAD_ID",
			.DIB_TYP  = "\007DIB_TYP",
			.DIB_ID   = "\006DIB_ID",
			.CABL_TYP = "\010CABL_TYP",
			.CABL_ID  = "\007CABL_ID",
			.CONT_TYP = "\010CONT_TYP",
			.CONT_ID  = "\007CONT_ID",
			.LASR_TYP = "\010LASR_TYP",
			.LASR_ID  = "\007LASR_ID",
			.EXTR_TYP = "\010EXTR_TYP",
			.EXTR_ID  = "\007EXTR_ID"
		};
		stdf_init_header(sdr.header, STDF_REC_SDR);
		stdf_write_record(f, &sdr);
	}

	{
		stdf_rec_pmr pmr = {
			.PMR_INDX = 1,
			.CHAN_TYP = 78,
			.CHAN_NAM = "\005CHAN1",
			.PHY_NAM = "\010PHY_NAM1",
			.LOG_NAM = "\010LOG_NAM1",
			.HEAD_NUM = 68,
			.SITE_NUM = 4
		};
		stdf_init_header(pmr.header, STDF_REC_PMR);
		stdf_write_record(f, &pmr);
	}

	{
		stdf_rec_pmr pmr = {
			.PMR_INDX = 2,
			.CHAN_TYP = 79,
			.CHAN_NAM = "\005CHAN2",
			.PHY_NAM = "\010PHY_NAM2",
			.LOG_NAM = "\010LOG_NAM2",
			.HEAD_NUM = 68,
			.SITE_NUM = 4
		};
		stdf_init_header(pmr.header, STDF_REC_PMR);
		stdf_write_record(f, &pmr);
	}

	{
		stdf_rec_pmr pmr = {
			.PMR_INDX = 3,
			.CHAN_TYP = 80,
			.CHAN_NAM = "\005CHAN3",
			.PHY_NAM = "\010PHY_NAM3",
			.LOG_NAM = "\010LOG_NAM3",
			.HEAD_NUM = 68,
			.SITE_NUM = 4
		};
		stdf_init_header(pmr.header, STDF_REC_PMR);
		stdf_write_record(f, &pmr);
	}

	{
		stdf_rec_pmr pmr = {
			.PMR_INDX = 4,
			.CHAN_TYP = 81,
			.CHAN_NAM = "\005CHAN4",
			.PHY_NAM = "\010PHY_NAM4",
			.LOG_NAM = "\010LOG_NAM4",
			.HEAD_NUM = 68,
			.SITE_NUM = 4
		};
		stdf_init_header(pmr.header, STDF_REC_PMR);
		stdf_write_record(f, &pmr);
	}

	{
		stdf_dtc_U2 pmr_indx[3] = { 1, 2, 4};
		stdf_rec_pgr pgr = {
			.GRP_INDX = 45678,
			.GRP_NAM = "\007GRP_NAM",
			.INDX_CNT = 3,
			.PMR_INDX = pmr_indx
		};
		stdf_init_header(pgr.header, STDF_REC_PGR);
		stdf_write_record(f, &pgr);
	}

	{
		stdf_dtc_U2 grp_indx[6] = { 2, 4, 6, 8, 10, 12 };
		stdf_dtc_U2 grp_mode[6] = { 0, 10, 20, 21, 22, 23 };
		stdf_dtc_U1 grp_radx[6] = { 0, 2, 8, 10, 16, 20 };
		stdf_dtc_Cn pgm_char[6] = { "\001A", "\001B", "\001C", "\001D", "\001E", "\001F" };
		stdf_dtc_Cn rtn_char[6] = { "\001G", "\001H", "\001I", "\001J", "\001K", "\001L" };
		stdf_dtc_Cn pgm_chal[6] = { "\001M", "\001N", "\001O", "\001P", "\001Q", "\001R" };
		stdf_dtc_Cn rtn_chal[6] = { "\001S", "\001T", "\001U", "\001V", "\001W", "\001X" };
		stdf_rec_plr plr = {
			.GRP_CNT = 6,
			.GRP_INDX = grp_indx,
			.GRP_MODE = grp_mode,
			.GRP_RADX = grp_radx,
			.PGM_CHAR = pgm_char,
			.RTN_CHAR = rtn_char,
			.PGM_CHAL = pgm_chal,
			.RTN_CHAL = rtn_chal
		};
		stdf_init_header(plr.header, STDF_REC_PLR);
		stdf_write_record(f, &plr);
	}

	{
		stdf_dtc_U8 pat_bgn[3] = {12,123,1234};
		stdf_dtc_U8 pat_end[3] = {11234,123456,1234567};
		stdf_dtc_Cn pat_files[3] = {"\010Pat1.pat", "\010Pat2.pat", "\010Pat3.pat"};
		stdf_dtc_Cn pat_labels[3] = {"\004Pat1", "\004Pat2", "\004Pat3"};
		stdf_dtc_Cn file_uid[3] = {"\004321a", "\004321b", "\004321c"};
		stdf_dtc_Cn atpg_descr[3] = {"\005stat1", "\005stat2", "\005stat3"};
		stdf_dtc_Cn src_id[3] = {"\003ID1", "\003ID2", "\003ID3"};
		stdf_rec_psr psr = {
			.CONT_FLG = 0,
			.PSR_INDX = 1,
			.PSR_NAM = "\010Patterns",
			.OPT_FLG = 0xA,
			.TOTP_CNT = 3,
			.LOCP_CNT = 3,
			.PAT_BGN = pat_bgn,
			.PAT_END = pat_end,
			.PAT_FILE = pat_files,
			.PAT_LBL = pat_labels,
			.FILE_UID = file_uid,
			.ATPG_DSC = atpg_descr,
			.SRC_ID = src_id
		};
		stdf_init_header(psr.header, STDF_REC_PSR);
		stdf_write_record(f, &psr);
	}

	{
		stdf_dtc_U2 pmr_index[3] = {1,2,3};
		stdf_dtc_Cn atpg_name[3] = {"\010DATAIO_1", "\010DATAIO_2", "\003CLK"};
		stdf_rec_nmr nmr = {
			.CONT_FLG = 0,
			.TOTM_CNT = 3,
			.LOCM_CNT = 3,
			.PMR_INDX = pmr_index,
			.ATPG_NAM = atpg_name
		};
		stdf_init_header(nmr.header, STDF_REC_NMR);
		stdf_write_record(f, &nmr);
	}

	{
		stdf_rec_cnr cnr = {
			.CHN_NUM = 0,
			.BIT_POS = 987654321,
			.CELL_NAM = (unsigned char*)"\066\001ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz1234567890ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz1234567890ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz1234567890ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz1234567890ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz1234567890"
		};
		stdf_init_header(cnr.header, STDF_REC_CNR);
		stdf_write_record(f, &cnr);
	}
	{
		stdf_dtc_U2 channel_list[4] = {10,11,12,13};
		stdf_rec_ssr ssr = {
			.SSR_NAM = "\016Scan structure",
			.CHN_CNT = 4,
			.CHN_LIST = channel_list
		};
		stdf_init_header(ssr.header, STDF_REC_SSR);
		stdf_write_record(f, &ssr);
	}

	{
		stdf_dtc_U2 master_clock[1] = {11};
		stdf_dtc_U2 slave_clock[1] = {13};
		stdf_dtc_Sn cell_list[6] = {(unsigned char*)"\006\000Block1",(unsigned char*)"\006\000Block2",(unsigned char*)"\006\000Block3",(unsigned char*)"\006\000Block4",(unsigned char*)"\006\000Block5",(unsigned char*)"\006\000Block6"};
		stdf_rec_cdr cdr = {
			.CONT_FLG = 0,
			.CDR_INDX = 1,
			.CHN_NAM = "\007Chain_1",
			.CHN_LEN = 1543,
			.SIN_PIN = 12,
			.SOUT_PIN = 18,
			.MSTR_CNT = 1,
			.M_CLKS = master_clock,
			.SLAV_CNT = 1,
			.S_CLKS = slave_clock,
			.INV_VAL = 1,
			.LST_CNT = 6,
			.CELL_LST = cell_list
		};
		stdf_init_header(cdr.header, STDF_REC_CDR);
		stdf_write_record(f, &cdr);
	}

	{
		stdf_rec_wir wir = {
			.HEAD_NUM = 2,
			.SITE_GRP = 3,
			.START_T = 4,
			.WAFER_ID = "\010WAFER_ID"
		};
		stdf_init_header(wir.header, STDF_REC_WIR);
		stdf_write_record(f, &wir);
	}

	{
		stdf_rec_wrr wrr = {
			.HEAD_NUM = 20,
			.SITE_GRP = 10,
			.FINISH_T = 5,
			.PART_CNT = 1000,
			.RTST_CNT = 2000,
			.ABRT_CNT = 3000,
			.GOOD_CNT = 4000,
			.FUNC_CNT = 5000,
			.WAFER_ID = "\010WAFER_ID",
			.FABWF_ID = "\010FABWF_ID",
			.FRAME_ID = "\010FRAME_ID",
			.MASK_ID  = "\007MASK_ID",
			.USR_DESC = "\010USR_DESC",
			.EXC_DESC = "\010EXC_DESC"
		};
		stdf_init_header(wrr.header, STDF_REC_WRR);
		stdf_write_record(f, &wrr);
	}

	{
		stdf_rec_wcr wcr = {
			.WAFR_SIZ = 4.1,
			.DIE_HT = 2500.2,
			.DIE_WID = 5200.3,
			.WF_UNITS = 2,
			.WF_FLAT = 'D',
			.CENTER_X = 50,
			.CENTER_Y = 70,
			.POS_X = 'L',
			.POS_Y = 'U'
		};
		stdf_init_header(wcr.header, STDF_REC_WCR);
		stdf_write_record(f, &wcr);
	}

	{
		stdf_rec_pir pir = {
			.HEAD_NUM = 30,
			.SITE_NUM = 60
		};
		stdf_init_header(pir.header, STDF_REC_PIR);
		stdf_write_record(f, &pir);
	}

	{
		stdf_rec_ptr ptr = {
			.TEST_NUM =    3000,
			.HEAD_NUM =    1,
			.SITE_NUM =    3,
			.TEST_FLG =    0,
			.PARM_FLG =    0xC0,
			.RESULT   =    5.0,
			.TEST_TXT =    "\010TEST_TXT",
			.ALARM_ID =    "\010ALARM_ID",
			.OPT_FLAG =    0xE
		};
		stdf_init_header(ptr.header, STDF_REC_PTR);
		stdf_write_record(f, &ptr);
	}

	{
		stdf_dtc_N1 rtn_stat[] = { 0x21, 0x43 };
		stdf_dtc_R4 rtn_rslt[] = { 1.2, 2.3, 3.4, 4.5 };
		stdf_dtc_U2 rtn_indx[] = { 1, 2, 3, 4 };
		stdf_rec_mpr mpr = {
			.TEST_NUM = 2024,
			.HEAD_NUM = 1,
			.SITE_NUM = 2,
			.TEST_FLG = 0,
			.PARM_FLG = 0xC0,
			.RTN_ICNT = 4,
			.RSLT_CNT = 4,
			.RTN_STAT = rtn_stat,
			.RTN_RSLT = rtn_rslt,
			.TEST_TXT = "\010TEST_TXT",
			.OPT_FLAG = 0xE,
			.RES_SCAL = 3,
			.LLM_SCAL = 3,
			.HLM_SCAL = 3,
			.LO_LIMIT = 1.9,
			.HI_LIMIT = 9.1,
			.START_IN = 0.2,
			.INCR_IN  = 0.3,
			.RTN_INDX = rtn_indx,
			.UNITS    = "\001A",
			.UNITS_IN = "\010UNITS_IN",
			.C_RESFMT = "\005%1.2f",
			.C_LLMFMT = "\005%3.4f",
			.C_HLMFMT = "\005%5.6f",
			.LO_SPEC  = 0.9,
			.HI_SPEC  = 9.0
		};
		stdf_init_header(mpr.header, STDF_REC_MPR);
		stdf_write_record(f, &mpr);
	}

	{
		stdf_dtc_U2 rtn_indx[] = { 1010, 2020, 3030, 4040, 5050, 6060, 7070, 8080 };
		stdf_dtc_N1 rtn_stat[] = { 0x13, 0x24, 0x57 };
		stdf_dtc_U2 pgm_indx[] = { 101, 202, 303 };
		stdf_dtc_N1 pgm_stat[] = { 0x42, 0x05 };
		// little endian
		unsigned char fail_pin[] = { 0x10, 0x00, 0x00, 0x04 };
		unsigned char spin_map[] = { 0x00, 0x00 };
		stdf_rec_ftr ftr = {
			.TEST_NUM = 2024,
			.HEAD_NUM = 1,
			.SITE_NUM = 2,
			.TEST_FLG = 0x14,
			.OPT_FLAG = 0x00,
			.CYCL_CNT = 1234,
			.REL_VADR = 5678,
			.REPT_CNT = 9012,
			.NUM_FAIL = 3456,
			.XFAIL_AD = 7890,
			.YFAIL_AD = 5432,
			.VECT_OFF = 10,
			.RTN_ICNT = 6,
			.PGM_ICNT = 3,
			.RTN_INDX = rtn_indx,
			.RTN_STAT = rtn_stat,
			.PGM_INDX = pgm_indx,
			.PGM_STAT = pgm_stat,
			.FAIL_PIN = fail_pin,
			.VECT_NAM = "\010VECT_NAM",
			.TIME_SET = "\010TIME_SET",
			.OP_CODE  = "\007OP_CODE",
			.TEST_TXT = "\010TEST_TXT",
			.ALARM_ID = "\010ALARM_ID",
			.PROG_TXT = "\010PROG_TXT",
			.RSLT_TXT = "\010RSLT_TXT",
			.PATG_NUM = 254,
			.SPIN_MAP = spin_map
		};
		stdf_init_header(ftr.header, STDF_REC_FTR);
		stdf_write_record(f, &ftr);
	}
	{
		// little endian
   		stdf_dtc_Dn mask_map = (unsigned char*)"\000\000";
   		stdf_dtc_Dn fal_map = (unsigned char*)"\110\000\004\000\001\000\004\000\000\001\000";
		stdf_dtc_U2 condition_count = 3;
		stdf_dtc_Cn condition_list[3] = {"\0041.1V","\0041.5v","\0043.3V"};
		stdf_dtc_U2 cycle_count = 2;
		stdf_dtc_U2 cycle_offset[2] = { 300,400 };
		stdf_dtc_U2 pmr_count = 2;
		stdf_dtc_U2 pmr_index[2] = { 30,40 };
                stdf_dtc_U2 expect_count = 11;
                stdf_dtc_U1 expect_data[11] = { 'L','H','L','L','H','H','H','L','L','L','L'};
		stdf_rec_str str = {
			.CONT_FLG = 0,
			.TEST_NUM = 2050,
			.HEAD_NUM = 1,
			.SITE_NUM = 1,
			.PSR_REF = 1,
			.TEST_FLG = 0x4,
			.LOG_TYP = "\010Standard",
			.TEST_TXT = "\004Scan",
			.ALARM_ID = "\010ALARM_ID",
			.PROG_TXT = "\010PROG_TXT",
			.RSLT_TXT = "\006Failed",
			.Z_VAL = 0,
			.FMU_FLG = 4,	
			.MASK_MAP = mask_map,
			.FAL_MAP = fal_map,
			.CYCL_CNT = 2,
			.TOTF_CNT = 3,
			.TOTL_CNT = 3,
			.CYC_BASE = 2,
			.BIT_BASE = 114,
			.COND_CNT = condition_count,
			.LIM_CNT = 0,
			.CYCL_SIZE = 2,
			.PMR_SIZE = 2,
			.CHN_SIZE = 4,
			.PAT_SIZE = 4,
			.BIT_SIZE = 4,
			.U1_SIZE = 0,
			.U2_SIZE = 0,
			.U3_SIZE = 0,
			.UTX_SIZE = 0,
			.CAP_BGN = 0,
			.LIM_INDX = 0,
			.LIM_SPEC = 0,
			.COND_LST = condition_list,
			.CYC_CNT = cycle_count,
			.CYC_OFST = cycle_offset,
			.PMR_CNT = pmr_count,
			.PMR_INDX = pmr_index,
			.CHN_CNT = 0,
			.CHN_NUM = 0,
			.EXP_CNT = expect_count,
			.EXP_DATA = expect_data,
			.CAP_CNT = 0,
			.CAP_DATA = 0,
			.NEW_CNT = 0,
			.NEW_DATA = 0,
			.PAT_CNT =  0,
			.PAT_NUM =  0,
			.BPOS_CNT = 0,
			.BIT_POS = 0,
			.USR1_CNT = 0,
			.USR1 = 0,
			.USR2_CNT = 0,
			.USR2 = 0,
			.USR3_CNT = 0,
			.USR3 = 0,
			.TXT_CNT = 0,
			.USER_TXT = 0
		};
		stdf_init_header(str.header, STDF_REC_STR);
		stdf_write_record(f, &str);
	}

	{
		stdf_rec_bps bps = {
			.SEQ_NAME = "\010SEQ_NAME"
		};
		stdf_init_header(bps.header, STDF_REC_BPS);
		stdf_write_record(f, &bps);
	}

	{
		stdf_rec_eps eps;
		stdf_init_header(eps.header, STDF_REC_EPS);
		stdf_write_record(f, &eps);
	}

	{
   		stdf_dtc_Cn Cn = "\017Gendata Example";
   		stdf_dtc_Vn_ele Vn_ele1 = { STDF_GDR_Cn, (void*)&Cn};   // 17bytes sum = 17bytes
   		stdf_dtc_U1 U1=255;
   		stdf_dtc_Vn_ele Vn_ele2 = { STDF_GDR_U1, (void*)&U1};   // 2bytes  sum = 19bytes -> next is U2 and since U2 type is on a odd position no pad byte is needed
   		stdf_dtc_U2 U2=65123;
   		stdf_dtc_Vn_ele Vn_ele3 = { STDF_GDR_U2, (void*)&U2};   // 3bytes  sum = 22bytes
   		stdf_dtc_I1 I1=-125;
   		stdf_dtc_Vn_ele Vn_ele4 = { STDF_GDR_I1, (void*)&I1};   // 2bytes sum = 24bytes -> since I2 is coming next and I2 data bytes should be even distributed a pad byte is needed
   		stdf_dtc_Vn_ele Vn_ele5 = { STDF_GDR_B0, 0};            // 1byte  sum = 25bytes
   		stdf_dtc_I2 I2=-31534;
   		stdf_dtc_Vn_ele Vn_ele6 = { STDF_GDR_I2, (void*)&I2};   // 3bytes sum = 28bytes -> since I4 is coming next and I4 data bytes should be even distributed a pad byte is needed
   		stdf_dtc_Vn_ele Vn_ele7 = { STDF_GDR_B0, 0};            // 1byte  sum = 29byte
   		stdf_dtc_I4 I4=-2147483646;
   		stdf_dtc_Vn_ele Vn_ele8 = { STDF_GDR_I4, (void*)&I4};   // 5bytes sum = 34byted -> since R4 is coming next and R4 data bytes should be even distributed a pad byte is needed
   		stdf_dtc_Vn_ele Vn_ele9 = { STDF_GDR_B0, 0};            // 1byte  sum = 35bytes
   		stdf_dtc_R4 R4=3.141593e-2;
   		stdf_dtc_Vn_ele Vn_ele10 = { STDF_GDR_R4, (void*)&R4};  // 5bytes sum = 40bytes -> since R8 is coming next and R8 data bytes should be even distributed a pad byte is needed
   		stdf_dtc_Vn_ele Vn_ele11 = { STDF_GDR_B0, 0};           // 1byte  sum = 41bytes 
   		stdf_dtc_R8 R8=3.141593e5;
   		stdf_dtc_Vn_ele Vn_ele12 = { STDF_GDR_R8, (void*)&R8};
   		stdf_dtc_Bn Bn = (unsigned char*)"\004\000\001\002\003";
   		stdf_dtc_Vn_ele Vn_ele13 = { STDF_GDR_Bn, (void*)&Bn};
   		 // little endian
   		stdf_dtc_Dn Dn = (unsigned char*)"\110\000\004\000\001\000\004\000\000\001\000";
   		stdf_dtc_Vn_ele Vn_ele14 = { STDF_GDR_Dn, (void*)&Dn};
   		stdf_dtc_N1 N1 = 0xF;
   		stdf_dtc_Vn_ele Vn_ele15 = { STDF_GDR_N1, (void*)&N1};

		stdf_dtc_Vn_ele Vn[] = {Vn_ele1, Vn_ele2, Vn_ele3, Vn_ele4, Vn_ele5, Vn_ele6, Vn_ele7, Vn_ele8, Vn_ele9, Vn_ele10, Vn_ele11, Vn_ele12, Vn_ele13, Vn_ele14, Vn_ele15};

		stdf_rec_gdr gdr = {
		    .FLD_CNT = 15,
		    .GEN_DATA =Vn
		};
		stdf_init_header(gdr.header, STDF_REC_GDR);
		stdf_write_record(f, &gdr);
	}

	{
		stdf_rec_dtr dtr = {
			.TEXT_DAT = "\202ABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZ"
		};
		stdf_init_header(dtr.header, STDF_REC_DTR);
		stdf_write_record(f, &dtr);
	}

    {
		stdf_rec_prr prr =
		{
			.HEAD_NUM = 13,
			.SITE_NUM =  3,
			.PART_FLG =  0x8,
			.NUM_TEST =  5,
			.HARD_BIN =  1,
			.SOFT_BIN =  1,
			.X_COORD =  -10,
			.Y_COORD =  24,
			.TEST_T =  2012,
			.PART_ID =  "\0011",
			.PART_TXT =  "\10TEXT_DAT",
			.PART_FIX =  0
		};
		stdf_init_header(prr.header, STDF_REC_PRR);
		stdf_write_record(f, &prr);
	}

	{
		stdf_rec_tsr tsr = {
			.HEAD_NUM = 13,
			.SITE_NUM = 23,
			.TEST_TYP = 'P',
			.TEST_NUM = 33,
			.EXEC_CNT = 101010,
			.FAIL_CNT = 202020,
			.ALRM_CNT = 303030,
			.TEST_NAM = "\010TEST_NAM",
			.SEQ_NAME = "\010SEQ_NAME",
			.TEST_LBL = "\010TEST_LBL",
			.OPT_FLAG = 0x4 | 0x6 | 0x7,
			.TEST_TIM = 1.0,
			.TEST_MIN = 1.5,
			.TEST_MAX = 33.33,
			.TST_SUMS = 66.66,
			.TST_SQRS = 8.125
		};
		stdf_init_header(tsr.header, STDF_REC_TSR);
		stdf_write_record(f, &tsr);
	}

	{
		stdf_rec_hbr hbr = {
			.HEAD_NUM = 1,
			.SITE_NUM = 2,
			.HBIN_NUM = 6,
			.HBIN_CNT = 8,
			.HBIN_PF = 'F',
			.HBIN_NAM = "\010HBIN_NAM"
		};
		stdf_init_header(hbr.header, STDF_REC_HBR);
		stdf_write_record(f, &hbr);
	}

	{
		stdf_rec_sbr sbr = {
			.HEAD_NUM = 1,
			.SITE_NUM = 2,
			.SBIN_NUM = 0,
			.SBIN_CNT = 6,
			.SBIN_PF = 'P',
			.SBIN_NAM = "\010SBIN_NAM"
		};
		stdf_init_header(sbr.header, STDF_REC_SBR);
		stdf_write_record(f, &sbr);
	}

        {
		stdf_rec_pcr pcr = {
			.HEAD_NUM = 1,
			.SITE_NUM = 2,
			.PART_CNT = 5,
			.RTST_CNT = 6,
			.ABRT_CNT = 7,
			.GOOD_CNT = 8,
			.FUNC_CNT = 9
		};
		stdf_init_header(pcr.header, STDF_REC_PCR);
		stdf_write_record(f, &pcr);
	}

	{
		stdf_rec_mrr mrr = {
			.FINISH_T = 4,
			.DISP_COD = ' ',
			.USR_DESC = "\010USR_DESC",
			.EXC_DESC = "\010EXC_DESC"
		};
		stdf_init_header(mrr.header, STDF_REC_MRR);
		stdf_write_record(f, &mrr);
	}

	stdf_close(f);

	return EXIT_SUCCESS;
}
