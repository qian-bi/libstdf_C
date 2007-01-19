#!/bin/bash -ex

if test -d .svn ; then
	if test -d ../m4 ; then
		rm -rf m4
		mkdir -p m4
		cp ../m4/*.m4 m4/
	fi
	if test -d ../liblzw ; then
		cp ../liblzw/lzw.h include/
		cp ../liblzw/lzw.c src/
	fi
fi

autoreconf -i -f

# broken autotools wrongly clobbers our INSTALL
svn revert INSTALL >& /dev/null || true

if test -x ./test.sh ; then
	exec ./test.sh "$@"
fi
