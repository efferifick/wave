/* The MIT License (MIT)
 *
 * Copyright (c) 2015 Erick Eduardo Ochoa Lopez
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE
 * SOFTWARE.
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <getopt.h>
#include <string.h>

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

//void iterate_over_samples(int, double, double);
float get_sample(double, double, int);
void print_array(double*, int);
void fill_array(double*, int);
void free_array(double*, int);


int
main(int argc, char** argv)
{
	int c;
	int optindex =  0;
	int sample_size; /* arg1 */
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

	double frequencies[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	char buf[1024]; // would be better if we got rid of this arbitrary
	//limit
	while(fgets(buf, sizeof(buf), f)) {
		char* tok = NULL;
		tok = strtok(buf, " ");
		sample_size = atoi(tok);
		fill_array(frequencies, 10);
		for(int i = 0; i < sample_size; i++)
		{
			int j = 0;
			float sample = 0;
			while((frequencies[j] != 0))
			{
				sample += get_sample(frequencies[j], sample_rate, i);
				//fprintf(stderr, "%lf\n", frequencies[j]);
				j++;
			}
			sample /= ++j;
			write(STDOUT_FILENO, &sample, sizeof(sample));
		}
		//iterate_over_samples(sample_size, frequency, sample_rate);
		free_array(frequencies, 10);
	}

	exit(EXIT_SUCCESS);
}
/*
void iterate_over_samples(int sample_size, double frequency, double sample_rate)
{
	double angleincr = TWO_PI * frequency / sample_rate;
	for(int i = 0; i < sample_size; i++)
		print_sample(angleincr, i);
}*/

float get_sample(double frequency, double rate, int sample_number)
{
	double angleincr = TWO_PI * frequency / rate;
	return (float) sin(angleincr*sample_number);
}

void
print_array(double *freq_arr, int len)
{
	for (int i = 0; i < len; i++)
		fprintf(stderr, "%lf\n", freq_arr[i]);
}

/*
* We need to call strtok with good string before calling this function.
*/
void
fill_array(double* freq_arr, int len)
{
	char* tok = NULL;
	int i = 0;
	tok = strtok(NULL, " ");
	while((tok != NULL) && (i < len)) //if i < len then overflow
	{
		freq_arr[i] = atof(tok); //should check for errors
		tok = strtok(NULL, " ");
		i++;
	}
}

void
free_array(double *freq_arr, int len)
{
	for (int i = 0; i < len; i++)
		freq_arr[i] = 0;
}
