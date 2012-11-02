#
# $Source: f:/miner/source/vecmat/rcs/makefile $
# $Revision: 1.3 $
# $Author: matt $
# $Date: 1996/02/07 18:15:43 $
#
# Makefile for vector/matrix library
#

# What to install
INSTALL_TARGS = vecmat.lib vecmat.h vecmat.inc

# What libs to build
LIBS = vecmat

# What objs in the lib
VECMAT_OBJS = vecmat.obj

# What to clean
CLEAN_TARGS = $(OBJDIR)\*.obj

# What test programs to make
TEST_PROGS = vectest 

# What object files and libs for the test programs
VECTEST_SRCS = vectest.c
VECTEST_LIBS = vecmat.lib io.lib fix.lib

#rules and targets common to all Miner makefiles
%include $(INIT)\makefile.def

### OPUS MKMF:  Do not remove this line!  Generated dependencies follow.

vecmat.obj: fix.inc psmacros.inc pstypes.inc vecmat.inc
