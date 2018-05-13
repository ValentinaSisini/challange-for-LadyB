# challange-for-LadyB
A terminal based coding game in C
## Stable version
Stable and ready to play version of this game, can be downloaded [here](http://www.isisinipazzi.it/tuki/tuki2.html)
## Description
Tuki has to order a serie of wood poles from the lowest to the highest before Giuli does it. You have to code a program that will guide Tuki's actions, which can be: jump left or right from pole to pole, select two poles and swap them together.
## Installing
No installation is required.
## How to play
You can start playing by challenging the algorithm written by Scuola_Sisini, in this case the game consists of writing your sorting algorithm in tuki_turn.c, compiling it with the rest of the program by typing "make" at the prompt, and then watching what happens when you run the executable `tuki`.
In steps:	
	- Take a look at the file `player.h`.
	- Create an algorithm to explore the labyrith.  
	- Implement `tuki_turn()` function with your algorithm, of course you need to use the functions defined
	in player.h here.
	- Compile the executable by typing `make` at the prompt. If there are no errors in your code the 	   	compilation should produce an executable file named `tuki`.  
	- Run `tuki` by typing `./tuki -p`
## Watch a demo
If you want to watch demo of a race with Tuki's algorithm implemented  run `tuki_demo` as follows:
```C
	- sudo chmod +x tuki_demo
	- ./tuki_demo
```
-----
You can challenge another programmer by writing your code and compiling it in the object file `giuli_turn.o`
## Licensing
Tuki is protected under the laws of the GPL and other countries. Any redistribution, reselling or copying is strictly allowed. You should have received a copy of it with this package, if not, write to the Free Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
![tuki1](code.jpg)
