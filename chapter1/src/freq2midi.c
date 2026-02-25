#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

/**
 * This program finds the MIDI note number associated with the given frequency
 */

int main()
{
  /**
   * semitone_ratio = ~1.0594631; Is the amount of change in frequency between a semitone
   * c0 = frequency of MIDI Note 0 ie lowest possible note
   * c5 = frequency of Middle C ie MIDI note 60
   */
  double semitone_ratio, c5, c0;
  semitone_ratio = pow(2, 1.0 / 12.0);
  // c5 = 220.0 * pow(semitone_ratio, 3.0);
  c5 = 207.5 * pow(semitone_ratio, 3.0);
  c0 = c5 * pow(0.5, 5.0);

  printf("Enter a frequency that isn't negative: ");
  char message[256];
  if (fgets(message, sizeof(message), stdin) == NULL)
  {
    printf("ERROR: There was an error reading your input.\n");
    return 1;
  }
  message[strcspn(message, "\n")] = '\0';

  double frequency = atof(message);
  if (frequency < 0)
  {
    printf("ERROR: %s isn't a valid frequency. Make sure your frequency is 0 or greater.\n", message);
    return 1;
  }

  double fracmidi = log(frequency/c0)/log(semitone_ratio);
  int midinote = (int)(fracmidi + 0.5);
  
  printf("The nearest MIDI note to the frequency %.2f is %d\n", frequency, midinote);
  
  // Exercise 1.2.6 PG.77
  // Get closest MIDI note above & below frequency
  double freqL = c0 * pow(semitone_ratio, floor(fracmidi));
  double freqH = c0 * pow(semitone_ratio, ceil(fracmidi));
  double freqRange = freqH - freqL;

  double midinoteFreq = c0 * pow(semitone_ratio, midinote);

  int bend = (int)(((frequency - midinoteFreq) / freqRange) * 100);

  printf("pitchbend = %d%%\n", bend);

  return 0;
}