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

#include <stdio.h>
#include <stdlib.h>

#include "vecmat.h"

vms_vector zero_vec={0,0,0};

vms_vector test_vec[10] = {

		{0x123456,0xff779923,0x2248822},
		{0x44456,0xffff9923,0x55f2},
		{0x2123456,0x2123456,0x2123456},
		{0xff00456,0x779923,0x22},
		{0x6665556,0xfff79923,0xffffffff},
		{0x123456,0xfc009923,0},

};


main()
{
	int i;
	fix d,d2;
	vms_vector t;

	for (i=0;i<6;i++) {

		d = vm_vec_normalized_dir(&t,&test_vec[i],&test_vec[(i+1)%5]);
		printf("\n%8lx %8lx %8lx, mag=%x\n",t.x,t.y,t.z,d);

		d2 = vm_vec_normalized_dir_quick(&t,&test_vec[i],&test_vec[(i+1)%5]);
		printf("%8lx %8lx %8lx, mag=%x (%f)\n",t.x,t.y,t.z,d2,f2fl(fixdiv(d,d2)));

	}

		
}

