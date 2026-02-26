#include <stdio.h>
#include <math.h>

int main(int argc, char const *argv[])
{
  if (argc != 3)
  {
    printf("usage: nscale notes midinote\n");
    return 1;
  }

  int notes = argv[1];
  if (notes <= 0 || notes >24)
  {
    printf("");
    return 1;
  }

  int midinote = argv[2];
  if (midinote < 0 || midinote > 127)
  {
    printf("ERROR: midinote must be in range 0-127.\n");
    return 1;
  }

  return 0;
}
