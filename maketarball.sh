#!/bin/bash

trap ":" INT QUIT TSTP

[ -z "$1" ] \
	&& VER=test \
	|| VER=$1

cd ..
rm -rf libstdf-${VER}
cp -r libstdf libstdf-${VER} || exit 1
cd libstdf-${VER} || exit 1
./clean.sh || exit 1
rm -rf clean.sh maketarball.sh test.sh
./autogen.sh || exit 1
rm -rf autom4te.cache `find -name CVS`

if [ "${VER}" == "test" ] ; then
	./configure && make -j
else
	cd ..

	echo -e "\n\n\n"
	tar -jcf libstdf-${VER}.tar.bz2 libstdf-${VER}
	ls -l libstdf-${VER}.tar.bz2
	md5sum libstdf-${VER}.tar.bz2

	echo -e "\n\n\n"
	echo "https://sourceforge.net/project/admin/editpackages.php?group_id=56856"

	echo -e "\n\n\n"
	echo "ftp -p upload.sourceforge.net"
	echo "cd /incoming"
	echo "lcd .."
	echo "put libstdf-${VER}.tar.bz2"
fi
