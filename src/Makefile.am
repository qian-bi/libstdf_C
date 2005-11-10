noinst_HEADERS = dtc.h rec.h

INCLUDES = -I$(top_builddir)/include \
	@ZIP_CFLAGS@ @GZIP_CFLAGS@ @BZIP2_CFLAGS@

lib_LTLIBRARIES = libstdf.la

libstdf_la_SOURCES = \
	libstdf.c \
	dtc.c \
	rec.c
libstdf_la_LDFLAGS = -version-info 0:1:0

libstdf_la_LIBADD = @ZIP_LIBS@ @GZIP_LIBS@ @BZIP2_LIBS@
