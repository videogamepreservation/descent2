#
# $Source: f:/miner/source/3d/rcs/makefile $
# $Revision: 1.10 $
# $Author: matt $
# $Date: 1996/01/08 14:59:22 $
#
# Makefile for 3d library
#

# What to install
INSTALL_TARGS = 3d.lib 3d.h 3d.inc

# What libs to build
LIBS = 3d

# What objs in the lib
3D_OBJS = globvars.obj setup.obj matrix.obj points.obj draw.obj clipper.obj \
	horizon.obj instance.obj rod.obj interp.obj

# What test programs to make
TEST_PROGS = 3dtest

# What object files and libs for the test programs
3DTEST_SRCS = 3dtest.c segment.c trakball.c
3DTEST_LIBS = 3d.lib vecmat.lib gr.lib io.lib fix.lib div0.lib texmap.lib misc.lib bm.lib

#rules and targets common to all Miner makefiles
%include $(INIT)\makefile.def

### OPUS MKMF:  Do not remove this line!  Generated dependencies follow.

3dtest.obj: 3d.h conio.h fix.h gr.h mono.h mouse.h out.c pstypes.h vecmat.h

clipper.obj: 3d.inc fix.inc gr.inc psmacros.inc pstypes.inc vecmat.inc

draw.obj: 3d.inc fix.inc gr.inc psmacros.inc pstypes.inc vecmat.inc

globvars.obj: 3d.inc fix.inc psmacros.inc pstypes.inc vecmat.inc

horizon.obj: 3d.inc fix.inc gr.inc psmacros.inc pstypes.inc vecmat.inc

instance.obj: 3d.inc fix.inc psmacros.inc pstypes.inc vecmat.inc

interp.obj: 3d.inc fix.inc gr.inc psmacros.inc pstypes.inc vecmat.inc

matrix.obj: 3d.inc fix.inc gr.inc psmacros.inc pstypes.inc vecmat.inc

objects.obj: 3d.inc fix.inc psmacros.inc pstypes.inc vecmat.inc

points.obj: 3d.inc fix.inc gr.inc psmacros.inc pstypes.inc vecmat.inc

rod.obj: 3d.inc fix.inc gr.inc psmacros.inc pstypes.inc vecmat.inc

segment.obj: 3d.h fix.h gr.h pstypes.h vecmat.h

setup.obj: 3d.inc fix.inc gr.inc psmacros.inc pstypes.inc vecmat.inc

trakball.obj: fix.h math.h mono.h pstypes.h

upvecs.obj: fix.h pstypes.h vecmat.h

upvecs2.obj: fix.h pstypes.h vecmat.h

