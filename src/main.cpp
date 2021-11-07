#include <iostream>
#include <SDL.h>
#include <SDL_mixer.h>
#include <vector>
#include <map>
#include <math.h>
#include <string>

#include "Exception.hpp"
#include "MediaManager.hpp"
#include "Game.hpp"
#include "Particle.hpp"
#include "Animation.hpp"
#include "Wave.hpp"
#include "Player.hpp"
#include "Entity.hpp"
#include "Config.hpp"
#include "Tile.hpp"
#include "Map.hpp"

using namespace std;

class MyGame:public Game {
	Waves *waves;

	Config *playerConf;
	Player *player;

	Map *level;
	int currentLevel;

	Mix_Chunk *backgroundMusic;

	Animation *tvStatic;
	SDL_Rect *staticDest;

	public:
	MyGame(Config &gameConf):Game(gameConf["name"], stoi(gameConf["screenW"]), stoi(gameConf["screenH"])) {
		backgroundMusic = media->readSound(gameConf["backgroundMusic"]);

		currentLevel=1;

		waves = new Waves(ren);

		level = new Map(media, ren, waves, NULL);
		level->initMap(currentLevel);

		playerConf = new Config("player");
		player = new Player(media, ren, waves, playerConf, level->getStartX(), level->getStartY());

		Mix_PlayChannel(-1,backgroundMusic,-1);

		//This block is for initing the static effect
		tvStatic = new Animation();
		tvStatic->readAnimation(media, "static");
		SDL_Texture *staticTexture = tvStatic->getTexture();
		SDL_SetTextureAlphaMod(staticTexture, 100);

		staticDest = new SDL_Rect();
		staticDest->x = 0;
		staticDest->x = 0;
		staticDest->w = stoi(gameConf["screenW"]);
		staticDest->h = stoi(gameConf["screenH"]);

		SDL_SetRenderDrawBlendMode(ren, SDL_BLENDMODE_BLEND);
	}

	void levelChange(int levelNum) {
		currentLevel = levelNum;
		waves->deleteWaves();

		level = new Map(media, ren, waves, NULL);
		level->initMap(currentLevel);

		player = new Player(media, ren, waves, playerConf, level->getStartX(), level->getStartY());
	}

	void update(double dt) {
		SDL_RenderClear(ren);

		waves->updateWaves(dt);

		player->update(dt);

		level->update(dt, player);

		tvStatic->update(dt);
		SDL_RenderCopy(ren, tvStatic->getTexture(), tvStatic->getFrame(), staticDest);
		 
		SDL_RenderPresent(ren);
	}

	void handleKeyUp(SDL_Event keyEvent) {
		if (keyEvent.key.keysym.sym==SDLK_a || keyEvent.key.keysym.sym==SDLK_LEFT ||
			keyEvent.key.keysym.sym==SDLK_d || keyEvent.key.keysym.sym==SDLK_RIGHT
		){
			player->stopMoving();
		}
	}

	void handleKeyDown(SDL_Event keyEvent) {
		if(!player->isMoving()){
			if(keyEvent.key.keysym.sym==SDLK_a || keyEvent.key.keysym.sym==SDLK_LEFT) {
				player->moveLeft();
			}
			else if(keyEvent.key.keysym.sym==SDLK_d || keyEvent.key.keysym.sym==SDLK_RIGHT) {
				player->moveRight();
			}
		}
		
		if(keyEvent.key.keysym.sym==SDLK_e)
			player->clap();
		else if(keyEvent.key.keysym.sym==SDLK_1)
			levelChange(1);
		else if(keyEvent.key.keysym.sym==SDLK_2)
			levelChange(2);
		else if(keyEvent.key.keysym.sym==SDLK_3)
			levelChange(3);
	}

	~MyGame() {
		delete player;
		delete waves;	
	}
};

int main(int argc, char* argv[]) {
	try {
		Config gameConf("game");

		MyGame g(gameConf);

		g.run();
	} catch (Exception e) {
		cerr << e;
	}
    return 0;
}