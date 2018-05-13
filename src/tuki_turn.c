#include "player.h"
#include <stdio.h>
#include <stdlib.h>



void tuki_turn()
{
  player_rotate(RIGHT);
  mark();
  switchpole();
  step();
  return;
}
