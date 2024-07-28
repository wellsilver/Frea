#ifndef main_cpp
#define main_cpp

int active = 1;

#include <thread>
#include <iostream>
#include <unistd.h>

#include "socket.hpp"

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