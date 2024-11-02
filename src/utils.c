#include "../include/utils.h"

void copy(int8 *destination, int8 *source, int16 size) {
  int8 *d, *s;
  int16 n;

  for (n = size, d = destination, s = source; n;
       --size, destination++, source++) {
    *d = *s;
  }

  return;
}
