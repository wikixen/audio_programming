// breakpoint.c allows for use of breakpoint files
#include <stdlib.h>
#include <stdio.h>

/**
 * usage: breakpoint [-c] [-n] [-s] [-d] [-t] [-i]  infile.txt 
 * -c: stretch/shrink times
 * -n: normalize breakpoint values relative to a max value
 * -s: shift breakpoint array up or down
 * -d: scale breakpoint values by a factor
 * -t: truncate/extend duration
 * -i: insert/delete point in array
 */

typedef unsigned short WORD;
typedef unsigned long DWORD;

// SFINFO is an object that describes a soundfile
typedef struct
{
  DWORD nSamples;
  DWORD sampleRate;
  WORD nChannels;
  char *name;
} SFINFO;

typedef struct 
{
  double time;
  double value;
} BREAKPOINT;


BREAKPOINT max_point(const BREAKPOINT *points, unsigned long nPoints);
BREAKPOINT *get_breakpoints(FILE *fp, long *pSize);

// max_point finds the largest value of a provided breakpoint file
BREAKPOINT max_point(const BREAKPOINT *points, unsigned long nPoints){
  BREAKPOINT point;
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

BREAKPOINT *get_breakpoints(FILE *fp, long *pSize)
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
  while (fgets(line, 80, fp))
  {
    got = sscanf(line, "%lf:%lf", &points[nPoints].time, &points[nPoints].value);
    if (got<0)
    {
      continue;
    }
    if (got==0)
    {
      printf("Line %lu has non-numeric data\n", nPoints++);
      break;
    }
    if (got == 1)
    {
      printf("Incomplete breakpoint found at point %lu\n", nPoints++);
      break;
    }
    if (points[nPoints].time < lastTime)
    {
      printf("data error at point %lu: time not increasing\n", nPoints++);
      break;
    }
    
    lastTime = points[nPoints].time;
    if (++nPoints == size)
    {
      BREAKPOINT *tmp;
      // size += NPOINTS;??
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
  if (nPoints)
  {
    *pSize = nPoints;
  }

  return points;
}

int main(int argc, char const *argv[])
{ 
  printf("breakpoint: Find the duration of breakpoint file\n");
  if (argc < 2)
  {
    printf("usage: breakpoint [-s] []  infile.txt \n");
    
    return 0;
  }

  int cFlag = 0, nFlag = 0, sFlag = 0, dFlag = 0, tFlag = 0, iFlag = 0;
  while (argc > 2)
  {
    if (argv[1][0] == '-')
    {
      char flag = argv[1][1];
      switch (flag)
      {
      case 'c':
        cFlag = 1;
        break;
      case 'n':
        nFlag = 1;
        break;
      case 's':
        sFlag = 1;
        break;
      case 'd':
        dFlag = 1;
        break;
      case 't':
        tFlag = 1;
        break;
      case 'i':
        iFlag = 1;
        break;
      default:
        printf("ERROR: incorrect flag option %s\n", argv[1]);
        return 1;
      }
      argc--;
      argv++;
    }
    else{
      break;
    }
  }

  FILE *fp = fopen(argv[1], "r");
  if (fp == NULL)
  {
    return 0;
  }
  
  long size = 0;
  BREAKPOINT *points = get_breakpoints(fp, &size);
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

  printf("read %d breakpoints\n", size);
  double duration = points[size--].time;
  printf("duration: %f seconds\n", duration);
  BREAKPOINT point = max_point(points, size);
  printf("max value: %f at %f secs\n", point.value, point.time);
  free(points);
  fclose(fp);

  return 0;
}
