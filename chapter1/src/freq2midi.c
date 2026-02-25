#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

/**
 * This program calculates the closest MIDI note to the provided frequency as well as it's pitch bend
 */

int main(int argc, char *argv[])
{
  // Usage description
  if (argc != 4)
  {
    printf("%s : finds the MIDI note closest to frequency.\n", argv[0]);
    printf("  usage: %s rootA frequency octaveDivision\n", argv[0]);
    printf("  range: 0 <= rootA\n");
    printf("  range: 0 <= frequency\n");
    printf("  range: 5 <= octaveDivision\n");

    return 1;
  }

  // a4 is the root A frequency in Hz
  double a4 = strtof(argv[1], NULL);
  if (a4 < 0)
  {
    printf("ERROR: %s frequency isn't a valid root frequency. Enter a value 0 or greater.\n", argv[1]);
    return 1;
  }

  double frequency = strtof(argv[2], NULL);
  if (frequency < 0)
  {
    printf("ERROR: %s frequency isn't a valid root frequency. Enter a value 0 or greater.\n", argv[2]);
    return 1;
  }

  // octaveDiv is number of semitones per octave
  int octaveDiv = strtol(argv[3], NULL, 10);
  if (octaveDiv < 5)
  {
    printf("ERROR: %s isn't a valid division of octaves. Make sure your division is above 5.\n", argv[3]);
    return 1;
  }

  // Formula to find nearest MIDI note; Made through algebra on other func
  double fracmidi = octaveDiv * log2(frequency / a4) + 69;
  int midinote = (int)(fracmidi + 0.5);

  printf("The nearest MIDI note to the frequency %.2f is %d\n", frequency, midinote);

  // Exercise 1.2.6 PG.77
  // Get (frequency of MIDI note above midinote)-(frequency of MIDI note below midinote)
  double freqL = a4 * pow(2, (floor(fracmidi) - 69.0) / (double)octaveDiv);
  double freqH = a4 * pow(2, (ceil(fracmidi) - 69.0) / (double)octaveDiv);
  double freqRange = freqH - freqL;

  double midinoteFreq = a4 * pow(2, (midinote - 69.0) / (double)octaveDiv);

  int bend = (int)(((frequency - midinoteFreq) / freqRange) * 100);

  printf("pitchbend = %d%%\n", bend);
}