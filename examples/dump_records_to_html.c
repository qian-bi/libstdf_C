/* dump_records_to_html.c
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

#define	MAX_WIDTH		25
#define	MAX_REC_STYLES	4
int width, rec_rot;
unsigned char *recbuff;

void write_rec(FILE *f, rec_header *h)
{
	int i;
	int towrite, written;
	unsigned char *rec;

	rec = recbuff;
	written = 0;

	do {
		towrite = MAX_WIDTH - width;
		if (h->REC_LEN < written + towrite)
			towrite = h->REC_LEN - written;
		for (i=0; i<towrite; ++i)
			fprintf(f, "<td class=r%i>%.2X</td>", rec_rot, rec[i]);
		width += towrite;
		rec += towrite*2;
		written += towrite;
		if (width == MAX_WIDTH) {
			fprintf(f, "</tr>\n<tr>");
			width = 0;
		}
	} while (written < h->REC_LEN);
}

int main(int argc, char *argv[])
{
	stdf_file *f;
	rec_header h;
	int byte_order;
	int in;
	int rec_count;
	FILE *out;

	if (argc != 3) {
		printf("Usage: %s <stdf file> <html file>\n", argv[0]);
		return EXIT_FAILURE;
	}

	f = stdf_open(argv[1]);
	if (!f) {
		perror("Could not stdf_open file");
		return EXIT_FAILURE;
	}
	byte_order = f->byte_order;
	stdf_close(f);

	if ((in=open(argv[1], O_RDONLY)) == -1) {
		perror("Could not open stdf file");
		return EXIT_FAILURE;
	}
unlink(argv[2]);
	if ((out=fopen(argv[2], "w")) == NULL) {
		perror("Could not open html file");
		return EXIT_FAILURE;
	}

	fprintf(out,
	        "<html>\n"
	        "<head>\n"
	        " <title>%s</title>\n"
	        " <style>\n"
	        "  td { border: 1px solid black; }\n"
	        "  td.r1 { background-color: #DDDAEC; }\n"
	        "  td.r2 { background-color: #FED0D4; }\n"
	        "  td.r3 { background-color: #D4FFA9; }\n"
	        "  td.r4 { background-color: #FEFFC5; }\n"
	        " </style>\n"
	        "</head>\n"
	        "<body>\n"
	        "<table style='border-collapse:collapse; font-family: monospace;'>\n<tr>",
	        argv[1]);

	recbuff = (char*)malloc(sizeof(char) * (0x0FFFF + 4));
	width = 0;
	rec_count = 0;
	rec_rot = 1;
	while (read(in, &h, 4) == 4) {
		if (byte_order != BYTE_ORDER)
			h.REC_LEN = bswap_16(h.REC_LEN);
		lseek(in, -4, SEEK_CUR);
		h.REC_LEN += 4;
		read(in, recbuff, h.REC_LEN);

		write_rec(out, &h);
		if (rec_count++ > 50)
			break;
		if (++rec_rot > MAX_REC_STYLES)
			rec_rot = 1;
	}
	free(recbuff);

	if (width != 0)
		fprintf(out, "</tr>\n");
	fprintf(out,
	        "</table>"
	        "</body>"
	        "</html>");

	fclose(out);
	close(in);

	return EXIT_SUCCESS;
}
