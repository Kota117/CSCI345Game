MACCFLAGS=-I/opt/homebrew/include/SDL2 -D_THREAD_SAFE
MACLIBS=-L/opt/homebrew/lib -lSDL2
SRC=src
MAINSRC=$(SRC)/main.cpp

MACBIN=bin/macGame

$(MACBIN):
	g++ $(MAINSRC) -o bin/game $(MACCFLAGS) $(MACLIBS)

