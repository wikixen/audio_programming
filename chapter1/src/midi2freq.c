#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

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
  double semitone_ratio, c5, c0;
  semitone_ratio = pow(2, 1.0 / 12.0);
  c5 = 220.0 * pow(semitone_ratio, 3.0);
  c0 = c5 * pow(0.5, 5.0);

  printf("Enter a MIDI note(0-127): ");
  char message[256];
  if (fgets(message, sizeof(message), stdin) == NULL)
  {
    printf("ERROR: There was an error reading your input.\n");
    return 1;
  }
  message[strcspn(message, "\n")] = '\0'; // Strips '\n' from message & replaces it with '\0'

  if (message[0]=='\0')
  {
    printf("Have a nice day!\n");
    return 0;
  }
  
  int midinote = atoi(message);
  if (midinote<0 || midinote>127)
  {
    printf("ERROR: %s isn't a valid MIDI note. Make sure your note is in the range 0-127.\n", message);
    return 1;
  }
  
  // Calculation of frequency of provided note
  double frequency = c0 * pow(semitone_ratio, midinote);

  // Exercise 1.2.5 PG.77
  // Only works for MIDI note 60-72 ie one octave
  double c0_length, c5_length, length_interval;
  c0_length = 660;
  c5_length = 330;
  length_interval = (c0_length - c5_length) / 12;

  double midinote_length = 330+((midinote%12) * length_interval);

  printf("MIDI Note %d has frequency %f & a string length of %.2f\n", midinote, frequency, midinote_length);

  return 0;
}