#ifndef sockh
#define sockh

char *s;

#include "main.c"
#include "types.h"
#include <pthread.h>
#include <sys/socket.h>
#include <netinet/in.h> // todo, check this for cross-distribution ability lol

void serverlistping(int fd) {
  int length, type;
  char *buf;
  char *buf2;
  char c = 0;
  char *resp = (char *) malloc(500);
  resp[0] = 0;

  sprintf(resp, "{\"version\": {\"name\": \"1.20.2\", \"protocol\": 764}, \"players\": {\"max\": %i, \"online\": %i}, \"description\": \"%s\", \"enforcesSecureChat\": false, \"previewsChat\": false}", 
  100, 0, "The Frea server"); // maxplayers, onlineplayers, motd
  printf("%s\n", resp);
  while (1) { 
    length = readvarintfd(fd);
    type = readvarintfd(fd);
    if (type == 0) {
      logprint(log_info, "asked");
      
      buf = writevarint(strlen(resp)+1);
      write(fd, buf, strlen(buf2)+1); // send packet header.size
      free(buf);
      buf = writevarint(0);
      write(fd, buf, strlen(buf)); // send packet header.type
      free(buf);
      buf = concat(2, writevarint(strlen(resp)), resp);
      write(fd, buf, strlen(buf)); // send str
      free(buf);
      continue;
    }
    if (type == 1) {
      logprint(log_info, "pong");
      buf = (char *) malloc(8);
      read(fd, buf, 8); // it wants us to save and send back the long it gives us

      buf2 = writevarint(9); // header.size
      write(fd, buf2, strlen(buf2));
      free(buf2);
      buf2 = writevarint(1); // header.type
      write(fd, buf2, strlen(buf2));
      free(buf2);
      write(fd, buf, 8); // send the long back

      free(buf);

      // we are done here. nothing else todo
      close(fd);
      return;
    }

    // invalid, we are done here
    close(fd);
    return;
  }
}

// any time a socket is connected this is called
void *client(void *arg) {
  int *fd = (int *) arg;
  int loop;

  int state = 0;
  
  pthread_detach(pthread_self()); 

  int length, type;

  length = readvarintfd(*fd);
  type = readvarintfd(*fd);

  if (type == 0) { // handshake
    int ver = readvarintfd(*fd); 
    free(readstringfd(*fd)); // we dont need host name
    read(*fd, &length, 2); // shove the port somewhere
    int nextstate = readvarintfd(*fd); // get the next state
    if (nextstate == 1) {
      logprint(log_info, "Pinged");
      serverlistping(*fd);
      close(*fd);
      return NULL;
    }
  } else { // bad socket or unsupported version
    close(*fd);
  }
  return NULL;
}

void *server(void *) {
  pthread_detach(pthread_self()); 

  int serverfd;
  struct sockaddr_in address;
  
  s = (char *) malloc(strlen("Minecraft Frea 0.0"));
  strcpy(s,"Minecraft Frea 0.0");

  logprint(log_info, s);

  free(s);

  serverfd = socket(AF_INET, SOCK_STREAM, 0);
  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = htons(25565);

  serverfdsave = &serverfd; // allow the main thread to unblock this thread to shut it down

  if (bind(serverfd, (struct sockaddr*) &address, sizeof(address)) < 0) {
    logprint(log_warn, "Bind failure. Check if port 25565 is free");
    failure = 2;
    return NULL;
  }

  pthread_t clientthread;
  listen(serverfd, 1);
  int *newfd;
  while (1) {
    newfd = (int *) calloc(1, sizeof(int));
    newfd[0] += accept(serverfd, NULL, NULL);
    pthread_create(&clientthread, NULL, &client, (void *) newfd);

    if (failure != 0) {
      close(serverfd);
      break;
    }
  }

  return NULL;
}

#endif