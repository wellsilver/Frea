#ifndef main_cpp
#define main_cpp

int active = 1;

#include <thread>
#include <iostream>
#include <unistd.h>

#include "socket.hpp"

int main(int argc, char **argv) {
  socketmain();
}

#endif