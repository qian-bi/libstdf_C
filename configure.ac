AC_INIT
AM_INIT_AUTOMAKE(libstdf, 0.1)
AM_CONFIG_HEADER(include/libstdf_sys_config.h)

AC_PROG_CC
AC_PROG_CXX
AM_PROG_LIBTOOL
AM_ENABLE_SHARED
AM_ENABLE_STATIC

AC_CHECK_HEADERS([endian.h sys/endian.h bswap.h byteswap.h])

CFLAGS="$CFLAGS -Wall -Werror"

enable_stdf_ver3_support="no"
AC_ARG_ENABLE(stdf-ver3, [  --enable-stdf-ver3           enable support for version 3 of STDF \[default=no\]], [
	if test "$enableval" = "yes" ; then
		AC_MSG_RESULT(yes)
		enable_stdf_ver3_support="yes"
	else
		AC_MSG_RESULT(no)
		enable_stdf_ver3_support="no"
	fi
	],[
	AC_MSG_RESULT($enable_stdf_ver3_support)
	]
)
if test "x$enable_stdf_ver3_support" = "xyes" ; then
	AC_DEFINE(STDF_VER3, 1, "Enable support for STDFv3")
fi

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
