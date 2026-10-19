// Generates ET tables for N-notes to the octave (N<=24)
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/**
 * usage: iscale [-m] [-i] N startval [outfile.txt]
 * -m: use MIDI note numbers instead of frequencies
 * -i: prints the calculated intervals along with abs freq
 * N: number of notes to process
 * startval: starting frequency or midinote if -m is set 
 * outfile: optional text filename for output data
 */

int main(int argc, char const *argv[])
{
  if (argc < 3)
  {
    printf("error: insufficient arguments provided\n");
    printf("usage: iscale [-m] [-i] N startval [outfile.txt]\n");
    return 1;
  }

  // Check for flags
  int mFlag = 0;
  int iFlag = 0;
  while (argc > 1)
  {
    if (argv[1][0] == '-')
    {
      char flag = argv[1][1];
      switch (flag)
      {
      case 'm':
        mFlag = 1; // Set to true if -m flag is used
        break;
      case 'i':
        iFlag = 1; // Set to true if -i flag is used
        break;
      default:
        printf("ERROR: incorrect flag option %s\n", argv[1]);
        return 1;
      }
      argc--;
      argv++;
    }
    else
    {
      break;
    }
  }
  
  // Read and check all args
  int notes = strtol(argv[1], NULL, 10);
  if (notes <= 0 || notes > 24)
  {
    printf("ERROR: notes must be in range 0-24.\n");
    return 1;
  }

  int startval = strtof(argv[2], NULL);
  if (mFlag == 1 && (startval > 127 || startval < 0))
  {
    printf("ERROR: %f isn't a valid midinote. Enter a value in the range 0-127.\n");
    return 1;
  }
  else if (mFlag == 0 && startval <= 0.0)
  {
    printf("ERROR: %f isn't a valid frequency. Enter a value greater than 0.\n", startval);
    return 1;
  }

  FILE *fp = NULL;
  if (argc == 4)
  {
    fp = fopen(argv[3], "w");
    if (fp == NULL)
    {
      printf("WARNING: unable to create file %s\n", argv[3]);
      perror("");
    }
  }

  double basefreq = startval, ratio = 0;
  if (mFlag == 1)
  {
    double c0, c5;
    ratio = pow(2.0, 1.0 / 12.0);
    c5 = 220.0 * pow(ratio, 3);
    c0 = c5 * pow(0.5, 5);
    basefreq = c0 * pow(ratio, startval);
  }

  ratio = pow(2.0, 1.0 / notes);
  double intervals[25] = {0};
  for (size_t i = 0; i <= notes; i++)
  {
    intervals[i] = basefreq;
    basefreq *= ratio;
  }

  int error = 0; // Flag to check if theres an error during execution of the code below
  for (size_t i = 0; i <= notes; i++)
  {
    if (iFlag == 1)
    {
      printf("%d:\t%f\t%f\n", i, pow(ratio, i), intervals[i]);
    }
    else
    {
      printf("%d:\t%f\n", i, intervals[i]);
    }
    if (fp)
    {
      if (iFlag == 1)
      {
        error = fprintf(fp, "%d:\t%f\t%f\n", i, pow(ratio, i), intervals[i]);
      }
      else
      {
        error = fprintf(fp, "%d:\t%f\n", i, intervals[i]);
      }
      if (error < 0)
      {
        break;
      }
    }
  }

  if (error < 0)
  {
    perror("There was an error writing the file.\n");
  }
  if (fp)
  {
    fclose(fp);
  }

  return 0;
}
