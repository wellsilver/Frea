#ifndef sockh
#define sockh

char *s;

#include "main.c"
#include <pthread.h>
#include <sys/socket.h>
#include <netinet/in.h> // todo, check this for cross-distribution ability lol

void *client(void *arg) {
  //int *fd = (int *) arg;



  pthread_detach(pthread_self()); 

  return NULL;
}

void *server(void *) {
  pthread_detach(pthread_self()); 

  int serverfd;
  struct sockaddr_in address;

  //pthread_t *tmp;
  sleep(5);
  
  s = malloc(strlen("Minecraft Frea 0.0"));
  strcpy(s,"Minecraft Frea 0.0");

  logprint(log_info, s);

  free(s);

  serverfd = socket(AF_INET, SOCK_STREAM, 0);
  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = htons(25565);

  if (bind(serverfd, (struct sockaddr*) &address, sizeof(address)) < 0) {
    logprint(log_warn, "Bind failure. Check if 25565 is free");
    failure = 2;
    return NULL;
  }
  
  while (1) {
    

    if (failure != 0) {
      close(serverfd);
      break;
    }
  }

  return NULL;
}

#endif