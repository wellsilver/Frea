#ifndef status_hpp
#define status_hpp

#include "packet.hpp"

void statushandler(int fd) {
  // entertain the client
  printf("statushandler has fd\n");
  while (1) {
    packet pckt(fd);
    if (pckt.badpacket) break;
    if (pckt.id == 0) { // Status Request
      printf("recieved status request\n");
    }
    if (pckt.id == 1) { // Ping Request
      printf("recieved ping request\n");
    }
    break;
  }
}

#endif