if BUILD_EXAMPLES
noinst_PROGRAMS = \
	example \
	record_stats \
	dump_records_to_ascii \
	dump_records_to_html \
	is_valid_stdf \
	stdf2xtdf \
	makestdf \
	extract_records
else
noinst_PROGRAMS =
endif

INCLUDES = -I$(top_builddir)/include @ECORE_CFLAGS@ @GLIB_CFLAGS@

LDADD = $(top_builddir)/src/libstdf.la
record_stats_LDADD = @HASH_LIBS@ @ECORE_LIBS@ @GLIB_LIBS@ $(LDADD)
