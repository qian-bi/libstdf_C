dnl Wrap the calls to AC_TYPE_* macros incase the host autoconf
dnl is too old and lacks the macro

dnl AC_STDF_TYPE_CHECK([standard macro], [desired type], [fallback type])
AC_DEFUN([_AC_STDF_TYPE_CHECK],
[
	m4_ifndef([$1], [AC_CHECK_TYPE([$2], [$3])], [$1])
])

AC_DEFUN([AC_STDF_TYPE_CHECKS],
[
	_AC_STDF_TYPE_CHECK([AC_TYPE_SIZE_T],    [size_t],    [unsigned long])
	_AC_STDF_TYPE_CHECK([AC_TYPE_SSIZE_T],   [ssize_t],   [signed long])
	_AC_STDF_TYPE_CHECK([AC_TYPE_UINTPTR_T], [uintptr_t], [size_t])
])
