
#include <math.h>
#include <stdlib.h>

double real_mag(double a,double b,double c)
{
	return sqrt(a*a + b*b + c*c);
	
}

#define SWAP(_a,_b) do {_t=_a; _a=_b; _b=_t;} while (0)

double approx_mag1(double a,double b,double c)
{
	double _t;

	if (fabs(a) < fabs(b))
		SWAP(a,b);

	if (fabs(b) < fabs(c)) {
		SWAP(b,c);

		if (fabs(a) < fabs(b))
			SWAP(a,b);
	}
	
	return a + b/2 + c/4;
	
}

double approx_mag2(double a,double b,double c)
{
	double _t;

	if (fabs(a) < fabs(b))
		SWAP(a,b);

	if (fabs(b) < fabs(c)) {
		SWAP(b,c);

		if (fabs(a) < fabs(b))
			SWAP(a,b);
	}
	
	return a + b*6/16 + c*3/16;
	
}

#define TEST_RANGE 50000.0

#define DEFAULT_N_TESTS 100000

void main(int argc,char **argv)
{
	double a,b,c;
	double mag,m1,m2;
	double error1,error2,total_error1,total_error2,worst_error1,worst_error2;
	double worst_error1_a,worst_error1_b,worst_error1_c;
	double worst_error2_a,worst_error2_b,worst_error2_c;
	double worst_error1_mag,worst_error1_m1,worst_error1_m2;
	double worst_error2_mag,worst_error2_m1,worst_error2_m2;
	long n_tests,i;

	if (argc>1)
		n_tests = atol(argv[1]);
	else
		n_tests = DEFAULT_N_TESTS;

	total_error1 = total_error2 = worst_error1 = worst_error2 = 0.0;

	for (i=0;i<n_tests;i++) {

		do {

			a = (double) rand() * TEST_RANGE / (double) RAND_MAX;
			b = (double) rand() * TEST_RANGE / (double) RAND_MAX;
			c = (double) rand() * TEST_RANGE / (double) RAND_MAX;

			//printf("%lf %lf %lf \n",a,b,c);

			mag = real_mag(a,b,c);

		} while (mag == 0.0);

		m1 = approx_mag1(a,b,c);
		m2 = approx_mag2(a,b,c);

		error1 = (m1>mag) ? (m1 / mag - 1.0) : (mag / m1 - 1.0);
		error2 = (m2>mag) ? (m2 / mag - 1.0) : (mag / m2 - 1.0);

		total_error1 += error1;
		total_error2 += error2;

		if (error1 > worst_error1) {
			worst_error1 = error1;
			worst_error1_a = a;
			worst_error1_b = b;
			worst_error1_c = c;

			worst_error1_mag = mag;
			worst_error1_m1 = m1;
			worst_error1_m2 = m2;
			
		}

		if (error2 > worst_error2) {
			worst_error2 = error2;
			worst_error2_a = a;
			worst_error2_b = b;
			worst_error2_c = c;

			worst_error2_mag = mag;
			worst_error2_m1 = m1;
			worst_error2_m2 = m2;
		}

	}

	printf("Average error:  %lf   %lf\n",total_error1/n_tests,total_error2/n_tests);
	printf("Worst error:    %lf   %lf\n",worst_error1,worst_error2);
	printf("Worst values:   (%lf,%lf,%lf)   (%lf,%lf,%lf)\n",worst_error1_a,worst_error1_b,worst_error1_c,
																				worst_error2_a,worst_error2_b,worst_error2_c);
	printf("Worst values:   (%lf,%lf,%lf)   (%lf,%lf,%lf)\n",worst_error1_mag,worst_error1_m1,worst_error1_m2,
																				worst_error2_mag,worst_error2_m1,worst_error2_m2);


}
ÿ