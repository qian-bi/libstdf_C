/* record_stats.c
 * Copyright (C) 2004 Mike Frysinger <vapier@gmail.com>
 * Released under the BSD license.  For more information,
 * please see: http://opensource.org/licenses/bsd-license.php
 *
 * $Header$
 */

#include <libstdf.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#if HAVE_GLIB
#include <glib.h>
#endif

#if HAVE_GLIB
void print_stat(gpointer key, gpointer value, gpointer user_data)
{
	printf("\t%s : %li\n", (char*)key, *((long*)value));
}
#endif

int main(int argc, char *argv[])
{
	stdf_file *f;
	char *recname;
	rec_unknown *rec;
#if HAVE_GLIB
	GHashTable *hash_table;
	long *stat;
#endif
	long cnt;
	int i;

	if (argc <= 1) {
		printf("Need some files to open!\n");
		return EXIT_FAILURE;
	}

for (i=1; i<argc; ++i) {
	printf("Analyzing %s\n", argv[i]);
	f = stdf_open(argv[i]);
	if (!f) {
		perror("Could not open file");
		continue;
	}

#if HAVE_GLIB
	hash_table = g_hash_table_new(g_str_hash, g_str_equal);
#endif
	cnt = 0;
	while ((rec=stdf_read_record(f)) != NULL) {
		recname = stdf_get_rec_name(rec->header.REC_TYP, rec->header.REC_SUB);
#if HAVE_GLIB
		stat = g_hash_table_lookup(hash_table, recname);
		if (!stat) {
			stat = (long*)malloc(sizeof(long));
			*stat = 0;
			g_hash_table_insert(hash_table, g_strdup(recname), stat);
		}
		(*stat)++;
#endif
		cnt++;
		stdf_free_record(f, rec);
	}
	stdf_close(f);
#if HAVE_GLIB
	g_hash_table_foreach(hash_table, print_stat, NULL);
	g_hash_table_destroy(hash_table);
#endif
	printf("\tTOTAL : %li\n", cnt);
}
	return EXIT_SUCCESS;
}
