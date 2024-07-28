#ifndef packet_hpp
#define packet_hpp

#include <sys/socket.h>
#include <netinet/in.h>

#include <inttypes.h>
#include <string>
#include <stdlib.h>

#define segmentbits 0x7F
#define continuebit 0x80

// read a varint from socket fd
int64_t readvarintfd(int fd, int *dist) {
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
  *dist += d;
  return value;
}

// read a varlong from socket fd
int64_t readvarlongfd(int fd, int *dist) {
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
  *dist += d;
  return value;
}

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

class packet {
public:
  bool badpacket=false;
  unsigned int length;
  unsigned int id;
  std::string data;

  // create a packet
  packet() {
    
  }
  // read a packet
  packet(int fd) {
    int dist=0;
    length = readvarintfd(fd, &dist);
    id = readvarintfd(fd, &dist);

    unsigned int dataleft = length-dist;

    data.resize(dataleft);
    int err = read(fd, (void *) data.c_str(), dataleft);
    if (err < 0) badpacket=true;
  }

  int32_t getvarint() {
    int value = 0;
    int position = 0;
    char currentByte;
    int d = 0;

    while (1) {
      currentByte = data[d];

      d++;
      value |= (currentByte & segmentbits) << position;

      if ((currentByte & continuebit) == 0) break;

      position += 7;
    }
    data.erase(data.begin(), data.begin()+d);
    return value;
  }

  std::string getstring() {
    int length = getvarint();
    std::string ret = data.substr(0, length);
    data.erase(data.begin(), data.begin()+length);
    return ret;
  }

  uint16_t getshort() {
    char convertable[2];
    convertable[0] = data[0];
    convertable[1] = data[1];
    return htons(*(uint16_t *) convertable);
  }
};

#endif