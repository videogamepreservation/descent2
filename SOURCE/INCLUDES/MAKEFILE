#
# $Source: f:/miner/source/includes/rcs/makefile $
# $Revision: 1.2 $
# $Author: matt $
# $Date: 1995/12/21 13:59:42 $
#
# Makefile for macro and header files
#

# What to install
INSTALL_TARGS = pstypes.h pstypes.inc psmacros.inc

# What to clean
CLEAN_TARGS = $(OBJDIR)\*.obj

# What test programs to make
TEST_PROGS = test

# What object files and libs for the test programs
TEST_SRCS = test.c mactest.c
TEST_LIBS = io.lib

#rules and targets common to all Miner makefiles
%include $(INIT)\makefile.def

### OPUS MKMF:  Do not remove this line!  Generated dependencies follow.

mactest.obj: psmacros.inc

test.obj: mono.h
