AC_DEFUN([AM_STDF_VAPIER_DEV],
[dnl Let's get anal with compiling while on my dev box
case $HOSTNAME in
	vapier*) CFLAGS="$CFLAGS -Wall -Wextra -Wnonnull -Wundef -Werror";;
esac
])

