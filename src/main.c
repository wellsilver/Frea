#include <stdio.h>
#include <pthread.h>

#include "client.h"
#include "console.h"

char serveractive=1;
char timebetweenjoins=1;


void init() {
    clients_size=1;
    clients = (struct client *) malloc(sizeof(struct client)*1); // players memory

}

void *handshaker(void **args) { // threaded

}

int main(int argc, char **argv) {
    init();
}