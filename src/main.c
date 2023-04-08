#include <stdio.h>

#include "client.h"
#include "console.h"

char serveractive=1;
char timebetweenjoins=1;

void loadconfig() {

}

int init() {
    loadconfig();
    clients_size=0;
    clients = (struct client *) malloc(sizeof(struct client)*0); // players memory
}

int main(int argc, char **argv) {
    if (!init()) return 1;
}