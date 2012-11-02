#
# $Source: f:/miner/source/win95/rcs/makefile $
# $Revision: 1.19 $
# $Author: matt $
# $Date: 1996/12/04 19:21:02 $
#
# Makefile for Win32 'PseudoBIOS' Extensions

# Build for Windows
TARGSYS = WIN32

# What to install
INSTALL_TARGS = winapp.h comm.h win95.lib dd.h ds.h winregs.h winmidi.h xtapi.h \
		tactile.h

# Where to install
LIB_SUBDIR = win

# What to clean
CLEAN_TARGS = $(OBJDIR)\*.obj

# What libs to make
LIBS = win95

# What object files for the lib
WIN95_OBJS = palette.obj key.obj findfile.obj timer.obj rbaudio.obj error.obj \
	     ipx.obj joyc.obj swift.obj mouse.obj winapp.obj mono.obj \
	     comm.obj winregs.obj ds.obj midifile.obj winmidi.obj winckpit.obj \
	     gfx.obj dd.obj ddgr.obj direct3d.obj xtapi.obj iforce.obj tactile.obj

# Where to find the DirectX stuff
EXTLIBS = $(MINERLOCAL)\source\extlib\dd


#rules and targets common to all Miner makefiles
%include $(INIT)\makefile.def

### OPUS MKMF:  Do not remove this line!  Generated dependencies follow.

comm.obj: cderr.h cguid.h comm.h commdlg.h ctype.h dde.h ddeml.h dlgs.h \
	 drivinit.h error.h excpt.h lzexpand.h mmsystem.h mono.h nb30.h \
	 objbase.h objerror.h ole.h ole2.h oleauto.h poppack.h pshpack1.h \
	 pshpack2.h pshpack4.h pshpack8.h rpc.h rpcdce.h rpcdcep.h rpcndr.h \
	 rpcnsi.h rpcnsip.h rpcnterr.h shellapi.h stdarg.h stdlib.h \
	 string.h winapp.h winbase.h wincon.h windef.h windows.h winerror.h \
	 wingdi.h winnetwk.h winnls.h winnt.h winperf.h winreg.h winsock.h \
	 winspool.h winsvc.h winuser.h winver.h

dd.obj:	 args.h cderr.h cguid.h commdlg.h ctype.h dd.h dde.h ddeml.h \
	 ddraw.h dlgs.h drivinit.h error.h excpt.h fix.h gr.h lzexpand.h \
	 mmsystem.h mono.h nb30.h objbase.h objerror.h ole.h ole2.h \
	 oleauto.h poppack.h pshpack1.h pshpack2.h pshpack4.h pshpack8.h \
	 pstypes.h rpc.h rpcdce.h rpcdcep.h rpcndr.h rpcnsi.h rpcnsip.h \
	 rpcnterr.h shellapi.h stdarg.h stdio.h stdlib.h string.h winapp.h \
	 winbase.h wincon.h windef.h windows.h windowsx.h winerror.h \
	 wingdi.h winnetwk.h winnls.h winnt.h winperf.h winreg.h winsock.h \
	 winspool.h winsvc.h winuser.h winver.h

ddgfx.obj: args.h cderr.h cguid.h commdlg.h ctype.h dd.h dde.h ddeml.h \
	 ddraw.h dlgs.h drivinit.h error.h excpt.h fix.h gr.h lzexpand.h \
	 mmsystem.h mono.h nb30.h objbase.h objerror.h ole.h ole2.h \
	 oleauto.h poppack.h pshpack1.h pshpack2.h pshpack4.h pshpack8.h \
	 pstypes.h rle.h rpc.h rpcdce.h rpcdcep.h rpcndr.h rpcnsi.h \
	 rpcnsip.h rpcnterr.h shellapi.h stdarg.h stdlib.h string.h \
	 winapp.h winbase.h wincon.h windef.h windows.h winerror.h wingdi.h \
	 winnetwk.h winnls.h winnt.h winperf.h winreg.h winsock.h \
	 winspool.h winsvc.h winuser.h winver.h

ddgr.obj: cderr.h cguid.h commdlg.h ctype.h dd.h dde.h ddeml.h ddraw.h \
	 dlgs.h drivinit.h error.h excpt.h fix.h gr.h lzexpand.h mem.h \
	 mmsystem.h mono.h nb30.h objbase.h objerror.h ole.h ole2.h \
	 oleauto.h poppack.h pshpack1.h pshpack2.h pshpack4.h pshpack8.h \
	 pstypes.h rpc.h rpcdce.h rpcdcep.h rpcndr.h rpcnsi.h rpcnsip.h \
	 rpcnterr.h shellapi.h stdarg.h stdlib.h string.h winapp.h \
	 winbase.h wincon.h windef.h windows.h winerror.h wingdi.h \
	 winnetwk.h winnls.h winnt.h winperf.h winreg.h winsock.h \
	 winspool.h winsvc.h winuser.h winver.h

ddraw.obj: args.h cderr.h cguid.h commdlg.h ctype.h dd.h dde.h ddeml.h \
	 ddraw.h dlgs.h drivinit.h error.h excpt.h fix.h gr.h lzexpand.h \
	 mem.h mmsystem.h mono.h nb30.h objbase.h objerror.h ole.h ole2.h \
	 oleauto.h poppack.h pshpack1.h pshpack2.h pshpack4.h pshpack8.h \
	 pstypes.h rpc.h rpcdce.h rpcdcep.h rpcndr.h rpcnsi.h rpcnsip.h \
	 rpcnterr.h shellapi.h stdarg.h stdio.h stdlib.h string.h winapp.h \
	 winbase.h wincon.h windef.h windows.h winerror.h wingdi.h \
	 winnetwk.h winnls.h winnt.h winperf.h winreg.h winsock.h \
	 winspool.h winsvc.h winuser.h winver.h

direct3d.obj: args.h cderr.h cguid.h commdlg.h ctype.h d3d.h d3dcaps.h \
	 d3dtypes.h dd.h dde.h ddeml.h ddraw.h direct3d.h dlgs.h drivinit.h \
	 error.h excpt.h fix.h gr.h lzexpand.h mmsystem.h mono.h nb30.h \
	 objbase.h objerror.h ole.h ole2.h oleauto.h poppack.h pshpack1.h \
	 pshpack2.h pshpack4.h pshpack8.h pstypes.h rpc.h rpcdce.h \
	 rpcdcep.h rpcndr.h rpcnsi.h rpcnsip.h rpcnterr.h shellapi.h \
	 stdarg.h stdio.h stdlib.h string.h winapp.h winbase.h wincon.h \
	 windef.h windows.h windowsx.h winerror.h wingdi.h winnetwk.h \
	 winnls.h winnt.h winperf.h winreg.h winsock.h winspool.h winsvc.h \
	 winuser.h winver.h

ds.obj:	 cderr.h cguid.h commdlg.h ctype.h dde.h ddeml.h dlgs.h drivinit.h \
	 ds.h dsound.h excpt.h fix.h lzexpand.h math.h mem.h mmsystem.h \
	 mono.h nb30.h objbase.h objerror.h ole.h ole2.h oleauto.h \
	 poppack.h pshpack1.h pshpack2.h pshpack4.h pshpack8.h pstypes.h \
	 rpc.h rpcdce.h rpcdcep.h rpcndr.h rpcnsi.h rpcnsip.h rpcnterr.h \
	 shellapi.h stdarg.h stdlib.h string.h winbase.h wincon.h windef.h \
	 windows.h winerror.h wingdi.h winnetwk.h winnls.h winnt.h \
	 winperf.h winreg.h winsock.h winspool.h winsvc.h winuser.h \
	 winver.h

error.obj: args.h cderr.h cguid.h commdlg.h ctype.h dde.h ddeml.h dlgs.h \
	 drivinit.h error.h excpt.h lzexpand.h mmsystem.h mono.h nb30.h \
	 objbase.h objerror.h ole.h ole2.h oleauto.h poppack.h pshpack1.h \
	 pshpack2.h pshpack4.h pshpack8.h rpc.h rpcdce.h rpcdcep.h rpcndr.h \
	 rpcnsi.h rpcnsip.h rpcnterr.h shellapi.h stdarg.h stdio.h stdlib.h \
	 string.h types.h winbase.h wincon.h windef.h windows.h winerror.h \
	 wingdi.h winnetwk.h winnls.h winnt.h winperf.h winreg.h winsock.h \
	 winspool.h winsvc.h winuser.h winver.h

findfile.obj: cderr.h cguid.h commdlg.h ctype.h dde.h ddeml.h dlgs.h \
	 drivinit.h excpt.h findfile.h lzexpand.h mmsystem.h nb30.h \
	 objbase.h objerror.h ole.h ole2.h oleauto.h poppack.h pshpack1.h \
	 pshpack2.h pshpack4.h pshpack8.h rpc.h rpcdce.h rpcdcep.h rpcndr.h \
	 rpcnsi.h rpcnsip.h rpcnterr.h shellapi.h stdarg.h stdlib.h \
	 string.h winbase.h wincon.h windef.h windows.h winerror.h wingdi.h \
	 winnetwk.h winnls.h winnt.h winperf.h winreg.h winsock.h \
	 winspool.h winsvc.h winuser.h winver.h

gfx.obj: args.h cderr.h cguid.h commdlg.h ctype.h dd.h dde.h ddeml.h \
	 ddraw.h direct3d.h dlgs.h drivinit.h error.h excpt.h fix.h gr.h \
	 lzexpand.h mem.h mmsystem.h mono.h nb30.h objbase.h objerror.h \
	 ole.h ole2.h oleauto.h poppack.h pshpack1.h pshpack2.h pshpack4.h \
	 pshpack8.h pstypes.h rpc.h rpcdce.h rpcdcep.h rpcndr.h rpcnsi.h \
	 rpcnsip.h rpcnterr.h shellapi.h stdarg.h stdio.h stdlib.h string.h \
	 winapp.h winbase.h wincon.h windef.h windows.h windowsx.h \
	 winerror.h wingdi.h winnetwk.h winnls.h winnt.h winperf.h winreg.h \
	 winsock.h winspool.h winsvc.h winuser.h winver.h

iforce.obj: cderr.h cguid.h commdlg.h ctype.h dde.h ddeml.h dlgs.h \
	 drivinit.h excpt.h fix.h iforce.h joy.h lzexpand.h mmsystem.h \
	 mono.h nb30.h objbase.h objerror.h ole.h ole2.h oleauto.h \
	 poppack.h pshpack1.h pshpack2.h pshpack4.h pshpack8.h pstypes.h \
	 rpc.h rpcdce.h rpcdcep.h rpcndr.h rpcnsi.h rpcnsip.h rpcnterr.h \
	 shellapi.h stdarg.h stdlib.h string.h tactile.h vecmat.h winapp.h \
	 winbase.h wincon.h windef.h windows.h winerror.h wingdi.h \
	 winnetwk.h winnls.h winnt.h winperf.h winreg.h winsock.h \
	 winspool.h winsvc.h winuser.h winver.h

ipx.obj: cderr.h cguid.h commdlg.h conio.h ctype.h dde.h ddeml.h dlgs.h \
	 drivinit.h error.h excpt.h ipx.h lzexpand.h malloc.h mmsystem.h \
	 mono.h nb30.h objbase.h objerror.h ole.h ole2.h oleauto.h \
	 poppack.h pshpack1.h pshpack2.h pshpack4.h pshpack8.h pstypes.h \
	 rpc.h rpcdce.h rpcdcep.h rpcndr.h rpcnsi.h rpcnsip.h rpcnterr.h \
	 shellapi.h stdarg.h stdio.h stdlib.h string.h winapp.h winbase.h \
	 wincon.h windef.h windows.h windowsx.h winerror.h wingdi.h \
	 winnetwk.h winnls.h winnt.h winperf.h winreg.h winsock.h \
	 winspool.h winsvc.h winuser.h winver.h wsipx.h

joyc.obj: args.h cderr.h cguid.h commdlg.h conio.h ctype.h dde.h ddeml.h \
	 dlgs.h drivinit.h excpt.h fix.h joy.h lzexpand.h mmsystem.h mono.h \
	 nb30.h objbase.h objerror.h ole.h ole2.h oleauto.h poppack.h \
	 pshpack1.h pshpack2.h pshpack4.h pshpack8.h pstypes.h rpc.h \
	 rpcdce.h rpcdcep.h rpcndr.h rpcnsi.h rpcnsip.h rpcnterr.h \
	 shellapi.h stdarg.h stdio.h stdlib.h string.h tactile.h types.h \
	 vecmat.h winapp.h winbase.h wincon.h windef.h windows.h winerror.h \
	 wingdi.h winnetwk.h winnls.h winnt.h winperf.h winreg.h winregs.h \
	 winsock.h winspool.h winsvc.h winuser.h winver.h

key.obj: cderr.h cguid.h commdlg.h conio.h ctype.h dde.h ddeml.h dlgs.h \
	 drivinit.h error.h excpt.h fix.h key.h lzexpand.h mmsystem.h \
	 mono.h nb30.h objbase.h objerror.h ole.h ole2.h oleauto.h \
	 poppack.h pshpack1.h pshpack2.h pshpack4.h pshpack8.h pstypes.h \
	 rpc.h rpcdce.h rpcdcep.h rpcndr.h rpcnsi.h rpcnsip.h rpcnterr.h \
	 shellapi.h stdarg.h stdio.h stdlib.h string.h timer.h winbase.h \
	 wincon.h windef.h windows.h winerror.h wingdi.h winnetwk.h \
	 winnls.h winnt.h winperf.h winreg.h winsock.h winspool.h winsvc.h \
	 winuser.h winver.h

midifile.obj: cderr.h cguid.h commdlg.h ctype.h dde.h ddeml.h dlgs.h \
	 drivinit.h error.h excpt.h global.h lzexpand.h memory.h midifile.h \
	 mmsystem.h mono.h muldiv32.h nb30.h objbase.h objerror.h ole.h \
	 ole2.h oleauto.h poppack.h pshpack1.h pshpack2.h pshpack4.h \
	 pshpack8.h pstypes.h rpc.h rpcdce.h rpcdcep.h rpcndr.h rpcnsi.h \
	 rpcnsip.h rpcnterr.h shellapi.h stdarg.h stdlib.h string.h \
	 winbase.h wincon.h windef.h windows.h windowsx.h winerror.h \
	 wingdi.h winnetwk.h winnls.h winnt.h winperf.h winreg.h winsock.h \
	 winspool.h winsvc.h winuser.h winver.h

mono.obj: conio.h dos.h fix.h i86.h key.h pstypes.h stdarg.h stdio.h \
	 stdlib.h string.h

mouse.obj: cderr.h cguid.h commdlg.h conio.h ctype.h dde.h ddeml.h dlgs.h \
	 drivinit.h error.h excpt.h fix.h i86.h lzexpand.h mmsystem.h \
	 mono.h mouse.h nb30.h objbase.h objerror.h ole.h ole2.h oleauto.h \
	 poppack.h pshpack1.h pshpack2.h pshpack4.h pshpack8.h pstypes.h \
	 rpc.h rpcdce.h rpcdcep.h rpcndr.h rpcnsi.h rpcnsip.h rpcnterr.h \
	 shellapi.h stdarg.h stdio.h stdlib.h string.h timer.h winbase.h \
	 wincon.h windef.h windows.h winerror.h wingdi.h winnetwk.h \
	 winnls.h winnt.h winperf.h winreg.h winsock.h winspool.h winsvc.h \
	 winuser.h winver.h

palette.obj: cderr.h cfile.h cguid.h commdlg.h conio.h ctype.h dd.h dde.h \
	 ddeml.h ddraw.h dlgs.h drivinit.h error.h excpt.h fix.h gr.h io.h \
	 key.h lzexpand.h mem.h mmsystem.h mono.h nb30.h objbase.h \
	 objerror.h ole.h ole2.h oleauto.h poppack.h pshpack1.h pshpack2.h \
	 pshpack4.h pshpack8.h pstypes.h rpc.h rpcdce.h rpcdcep.h rpcndr.h \
	 rpcnsi.h rpcnsip.h rpcnterr.h shellapi.h stat.h stdarg.h stdio.h \
	 stdlib.h string.h types.h winapp.h winbase.h wincon.h windef.h \
	 windows.h winerror.h wingdi.h winnetwk.h winnls.h winnt.h \
	 winperf.h winreg.h winsock.h winspool.h winsvc.h winuser.h \
	 winver.h

rbaudio.obj: cderr.h cguid.h commdlg.h ctype.h dde.h ddeml.h dlgs.h \
	 drivinit.h error.h excpt.h fix.h lzexpand.h mmsystem.h mono.h \
	 nb30.h objbase.h objerror.h ole.h ole2.h oleauto.h poppack.h \
	 pshpack1.h pshpack2.h pshpack4.h pshpack8.h pstypes.h rbaudio.h \
	 rpc.h rpcdce.h rpcdcep.h rpcndr.h rpcnsi.h rpcnsip.h rpcnterr.h \
	 shellapi.h stdarg.h stdlib.h string.h winapp.h winbase.h wincon.h \
	 windef.h windows.h winerror.h wingdi.h winnetwk.h winnls.h winnt.h \
	 winperf.h winreg.h winsock.h winspool.h winsvc.h winuser.h \
	 winver.h

swift.obj: swift.h

tactile.obj: fix.h iforce.h mono.h pstypes.h tactile.h types.h vecmat.h

timer.obj: error.h fix.h mono.h pstypes.h types.h

winapp.obj: cderr.h cguid.h commdlg.h ctype.h dd.h dde.h ddeml.h ddraw.h \
	 dlgs.h drivinit.h excpt.h fix.h gr.h lzexpand.h mmsystem.h mono.h \
	 nb30.h objbase.h objerror.h ole.h ole2.h oleauto.h poppack.h \
	 pshpack1.h pshpack2.h pshpack4.h pshpack8.h pstypes.h rpc.h \
	 rpcdce.h rpcdcep.h rpcndr.h rpcnsi.h rpcnsip.h rpcnterr.h \
	 shellapi.h stdarg.h stdio.h stdlib.h string.h winapp.h winbase.h \
	 wincon.h windef.h windows.h winerror.h wingdi.h winnetwk.h \
	 winnls.h winnt.h winperf.h winreg.h winsock.h winspool.h winsvc.h \
	 winuser.h winver.h

winckpit.obj: psmacros.inc pstypes.inc

winmidi.obj: cderr.h cguid.h commdlg.h ctype.h dde.h ddeml.h dlgs.h \
	 drivinit.h error.h excpt.h global.h lzexpand.h mem.h midifile.h \
	 midiseq.h mmreg.h mmsystem.h mono.h nb30.h objbase.h objerror.h \
	 ole.h ole2.h oleauto.h poppack.h pshpack1.h pshpack2.h pshpack4.h \
	 pshpack8.h pstypes.h rpc.h rpcdce.h rpcdcep.h rpcndr.h rpcnsi.h \
	 rpcnsip.h rpcnterr.h shellapi.h stdarg.h stdlib.h string.h \
	 winapp.h winbase.h wincon.h windef.h windows.h windowsx.h \
	 winerror.h wingdi.h winmidi.h winnetwk.h winnls.h winnt.h \
	 winperf.h winreg.h winregs.h winsock.h winspool.h winsvc.h \
	 winuser.h winver.h

winmono.obj: cderr.h cguid.h commdlg.h ctype.h dde.h ddeml.h dlgs.h \
	 drivinit.h excpt.h lzexpand.h malloc.h mmsystem.h nb30.h objbase.h \
	 objerror.h ole.h ole2.h oleauto.h poppack.h pshpack1.h pshpack2.h \
	 pshpack4.h pshpack8.h rpc.h rpcdce.h rpcdcep.h rpcndr.h rpcnsi.h \
	 rpcnsip.h rpcnterr.h shellapi.h stdarg.h stdio.h stdlib.h string.h \
	 winbase.h wincon.h windef.h windows.h winerror.h wingdi.h \
	 winnetwk.h winnls.h winnt.h winperf.h winreg.h winsock.h \
	 winspool.h winsvc.h winuser.h winver.h

winregs.obj: cderr.h cguid.h commdlg.h ctype.h dde.h ddeml.h dlgs.h \
	 drivinit.h error.h excpt.h lzexpand.h mmsystem.h mono.h nb30.h \
	 objbase.h objerror.h ole.h ole2.h oleauto.h poppack.h pshpack1.h \
	 pshpack2.h pshpack4.h pshpack8.h rpc.h rpcdce.h rpcdcep.h rpcndr.h \
	 rpcnsi.h rpcnsip.h rpcnterr.h shellapi.h stdarg.h stdlib.h \
	 string.h winbase.h wincon.h windef.h windows.h winerror.h wingdi.h \
	 winnetwk.h winnls.h winnt.h winperf.h winreg.h winregs.h winsock.h \
	 winspool.h winsvc.h winuser.h winver.h

xtapi.obj: cderr.h cguid.h comm.h commdlg.h ctype.h dde.h ddeml.h dlgs.h \
	 drivinit.h error.h excpt.h lzexpand.h mmsystem.h mono.h nb30.h \
	 objbase.h objerror.h ole.h ole2.h oleauto.h poppack.h pshpack1.h \
	 pshpack2.h pshpack4.h pshpack8.h pstypes.h rpc.h rpcdce.h \
	 rpcdcep.h rpcndr.h rpcnsi.h rpcnsip.h rpcnterr.h shellapi.h \
	 stdarg.h stdlib.h string.h winapp.h winbase.h wincon.h windef.h \
	 windows.h windowsx.h winerror.h wingdi.h winnetwk.h winnls.h \
	 winnt.h winperf.h winreg.h winsock.h winspool.h winsvc.h winuser.h \
	 winver.h xtapi.h
