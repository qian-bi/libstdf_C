#!/bin/sh
aclocal || exit 1
autoconf || exit 1
automake -a || exit 1

./configure "$@" && make -j
