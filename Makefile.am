SUBDIRS = doc include src examples tests

bin_SCRIPTS = libstdf-config

WIN32_EXTRA_DIST = \
	win32/Makefile.mingw win32/libstdf.def win32/libstdf.dsp \
	win32/libstdf.dsw win32/libstdf.mak win32/libstdf.rc
EXTRA_DIST = AUTHORS COPYING ChangeLog INSTALL NEWS README $(WIN32_EXTRA_DIST)

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = libstdf.pc

install-data-hook:
if BUILD_DOCS
	doxygen
	$(INSTALL) -d $(DESTDIR)/$(pkgdatadir)/
	cp -pR $(top_srcdir)/doc/html/* $(DESTDIR)/$(pkgdatadir)/
endif

ACLOCAL_AMFLAGS = -I m4
