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
```
  usage:
	wave [options] [FREQ1 FREQ2 FREQ3 ..]
  options:

  -h                    Prints out this message.
  --help

  -v                    Prints out program version.
  --version

  -a DOUBLE             Sets amplitude value.
  --amplitude DOUBLE    Default: 1.0

  -r INT                Sets sampling rate.
  --rate INT            Default: 48000 (Hz).

  -s INT                Sets number of samples to be taken.
  --samples INT         Defaults: 48000. (Equivalent to 1 second on default
                          rate)

  -f PATH               Sets input file.
  --file PATH

  NOTES:
    For best results amplitude should be in the range of [0, 1].

    If no arguments are given, input is expected from stdin. This is useful
    for piping. Input is expected as though reading from a file. Ex.

      \`\`\`
        echo 48000 1.0 440 | ./wave | sox -t raw -r 48000 -e
        floating-point -c 1 -b 32 - -tcoreaudio
      \`\`\`

    If reading from a file or stdin, the program expects this input format.

      \`\`\`
        <SAMPLES> <AMPLITUDE> <FREQ1> <FREQ2> <...>
        <SAMPLES> <AMPLITUDE> <FREQ1> <FREQ2> <...>
        <SAMPLES> <AMPLITUDE> <FREQ1> <FREQ2> <...>
      \`\`\`

    Each line will output after the one before has run out of samples.

    Get the source or file an issue at:
      \`\`\`
        https://github.com/efferifick/wave
      \`\`\`

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
