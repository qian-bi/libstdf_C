if BUILD_EXAMPLES
extra_examples  =
if USE_LIBPNG
extra_examples += stdf2img
endif
noinst_PROGRAMS = \
	example \
	record_stats \
	dump_records_to_ascii \
	dump_records_to_html \
	is_valid_stdf \
	stdf2xtdf \
	makestdf \
	extract_records \
	read_write \
	stdf2wafermap \
	$(extra_examples)
else
noinst_PROGRAMS =
endif

AM_CPPFLAGS = \
	-I$(top_srcdir)/include -I$(top_builddir)/include \
	$(HASH_CFLAGS) $(ECORE_CFLAGS) $(GLIB_CFLAGS) \
	$(LIBPNG_CPPFLAGS)

LDADD = $(top_builddir)/src/libstdf.la
record_stats_LDADD = $(HASH_LIBS) $(ECORE_LIBS) $(GLIB_LIBS) $(LDADD)
if USE_LIBPNG
stdf2img_LDADD = $(libpng_LIBS) $(LDADD)
endif
