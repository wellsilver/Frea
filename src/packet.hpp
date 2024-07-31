#ifndef packet_hpp
#define packet_hpp

#include <sys/socket.h>
#include <netinet/in.h>

#include <inttypes.h>
#include <string>
#include <stdlib.h>
#include <cstring>

#define segmentbits 0x7F
#define continuebit 0x80

// read a varint from socket fd
int64_t readvarintfd(int fd, int *dist) {
  int value = 0;
  int position = 0;
  char currentByte;
  int d = 0;
  int readerr;

  while (1) {
    readerr = read(fd, &currentByte, 1);
    if (readerr < 0) throw std::runtime_error("failed to read");

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
    if (read(fd, &currentByte, 1) != 0) throw std::runtime_error("failed to read");
    d++;
    value |= (long) (currentByte & segmentbits) << position;

    if ((currentByte & continuebit) == 0) break;

    position += 7;
  }
  *dist += d;
  return value;
}

// read a varint
int64_t readvarintt(char *v) {
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
int64_t readvarlongt(char *v) {
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

char *writevarintt(int v) {
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

char *writevarlongt(int v) {
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
    length = 0;
    id = 0;
  }
  // read a packet
  packet(int fd) {
    int dist=0;
    unsigned int dataleft;
    try {
      length = readvarintfd(fd, &dist);
      dataleft = length-dist;
      id = readvarintfd(fd, &dist);
    } catch (std::runtime_error err) {
      badpacket = true;
      return;
    }

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
    data.erase(data.begin(), data.begin()+2);
    return htons(*(uint16_t *) convertable);
  }

  void writestring(std::string str) {
    char *length_ = writevarintt(str.size());
    std::string strlength(length_);
    data += strlength;
    data += str;
    free(length_);
    length += data.length();
  }

  void sendp(int fd) { // this is so weird because id could be 0 which as a varint would be 0.
    char *buf = writevarintt(data.size()+1);
    write(fd, buf, strlen(buf));
    char *id_ = writevarintt(id);
    write(fd, id_, 1);
    write(fd, data.c_str(), data.length());
    free(buf);
    free(id_);
  }
};

#endif