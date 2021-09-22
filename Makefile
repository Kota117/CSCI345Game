SRC=src
MAINSRC=$(SRC)/main.cpp

LINUXFLAGS=-I/usr/include/SDL2 -D_REENTRANT
LINUXLIBS=-lSDL2

MACCFLAGS=-I/opt/homebrew/include/SDL2 -D_THREAD_SAFE
MACLIBS=-L/opt/homebrew/lib -lSDL2

WININCPATH=..\\SDL2-2.0.16\\x86_64-w64-mingw32
WINFLAGS="-I$(WININCPATH)\\include\\SDL2"
WINLIBS="-L$(WININCPATH)\\lib" -lmingw32 -lSDL2main -lSDL2

MACBIN=bin/game
LINUXBIN=bin/game
WINBIN=bin/game.exe

linux: $(LINUXBIN)
mac: $(MACBIN)
win: $(WINBIN)

run:
ifeq ($(OS),Windows_NT)
	$(WINBIN)
else
	./bin/game
endif

$(LINUXBIN): $(MAINSRC)
		g++ $(MAINSRC) $(LINUXFLAGS) -o $(LINUXBIN) $(LINUXLIBS)
		
$(MACBIN): $(MAINSRC)
	g++ $(MAINSRC) -o $(MACBIN) $(MACCFLAGS) $(MACLIBS)

$(WINBIN): $(MAINSRC)
	g++.exe $(MAINSRC) -o $(WINBIN) $(WINFLAGS) $(WINLIBS)
