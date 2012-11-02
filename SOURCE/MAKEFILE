#
# $Source: f:/miner/source/rcs/makefile $
# $Revision: 1.9 $
# $Author: matt $
# $Date: 1996/12/05 19:03:06 $
#
# Makefile for Descent libraries
#

# Subdirs to make for the DOS libraries
SUBSYSTEMS_DOS = includes misc mem fix cfile pa_null 2d vga bios iff div \
	ui vecmat 3d texmap tactile unarj sosdigi sosmidi

# Subdirs to make for the Win95 libraries
SUBSYSTEMS_WIN = includes misc mem fix cfile pa_null 2d vga novga iff div \
	vecmat tactile win95 3d texmap unarj 

# All subdirs.  Needed for clean to work
SUBSYSTEMS = includes misc mem fix cfile pa_null 2d vga bios iff div \
	ui vecmat 3d texmap tactile unarj sosdigi sosmidi win95 novga

# What to make in the subdirs if nothing specified
SUBTARGETS = optimize no_mono no_debug linstall		#for release
##SUBTARGETS = linstall					#for debugging

# When making clean here, delete libs
CLEAN_TARGS = lib\*.lib

real_clean .NODEFAULT: 
	make clean
	del lib\*.h
	del lib\*.inc
	%if %exists(lib\win)
	  del lib\win\*.h
	  del lib\win\*.inc
	  del lib\win\*.lib
	%endif

# If no targs specified, exit with message
default:
	@%error 1 Error: You must specify either DOS or WIN

# Build libs for DOS
dos:
	(cd bios;make linstallh)
	make SUBSYSTEMS=$$(SUBSYSTEMS_DOS) all

# Build libs for Win
win:
	(cd bios;make linstallh)
	(cd win95;make linstallh)
	make SUBSYSTEMS=$$(SUBSYSTEMS_WIN) all


#all Miner makefiles should include this
%include $(INIT)\makefile.def

