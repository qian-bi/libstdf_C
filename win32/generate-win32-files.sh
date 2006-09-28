#!/bin/bash
# Copyright (C) 2004-2006 Mike Frysinger <vapier@gmail.com>
# Released under the BSD license.  For more information,
# please see: http://opensource.org/licenses/bsd-license.php

name=libstdf
NAME="`echo ${name} | tr '[a-z]' '[A-Z]'`"

dsp="${name}.dsp"
dsw="${name}.dsw"
def="${name}.def"
mak="${name}.mak"
rc="${name}.rc"
srcdir=..

####################################################################
# DSP
####################################################################
sed \
	-e "s:PROJ_LCASE_NAME:${name}:g" \
	-e "s:PROJ_UCASE_NAME:${NAME}:g" \
	./in/"${dsp}".in > "${dsp}"
src_files="
# Begin Source File
SOURCE=${name}.def
# End Source File"
for f in "${srcdir}"/src/*.c ; do
src_files="${src_files}
# Begin Source File
SOURCE=${f//\//\\}
# End Source File"
done
header_files=""
for f in "${srcdir}"/src/*.h ; do
header_files="${header_files}
# Begin Source File
SOURCE=${f//\//\\}
# End Source File
"
done
cat << EOF >> "${dsp}"
# Begin Group "Source Files"

# PROP Default_Filter "c;rc;def"
${src_files}
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h"
${header_files}
# End Group
# End Target
# End Project
EOF
unix2dos "${dsp}"


####################################################################
# DSW
####################################################################
sed \
	-e "s:PROJ_LCASE_NAME:${name}:g" \
	-e "s:PROJ_DSP_FILE:$(basename ${dsp}):g" \
	./in/"${dsw}".in > "${dsw}"
unix2dos "${dsw}"


####################################################################
# DEF
####################################################################
cat << EOF > "${def}"
EXPORTS
$(grep ^extern "${srcdir}"/include/libstdf_funcs.h | awk '{print $3}' | cut -d\( -f1)
EOF
unix2dos "${def}"


####################################################################
# MAK
####################################################################
src_files="$(echo ${srcdir}/src/*.c | sed 's:\.c:.obj:g')"
exe_files="$(echo ${srcdir}/examples/*.c | sed 's:\.c:.exe:g')"
sed \
	-e "s:PROJ_LCASE_NAME:${name}:g" \
	-e "s:PROJ_SRC_FILES:${src_files}:g" \
	-e "s:PROJ_EXE_FILES:${exe_files}:g" \
	-e "s:PROJ_INC_DIR:${srcdir}/include:g" \
	./in/"${mak}".in > "${mak}"
unix2dos "${mak}"


####################################################################
# RC
####################################################################
ver=$(head -n 1 "${srcdir}"/configure.ac | cut -d, -f2)
ver_mangled=$(echo ${ver} | sed 's:\.:,:g'),0,0
sed \
	-e "s:PROJ_LCASE_NAME:${name}:g" \
	-e "s:PROJ_VER_MANGLED:${ver_mangled}:" \
	-e "s:PROJ_VER:${ver}:" \
	./in/"${rc}".in > "${rc}"
unix2dos "${rc}"
