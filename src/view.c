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
#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <string.h>
#include <math.h>
#include "view.h"

extern Player tuki_player,giuli_player;
extern int dl1,dl2,dl3;

/*char at  row,col*/
char chat[BFHEIGHT] [BFWIDTH];
/*color at  row,col*/
int clat[BFHEIGHT] [BFWIDTH];

void pole(int d, char side,int l,int bgc);

void display_grass()
{
  for(int i=1;i<BFWIDTH;i++)
    for(int j=1;j<BFHEIGHT;j++)
      {
	chat[j][i]=32;
	clat[j][i]=BGC;
	printf("\x1b[0;%dm\x1b[%d;%dH \x1b[0m",clat[j][i],j,i);
      }
  for(int i=1;i<BFWIDTH;i++)
    {
      printf("\x1b[%d;%dH\x1b[0;%dm\x1b[1;32m~\x1b[0m",BASELINE,i,GGC);
      printf("\x1b[%d;%dH\x1b[0;%dm\x1b[1;32m~\x1b[0m",BASELINE+1,i,GGC);
      chat[BASELINE][i]=' ';
      chat[BASELINE+1][i]=' ';
      clat[BASELINE][i]=GGC;
      clat[BASELINE+1][i]=GGC;
    }
 

}


void mark_pole(int d, char side,int l)
{
  int bgc=105;
  pole(d, side, l, bgc);
  //delay(200);

}
void umark_pole(int d, char side,int l)
{
  int bgc=43;
  pole(d, side, l, bgc);

}
void display_pole(int d, char side,int l)
{
  int bgc=43;
  pole(d, side, l, bgc);
  
}

void blink_pole(int d1, char side1,int l1,int d2, char side2,int l2)
{
  int C1=101,C2=43;
  pole(d1, side1, l1, C1);
  pole(d2, side2, l2, C1);
  delay(dl1);
  pole(d1, side1, l1, C2);
  pole(d2, side2, l2, C2);
  delay(dl2);
  pole(d1, side1, l1, C1);
  pole(d2, side2, l2, C1);
  delay(dl2);
  pole(d1, side1, l1, C2);
  pole(d2, side2, l2, C2);
  delay(dl2);
  pole(d1, side1, l1, C1);
  pole(d2, side2, l2, C1);
  delay(dl3);
}


void pole(int d, char side,int l,int bgc)
{
  int bfwm=XCENTRALPOLE;
  int sgn=1;
  if(side=='L') sgn=-1;
  int x=bfwm+sgn*d;
  /*clear the space for the pole*/
  for(int i=0;i<NPOLES;i++)
    {
      chat[BASELINE-i][x]=32;
      clat[BASELINE-i][x]=BGC;
      printf("\x1b[%d;%dH\x1b[0;%dm \x1b[0m",BASELINE-i,x,BGC);
    }
  for(int i=0;i<l;i++)
    {
      char polec=' ';
      printf("\x1b[%d;%dH\x1b[1;%dm%c\x1b[0m",BASELINE-i,x,bgc,polec);
      chat[BASELINE-i][x]=polec;
      clat[BASELINE-i][x]=bgc;
    }
  if(giuli_player.posX==x)
    {
      set_coordinates(x,BASELINE-l,&giuli_player);
    }
  if(tuki_player.posX==x)
    {
      set_coordinates(x,BASELINE-l,&tuki_player);
    }
  fflush(stdout);
}

void display_central_pole(int l)
{
  char polec=' ';
  int x=XCENTRALPOLE;
  display_pole(0, 'R', l);
  printf("\x1b[%d;%dH\x1b[0;%dm\x1b[1;32m\u2618\x1b[0m",BASELINE-l+1,x,BGC);
  chat[BASELINE-l+1][x]=polec;
  clat[BASELINE-l+1][x]=43;
}


void deletep(Player* p){
  int x=p->posX;
  int y=p->posY;
  //printf("\x1b[%d;%dH\x1b[0;%dm%c\x1b[0m",y,x,clat[y][x],chat[y][x]);
  
  y=p->posYold-1;
  //printf("\x1b[%d;%dH\x1b[0;%dm%c\x1b[0m",y,x,clat[y][x],chat[y][x]);
  //fflush(stdout);
}

void jump(Player * p)
{
  
  /*Monitor coordinate*/
  int xo=p->posXold;
  int yo=p->posYold;
  int x=p->posX;
  int yf=p->posY;
  
  
  /*Physical coordinate*/
  float a=x;
  float ao=xo;
  float bf=BFHEIGHT-yf;
  float bo=BFHEIGHT-yo;
  float b=bo;

  
  float dt=0.005;/*secondo*/
  float t=0;
  float g=9.81;/*m/s^2*/
  float vy;//m/s
  float vb;
  int ax=xo;
  int color=31;
  int my,mb=-1;
  int fcd=0;//Flag to indicate fall direction changed

  /*Delete*/
  deletep(p);
  /*Calculate vy on bf-bo*/
  if((bf-bo)<0) vy=3;// No need of big jump
  else if((bf-bo)==0)
    {
      vy=5;
    }
  else 
    {
      vy=sqrt(2*g*(bf-bo)*1.5);
      
    }
  while(!(ax==a&&(int)b==bf)){
    float _b=b;
    vb=vy-g*(t);
    b=bo+vy*t-0.5*g*(t*t);
    my=(int)(BFHEIGHT-b);
    t+=dt;
    if(b>0 && b<BFHEIGHT)
      {
	if(vb>0)
	  {
	    
	    fflush(stdout);
	    ax=xo;
       	  }
	else
	  {
	    ax=a;
	  }
	set_coordinates(ax, my, p);
	delay(1);
	mb=my;
      }
    
  }
delay(1);
  




}


void display_player(Player p)
{
  int x=p.posX;
  int y=p.posY;
  int xo=p.posXold;
  int yo=p.posYold;
  /*restore the char at the previous player position*/
  printf("\x1b[%d;%dH\x1b[0;%dm%c\x1b[0m",yo,xo,clat[yo][xo],chat[yo][xo]);
  printf("\x1b[%d;%dH\x1b[0;%dm%c\x1b[0m",yo,xo+1,clat[yo][xo+1],chat[yo][xo+1]);
  if(xo>0)
    printf("\x1b[%d;%dH\x1b[0;%dm%c\x1b[0m",yo,xo-1,clat[yo][xo-1],chat[yo][xo-1]);
  
  if(strcmp(p.name,"tuki")==0)
    {
      if(p.dir==1)
	{
	  printf("\x1b[%d;%dH\x1b[1;%dm\x1b[1;30m@\x1b[0m\x1b[1;%dm\x1b[1;33m>\x1b[0m\x1b[0m",y,x,BGC,BGC);
	}
      else
	{
	  printf("\x1b[%d;%dH\x1b[1;%dm\x1b[1;33m<\x1b[0m\x1b[1;%dm\x1b[1;30m@\x1b[0m\x1b[0m",y,x-1,BGC,BGC);

	  if(xo==0)
	    {
	      printf("\x1b[%d;%dH\x1b[0;%dm%c\x1b[0m",yo,xo,clat[yo][xo-1],chat[yo][xo-1]);
	    }
	  else
	    {
	      printf("\x1b[%d;%dH\x1b[0;%dm%c\x1b[0m",yo,xo-1,clat[yo][xo-1],chat[yo][xo-1]);
	    }
	  //printf("\x1b[%d;%dH\x1b[1;33m<\x1b[0m\x1b[1;30m@\x1b[0m",y,x-1);
	}
    }
  if(strcmp(p.name,"giuli")==0)
    {
      /*restore the char at the previous player position*/
      if(p.dir==1)
	printf("\x1b[%d;%dH\x1b[1;%dm\x1b[1;31moò\x1b[0m",y,x,BGC);
      else
	printf("\x1b[%d;%dH\x1b[1;%dm\x1b[1;31mòo\x1b[0m",y,x,BGC);
    }
  fflush(stdout);
  



  
}
