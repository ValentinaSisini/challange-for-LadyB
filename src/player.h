/*Defined direction*/
#ifndef PLAYER
typedef enum  {LEFT,RIGHT,IN} direction;
typedef unsigned int utf8code;
typedef char utf[4];
#endif
#define PLAYER

/**
 *Makes the player rotate in the 'd' direction
 */
void player_rotate(direction d);

/**
 *Makes a step in the actual 'd' direction
 *Be aware that at the end of this  function  
 *the control is given to Giuli and
 *the instructions under it will not be executed 
 */
int step();


/**
 *Mark the pole to be switched with the last one marked before
 *returns the poles heigh 
 */
int mark();

/**
 *Discharge the marks
 */
int umark();


/**
 *switch the last two marked poles
 */
void switchpole();

/**
 *returns pole height
 */
int pole_height();

/**
 *Created a delay
 */
void delay(int milliseconds);
