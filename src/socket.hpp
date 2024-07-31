#ifndef socket_hpp
#define socket_hpp

#include "status.hpp"

#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <arpa/inet.h>

void *handshake(void *fd_ptr) {
  int fd = *((int *) fd_ptr);
  delete (int *) fd_ptr;

  struct timeval timeout;
  timeout.tv_sec = 1;
  timeout.tv_usec = 0;

  if (setsockopt (fd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof timeout) < 0)
    printf("setsockopt failed\n");

  if (setsockopt (fd, SOL_SOCKET, SO_SNDTIMEO, &timeout, sizeof timeout) < 0)
    printf("setsockopt failed\n");

  printf("Incoming connection\n");
  packet handshake(fd);
  if (handshake.id == 0 && !handshake.badpacket) {
    printf("ver: %i\n", handshake.getvarint()); // this is in the way of the state. should be sanity checked later
    printf("host: %s\n", handshake.getstring().c_str()); // this is in the way of the state. should be sanity checked later
    printf("port: %i\n", handshake.getshort()); // this is in the way of the state. should be sanity checked later
    uint32_t req = handshake.getvarint();
    printf("next: %i\n", req);
    if (req==1) statushandler(fd);
  } else {
    printf("bad incoming %i\n", handshake.id);
  }

  printf("Connection done\n");

  close(fd);
  return NULL;
}


void socketmain() {
  int fd, err;

  fd = socket(AF_INET, SOCK_STREAM, 0);
  if (fd == 0) {
    printf("Bad Socket");
    active = 0;
  }
  
  struct sockaddr_in serv_addr;
  serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(25565);

  err = bind(fd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
  if (err != 0) {
    printf("Cant Bind!");
    active = 0;
  }

  err = listen(fd, 1);
  if (err != 0) {
    printf("Cant listen!");
    active = 0;
  }

  if (active) printf("Created socket successfully\n");

  pthread_t ptid;

  struct timeval timeout;
  timeout.tv_sec = 1;
  timeout.tv_usec = 0;

  if (setsockopt (fd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof timeout) < 0)
    printf("setsockopt failed\n");

  if (setsockopt (fd, SOL_SOCKET, SO_SNDTIMEO, &timeout, sizeof timeout) < 0)
    printf("setsockopt failed\n");

  while (active) {
    int *newcon = new int;
    *newcon = accept(fd, NULL, NULL);
    if (*newcon <= 0) {delete newcon;continue;}
    pthread_create(&ptid, NULL, &handshake, newcon);
    pthread_detach(ptid);
  }

  close(fd);
}

#endif