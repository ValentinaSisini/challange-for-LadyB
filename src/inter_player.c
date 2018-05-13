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


#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include "player.h"


void inter_turn()
{

  static direction dir;
  //Wait for user choice
  char c;
  
  read(STDIN_FILENO, &c, 1);
  switch(c)
    {
    case 'j':
      dir=LEFT;
      player_rotate(dir);
      step();
      return;
      break;
    case 'l':
      dir=RIGHT;
      player_rotate(dir);
      step();
      return;
      break;
    case 'i':
      mark();
      return;
      break;
    case 32:
      player_rotate(dir);
      switchpole();
      step();
      return;
      break;
    }
  player_rotate(dir);
  
  return;


}
