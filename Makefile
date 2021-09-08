MACCFLAGS=-I/opt/homebrew/include/SDL2 -D_THREAD_SAFE
MACLIBS=-L/opt/homebrew/lib -lSDL2
SRC=src
MAINSRC=$(SRC)/main.cpp

MACBIN=bin/game

MAC: $(MACBIN)

$(MACBIN):
	g++ $(MAINSRC) -o $(MACBIN) $(MACCFLAGS) $(MACLIBS)

