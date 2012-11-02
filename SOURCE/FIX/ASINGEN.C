
#include <math.h>
#include <stdio.h>

#define PI 3.1415926

main()
{
	int i;
	double t,s;

	printf(";asin\n");
	for (i=0,t=0.0;i<=256;i++,t+=1.0/256.0) {
		s=asin(t);
		printf("	dw	%d\n",(int) (32768*(s/PI) + (s<0?-0.5:0.5) ),t);
	}

	printf(";acos\n");
	for (i=0,t=0.0;i<=256;i++,t+=1.0/256.0) {
		s=acos(t);
		printf("	dw	%d\n",(int) (32768*(s/PI) + (s<0?-0.5:0.5) ),t);
	}

	printf(";high resolution acos\n");
	for (i=0,t=1.0 - 16/256.0;i<=64;i++,t+=1.0/(4.0*256.0)) {
		s=acos(t);
		printf("	dw	%d\n",(int) (32768*(s/PI) + (s<0?-0.5:0.5) ),t);
	}


}


ÿ