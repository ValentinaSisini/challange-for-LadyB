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
#include <stdlib.h>
#include <time.h>
#include <stdio.h> 
#include <string.h>
#include "machine.h"
#include "view.h"


extern int stdin_copy;
extern Player giuli_player;
extern Player tuki_player;
extern int g_poles [NPOLES];
extern int t_poles [NPOLES];
extern int t_or_g_1_or_2;
extern FILE* debug;
extern int game;


void giuli_turn();
void tuki_turn();
void (*challenger)(void);



int inv_dist(int x,char *r){
  int sgn=1;
  if(x<XCENTRALPOLE)sgn=-1;
  int dist=(x-XCENTRALPOLE)/SCALE*sgn-1;

  *r=(x-XCENTRALPOLE)%SCALE;
  if(dist<0)
    {
      *r=1;
    }
  
  return dist;
}
int dist(int pole_index){
  return (pole_index+1)*SCALE;
  
}


int get_pole_index(Player p){
  char r=0;
  int inx;
  int i=inv_dist(p.posX,&r);
  
  if(i>(NPOLES-1)||r!=0)
    {
      //out of poles
      inx=-1;
    }
  else
    {
      inx=i;
    }
  return inx;
}


/**
 *Prepares the logical battlefield and shows it by view.c 
 */
void init_battlefield()
{
  display_grass();
  for (int i=0;i<NPOLES;i++)
  {
    display_pole(dist(i),'L',t_poles[i]);
    display_pole(dist(i),'R',g_poles[i]);
  }
  display_central_pole(HEIGHTCENTRALPOLE);
 
  /*Players positions*/
  tuki_player.posX=XCENTRALPOLE-NPOLES*SCALE;
  giuli_player.posX=XCENTRALPOLE+NPOLES*SCALE;
  giuli_player.posY=BFHEIGHT-g_poles[9];
  tuki_player.posY=BFHEIGHT-t_poles[9];
  display_player(tuki_player);
  display_player(giuli_player);

}


/**
 * Assign the initial order of the poles
 * and display to the scrren
 */
void prepare_battle_field()
{
  /* create the ordered arrays*/
  for (int i=0;i<NPOLES;i++)
  {
    g_poles[i]=i+1;
    t_poles[i]=i+1;
  }
  /*make a mess*/
  srand(time(NULL));
  for (int i=0;i<50;i++)
    {
      for (int j=0;j<9;j++)
	{
	  if(rand()%2){
	    int t=g_poles[j];
	    g_poles[j]=g_poles[j+1];
	    g_poles[j+1]=t;
	  }
	}
      for (int j=0;j<9;j++)
	{
	  if(rand()%2){
	    int t=t_poles[j];
	    t_poles[j]=t_poles[j+1];
	    t_poles[j+1]=t;
	  }
	}
      
    }
  
}



/**
 * Implements games's logic.
 */
void machine_turn(){
  t_or_g_1_or_2=2;
  giuli_turn();    
  if(game==0) return;
  t_or_g_1_or_2=1;
  (*challenger)(); 
}
/**
 * Creates a delay
 */  
void delay(int milliseconds){
  long pause;
  clock_t now,then;
  
  pause = milliseconds*(CLOCKS_PER_SEC/1000);
  now = then = clock();
  while( (now-then) < pause )
    now = clock();
}


