#include "player.h"

#define GUTF 9787
#define TUTF 0040
#define SADGUTF 9785

/*battlefield utf*/
#define TLC 9487
#define TRC 9491
#define BRC 9499
#define BLC 9495
#define HLN 9473
#define VLN 9475
#define TT 9531
#define TB 9523
#define TL 9507
#define TR 9523
#define FUTF 9752



#define DELAY 50
#define NPOLES 10
#define HEIGHTCENTRALPOLE 12
#define SCALE 2
#ifndef PLAYERS

typedef struct  {
  int posX; // horizontal position of player
  int posXold; // old horizontal position of player
  int posY; // vertical position of player
  int posYold; // old vertical position of player
  direction dir; //movement direction
  int switchp;
  int score;
  char name[10];
  utf8code UTF;
  int  marked[2];
 } Player;
#define PLAYERS
#endif

void machine_turn();
void prepare_battle_field();
void init_battlefield();
void delay(int milliseconds);
int get_pole_index(Player p);
int dist(int pole_index);
int inv_dist(int x,char *r);
