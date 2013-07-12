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
const static double TWO_PI = 2*M_PI;

static int verbose = 0;

/*
 * you need... sample, freq, samplerate...
 *
 */

void iterate_over_samples(int, double, double);
void print_sample(double, int);

int
main(int argc, char** argv)
{
	int c;
	int optindex =  0;
	int sample_size; /* arg1 */
	double frequency; /* arg2 */
	double sample_rate; /* arg3 */
	FILE *f;
	

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
			fprintf(stderr, "Non-valid option");
			exit(EXIT_FAILURE);
		}	
	}

	if ((argc - optind) == 0) {
		//get input from stdin...
		f = stdin;
	} else {
		f = fopen(argv[optind], "r");
	}

	//sample_size = atoi(argv[optind++]);
	sample_size = 12000; //This should be the new default? Or another argument.
	//I feel like this should be an argument and not an option.
	//frequency = atof(argv[optind++]); This should now be the only input
	//i get it at the while loop
	//sample_rate = atof(argv[optind]);
	sample_rate = 48000; //This is also a default option.


	char buf[1024];
	while(fgets(buf, sizeof(buf), f)) {
		sscanf(buf, "%d %lf", &sample_size, &frequency);
		iterate_over_samples(sample_size, frequency, sample_rate);
	}

	exit(EXIT_SUCCESS);
}

void iterate_over_samples(int sample_size, double frequency, double sample_rate)
{
	double angleincr = TWO_PI * frequency / sample_rate;
	for(int i = 0; i < sample_size; i++)
		print_sample(angleincr, i);
}

void print_sample(double angleincr, int i)
{
	float sample = sin(angleincr*i);
	write(STDOUT_FILENO, &sample, sizeof(sample));
}
