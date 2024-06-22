#ifndef main_cpp
#define main_cpp

#include <thread>

#include "status.hpp"

int main() {
  std::thread status(statushandler);

  status.join();
}

#endif