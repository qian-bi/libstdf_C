#!/bin/bash -ex

autoreconf -i -f

# broken autotools wrongly clobbers our INSTALL
git checkout -f INSTALL || true
