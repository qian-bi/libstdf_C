#!/bin/bash

trap ":" INT QUIT TSTP

[ -z "${VER}" ] \
	&& VER=test \
	|| VER=$1

cd ..
rm -rf libstdf-${VER}
cp -r libstdf libstdf-${VER} || exit 1
cd libstdf-${VER} || exit 1
./clean.sh || exit 1
./autogen.sh || exit 1
rm -rf `find -name CVS`
rm -rf clean.sh maketarball.sh test.sh

if [ "${VER}" == "test" ] ; then
	./configure && make -j
else
	cd ..

	tar -jcf libstdf-${VER}.tar.bz2 libstdf-${VER}

	ls -l libstdf-${VER}.tar.bz2
	md5sum libstdf-${VER}.tar.bz2
fi
