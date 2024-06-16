// linux only
#ifndef mainh
#define mainh

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> 
#include <string.h>
#include <signal.h>
#include "util.h"

// cnsl is a pipe to log shutdowns and prints to main thread
int *cnsl;
int failure = 0;
int *serverfdsave;

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
  str = concat(2, str, "\n");
  write(cnsl[1], str, strlen(str));
  free(str);
}

#include "commands.h"
#include "socket.h"
#include <pthread.h>

void siginthandle(int dummy) {
  failure = 1;
  logprint(log_info, "Quit");
}

void *stdinhandle(void *) {
  char *buf = (char *) malloc(128);
  int loop;
  while (failure==0) {
    sleep(0);
    for (loop=0;loop<128;loop++) {
      buf[loop] = 0;
    }
    read(STDIN_FILENO, buf, 128);
    write(cnsl[1], "", 1);
    processcmd(0,buf);
  }
  return NULL;
}

int main(int argc, char **argv) {
  pthread_t servert;
  pthread_t stdinhndl;

  signal(SIGINT, siginthandle);

  cnsl = (int *) malloc(2 * sizeof(int));

  pipe(cnsl); // init cnsl pipe

  pthread_create(&servert,NULL,&server,NULL);
  pthread_create(&stdinhndl,NULL,&stdinhandle,NULL);
  
  char *buf = (char *) malloc(128);

  int loop;

  while (failure==0) {
    for (loop=0;loop<128;loop++) {
      buf[loop] = 0;
    }
    read(cnsl[0],buf,128);

    printf("\e[2K\e[0E%s\e[0;37m> ", buf); // string: clear line (remove "> ") replace with buf, then move cursor down and place new "> "
    fflush(stdout);
  }
  close(*serverfdsave); // stop the server thread so there isnt any binding troubles on restart with the same terminal
  return 0;
}

#endif