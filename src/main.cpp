#ifndef main_cpp
#define main_cpp

int active = 1;

void log(int level, const char *str);

#include <thread>
#include <iostream>
#include <unistd.h>

#include "socket.hpp"

void log(int level, const char *str) {
  printf("%i : %s\n", level, str);
}

int main() {
  socketmain();

  // wait
  while (active) {
    char c;
    read(STDIN_FILENO, &c, 1);
    active=0;
  }
  
}

#endif