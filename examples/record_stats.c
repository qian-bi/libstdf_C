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
	printf("Record %s appeared %li times\n", (char*)key, *((long*)value));
}
#endif

int main(int argc, char *argv[])
{
	stdf_file *f;
	char *recname;
	rec_unknown *rec;
#if HAVE_GLIB
	GHashTable *hash_table;
#endif
	long *stat, cnt;

	if (argc != 2) {
		printf("Need a file to open!\n");
		return -1;
	}

	f = stdf_open(argv[1]);
	if (!f) {
		perror("Could not open file");
		return 1;
	}

#if HAVE_GLIB
	hash_table = g_hash_table_new(g_str_hash, g_str_equal);
#endif
	cnt = 0;
	printf("Analyzing [%s]\n", argv[1]);
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
		stdf_free_record(rec);
	}
	stdf_close(f);
#if HAVE_GLIB
	g_hash_table_foreach(hash_table, print_stat, NULL);
	g_hash_table_destroy(hash_table);
#endif
	printf("%li records found\n", cnt);
	return 0;
}
