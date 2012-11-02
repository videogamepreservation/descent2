#
# $Source: f:/miner/source/main/rcs/makefile $
# $Revision: 2.120 $
# $Author: matt $
# $Date: 1997/01/27 16:25:59 $
#
# Makefile for DESCENT execuable
#

INI_FILE = inferno.ini

# Load this person's personal settings
%include $(INI_FILE)

# Set target system if not in INI file, and make sure it's in uppercase
TARGSYS ?= DOS4G
TARGSYS := $(TARGSYS,UC)

# Where we're running the game.  Set here if no env var
MINERGAME ?= $(MINERLOCAL)\game

# Where to find libraries we didn't write
EXTLIB_DIR = $(MINERLOCAL)\source\extlib
EXTLIBS = $(EXTLIB_DIR)\greenlf $(EXTLIB_DIR)\glide	\
	  $(EXTLIB_DIR)\mvelib  $(EXTLIB_DIR)\iforce	\
	  $(EXTLIB_DIR)\s3dtk   $(EXTLIB_DIR)\loadpats	\
	  $(EXTLIB_DIR)\eregwin $(EXTLIB_DIR)\dd	\
	  $(EXTLIB_DIR)\soswin  $(EXTLIB_DIR)\mvewin    \
	  $(EXTLIB_DIR)\rend    

# What programs to make
%if $(TARGSYS) == "WIN32"
PROGS = $(MINERGAME)\descentw 
%else
%ifdef SHAREWARE
PROGS = $(MINERGAME)\d2share
%else
PROGS = $(MINERGAME)\descent2
%endif
%endif

# Files to link for Descent
DESCENT2_SRCS = cdmix32.c game.c gamerend.c gamecntl.c gauges.c mglobal.c \
	scores.c object.c laser.c physics.c bm.c \
	menu.c gamesave.c switch.c effects.c texmerge.c polyobj.c \
	gamemine.c fireball.c ai.c ai2.c aipath.c hostage.c \
	powerup.c fuelcen.c render.c piggy.c digiobj.c \
	gameseg.c wall.c collide.c lighting.c \
	robot.c morph.c vclip.c weapon.c fvi.c newdemo.c titles.c \
	vfx.c vfxread.c gameseq.c controls.c automap.c text.c \
	network.c newmenu.c netmisc.c \
	gamefont.c hud.c playsave.c \
	endlevel.c kconfig.c crypt.c\
	multi.c cntrlcen.c credits.c config.c kmatrix.c	\
	paging.c mission.c songs.c bmread.c \
	escort.c gamepal.c multibot.c state.c \
	3dmax.c 
	#demo.c garage.c radar.c coindev.c serial.c hash.c args.c 


%if $(TARGSYS) == "WIN32"
  DESCENT2_SRCS += winferno.c descentw.c winmodem.c joydefsw.c windigi.c winmovie.c
%else
  DESCENT2_SRCS += inferno.c modem.c joydefs.c victor.c iglasses.c movie.c digi.c
%endif

%if %defined(ARCADE)
  DESCENT2_SRCS += arcade.c 
%endif

%if !%defined(NDEBUG)
  DESCENT2_SRCS += dumpmine.c
%endif

%if !%defined(RELEASE_VERSION)
  DESCENT2_SRCS += slew.c
%endif

%if %defined(SHAREWARE)
  DESCENT2_SRCS += terrain.c 
%endif

# These sources are always built optimized
DESCENT2_SRCS_OPT = ## gameseg.c wall.c

DESCENT2_LIBS =	3d.lib gr.lib fix.lib iff.lib vecmat.lib \
		div0.lib misc.lib \
          	loadpats.lib cfile.lib \
		vidlib.lib texmap.lib unarj.lib
		#iglasses.lib
		#lzw.lib sosdwxcr.lib sosmwxcr.lib 

%if %defined(TACTILE) && $(TARGSYS) != "WIN32"
DESCENT2_LIBS += tactile.lib
%endif

%if !%defined(NDEBUG)
DESCENT2_LIBS += mem.lib
%endif

%if $(TARGSYS) == "WIN32"
  DESCENT2_LIBS += win95.lib \
		   ddraw.lib dsound.lib mvelibw.lib \
		   novga.lib tapi32.lib

%if %defined(TACTILE)
  DESCENT2_LIBS += iforce.lib
%endif

%if %defined(RELEASE_VERSION)
  DESCENT2_LIBS += eregreg2.lib
%endif

%else
  DESCENT2_LIBS += io.lib vga.lib gclfr32.lib sosdigi.lib sosmidi.lib mvelb32s.lib \
		poly_acc.lib s3dtkwdr.lib vlib.lib spd3d.lib

%endif

DESCENT2_DIRS = editor

# Link flags for Descent
DESCENT2_LFLAGS = option map,verbose,stack=50k

# Files in the editor subdirectory
%ifdef BUILD_EDITOR

EDITOR_SRCS = med.c einit.c eglobal.c mine.c curves.c kcurve.c \
	khelp.c ksegsel.c kbuild.c kview.c kmine.c seguvs.c ksegsize.c \
	ksegmove.c ktmap.c kfuncs.c autosave.c group.c texture.c \
	fixseg.c eobject.c elight.c texpage.c objpage.c medmisc.c \
	medsel.c kgame.c info.c medwall.c eswitch.c medrobot.c meddraw.c \
	segment.c centers.c	#ehostage.c 

# These sources are always built optimized
EDITOR_SRCS_OPT =  

DESCENT2_LIBS += ui.lib #medlisp.lib 

# Editor data files to check out with source
EDITOR_DATA = segmove.pad segsize.pad texture.pad object.pad curve.pad \
	      group.pad objmov.pad lighting.pad test.pad med.mnu global.key

EDITOR_DATA_FILES = $(EDITOR_DATA,<$(MINERGAME)\)

$(EDITOR_DATA_FILES) .NODEFAULT: $(@,F,<editor\)
	%if %exists($@)
	  @attrib -r $@	#allow overwriting
	%endif
	@echo Copying $< to $(@,D)
	@copy $< $@ >nul

DESCENT2_DATA = $(EDITOR_DATA_FILES)

%else
EDITOR_SRCS = 
%endif

# Force include of settings for all files
GLOBAL_INCFILES += settings.h

# DescentW is really Descent 2
DESCENTW_SRCS   = $(DESCENT2_SRCS)
DESCENTW_LIBS   = $(DESCENT2_LIBS)
DESCENTW_DIRS   = $(DESCENT2_DIRS)
DESCENTW_LFLAGS = $(DESCENT2_LFLAGS)
DESCENTW_DATA   = $(DESCENT2_DATA)

# D2Share is really Descent 2
D2SHARE_SRCS   = $(DESCENT2_SRCS)
D2SHARE_LIBS   = $(DESCENT2_LIBS)
D2SHARE_DIRS   = $(DESCENT2_DIRS)
D2SHARE_LFLAGS = $(DESCENT2_LFLAGS)
D@SHARE_DATA   = $(DESCENT2_DATA)

#Rule for getting game data files
data .NODEFAULT:
	@($(MINERGAME,Z);cd $(MINERGAME);			\
	  dir /b /a-d $(MINER)\game >dir.out;			\
	  rcsfiles >files.rcs;					\
	  $(MAKE) $(MFLAGS) -f $(MINER)\game\makefile all;	\
	  del dir.out)

# Check various settings for conflicts
.BEFORE::
	%if %defined(BUILD_EDITOR) && %defined(NDEBUG)
	  %error 1 Error in $(INI_FILE,UC): If BUILD_EDITOR defined, NDEBUG must not be.
	%endif
	%if %defined(ARCADE) && !%defined(NEWDEMO)
	  %error 1 Error in $(INI_FILE,UC): ARCADE option requires that NEWDEMO also be set.
	%endif
	%if %defined(DEMO_ONLY) && !%defined(NEWDEMO)
	  %error 1 Error in $(INI_FILE,UC): DEMO_ONLY defined, but NEWDEMO not.
	%endif

# Set up personal settings
.BEFORE::
	@%echo // >settings.tmp
	@%echo // Auto-generated include file >>settings.tmp
	@%echo // Generated by MAKE from $(INI_FILE,UC) settings >>settings.tmp
	@%echo // >>settings.tmp
	@%echo >>settings.tmp
	@%echo \#define NEWDEMO				//Demo is always IN >>settings.tmp
	%do def_setting  _IVAR="BUILD_EDITOR" _SVAR="EDITOR"
	%do def_setting  _IVAR="RELEASE_VERSION" _SVAR="RELEASE"
	%do def_setting  _IVAR="NDEBUG"
	%do def_setting  _IVAR="NMONO"
	%do def_setting  _IVAR="TARGSYS" _IVAL="WIN32" _SVAR="WINDOWS"
	%do def_setting  _IVAR="SHAREWARE"
	%do def_setting  _IVAR="TACTILE"
	%do def_setting  _IVAR="SANTA"
	%do def_setting  _IVAR="ARCADE"
	%do def_setting  _IVAR="DEMO_ONLY"
	%do def_setting  _IVAR="MARK_ON" _SVAR="_MARK_ON"
	%do def_setting  _IVAR="PASSWORD" _SVAL=$(PASSWORD)
	%do def_setting  _IVAR="STORE_DEMO"
	%do def_setting  _IVAR="D2_OEM"
	%do def_setting  _IVAR="COMPILATION"
	%do def_setting  _IVAR="SATURN"
	%do def_setting  _IVAR="COMPACT_SEGS"
	%do def_setting  _IVAR="NOSERIAL"
	%do def_setting2 _IVAR="NO_MULTIPLAYER" _SVAR="NETWORK"
	%do def_setting2 _IVAR="PIGGY_NO_PAGING" _SVAR="PIGGY_USE_PAGING"
	%if !%exists(settings.h)
	  @copy settings.tmp settings.h >nul
	%else
	  --1 @diff settings.tmp settings.h >____temp.tmp
	  %if ! -z ____temp.tmp
	    @attrib -r settings.h	#make sure we can write to the file
	    @copy settings.tmp settings.h >nul
	  %endif
	%endif
	@del settings.tmp
	@del ____temp.tmp

# Subroutine to add lines to SETTINGS.H
def_setting .PHONY .NODEFAULT:
	#%if !%defined(_SVAR)
	%if "$(_SVAR)" == ""
	  @%set _SVAR = $(_IVAR)
	%endif
	%if "$(_SVAL)" == ""
	  @%set _SVAL = 1
	%endif
	@%if "$(_IVAL)" != ""
	  @%if "$($(_IVAR))" == $(_IVAL)
	    @%echo \#define $(_SVAR) $(_SVAL)				//$(_IVAR) == $(_IVAL)  >>settings.tmp
	  %else
	    @%echo //\#define $(_SVAR)				//$(_IVAR) != $(_IVAL) >>settings.tmp
	  %endif
	%else
	  @%if %defined($(_IVAR))
	    @%echo \#define $(_SVAR) $(_SVAL)				//$(_IVAR) set  >>settings.tmp
	  %else
	    @%echo //\#define $(_SVAR)				//$(_IVAR) not set >>settings.tmp
	  %endif
	%endif
	@%set _SVAR =
	@%set _SVAL =
	@%set _IVAL =
	
# Version of def_settings to add line if variable NOT set (or not equal)
def_setting2 .PHONY .NODEFAULT:
	#%if !%defined(_SVAR)
	%if "$(_SVAR)" == ""
	  @%set _SVAR = $(_IVAR)
	%endif
	%if "$(_SVAL)" == ""
	  @%set _SVAL = 1
	%endif
	@%if "$(_IVAL)" != ""
	  @%if "$($(_IVAR))" != $(_IVAL)
	    @%echo \#define $(_SVAR) $(_SVAL)				//$(_IVAR) != $(_IVAL)  >>settings.tmp
	  %else
	    @%echo //\#define $(_SVAR)				//$(_IVAR) == $(_IVAL) >>settings.tmp
	  %endif
	%else
	  @%if %defined($(_IVAR))
	    @%echo //\#define $(_SVAR)				//$(_IVAR) set >>settings.tmp
	  %else
	    @%echo \#define $(_SVAR) $(_SVAL)				//$(_IVAR) not set  >>settings.tmp
	  %endif
	%endif
	@%set _SVAR =
	@%set _SVAL =
	

#set up serial number and name header file
.BEFORE::
	@%echo // >vers_id.tmp
	@%echo // Auto-generated include file >>vers_id.tmp
	@%echo // Generated by MAKE from $(INI_FILE,UC) settings >>vers_id.tmp
	@%echo // >>vers_id.tmp
	@%echo >>vers_id.tmp
	@%if !%defined(VERSION_NAME) && !%defined(RELEASE_VERSION)
	  @%set VERSION_NAME = "Internal Development ($(USER))"
	@%endif
	@%echo >>vers_id.tmp
	@%if %defined(VERSION_NAME)
	  @%echo \#define VERSION_NAME	$(VERSION_NAME) >>vers_id.tmp
	@%endif
	@%if %defined(SHAREWARE)
	  @%echo \#define VERSION_TYPE	"Interactive Demo" >>vers_id.tmp
	@%else
	  @%if %defined(D2_OEM)
	    @%echo \#define VERSION_TYPE	"Destination:Quartzon" >>vers_id.tmp
	  @%else
	    @%if %defined(COMPILATION)
	      @%echo \#define VERSION_TYPE	"Full Version (Comp)" >>vers_id.tmp
	    @%else
	      @%echo \#define VERSION_TYPE	"Full Version" >>vers_id.tmp
	    @%endif
	  @%endif
	@%endif
	@%echo >>vers_id.tmp
	%if !%exists(vers_id.h)
	  @copy vers_id.tmp vers_id.h >nul
	%else
	  --1 @diff vers_id.tmp vers_id.h >____temp.tmp
	  %if ! -z ____temp.tmp
	    @attrib -r vers_id.h	#make sure we can write to the file
	    @copy vers_id.tmp vers_id.h >nul
	  %endif
	%endif
	# If release version, always touch file to force rebuild of
	# inferno.c to update date/time
	%if %defined(RELEASE_VERSION)
	  @touch vers_id.h
	%endif

	@del vers_id.tmp
	@del ____temp.tmp
	 

	 
#rules and targets common to all Miner makefiles
%include $(INIT)\makefile.def

### OPUS MKMF:  Do not remove this line!  Generated dependencies follow.

3dfx.obj: 3d.h 3dfx_des.h aistruct.h bm.h digi.h fix.h game.h gameseg.h \
	 glide.h glidesys.h glideutl.h gr.h inferno.h mono.h object.h \
	 piggy.h pstypes.h segment.h setjmp.h settings.h sounds.h stdarg.h \
	 stdio.h stdlib.h string.h texmap.h vecmat.h

3dmax.obj: dos.h i86.h inferno.h mono.h pstypes.h settings.h stdio.h \
	 string.h

ai.obj:	 3d.h ai.h aistruct.h bm.h cntrlcen.h collide.h config.h controls.h \
	 digi.h editor.h effects.h error.h fireball.h fix.h fuelcen.h fvi.h \
	 game.h gameseg.h gameseq.h gamestat.h gauges.h gr.h inferno.h \
	 kconfig.h key.h laser.h mission.h mono.h morph.h multi.h \
	 multibot.h network.h object.h physics.h piggy.h player.h polyobj.h \
	 powerup.h pstypes.h render.h robot.h segment.h setjmp.h settings.h \
	 sounds.h stdio.h stdlib.h string.h text.h time.h timer.h vclip.h \
	 vecmat.h wall.h weapon.h

ai2.obj: 3d.h ai.h aistruct.h bm.h cntrlcen.h collide.h digi.h editor.h \
	 effects.h error.h fireball.h fix.h fvi.h game.h gameseg.h \
	 gameseq.h gauges.h gr.h inferno.h kdefs.h key.h laser.h mission.h \
	 mono.h morph.h multi.h multibot.h network.h object.h physics.h \
	 piggy.h player.h polyobj.h powerup.h pstypes.h render.h robot.h \
	 segment.h setjmp.h settings.h sounds.h stdio.h stdlib.h string.h \
	 text.h time.h timer.h vclip.h vecmat.h wall.h weapon.h

aipath.obj: 3d.h ai.h aistruct.h digi.h editor.h error.h fireball.h fix.h \
	 fvi.h game.h gameseg.h gr.h inferno.h mono.h object.h physics.h \
	 piggy.h player.h pstypes.h robot.h segment.h setjmp.h settings.h \
	 sounds.h stdio.h stdlib.h string.h vecmat.h wall.h weapon.h

arcade.obj: 3d.h aistruct.h arcade.h config.h ctype.h digi.h dos.h error.h \
	 fix.h game.h gamefont.h gameseg.h gamestat.h gauges.h gr.h i86.h \
	 iff.h inferno.h io.h joy.h joydefs.h kconfig.h key.h mem.h mono.h \
	 mouse.h object.h palette.h piggy.h player.h pstypes.h render.h \
	 segment.h setjmp.h settings.h sounds.h stat.h stdarg.h stdio.h \
	 stdlib.h string.h types.h vecmat.h vfx1.h weapon.h

automap.obj: 3d.h aistruct.h automap.h bm.h cderr.h cguid.h cntrlcen.h \
	 commdlg.h config.h ctype.h dd.h dde.h ddeml.h ddraw.h desw.h \
	 digi.h dlgs.h endlevel.h error.h excpt.h fix.h game.h gamefont.h \
	 gameseg.h gameseq.h gamestat.h gauges.h gr.h iff.h imm.h inferno.h \
	 joy.h kconfig.h key.h lzexpand.h math.h mcx.h mem.h menu.h \
	 mission.h mmsystem.h mono.h mouse.h multi.h nb30.h network.h \
	 newmenu.h oaidl.h objbase.h object.h objidl.h ole.h ole2.h \
	 oleauto.h oleidl.h pa_enabl.h palette.h pcx.h piggy.h player.h \
	 poly_acc.h polyobj.h poppack.h powerup.h prsht.h pshpack1.h \
	 pshpack2.h pshpack4.h pshpack8.h pstypes.h render.h robot.h rpc.h \
	 rpcdce.h rpcdcep.h rpcndr.h rpcnsi.h rpcnsip.h rpcnterr.h \
	 screens.h segment.h segpoint.h setjmp.h settings.h shellapi.h \
	 songs.h sounds.h stdarg.h stdio.h stdlib.h string.h switch.h \
	 text.h textures.h timer.h unknwn.h vclip.h vecmat.h vga.h wall.h \
	 weapon.h winapp.h winbase.h wincon.h windef.h windows.h winerror.h \
	 wingdi.h winnetwk.h winnls.h winnt.h winperf.h winreg.h winsock.h \
	 winspool.h winsvc.h winuser.h winver.h wtypes.h

bm.obj:	 3d.h aistruct.h bm.h byteswap.h cfile.h cntrlcen.h digi.h \
	 effects.h endlevel.h error.h fix.h game.h gameseg.h gameseq.h \
	 gauges.h gr.h iff.h inferno.h mem.h mission.h mono.h multi.h \
	 object.h piggy.h player.h polyobj.h powerup.h pstypes.h robot.h \
	 segment.h setjmp.h settings.h sounds.h stdio.h stdlib.h textures.h \
	 vclip.h vecmat.h wall.h weapon.h

bmread.obj: 3d.h aistruct.h args.h bm.h cfile.h cntrlcen.h compbit.h \
	 ctype.h digi.h effects.h endlevel.h error.h fix.h game.h gamepal.h \
	 gameseg.h gameseq.h gauges.h gr.h hostage.h iff.h inferno.h \
	 laser.h mem.h mission.h mono.h multi.h object.h piggy.h player.h \
	 polyobj.h powerup.h pstypes.h robot.h segment.h setjmp.h \
	 settings.h sounds.h stdarg.h stdio.h stdlib.h string.h texpage.h \
	 textures.h vclip.h vecmat.h wall.h weapon.h

cdmix32.obj: cderr.h cguid.h commdlg.h conio.h ctype.h dde.h ddeml.h dlgs.h \
	 error.h excpt.h imm.h inferno.h lzexpand.h mcx.h mmsystem.h mono.h \
	 nb30.h oaidl.h objbase.h objidl.h ole.h ole2.h oleauto.h oleidl.h \
	 poppack.h prsht.h pshpack1.h pshpack2.h pshpack4.h pshpack8.h \
	 pstypes.h rpc.h rpcdce.h rpcdcep.h rpcndr.h rpcnsi.h rpcnsip.h \
	 rpcnterr.h settings.h shellapi.h sos.h sosfnct.h stdarg.h stdio.h \
	 stdlib.h string.h unknwn.h winbase.h wincon.h windef.h windows.h \
	 windowsx.h winerror.h wingdi.h winnetwk.h winnls.h winnt.h \
	 winperf.h winreg.h winsock.h winspool.h winsvc.h winuser.h \
	 winver.h wtypes.h

cdrom.obj: dos.h dpmi.h i86.h pstypes.h stdio.h stdlib.h string.h

cntrlcen.obj: ai.h aistruct.h cderr.h cguid.h cntrlcen.h commdlg.h ctype.h \
	 dd.h dde.h ddeml.h ddraw.h desw.h digi.h dlgs.h endlevel.h error.h \
	 excpt.h fireball.h fix.h fvi.h game.h gameseg.h gameseq.h gr.h \
	 imm.h inferno.h io.h laser.h lzexpand.h mcx.h mission.h mmsystem.h \
	 mono.h multi.h nb30.h oaidl.h objbase.h object.h objidl.h ole.h \
	 ole2.h oleauto.h oleidl.h piggy.h player.h poppack.h prsht.h \
	 pshpack1.h pshpack2.h pshpack4.h pshpack8.h pstypes.h robot.h \
	 rpc.h rpcdce.h rpcdcep.h rpcndr.h rpcnsi.h rpcnsip.h rpcnterr.h \
	 segment.h setjmp.h settings.h shellapi.h sounds.h stat.h stdarg.h \
	 stdio.h stdlib.h string.h types.h unistd.h unknwn.h vclip.h \
	 vecmat.h wall.h weapon.h winapp.h winbase.h wincon.h windef.h \
	 windows.h winerror.h wingdi.h winnetwk.h winnls.h winnt.h \
	 winperf.h winreg.h winsock.h winspool.h winsvc.h winuser.h \
	 winver.h wtypes.h

coindev.obj: coindev.h conio.h dos.h i86.h stdio.h

collide.obj: 3d.h ai.h aistruct.h automap.h bm.h cntrlcen.h collide.h \
	 digi.h editor.h effects.h endlevel.h error.h fireball.h fix.h \
	 fuelcen.h fvi.h game.h gameseg.h gameseq.h gauges.h gr.h hostage.h \
	 inferno.h key.h laser.h mission.h mono.h multi.h multibot.h \
	 network.h newdemo.h newmenu.h object.h palette.h physics.h piggy.h \
	 player.h polyobj.h powerup.h pstypes.h render.h rle.h robot.h \
	 scores.h segment.h setjmp.h settings.h slew.h sounds.h stdio.h \
	 stdlib.h string.h switch.h tactile.h texmap.h text.h textures.h \
	 vclip.h vecmat.h wall.h weapon.h

config.obj: aistruct.h args.h cderr.h cguid.h commdlg.h config.h conio.h \
	 ctype.h dde.h ddeml.h digi.h dlgs.h dos.h error.h excpt.h fix.h \
	 game.h gameseg.h gamestat.h gr.h i86.h imm.h inferno.h joy.h \
	 kconfig.h lzexpand.h mcx.h memory.h menu.h mission.h mmsystem.h \
	 mono.h movie.h nb30.h oaidl.h objbase.h object.h objidl.h ole.h \
	 ole2.h oleauto.h oleidl.h pa_enabl.h palette.h piggy.h player.h \
	 poly_acc.h poppack.h prsht.h pshpack1.h pshpack2.h pshpack4.h \
	 pshpack8.h pstypes.h rpc.h rpcdce.h rpcdcep.h rpcndr.h rpcnsi.h \
	 rpcnsip.h rpcnterr.h segment.h setjmp.h settings.h shellapi.h \
	 songs.h sos.h sosfnct.h sosm.h sosmfnct.h sounds.h stdarg.h \
	 stdio.h stdlib.h string.h unknwn.h vecmat.h weapon.h winapp.h \
	 winbase.h wincon.h windef.h windows.h windowsx.h winerror.h \
	 wingdi.h winnetwk.h winnls.h winnt.h winperf.h winreg.h winsock.h \
	 winspool.h winsvc.h winuser.h winver.h wtypes.h

controls.obj: 3d.h aistruct.h args.h config.h controls.h digi.h error.h \
	 fireball.h fix.h game.h gameseg.h gameseq.h gamestat.h gr.h \
	 inferno.h joy.h joydefs.h kconfig.h key.h laser.h mission.h mono.h \
	 mouse.h multi.h object.h palette.h piggy.h player.h pstypes.h \
	 render.h segment.h setjmp.h settings.h sounds.h stdlib.h timer.h \
	 vclip.h vecmat.h victor.h weapon.h

credits.obj: aistruct.h cderr.h cfile.h cguid.h commdlg.h compbit.h ctype.h \
	 dd.h dde.h ddeml.h ddraw.h desw.h digi.h dlgs.h error.h excpt.h \
	 fix.h game.h gamefont.h gamepal.h gameseg.h gameseq.h gr.h iff.h \
	 imm.h inferno.h joy.h key.h lzexpand.h mcx.h mem.h menu.h \
	 mission.h mmsystem.h mono.h mouse.h multi.h nb30.h network.h \
	 newmenu.h oaidl.h objbase.h object.h objidl.h ole.h ole2.h \
	 oleauto.h oleidl.h pa_enabl.h palette.h pcx.h piggy.h player.h \
	 poly_acc.h poppack.h prsht.h pshpack1.h pshpack2.h pshpack4.h \
	 pshpack8.h pstypes.h rpc.h rpcdce.h rpcdcep.h rpcndr.h rpcnsi.h \
	 rpcnsip.h rpcnterr.h screens.h segment.h setjmp.h settings.h \
	 shellapi.h songs.h sounds.h stdarg.h stdio.h stdlib.h string.h \
	 timer.h unknwn.h vecmat.h weapon.h winapp.h winbase.h wincon.h \
	 windef.h windows.h winerror.h wingdi.h winnetwk.h winnls.h winnt.h \
	 winperf.h winreg.h winsock.h winspool.h winsvc.h winuser.h \
	 winver.h wtypes.h

crypt.obj: inferno.h pstypes.h settings.h stdlib.h string.h time.h

descentw.obj: aistruct.h cderr.h cguid.h cntrlcen.h commdlg.h config.h \
	 ctype.h dd.h dde.h ddeml.h ddraw.h desw.h digi.h dlgs.h dsound.h \
	 error.h excpt.h fix.h game.h gamepal.h gameseg.h gr.h imm.h \
	 inferno.h key.h lzexpand.h mcx.h menu.h mmsystem.h modem.h mono.h \
	 mvelibw.h nb30.h newdemo.h oaidl.h objbase.h object.h objidl.h \
	 ole.h ole2.h oleauto.h oleidl.h piggy.h player.h poppack.h prsht.h \
	 pshpack1.h pshpack2.h pshpack4.h pshpack8.h pstypes.h rpc.h \
	 rpcdce.h rpcdcep.h rpcndr.h rpcnsi.h rpcnsip.h rpcnterr.h \
	 screens.h segment.h setjmp.h settings.h shellapi.h songs.h \
	 sounds.h stdarg.h stdio.h stdlib.h string.h text.h unknwn.h \
	 vecmat.h vers_id.h wall.h weapon.h winapp.h winbase.h wincon.h \
	 windef.h windows.h winerror.h wingdi.h winnetwk.h winnls.h winnt.h \
	 winperf.h winreg.h winsock.h winspool.h winsvc.h winuser.h \
	 winver.h wtypes.h

digi.obj: aistruct.h args.h cderr.h cfile.h cguid.h commdlg.h config.h \
	 ctype.h dde.h ddeml.h digi.h dlgs.h dpmi.h error.h excpt.h fix.h \
	 game.h gameseg.h gr.h imm.h inferno.h joy.h key.h lzexpand.h mcx.h \
	 mmsystem.h mono.h nb30.h oaidl.h objbase.h object.h objidl.h ole.h \
	 ole2.h oleauto.h oleidl.h piggy.h player.h poppack.h prsht.h \
	 pshpack1.h pshpack2.h pshpack4.h pshpack8.h pstypes.h rpc.h \
	 rpcdce.h rpcdcep.h rpcndr.h rpcnsi.h rpcnsip.h rpcnterr.h \
	 segment.h setjmp.h settings.h shellapi.h sos.h sosfnct.h sosm.h \
	 sosmfnct.h sounds.h stdarg.h stdio.h stdlib.h string.h text.h \
	 timer.h unknwn.h vecmat.h weapon.h winbase.h wincon.h windef.h \
	 windows.h windowsx.h winerror.h wingdi.h winnetwk.h winnls.h \
	 winnt.h winperf.h winreg.h winsock.h winspool.h winsvc.h winuser.h \
	 winver.h wtypes.h

digiobj.obj: aistruct.h args.h cfile.h config.h digi.h dpmi.h error.h fix.h \
	 game.h gameseg.h gamestat.h gr.h inferno.h joy.h kconfig.h key.h \
	 mono.h newdemo.h object.h piggy.h player.h pstypes.h segment.h \
	 setjmp.h settings.h sounds.h stdio.h text.h timer.h vecmat.h \
	 wall.h weapon.h

dumpmine.obj: 3d.h aistruct.h bm.h digi.h editor.h errno.h error.h fix.h \
	 fuelcen.h game.h gamemine.h gamesave.h gameseg.h gameseq.h gr.h \
	 inferno.h key.h menu.h mission.h mono.h newmenu.h object.h \
	 palette.h piggy.h player.h polyobj.h powerup.h pstypes.h robot.h \
	 segment.h setjmp.h settings.h sounds.h stdarg.h stdio.h string.h \
	 switch.h textures.h vclip.h vecmat.h wall.h weapon.h

effects.obj: aistruct.h bm.h cntrlcen.h digi.h effects.h error.h fix.h \
	 game.h gameseg.h gr.h inferno.h mem.h mono.h object.h piggy.h \
	 pstypes.h segment.h setjmp.h settings.h sounds.h stdarg.h stdio.h \
	 stdlib.h string.h textures.h vclip.h vecmat.h wall.h

endlevel.obj: 3d.h aistruct.h bm.h cfile.h compbit.h ctype.h digi.h \
	 endlevel.h error.h fireball.h fix.h fvi.h game.h gameseg.h \
	 gameseq.h gauges.h gr.h iff.h inferno.h joy.h key.h mem.h \
	 mission.h mono.h movie.h multi.h network.h newdemo.h object.h \
	 palette.h piggy.h player.h polyobj.h pstypes.h render.h robot.h \
	 screens.h segment.h setjmp.h settings.h songs.h sounds.h stdio.h \
	 stdlib.h string.h terrain.h texmap.h text.h vclip.h vecmat.h \
	 wall.h weapon.h

escort.obj: 3d.h ai.h aistruct.h automap.h cderr.h cguid.h cntrlcen.h \
	 commdlg.h ctype.h dd.h dde.h ddeml.h ddraw.h desw.h digi.h dlgs.h \
	 editor.h error.h excpt.h fireball.h fix.h fuelcen.h fvi.h game.h \
	 gamefont.h gameseg.h gameseq.h gauges.h gr.h imm.h inferno.h key.h \
	 laser.h lzexpand.h mcx.h mission.h mmsystem.h mono.h nb30.h \
	 newmenu.h oaidl.h objbase.h object.h objidl.h ole.h ole2.h \
	 oleauto.h oleidl.h pa_enabl.h palette.h physics.h piggy.h player.h \
	 playsave.h poppack.h powerup.h prsht.h pshpack1.h pshpack2.h \
	 pshpack4.h pshpack8.h pstypes.h robot.h rpc.h rpcdce.h rpcdcep.h \
	 rpcndr.h rpcnsi.h rpcnsip.h rpcnterr.h screens.h segment.h \
	 setjmp.h settings.h shellapi.h sounds.h stdarg.h stdio.h stdlib.h \
	 string.h text.h unknwn.h vclip.h vecmat.h wall.h weapon.h winapp.h \
	 winbase.h wincon.h windef.h windows.h winerror.h wingdi.h \
	 winnetwk.h winnls.h winnt.h winperf.h winreg.h winsock.h \
	 winspool.h winsvc.h winuser.h winver.h wtypes.h

fireball.obj: 3d.h ai.h aistruct.h automap.h bm.h cntrlcen.h collide.h \
	 digi.h endlevel.h error.h fireball.h fix.h fuelcen.h fvi.h game.h \
	 gameseg.h gameseq.h gauges.h gr.h inferno.h laser.h mission.h \
	 mono.h multi.h network.h newmenu.h object.h physics.h piggy.h \
	 player.h polyobj.h powerup.h pstypes.h robot.h scores.h segment.h \
	 setjmp.h settings.h sounds.h stdio.h stdlib.h string.h timer.h \
	 vclip.h vecmat.h wall.h weapon.h

fuelcen.obj: 3d.h ai.h aistruct.h bm.h collide.h digi.h error.h fireball.h \
	 fix.h fuelcen.h fvi.h game.h gamemine.h gamesave.h gameseg.h \
	 gameseq.h gauges.h gr.h inferno.h laser.h math.h mission.h mono.h \
	 morph.h multi.h multibot.h network.h object.h piggy.h player.h \
	 polyobj.h powerup.h pstypes.h robot.h segment.h setjmp.h \
	 settings.h sounds.h stdio.h stdlib.h string.h vclip.h vecmat.h \
	 wall.h weapon.h

fvi.obj: aistruct.h bm.h digi.h error.h fix.h fvi.h game.h gameseg.h gr.h \
	 inferno.h laser.h malloc.h memory.h mono.h object.h piggy.h \
	 player.h pstypes.h rle.h robot.h segment.h setjmp.h settings.h \
	 sounds.h stdlib.h string.h texmerge.h textures.h vecmat.h wall.h \
	 weapon.h

game.obj: 3d.h 3dmax.h ai.h aistruct.h arcade.h args.h automap.h bm.h \
	 cderr.h cguid.h cntrlcen.h coindev.h collide.h commdlg.h config.h \
	 controls.h ctype.h dd.h dde.h ddeml.h ddraw.h desc_id.h desw.h \
	 digi.h dlgs.h dpmi.h editor.h effect2d.h effects.h endlevel.h \
	 error.h excpt.h files.h fireball.h fix.h fuelcen.h fvi.h game.h \
	 gamefont.h gamepal.h gameseg.h gameseq.h gamestat.h gauges.h gr.h \
	 ibitblt.h iff.h imm.h inferno.h joy.h joydefs.h kconfig.h key.h \
	 laser.h lighting.h lzexpand.h malloc.h mcx.h mem.h menu.h \
	 mission.h mmsystem.h mono.h morph.h mouse.h multi.h multibot.h \
	 nb30.h network.h newdemo.h newmenu.h oaidl.h objbase.h object.h \
	 objidl.h ole.h ole2.h oleauto.h oleidl.h pa_enabl.h palette.h \
	 pcx.h physics.h piggy.h player.h playsave.h poly_acc.h poppack.h \
	 powerup.h prsht.h pshpack1.h pshpack2.h pshpack4.h pshpack8.h \
	 pstypes.h rbaudio.h render.h robot.h rpc.h rpcdce.h rpcdcep.h \
	 rpcndr.h rpcnsi.h rpcnsip.h rpcnterr.h screens.h segment.h \
	 setjmp.h settings.h shellapi.h slew.h songs.h sounds.h state.h \
	 stdarg.h stdio.h stdlib.h string.h switch.h texmap.h text.h \
	 textures.h time.h timer.h unknwn.h vclip.h vecmat.h vfx.h vga.h \
	 victor.h wall.h weapon.h winapp.h winbase.h wincon.h windef.h \
	 windows.h winerror.h wingdi.h winnetwk.h winnls.h winnt.h \
	 winperf.h winreg.h winsock.h winspool.h winsvc.h winuser.h \
	 winver.h wtypes.h

gamecntl.obj: 3d.h ai.h aistruct.h arcade.h args.h automap.h bm.h cderr.h \
	 cguid.h cntrlcen.h coindev.h commdlg.h config.h ctype.h dd.h dde.h \
	 ddeml.h ddraw.h desc_id.h desw.h digi.h dlgs.h dpmi.h editor.h \
	 effect2d.h effects.h endlevel.h error.h excpt.h fix.h fvi.h game.h \
	 gamefont.h gameseg.h gameseq.h gamestat.h gauges.h gr.h ibitblt.h \
	 iff.h imm.h inferno.h joy.h joydefs.h kconfig.h key.h laser.h \
	 lighting.h lzexpand.h malloc.h mcx.h mem.h menu.h mission.h \
	 mmsystem.h mono.h morph.h mouse.h movie.h multi.h multibot.h \
	 nb30.h network.h newdemo.h newmenu.h oaidl.h objbase.h object.h \
	 objidl.h ole.h ole2.h oleauto.h oleidl.h pa_enabl.h palette.h \
	 pcx.h physics.h piggy.h player.h playsave.h poly_acc.h poppack.h \
	 powerup.h prsht.h pshpack1.h pshpack2.h pshpack4.h pshpack8.h \
	 pstypes.h rbaudio.h render.h robot.h rpc.h rpcdce.h rpcdcep.h \
	 rpcndr.h rpcnsi.h rpcnsip.h rpcnterr.h scores.h screens.h \
	 segment.h setjmp.h settings.h shellapi.h slew.h songs.h sounds.h \
	 state.h stdarg.h stdio.h stdlib.h string.h switch.h tactile.h \
	 texmap.h text.h textures.h time.h timer.h titles.h unknwn.h \
	 vclip.h vecmat.h victor.h wall.h weapon.h winapp.h winbase.h \
	 wincon.h windef.h windows.h winerror.h wingdi.h winnetwk.h \
	 winnls.h winnt.h winperf.h winreg.h winsock.h winspool.h winsvc.h \
	 winuser.h winver.h wtypes.h

gamefont.obj: fix.h gamefont.h gr.h inferno.h pstypes.h settings.h stdlib.h

gamemine.obj: aistruct.h bm.h byteswap.h cfile.h digi.h editor.h error.h \
	 fix.h fuelcen.h game.h gamemine.h gameseg.h gr.h hash.h inferno.h \
	 key.h math.h mono.h newmenu.h object.h piggy.h pstypes.h segment.h \
	 setjmp.h settings.h sounds.h stdio.h stdlib.h string.h switch.h \
	 textures.h vecmat.h wall.h

gamepal.obj: 3d.h digi.h fix.h gamepal.h gr.h inferno.h math.h mission.h \
	 newmenu.h pa_enabl.h palette.h piggy.h poly_acc.h pstypes.h rle.h \
	 settings.h sounds.h stdlib.h string.h texmerge.h vecmat.h

gamerend.obj: 3d.h aistruct.h automap.h bm.h cderr.h cguid.h cntrlcen.h \
	 commdlg.h ctype.h dd.h dde.h ddeml.h ddraw.h desw.h digi.h dlgs.h \
	 endlevel.h error.h excpt.h fix.h game.h gamefont.h gameseg.h \
	 gameseq.h gauges.h gr.h ibitblt.h imm.h inferno.h laser.h \
	 lzexpand.h mcx.h menu.h mission.h mmsystem.h mono.h multi.h nb30.h \
	 newdemo.h newmenu.h oaidl.h objbase.h object.h objidl.h ole.h \
	 ole2.h oleauto.h oleidl.h pa_enabl.h palette.h piggy.h player.h \
	 playsave.h poly_acc.h poppack.h powerup.h prsht.h pshpack1.h \
	 pshpack2.h pshpack4.h pshpack8.h pstypes.h render.h robot.h rpc.h \
	 rpcdce.h rpcdcep.h rpcndr.h rpcnsi.h rpcnsip.h rpcnterr.h \
	 screens.h segment.h setjmp.h settings.h shellapi.h sounds.h \
	 stdarg.h stdio.h stdlib.h string.h text.h unknwn.h vclip.h \
	 vecmat.h vfx.h wall.h weapon.h winapp.h winbase.h wincon.h \
	 windef.h windows.h winerror.h wingdi.h winnetwk.h winnls.h winnt.h \
	 winperf.h winreg.h winsock.h winspool.h winsvc.h winuser.h \
	 winver.h wtypes.h

gamesave.obj: 3d.h aistruct.h automap.h bm.h byteswap.h cfile.h cntrlcen.h \
	 digi.h editor.h error.h fix.h fuelcen.h game.h gamefont.h \
	 gamemine.h gamepal.h gamesave.h gameseg.h gameseq.h gr.h inferno.h \
	 key.h laser.h math.h menu.h mission.h mono.h newdemo.h newmenu.h \
	 object.h palette.h piggy.h player.h polyobj.h powerup.h pstypes.h \
	 robot.h screens.h segment.h setjmp.h settings.h sounds.h stdio.h \
	 stdlib.h string.h switch.h text.h vclip.h vecmat.h wall.h weapon.h

gameseg.obj: aistruct.h bm.h byteswap.h digi.h error.h fix.h fuelcen.h \
	 fvi.h game.h gameseg.h gr.h inferno.h malloc.h memory.h mono.h \
	 object.h piggy.h pstypes.h segment.h setjmp.h settings.h sounds.h \
	 stdio.h stdlib.h string.h vecmat.h wall.h

gameseq.obj: 3d.h ai.h aistruct.h arcade.h args.h automap.h bm.h cderr.h \
	 cfile.h cguid.h cntrlcen.h collide.h commdlg.h config.h controls.h \
	 ctype.h dd.h dde.h ddeml.h ddraw.h desw.h digi.h dlgs.h editor.h \
	 effect2d.h effects.h endlevel.h errno.h error.h excpt.h fireball.h \
	 fix.h fuelcen.h fvi.h game.h gamefont.h gamepal.h gamesave.h \
	 gameseg.h gameseq.h gamestat.h gauges.h gr.h ibitblt.h iff.h imm.h \
	 inferno.h io.h joy.h kconfig.h key.h laser.h lighting.h lzexpand.h \
	 malloc.h mcx.h mem.h menu.h mission.h mmsystem.h modem.h mono.h \
	 morph.h movie.h multi.h nb30.h netmisc.h network.h newdemo.h \
	 newmenu.h oaidl.h objbase.h object.h objidl.h ole.h ole2.h \
	 oleauto.h oleidl.h pa_enabl.h paging.h palette.h pcx.h physics.h \
	 piggy.h player.h playsave.h poly_acc.h poppack.h powerup.h prsht.h \
	 pshpack1.h pshpack2.h pshpack4.h pshpack8.h pstypes.h render.h \
	 robot.h rpc.h rpcdce.h rpcdcep.h rpcndr.h rpcnsi.h rpcnsip.h \
	 rpcnterr.h scores.h screens.h segment.h setjmp.h settings.h \
	 shellapi.h slew.h songs.h sounds.h stat.h state.h stdarg.h stdio.h \
	 stdlib.h string.h switch.h tactile.h texmap.h texmerge.h text.h \
	 textures.h time.h timer.h titles.h types.h unistd.h unknwn.h \
	 vclip.h vecmat.h wall.h weapon.h winapp.h winbase.h wincon.h \
	 windef.h windows.h winerror.h wingdi.h winnetwk.h winnls.h winnt.h \
	 winperf.h winreg.h winsock.h winspool.h winsvc.h winuser.h \
	 winver.h wtypes.h

gauges.obj: 3d.h aistruct.h bm.h cderr.h cguid.h cntrlcen.h collide.h \
	 commdlg.h controls.h ctype.h dd.h dde.h ddeml.h ddraw.h desw.h \
	 digi.h dlgs.h endlevel.h error.h excpt.h fix.h fvi.h game.h \
	 gamefont.h gameseg.h gameseq.h gauges.h gr.h imm.h inferno.h key.h \
	 laser.h lzexpand.h mcx.h menu.h mission.h mmsystem.h mono.h \
	 multi.h nb30.h network.h newdemo.h oaidl.h objbase.h object.h \
	 objidl.h ole.h ole2.h oleauto.h oleidl.h pa_enabl.h physics.h \
	 piggy.h player.h poly_acc.h poppack.h powerup.h prsht.h pshpack1.h \
	 pshpack2.h pshpack4.h pshpack8.h pstypes.h render.h rpc.h rpcdce.h \
	 rpcdcep.h rpcndr.h rpcnsi.h rpcnsip.h rpcnterr.h screens.h \
	 segment.h setjmp.h settings.h shellapi.h sounds.h stdarg.h stdio.h \
	 stdlib.h string.h text.h unknwn.h vclip.h vecmat.h wall.h weapon.h \
	 winapp.h winbase.h wincon.h windef.h windows.h winerror.h wingdi.h \
	 winnetwk.h winnls.h winnt.h winperf.h winreg.h winsock.h \
	 winspool.h winsvc.h winuser.h winver.h wtypes.h

hostage.obj: aistruct.h digi.h error.h fix.h game.h gameseg.h gauges.h gr.h \
	 hostage.h inferno.h newdemo.h object.h piggy.h player.h pstypes.h \
	 segment.h setjmp.h settings.h sounds.h stdio.h stdlib.h string.h \
	 text.h vclip.h vecmat.h weapon.h

hud.obj: aistruct.h arcade.h args.h cderr.h cguid.h collide.h commdlg.h \
	 ctype.h dd.h dde.h ddeml.h ddraw.h desw.h digi.h dlgs.h error.h \
	 excpt.h fix.h fvi.h game.h gamefont.h gameseg.h gauges.h gr.h \
	 imm.h inferno.h laser.h lzexpand.h mcx.h menu.h mmsystem.h mono.h \
	 nb30.h newdemo.h oaidl.h objbase.h object.h objidl.h ole.h ole2.h \
	 oleauto.h oleidl.h pa_enabl.h physics.h piggy.h player.h poppack.h \
	 prsht.h pshpack1.h pshpack2.h pshpack4.h pshpack8.h pstypes.h \
	 rpc.h rpcdce.h rpcdcep.h rpcndr.h rpcnsi.h rpcnsip.h rpcnterr.h \
	 screens.h segment.h setjmp.h settings.h shellapi.h sounds.h \
	 stdarg.h stdio.h stdlib.h string.h text.h unknwn.h vecmat.h wall.h \
	 weapon.h winapp.h winbase.h wincon.h windef.h windows.h winerror.h \
	 wingdi.h winnetwk.h winnls.h winnt.h winperf.h winreg.h winsock.h \
	 winspool.h winsvc.h winuser.h winver.h wtypes.h

iglasses.obj: _debug.h _defs286.h _defs386.h args.h cderr.h cguid.h \
	 commdlg.h commlib.h compiler.h conio.h ctype.h dde.h ddeml.h \
	 dlgs.h dos.h error.h excpt.h fast.h fix.h i86.h iglasses.h imm.h \
	 inferno.h key.h lzexpand.h malloc.h math.h mcx.h mmsystem.h mono.h \
	 nb30.h oaidl.h objbase.h objidl.h ole.h ole2.h oleauto.h oleidl.h \
	 poppack.h prsht.h pshpack1.h pshpack2.h pshpack4.h pshpack8.h \
	 pstypes.h rpc.h rpcdce.h rpcdcep.h rpcndr.h rpcnsi.h rpcnsip.h \
	 rpcnterr.h settings.h shellapi.h stdarg.h stddef.h stdio.h \
	 stdlib.h string.h text.h timer.h unknwn.h winbase.h wincon.h \
	 windef.h windows.h winerror.h wingdi.h winnetwk.h winnls.h winnt.h \
	 winperf.h winreg.h winsock.h winspool.h winsvc.h winuser.h \
	 winver.h wtypes.h

inferno.obj: 3d.h 3dfx_des.h aistruct.h arcade.h args.h bm.h cfile.h \
	 coindev.h compbit.h config.h conio.h ctype.h desc_id.h digi.h \
	 direct.h div0.h dos.h dpmi.h editor.h effects.h error.h fix.h \
	 game.h gamefont.h gamepal.h gameseg.h gameseq.h gamestat.h glide.h \
	 glidesys.h glideutl.h gr.h i86.h iff.h iglasses.h inferno.h io.h \
	 ipx.h joy.h joydefs.h kconfig.h kdefs.h key.h malloc.h math.h \
	 mem.h menu.h mission.h modem.h mono.h mouse.h movie.h multi.h \
	 network.h newdemo.h newmenu.h object.h pa_enabl.h palette.h pcx.h \
	 piggy.h player.h poly_acc.h polyobj.h pstypes.h robot.h screens.h \
	 segment.h segpoint.h setjmp.h settings.h songs.h sounds.h stat.h \
	 stdio.h stdlib.h string.h tactile.h texmap.h texmerge.h text.h \
	 time.h timer.h titles.h types.h unistd.h vclip.h vecmat.h \
	 vers_id.h vfx.h vga.h victor.h wall.h weapon.h

joydefs.obj: 3d.h aistruct.h args.h config.h controls.h digi.h dos.h \
	 error.h fix.h game.h gameseg.h gamestat.h gr.h i86.h inferno.h \
	 joy.h joydefs.h kconfig.h key.h mono.h newmenu.h object.h \
	 palette.h piggy.h player.h playsave.h pstypes.h render.h segment.h \
	 setjmp.h settings.h sounds.h stdio.h stdlib.h string.h text.h \
	 timer.h vecmat.h victor.h weapon.h

joydefsw.obj: 3d.h aistruct.h args.h cderr.h cguid.h commdlg.h config.h \
	 controls.h ctype.h dd.h dde.h ddeml.h ddraw.h desw.h digi.h dlgs.h \
	 dos.h error.h excpt.h fix.h game.h gamefont.h gameseg.h gamestat.h \
	 gr.h i86.h imm.h inferno.h joy.h joydefs.h kconfig.h key.h \
	 lzexpand.h mcx.h mmsystem.h mono.h nb30.h newmenu.h oaidl.h \
	 objbase.h object.h objidl.h ole.h ole2.h oleauto.h oleidl.h \
	 palette.h piggy.h player.h playsave.h poppack.h prsht.h pshpack1.h \
	 pshpack2.h pshpack4.h pshpack8.h pstypes.h render.h rpc.h rpcdce.h \
	 rpcdcep.h rpcndr.h rpcnsi.h rpcnsip.h rpcnterr.h screens.h \
	 segment.h setjmp.h settings.h shellapi.h sounds.h stdarg.h stdio.h \
	 stdlib.h string.h text.h timer.h unknwn.h vecmat.h victor.h \
	 weapon.h winapp.h winbase.h wincon.h windef.h windows.h winerror.h \
	 wingdi.h winnetwk.h winnls.h winnt.h winperf.h winreg.h winsock.h \
	 winspool.h winsvc.h winuser.h winver.h wtypes.h

kconfig.obj: 3d.h ai.h aistruct.h arcade.h args.h automap.h cderr.h cguid.h \
	 cntrlcen.h commdlg.h config.h ctype.h dd.h dde.h ddeml.h ddraw.h \
	 desw.h digi.h dlgs.h dos.h endlevel.h error.h excpt.h fix.h fvi.h \
	 game.h gamefont.h gameseg.h gameseq.h gamestat.h gauges.h gr.h \
	 i86.h iff.h iglasses.h imm.h inferno.h io.h joy.h joydefs.h \
	 kconfig.h key.h lighting.h lzexpand.h mcx.h mem.h menu.h mission.h \
	 mmsystem.h mono.h mouse.h multi.h nb30.h newmenu.h oaidl.h \
	 objbase.h object.h objidl.h ole.h ole2.h oleauto.h oleidl.h \
	 pa_enabl.h palette.h piggy.h player.h poly_acc.h poppack.h prsht.h \
	 pshpack1.h pshpack2.h pshpack4.h pshpack8.h pstypes.h render.h \
	 robot.h rpc.h rpcdce.h rpcdcep.h rpcndr.h rpcnsi.h rpcnsip.h \
	 rpcnterr.h screens.h segment.h setjmp.h settings.h shellapi.h \
	 songs.h sounds.h stat.h stdarg.h stdio.h stdlib.h string.h \
	 tactile.h text.h timer.h types.h unknwn.h vecmat.h vfx.h victor.h \
	 wall.h weapon.h winapp.h winbase.h wincon.h windef.h windows.h \
	 winerror.h wingdi.h winnetwk.h winnls.h winnt.h winperf.h winreg.h \
	 winsock.h winspool.h winsvc.h winuser.h winver.h wtypes.h

kmatrix.obj: aistruct.h cderr.h cguid.h cntrlcen.h commdlg.h ctype.h dd.h \
	 dde.h ddeml.h ddraw.h desw.h digi.h dlgs.h error.h excpt.h fix.h \
	 game.h gamefont.h gameseg.h gameseq.h gauges.h gr.h imm.h \
	 inferno.h joy.h key.h kmatrix.h lzexpand.h mcx.h mem.h menu.h \
	 mission.h mmsystem.h mono.h mouse.h multi.h nb30.h network.h \
	 newmenu.h oaidl.h objbase.h object.h objidl.h ole.h ole2.h \
	 oleauto.h oleidl.h pa_enabl.h palette.h pcx.h piggy.h player.h \
	 poly_acc.h poppack.h prsht.h pshpack1.h pshpack2.h pshpack4.h \
	 pshpack8.h pstypes.h rpc.h rpcdce.h rpcdcep.h rpcndr.h rpcnsi.h \
	 rpcnsip.h rpcnterr.h screens.h segment.h setjmp.h settings.h \
	 shellapi.h songs.h sounds.h stdarg.h stdio.h stdlib.h string.h \
	 text.h timer.h unknwn.h vecmat.h wall.h weapon.h winapp.h \
	 winbase.h wincon.h windef.h windows.h winerror.h wingdi.h \
	 winnetwk.h winnls.h winnt.h winperf.h winreg.h winsock.h \
	 winspool.h winsvc.h winuser.h winver.h wtypes.h

laser.obj: 3d.h ai.h aistruct.h args.h bm.h digi.h error.h fireball.h fix.h \
	 fvi.h game.h gameseg.h gameseq.h gr.h inferno.h key.h laser.h \
	 mission.h modem.h mono.h multi.h network.h newdemo.h object.h \
	 physics.h piggy.h player.h polyobj.h powerup.h pstypes.h render.h \
	 robot.h segment.h segpoint.h setjmp.h settings.h sounds.h stdio.h \
	 stdlib.h tactile.h texmap.h textures.h timer.h vclip.h vecmat.h \
	 weapon.h

lighting.obj: 3d.h aistruct.h digi.h error.h fix.h fvi.h game.h gameseg.h \
	 gameseq.h gr.h inferno.h laser.h lighting.h mission.h mono.h \
	 multi.h object.h piggy.h player.h powerup.h pstypes.h render.h \
	 robot.h segment.h setjmp.h settings.h sounds.h stdlib.h string.h \
	 timer.h vclip.h vecmat.h weapon.h

menu.obj: 3d.h aistruct.h args.h bm.h cderr.h cguid.h commdlg.h config.h \
	 credits.h ctype.h dd.h dde.h ddeml.h ddraw.h desw.h digi.h dlgs.h \
	 editor.h effects.h errno.h error.h excpt.h fix.h game.h gamefont.h \
	 gamemine.h gamepal.h gamesave.h gameseg.h gameseq.h gamestat.h \
	 gauges.h gr.h iff.h imm.h inferno.h joy.h joydefs.h kconfig.h \
	 key.h lzexpand.h mcx.h mem.h menu.h mission.h mmsystem.h modem.h \
	 mono.h movie.h multi.h nb30.h network.h newdemo.h newmenu.h \
	 oaidl.h objbase.h object.h objidl.h ole.h ole2.h oleauto.h \
	 oleidl.h pa_enabl.h palette.h piggy.h player.h playsave.h \
	 poly_acc.h polyobj.h poppack.h powerup.h prsht.h pshpack1.h \
	 pshpack2.h pshpack4.h pshpack8.h pstypes.h rbaudio.h robot.h rpc.h \
	 rpcdce.h rpcdcep.h rpcndr.h rpcnsi.h rpcnsip.h rpcnterr.h scores.h \
	 screens.h segment.h setjmp.h settings.h shellapi.h slew.h songs.h \
	 sounds.h state.h stdarg.h stdio.h stdlib.h string.h texmap.h \
	 text.h time.h timer.h titles.h unknwn.h vclip.h vecmat.h vga.h \
	 weapon.h winapp.h winbase.h wincon.h windef.h windows.h winerror.h \
	 wingdi.h winnetwk.h winnls.h winnt.h winperf.h winreg.h winsock.h \
	 winspool.h winsvc.h winuser.h winver.h wtypes.h

mglobal.obj: 3d.h aistruct.h bm.h digi.h fix.h game.h gameseg.h gr.h \
	 inferno.h object.h piggy.h pstypes.h segment.h setjmp.h settings.h \
	 sounds.h vecmat.h

mission.obj: aistruct.h cfile.h ctype.h digi.h direct.h error.h findfile.h \
	 fix.h game.h gameseg.h gameseq.h gr.h inferno.h mission.h mono.h \
	 object.h piggy.h player.h pstypes.h segment.h setjmp.h settings.h \
	 songs.h sounds.h stdio.h stdlib.h string.h titles.h types.h \
	 vecmat.h weapon.h

modem.obj: _debug.h _defs286.h _defs386.h aistruct.h args.h cderr.h cguid.h \
	 cntrlcen.h collide.h commdlg.h commlib.h compiler.h conio.h \
	 ctype.h dde.h ddeml.h digi.h dlgs.h dos.h dpmi.h endlevel.h \
	 error.h excpt.h fast.h fix.h game.h gamesave.h gameseg.h gameseq.h \
	 gauges.h glfmodem.h gr.h i86.h imm.h inferno.h io.h laser.h \
	 lzexpand.h malloc.h mcx.h menu.h mission.h mmsystem.h modem.h \
	 mono.h multi.h multibot.h nb30.h netmisc.h network.h newmenu.h \
	 oaidl.h objbase.h object.h objidl.h ole.h ole2.h oleauto.h \
	 oleidl.h pa_enabl.h palette.h pcx.h piggy.h player.h poly_acc.h \
	 poppack.h prsht.h pshpack1.h pshpack2.h pshpack4.h pshpack8.h \
	 pstypes.h rpc.h rpcdce.h rpcdcep.h rpcndr.h rpcnsi.h rpcnsip.h \
	 rpcnterr.h scores.h segment.h setjmp.h settings.h shellapi.h \
	 sounds.h stat.h stdarg.h stddef.h stdio.h stdlib.h string.h text.h \
	 time.h timer.h types.h unknwn.h vecmat.h wall.h weapon.h winbase.h \
	 wincon.h windef.h windows.h winerror.h wingdi.h winnetwk.h \
	 winnls.h winnt.h winperf.h winreg.h winsock.h winspool.h winsvc.h \
	 winuser.h winver.h wtypes.h

morph.obj: 3d.h aistruct.h bm.h digi.h error.h fix.h game.h gameseg.h gr.h \
	 inferno.h lighting.h mono.h morph.h newdemo.h object.h piggy.h \
	 polyobj.h pstypes.h robot.h segment.h setjmp.h settings.h sounds.h \
	 stdio.h stdlib.h string.h texmap.h vecmat.h

movie.obj: aistruct.h args.h byteswap.h cderr.h cfile.h cguid.h commdlg.h \
	 config.h conio.h ctype.h dde.h ddeml.h digi.h dlgs.h dos.h error.h \
	 excpt.h fcntl.h fix.h game.h gamefont.h gamepal.h gameseg.h gr.h \
	 i86.h imm.h inferno.h io.h joy.h key.h lzexpand.h malloc.h mcx.h \
	 mem.h menu.h mmsystem.h mono.h movie.h mvegfx.h mvelib32.h nb30.h \
	 newmenu.h oaidl.h objbase.h object.h objidl.h ole.h ole2.h \
	 oleauto.h oleidl.h pa_enabl.h palette.h piggy.h player.h \
	 poly_acc.h poppack.h prsht.h pshpack1.h pshpack2.h pshpack4.h \
	 pshpack8.h pstypes.h rpc.h rpcdce.h rpcdcep.h rpcndr.h rpcnsi.h \
	 rpcnsip.h rpcnterr.h screens.h segment.h setjmp.h settings.h \
	 shellapi.h songs.h sos.h sosfnct.h sounds.h stat.h stdarg.h \
	 stdio.h stdlib.h string.h text.h timer.h types.h unknwn.h vecmat.h \
	 vga.h weapon.h winbase.h wincon.h windef.h windows.h windowsx.h \
	 winerror.h wingdi.h winnetwk.h winnls.h winnt.h winperf.h winreg.h \
	 winsock.h winspool.h winsvc.h winuser.h winver.h wtypes.h

multi.obj: 3d.h _debug.h _defs286.h _defs386.h ai.h aistruct.h args.h bm.h \
	 byteswap.h cderr.h cfile.h cguid.h cntrlcen.h collide.h commdlg.h \
	 commlib.h compiler.h config.h conio.h ctype.h dde.h ddeml.h digi.h \
	 dlgs.h dos.h effects.h endlevel.h error.h excpt.h fireball.h fix.h \
	 fuelcen.h fvi.h game.h gameseg.h gameseq.h gamestat.h gauges.h \
	 glfmodem.h gr.h i86.h imm.h inferno.h kconfig.h key.h kmatrix.h \
	 laser.h lzexpand.h malloc.h mcx.h mem.h mission.h mmsystem.h \
	 modem.h mono.h multi.h multibot.h nb30.h network.h newdemo.h \
	 newmenu.h oaidl.h objbase.h object.h objidl.h ole.h ole2.h \
	 oleauto.h oleidl.h physics.h piggy.h player.h playsave.h polyobj.h \
	 poppack.h powerup.h prsht.h pshpack1.h pshpack2.h pshpack4.h \
	 pshpack8.h pstypes.h robot.h rpc.h rpcdce.h rpcdcep.h rpcndr.h \
	 rpcnsi.h rpcnsip.h rpcnterr.h scores.h segment.h setjmp.h \
	 settings.h shellapi.h sounds.h state.h stdarg.h stddef.h stdio.h \
	 stdlib.h string.h switch.h text.h textures.h time.h timer.h \
	 unknwn.h vclip.h vecmat.h wall.h weapon.h winbase.h wincon.h \
	 windef.h windows.h winerror.h wingdi.h winnetwk.h winnls.h winnt.h \
	 winperf.h winreg.h winsock.h winspool.h winsvc.h winuser.h \
	 winver.h wtypes.h

multibot.obj: ai.h aistruct.h byteswap.h digi.h effects.h error.h \
	 fireball.h fix.h fuelcen.h fvi.h game.h gameseg.h gameseq.h \
	 gauges.h gr.h inferno.h laser.h mission.h modem.h mono.h morph.h \
	 multi.h multibot.h network.h object.h physics.h piggy.h player.h \
	 powerup.h pstypes.h robot.h scores.h segment.h setjmp.h settings.h \
	 sounds.h stdio.h stdlib.h string.h text.h timer.h vclip.h vecmat.h \
	 weapon.h

netmisc.obj: aistruct.h byteswap.h digi.h error.h fix.h game.h gameseg.h \
	 gameseq.h gr.h inferno.h ipx.h mission.h mono.h multi.h network.h \
	 object.h piggy.h player.h powerup.h pstypes.h segment.h setjmp.h \
	 settings.h sounds.h stdio.h string.h vclip.h vecmat.h weapon.h

network.obj: aistruct.h args.h automap.h bm.h byteswap.h cntrlcen.h \
	 config.h digi.h effects.h endlevel.h error.h fireball.h fix.h \
	 fvi.h game.h gamemine.h gamesave.h gameseg.h gameseq.h gamestat.h \
	 gauges.h gr.h inferno.h ipx.h kconfig.h key.h kmatrix.h laser.h \
	 menu.h mission.h mono.h multi.h multibot.h netmisc.h network.h \
	 newdemo.h newmenu.h object.h palette.h physics.h piggy.h player.h \
	 playsave.h powerup.h pstypes.h segment.h setjmp.h settings.h \
	 sounds.h stdio.h stdlib.h string.h switch.h text.h timer.h vclip.h \
	 vecmat.h wall.h weapon.h

newdemo.obj: 3d.h ai.h aistruct.h args.h bm.h cntrlcen.h collide.h \
	 controls.h ctype.h digi.h direct.h dos.h editor.h errno.h error.h \
	 findfile.h fireball.h fix.h fuelcen.h fvi.h game.h gamemine.h \
	 gamesave.h gameseg.h gameseq.h gauges.h gr.h hostage.h i86.h \
	 inferno.h io.h key.h laser.h lighting.h limits.h malloc.h \
	 mission.h mono.h morph.h multi.h network.h newdemo.h newmenu.h \
	 object.h palette.h physics.h piggy.h player.h polyobj.h powerup.h \
	 pstypes.h render.h robot.h segment.h setjmp.h settings.h slew.h \
	 sounds.h stat.h stdarg.h stdio.h stdlib.h string.h switch.h \
	 texmap.h text.h types.h vclip.h vecmat.h wall.h weapon.h

newmenu.obj: aistruct.h cderr.h cguid.h commdlg.h config.h ctype.h dd.h \
	 dde.h ddeml.h ddraw.h desw.h digi.h dlgs.h endlevel.h error.h \
	 excpt.h findfile.h fix.h game.h gamefont.h gamepal.h gameseg.h \
	 gameseq.h gamestat.h gr.h iff.h imm.h inferno.h io.h joy.h \
	 kconfig.h key.h lzexpand.h math.h mcx.h mem.h menu.h mission.h \
	 mmsystem.h mono.h mouse.h multi.h nb30.h network.h newdemo.h \
	 newmenu.h oaidl.h objbase.h object.h objidl.h ole.h ole2.h \
	 oleauto.h oleidl.h pa_enabl.h palette.h pcx.h piggy.h player.h \
	 poly_acc.h poppack.h prsht.h pshpack1.h pshpack2.h pshpack4.h \
	 pshpack8.h pstypes.h rpc.h rpcdce.h rpcdcep.h rpcndr.h rpcnsi.h \
	 rpcnsip.h rpcnterr.h screens.h segment.h setjmp.h settings.h \
	 shellapi.h songs.h sounds.h stat.h stdarg.h stdio.h stdlib.h \
	 string.h tactile.h text.h timer.h types.h unistd.h unknwn.h \
	 vecmat.h weapon.h winapp.h winbase.h wincon.h windef.h windows.h \
	 winerror.h wingdi.h winnetwk.h winnls.h winnt.h winperf.h winreg.h \
	 winsock.h winspool.h winsvc.h winuser.h winver.h wtypes.h

newmodel.obj: 3d.h aistruct.h digi.h error.h fix.h game.h gameseg.h gr.h \
	 inferno.h io.h object.h piggy.h polyobj.h pstypes.h robot.h \
	 segment.h setjmp.h settings.h sounds.h stat.h stdio.h stdlib.h \
	 string.h types.h vecmat.h

object.obj: 3d.h 3dfx_des.h ai.h aistruct.h arcade.h args.h bm.h cderr.h \
	 cguid.h cntrlcen.h collide.h commdlg.h ctype.h dd.h dde.h ddeml.h \
	 ddraw.h desw.h digi.h dlgs.h editor.h endlevel.h error.h excpt.h \
	 fireball.h fix.h fuelcen.h fvi.h game.h gameseg.h gameseq.h \
	 gauges.h glide.h glidesys.h glideutl.h gr.h hostage.h imm.h \
	 inferno.h key.h laser.h lighting.h lzexpand.h mcx.h menu.h \
	 mission.h mmsystem.h mono.h morph.h multi.h nb30.h network.h \
	 newdemo.h newmenu.h oaidl.h objbase.h object.h objidl.h ole.h \
	 ole2.h oleauto.h oleidl.h pa_enabl.h physics.h piggy.h player.h \
	 polyobj.h poppack.h powerup.h prsht.h pshpack1.h pshpack2.h \
	 pshpack4.h pshpack8.h pstypes.h render.h robot.h rpc.h rpcdce.h \
	 rpcdcep.h rpcndr.h rpcnsi.h rpcnsip.h rpcnterr.h segment.h \
	 setjmp.h settings.h shellapi.h slew.h sounds.h stdarg.h stdio.h \
	 stdlib.h string.h switch.h tactile.h texmap.h text.h textures.h \
	 unknwn.h vclip.h vecmat.h wall.h weapon.h winapp.h winbase.h \
	 wincon.h windef.h windows.h winerror.h wingdi.h winnetwk.h \
	 winnls.h winnt.h winperf.h winreg.h winsock.h winspool.h winsvc.h \
	 winuser.h winver.h wtypes.h

paging.obj: 3d.h aistruct.h bm.h cderr.h cguid.h cntrlcen.h commdlg.h \
	 ctype.h dd.h dde.h ddeml.h ddraw.h desw.h digi.h dlgs.h effects.h \
	 error.h excpt.h fireball.h fix.h fuelcen.h game.h gamemine.h \
	 gameseg.h gauges.h gr.h imm.h inferno.h lzexpand.h math.h mcx.h \
	 mission.h mmsystem.h mono.h nb30.h oaidl.h objbase.h object.h \
	 objidl.h ole.h ole2.h oleauto.h oleidl.h palette.h piggy.h \
	 polyobj.h poppack.h powerup.h prsht.h pshpack1.h pshpack2.h \
	 pshpack4.h pshpack8.h pstypes.h robot.h rpc.h rpcdce.h rpcdcep.h \
	 rpcndr.h rpcnsi.h rpcnsip.h rpcnterr.h segment.h setjmp.h \
	 settings.h shellapi.h sounds.h stdarg.h stdio.h stdlib.h string.h \
	 texmerge.h text.h textures.h timer.h unknwn.h vclip.h vecmat.h \
	 wall.h weapon.h winapp.h winbase.h wincon.h windef.h windows.h \
	 winerror.h wingdi.h winnetwk.h winnls.h winnt.h winperf.h winreg.h \
	 winsock.h winspool.h winsvc.h winuser.h winver.h wtypes.h

physics.obj: ai.h aistruct.h bm.h collide.h digi.h error.h fix.h fvi.h \
	 game.h gameseg.h gr.h inferno.h joy.h key.h laser.h mono.h \
	 newdemo.h object.h physics.h piggy.h player.h pstypes.h robot.h \
	 segment.h setjmp.h settings.h sounds.h stdio.h stdlib.h tactile.h \
	 timer.h vecmat.h wall.h weapon.h

piggy.obj: aistruct.h args.h bm.h bmread.h byteswap.h cderr.h cfile.h \
	 cguid.h commdlg.h ctype.h dd.h dde.h ddeml.h ddraw.h desw.h digi.h \
	 dlgs.h dos.h dpmi.h ds.h dsound.h errno.h error.h excpt.h files.h \
	 findfile.h fix.h game.h gamefont.h gameseg.h gr.h hash.h i86.h \
	 iff.h imm.h inferno.h io.h lzexpand.h mcx.h mem.h mmsystem.h \
	 mono.h nb30.h newmenu.h oaidl.h objbase.h object.h objidl.h ole.h \
	 ole2.h oleauto.h oleidl.h paging.h palette.h piggy.h poppack.h \
	 prsht.h pshpack1.h pshpack2.h pshpack4.h pshpack8.h pstypes.h \
	 rle.h rpc.h rpcdce.h rpcdcep.h rpcndr.h rpcnsi.h rpcnsip.h \
	 rpcnterr.h screens.h segment.h setjmp.h settings.h shellapi.h \
	 songs.h sounds.h stat.h stdarg.h stdio.h stdlib.h string.h \
	 texmerge.h text.h types.h unarj.h unistd.h unknwn.h vecmat.h \
	 winapp.h winbase.h wincon.h windef.h windows.h winerror.h wingdi.h \
	 winnetwk.h winnls.h winnt.h winperf.h winreg.h winsock.h \
	 winspool.h winsvc.h winuser.h winver.h wtypes.h

playsave.obj: aistruct.h cderr.h cguid.h commdlg.h config.h ctype.h dd.h \
	 dde.h ddeml.h ddraw.h desw.h digi.h dlgs.h errno.h error.h excpt.h \
	 files.h fix.h game.h gameseg.h gameseq.h gamestat.h gauges.h gr.h \
	 imm.h inferno.h io.h joy.h joydefs.h kconfig.h lzexpand.h mcx.h \
	 menu.h mission.h mmsystem.h mono.h multi.h nb30.h newmenu.h \
	 oaidl.h objbase.h object.h objidl.h ole.h ole2.h oleauto.h \
	 oleidl.h pa_enabl.h palette.h piggy.h player.h playsave.h \
	 poly_acc.h poppack.h powerup.h prsht.h pshpack1.h pshpack2.h \
	 pshpack4.h pshpack8.h pstypes.h rpc.h rpcdce.h rpcdcep.h rpcndr.h \
	 rpcnsi.h rpcnsip.h rpcnterr.h screens.h segment.h setjmp.h \
	 settings.h shellapi.h sounds.h stat.h state.h stdarg.h stdio.h \
	 stdlib.h string.h text.h types.h unistd.h unknwn.h vclip.h \
	 vecmat.h weapon.h winapp.h winbase.h wincon.h windef.h windows.h \
	 winerror.h wingdi.h winnetwk.h winnls.h winnt.h winperf.h winreg.h \
	 winsock.h winspool.h winsvc.h winuser.h winver.h wtypes.h

polyobj.obj: 3d.h 3dfx_des.h aistruct.h args.h bm.h cfile.h digi.h error.h \
	 fix.h game.h gameseg.h glide.h glidesys.h glideutl.h gr.h \
	 inferno.h lighting.h mem.h mono.h object.h pa_enabl.h piggy.h \
	 polyobj.h pstypes.h robot.h segment.h setjmp.h settings.h sounds.h \
	 stdio.h stdlib.h string.h texmap.h textures.h vecmat.h

powerup.obj: 3d.h ai.h aistruct.h config.h controls.h digi.h editor.h \
	 error.h fireball.h fix.h fvi.h game.h gameseg.h gameseq.h \
	 gamestat.h gauges.h gr.h inferno.h kconfig.h key.h laser.h \
	 lighting.h mission.h mono.h multi.h newdemo.h object.h piggy.h \
	 player.h powerup.h pstypes.h robot.h scores.h segment.h setjmp.h \
	 settings.h sounds.h stdarg.h stdio.h string.h text.h vclip.h \
	 vecmat.h wall.h weapon.h

rbtest.obj: dpmi.h error.h fix.h key.h pstypes.h rbaudio.h stdio.h timer.h

render.obj: 3d.h aistruct.h automap.h bm.h cntrlcen.h digi.h editor.h \
	 endlevel.h error.h fix.h fvi.h game.h gameseg.h gr.h inferno.h \
	 key.h laser.h lighting.h mem.h mono.h newdemo.h newmenu.h object.h \
	 pa_enabl.h physics.h piggy.h player.h poly_acc.h pstypes.h \
	 render.h screens.h segment.h segpoint.h setjmp.h settings.h \
	 sounds.h stdio.h stdlib.h string.h texmap.h texmerge.h textures.h \
	 vclip.h vecmat.h wall.h weapon.h

robot.obj: 3d.h aistruct.h digi.h error.h fix.h game.h gameseg.h gr.h \
	 inferno.h mono.h object.h piggy.h polyobj.h pstypes.h robot.h \
	 segment.h setjmp.h settings.h sounds.h vecmat.h

scores.obj: aistruct.h cderr.h cguid.h commdlg.h ctype.h dd.h dde.h ddeml.h \
	 ddraw.h desw.h digi.h dlgs.h error.h excpt.h fix.h game.h \
	 gamefont.h gameseg.h gr.h imm.h inferno.h io.h joy.h key.h \
	 lzexpand.h mcx.h mem.h menu.h mmsystem.h mono.h mouse.h nb30.h \
	 newmenu.h oaidl.h objbase.h object.h objidl.h ole.h ole2.h \
	 oleauto.h oleidl.h palette.h piggy.h player.h poppack.h prsht.h \
	 pshpack1.h pshpack2.h pshpack4.h pshpack8.h pstypes.h rpc.h \
	 rpcdce.h rpcdcep.h rpcndr.h rpcnsi.h rpcnsip.h rpcnterr.h \
	 screens.h segment.h setjmp.h settings.h shellapi.h songs.h \
	 sounds.h stat.h stdarg.h stdio.h stdlib.h string.h text.h timer.h \
	 types.h unknwn.h vecmat.h weapon.h winapp.h winbase.h wincon.h \
	 windef.h windows.h winerror.h wingdi.h winnetwk.h winnls.h winnt.h \
	 winperf.h winreg.h winsock.h winspool.h winsvc.h winuser.h \
	 winver.h wtypes.h

slew.obj: aistruct.h args.h config.h digi.h error.h fix.h fvi.h game.h \
	 gameseg.h gamestat.h gr.h inferno.h joy.h joydefs.h kconfig.h \
	 key.h object.h physics.h piggy.h player.h pstypes.h segment.h \
	 setjmp.h settings.h sounds.h stdlib.h vecmat.h weapon.h

songs.obj: aistruct.h args.h cfile.h config.h ctype.h digi.h direct.h \
	 error.h fix.h game.h gameseg.h gamestat.h gr.h inferno.h kconfig.h \
	 mono.h object.h piggy.h player.h pstypes.h rbaudio.h segment.h \
	 setjmp.h settings.h songs.h sounds.h stdio.h stdlib.h string.h \
	 timer.h types.h vecmat.h weapon.h

state.obj: 3d.h ai.h aistruct.h args.h automap.h bm.h cderr.h cfile.h \
	 cguid.h cntrlcen.h commdlg.h controls.h ctype.h dd.h dde.h ddeml.h \
	 ddraw.h desw.h digi.h dlgs.h errno.h error.h excpt.h files.h \
	 fireball.h fix.h fuelcen.h fvi.h game.h gamemine.h gameseg.h \
	 gameseq.h gauges.h gr.h hash.h imm.h inferno.h io.h key.h laser.h \
	 lzexpand.h math.h mcx.h mem.h menu.h mission.h mmsystem.h mono.h \
	 morph.h multi.h multibot.h nb30.h network.h newdemo.h newmenu.h \
	 oaidl.h objbase.h object.h objidl.h ole.h ole2.h oleauto.h \
	 oleidl.h pa_enabl.h paging.h pcx.h piggy.h player.h poly_acc.h \
	 poppack.h prsht.h pshpack1.h pshpack2.h pshpack4.h pshpack8.h \
	 pstypes.h render.h robot.h rpc.h rpcdce.h rpcdcep.h rpcndr.h \
	 rpcnsi.h rpcnsip.h rpcnterr.h screens.h segment.h setjmp.h \
	 settings.h shellapi.h sounds.h stat.h stdarg.h stdio.h stdlib.h \
	 string.h switch.h text.h textures.h titles.h types.h unistd.h \
	 unknwn.h vecmat.h wall.h weapon.h winapp.h winbase.h wincon.h \
	 windef.h windows.h winerror.h wingdi.h winnetwk.h winnls.h winnt.h \
	 winperf.h winreg.h winsock.h winspool.h winsvc.h winuser.h \
	 winver.h wtypes.h

switch.obj: 3d.h aistruct.h bm.h cntrlcen.h digi.h editor.h endlevel.h \
	 error.h fix.h fuelcen.h game.h gameseg.h gameseq.h gauges.h gr.h \
	 inferno.h math.h mission.h mono.h multi.h network.h newdemo.h \
	 newmenu.h object.h palette.h piggy.h player.h pstypes.h robot.h \
	 segment.h setjmp.h settings.h sounds.h stdio.h stdlib.h string.h \
	 switch.h texmap.h vecmat.h wall.h weapon.h

terrain.obj: 3d.h aistruct.h bm.h digi.h endlevel.h error.h fireball.h \
	 fix.h gameseg.h gr.h iff.h inferno.h mem.h mono.h object.h piggy.h \
	 pstypes.h segment.h settings.h sounds.h stdio.h stdlib.h string.h \
	 texmap.h textures.h vecmat.h

texmerge.obj: aistruct.h bm.h digi.h error.h fix.h game.h gameseg.h gr.h \
	 inferno.h mono.h object.h pa_enabl.h piggy.h poly_acc.h pstypes.h \
	 rle.h segment.h setjmp.h settings.h sounds.h stdlib.h textures.h \
	 vecmat.h

text.obj: args.h cfile.h compbit.h error.h inferno.h mem.h pstypes.h \
	 settings.h stdio.h stdlib.h string.h text.h

titles.obj: 3d.h aistruct.h arcade.h bm.h cderr.h cfile.h cguid.h coindev.h \
	 commdlg.h compbit.h ctype.h dd.h dde.h ddeml.h ddraw.h desw.h \
	 digi.h dlgs.h error.h excpt.h fix.h game.h gamefont.h gameseg.h \
	 gameseq.h gr.h iff.h imm.h inferno.h joy.h key.h kmatrix.h \
	 lzexpand.h mcx.h mem.h menu.h mission.h mmsystem.h mono.h movie.h \
	 multi.h nb30.h newmenu.h oaidl.h objbase.h object.h objidl.h ole.h \
	 ole2.h oleauto.h oleidl.h pa_enabl.h palette.h pcx.h piggy.h \
	 player.h poly_acc.h polyobj.h poppack.h prsht.h pshpack1.h \
	 pshpack2.h pshpack4.h pshpack8.h pstypes.h robot.h rpc.h rpcdce.h \
	 rpcdcep.h rpcndr.h rpcnsi.h rpcnsip.h rpcnterr.h screens.h \
	 segment.h setjmp.h settings.h shellapi.h songs.h sounds.h state.h \
	 stdarg.h stdio.h stdlib.h string.h text.h textures.h timer.h \
	 unknwn.h vecmat.h weapon.h winapp.h winbase.h wincon.h windef.h \
	 windows.h winerror.h wingdi.h winnetwk.h winnls.h winnt.h \
	 winperf.h winreg.h winsock.h winspool.h winsvc.h winuser.h \
	 winver.h wtypes.h

vclip.obj: aistruct.h digi.h error.h fix.h game.h gameseg.h gr.h inferno.h \
	 laser.h object.h piggy.h pstypes.h segment.h setjmp.h settings.h \
	 sounds.h stdlib.h vclip.h vecmat.h weapon.h

vfx.obj: conio.h error.h fix.h gr.h i86.h inferno.h mono.h pstypes.h \
	 settings.h stdio.h stdlib.h string.h text.h vfx1.h

vfxread.obj: conio.h dos.h error.h fix.h gr.h i86.h inferno.h mono.h \
	 pstypes.h settings.h stddef.h stdio.h stdlib.h string.h vfx1.h

victor.obj: _debug.h _defs286.h _defs386.h args.h cderr.h cguid.h commdlg.h \
	 commlib.h compiler.h conio.h ctype.h dde.h ddeml.h dlgs.h dos.h \
	 error.h excpt.h fast.h fix.h i86.h imm.h inferno.h key.h \
	 lzexpand.h malloc.h mcx.h mmsystem.h mono.h nb30.h oaidl.h \
	 objbase.h objidl.h ole.h ole2.h oleauto.h oleidl.h poppack.h \
	 prsht.h pshpack1.h pshpack2.h pshpack4.h pshpack8.h pstypes.h \
	 rpc.h rpcdce.h rpcdcep.h rpcndr.h rpcnsi.h rpcnsip.h rpcnterr.h \
	 settings.h shellapi.h stdarg.h stddef.h stdio.h stdlib.h string.h \
	 text.h unknwn.h victor.h winbase.h wincon.h windef.h windows.h \
	 winerror.h wingdi.h winnetwk.h winnls.h winnt.h winperf.h winreg.h \
	 winsock.h winspool.h winsvc.h winuser.h winver.h wtypes.h

wall.obj: aistruct.h bm.h collide.h digi.h editor.h effects.h error.h \
	 fireball.h fix.h game.h gameseg.h gameseq.h gauges.h gr.h \
	 inferno.h laser.h math.h mission.h mono.h multi.h newdemo.h \
	 object.h piggy.h player.h pstypes.h segment.h setjmp.h settings.h \
	 sounds.h stdio.h stdlib.h string.h switch.h text.h textures.h \
	 vclip.h vecmat.h wall.h weapon.h

weapon.obj: ai.h aistruct.h args.h digi.h error.h fireball.h fix.h fvi.h \
	 game.h gameseg.h gameseq.h gauges.h gr.h inferno.h laser.h \
	 mission.h mono.h multi.h newdemo.h newmenu.h object.h piggy.h \
	 player.h powerup.h pstypes.h robot.h segment.h setjmp.h settings.h \
	 sounds.h stdio.h stdlib.h string.h tactile.h text.h vclip.h \
	 vecmat.h weapon.h

windigi.obj: aistruct.h args.h cderr.h cfile.h cguid.h commdlg.h config.h \
	 ctype.h dd.h dde.h ddeml.h ddraw.h desw.h digi.h dlgs.h ds.h \
	 dsound.h error.h excpt.h fix.h game.h gameseg.h gr.h imm.h \
	 inferno.h joy.h key.h lzexpand.h mcx.h mmreg.h mmsystem.h mono.h \
	 nb30.h newdemo.h oaidl.h objbase.h object.h objidl.h ole.h ole2.h \
	 oleauto.h oleidl.h piggy.h player.h poppack.h prsht.h pshpack1.h \
	 pshpack2.h pshpack4.h pshpack8.h pstypes.h rpc.h rpcdce.h \
	 rpcdcep.h rpcndr.h rpcnsi.h rpcnsip.h rpcnterr.h segment.h \
	 setjmp.h settings.h shellapi.h sounds.h stdarg.h stdio.h stdlib.h \
	 string.h text.h timer.h unknwn.h vecmat.h wall.h weapon.h winapp.h \
	 winbase.h wincon.h windef.h windows.h winerror.h wingdi.h \
	 winmidi.h winnetwk.h winnls.h winnt.h winperf.h winreg.h winsock.h \
	 winspool.h winsvc.h winuser.h winver.h wtypes.h

winferno.obj: 3d.h 3dfx_des.h aistruct.h args.h bm.h cderr.h cfile.h \
	 cguid.h commdlg.h config.h ctype.h dd.h dde.h ddeml.h ddraw.h \
	 desc_id.h desw.h digi.h dlgs.h error.h excpt.h fix.h game.h \
	 gamefont.h gamepal.h gameseg.h gameseq.h gamestat.h glide.h \
	 glidesys.h glideutl.h gr.h imm.h inferno.h ipx.h joy.h kconfig.h \
	 key.h lzexpand.h mcx.h mem.h menu.h mission.h mmsystem.h modem.h \
	 mono.h movie.h multi.h nb30.h network.h newdemo.h newmenu.h \
	 oaidl.h objbase.h object.h objidl.h ole.h ole2.h oleauto.h \
	 oleidl.h palette.h pcx.h piggy.h player.h polyobj.h poppack.h \
	 prsht.h pshpack1.h pshpack2.h pshpack4.h pshpack8.h pstypes.h \
	 robot.h rpc.h rpcdce.h rpcdcep.h rpcndr.h rpcnsi.h rpcnsip.h \
	 rpcnterr.h screens.h segment.h setjmp.h settings.h shellapi.h \
	 songs.h sounds.h stdarg.h stdio.h stdlib.h string.h texmap.h \
	 texmerge.h text.h time.h timer.h unknwn.h vecmat.h vers_id.h \
	 weapon.h winapp.h winbase.h wincon.h windef.h windows.h winerror.h \
	 wingdi.h winnetwk.h winnls.h winnt.h winperf.h winreg.h winsock.h \
	 winspool.h winsvc.h winuser.h winver.h wtypes.h

winmodem.obj: aistruct.h args.h cderr.h cguid.h cntrlcen.h collide.h comm.h \
	 commdlg.h ctype.h dd.h dde.h ddeml.h ddraw.h desw.h digi.h dlgs.h \
	 dos.h endlevel.h error.h excpt.h fix.h game.h gamesave.h gameseg.h \
	 gameseq.h gauges.h gr.h i86.h imm.h inferno.h io.h laser.h \
	 lzexpand.h mcx.h menu.h mission.h mmsystem.h modem.h mono.h \
	 multi.h multibot.h nb30.h netmisc.h network.h newmenu.h oaidl.h \
	 objbase.h object.h objidl.h ole.h ole2.h oleauto.h oleidl.h \
	 palette.h pcx.h piggy.h player.h poppack.h prsht.h pshpack1.h \
	 pshpack2.h pshpack4.h pshpack8.h pstypes.h rpc.h rpcdce.h \
	 rpcdcep.h rpcndr.h rpcnsi.h rpcnsip.h rpcnterr.h scores.h \
	 segment.h setjmp.h settings.h shellapi.h sounds.h stat.h stdarg.h \
	 stdio.h stdlib.h string.h text.h time.h timer.h types.h unknwn.h \
	 vecmat.h wall.h weapon.h winapp.h winbase.h wincon.h windef.h \
	 windows.h winerror.h wingdi.h winnetwk.h winnls.h winnt.h \
	 winperf.h winreg.h winsock.h winspool.h winsvc.h winuser.h \
	 winver.h wtypes.h xtapi.h

winmovie.obj: byteswap.h cderr.h cfile.h cguid.h commdlg.h ctype.h dd.h \
	 dde.h ddeml.h ddraw.h desw.h digi.h dlgs.h dsound.h error.h \
	 excpt.h fcntl.h fix.h gamefont.h gr.h imm.h inferno.h io.h key.h \
	 lzexpand.h mcx.h mem.h mmsystem.h mono.h movie.h mvegfx.h \
	 mvelibw.h nb30.h newmenu.h oaidl.h objbase.h objidl.h ole.h ole2.h \
	 oleauto.h oleidl.h palette.h poppack.h prsht.h pshpack1.h \
	 pshpack2.h pshpack4.h pshpack8.h pstypes.h rpc.h rpcdce.h \
	 rpcdcep.h rpcndr.h rpcnsi.h rpcnsip.h rpcnterr.h screens.h \
	 settings.h shellapi.h stat.h stdarg.h stdio.h stdlib.h string.h \
	 text.h types.h unknwn.h vecmat.h winapp.h winbase.h wincon.h \
	 windef.h windows.h windowsx.h winerror.h wingdi.h winnetwk.h \
	 winnls.h winnt.h winperf.h winreg.h winsock.h winspool.h winsvc.h \
	 winuser.h winver.h wtypes.h

