
#include <math.h>

#include "mono.h"
#include "fix.h"

dist_2d(fix x,fix y);

#pragma aux dist_2d parm [eax] [ebx] value [eax] modify [ecx edx] = \
	"imul	eax"			\
	"xchg	ebx,eax"		\
	"mov	ecx,edx"		\
	"imul	eax"			\
	"add	eax,ebx"		\
	"adc	edx,ecx"		\
	"call	quad_sqrt";

// Given mouse movement in dx, dy, returns a 3x3 rotation matrix in RotMat.
// Taken from Graphics Gems III, page 51, "The Rolling Ball"

void GetMouseRotation( int idx, int idy, fix * RotMat )
{
	fix dr, cos_theta, sin_theta, denom, cos_theta1;
	fix Radius = i2f(100);
	fix dx,dy;
	fix dxdr,dydr;

	dx = i2f(idx); dy = i2f(idy);

	dr = dist_2d(dx,dy);

	denom = dist_2d(Radius,dr);

	cos_theta = fixdiv(Radius,denom);
	sin_theta = fixdiv(dr,denom);

	cos_theta1 = f1_0 - cos_theta;

	dxdr = fixdiv(dx,dr);
	dydr = fixdiv(dy,dr);

	RotMat[0] = cos_theta + fixmul(fixmul(dydr,dydr),cos_theta1);
	RotMat[1] = - fixmul(fixmul(dxdr,dydr),cos_theta1);
	RotMat[2] = fixmul(dxdr,sin_theta);

	RotMat[3] = RotMat[1];
	RotMat[4] = cos_theta + fixmul(fixmul(dxdr,dxdr),cos_theta1);
	RotMat[5] = fixmul(dydr,sin_theta);

	RotMat[6] = -RotMat[2];
	RotMat[7] = -RotMat[5];
	RotMat[8] = cos_theta;


}

ÿ