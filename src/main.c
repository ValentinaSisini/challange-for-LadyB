/** This file is part of challengexladyb.
 *  Copyright (C) 2018 Francesco Sisini 
 *
 *  challengexladyb is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation version 3.
 *
 *  challengexladyb is distributed in the hope that it will be useful,
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

/*
 * I want to thanks jeremy for his tutorial about kilo.c text editor
 * https://viewsourcecode.org/
 * The Terminal setting code is taken from  Salvatore Sanfilippo <antirez at gmail dot com> code
 * https://github.com/snaptokenof Kilo.c
 * with the following disclaimer:
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE
*/

/*** includes ***/
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <termios.h>
#include <string.h>
#include "machine.h"
#include "view.h"
#include "player.h"


#define MAJOR    0
#define MINOR    1
#define RELEASE  1

/**
 * TERMINAL SECTION
 */

/*** defines ***/
#define CTRL_KEY(k) ((k) & 0x1f)

/*** data ***/
struct termios orig_termios;

struct editorConfig {
  int cx, cy;
  int screenrows;
  int screencols;
  struct termios orig_termios;
};
struct editorConfig E;

/***termimal***/
void die(const char *s) {
  write(STDOUT_FILENO, "\x1b[2J", 4);
  write(STDOUT_FILENO, "\x1b[H", 3);

  perror(s);
  exit(1);
}

int get_cursor_position(int *rows, int *cols) {
  char buf[32];
  unsigned int i = 0;
  if (write(STDOUT_FILENO, "\x1b[6n", 4) != 4) return -1;
  while (i < sizeof(buf) - 1) {
    if (read(STDIN_FILENO, &buf[i], 1) != 1) break;
    if (buf[i] == 'R') break;
    i++;
  }
  buf[i] = '\0';
  if (buf[0] != '\x1b' || buf[1] != '[') return -1;
  if (sscanf(&buf[2], "%d;%d", rows, cols) != 2) return -1;
  return 0;
}


int getWindowSize(int *rows, int *cols) {
  struct winsize ws;

  if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == -1 || ws.ws_col == 0) {
    if (write(STDOUT_FILENO, "\x1b[999C\x1b[999B", 12) != 12) return -1;
    return get_cursor_position(rows, cols);
  } else {
    *cols = ws.ws_col;
    *rows = ws.ws_row;
    return 0;
  }
}

char editorReadKey() {
  int nread;
  char c;
  while ((nread = read(STDIN_FILENO, &c, 1)) != 1) {
    if (nread == -1 && errno != EAGAIN) die("read");
  }
  return c;
}

void disableRawMode() {
  if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &E.orig_termios) == -1)
    die("tcsetattr");
}
void enableRawMode() {

  if (tcgetattr(STDIN_FILENO, &E.orig_termios) == -1) die("tcgetattr");
  atexit(disableRawMode);

  struct termios raw = E.orig_termios;
  raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
  raw.c_oflag &= ~(OPOST);
  raw.c_cflag |= (CS8);
  raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
  raw.c_cc[VMIN] = 0;
  raw.c_cc[VTIME] = 5;
  if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1) die("tcsetattr");

}

/*** input ***/
int editorProcessKeypress() {
  char c = editorReadKey();
  switch (c) {
    case CTRL_KEY('q'):
      
      write(STDOUT_FILENO, "\x1b[?25l", 6);
      write(STDOUT_FILENO, "\x1b[2J", 4);
      write(STDOUT_FILENO, "\x1b[H", 3);
      exit(0);
      break;
  case 'p':
    return 1;
  case 'a':
    return 2;
  }
}

/*** init ***/

void initEditor() {
  if (getWindowSize(&E.screenrows, &E.screencols) == -1) die("getWindowSize");
  if(E.screenrows<BFHEIGHT||E.screencols<BFWIDTH)
    {
      char str[100];
      sprintf(str,"WARNING!\nThe terminal monitor needs to be at least %d rows and %d cols\n Resize it and launch the game again, please",BFHEIGHT,BFWIDTH);
      die(str);
    }
  

}

/**
 * GAME SECTION
 */


#define MARGIN(str) (BFWIDTH-strlen(str))/2.

/*the poles array*/
int g_poles [NPOLES];
int t_poles [NPOLES];

/*The game player*/
Player giuli_player, tuki_player, ladyb;

/*Game status*/
int t_or_g_1_or_2;
int game=1;
int jdelay=200;
int dl1=50,dl2=100,dl3=200;

/*the challenger*/
void (*challenger)(void);
void tuki_turn();
void inter_turn();

/*
 * evaluate command line parameters 
 */
static void evaluateCommandLine(int argc, char **argv);


void show_game_option()
{
  
  char wc[50];
  strcpy(wc,"Giuly and Tuki: Challenge for Lady Beetle");
  char opt1[30];
  strcpy(opt1,"Press <p> to interactive play");
  char opt2[30];
  strcpy(opt2,"Press <a> to play your algorithm");
  char opt3[30];
  strcpy(opt3,"<ctrl-q> quit");
  printf("\x1b[?25l");
  printf("\x1b[4;%dH\x1b[1m\x1b[%d;32m%s\x1b[0m",(int)(MARGIN(wc)),BGC,wc);
  printf("\x1b[5;%dH\x1b[%d;30m%s\x1b[0m",(int)(MARGIN(opt1)),BGC,opt1);
  printf("\x1b[6;%dH\x1b[%d;30m%s\x1b[0m",(int)(MARGIN(opt2)),BGC,opt2);
  printf("\x1b[7;%dH\x1b[%d;30m%s\x1b[0m",(int)(MARGIN(opt3)),BGC,opt3);
  fflush(stdout);
}

void clear_message(){
  for(int i=1;i<=8;i++)
    for(int j=1;j<BFWIDTH;j++)
      printf("\x1b[%dm\x1b[%d;%dH ",BGC,i,j);
  fflush(stdout);
}



void show_presentation(){
  write(STDOUT_FILENO, "\x1b[2J", 4);
  write(STDOUT_FILENO, "\x1b[H", 3);
  for(int i=1;i<BFHEIGHT;i++)
    for(int j=1;j<BFWIDTH;j++)
      printf("\x1b[%dm\x1b[%d;%dH ",BGC,i,j);
  fflush(stdout);
  delay(1000);
  char i1[40];
  strcpy(i1,"-Lo Studio Giallo- presents:");
  char i2[30];
  strcpy(i2,"Quest for LadyB");
  char i3[40];
  strcpy(i3,"coding and interactive Terminal game ");
  char i4[30];
  strcpy(i4,"Copyleft @ScuolaSisini");
  char i5[50];
  strcpy(i5,"find more games on iSisiniPazzi.it/codinggames");
  printf("\x1b[4;%dH\x1b[1m\x1b[%d;33m%s\x1b[0m",(int)(MARGIN(i1)),BGC,i1);fflush(stdout);
  delay(200);
  printf("\x1b[5;%dH\x1b[%d;30m%s\x1b[0m",(int)(MARGIN(i2)),BGC,i2);
  fflush(stdout);
  delay(200);
  printf("\x1b[6;%dH\x1b[%d;30m%s\x1b[0m",(int)(MARGIN(i3)),BGC,i3);
  fflush(stdout);
  delay(200);
  printf("\x1b[7;%dH\x1b[%d;30m%s\x1b[0m",(int)(MARGIN(i4)),BGC,i4);
  fflush(stdout);
  delay(200);
  printf("\x1b[8;%dH\x1b[%d;30m%s\x1b[0m",(int)(MARGIN(i5)),BGC,i5);
  fflush(stdout);
  delay(3000);

}

void show_intercative_instruction()
{
  
  char i1[30];
  strcpy(i1,"Swap the poles to sort them");
  char i2[30];
  strcpy(i2,"<l> right");
  char i3[30];
  strcpy(i3,"<j> left");
  char i4[30];
  strcpy(i4,"<i> mark pole");
  char i5[30];
  strcpy(i5,"<space> swap poles");
  printf("\x1b[4;%dH\x1b[1m\x1b[%d;32m%s\x1b[0m",(int)(MARGIN(i1)),BGC,i1);
  printf("\x1b[5;%dH\x1b[%d;30m%s\x1b[0m",(int)(MARGIN(i2)),BGC,i2);
  printf("\x1b[6;%dH\x1b[%d;30m%s\x1b[0m",(int)(MARGIN(i3)),BGC,i3);
  printf("\x1b[7;%dH\x1b[%d;30m%s\x1b[0m",(int)(MARGIN(i4)),BGC,i4);
  printf("\x1b[7;%dH\x1b[%d;30m%s\x1b[0m",(int)(MARGIN(i5)),BGC,i5);
  fflush(stdout);
  delay(3000);
}



int main(int argc, char **argv)
{
  
  strcpy(tuki_player.name, "tuki");
  strcpy(giuli_player.name, "giuli");
  tuki_player.marked[0]=-1;
  giuli_player.marked[0]=-1;
  evaluateCommandLine(argc, argv);

  //Init the terminal
  enableRawMode();
  initEditor();

  /*presentation*/
  show_presentation();
  
  while(1)
    {
      //Prepare the players
      set_coordinates(XCENTRALPOLE-NPOLES*SCALE,BASELINE+1,&tuki_player);
      tuki_player.UTF=TUTF;
      set_coordinates(XCENTRALPOLE+NPOLES*SCALE,BASELINE+1,&giuli_player);
      giuli_player.UTF=GUTF;
      
      /*prepares the battlefield*/
      prepare_battle_field();
      init_battlefield();
      
      /*Show option*/
      show_game_option();
      
      //Wait for user choice
      char chs=0;
      while (1) {
	chs= editorProcessKeypress();
	if(chs==2)
	  {
	    jdelay=200;
	    challenger=&tuki_turn;
	    break;
	  }
	if(chs==1)
	  {
	    jdelay=20;dl1=10;dl2=20;dl3=40;
	    challenger=&inter_turn;
	    clear_message();
	    show_intercative_instruction();
	    break;
	  }
	
      }
      clear_message();
      
      
      
      
      
      
      /**
       *START GAME
       */
      
      /*starts the loop that gives the control to the engine and to the 
	gamer alternatively*/
      
      game=1;
      while(game){
	
	t_or_g_1_or_2=2;
	machine_turn();
	t_or_g_1_or_2=1;
	(*challenger)();
      }  
      /*end of the game*/
    }
}


static void evaluateCommandLine(int argc, char **argv){
  
  if(argc == 2 && strcmp(argv[1], "-gpl") == 0) {
    fprintf(stderr,"GPL");
  }else
    if ((argc == 2 && strcmp(argv[1], "-h") == 0)||argc==1)
      {
	
	
	fprintf(stderr,"Usage  ./tuki [OPTIONS]\n");
	fprintf(stderr,"-p\tPLAY THE GAME\n");
	fprintf(stderr,"-i\tprint game instructions\n");
	fprintf(stderr,"-v\tgame version\n");
	//fprintf(stderr,"-s\tskip intro and play\n");
	fprintf(stderr,"-h\tthis help\n");
	fprintf(stderr,"-c\tthis credits\n");
	fprintf(stderr,"-GPL\tprint GPL license\n");
	exit(0);
      }
    else if (argc == 2 && strcmp(argv[1], "-v") == 0) {
      fprintf(stderr, "*** tuki %i.%i.%i\nCopyleft 2018 iSisiniPazzi\n", MAJOR, MINOR, RELEASE);
      getchar();
      exit(1);
    }
    else if (argc == 2 && strcmp(argv[1], "-p") == 0) {
    return;
    }
  else if (argc == 2 && strcmp(argv[1], "-i") == 0) {

    fprintf(stderr,"Edit tuki_turn.c and implement your code in order to make Tuki prepare stairs for Ladybeetle\n");
    fprintf(stderr,"Use the player.h API to interact with the environment.\n");
	fprintf(stderr,"Compile and link your code to the rest of the binary object by typeing \"make\" at the prompt\n");
      
      getchar();
      exit(1);
    }
  else if (argc == 2 && strcmp(argv[1], "-c") == 0) {
    fprintf(stderr,"\nCredits:\n\n");
    fprintf(stderr,"A game developed by \"Scuola Sisini\" facebook.com/MatematicaFisicaSisini/\n");
    fprintf(stderr,"Francesco Sisini \t Main developer\t (francescomichelesisini@gmail.com)\n");
    fprintf(stderr,"Valentina Sisini \t Assistant developer\n");
    fprintf(stderr,"Giorgio Dall'Aglio \t Beta testing and MacOS porting\n");
    exit(1);
  }
  
  else if (argc >1) {
    fprintf(stderr," ");
    exit(1);
  }
}
