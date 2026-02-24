#include <stdio.h>
#include <math.h>

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
  double semitone_ratio = pow(2, 1.0 / 12.0),
         c5 = 220.0 * pow(semitone_ratio, 3.0),
         c0 = c5 * pow(0.5, 5.0);

  double frequency = 430.0;

  double fracmidi = log(frequency/c0)/log(semitone_ratio);
  int midinote = (int)(fracmidi + 0.5);
  
  printf("The nearest MIDI note to the frequency %.2f is %d\n", frequency, midinote);
  
  // Exercise 1.2.6 PG.77
  int midiL = floor(fracmidi),
      midiH = ceil(fracmidi);

  double freqL = c0 * pow(semitone_ratio, midiL);
  double freqH = c0 * pow(semitone_ratio, midiH);

  int bend = (int)(((freqH - frequency) / (freqH - freqL)) * 100 * -1);

  printf("pitchbend = %d%%\n", bend);

  return 0;
}