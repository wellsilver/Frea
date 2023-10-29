#ifndef sockh
#define sockh

char *s;

#include <pthread.h>

void *client(void *arg) {
  //int *fd = (int *) arg;



  pthread_detach(pthread_self()); 

  return NULL;
}

void *server(void *) {
  pthread_detach(pthread_self()); 

  //pthread_t *tmp;
  sleep(5);
  
  s = malloc(strlen("Minecraft Frea 0.0"));
  strcpy(s,"Minecraft Frea 0.0");

  logprint(log_info, s);

  while (1) {
    
  }

  return NULL;
}

#endif