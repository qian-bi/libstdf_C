#!/bin/bash -ex

if [ -d .git ] ; then
	if [ -d ../liblzw ] ; then
		cp ../liblzw/lzw.h include/
		cp ../liblzw/lzw.c src/
	fi
fi

autoreconf -i -f

# broken autotools wrongly clobbers our INSTALL
git checkout -f INSTALL || true
