// linux only

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> 
#include <string.h>
#include "util.h"

// cnsl is a pipe to log shutdowns and prints to main thread
int *cnsl;

enum logtype {
  log_warn=1,
  log_info=2,
};

void logprint(enum logtype type,char *str) {
  switch (type) {
    case log_info:
      str = concat(2,"\e[0;37m[INFO] ", str);
    break;
    case log_warn:
      str = concat(2,"\e[0;31m[WARN] ", str);
    break;
  }
  write(cnsl[1], str, strlen(str));
  free(str);
}

#include "socket.h"
#include <pthread.h>

int main(int argc, char **argv) {
  pthread_t servert;

  cnsl = (int *) malloc(2 * sizeof(int));

  pipe(cnsl); // init cnsl pipe

  pthread_create(&servert,NULL,&server,NULL);
  
  char *buf = (char *) malloc(128);
  while (1) {
    read(cnsl[0],buf,128);
    if (buf[0] != '\r') {
      // for some reason printf isnt formatting anything after %s so haved to use sprintf
      printf("\e[2K\e[0E%s\n> ", buf); // string: clear line (remove "> ") replace with buf, then move cursor down and place new "> "
      fflush(stdout);
    }
  }
}