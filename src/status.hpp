#ifndef status_hpp
#define status_hpp

#include "packet.hpp"
#include <stdarg.h>

char* concat(int count, ...) {
  va_list ap;
  int i;

  // Find required length to store merged string
  int len = 1; // room for NULL
  va_start(ap, count);
  for(i=0 ; i<count ; i++)
    len += strlen(va_arg(ap, char*));
  va_end(ap);

  // Allocate memory to concat strings
  char *merged = (char *) calloc(sizeof(char),len);
  int null_pos = 0;

  // Actually concatenate strings
  va_start(ap, count);
  for(i=0 ; i<count ; i++)
  {
    char *s = va_arg(ap, char*);
    strcpy(merged+null_pos, s);
    null_pos += strlen(s);
  }
  va_end(ap);

  return merged;
}


void statushandler(int fd) {
  char *resp = (char *) malloc(500);
  resp[0] = 0;

  sprintf(resp, "{\"version\": {\"name\": \"Frea 1.21\", \"protocol\": 767}, \"players\": {\"max\": %i, \"online\": %i}, \"description\": \"%s\", \"enforcesSecureChat\": false, \"previewsChat\": false}", 
  1000, 0, "The Frea server"); // maxplayers, onlineplayers, motd

  while (1) {
    packet pkt(fd);
    if (pkt.id == 0) {
      packet sending;
      sending.id = 0;
      sending.writestring(std::string(resp));
      sending.sendp(fd);
      continue;
    }
    else if (pkt.id == 1) {
      return;
    }
    else return;
  }
}

#endif