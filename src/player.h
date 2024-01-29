#ifndef player_h
#define player_h

typedef struct player {
  int x,y;
  char uuid[17];
  char username[17];
  char op;
} player;

#endif