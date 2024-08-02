#ifndef main_cpp
#define main_cpp

int active = 1;

#include <thread>
#include <iostream>
#include <unistd.h>
#include <signal.h>

#include "socket.hpp"

void intHandler(int dummy) {
  printf("Stopping\n");
  active = 0;
}

int main(int argc, char **argv) {
  signal(SIGINT, intHandler);

  pthread_t socketthread;
  pthread_create(&socketthread, NULL, socketmain, NULL);

  while (active) {
    sleep(1);
  }
}

#endif