CFLAGS = -g -Wall
all : Othello

Othello.o : Othello.c
	gcc ${CFLAGS} -c Othello.c

ListePos.o : ListePos.c
	gcc ${CFLAGS} -c ListePos.c

ArbreMiMa.o : ArbreMiMa.c
	gcc ${CFLAGS} -c ArbreMiMa.c

Affichage.o : Affichage.c
	gcc ${CFLAGS} `sdl2-config --cflags` -c Affichage.c

Main.o : Main.c
	gcc ${CFLAGS} -c Main.c

Othello : ListePos.o Othello.o Main.o Affichage.o ArbreMiMa.o
	gcc ${CFLAGS} -o Othello ListePos.o Othello.o Main.o Affichage.o ArbreMiMa.o `sdl2-config --libs`

clean :
	rm -f *.o Othello
