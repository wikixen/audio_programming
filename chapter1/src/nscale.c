#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/*
`* nscale displays the frequencies of equal-tempered intervals for N notes per octave
 * 24 is the max amount of notes that can be shown; Chosen for readability
 * Frequency = (Root A) * 2^[(Midinote)-(Middle C Midinote ie 69)/ notes] 
*/

int main(int argc, char const *argv[])
{
  int test = 0;
  if (argc != 3)
  {
    printf("usage: nscale notes midinote\n");
    return 1;
  }

  int notes = strtol(argv[1], NULL, 10);
  if (notes <= 0 || notes >24)
  {
    printf("ERROR: notes must be in range 0-24.\n");
    return 1;
  }

  int midinote = strtol(argv[2], NULL, 10);
  if (midinote < 0 || midinote > 127)
  {
    printf("ERROR: midinote must be in range 0-127.\n");
    return 1;
  }

  double intervals[24] = {0};
  int inc = midinote;
  for (size_t i = 0; i <= notes; i++)
  {
    // Calculation of frequency for provided MIDI notes
    intervals[i] = 440 * pow(2, (inc - 69.0) / notes); 
    printf("%d: %.2f\n", inc, intervals[i]);
    inc++;
  }

  return 0;
}
