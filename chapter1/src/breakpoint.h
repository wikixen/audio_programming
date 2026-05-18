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
BREAKPOINT *get_breakpoints(FILE *fp, long *pSize, bool tFlag);