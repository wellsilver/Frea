#ifndef sockh
#define sockh

#include "main.c"
#include "types.h"
#include "player.h"

#include <pthread.h>
#include <sys/socket.h>
#include <netinet/in.h> // todo, check this for cross-distribution ability lol
#include <stdio.h>

void client_status(int fd) {
  int length, type;
  char *buf;
  char *buf2;
  char c = 0;
  char *resp = (char *) malloc(500);
  resp[0] = 0;

  sprintf(resp, "{\"version\": {\"name\": \"Frea 1.20.2-1.20.4\", \"protocol\": 764}, \"players\": {\"max\": %i, \"online\": %i}, \"description\": \"%s\", \"enforcesSecureChat\": false, \"previewsChat\": false}", 
  1000, 0, "The Frea server"); // maxplayers, onlineplayers, motd

  while (1) {
    length = readvarintfd(fd);
    type = readvarintfd(fd);
    if (type == 0) {
      buf2 = writevarint(strlen(resp));
      buf = writevarint(strlen(resp)+1+strlen(buf2));
      write(fd, buf, strlen(buf2)+1); // send packet header.size
      free(buf);
      free(buf2);
      buf = writevarint(0);
      write(fd, buf, strlen(buf)); // send packet header.type
      free(buf);
      buf = concat(2, writevarint(strlen(resp)), resp);
      write(fd, buf, strlen(buf)); // send str
      free(buf);
      continue;
    }
    else if (type == 1) {
      uint64_t clientstatusgar = readvarlongfd(fd); // it wants us to save this to send back
      buf2 = writevarint(9); // header.size
      write(fd, buf2, strlen(buf2));
      free(buf2);
      buf2 = writevarint(1); // header.type
      write(fd, buf2, strlen(buf2));
      free(buf2);

      buf2 = writevarlong(clientstatusgar);
      write(fd, buf2, strlen(buf2)); // send the long back
      free(buf2);

      continue;
    }
    else if (type == 0) return;
  }
}

int client_login(int fd, player *pl) { // login state
  int length, type;
  char *buf;
  length = readvarintfd(fd);
  type = readvarintfd(fd);
  if (type==0) {
    buf = readstringfd(fd); // get username
    strcpy(pl->username, buf); // save username
    free(buf);
    recv(fd, pl->uuid, 16, 0); // uuid
  }
  else {
    return -1; // fail
  }
  length = 1+strlen(pl->username)+1+16+1;
  buf = writevarint(length);
  write(fd, buf, strlen(buf)); // send length
  free(buf);
  buf = writevarint(2);
  write(fd, buf, strlen(buf)); // send type
  free(buf);
  write(fd, pl->uuid, 16); // send uuid
  buf = writevarint(strlen(pl->username)); 
  write(fd, buf, strlen(buf)); // username.length
  free(buf);
  write(fd, pl->username, strlen(pl->username)); // username
  // dont free pl->username
  buf = writevarint(0);
  write(fd, buf, strlen(buf));
  free(buf);
}

// any time a socket is connected this is called
void *client(void *arg) {
  int *fd = (int *) arg;
  int loop;

  int state = 0;
  
  //pthread_detach(pthread_self()); 

  int length, type;

  length = readvarintfd(*fd);
  type = readvarintfd(*fd);

  player *pl = (player *) malloc(sizeof(struct player));
  pl->uuid[16] = 0; // null terminator
  pl->username[16] = 0; // null terminator

  if (type == 0) { // handshake
    int ver = readvarintfd(*fd); 
    free(readstringfd(*fd)); // we dont need host name
    read(*fd, &length, 2); // shove the port somewhere
    int nextstate = readvarintfd(*fd); // get the next state
    if (nextstate == 1) {
      client_status(*fd); // handle ping
      goto end;
    }
    if (nextstate == 2) {
      if (client_login(*fd, pl) == -1) goto end; // end on failure
      
    }
  }
  end:

  close(*fd);
  free(pl);
  return NULL;
}

void *server(void *) {
  pthread_detach(pthread_self()); 

  int serverfd;
  struct sockaddr_in address;
  
  char *s = (char *) malloc(19);
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