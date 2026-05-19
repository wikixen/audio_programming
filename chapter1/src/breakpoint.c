// breakpoint.c allows for use of breakpoint files
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "breakpoint.h"

/**
 * usage: breakpoint [-n number] [-c number] [-s number] [-t timestamp number] [-i ins/del timestamp]  infile.txt
 * -n: normalize breakpoint values relative to a max value
 * -c: scale breakpoint values by a factor
 * -s: shift breakpoint array up or down
 * -t: truncate duration by a number between 0 or 1
 * -i: insert/delete point in array
 */

int main(int argc, char const *argv[])
{
  printf("breakpoint: Find the duration of breakpoint file\n");
  if (argc < 2)
  {
    printf("usage: breakpoint [-n number] [-c number] [-s number] [-t timestamp number] infile.txt \n");
    printf("\t-n: normalize breakpoint values relative to a max value\n");
    printf("\t-c: scale breakpoint values by a factor\n");
    printf("\t-s: shift breakpoint array up or down\n");
    printf("\t-t: truncate duration by a number between 0 or 1\n");

    return 0;
  }

  FLAG flags[4] = {
      {false, 0}, // n
      {false, 0}, // c
      {false, 0}, // s
      {false, 0}, // t
  };

  int check = 0;
  while (argc > 2)
  {
    if (argv[1][0] == '-')
    {
      char flag = argv[1][1];
      switch (flag)
      {
      case 'n':
        flags[0].state = 1;
        flags[0].value = strtof(argv[1 + 1], NULL);
        check++;
        break;
      case 'c':
        flags[1].state = 1;
        flags[1].value = strtof(argv[1 + 1], NULL);
        check++;
        break;
      case 's':
        flags[2].state = 1;
        flags[2].value = strtof(argv[1 + 1], NULL);
        check++;
        break;
      case 't':
        flags[3].state = 1;
        flags[3].value = strtof(argv[1 + 1], NULL);
        if (flags[3].value < 0 || flags[3].value > 1)
        {
          printf("ERROR: t flag value must be within the range 0-1\n");
          return 1;
        }
        check++;
        break;
      default:
        printf("ERROR: incorrect flag option %s\n", argv[1]);
        return 1;
      }

      if (check > 0)
      {
        argc--;
        argv++;
        check = 0;
      }

      argc--;
      argv++;
    }
    else
    {
      break;
    }
  }
  FILE *fp = fopen(argv[1], "r");
  if (fp == NULL)
  {
    printf("ERROR: Could not open file '%s'. Make sure the path is correct.\n", argv[argc-1]);
    return 0;
  }

  long size = 0;
  // flags[3] is t(runcate) flag
  BREAKPOINT *points = get_breakpoints(fp, &size, flags[3]);
  if (points == NULL)
  {
    printf("No breakpoints read.\n");
    fclose(fp);
    return 1;
  }

  if (size < 2)
  {
    printf("Error: at least two breakpoints required\n");
    free(points);
    fclose(fp);
    return 1;
  }

  if (points[0].time != 0.0)
  {
    printf("Error in breakpoint data: first time must be 0.0\n");
    free(points);
    fclose(fp);
    return 1;
  }

  // Flag manipulations

  // n, c, or s flag == true
  if (flags[0].state == true || flags[1].state == true || flags[2].state == true)
  {
    points = alter_breakpoint(points, flags, size);
  }

  printf("read %ld breakpoints\n", size);
  double duration = points[size-1].time;
  printf("duration: %f seconds\n", duration);
  BREAKPOINT point = max_point(points, size);
  printf("max value: %f at %f secs\n", point.value, point.time);
  free(points);
  fclose(fp);

  return 0;
}

// max_point finds the largest value of a provided breakpoint file
BREAKPOINT max_point(const BREAKPOINT *points, unsigned long nPoints)
{
  BREAKPOINT point = points[0];
  for (size_t i = 0; i < nPoints; i++)
  {
    if (point.value < points[i].value)
    {
      point.time = points[i].time;
      point.value = points[i].value;
    }
  }

  return point;
}

BREAKPOINT *get_breakpoints(FILE *fp, long *pSize, FLAG tFlag)
{
  if (fp == NULL)
  {
    return NULL;
  }

  long size = 64;
  BREAKPOINT *points = (BREAKPOINT *)malloc(sizeof(BREAKPOINT) * size);
  if (points == NULL)
  {
    return NULL;
  }

  char line[80];
  int got = 0;
  unsigned long nPoints = 0;
  float lastTime = 0.0;

  BREAKPOINT tempPoint = {0, 0};
  while (fgets(line, 80, fp))
  {
    got = sscanf(line, "%lf:%lf", &tempPoint.time, &tempPoint.value);
    if (got < 0)
    {
      continue;
    }
    if (got == 0)
    {
      printf("Line %lu has non-numeric data\n", nPoints++);
      break;
    }
    if (got == 1)
    {
      printf("Incomplete breakpoint found at point %lu\n", nPoints++);
      break;
    }
    if (tempPoint.time < lastTime)
    {
      printf("data error at point %lu: time not increasing\n", nPoints++);
      break;
    }

    // Truncates file to requested time
    if (tFlag.state == true && tempPoint.time >= tFlag.value)
    {
      break;
    }

    points[nPoints].time = tempPoint.time;
    points[nPoints].value = tempPoint.value;
    lastTime = tempPoint.time;

    if (++nPoints == size)
    {
      BREAKPOINT *tmp;
      size += nPoints;
      tmp = (BREAKPOINT *)realloc(points, sizeof(BREAKPOINT) * size);
      if (tmp == NULL) // Release memory & return NULL
      {
        nPoints = 0;
        free(points);
        points = NULL;
        break;
      }
      points = tmp;
    }
  }

  // Check that file size>0
  if (nPoints)
  {
    *pSize = nPoints;
  }

  return points;
}

BREAKPOINT *alter_breakpoint(BREAKPOINT *points, FLAG *flags, long size)
{
  BREAKPOINT max = max_point(points, size);
  for (size_t i = 0; i < size; i++)
  {
    if (flags[0].state == true)
    {
      points[i].value = (points[i].value/max.value) * flags[0].value;
    }
    if (flags[1].state == true)
    {
      points[i].value *= flags[1].value;
    }
    if (flags[2].state == true)
    {
      points[i].value += flags[2].value;
    }
  }
  return points;
}

