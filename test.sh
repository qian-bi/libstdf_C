#!/bin/sh

./configure --disable-static "$@" && make -j
