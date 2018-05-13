#include "player.h"
#include "machine.h"
#define BFWIDTH NPOLES*2+1+30
#define BFHEIGHT NPOLES+2+2+1+5
#define BASELINE BFHEIGHT
#define XCENTRALPOLE (BFWIDTH)/2
#define BGC 44
#define GGC 102

enum chartype {WALL,ITEM,OBSTACLE,DOOR,FOOD,FLOWER,UNKNOWN,GIULI,TUKI,EMPTY};


/**
 *From player.c
 */
void set_coordinates(int x, int y, Player *p);

/**
 *Delete the previous pole and prints a new one  
 */
void display_pole(int distance, char side_L_R,int length);


/**
 *Makes the marked poles blink before switching
 */
void blink_pole(int d1, char side1,int l1,int d2, char side2,int l2);
/**
 *Displaies the player
 */
void display_player(Player p);
void deletep(Player* p);
void jump(Player* p);

/**
 *Displaies the grass
 */
void display_grass();

void display_central_pole(int l);
void mark_pole(int d, char side,int l);
void umark_pole(int d, char side,int l);
