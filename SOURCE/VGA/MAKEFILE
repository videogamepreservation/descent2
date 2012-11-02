#
# $Source: f:/miner/source/vga/rcs/makefile $
# $Revision: 1.7 $
# $Author: matt $
# $Date: 1996/03/05 13:35:39 $
#
# Makefile for bios functions
#

# What to install, and where to install it
INSTALL_TARGS = vga.h vga.lib

# What to clean
CLEAN_TARGS = $(OBJDIR)\*.obj

# What libs to make
LIBS = vga

# What object files for the lib
VGA_OBJS = vga.obj palette.obj vesa.obj modex.obj framebuf.obj

#rules and targets common to all Miner makefiles
%include $(INIT)\makefile.def

### OPUS MKMF:  Do not remove this line!  Generated dependencies follow.

framebuf.obj: 3dfx.h dos.h fxdpmi.h fxvesa.h i86.h mono.h stdio.h stdlib.h \
	 string.h

modex.obj: psmacros.inc tweak.inc vgaregs.inc

palette.obj: cfile.h conio.h error.h fix.h gr.h grdef.h io.h key.h mem.h \
	 mono.h pa_enabl.h poly_acc.h pstypes.h stat.h stdio.h stdlib.h \
	 string.h types.h

vesa.obj: fix.inc gr.inc psmacros.inc pstypes.inc vgaregs.inc

vga.obj: 3dfx.h conio.h dos.h error.h fix.h fxvesa.h gr.h grdef.h i86.h \
	 malloc.h mem.h mono.h pa_enabl.h palette.h poly_acc.h pstypes.h \
	 stdio.h stdlib.h string.h types.h vga.h
