dnl
dnl AC_STDF_GCC_ATTR_CHECK(define suffix, checking display, compile global, compile body)
dnl
AC_DEFUN([AC_STDF_GCC_ATTR_CHECK],
[dnl The usage of -Werror is catch things where gcc will simply warn about the attribute not being supported
	AC_MSG_CHECKING(if the compiler supports __attribute__ (( [$2] )) )
	save_CFLAGS="$CFLAGS"
	CFLAGS="$CFLAGS -Wattributes -Werror"
	AC_TRY_COMPILE([$3], [$4], [stdf_gcc_attr="yes"], [stdf_gcc_attr="no"])
	if test x"$stdf_gcc_attr" = xyes ; then
		AC_DEFINE(STDF_HAVE_GCC_ATTRIBUTE_$1, 1, [compiler supports __attribute__ (($2))])
		STDF_HAVE_GCC_ATTRIBUTE_$1="1"
		STDF_GCC_ATTRIBUTE_$1="__attribute__ (($2))"
	else
		STDF_HAVE_GCC_ATTRIBUTE_$1="0"
		STDF_GCC_ATTRIBUTE_$1=""
	fi
	CFLAGS="$save_CFLAGS"
	AC_SUBST(STDF_HAVE_GCC_ATTRIBUTE_$1)
	AC_SUBST(STDF_GCC_ATTRIBUTE_$1)
	AC_MSG_RESULT($stdf_gcc_attr)
])
AC_DEFUN([AC_STDF_GCC_ATTRIBUTES],
[dnl
dnl ********************************************************
dnl *                    gcc attributes                    *
dnl ********************************************************
AC_STDF_GCC_ATTR_CHECK([UNUSED],
	[__unused__],
	[],
	[int i __attribute__ ((__unused__))])
AC_STDF_GCC_ATTR_CHECK([CONST],
	[__const__],
	[__attribute__((__const__)) void stdf_foo_moo(void) {}],
	[])
AC_STDF_GCC_ATTR_CHECK([VISIBILITY],
	[__visibility__("hidden")],
	[__attribute__((__visibility__("hidden"))) void stdf_foo_moo(void) {}],
	[])
AC_STDF_GCC_ATTR_CHECK([STRONG_ALIAS],
	[__alias__("symbol")],
	[int stdf_foo(void) {} extern int stdf_moo(void) __attribute__ (( __alias__ ("stdf_foo")));],
	[int i = stdf_moo();])
AC_STDF_GCC_ATTR_CHECK([WEAK_ALIAS],
	[__weak__, __alias__("symbol")],
	[int stdf_foo(void) {} extern int stdf_moo(void) __attribute__ (( __weak__, __alias__ ("stdf_foo")));],
	[int i = stdf_moo();])
])
