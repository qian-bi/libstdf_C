/**
 * @file read_write.c
 */
/*
 * Copyright (C) 2017 Stefan Brandner <stefan.brandner@gmx.at>
 * Released under the BSD license.  For more information,
 * please see: http://opensource.org/licenses/bsd-license.php
 */

#include <config.h>
#include <libstdf.h>

#include <internal/headers.h>

void usage(char *prog)
{
	printf("Usage: %s <stdf input file> <stdf output file>\n", prog);
}

int main(int argc, char *argv[])
{
	stdf_rec_unknown *rec;
	stdf_file *f;
	stdf_file *write_f;
	stdf_dtc_U2 stdf_byte_order;
	char *filename_in, *filename_out;
	long count=0;

	if (argc != 3) {
		if (argc == 1)
			fprintf(stderr, "Missing source/destination files!\n");
		else if (argc == 2)
			fprintf(stderr, "Missing destination file!\n");
		else
			fprintf(stderr, "Too many arguments!\n");
		usage(argv[0]);
		return EXIT_FAILURE;
	}
	filename_in = argv[1];
	filename_out = argv[2];

	f = stdf_open(filename_in);
	if (!f) {
		perror("Could not stdf_open file");
		return EXIT_FAILURE;
	}

	write_f = stdf_open_ex(filename_out,STDF_OPTS_WRITE | STDF_OPTS_CREATE, 0644);
	if (!write_f) {
		perror("Could not open file for writing");
		return EXIT_FAILURE;
	}

	printf("Write_Read\n"
	       "Source: '%s'\n"
	       "Output: '%s'\n",
	       filename_in, filename_out);

        stdf_get_setting(f, STDF_SETTING_BYTE_ORDER, &stdf_byte_order);
        stdf_set_setting(write_f, STDF_SETTING_BYTE_ORDER, stdf_byte_order);

	count = 0;
	while ((rec=stdf_read_record_raw(f)) != NULL) {
		stdf_write_record_raw(write_f, rec);
		stdf_free_record(rec);
		count++;

	}
	printf("\n\nTransfered %li records\n", count);

	stdf_close(f);
        stdf_close(write_f);

	return EXIT_SUCCESS;
}
