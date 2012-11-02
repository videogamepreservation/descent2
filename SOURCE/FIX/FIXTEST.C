//
//	test program for fixed-point
//

#include <stdio.h>
#include <conio.h>
#include <stdlib.h>

#include "types.h"
#include "fix.h"
#include "graph.h"

#define f1_0 0x10000
#define f0_5 0x8000

#define f7_0 0x70000

show_sincos(fixang a)
{
	fix s,c;
	fix fs,fc;

	fix_fastsincos(a,&fs,&fc);
	fix_sincos(a,&s,&c);

	printf("%x: sin = %x(%x), cos = %x(%x)\n",a,s,fs,c,fc);
}

mul_test()
{
	printf("%x * %x = %x\n",f1_0,f1_0,fixmul(f1_0,f1_0));
	printf("%x * %x = %x\n",f1_0,f0_5,fixmul(f1_0,f0_5));
	printf("%x * %x = %x\n",f0_5,f0_5,fixmul(f0_5,f0_5));
	printf("%x * %x = %x\n",f7_0,f1_0,fixmul(f7_0,f1_0));
	printf("%x * %x = %x\n",f7_0,f0_5,fixmul(f7_0,f0_5));
	printf("%x * %x = %x\n",f7_0,f7_0,fixmul(f7_0,f7_0));
}

sincos_test()
{

	show_sincos(f1_0);
	show_sincos(f0_5);
	show_sincos(f0_5/2);
}

#define N_POINTS 1024
#define RAD 75

circle_test()
{
	int i;
	fixang f;
	fix s,c;
	short x,y;

	_setvideomode(_MRES256COLOR);		// 320x200x256 (mode 13h)

//printf("fast:\n");

	for (i=0,f=0;i<N_POINTS;i++,f+=65536/N_POINTS) {

		fix_fastsincos(f,&s,&c);

		x = 160 + fixmul(c,RAD);
		y = 100 + fixmul(s,RAD);
//printf("f=%x, s,c=%x,%x  x,y=%d,%d\n",f,s,c,x,y);

		_setcolor(1);
		_setpixel(x,y);


	}

	getch();

//printf("good:\n");
	for (i=0,f=0;i<N_POINTS;i++,f+=65536/N_POINTS) {

		fix_sincos(f,&s,&c);

		x = 160 + fixmul(c,RAD);
		y = 100 + fixmul(s,RAD);
//printf("f=%x, s,c=%x,%x  x,y=%d,%d\n",f,s,c,x,y);

		_setcolor(4);
		_setpixel(x,y);

	}

	getch();

	_setvideomode(_DEFAULTMODE);
}

show_sqrt(long v)
{
	long r=long_sqrt(v),
		  e=labs(v-r*r),
		 e1=labs(v-(r-1)*(r-1)),
		 e2=labs(v-(r+1)*(r+1));

	printf("sqrt(%d) = %d",v,r);

	if (e1 < e) printf("  ERROR: %d is closer",r-1);
	if (e2 < e) printf("  ERROR: %d is closer",r+1);

	printf("\n");

}

quad_show_sqrt(long low,long high)
{
	long r=quad_sqrt(low,high);

	printf("sqrt(%x%08x) = %x",high,low,r);

	printf("\n");

}

test_sqrt(long v)
{
	long r=long_sqrt(v),
		  e=labs(v-r*r),
		 e1=labs(v-(r-1)*(r-1)),
		 e2=labs(v-(r+1)*(r+1));

	if (e1 < e) {printf("ERROR: sqrt(%d) returned %d, but %d is closer\n",v,r,r-1); exit(100);}
	if (e2 < e) {printf("ERROR: sqrt(%d) returned %d, but %d is closer\n",v,r,r+1); exit(100);}

}


#define N_TESTS 12345678

//long count[10];

sqrt_time()
{
	unsigned int i;
	long t;

	for (i=N_TESTS,t=0;i;i--,t+=0xffffffff/N_TESTS)
		long_sqrt(t);

//	printf("counts:\n");
//	for (i=0;i<10;i++) printf(" %d: %d\n",i,count[i]);
}

quad_sqrt_time()
{
	unsigned int i;
	long t;

	for (i=N_TESTS,t=0;i;i--,t+=0xffffffff/N_TESTS)
		quad_sqrt(0x3a2cc81d,t);

//	printf("counts:\n");
//	for (i=0;i<10;i++) printf(" %d: %d\n",i,count[i]);
}

sqrt_test()
{
	unsigned int i;

//	for (i=0;i<=0x7fffffff;i+=0x20c49b) show_sqrt(i);

	for (i=0;i<=0x7fffffff;i++) {if ((i&0xffff)==0) printf("%d\n",i); test_sqrt(i);}

//	show_sqrt(100);
//	show_sqrt(1000);
//	show_sqrt(65536);
//	show_sqrt(123456);
}

quad_sqrt_test()
{
//	unsigned int i;
//	for (i=0;i<=0x7fffffff;i++) {if ((i&0xffff)==0) printf("%d\n",i); test_sqrt(i);}

	quad_show_sqrt(100,0);
	quad_show_sqrt(1000,0);
	quad_show_sqrt(65536,0);
	quad_show_sqrt(123456,0);

	quad_show_sqrt(0x9abcdef0,0x12345678);
	quad_show_sqrt(0x0,0x4522);


}
acos_test()
{
	int i;
	fixang t,check;
	fix s,c;

		fix_sincos(0x1cd,&s,&c);
		printf("1cd: %x  %x  %x\n",s,c,fix_acos(0xffb8));

	for (i=0,t=0;i<150;i++,t+=f1_0/150) {
		fix_sincos(t,&s,&c);
		check = fix_acos(c);
		printf("%x  %x  %x  %x\n",(unsigned) t,c,check,labs(labs(t)-check));
	}


}
atan2_show(fix x,fix y)
{
	printf("x,y=%d,%d,  a=%x\n",x,y,(unsigned short) fix_atan2(x,y));
}

atan2_test()
{
	atan2_show( 0x100000, 0x100000);
	atan2_show( 0x100000, 0x200000);
	atan2_show(-0x100000, 0x200000);
	atan2_show(-0x100000,-0x200000);
	atan2_show( 0x100000,-0x200000);
	atan2_show( 0x100000, 0xa00000);

}

main()
{

	atan2_test();

//	acos_test();

//	mul_test();			//print out a bunch of values

//	sincos_test();			//print out a bunch of values

//	circle_test();		//draw a circle

//	sqrt_test();

//	quad_sqrt_test();

//	sqrt_time();		//timing test

//	quad_sqrt_time();		//timing test

}

ÿ