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

#include <errno.h>
#include <getopt.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "dbg.h"

#ifndef M_PI
#define M_PI (3.141592654)
#endif

#ifndef MAX_FREQS
#define MAX_FREQS 10
#endif

const static double TWO_PI = 2*M_PI;

void start_wave(FILE* file, const long int rate);
int get_all_frequencies(double*, int, char*);
void calculate_sine_of(double* frequencies, int freq_quantity, double amplitude, long int samples, const long int rate);
double get_sample(double frequency, int sample_location, const long int rate);

  int
main(int argc, char** argv) 
{

  bool is_filein = false;
  bool is_stdin = false;
  bool is_argin = false;
  const char* PRGM_NAME = argv[0];
  char* HELP_STR;
  char* VERSION_STR;
  char* endptr = NULL;
  char* filename;
  int retval = 0;
  int option_index = 0;
  int current_option = 0;
  int cnt_freqs = 0;
  long int samples = 48000;
  long int rate = 48000;
  double frequency = 0;
  double amplitude = 1;
  FILE* file = NULL;

  const struct option long_opts[] = 
  {
    {"version", no_argument, 0, 'v'},
    {"help", no_argument, 0, 'h'},
    {"samples", required_argument, 0, 's'},
    {"rate", required_argument, 0, 'r'},
    {"amplitude", required_argument, 0, 'a'},
    {"file", required_argument, 0, 'f'},
    {0, 0, 0, 0}
  };


  do {

    current_option = getopt_long(argc, argv, "whvs:r:a:f:", long_opts, &option_index);

    switch (current_option) 
    {
      case -1:
        /* No more options. Leaves optind as the index of next argv */
        break;
      case 'h': /* Help */
        retval = asprintf(&HELP_STR, "usage: %s\n", PRGM_NAME);
        check(0 != retval, "asprintf returns non zero");
        retval = fprintf(stdout, "%s", HELP_STR);
        check(0 < retval, "fprintf fails");
        free(HELP_STR);
        break;
      case 'v': /* Version */
        retval = asprintf(&VERSION_STR, "%s version 0.02\n", PRGM_NAME);
        check(0 != retval, "asprintf returns non zero");
        retval = fprintf(stdout, "%s", VERSION_STR);
        check(0 < retval, "fprintf fails");
        free(VERSION_STR);
        break;
      case 's': /* Samples defaults to 48k (or 1 sec) */ 
        endptr = NULL; 
        samples = strtol(optarg, &endptr, 10);
        check(0 == errno, "strtol sets errno to non zero");
        check(endptr != optarg, "no number found");
        check(0 < samples, "zero or negative samples");
        debug("samples: %ld", samples);
        break;
      case 'r': /* Rate defaults to 48 kHz */ 
        endptr = NULL;
        rate = strtol(optarg, &endptr, 10);
        check(0 == errno, "strtol sets errno to non zero");
        check(endptr != optarg, "no number found");
        check(0 < rate, "zero or negative rate");
        debug("rate: %ld", rate);
        break;
      case 'a': /* Amplitude defaults to 1.0 */
        amplitude = strtod(optarg, &endptr);
        check(0 == errno, "strtod sets errno to non zero");
        check(endptr != optarg, "no number found");
        /* zero amplitude is valid. it is silence. */
        check(0 <= amplitude, "negative amplitude");
        debug("amplitude: %lf", amplitude);
        break;
      case 'f': /* File, input file */
        /* This option will diregard samples and amplitudes as they are to be
         * expected on the input file */
        filename = optarg;
        is_filein = true;
        debug("file name: %s", filename);
        break;
      default:
        retval = fprintf(stderr, "unrecognized option, ignoring\n");
        check(0 < retval, "fprintf fails");
        break;
    }//switch

  } while (current_option != -1);

  cnt_freqs = argc - optind;

  if ((0 == cnt_freqs) && (!is_filein)) {
    /* This option will disregard samples and amplitudes as they are to be
     * expected in stdin */
    is_stdin = true;
  } else {
    is_argin = true;
  }

  if (is_stdin) {
    //get input from stdin...
    file = stdin;
    debug("using stdin");
  } else if (is_filein) {
    file = fopen(filename, "r");
    check(NULL != file, "fopen fails");
    debug("using a specified file");
  } else if (is_argin) {
    // write a temporary file that looks like the ones you are used to.
    // so... SAMPLES AMPLITUDE FREQ1 FREQ2 ...
    file = fopen("/tmp/wavetmpfile.txt", "w");
    check(NULL != file, "fopen fails");
    debug("creating a tmp file");
    retval = fprintf(file, "%ld %lf ", samples, amplitude);
    check(0 < retval, "fprintf fails");


    for (int i = 0; i < cnt_freqs; i++) {
      endptr = NULL;
      frequency = strtod(argv[optind + i], &endptr);
      check(0 == errno, "strtod sets errno to non zero");
      check(argv[optind + i] != endptr, "no frequency found");
      check(0 < frequency, "frequency is zero or negative");
      retval = fprintf(file, "%lf ", frequency);
      check(0 < retval, "fprintf fails");
      debug("frequency: %lf", frequency);
    }

    fclose(file);
    file = fopen("/tmp/wavetmpfile.txt", "r");
    check(NULL != file, "fopen fails");
  }

  // after this, we always take input from f, and we always have the rate in
  // rate.
  // samples and frequencies are read from f.

  start_wave(file, rate);

  exit(EXIT_SUCCESS);

error:
  exit(EXIT_FAILURE);
}

  void
start_wave(FILE* file, const long int rate) 
{
  char* buffer = NULL;
  char* buffercpy = NULL;
  char* retstr = NULL;
  char* savebuffer = NULL;
  char* tok = NULL;
  char* endptr = NULL;
  long int samples = 0;
  double amplitude = 0;
  int freq_quantity = 0;
  unsigned int size = BUFSIZ;
  double* frequencies = NULL;

  buffer = calloc(size, sizeof(char));
  check(NULL != buffer, "calloc failed");

  buffercpy = calloc(size, sizeof(char));
  check(NULL != buffercpy, "calloc failed");

  // ten frequencies at the same time looks ok to me.
  // at least for now.
  frequencies = calloc(MAX_FREQS, sizeof(double));
  check(NULL != frequencies, "calloc failed");

  do {

    retstr = fgets(buffer, size, file);
    debug("working with: %s", buffer);

    if (NULL == retstr) { break; }

    /* First we expect an integer that correspond to samples */
    tok = strtok_r(buffer, " \n", &savebuffer);
    endptr = NULL;
    samples = strtol(tok, &endptr, 10);
    check(0 == errno, "strtol sets errno to non zero");

    // we could continue instead of ending. be more unixy.
    check(tok != endptr, "no integer found");
    check(0 < samples, "samples is zero or negative");

    debug("samples: %ld", samples);

    /* Then we expect a double that corresponds to amplitude */
    tok = strtok_r(NULL, " \n", &savebuffer);
    endptr = NULL;
    amplitude = strtod(tok, &endptr);
    check(0 == errno, "strtod sets errno to non zero");
    // we could continue nstead of ending. be more unixy.
    check(tok != endptr, "no double found");
    /* zero amplitude is valid. it is silence. */
    check(0 <= amplitude, "amplitude is negative");
    debug("amplitude: %lf", amplitude);

    /* save buffer contains our position in the line. we can continue to use
     * strtok_r(NULL, " \n", &savebuffer) or we can make a new one in a new
     * function to make that function more explicit. We choose the latter.
     */

    memcpy(buffercpy, savebuffer, size); // it is safe to drop the return value.

    /* Then we need to look for all the frequencies in this line */
    freq_quantity = get_all_frequencies(frequencies, MAX_FREQS, buffercpy);

    /* Then we call the output function */
    calculate_sine_of(frequencies, freq_quantity, amplitude, samples, rate);

    /* Then we free */
    memset(buffer, '\0', size);
    memset(buffercpy, '\0', size);
    memset(frequencies, 0, sizeof(double) * MAX_FREQS);

  } while(true);

  free(buffer);
  free(buffercpy);
  free(frequencies);
  fclose(file);

  return;

error:
  exit(EXIT_FAILURE);

}

  int
get_all_frequencies(double* frequencies, int max, char* line)
{
  char* tok = NULL;
  char* savebuffer = NULL;
  int count = 0;
  double frequency = 0.0;
  char* endptr = NULL;

  do {

    // setting the tokenizer.
    if (count == 0)
      tok = strtok_r(line, " \n", &savebuffer);
    else
      tok = strtok_r(NULL, " \n", &savebuffer);

    // error checking the first one.
    if ((count == 0) && (tok == NULL)) 
    { 
      frequencies[count] = 0.0;
    }

    // parsing.
    if (tok != NULL) 
    {
      frequency = strtod(tok, &endptr);
      check(0 == errno, "strtod sets errno to zero");
      check(tok != endptr, "no frequency found");
      frequencies[count] = frequency;
      debug("frequency[%d]: %lf", count, frequencies[count]);
    }

    count++;

  } while ((tok != NULL) && (count < max));

  return count;


error:
  exit(EXIT_FAILURE);
}


  void
calculate_sine_of(double* frequencies, int freq_quantity, double amplitude, long int samples, const long int rate) 
{

  double sample = 0.0;
  float sample_f = 0.0;

  for (long int i = 0; i < samples; i++) {
    for (int j = 0; j < freq_quantity; j++) {
      sample += get_sample(frequencies[j], i, rate);
    }

    sample = sample / freq_quantity * amplitude;
    sample_f = (float)sample;
    write(STDOUT_FILENO, &sample_f, sizeof(sample_f));
  }
}

  double
get_sample(double frequency, int sample_location, const long int rate) 
{
  double angleincr = TWO_PI * frequency / rate;
  return sin(angleincr*sample_location);
}
