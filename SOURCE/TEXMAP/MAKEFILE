#
# $Source: f:/miner/source/texmap/rcs/makefile $
# $Revision: 1.29 $
# $Author: matt $
# $Date: 1996/12/04 19:27:43 $
#
# Makefile for hack3df testbed
#

#Uncomment the following 3 lines if you want to build with only C code.
#NASM = 1
#GLOBAL_CFLAGS+=/dNASM
#GLOBAL_AFLAGS+=/DNASM

# What to install
INSTALL_TARGS = texmap.lib texmap.h

# What libs to build
LIBS = texmap

TEXMAP_OBJS = 	ntmap.obj tmapflat.obj  

%if !%defined(NASM)
TEXMAP_OBJS +=	tmap_per.obj tmap_flt.obj tmapfade.obj tmap_ed.obj 
TEXMAP_OBJS +=  tmap_pnn.obj tmap_pln.obj tmap_plt.obj tmap_pnt.obj
TEXMAP_OBJS +=  tmap_qnn.obj tmap_qln.obj tmap_qlt.obj tmap_qnt.obj
TEXMAP_OBJS +=  tmap_lnn.obj tmap_lln.obj tmap_llt.obj tmap_lnt.obj
%else
TEXMAP_OBJS += scanline.obj
%endif

# What to clean
CLEAN_TARGS = $(OBJDIR)\*.obj

#rules and targets common to all Miner makefiles
%include $(INIT)\makefile.def

### OPUS MKMF:  Do not remove this line!  Generated dependencies follow.

ntmap.obj: 3d.h conio.h error.h fix.h gr.h key.h limits.h math.h mono.h \
	 ntmapout.h pa_enabl.h poly_acc.h pstypes.h rle.h scanline.h \
	 stdio.h stdlib.h texmap.h texmapl.h vecmat.h

scanline.obj: 3d.h conio.h fix.h gr.h grdef.h limits.h math.h mono.h \
	 pstypes.h scanline.h stdio.h stdlib.h texmap.h texmapl.h vecmat.h

tmap_ed.obj: psmacros.inc pstypes.inc tmap_inc.asm

tmap_flt.obj: psmacros.inc tmap_inc.asm

tmap_inc.obj: psmacros.inc

tmap_lln.obj: psmacros.inc pstypes.inc tmap_inc.asm

tmap_llt.obj: psmacros.inc pstypes.inc tmap_inc.asm

tmap_lnn.obj: psmacros.inc pstypes.inc tmap_inc.asm

tmap_lnt.obj: psmacros.inc pstypes.inc tmap_inc.asm

tmap_per.obj: psmacros.inc tmap_inc.asm

tmap_pln.obj: psmacros.inc pstypes.inc tmap_inc.asm

tmap_plt.obj: psmacros.inc pstypes.inc tmap_inc.asm

tmap_pnn.obj: psmacros.inc pstypes.inc tmap_inc.asm

tmap_pnt.obj: psmacros.inc pstypes.inc tmap_inc.asm

tmap_qln.obj: psmacros.inc pstypes.inc tmap_inc.asm

tmap_qlt.obj: psmacros.inc pstypes.inc tmap_inc.asm

tmap_qnn.obj: psmacros.inc pstypes.inc tmap_inc.asm

tmap_qnt.obj: psmacros.inc pstypes.inc tmap_inc.asm

tmapfade.obj: psmacros.inc tmap_inc.asm

tmapflat.obj: 3d.h conio.h error.h fix.h gr.h grdef.h limits.h math.h \
	 mono.h pa_enabl.h poly_acc.h pstypes.h scanline.h stdio.h stdlib.h \
	 texmap.h texmapl.h vecmat.h
