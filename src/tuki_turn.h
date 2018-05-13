/*Defined direction*/
typedef enum  {LEFT,UP,RIGHT,DOWN,IN} direction;
typedef char strcode[4];

/*Make the player turn in the 'd' direction
*/
void turn(direction d);


/*Make the tuki take a step in the actual 'd' direction
*/
int step();

/*reads the battle-field at the position next the tuki one
 *given by the actual tuki position and direction direction
 *The int value of the player char is returnde by the function
 *while the UTF8 represetation of the play-groud is coded in 'r'
 */
int read_battle_field(strcode r);

