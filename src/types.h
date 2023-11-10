
#ifndef typesh
#define typesh

#include <byteswap.h>
#define segmentbits 0x7F
#define continuebit 0x80

// the more I learn about var ints, the more madder I am.
// first of all, why is it big endian? minecraft was made in 2006-9?
// big endian is, I mean, ok to deal with
// but you could just send the size of the integers instead? It would waste a byte but these things are wasting 9 so

// read a varint (varint or varlong)
int64_t readvarint(char *v) {
  int value = 0;
  int position = 0;
  char currentByte;
  int d = 0;

  while (1) {
    currentByte = v[d];
    d++;
    value |= (currentByte & segmentbits) << position;

    if ((currentByte & continuebit) == 0) break;

    position += 7;
  }
  return value;
}

int64_t readvarlong(char *v) {
  int value = 0;
  int position = 0;
  char currentByte;
  int d = 0;

  while (1) {
    currentByte = v[d];
    d++;
    value |= (long) (currentByte & segmentbits) << position;

    if ((currentByte & continuebit) == 0) break;

    position += 7;
  }
  return value;
}

char *writevarshort(int v) {
  return "";
}

char *writevarlong(int v) {
  return "";
}

#endif