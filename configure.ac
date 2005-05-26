AC_INIT(libstdf, 0.3, [vapier@gmail.com])
AM_INIT_AUTOMAKE
AC_CONFIG_HEADER(include/libstdf_sys_config.h)

AC_PROG_CC
dnl Next four lines is a hack to prevent libtool checking for CXX/F77
m4_undefine([AC_PROG_CXX])
m4_defun([AC_PROG_CXX],[])
m4_undefine([AC_PROG_F77])
m4_defun([AC_PROG_F77],[])
AC_PROG_LIBTOOL
AC_ENABLE_SHARED
AC_ENABLE_STATIC

AC_CHECK_HEADERS([ \
stdint.h inttypes.h sys/types.h sys/stat.h \
unistd.h fcntl.h stdio.h stdlib.h \
string.h endian.h sys/endian.h machine/endian.h \
cmplrs/host.h sys/isa_defs.h \
])

dnl Let's get anal with compiling while on my dev box
if test "x$HOSTNAME" = "xvapier" ; then
	CFLAGS="$CFLAGS -Wall -Werror -fbounds-checking"
fi

dnl ********************************************************
dnl *                 Disable Warnings                     *
dnl ********************************************************
enable_annoy_untested="yes"
AC_MSG_CHECKING(whether to annoy you about untested code)
AC_ARG_ENABLE(warn-untested,
	AC_HELP_STRING([--enable-warn-untested],[don't warn about untested code @<:@default=warn@:>@]),
	[
	if test "x$enableval" = "xyes" ; then
		enable_annoy_untested="yes"
	else
		enable_annoy_untested="no"
	fi
	]
)
if test "x$enable_annoy_untested" = "xyes" ; then
	AC_DEFINE(WARN_UNTESTED, 1, [Annoy people about unimplemented code])
fi
AC_MSG_RESULT($enable_annoy_untested)

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
stdf_force_endian="auto"
AC_MSG_CHECKING(for endian type)
AC_ARG_ENABLE(endian,
	AC_HELP_STRING([--enable-endian],[force endian type @<:@default=detect@:>@]),
	[stdf_force_endian=$enableval])
if test "x$stdf_force_endian" = "xlittle" ; then
	AC_DEFINE(STDF_FORCE_ENDIAN, 1234, [Force endian size to little])
elif test "x$stdf_force_endian" = "xbig" ; then
	AC_DEFINE(STDF_FORCE_ENDIAN, 4321, [Force endian size to big])
elif test "x$stdf_force_endian" != "xauto" ; then
	AC_MSG_ERROR(Valid values for endian are "little", "big", or "auto")
fi
AC_MSG_RESULT($stdf_force_endian)

dnl ********************************************************
dnl *                     zip [zziplib]                    *
dnl ********************************************************
enable_zip_compression="maybe"
AC_MSG_CHECKING(whether zip support should be enabled)
AC_ARG_ENABLE(zip,
	AC_HELP_STRING([--enable-zip],[enable support for zip (zlib) @<:@default=auto@:>@]),
	[
	if test "x$enableval" = "xyes" ; then
		enable_zip_compression="yes"
	else
		enable_zip_compression="no"
	fi
	]
)
AC_MSG_RESULT($enable_zip_compression)
zip_found="no"
if test "x$enable_zip_compression" != "xno" ; then
	PKG_CHECK_MODULES(ZZIPLIB, zziplib >= 0.13,
		[
			zip_found="yes"
			AC_DEFINE(HAVE_ZIP, 1, [Has zip support])
			AM_CONDITIONAL(HAVE_ZIP, true)
			ZIP_CFLAGS="${ZZIPLIB_CFLAGS}"
			ZIP_LIBS="${ZZIPLIB_LIBS}"
		], [ dontdie="" ])
fi
if test "x$zip_found" = "xno" ; then
	if test "x$enable_zip_compression" = "xyes" ; then
		echo ""
		echo "zip support was requested but zziplib was not found!"
		echo ""
		echo "You can grab it from http://zziplib.sourceforge.net/"
		echo ""
		AC_MSG_ERROR([zziplib not found!])
	fi
	AM_CONDITIONAL(HAVE_ZIP, false)
	ZIP_CFLAGS=""
	ZIP_LIBS=""
fi
AC_SUBST(ZIP_CFLAGS)
AC_SUBST(ZIP_LIBS)

dnl ********************************************************
dnl *                     gzip [zlib]                      *
dnl ********************************************************
enable_gzip_compression="maybe"
AC_MSG_CHECKING(whether gzip support should be enabled)
AC_ARG_ENABLE(gzip,
	AC_HELP_STRING([--enable-gzip],[enable support for gzip (zlib) @<:@default=auto@:>@]),
	[
	if test "x$enableval" = "xyes" ; then
		enable_gzip_compression="yes"
	else
		enable_gzip_compression="no"
	fi
	]
)
AC_MSG_RESULT($enable_gzip_compression)
gzip_found="no"
if test "x$enable_gzip_compression" != "xno" ; then
	AC_CHECK_LIB(z, uncompress,
		[
			AC_CHECK_HEADERS([zlib.h],
				[
					gzip_found="yes"
					AC_DEFINE(HAVE_GZIP, 1, [Has gzip support])
					AM_CONDITIONAL(HAVE_GZIP, true)
					GZIP_CFLAGS=""
					GZIP_LIBS="-lz"
				])
		])
fi
if test "x$gzip_found" = "xno" ; then
	if test "x$enable_gzip_compression" = "xyes" ; then
		echo ""
		echo "gzip support was requested but zlib was not found!"
		echo ""
		echo "You can grab it from http://www.gzip.org/zlib/"
		echo ""
		AC_MSG_ERROR([zlib not found!])
	fi
	AM_CONDITIONAL(HAVE_GZIP, false)
	GZIP_CFLAGS=""
	GZIP_LIBS=""
fi
AC_SUBST(GZIP_CFLAGS)
AC_SUBST(GZIP_LIBS)

dnl ********************************************************
dnl *                     bzip2                            *
dnl ********************************************************
enable_bzip2_compression="maybe"
AC_MSG_CHECKING(whether bzip2 support should be enabled)
AC_ARG_ENABLE(bzip2,
	AC_HELP_STRING([--enable-bzip2],[enable support for bzip2 files @<:@default=auto@:>@]),
	[
	if test "x$enableval" = "xyes" ; then
		enable_bzip2_compression="yes"
	else
		enable_bzip2_compression="no"
	fi
	]
)
AC_MSG_RESULT($enable_bzip2_compression)
bzip2_found="no"
if test "x$enable_bzip2_compression" != "xno" ; then
	AC_CHECK_LIB(bz2, BZ2_bzRead,
		[
			AC_CHECK_HEADERS([bzlib.h],
				[
					bzip2_found="yes"
					AC_DEFINE(HAVE_BZIP2, 1, [Has bzip2 support])
					AM_CONDITIONAL(HAVE_BZIP2, true)
					BZIP2_CFLAGS=""
					BZIP2_LIBS="-lbz2"
				])
		])
fi
if test "x$bzip2_found" = "xno" ; then
	if test "x$enable_bzip2_compression" = "xyes" ; then
		echo ""
		echo "bzip2 support was requested but bzlib was not found!"
		echo ""
		echo "You can grab it from http://sources.redhat.com/bzip2/"
		echo ""
 		AC_MSG_ERROR([bzlib not found!])
	fi
	AM_CONDITIONAL(HAVE_BZIP2, false)
	BZIP2_CFLAGS=""
	BZIP2_LIBS=""
fi
AC_SUBST(BZIP2_CFLAGS)
AC_SUBST(BZIP2_LIBS)

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
HASH_LIBS=""
AC_CHECK_LIB(fhash, hash_initialise,
	[
		AC_CHECK_HEADERS([hash.h], [HASH_LIBS="-lhash"])
	])
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





AC_OUTPUT([
Makefile
include/Makefile
src/Makefile
doc/Makefile
doc/man/Makefile
examples/Makefile
libstdf-config
libstdf.pc
libstdf-doc.c
],[
chmod +x libstdf-config
])
