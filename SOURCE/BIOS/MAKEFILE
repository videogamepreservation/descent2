#
# $Source: f:/miner/source/bios/rcs/makefile $
# $Revision: 1.20 $
# $Author: samir $
# $Date: 1995/06/12 18:37:24 $
#
# Makefile for bios functions
#

# What to install, and where to install it
INSTALL_TARGS = io.lib joy.h key.h mono.h mouse.h timer.h swift.h dpmi.h \
		ipx.h rbaudio.h	findfile.h error.h

# What to clean
CLEAN_TARGS = $(OBJDIR)\*.obj

# What libs to make
LIBS = io

# What test programs to make
TEST_PROGS = demo
# testj testk testm testms

# What object files for the lib
IO_OBJS = key.obj joy.obj mouse.obj timer.obj mono.obj \
	  joyc.obj dpmi.obj ipx.obj rbaudio.obj	findfile.obj error.obj

# What object files and libs for the test programs
DEMO_SRCS = cdrom.c
DEMO_LIBS = io.lib

#rules and targets common to all Miner makefiles
%include $(INIT)\makefile.def

### OPUS MKMF:  Do not remove this line!  Generated dependencies follow.

dpmi.obj: conio.h dos.h dpmi.h error.h i86.h malloc.h mono.h stdio.h \
	 stdlib.h string.h types.h

findfile.obj: findfile.h

ipx.obj: assert.h conio.h dos.h dpmi.h error.h fix.h i86.h ipx.h key.h \
	 malloc.h stdio.h stdlib.h string.h timer.h types.h

joyc.obj: conio.h dos.h dpmi.h fix.h i86.h joy.h mono.h stdio.h stdlib.h \
	 types.h

key.obj: conio.h dos.h dpmi.h error.h fix.h i86.h key.h stdio.h stdlib.h \
	 timer.h types.h

mono.obj: conio.h dos.h fix.h i86.h key.h stdarg.h stdio.h stdlib.h \
	 string.h types.h

mouse.obj: conio.h dos.h dpmi.h error.h fix.h i86.h mouse.h stdio.h \
	 stdlib.h string.h timer.h types.h

rbaudio.obj: dos.h dpmi.h error.h i86.h mono.h rbaudio.h string.h types.h

swift.obj: dos.h i86.h stddef.h stdio.h stdlib.h string.h swift.h \
	 swiftcfg.h

