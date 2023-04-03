#ifndef client_h
#define client_h
#include <sys/socket.h>
#include <sys/select.h>
#include <stdlib.h> // malloc

enum version {
    v_1_19_4 = 762,
};

struct client {
    char thrd; // the connection thread the player is assigned to
    int fd; // socket file descriptor
    char ver;
    double x,y;
};

int clients_size;
struct client *clients;

#endif