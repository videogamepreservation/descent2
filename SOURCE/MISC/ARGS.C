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


#pragma off (unreferenced)
static char rcsid[] = "$Id: args.c 2.4 1996/05/24 14:08:43 jed Exp $";
#pragma on (unreferenced)

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef MACINTOSH
#include "strutil.h"
#endif

int Num_args=0;
char * Args[100];

int FindArg( char * s )	{
	int i;

	for (i=0; i<Num_args; i++ )
		if (! stricmp( Args[i], s))
			return i;

	return 0;
}

void InitArgs( int argc,char **argv )	{
	int i;
//NO_DESCENT_INI	FILE * fp;
//NO_DESCENT_INI	char line[100];

	Num_args=0;

	for (i=0; i<argc; i++ )	{
		Args[Num_args++] = strdup( argv[i] );
	}

//NO_DESCENT_INI	fp = fopen( "descent.ini", "rt" );		
//NO_DESCENT_INI	if (fp!=NULL) {
//NO_DESCENT_INI		while( fgets( line, 100, fp ) != NULL )	{
//NO_DESCENT_INI			char *p;
//NO_DESCENT_INI			p = strchr(line,';');
//NO_DESCENT_INI			if (p) *p = '\0';		// Remove comments
//NO_DESCENT_INI			p = strchr(line,10);
//NO_DESCENT_INI			if (p) *p = '\0';		// Remove EOL
//NO_DESCENT_INI			p = strchr(line,13);
//NO_DESCENT_INI			if (p) *p = '\0';		// Remove CR
//NO_DESCENT_INI			p = strtok( line, " \t" );
//NO_DESCENT_INI			while( p )	{
//NO_DESCENT_INI				Args[Num_args++] = strdup( p );
//NO_DESCENT_INI				p = strtok( NULL, " \t" );
//NO_DESCENT_INI			}
//NO_DESCENT_INI		}
//NO_DESCENT_INI		fclose(fp);
//NO_DESCENT_INI	}

	for (i=0; i< Num_args; i++ )	{
		if ( Args[i][0] == '/' )  
			Args[i][0] = '-';

		if ( Args[i][0] == '-' )
			strlwr( Args[i]  );		// Convert all args to lowercase

		//printf( "Args[%d] = '%s'\n", i, Args[i] );
	}

}
