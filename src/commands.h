#ifndef commandsh
#define commandsh

#include "types.h"
#include "main.c"

void processcmd(char *cmd) {
  if (strcmp(cmd, "stop")==0) {
    failure = 1;
    return;
  }
}

#endif