AC_DEFUN([AC_STDF_LIBTOOL],
[
dnl Next four lines is a hack to prevent libtool checking for CXX/F77
m4_undefine([AC_PROG_CXX])
m4_defun([AC_PROG_CXX],[])
m4_undefine([AC_PROG_F77])
m4_defun([AC_PROG_F77],[])
AC_LIBTOOL_WIN32_DLL
AM_PROG_AR
LT_INIT
AC_ENABLE_SHARED
AC_ENABLE_STATIC
])
