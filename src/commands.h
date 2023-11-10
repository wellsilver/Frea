#ifndef commandsh
#define commandsh

#include "types.h"
#include "main.c"

void processcmd(char *cmd) {
  int v = readvarlong("\x80\x80\x80\x80\xf8\xff\xff\xff\xff\x01");
  printf("%i\n",v);
}

#endif