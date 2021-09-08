MACCFLAGS=-I/opt/homebrew/include/SDL2 -D_THREAD_SAFE
MACLIBS=-L/opt/homebrew/lib -lSDL2
SRC=src
MAINSRC=$(SRC)/main.cpp
LINUXFLAGS=-I/usr/include/SDL2 -D_REENTRANT
LINUXLIBS=-lSDL2

MACBIN=bin/game
LINUXBIN=bin/linuxGame

MAC: $(MACBIN)

$(LINUXBIN):
		g++ $(MAINSRC) $(LINUXFLAGS) -o $(LINUXBIN) $(LINUXLIBS)
		
$(MACBIN):
	g++ $(MAINSRC) -o $(MACBIN) $(MACCFLAGS) $(MACLIBS)

