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

#if defined(HAVE_GETOPT_H)
# include <getopt.h>
#endif

#define	MAX_WIDTH		25
#define	MAX_REC_STYLES	4
int width, rec_rot;
unsigned char *recbuff;

#define	OUT_HEX			1
#define	OUT_ASCII		2

void write_rec(FILE *f, rec_header *h, int type)
{
	int i;
	int towrite, written;
	unsigned char *rec;
	int tagged;

	rec = recbuff;
	written = 0;
	tagged = 0;

	do {
		towrite = MAX_WIDTH - width;
		if (h->REC_LEN < written + towrite)
			towrite = h->REC_LEN - written;
		for (i=0; i<towrite; ++i) {
			if (tagged > 3) {						/* raw data */
				fprintf(f, "<td class=r%i>", rec_rot);
				if (type == OUT_HEX)
					fprintf(f, "%.2X", rec[i]);
				else {
					if (rec[i] < 0x20)
						fprintf(f, "%.2X", rec[i]);
					else
						fprintf(f, "%c", rec[i]);
				}
				fprintf(f, "</td>");
			} else {								/* record header */
				if (type == OUT_HEX) {
					fprintf(f, "<td class=r%i><span class='head", rec_rot);
					fprintf(f, (tagged<2)?"len":"type");
					fprintf(f, "'>%.2X</span></td>", rec[i]);
				} else {
					if (tagged == 0)		/* rec len */
						fprintf(f, "<td class=r%i colspan=2><span class=headlen>%i</span></td>",
						        rec_rot, h->REC_LEN);
					else if (tagged == 2)	/* rec type */
						fprintf(f, "<td class=r%i colspan=2><span class=headtype>%s</span></td>",
						        rec_rot, stdf_get_rec_name_from_head((*h)));
					else if (width == 0 && i == 0)
						fprintf(f, "<td class=r%i></td>", rec_rot);
				}
				tagged++;
			}
		}
		width += towrite;
		rec += towrite;
		written += towrite;
		if (width == MAX_WIDTH) {
			fprintf(f, "</tr>\n<tr>");
			width = 0;
		}
	} while (written < h->REC_LEN);
}

void usage(char *prog)
{
	printf("Usage: %s [options] <stdf file> <html file>\n"
#if defined(HAVE_GETOPT_H)
	       "Options:\n"
	       "\t-h\tthis screen\n"
	       "\t-c\t# of records to output (default is 25; 0 to show all)\n"
#else
	       "\nin the excellent words of netcat:\n"
	       "/* If your shitbox doesn't have getopt, step into the nineties already. */\n\n"
#endif
	       , prog);
}

int main(int argc, char *argv[])
{
	stdf_file *f;
	rec_header h;
	int byte_order, stdfver;
	char cpu_name[256];
	int in;
	FILE *out;
	int x, rec_count, max_recs, type;

	max_recs = 25;
#if defined(HAVE_GETOPT_H)
	while ((x=getopt(argc, argv, "c:h")) != EOF) {
		switch (x) {
			case 'c':
				max_recs = atoi(optarg);
				break;
			case 'h':
				usage(argv[0]);
				return EXIT_SUCCESS;
			default:
				usage(argv[0]);
				return EXIT_FAILURE;
		}
	}
	x = argc - optind;
#else
	x = argc - 1;
#endif

	if (x != 2) {
		if (x == 0)
			fprintf(stderr, "Missing source/destination files!\n");
		else if (x == 1)
			fprintf(stderr, "Missing destination file!\n");
		else
			fprintf(stderr, "Too many arguements!\n");
		usage(argv[0]);
		return EXIT_FAILURE;
	}
#if defined(HAVE_GETOPT_H)
	x = optind;
#else
	x = 1;
#endif

	f = stdf_open(argv[x]);
	if (!f) {
		perror("Could not stdf_open file");
		return EXIT_FAILURE;
	}
	byte_order = f->byte_order;
	stdfver = f->ver;
	stdf_close(f);
	if (f->byte_order == LITTLE_ENDIAN)
		sprintf(cpu_name, "Little Endian [intel/x86]");
	else if (f->byte_order == BIG_ENDIAN)
		sprintf(cpu_name, "Big Endian [sun/sparc]");
	else
		sprintf(cpu_name, "Unknown Endian [???]");

	if ((in=open(argv[x], O_RDONLY)) == -1) {
		perror("Could not open stdf file");
		return EXIT_FAILURE;
	}
	if ((out=fopen(argv[x+1], "w")) == NULL) {
		perror("Could not open html file");
		return EXIT_FAILURE;
	}

	fprintf(out,
	        "<html>\n"
	        "<head>\n"
	        " <title>%s</title>\n"
	        " <style>\n"
	        "  table { border-collapse:collapse; font-family:monospace; }\n"
	        "  td { border: 1px solid #C0C0C0; text-align:center; }\n"
	        "  th { border: 1px solid black; text-align:center; }\n"
	        "  td.r1 { background-color: #DDDAEC; }\n"
	        "  td.r2 { background-color: #D4FFA9; }\n"
	        "  td.r3 { background-color: #FED0D4; }\n"
	        "  td.r4 { background-color: #FEFFC5; }\n"
	        "  span.headlen { font-weight:bolder; }\n"
	        "  span.headtype { font-style:italic; font-weight:bolder; }\n"
	        " </style>\n"
	        "</head>\n"
	        "<body>\n"
	        "<h1>File: %s<br>STDF v%i<br>CPU Type: %i (%s)</h1>\n"
	        "<table><tr>\n",
	        argv[x], argv[x], stdfver, byte_order, cpu_name);

	recbuff = (char*)malloc(sizeof(char) * (0x0FFFF + 4));
	for (type=1; type<3; type++) {
		lseek(in, 0, SEEK_SET);

		width = 0;
		rec_count = max_recs;
		rec_rot = 1;

		fprintf(out, "<td><table>\n<tr>");
		for (width=0; width<MAX_WIDTH; ++width)
			if (type == OUT_HEX)
				fprintf(out, "<th>%.2X</th>", width);
			else
				fprintf(out, "<th>%i</th>", width);
		fprintf(out, "</tr>\n<tr>");

		while (read(in, &h, 4) == 4) {
			if (byte_order != BYTE_ORDER)
				h.REC_LEN = bswap_16(h.REC_LEN);
			lseek(in, -4, SEEK_CUR);
			h.REC_LEN += 4;
			read(in, recbuff, h.REC_LEN);

			write_rec(out, &h, type);
			if (--rec_count == 0)
				break;
			if (++rec_rot > MAX_REC_STYLES)
				rec_rot = 1;
		}
		if (width != 0)
			fprintf(out, "</tr>\n");
		fprintf(out, "</table></td>\n");
	}
	free(recbuff);

	fprintf(out,
	        "</tr></table>\n"
	        "</body>\n"
	        "</html>");

	fclose(out);
	close(in);

	return EXIT_SUCCESS;
}
