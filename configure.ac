AC_INIT([libstdf], [0.7], [vapier@gmail.com, stefan.brandner@gmx.at])
AC_CONFIG_AUX_DIR([build-aux])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_HEADER([config.h])

AM_INIT_AUTOMAKE([-Wall 1.12 dist-xz no-dist-gzip silent-rules])
AM_SILENT_RULES([yes])

AC_STDF_LIBTOOL

AC_PROG_CC
AC_C_CONST

AC_HEADER_STDC
AC_CHECK_HEADERS(m4_flatten([
	assert.h
	byteswap.h
	ctype.h
	endian.h
	errno.h
	fcntl.h
	features.h
	inttypes.h
	stdarg.h
	stdint.h
	stdio.h
	stdlib.h
	string.h
	time.h
	unistd.h
	cmplrs/host.h
	machine/endian.h
	sys/cdefs.h
	sys/endian.h
	sys/isa_defs.h
	sys/param.h
	sys/stat.h
	sys/types.h
]))

dnl a few vars for exported libstdf headers
dnl xxx: should make this into a macro with as_tr_sh/as_tr_cpp ...
test "x$ac_cv_header_stdint_h" = "xyes" && HAVE_STDINT_H=1 || HAVE_STDINT_H=0
AC_SUBST(HAVE_STDINT_H)
test "x$ac_cv_header_inttypes_h" = "xyes" && HAVE_INTTYPES_H=1 || HAVE_INTTYPES_H=0
AC_SUBST(HAVE_INTTYPES_H)
test "x$ac_cv_header_sys_types_h" = "xyes" && HAVE_SYS_TYPES_H=1 || HAVE_SYS_TYPES_H=0
AC_SUBST(HAVE_SYS_TYPES_H)
test "x$ac_cv_header_cmplrs_host_h" = "xyes" && HAVE_CMPLRS_HOST_H=1 || HAVE_CMPLRS_HOST_H=0
AC_SUBST(HAVE_CMPLRS_HOST_H)

AC_STDF_TYPE_CHECKS
AC_STDF_GCC_ATTRIBUTES

AM_STDF_VAPIER_DEV
AM_STDF_DEBUG_OPTIONS

dnl ********************************************************
dnl *                 SDTF v3 [stdf-ver3]                  *
dnl ********************************************************
enable_stdf_ver3_support="no"
AC_MSG_CHECKING(whether STDFv3 support should be enabled)
AC_ARG_ENABLE(stdf-ver3,
	AC_HELP_STRING([--enable-stdf-ver3],[enable support for STDFv3 @<:@default=v4 only@:>@]),
	[
	if test "x$enableval" = "xyes" ; then
		enable_stdf_ver3_support="yes"
	else
		enable_stdf_ver3_support="no"
	fi
	]
)
if test "x$enable_stdf_ver3_support" = "xyes" ; then
	AC_DEFINE(STDF_VER3, 1, [Enable support for STDFv3])
fi
AC_MSG_RESULT($enable_stdf_ver3_support)

dnl ********************************************************
dnl *                 Force endian types                   *
dnl ********************************************************
stdf_endian_force="auto"
AC_MSG_CHECKING(for endian type)
AC_ARG_ENABLE(endian,
	AC_HELP_STRING([--enable-endian],[force endian type @<:@default=detect@:>@]),
	[stdf_endian_force=$enableval])
if test "x$stdf_endian_force" = "xlittle" ; then
	AC_DEFINE(STDF_ENDIAN_FORCE, STDF_ENDIAN_LITTLE, [Force endian type to little])
elif test "x$stdf_endian_force" = "xbig" ; then
	AC_DEFINE(STDF_ENDIAN_FORCE, STDF_ENDIAN_BIG, [Force endian type to big])
elif test "x$stdf_endian_force" != "xauto" ; then
	AC_MSG_ERROR(Valid values for endian are "little", "big", or "auto")
fi
AC_MSG_RESULT($stdf_endian_force)
AC_C_BIGENDIAN

AM_STDF_ZZIPLIB
AM_STDF_LIBLZW
AM_STDF_GZIP
AM_STDF_BZIP2

dnl ********************************************************
dnl *                 Example Apps [examples]              *
dnl ********************************************************
enable_example_programs="yes"
AC_MSG_CHECKING(whether example programs should be built)
AC_ARG_ENABLE(examples,
	AC_HELP_STRING([--disable-examples],[disable building the examples @<:@default=build@:>@]),
	[
	if test "x$enableval" = "xyes" ; then
		enable_example_programs="yes"
	else
		enable_example_programs="no"
	fi
	]
)
AC_MSG_RESULT($enable_example_programs)
if test "x$enable_example_programs" = "xyes" ; then
	AC_CHECK_HEADERS([getopt.h])
	AM_CONDITIONAL(BUILD_EXAMPLES, true)
else
	AM_CONDITIONAL(BUILD_EXAMPLES, false)
fi

dnl ********************************************************
dnl *                     HASH [hash / ecore / glib]       *
dnl ********************************************************
if test "x$enable_example_programs" = "xyes" ; then
dnl First check for libhash ...
HASH_CFLAGS=""
HASH_LIBS=""
AC_CHECK_LIB(hash, hash_initialise,
	[
		AC_CHECK_HEADERS([hash.h], [HASH_LIBS="-lhash"])
	])
AC_SUBST(HASH_CFLAGS)
AC_SUBST(HASH_LIBS)

dnl If that fails, check for ecore ...
ECORE_LIBS=""
PKG_CHECK_MODULES(ECORE, ecore,
[
	AC_DEFINE(HAVE_ECORE, 1, [Has ecore support (used by example programs)])
	AM_CONDITIONAL(HAVE_ECORE, true)
],[
	AM_CONDITIONAL(HAVE_ECORE, false)
	ECORE_CFLAGS=""
	ECORE_LIBS=""
])
if test "x$HASH_LIBS" != "x" ; then
	ECORE_CFLAGS=""
	ECORE_LIBS=""
fi
AC_SUBST(ECORE_CFLAGS)
AC_SUBST(ECORE_LIBS)

dnl If that fails, check for glib ...
PKG_CHECK_MODULES(GLIB, glib-2.0 >= 2.0,
[
	AC_DEFINE(HAVE_GLIB, 1, [Has glib support (used by example programs)])
	AM_CONDITIONAL(HAVE_GLIB, true)
],[
	AM_CONDITIONAL(HAVE_GLIB, false)
	GLIB_CFLAGS=""
	GLIB_LIBS=""
])
if test "x$HASH_LIBS" != "x" -o "x$ECORE_LIBS" != "x" ; then
	GLIB_CFLAGS=""
	GLIB_LIBS=""
fi
AC_SUBST(GLIB_CFLAGS)
AC_SUBST(GLIB_LIBS)
fi

dnl ********************************************************
dnl *                        PNG                           *
dnl ********************************************************

PKG_CHECK_MODULES([libpng], [libpng12], [HAVE_LIBPNG=1], [HAVE_LIBPNG=0])
AM_CONDITIONAL([USE_LIBPNG], [test "$HAVE_LIBPNG" -eq 1])
AM_COND_IF([USE_LIBPNG],
			[AC_MSG_NOTICE([Using PNG for stdf2img])],
		   	[AC_MSG_WARN([libpng not found. Example stdf2img will not be created.])])


dnl ********************************************************
dnl *                     docs [doxygen]                   *
dnl ********************************************************
enable_doxygen="no"
AC_MSG_CHECKING(whether to build documentation with doxygen)
AC_ARG_ENABLE(docs,
	AC_HELP_STRING([--enable-docs],[generate a lot of API docs @<:@default=no@:>@]),
	[
	if test "x$enableval" = "xyes" ; then
		enable_doxygen="yes"
	else
		enable_doxygen="no"
	fi
	]
)
AC_MSG_RESULT($enable_doxygen)
if test "x$enable_doxygen" = "xyes" ; then
	AC_CHECK_PROG(enable_doxygen_detect, doxygen, yes, no)
	if test "x$enable_doxygen_detect" = "xno" ; then
		echo ""
		echo "documentation building was requested by doxygen was not found!"
		echo ""
		echo "You can grab it from http://www.doxygen.org/"
		echo ""
 		AC_MSG_ERROR([doxygen not found!])
	fi
	AM_CONDITIONAL(BUILD_DOCS, true)
else
	AM_CONDITIONAL(BUILD_DOCS, false)
fi




AC_CONFIG_FILES([
Makefile
include/libstdf_systems.h
include/Makefile
include/internal/Makefile
src/Makefile
tests/Makefile
doc/Makefile
doc/man/Makefile
doc/man/man3/Makefile
examples/Makefile
libstdf-config
libstdf.pc
libstdf-doc.c
])
AC_CONFIG_COMMANDS([config-perms], [chmod +x libstdf-config])
AC_OUTPUT
