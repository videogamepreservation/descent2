
#include <math.h>

#define PI 3.1415926

main()
{
	int i,c;
	double t,s;

	for (i=c=0,t=0.0;i<256+64;i++,t+=PI/128) {
		s=sin(t);
		//if (c==0) printf("	dw	");
		c+=printf("	dw	%d\n",(int) (16384*s+(s<0?-0.5:0.5)),t);
		//if (c++<45) printf(","); else {printf("\n"); c=0;}
	}


}


ÿ