#ifndef status_hpp
#define status_hpp

#include "packet.hpp"

void statushandler(int fd) {
  std::string resp = "{\"version\": {\"name\": \"Frea 1.21\", \"protocol\": 767}, \"players\": {\"max\": 100, \"online\": 0}, \"description\": \"The Frea Server\", \"enforcesSecureChat\": false, \"previewsChat\": false}";

  while (1) {
    packet pkt(fd);
    if (pkt.badpacket) return;
    else if (pkt.id == 0) {
      packet sending;
      sending.id = 0;
      sending.writestring(resp);
      sending.sendp(fd);
    }
    else if (pkt.id == 1) {
      pkt.sendp(fd); // supposed to send the exact same thing back
    }
  }
}

#endif