#
# $Source: f:/miner/source/fix/rcs/makefile $
# $Revision: 1.6 $
# $Author: matt $
# $Date: 1995/12/21 14:00:19 $
#
# Makefile for fixed-point library
#

# What to install, and where to install it
INSTALL_TARGS = fix.lib fix.h fix.inc

# What to clean
CLEAN_TARGS = $(OBJDIR)\*.obj

# What libs to make
LIBS = fix

# What test programs to make
TEST_PROGS = fixtest

# What object files for the lib
FIX_OBJS = fix.obj

# What object files and libs for the test programs
FIXTEST_SRCS = fixtest.c
FIXTEST_LIBS = fix.lib io.lib

#rules and targets common to all Miner makefiles
%include $(INIT)\makefile.def

### OPUS MKMF:  Do not remove this line!  Generated dependencies follow.

asingen.obj: math.h stdio.h

fix.obj: fix.inc psmacros.inc pstypes.inc

fixtest.obj: conio.h fix.h graph.h pstypes.h stdio.h stdlib.h types.h

singen.obj: math.h

test.obj: psmacros.inc
