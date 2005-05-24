# Howto build:
# from a command prompt, cd to the libstdf/win32 dir.
# then run 'nmake -f libstdf.mak'
# to build examples 'nmake -f libstdf.mak examples'

STATICLIB	= libstdf.lib
SHAREDLIB	= libstdf.dll
IMPORTLIB	= libstdf-import.lib

CC			= cl.exe
LD			= link.exe
AR			= lib.exe
RC			= rc.exe

CFLAGS		= /nologo /MD /W3 /O2 /I ../include /DWIN32
LDFLAGS		= /nologo /release /machine:I386
ARFLAGS		= /nologo
RCFLAGS		= /r /dNDEBUG

DEFFILE		= libstdf.def
RCFILE		= libstdf.rc
RESFILE		= libstdf.res
OBJS		= ../src/dtc.obj ../src/libstdf.obj ../src/rec.obj

all: $(STATICLIB) $(SHAREDLIB)

$(STATICLIB): $(OBJS) $(DEFFILE)
	$(AR) $(ARFLAGS) /out:$(STATICLIB) $(OBJS)

$(SHAREDLIB): $(OBJS) $(DEFFILE) $(RESFILE)
	$(LD) $(LDFLAGS) /def:$(DEFFILE) /dll /implib:$(IMPORTLIB) /out:$(SHAREDLIB) $(OBJS) $(RESFILE)

.c.obj:
	$(CC) $(CFLAGS) /c $< /Fo$@

$(RESFILE): $(RCFILE)
	$(RC) $(RCFLAGS) /fo $(RESFILE) $(RCFILE)

EXAMPLES	= ../examples/dump_records_to_ascii.exe ../examples/dump_records_to_html.exe ../examples/example.exe ../examples/is_valid_stdf.exe ../examples/makestdf.exe ../examples/record_stats.exe ../examples/stdf2xtdf.exe

examples: $(EXAMPLES)
.c.exe:
	$(CC) $(CFLAGS) $(IMPORTLIB) $<

clean:
	-del $(STATICLIB)
	-del $(SHAREDLIB)
	-del $(IMPORTLIB)
	-del ..\src\*.obj
	-del *.exe
	-del $(RESFILE)
