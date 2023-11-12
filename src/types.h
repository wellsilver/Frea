
#ifndef typesh
#define typesh

#include <byteswap.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include "util.h"

#define segmentbits 0x7F
#define continuebit 0x80

// the more I learn about var ints, the more madder I am.
// first of all, why is it big endian? minecraft was made in 2006-9?
// big endian is, I mean, ok to deal with
// but you could just send the size of the integers instead? It would waste a byte but these things are wasting 9 so

// read a varint
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

// read a varlong
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

// read a varint from socket fd
int64_t readvarintfd(int fd) {
  int value = 0;
  int position = 0;
  char currentByte;
  int d = 0;

  while (1) {
    read(fd, &currentByte, 1);

    d++;
    value |= (currentByte & segmentbits) << position;

    if ((currentByte & continuebit) == 0) break;

    position += 7;
  }
  return value;
}

// read a varlong from socket fd
int64_t readvarlongfd(int fd) {
  int value = 0;
  int position = 0;
  char currentByte;
  int d = 0;

  while (1) {
    read(fd, &currentByte, 1);
    d++;
    value |= (long) (currentByte & segmentbits) << position;

    if ((currentByte & continuebit) == 0) break;

    position += 7;
  }
  return value;
}

char *writevarint(int v) {
  int csize = 1;
  char *c = (char *) malloc(1);
  c[0] = 0;

  while (1) {
    if ((v & ~segmentbits) == 0) {
      csize++;
      c = (char *) realloc(c, csize);
      c[csize-2] = v;
      c[csize-1] = 0;
      return c;
    }
    csize++;
    c = (char *) realloc(c, csize);
    c[csize-2] = (v & segmentbits) | continuebit;
    c[csize-1] = 0;

    // Note: >>> means that the sign bit is shifted with the rest of the number rather than being left alone
    v >>= 7;
  }
}

char *writevarlong(int v) {
  int csize = 1;
  char *c = (char *) malloc(1);
  c[0] = 0;

  while (1) {
    if ((v & ~((long) segmentbits)) == 0) {
      csize++;
      c = (char *) realloc(c, csize);
      c[csize-2] = v;
      c[csize-1] = 0;
      return c;
    }
    csize++;
    c = (char *) realloc(c, csize);
    c[csize-2] = (v & segmentbits) | continuebit;
    c[csize-1] = 0;

    // Note: >>> means that the sign bit is shifted with the rest of the number rather than being left alone
    v >>= 7;
  }
}

uint64_t writeposition(long x,long y,long z) {
  return ((x & 0x3FFFFFF) << 38) | ((z & 0x3FFFFFF) << 12) | (y & 0xFFF);
}

// get position x pos=uint64_t
#define rposx(pos) (pos >> 38)
// get position y pos=uint64_t
#define rposy(pos) (pos << 52 >> 52)
// get position z pos=uint64_t
#define rposz(pos) (pos << 26 >> 38)


char *readstringfd(int fd) {
  int length = readvarintfd(fd);
  char *str = malloc(length+1);
  str[length] = 0; // null terminator
  read(fd, str, length);
  return str;
}

#endif