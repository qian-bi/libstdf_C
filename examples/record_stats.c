/**
 * @file record_stats.c
 */
/*
 * Copyright (C) 2004-2007 Mike Frysinger <vapier@gmail.com>
 * Released under the BSD license.  For more information,
 * please see: http://opensource.org/licenses/bsd-license.php
 */

#include <config.h>
#include <libstdf.h>

#include <internal/headers.h>

/*
 * libhash
 */
#if defined(HAVE_HASH_H)
#include <hash.h>
#define HASH_VARS \
	void *key, *val; \
	hash hash_table; \
	struct hash_iterator hit;
#define HASH_INIT \
	hash_initialise(&hash_table, 61, \
	                hash_hash_string, hash_compare_string, hash_copy_string, \
	                free, free);
#define HASH_UPDATE(recname) \
	key = (void*)recname; \
	if (!hash_retrieve(&hash_table, key, (void**)&val)) { \
		val = malloc(sizeof(long)); \
		*(long*)val = 0; \
		hash_insert(&hash_table, key, val); \
	} \
	(*(long*)val)++;
#define HASH_PRINT \
	key = NULL; \
	hash_iterator_initialise(&hash_table, &hit); \
	while (hash_fetch_next(&hash_table, &hit, (void **)&key, (void **)&val)) \
		printf("\t%s : %li\n", (char*)key, *(long*)val); \
	hash_iterator_deinitialise(&hash_table, &hit); \
	hash_deinitialise(&hash_table);

/*
 * Ecore
 */
#elif defined(HAVE_ECORE)
#include <Ecore.h>
#define HASH_VARS \
	Ecore_Hash *hash_table; \
	long *stat;
#define HASH_INIT \
	hash_table = ecore_hash_new(ecore_str_hash, ecore_str_compare);
#define HASH_UPDATE(recname) \
	stat = ecore_hash_get(hash_table, recname); \
	if (!stat) { \
		stat = malloc(sizeof(long)); \
		*stat = 0; \
		ecore_hash_set(hash_table, strdup(recname), stat); \
	} \
	(*stat)++;
#define HASH_PRINT \
	ecore_hash_for_each_node(hash_table, print_stat, NULL); \
	ecore_hash_destroy(hash_table);
void print_stat(void *value, void *user_data)
{
	Ecore_Hash_Node *node = ECORE_HASH_NODE(value);
	printf("\t%s : %li\n", (char*)(node->key), *((long*)(node->value)));
}

/*
 * glib
 */
#elif defined(HAVE_GLIB)
#include <glib.h>
#define HASH_VARS \
	GHashTable *hash_table; \
	long *stat;
#define HASH_INIT \
	hash_table = g_hash_table_new(g_str_hash, g_str_equal);
#define HASH_UPDATE(recname) \
	stat = g_hash_table_lookup(hash_table, recname); \
	if (!stat) { \
		stat = malloc(sizeof(long)); \
		*stat = 0; \
		g_hash_table_insert(hash_table, g_strdup(recname), stat); \
	} \
	(*stat)++;
#define HASH_PRINT \
	g_hash_table_foreach(hash_table, print_stat, NULL); \
	g_hash_table_destroy(hash_table);
void print_stat(gpointer key, gpointer value, gpointer user_data)
{
	(void)user_data;
	printf("\t%s : %li\n", (char*)key, *((long*)value));
}

#else
#define HASH_VARS
#define HASH_INIT
#define HASH_UPDATE(recname) if (recname) {}
#define HASH_PRINT
#endif

int main(int argc, char *argv[])
{
	stdf_file *f;
	char *recname;
	stdf_rec_unknown *rec;
	long cnt;
	int i;
	HASH_VARS

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

	HASH_INIT
	cnt = 0;
	while ((rec=stdf_read_record(f)) != NULL) {
		recname = stdf_get_rec_name(rec->header.REC_TYP, rec->header.REC_SUB);
		HASH_UPDATE(recname)
		cnt++;
		stdf_free_record(rec);
	}
	stdf_close(f);
	HASH_PRINT

	printf("\tTOTAL : %li\n", cnt);
}
	return EXIT_SUCCESS;
}
