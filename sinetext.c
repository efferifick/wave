#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>

#ifndef M_PI
#define M_PI (3.141592654)
#endif

int
main(int argc, char** argv)
{
	int i, n_samples;
	double sample;
	const double TWO_PI = 2*M_PI;
	double angleincr;
	n_samples = 50;
	angleincr = TWO_PI / n_samples;
	for (i = 0; i < n_samples; i++) 
	{
		sample = sin(angleincr*i);
		printf("%.8lf\n", sample);
	}
	fprintf(stderr, "done\n");
	exit(EXIT_SUCCESS);
}
