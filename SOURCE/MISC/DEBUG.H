/*
THE COMPUTER CODE CONTAINED HEREIN IS THE SOLE PROPERTY OF PARALLAX
SOFTWARE CORPORATION ("PARALLAX").  PARALLAX, IN DISTRIBUTING THE CODE TO
END-USERS, AND SUBJECT TO ALL OF THE TERMS AND CONDITIONS HEREIN, GRANTS A
ROYALTY-FREE, PERPETUAL LICENSE TO SUCH END-USERS FOR USE BY SUCH END-USERS
IN USING, DISPLAYING,  AND CREATING DERIVATIVE WORKS THEREOF, SO LONG AS
SUCH USE, DISPLAY OR CREATION IS FOR NON-COMMERCIAL, ROYALTY OR REVENUE
FREE PURPOSES.  IN NO EVENT SHALL THE END-USER USE THE COMPUTER CODE
CONTAINED HEREIN FOR REVENUE-BEARING PURPOSES.  THE END-USER UNDERSTANDS
AND AGREES TO THE TERMS HEREIN AND ACCEPTS THE SAME BY USE OF THIS FILE.  
COPYRIGHT 1993-1999 PARALLAX SOFTWARE CORPORATION.  ALL RIGHTS RESERVED.
*/



#ifndef _DEBUG_H
#define _DEBUG_H

//The debug() function takes its args in a second set of parens, so that
//it can pass (or not pass) all the args to whatever function it calls.

#ifndef NDEBUG			//if debugging is allowed

#ifdef __DOS__		//MS-DOS, that is

#ifndef NMONO		//if mono not turned off

#include "mono.h"		//DOS version uses mprintf() for debug

#define debug(args) mprintf(args)

#else		//ifndef NMONO

#define debug(args)

#endif	//ifndef NMONO

#else		//ifdef __DOS__  - must be Windows

#define debug(args)		//do some windows thing here

#endif	//ifdef __DOS__

#endif	//ifndef NDEBUG

#endif	//ifndef _DEBUG_H 
