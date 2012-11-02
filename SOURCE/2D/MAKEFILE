#
# $Source: f:/miner/source/2d/rcs/makefile $
# $Revision: 1.44 $
# $Author: matt $
# $Date: 1996/12/04 18:52:36 $
#
# Makefile for bios functions
#

# What to install, and where to install it
INSTALL_TARGS = effect2d.h gr.h gr.lib grdef.h gr.inc palette.h ibitblt.h rle.h pcx.h

# What to clean
CLEAN_TARGS = $(OBJDIR)\*.obj

# What libs to make
LIBS = gr

# What test programs to make
#TEST_PROGS = testg testp vesatest
#PROGS = testg

# What object files for the lib
GR_OBJS = linear.obj canvas.obj bitmap.obj gr.obj font.obj \
	bitblt.obj scanline.obj pixel.obj poly.obj disc.obj rect.obj box.obj  \
	gpixel.obj line.obj lbitblt.obj	   \
	scale.obj scalea.obj circle.obj ibitblt.obj rle.obj pcx.obj
#effects.obj roller.obj 

# What object files and libs for the test programs
#TESTG_SRCS = testg.c tmapbits.c
#TESTG_LIBS = io.lib gr.lib misc.lib mem.lib iff.lib  cfile.lib

#TESTP_OBJS = testp.obj
#TESTP_LIBS = io.lib gr.lib misc.lib mem.lib cflib.lib 

#VESATEST_OBJS = vesatest.obj
#VESATEST_LIBS = io.lib gr.lib misc.lib mem.lib cflib.lib 

#rules and targets common to all Miner makefiles
%include $(INIT)\makefile.def

### OPUS MKMF:  Do not remove this line!  Generated dependencies follow.

bitblt.obj: byteswap.h error.h fix.h gr.h grdef.h mem.h mono.h pa_enabl.h \
	 poly_acc.h pstypes.h rle.h

bitmap.obj: error.h fix.h gr.h grdef.h malloc.h mem.h pa_enabl.h poly_acc.h \
	 pstypes.h stdio.h stdlib.h

box.obj: fix.h gr.h grdef.h mem.h pstypes.h

canvas.obj: error.h fix.h gr.h grdef.h malloc.h mem.h pa_enabl.h poly_acc.h \
	 pstypes.h stdio.h stdlib.h

circle.obj: fix.h gr.h grdef.h mem.h pstypes.h

disc.obj: fix.h gr.h grdef.h mem.h pstypes.h

effects.obj: effect2d.h fix.h gr.h grdef.h mem.h pstypes.h stdio.h stdlib.h

font.obj: byteswap.h cfile.h error.h fcntl.h fix.h gr.h grdef.h io.h mem.h \
	 mono.h pa_enabl.h poly_acc.h pstypes.h stat.h stdarg.h stdio.h \
	 stdlib.h string.h types.h unistd.h

gpixel.obj: error.h fix.h gr.h grdef.h mem.h pa_enabl.h poly_acc.h \
	 pstypes.h

gr.obj:	 conio.h dos.h dpmi.h error.h fix.h gr.h grdef.h i86.h malloc.h \
	 mem.h mono.h pa_enabl.h palette.h poly_acc.h pstypes.h stdio.h \
	 stdlib.h string.h types.h

ibitblt.obj: conio.h dos.h error.h fix.h gr.h grdef.h i86.h ibitblt.h mem.h \
	 pa_enabl.h poly_acc.h pstypes.h stdio.h stdlib.h string.h types.h

lbitblt.obj: fix.inc gr.inc psmacros.inc pstypes.inc

line.obj: clip.h fix.h gr.h grdef.h mem.h pa_enabl.h poly_acc.h pstypes.h \
	 stdlib.h

linear.obj: psmacros.inc

macstubs.obj: error.h fix.h gr.h pstypes.h

mspeed.obj: conio.h fix.h gr.h grdef.h math.h pstypes.h stdio.h stdlib.h \
	 timer.h

paging.obj: psmacros.inc pstypes.inc

pcx.obj: byteswap.h cfile.h fix.h gr.h grdef.h mem.h pa_enabl.h pcx.h \
	 poly_acc.h pstypes.h stdio.h stdlib.h string.h

pflow.obj: psmacros.inc pstypes.inc

pixel.obj: error.h fix.h gr.h grdef.h mem.h pa_enabl.h poly_acc.h pstypes.h

poly.obj: fix.h gr.h grdef.h mem.h pstypes.h

rect.obj: fix.h gr.h grdef.h mem.h pa_enabl.h poly_acc.h pstypes.h

rle.obj: dpmi.h error.h fix.h gr.h grdef.h key.h malloc.h mem.h mono.h \
	 pa_enabl.h poly_acc.h pstypes.h stdio.h stdlib.h string.h

scale.obj: conio.h error.h fix.h gr.h limits.h math.h mono.h pa_enabl.h \
	 poly_acc.h pstypes.h rle.h stdio.h stdlib.h

scalea.obj: psmacros.inc pstypes.inc

scalemac.obj: error.h fix.h gr.h grdef.h pstypes.h

scanline.obj: fix.h gr.h grdef.h mem.h pa_enabl.h poly_acc.h pstypes.h

selector.obj: psmacros.inc pstypes.inc

vesalib.obj: dpmi.h pstypes.h

vesatest.obj: conio.h fix.h gr.h malloc.h pstypes.h stdio.h stdlib.h
