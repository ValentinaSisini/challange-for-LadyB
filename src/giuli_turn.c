#include "player.h"
#include <stdio.h>
#include <stdlib.h>



void giuli_turn()
{
  player_rotate(RIGHT);
  mark();
  switchpole();
  step();
  return;
}
