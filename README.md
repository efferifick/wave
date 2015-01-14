## Compiling
```bash
	gcc -Wall -std=c99 -o wave wave.c -lm -DNDEBUG
```

## Usage
```bash
  usage:
	wave [options] FREQ1 FREQ2 FREQ3
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
```

If a file is specified, it needs to have the following format.

```
<SAMPLES> <AMPLITUDE> <FREQ1> <FREQ2> <...>
<SAMPLES> <AMPLITUDE> <FREQ1> <FREQ2> <...>
<SAMPLES> <AMPLITUDE> <FREQ1> <FREQ2> <...>
```

The output is given by a float. This is done so that we can play its output
with the standard audio format PCM 32 bytes.

## Example to produce sound

on linux:
```bash
m4 -R music.m4f -Q chopsticks.txt |awk '{$1 = $1*48000; print $0}' | ./wave |
aplay -f FLOAT_LE -r 48000 -c 1 -q
```
Please select proper aplay endianness for your machine. 

on mac I haven't been able to pipe directly but I was able to create an mp3

```bash
m4 -R music.m4f -Q chopsticks.txt | awk '{$1 = $1*48000; print $0}' | ./wave > chopsticks.raw
sox -r 48000 -e floating-point -c 1 -b 32 chopsticks.raw chopsticks.mp3
```

