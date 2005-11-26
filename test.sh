#!/bin/sh

./configure \
	--disable-static \
	--enable-stdf-ver3 \
	--enable-lzw \
	"$@" \
	|| exit 1
make -j || exit 1
