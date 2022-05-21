EXE = bin/main
SRC = main.c routeFind.c utils.c visual.c
LINKS = -lSDL2 -lui
INCDIR = -I/usr/include/SDL2
all:
	gcc -o $(EXE) $(SRC) $(LINKS) $(INCDIR)

clean:
	rm -f $(EXE)