## What is this?

This repo contains a small C program that outputs a sine wave of a determined
frequency in binary form. That
means that when you say something like

```bash
./wave 100.0
```

You will only see gibberish. However, it is useful because you can pipe the
output to `aplay` or `sox` and be able to play music.

## Compiling
```bash
	gcc -Wall -std=c99 -o wave wave.c -lm -DNDEBUG
```

## Usage
```bash
  usage:
	wave [options] [FREQ1 FREQ2 FREQ3 ..]
  options:

  -a, --amplitude    takes an integer for the amplitude of the wave.
                     defaults to 1.
  -s, --samples      how many samples do we need?
                     defaults to 48000. (one second).
  -r, --rate         at what rate are we to sample?
                     defaults to 48kHz.
  -f, --file         do we want input from a file? if so, which one?
  -h, --help         prints help
  -v, --version      prints version

  If no frequencies are specified, it will take input from stdin. This is
  useful for piping.
```

If a file is specified, it needs to have the following format.
If the program is receiving input through stdin, it also needs the same format.

```
<SAMPLES> <AMPLITUDE> <FREQ1> <FREQ2> <...>
<SAMPLES> <AMPLITUDE> <FREQ1> <FREQ2> <...>
<SAMPLES> <AMPLITUDE> <FREQ1> <FREQ2> <...>
```

The output is given by a float. This is done so that we can play its output
with the standard audio format PCM 32 bytes.

## Example to produce music

on linux:
```bash
m4 -R ../music.m4f -Q ../chopsticks.txt | awk '{$1 = $1*48000; print $0}' | ./wave | aplay -f FLOAT_LE -r 48000 -c 1 -q
```

on mac:
```bash
m4 -R ../music.m4f -Q ../chopsticks.txt | awk '{$1 = $1*48000; print $0}' | ./wave | sox -t raw -r 48000 -e floating-point -c 1 -b 32 - -tcoreaudio
```

Please select proper aplay endianness for your machine. 

