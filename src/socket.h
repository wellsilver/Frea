#ifndef sockh
#define sockh

char *s;

#include "main.c"
#include "types.h"
#include <pthread.h>
#include <sys/socket.h>
#include <netinet/in.h> // todo, check this for cross-distribution ability lol

// any time a socket is connected this is called
void *client(void *arg) {
  int fd = *(int *) arg;
  int loop;
  free(arg);

  pthread_detach(pthread_self()); 
  char *buf = (char *) malloc(128);
  while (1) {
    for (loop=0;loop<128;loop++) {
      buf[loop] = 0;
    }
    read(fd, buf, 128);
  }

  return NULL;
}

void *server(void *) {
  pthread_detach(pthread_self()); 

  int serverfd;
  struct sockaddr_in address;
  
  s = (char *) malloc(strlen("Minecraft Frea 0.0"));
  strcpy(s,"Minecraft Frea 0.0");

  logprint(log_info, s);

  free(s);

  serverfd = socket(AF_INET, SOCK_STREAM, 0);
  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = htons(25565);

  if (bind(serverfd, (struct sockaddr*) &address, sizeof(address)) < 0) {
    logprint(log_warn, "Bind failure. Check if port 25565 is free");
    failure = 2;
    return NULL;
  }
  pthread_t clientthread;
  listen(serverfd, 1);
  while (1) {
    pthread_create(&clientthread,NULL,&client,(void *) (uint64_t) accept(serverfd, NULL, NULL));

    if (failure != 0) {
      close(serverfd);
      break;
    }
  }

  return NULL;
}

#endif