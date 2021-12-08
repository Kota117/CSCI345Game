SRC=src
MAINSRC=$(SRC)/main.cpp
HEADERS= $(SRC)/Exception.hpp $(SRC)/Game.hpp $(SRC)/MediaManager.hpp $(SRC)/Particle.hpp $(SRC)/Animation.hpp $(SRC)/Wave.hpp $(SRC)/Player.hpp $(SRC)/NPC.hpp $(SRC)/Config.hpp $(SRC)/Character.hpp $(SRC)/Tile.hpp $(SRC)/Map.hpp $(SRC)/Lightning.hpp $(SRC)/Menus.hpp

LINUXFLAGS=-I/usr/include/SDL2 -D_REENTRANT
LINUXLIBS=-lSDL2 -lSDL2_mixer -lSDL2_ttf

MACCFLAGS=-I/opt/homebrew/include/SDL2 -D_THREAD_SAFE
MACLIBS=-L/opt/homebrew/lib -lSDL2 -lSDL2_mixer

WININCPATH=..\\SDL2-2.0.16\\x86_64-w64-mingw32
WINMIXERPATH=..\\SDL2_mixer-2.0.4\\x86_64-w64-mingw32
WINFLAGS="-I$(WININCPATH)\\include\\SDL2" "-I$(WINMIXERPATH)\\include\\SDL2"
WINLIBS="-L$(WININCPATH)\\lib" "-L$(WINMIXERPATH)\\lib"  -lmingw32 -lSDL2main -lSDL2 -lSDL2_mixer

WININC32PATH=..\\SDL2-2.0.16\\i686-w64-mingw32
WINMIXER32PATH=..\\SDL2_mixer-2.0.4\\i686-w64-mingw32
WIN32FLAGS="-I$(WININC32PATH)\\include\\SDL2" "-I$(WINMIXER32PATH)\\include\\SDL2"
WIN32LIBS="-L$(WININC32PATH)\\lib" "-L$(WINMIXER32PATH)\\lib"  -lmingw32 -lSDL2main -lSDL2 -lSDL2_mixer


MACBIN=bin/unix_game
LINBIN=bin/game
WINBIN=bin/game.exe
WIN32BIN=bin/game32.exe

UNAME=$(shell uname -s)

win: $(WINBIN)
win32: $(WIN32BIN)
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

run32:
	$(WIN32BIN)

$(LINBIN): $(MAINSRC) $(HEADERS)
	g++ $(MAINSRC) -o $(LINBIN) $(LINUXFLAGS) $(LINUXLIBS)
		
$(MACBIN): $(MAINSRC) $(HEADERS)
	g++ -std=c++11 $(MAINSRC) -o $(MACBIN) $(MACCFLAGS) $(MACLIBS)

$(WINBIN): $(MAINSRC) $(HEADERS)
	g++.exe -g $(MAINSRC) -o $(WINBIN) $(WINFLAGS) $(WINLIBS)

$(WIN32BIN): $(MAINSRC) $(HEADERS)
	g++.exe $(MAINSRC) -o $(WIN32BIN) $(WIN32FLAGS) $(WIN32LIBS)
