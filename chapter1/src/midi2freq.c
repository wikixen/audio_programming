#include <stdio.h>
#include <math.h>

/**
 * This program calculates the frequency of a MIDI note number
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

  // Actual calculation of frequency of provided note
  int midinote = 72; //C# above A=440
  double frequency = c0 * pow(semitone_ratio, midinote);

  // Exercise 1.2.5 PG.77
  // Only works for MIDI note 60-72 ie one octave 
  double c0_length = 660,
         c5_length = 330,
         length_interval = (c0_length - c5_length) / 12;

  double midinote_length = 330+((midinote%12) * length_interval);

  printf("MIDI Note %d has frequency %f & a string length of %.2f\n", midinote, frequency, midinote_length);

  return 0;
}