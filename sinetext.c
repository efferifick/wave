#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <getopt.h>

#ifndef M_PI
#define M_PI (3.141592654)
#endif

const static char* HELP_STR = "sinetext #_of_samples" 
				"#_of_frequency_in_Hz #_of_sample_rate";
const static char* VERSION_STR = "sinetext version 0.01";

static int verbose = 0;

/*
 * you need... sample, freq, samplerate...
 *
 */
int
main(int argc, char** argv)
{
	int c;
	int optindex =  0;
	int sample_size; /* arg1 */
	const double TWO_PI = 2*M_PI;
	double frequency; /* arg2 */
	double sample;
	double sample_rate; /* arg3 */
	double angleincr;
	

	const struct option long_opts[] = 
	{
		{"verbose", no_argument, &verbose, 1},
		{"version", no_argument, 0, 'v'},
		{"help", no_argument, 0, 'h'},
		{0, 0, 0, 0}
	};

	while ((c = getopt_long(argc, argv, "vh", long_opts, &optindex)) != -1)
	{
		switch(c)
		{
			case 0:
			break;
			
			case 'h':
			printf("%s\n", HELP_STR);
			exit(EXIT_SUCCESS);
			break;

			case 'v':
			printf("%s\n", VERSION_STR);
			exit(EXIT_SUCCESS);
			break;

			default:
			printf("%s\n", HELP_STR);
			exit(EXIT_FAILURE);
		}	
	}

	if ((argc - optind) != 3)
	{
		printf("%s\n", HELP_STR);
		exit(EXIT_FAILURE);
	}

	sample_size = atoi(argv[optind++]);
	frequency = atof(argv[optind++]);
	sample_rate = atof(argv[optind]);
	angleincr = TWO_PI * frequency / sample_rate;

	for (int i = 0; i < sample_size; i++) 
	{
		sample = sin(angleincr*i);
		printf("%.8f\n", sample);
	}
	exit(EXIT_SUCCESS);
}
