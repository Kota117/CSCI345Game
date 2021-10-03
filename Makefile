SRC=src
MAINSRC=$(SRC)/main.cpp
HEADERS= $(SRC)/Exception.hpp $(SRC)/Game.hpp $(SRC)/MediaManager.hpp $(SRC)/Particle.hpp $(SRC)/Animation.hpp $(SRC)/Wave.hpp

LINUXFLAGS=-I/usr/include/SDL2 -D_REENTRANT
LINUXLIBS=-lSDL2 -lSDL2_mixer

MACCFLAGS=-I/opt/homebrew/include/SDL2 -D_THREAD_SAFE
MACLIBS=-L/opt/homebrew/lib -lSDL2 -lSDL2_mixer

WININCPATH=..\\SDL2-2.0.16\\x86_64-w64-mingw32
WINFLAGS="-I$(WININCPATH)\\include\\SDL2"
WINLIBS="-L$(WININCPATH)\\lib" -lmingw32 -lSDL2main -lSDL2

MACBIN=bin/unix_game
LINBIN=bin/game
WINBIN=bin/game.exe

UNAME=$(shell uname -s)

win: $(WINBIN)
mac: $(MACBIN)
linux: $(LINBIN)

run:
ifeq ($(OS),Windows_NT)
	$(WINBIN)
else ifeq ($(UNAME),Darwin)
	./bin/unix_game
else
	./bin/game
endif

$(LINBIN): $(MAINSRC) $(HEADERS)
		g++ $(MAINSRC) -o $(LINBIN) $(LINUXFLAGS) $(LINUXLIBS)
		
$(MACBIN): $(MAINSRC) $(HEADERS)
	g++ -std=c++11 $(MAINSRC) -o $(MACBIN) $(MACCFLAGS) $(MACLIBS)

$(WINBIN): $(MAINSRC) $(HEADERS)
	g++.exe $(MAINSRC) -o $(WINBIN) $(WINFLAGS) $(WINLIBS)
