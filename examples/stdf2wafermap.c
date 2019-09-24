/**
 * @file stdf2wafermap.c
 */
/*
 * Copyright (C) 2019 Stefan Brandner <stefan.brandner@gmx.at>
 * Released under the BSD license.  For more information,
 * please see: http://opensource.org/licenses/bsd-license.php
 */

#include <config.h>
#include <libstdf.h>

#include <internal/headers.h>

unsigned udc(int u) //unsigned digit count
{
	if (abs(u) < 10)    return 1;
	if (abs(u) < 100)   return 2;
	if (abs(u) < 1000)  return 3;
	if (abs(u) < 10000) return 4;
	return 0; //number was not supported
}

int main(int argc, char *argv[])
{
	stdf_file *f;
	stdf_rec_unknown *r;

	stdf_dtc_I2 x_min, y_min, x_max, y_max, x, y, x_range, y_range;
	stdf_dtc_U2 **hard_bins, **soft_bins, sb_max, i;
	stdf_dtc_U1 has_wafer_data = 0;

	char column_str[4];
	const stdf_dtc_I2 COORD_MAX = 9999;
	const stdf_dtc_I2 COORD_MIN = -9999;

	if (argc != 2) {
		printf("Usage: %s <stdf file>\n", argv[0]);
		return EXIT_FAILURE;
	}

	f = stdf_open(argv[1]);
	if (!f)
		return EXIT_FAILURE;

	x_min = y_min = COORD_MAX;
	x_max = y_max = COORD_MIN;

	while ((r=stdf_read_record(f)) != NULL) {
		switch (HEAD_TO_REC(r->header)) {
			case STDF_REC_MIR:
				break;
			case STDF_REC_PRR: {
				stdf_rec_prr *prr = (stdf_rec_prr*)r;
				// find min/max coordinate and exclude invalid coordinates e.g. -32768
				if (x_min > prr->X_COORD && prr->X_COORD > COORD_MIN) x_min = prr->X_COORD;
				if (y_min > prr->Y_COORD && prr->Y_COORD > COORD_MIN) y_min = prr->Y_COORD;
				if (x_max < prr->X_COORD && prr->X_COORD < COORD_MAX) x_max = prr->X_COORD;
				if (y_max < prr->Y_COORD && prr->Y_COORD < COORD_MAX) y_max = prr->Y_COORD;
				break;
			}
			case STDF_REC_WIR:
				has_wafer_data = 1;
				break;
		}
		stdf_free_record(r);
	}
	stdf_close(f);

	if (!has_wafer_data) {
		printf("There are no wafer data in this stdf file!\n");
		return EXIT_FAILURE;
	}

	printf("Wafermap\n\n");

	x_range = x_max - x_min + 1;
	y_range = y_max - y_min + 1;

	hard_bins = (stdf_dtc_U2**)malloc(x_range * sizeof(stdf_dtc_U2 *));
	for (x=0; x < x_range; x++) {
		hard_bins[x] = (stdf_dtc_U2*)calloc(y_range, sizeof(stdf_dtc_U2));
	}

	soft_bins = (stdf_dtc_U2**)malloc(x_range * sizeof(stdf_dtc_U2 *));
	for (x=0; x < x_range; x++) {
		soft_bins[x] = (stdf_dtc_U2*)calloc(y_range, sizeof(stdf_dtc_U2));
	}

	f = stdf_open(argv[1]);
	if (!f)
		return EXIT_FAILURE;

	sb_max = 0;
	while ((r=stdf_read_record(f)) != NULL) {
		if (HEAD_TO_REC(r->header) == STDF_REC_PRR) {
			stdf_rec_prr *prr = (stdf_rec_prr*)r;
			if (prr->Y_COORD < y_min || prr->Y_COORD > y_max || prr->X_COORD < x_min || prr->X_COORD > x_max) {
				printf("Ignoring die with wafercoordinates X = %i Y = %i\n\n", prr->X_COORD, prr->Y_COORD);
				continue;
			}
			x = prr->X_COORD - x_min;
			y = prr->Y_COORD - y_min;
			hard_bins[x][y] = prr->HARD_BIN; // or *(*(hard_bins+x)+y) = = prr->HARD_BIN;
			soft_bins[x][y] = prr->SOFT_BIN; // or *(*(soft_bins+x)+y) = = prr->SOFT_BIN;
			if (prr->SOFT_BIN > sb_max) sb_max = prr->SOFT_BIN;
		}
		stdf_free_record(r);
	}
	stdf_close(f);

	printf("HBIN Map\n\n");
	printf("MAP ROWS            : %i\n", y_range);
	printf("MAP COLUMNS         : %i\n", x_range);

	for (i=0; i<3; i++) {
		printf("\n    ");
		for (x=x_min; x<=x_max; x++) {
			sprintf(column_str, "%03i", x);
			printf("%c", column_str[i]);
		}
	}
	printf("\n\n");

	i=y_min;
	for (y=0; y<y_range; y++) {
		printf("%03i ", i++);
		for (x=0; x<x_range; x++) {
			if (hard_bins[x][y] == 0)
				printf(" ");
			else
				printf("%x", hard_bins[x][y]);
		}
		printf("\n");
	}

	printf("\nSBIN Map\n\n");
	printf("MAP ROWS            : %i\n", y_range);
	printf("MAP COLUMNS         : %i\n", x_range);

	unsigned sb_digits = udc(sb_max);

	for (i=0; i<3; i++) {
		printf("\n   %*c", sb_digits, ' ');
		for (x=x_min; x<=x_max; x++) {
			sprintf(column_str, "%03i", x);
			printf("%c", column_str[i]);
			printf("%*c", sb_digits, ' ');
		}
	}
	printf("\r\n\r\n");

	i=y_min;
	for (y=0; y<y_range; y++) {
		printf("%03i ", i++);
		for (x=0; x<x_range; x++) {
			if (soft_bins[x][y] == 0)
				printf("%*c", sb_digits+1, ' ');
			else
				printf("%*i ", sb_digits, soft_bins[x][y]);
		}
		printf("\n");
	}

	for (x=0; x < x_range; x++) {
		free(hard_bins[x]);
		free(soft_bins[x]);
	}
	free(hard_bins);
	free(soft_bins);

	return EXIT_SUCCESS;
}
