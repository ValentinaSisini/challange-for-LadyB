LIBS=-lm
SRC=src/
BIN=./
CFILES= $(SRC)main.c $(SRC)view.c $(SRC)machine.c $(SRC)player.c $(SRC)giuli_turn.c $(SRC)tuki_turn.c $(SRC)inter_player.c
HFILES=$(SRC)view.h $(SRC)machine.h $(SRC)player.h 
OFILES=$(BIN)main.o $(BIN)view.o $(BIN)machine.o $(BIN)player.o $(BIN)giuli_turn.o $(BIN)tuki_turn.o 
all:	tuki

tuki:	main.o view.o machine.o player.o giuli_turn.o tuki_turn.o inter_player.o
	gcc -o tuki main.o view.o machine.o player.o giuli_turn.o tuki_turn.o inter_player.o $(LIBS)		

main.o:	$(SRC)main.c $(SRC)player.h $(SRC)view.h $(SRC)machine.h
	gcc -c   $(SRC)main.c
view.o:	$(SRC)view.c $(SRC)view.h
	gcc -c   $(SRC)view.c
machine.o: $(SRC)machine.c $(SRC)machine.h
	gcc -c   $(SRC)machine.c
player.o: $(SRC)player.c $(SRC)player.h
	gcc -c   $(SRC)player.c
giuli_turn.o: $(SRC)giuli_turn.c 
	gcc -c   $(SRC)giuli_turn.c
inter_player.o:	 $(SRC)inter_player.c 
	gcc -c $(SRC)inter_player.c
tuki_turn.o: $(SRC)tuki_turn.c 	
	gcc -c   $(SRC)tuki_turn.c

clean:
	rm -f tuki $(OFILES)

