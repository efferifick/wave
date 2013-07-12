## Compiling
	gcc -Wall -std=c99 -o wave wave.c -lm

## Usage
	wave FREQ

Where frequency is to be casted to a double. It will be the frequency played.
At the moment, wave defaults to a sampling rate of 48000 Hz and samples the
frequency FREQ 12000 times. This can be easily changed so that the code can
take different sampling rates and outputs variable samples. 

The output is given by a float. This is done so that we can play its output
with the standard audio format PCM 32 bytes.

## Example to produce sound

	m4 -R music.m4f -Q littlelamb.txt |awk '{$1 = $1*48000}' | ./wave | aplay -f FLOAT_LE -r 48000 -c 1 -q

Please select proper aplay endianness for your machine. 
