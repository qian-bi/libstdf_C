#!/bin/bash
# Copyright (C) 2004 Mike Frysinger <vapier@gmail.com>
# Released under the BSD license.  For more information,
# please see: http://opensource.org/licenses/bsd-license.php
#
# $Header$

name=libstdf
NAME="`echo ${name} | tr '[a-z]' '[A-Z]'`"

dsp=${PWD}/${name}.dsp
dsw=${PWD}/${name}.dsw
def=${PWD}/${name}.def
srcdir=../..

cat << EOF > ${dsp}
# Microsoft Developer Studio Project File - Name="${name}" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=${name} - Win32 Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "${name}.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "${name}.mak" CFG="${name} - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "${name} - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "${NAME}_EXPORTS" /FD /c
# ADD CPP /nologo /Gz /MD /W3 /GX /O2 /I "..\include\" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "${NAME}_EXPORTS" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# Begin Target

# Name "${name} - Win32 Release"
EOF

cat << EOF >> ${dsp}
# Begin Group "Source Files"

# PROP Default_Filter "c;rc;def"
# Begin Source File
SOURCE=${name}.def
# End Source File
EOF
for f in ${srcdir}/src/*.c ; do
cat << EOF >> ${dsp}
# Begin Source File
SOURCE=${f//\//\\}
# End Source File
EOF
done
cat << EOF >> ${dsp}
# End Group
EOF

cat << EOF >> ${dsp}
# Begin Group "Header Files"

# PROP Default_Filter "h"
EOF
for f in ${srcdir}/src/*.h ; do
cat << EOF >> ${dsp}
# Begin Source File
SOURCE=${f//\//\\}
# End Source File
EOF
done
cat << EOF >> ${dsp}
# End Group
EOF

cat << EOF >> ${dsp}
# End Target
# End Project
EOF
unix2dos ${dsp}


cat << EOF > ${dsw}
Microsoft Developer Studio Workspace File, Format Version 6.00
Project: "${name}"="${name}.dsp"
EOF
unix2dos ${dsw}


cat << EOF > ${def}
EXPORTS
$(grep ^extern ${srcdir}/include/libstdf_funcs.h | awk '{print $3}' | cut -d\( -f1)
EOF
unix2dos ${def}
