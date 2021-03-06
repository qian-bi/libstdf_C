/**
 * @file stdf2img.c
 */
/*
 * Copyright (C) 2005-2007 Mike Frysinger <vapier@gmail.com>
 * Copyright (C) 2019 Stefan Brandner <stefan.brandner@gmx.at>
 * Released under the BSD license.  For more information,
 * please see: http://opensource.org/licenses/bsd-license.php
 */

#include <config.h>
#include <libstdf.h>
#include <png.h>

#include <internal/headers.h>

int main(int argc, char *argv[])
{
	stdf_file *f;
	stdf_rec_unknown *r;

	stdf_dtc_I2 x_min, y_min, x_max, y_max, x, y, x_range, y_range;
	stdf_dtc_U2 **hard_bins, **soft_bins, sb_max;
	stdf_dtc_U1 has_wafer_data = 0;

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

	printf("Creating Wafermap hb_wafermap.png\n\n");

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

	{
		FILE *outimg;

		png_structp png_ptr = NULL;
		png_infop info_ptr = NULL;
		//size_t x, y;
		png_byte ** row_pointers = NULL;

		int pixel_size = 3;
		int depth = 8;

		outimg = fopen("hb_wafermap.png", "wb");
		if (!outimg) {
			return EXIT_FAILURE;
		}
		png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
		if (png_ptr == NULL) {
			fclose(outimg);
			return EXIT_FAILURE;
		}
		info_ptr = png_create_info_struct(png_ptr);
		if (info_ptr == NULL) {
			png_destroy_write_struct(&png_ptr, &info_ptr);
			fclose(outimg);
			return EXIT_FAILURE;
		}

		/* Set up error handling. */
		if (setjmp(png_jmpbuf(png_ptr))) {
			png_destroy_write_struct(&png_ptr, &info_ptr);
			fclose(outimg);
			return EXIT_FAILURE;
		}

		/* Set image attributes. */
		png_set_IHDR(png_ptr,
					 info_ptr,
					 x_range,
					 y_range,
					 depth,
					 PNG_COLOR_TYPE_RGB,
					 PNG_INTERLACE_NONE,
					 PNG_COMPRESSION_TYPE_DEFAULT,
					 PNG_FILTER_TYPE_DEFAULT);

		/* Initialize rows of PNG. */
		row_pointers = png_malloc(png_ptr, y_range * sizeof(png_byte *));
		for (y = 0; y < y_range; y++) {
			png_byte *row = png_malloc(png_ptr, sizeof(uint8_t) * x_range * pixel_size);
			row_pointers[y] = row;
			for (x = 0; x < x_range; x++) {
				if (hard_bins[x][y] == 0) {
					// color white
					*row++ = 255; // red
					*row++ = 255; // green
					*row++ = 255; // blue
					// printf(" ");
				}
				else if (hard_bins[x][y] == 1) {
					// color green
					*row++ = 0;   // red
					*row++ = 247; // green
					*row++ = 0;   // blue
					// printf("P");
				}
				else {
					// color red
					*row++ = 255; // red
					*row++ = 0;   // green
					*row++ = 0;   // blue
					// printf("F");
				}
			}
			// printf("\n");
		}

		/* Write the image data to "outimg". */
		png_init_io(png_ptr, outimg);
		png_set_rows(png_ptr, info_ptr, row_pointers);
		png_write_png(png_ptr, info_ptr, PNG_TRANSFORM_IDENTITY, NULL);

		for (y = 0; y < y_range; y++) {
			png_free(png_ptr, row_pointers[y]);
		}
		png_free(png_ptr, row_pointers);

		png_destroy_write_struct(&png_ptr, &info_ptr);
		fclose(outimg);
	}

	for (x=0; x < x_range; x++) {
		free(hard_bins[x]);
		free(soft_bins[x]);
	}
	free(hard_bins);
	free(soft_bins);

	return EXIT_SUCCESS;
}
