AC_INIT
AC_CANONICAL_BUILD
AC_CANONICAL_HOST
AC_CANONICAL_TARGET
AM_INIT_AUTOMAKE(libstdf, 0.1)

AC_PROG_CC
AC_PROG_CXX
AM_ENABLE_SHARED
AM_PROG_LIBTOOL

CFLAGS="$CFLAGS -Wall -Werror"

PKG_CHECK_MODULES(GLIB, glib-2.0 >= 2.0,
	[
		AC_DEFINE(HAVE_GLIB, 1, [Has glib support (used by example programs)])
		AM_CONDITIONAL(HAVE_GLIB, true)
		AC_SUBST(GLIB_CFLAGS)
		AC_SUBST(GLIB_LIBS)
	],
	[ AM_CONDITIONAL(HAVE_GLIB, false) ])

AC_OUTPUT([
Makefile
include/Makefile
src/Makefile
examples/Makefile
libstdf-config
libstdf.pc
],[
chmod +x libstdf-config
])
