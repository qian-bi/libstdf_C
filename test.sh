#!/bin/bash -ex

doit() {
	$d/configure \
		--disable-static \
		--enable-stdf-ver3 \
		--enable-lzw \
		"$@"
	make clean
	make -j
	make check
	[[ $d == "." ]] && make distcheck
	make distclean
}

# build up in src tree
d=. doit "$@"

# make sure out of tree building works
rm -rf build
mkdir build
cd build
d=.. doit "$@"
