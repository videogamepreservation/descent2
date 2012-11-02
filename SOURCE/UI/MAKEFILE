#
# $Source: f:/miner/source/ui/rcs/makefile $
# $Revision: 1.9 $
# $Author: matt $
# $Date: 1995/05/30 22:55:30 $
#
# Makefile for user-interface functions
#

# What to install, and where to install it
INSTALL_TARGS = ui.lib ui.h func.h

# What to clean
CLEAN_TARGS = $(OBJDIR)\*.obj

# What libs to make
LIBS = ui

# What programs to make
TEST_PROGS = testg

# What object files for the lib
UI_OBJS = ui.obj mouse.obj window.obj uidraw.obj button.obj \
		  gadget.obj listbox.obj message.obj popup.obj scroll.obj \
		  radio.obj checkbox.obj inputbox.obj userbox.obj menu.obj \
		  harderr.obj file.obj keytrap.obj icon.obj keypress.obj \
		  menubar.obj keypad.obj func.obj

# What object files and libs for the test programs
TESTG_SRCS = testg.c
TESTG_LIBS = io.lib gr.lib ui.lib misc.lib mem.lib readfile.lib iff.lib func.lib cflib.lib cfile.lib
TESTG_LFLAGS = option stack=50k

#rules and targets common to all Miner makefiles
%include $(INIT)\makefile.def

### OPUS MKMF:  Do not remove this line!  Generated dependencies follow.

barbox.obj: fix.h gr.h key.h stdarg.h stdio.h types.h ui.h

button.obj: fix.h gr.h key.h mem.h mono.h stdlib.h string.h types.h ui.h

checkbox.obj: fix.h gr.h key.h mem.h stdlib.h string.h types.h ui.h

file.obj: conio.h ctype.h direct.h dos.h fcntl.h fix.h gr.h i86.h io.h \
	 key.h mem.h mono.h stat.h stdio.h stdlib.h string.h types.h ui.h

func.obj: func.h stdlib.h string.h

gadget.obj: fix.h gr.h key.h mem.h stdio.h stdlib.h types.h ui.h

icon.obj: fix.h gr.h key.h mem.h stdlib.h string.h types.h ui.h

inputbox.obj: fix.h gr.h key.h mem.h stdlib.h string.h types.h ui.h

keypad.obj: conio.h direct.h dos.h error.h fix.h func.h gr.h i86.h key.h \
	 math.h mem.h mono.h stdio.h stdlib.h string.h types.h ui.h

keypress.obj: fix.h gr.h key.h stdarg.h stdio.h string.h types.h ui.h

keytrap.obj: fix.h gr.h key.h stdlib.h string.h types.h ui.h

listbox.obj: fix.h gr.h key.h stdlib.h types.h ui.h

menu.obj: fix.h gr.h mem.h stdlib.h types.h ui.h

menubar.obj: ctype.h error.h fix.h func.h gr.h key.h mem.h mono.h stdio.h \
	 stdlib.h string.h types.h ui.h

message.obj: fix.h gr.h key.h stdarg.h stdio.h types.h ui.h

mouse.obj: fix.h gr.h mem.h mouse.h stdlib.h types.h ui.h

number.obj: fix.h gr.h stdarg.h stdio.h types.h ui.h

popup.obj: fix.h gr.h mouse.h types.h ui.h

radio.obj: fix.h gr.h key.h mono.h stdlib.h string.h types.h ui.h

scroll.obj: fix.h gr.h key.h stdlib.h types.h ui.h

ui.obj:	 dos.h fix.h gr.h i86.h key.h mono.h mouse.h stdarg.h stdio.h \
	 stdlib.h string.h types.h ui.h

uidraw.obj: fix.h gr.h types.h ui.h

userbox.obj: fix.h gr.h key.h stdlib.h string.h types.h ui.h

window.obj: dos.h fix.h gr.h i86.h key.h mem.h mono.h mouse.h stdarg.h \
	 stdio.h stdlib.h types.h ui.h
