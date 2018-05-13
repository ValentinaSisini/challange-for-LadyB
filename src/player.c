/** This file is part of GandT-challanegforLadyB.
 *  Copyright (C) 2018 Francesco Sisini 
 *
 *  GandT-challanegforLadyB is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation version 3.
 *
 *  GandT-challanegforLadyB is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with Foobar.  If not, see <http://www.gnu.org/licenses/>.
 *
 *
 * Homepage: www.isisinipazzi.it/codinggames
 * mail to:  francescomichelesisini@gmail.com
 */

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "view.h"
#include "player.h"
#include "machine.h"
extern int g_poles[NPOLES];
extern int t_poles[NPOLES];
extern Player tuki_player;
extern Player giuli_player;

extern int t_or_g_1_or_2;
extern char g_marked[2];
extern char t_marked[2];
extern int jdelay;
extern int game;

void set_coordinates(int x, int y, Player *p){
  static int init=0;
  init++;
  if(init>1){
    init=1;
    p->posXold=p->posX;
    p->posYold=p->posY;
  }
  else{
     p->posXold=x;
     p->posYold=y;
  }
  p->posX=x;
  p->posY=y;
  display_player(*p);
}

/**
 *Switch the last two marked poles
 */
void switchpole()
{
  if(t_or_g_1_or_2==2)
    {
      giuli_player.switchp=1;
     
    }
  else
    {
      tuki_player.switchp=1;
    }
}

void _switchpole()
{
  
  fflush(stdout);
  Player* p=&tuki_player;
  int *pole=t_poles;
  char side='L';

  if(t_or_g_1_or_2==2)
    {
      giuli_player.switchp=0;
     
    }
  else
    {
      tuki_player.switchp=0;
    }
  
  if(t_or_g_1_or_2==2)
    {
      
      p=&giuli_player;
      side='R';
      pole=g_poles;
     
    }
  /*Not possible to switch poles if just one is marked*/
  if(p->marked[0]==-1||p->marked[1]==-1) return;

  blink_pole(dist(p->marked[1]),side,pole[p->marked[1]],dist(p->marked[0]),side,pole[p->marked[0]]);
 
  int tmp=pole[p->marked[0]];
  pole[p->marked[0]]=pole[p->marked[1]];
  pole[p->marked[1]]=tmp;

  display_pole(dist(p->marked[1]),side,pole[p->marked[1]]);
  display_pole(dist(p->marked[0]),side,pole[p->marked[0]]);
  umark_pole(dist(p->marked[1]),side,pole[p->marked[1]]);
  umark_pole(dist(p->marked[0]),side,pole[p->marked[0]]);
  
  p->marked[0]=-1;
  p->marked[1]=-1;
}

int pole_height(){
   Player* p=&tuki_player;
  int *pole=t_poles;
  char side='L';
  if(t_or_g_1_or_2==2)
    {
      p=&giuli_player;
      side='R';
      pole=g_poles;
    }
  int i=get_pole_index(*p);
  if(i>=0)
    {
      return pole[i];
    }
  return -1;
  
}

int umark(){
  Player* p=&tuki_player;
  if(t_or_g_1_or_2==2)
    {
      p=&giuli_player;
    }
  p->marked[0]=-1;
  p->marked[1]=-1;
  return -1;
}

int mark()
{
  Player* p=&tuki_player;
  int *pole=t_poles;
  char side='L';
  if(t_or_g_1_or_2==2)
    {
      p=&giuli_player;
      side='R';
      pole=g_poles;
    }
  int i=get_pole_index(*p);
  if(i>=0)//&&i!=p->marked[0])
    {
      /*put in FIFO*/
      if(p->marked[1]>=0)
	{
	  umark_pole(dist(p->marked[1]),side,pole[p->marked[1]]);
	}
      p->marked[1]=p->marked[0];
      p->marked[0]=i;
      mark_pole(dist(p->marked[0]),side,pole[p->marked[0]]);
      
      
      if(p->marked[1]>=0){
	mark_pole(dist(p->marked[1]),side,pole[p->marked[1]]);
	fflush(stdout);

      }
    }
  //display_player(*p);
  return i;
}

/**
 *handle the player victory
 */
void tuki_win(){
  game=0;
  char* strw="****TUKI HAS WON!!!****";
  int y=5;
  int x=(BFWIDTH-strlen(strw))/2;
    printf("\x1b[%d;%dH%s",y,x,strw);
  printf("\x1b[%d;%dH",BFHEIGHT+2,1);
  delay(3000);
}


/**
 *handle the player victory
 */
void giuli_win(){
  game=0;
  char* strw="****GIULII HAS WON!!!****";
  int y=5;
  int x=(BFWIDTH-strlen(strw))/2;
  printf("\x1b[%d;%dH%s",y,x,strw);
  printf("\x1b[%d;%dH",BFHEIGHT+2,1);
  delay(3000);
}


int sorted(int array[]){
  for(int i=0;i<NPOLES-1;i++)
    {
      if(array[i]<array[i+1]) return 0;
    }
  return 1;
}

void check_winner()
{
  if(sorted(t_poles)) tuki_win();
  if(sorted(g_poles)) giuli_win();
  
}


/**
 *handles player victory
 */
void player_win(){

   if( t_or_g_1_or_2==1)
     {

       tuki_win();
     }else
     {

       giuli_win();
     }
}

void hit_wall()
{

}


/**
 * Makes the player rotate in the 'd' drection
 */
void player_rotate(direction d)
{
  
  /*choose the player*/
  Player* player;//the actual player
  if(t_or_g_1_or_2==1){
   player=&tuki_player;
  }
  else{
    player=&giuli_player;
  }
  
  switch(d)
    {

    case RIGHT:
      {
	player->dir=RIGHT;
	break;
      }
    case LEFT:
      {
	player->dir=LEFT;
	break;
      }
    case IN:
      {
	player->dir=IN;
	break;
      }
    }
}


/**
 * Makes a step in the direction of the player
 */
int step(){
  
  int *poles;
  /*left and right limit for the player*/
  int ll,rl;
  /*choose the player*/
  Player * player;//the actual player
  int a_pole,n_pole;
  
  if(t_or_g_1_or_2==1)
    {
      player=&tuki_player;
      ll=1;
      rl=XCENTRALPOLE;
      poles=t_poles;
    }
  else
    {
      player=&giuli_player;
      rl=BFWIDTH-1;
      ll=XCENTRALPOLE;
      poles=g_poles;
  } 
  
  /*the pole index where the player is*/
  a_pole=get_pole_index(*player);
  n_pole=a_pole;
  
  direction d=player->dir;
  int x_old=player->posX;
  int y_old=player->posY;
  int y=y_old;
  
  if(d==IN)
    if(t_or_g_1_or_2==1)
      {
	set_coordinates(player->posX,y,player);
	if(player->switchp==1)
	  {
	    jump(player);
	    delay(jdelay);
	    _switchpole();
	    delay(jdelay);
	  }
	machine_turn();
      }
  
  /*identify the poles*/

  /***Assign y coordinates***/

  
  
  if(t_or_g_1_or_2==1)
    {
      if(d==RIGHT && x_old==XCENTRALPOLE)d=LEFT;
      if(x_old<=(XCENTRALPOLE-(NPOLES+1)*SCALE))
	{
	  a_pole=NPOLES;
	  d=RIGHT;
	}
      if(d==LEFT)
	{
	  if(a_pole<(NPOLES-1))n_pole=a_pole+1;
	  else n_pole=-1;
	  if(n_pole==-1) y=BFHEIGHT+1;
	  else y=BFHEIGHT-(poles[n_pole]-1);  
	}
      if(d==RIGHT)
	{
	  if(a_pole>0)n_pole=a_pole-1;
	  else n_pole=-1;
	  if(n_pole==-1)y=BFHEIGHT-HEIGHTCENTRALPOLE;
	  else y=BFHEIGHT-(poles[n_pole]-1);  
	}
    }
  if(t_or_g_1_or_2==2)
    {
      if(d==LEFT && x_old==XCENTRALPOLE)d=RIGHT;
      
      if(d==RIGHT && x_old==(XCENTRALPOLE+(NPOLES+1)*SCALE))d=LEFT;
      if(d==RIGHT)
	{
	  if(a_pole<(NPOLES-1))n_pole=a_pole+1;
	  else n_pole=-1;
	  if(n_pole==-1) y=BFHEIGHT-1;
	  else y=BFHEIGHT-(poles[n_pole]-1);  
	}
      if(d==LEFT)
	{
	  if(a_pole>0)n_pole=a_pole-1;
	  else n_pole=-1;
	  if(n_pole==-1)y=BFHEIGHT-HEIGHTCENTRALPOLE;
	  else y=BFHEIGHT-(poles[n_pole]-1);  
	}
    }
    

  
  /***Assign x coordiates***/
  if(d==LEFT)
    if(x_old>(ll))
      {
	set_coordinates(player->posX-SCALE,y,player);
      }
  if(d==RIGHT)
    if(x_old<(rl))
      {
	set_coordinates(player->posX+SCALE,y,player);
      }
  
  
  jump(player);
  
  if(player->switchp==1)
    {
      delay(jdelay);
      _switchpole();
      delay(jdelay);
    }
  /*check for who complete the race*/
  check_winner();
  if(game==0) return 1;
  
  /*Here is handled the cooperative parallelism tuki/giuli */
    if(t_or_g_1_or_2==1)
      {
	machine_turn();
      }
    return 1;
}




