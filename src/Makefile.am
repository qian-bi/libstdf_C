noinst_HEADERS = dtc.h rec.h

# Internal macro to define internal libstdf stuff
AM_CPPFLAGS = -D__IN_LIBSTDF
AM_CFLAGS   = @DEBUG_CFLAGS@

INCLUDES = -I$(top_builddir)/include \
	@ZIP_CFLAGS@ @GZIP_CFLAGS@ @BZIP2_CFLAGS@

lib_LTLIBRARIES = libstdf.la
EXTRA_STDF_SOURCES  =
if HAVE_LZW
EXTRA_STDF_SOURCES += lzw.c
endif
libstdf_la_SOURCES = \
	libstdf.c \
	dtc.c \
	rec.c \
	$(EXTRA_STDF_SOURCES)
libstdf_la_CFLAGS = -no-suppress
libstdf_la_LDFLAGS = -no-suppress -version-info 0:1:0

libstdf_la_LIBADD = @ZIP_LIBS@ @GZIP_LIBS@ @BZIP2_LIBS@ @DEBUG_LIBS@
