/**
 * @file is_valid_stdf.c
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

#define	print_msg(m) printf("\t" m "\n");
#define	print_err(m) printf("\tERROR: " m "\n");

int main(int argc, char *argv[])
{
	stdf_file *f;
	stdf_rec_unknown *rec;
	stdf_rec_header prev_rec;
	long stdf_rec_mrr_cnt, stdf_rec_pcr_cnt, stdf_rec_hbr_cnt, stdf_rec_sbr_cnt, stdf_rec_wcr_cnt;
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

	/* STDF spec requires every STDF file have an initial sequence.
	 * A valid sequence can be any of these:
	 FAR -      - MIR
	 FAR - ATRs - MIR
	 FAR -      - MIR - RDR
	 FAR - ATRs - MIR - RDR
	 FAR -      - MIR -     - SDRs
	 FAR - ATRs - MIR -     - SDRs
	 FAR -      - MIR - RDR - SDRs
	 FAR - ATRs - MIR - RDR - SDRs

	 * every STDF V4 2007 must contain one of these initial sequences
 	 FAR - VUR  - MIR
	 FAR - ATRs - VUR  - MIR
	 FAR - VUR  - MIR  - RDR
	 FAR - ATRs - VUR  - MIR  - RDR
	 FAR - VUR  - MIR  - SDRs
	 FAR - ATRs - VUR  - MIR  - SDRs
	 FAR - VUR  - MIR  - RDR  - SDRs
	 FAR - ATRs - VUR  - MIR  - RDR  - SDRs
	 */

	/* Find the FAR record */
	rec = stdf_read_record(f);
	if (rec == NULL || HEAD_TO_REC(rec->header) != STDF_REC_FAR) {
		print_err("First record is not FAR!");
		goto next_file;
	}
	stdf_free_record(rec);
	/* Try to read all the ATR records (if they exist) */
	while ((rec=stdf_read_record(f)) != NULL) {
		if (HEAD_TO_REC(rec->header) != STDF_REC_ATR)
			break;
		else
			stdf_free_record(rec);
	}
	if (rec == NULL) {
		print_err("Initial sequence not found!");
		goto next_file;
	}
        if (HEAD_TO_REC(rec->header) == STDF_REC_VUR) {
                printf("\nis stdf V4-2007!\n");
		rec = stdf_read_record(f);
        }
        
	/* We should now have the MIR record already read in */
	if (HEAD_TO_REC(rec->header) != STDF_REC_MIR) {
		print_err("Initial sequence wrong: MIR not located!");
		goto next_file;
	}
	/* Try to read the RDR record (if it exists) */
	stdf_free_record(rec);
	if ((rec=stdf_read_record(f)) == NULL) {
		print_err("EOF found after initial sequence!");
		goto next_file;
	}
	if (HEAD_TO_REC(rec->header) == STDF_REC_RDR) {
		stdf_free_record(rec);
		rec = stdf_read_record(f);
		if (rec == NULL) {
			print_err("EOF found after initial sequence!");
			goto next_file;
		}
	}
	/* Try to read the SDR records (if they exist) */
	while (HEAD_TO_REC(rec->header) == STDF_REC_SDR) {
		stdf_free_record(rec);
		rec = stdf_read_record(f);
		if (rec == NULL) {
			print_err("EOF found after initial sequence!");
			goto next_file;
		}
	}

	/* Now we read the rest of the file */
	stdf_rec_mrr_cnt = stdf_rec_pcr_cnt = stdf_rec_hbr_cnt = stdf_rec_sbr_cnt = stdf_rec_wcr_cnt = 0;
	while (1) {
		memcpy(&prev_rec, &rec->header, sizeof(stdf_rec_header));
		stdf_free_record(rec);
		rec = stdf_read_record(f);
		if (rec == NULL)
			break;

		switch (HEAD_TO_REC(rec->header)) {
			case STDF_REC_FAR:
			case STDF_REC_ATR:
			case STDF_REC_MIR:
			case STDF_REC_RDR:
			case STDF_REC_SDR:
				printf("\tFound %s outside of initial sequence!\n",
				       stdf_get_rec_name(rec->header.REC_TYP, rec->header.REC_SUB));
				goto next_file;
			case STDF_REC_MRR:
				if (++stdf_rec_mrr_cnt > 1) {
					print_err("More than one STDF_REC_MRR was found!");
					goto next_file;
				}
				break;
			case STDF_REC_PCR: ++stdf_rec_pcr_cnt; break;
			case STDF_REC_HBR: ++stdf_rec_hbr_cnt; break;
			case STDF_REC_SBR: ++stdf_rec_sbr_cnt; break;

			/* need some logic with these ... */
			case STDF_REC_PMR: break;
			case STDF_REC_PGR: break;
			case STDF_REC_PLR: break;
                        case STDF_REC_PSR: break;
                        case STDF_REC_NMR: break;
                        case STDF_REC_CNR: break;
                        case STDF_REC_SSR: break;
                        case STDF_REC_CDR: break;
	

			case STDF_REC_WIR: break; /* only 1 per wafer */
			case STDF_REC_WRR: break; /* only 1 per wafer */
			case STDF_REC_WCR:
				if (++stdf_rec_wcr_cnt > 1) {
					print_err("More than one STDF_REC_WCR was found!");
					goto next_file;
				}
				break;

			/* each PIR must have a PRR for same HEAD/SITE */
			/* PTR/MPR/FTR/STR records must appear between the right PIR/PRR pairs */
			/* each BPS/EPS pair must be inside the PIR/PRR pair */
			case STDF_REC_PIR: break; /* only 1 per part tested */
			case STDF_REC_TSR: break;
			case STDF_REC_PTR: break; 
			case STDF_REC_MPR: break; 
			case STDF_REC_FTR: break; 
			case STDF_REC_STR: break; 
			case STDF_REC_BPS: break;
			case STDF_REC_EPS: break;
			case STDF_REC_PRR: break; /* only 1 per part tested */

			case STDF_REC_GDR: break;
			case STDF_REC_DTR: break;
	
			default:
				print_err("Uknown record found!");
				goto next_file;
		}
	}
	if (HEAD_TO_REC(prev_rec) != STDF_REC_MRR) {
		print_err("STDF_REC_MRR was not the last record in the stream!");
		goto next_file;
	}

	print_msg("... is valid");
next_file:
	stdf_free_record(rec);
	stdf_close(f);
}
	return EXIT_SUCCESS;
}
