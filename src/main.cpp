#include <iostream>
#include <SDL.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <vector>
#include <map>
#include <math.h>
#include <string>
#include <SDL_mutex.h>

#include "Exception.hpp"
#include "MediaManager.hpp"
#include "Game.hpp"
#include "Particle.hpp"
#include "Animation.hpp"
#include "Wave.hpp"
#include "Player.hpp"
#include "NPC.hpp"
#include "Key.hpp"
#include "Config.hpp"
#include "Tile.hpp"
#include "Map.hpp"
#include "Menus.hpp"


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

		waves = new Waves(ren);

		currentLevel=1;
		level = new Map(media, ren, waves, NULL);
		level->initMap(currentLevel);

		playerConf = new Config("player");
		player = new Player(media, ren, waves, playerConf, level->getStartX(), level->getStartY());

		Mix_PlayChannel(-1,backgroundMusic,-1);

		//This block is for initing the static effect
		tvStatic = new Animation(100);
		tvStatic->readAnimation(media, "static");

		staticDest = new SDL_Rect();
		staticDest->x = 0;
		staticDest->x = 0;
		staticDest->w = stoi(gameConf["screenW"]);
		staticDest->h = stoi(gameConf["screenH"]);

		SDL_SetRenderDrawBlendMode(ren, SDL_BLENDMODE_BLEND);
	}

	void levelChange(int levelNum) {
		Map *oldLevel = level;

		Map *newLevel = new Map(media, ren, waves, NULL);
		newLevel->initMap(levelNum);
		level = newLevel;
		player->setX(level->getStartX());
		player->setY(level->getStartY());

		delete oldLevel;
		currentLevel = levelNum;
	}

	void update(double dt) {
		player->update(dt);
		level->update(dt, player);


		tvStatic->update(dt);
		if(player->unlockedDoor() && !player->leftTheBuilding()) { 
			levelChange(currentLevel+1);
			player->leave();
			}
		if(player->getY()>=player->getMaxY()){
			player->setVY(0);
			levelChange(currentLevel);
		}
	}

	void render(){
		SDL_RenderClear(ren);
		SDL_RenderCopy(ren, tvStatic->getTexture(), tvStatic->getFrame(), staticDest);

		level->render(player);
		  
		SDL_RenderPresent(ren);
	}

	/*void handleKeyUp(SDL_Event keyEvent) {
		if (keyEvent.key.keysym.sym==SDLK_a || keyEvent.key.keysym.sym==SDLK_LEFT ||
			keyEvent.key.keysym.sym==SDLK_d || keyEvent.key.keysym.sym==SDLK_RIGHT
		){
			player->stopMoving();
		}
		if (keyEvent.key.keysym.sym==SDLK_e) { player->setClap(false); }
	}
	*/

	void handleKeyUp(SDL_Event keyEvent) {
		switch(keyEvent.key.keysym.sym)
		{
			case SDLK_LEFT:
				player->stopMoving();
				break;
			case SDLK_a:
				player->stopMoving();
				break;
			case SDLK_RIGHT:
				player->stopMoving();
				break;
			case SDLK_d:
				player->stopMoving();
				break;
			case SDLK_e:
				player->setClap(false);
				break;
		}
	}
	void handleKeyDown(SDL_Event keyEvent) {
		switch (keyEvent.key.keysym.sym)
		{
			case SDLK_LEFT:
				player->moveLeft();
				break;
			case SDLK_a:
				player->moveLeft();
				break;
			case SDLK_RIGHT:
				player->moveRight();
				break;
			case SDLK_d:
				player->moveRight();
				break;
			case SDLK_SPACE:
				if(player->isOnTile()){
					player->jump();
				}
				break;
			case SDLK_e:
				player->clap();
				break;
			case SDLK_m:
				pauseMenu();
				break;
			case SDLK_1:
				levelChange(1);
				break;
			case SDLK_2:
				levelChange(2);
				break;
			case SDLK_3:
				levelChange(3);
				break;
			default:
				break;
		}
	}
	/*void handleKeyDown(SDL_Event keyEvent) {
		if(!player->isMoving()){
			if(keyEvent.key.keysym.sym==SDLK_a || keyEvent.key.keysym.sym==SDLK_LEFT)
				player->moveLeft();
			else if(keyEvent.key.keysym.sym==SDLK_d || keyEvent.key.keysym.sym==SDLK_RIGHT)
				player->moveRight();

			else if(keyEvent.key.keysym.sym==SDLK_SPACE)
				player->jump();
		}

		if(player->isMoving()){ //for jumping while moving
			if(keyEvent.key.keysym.sym==SDLK_SPACE)
				player->jump();
		}
		
		if(keyEvent.key.keysym.sym==SDLK_e)
			player->clap();
		else if(keyEvent.key.keysym.sym==SDLK_1)
			levelChange("level1");
		else if(keyEvent.key.keysym.sym==SDLK_2)
			levelChange("level2");
		else if(keyEvent.key.keysym.sym==SDLK_m){
			cout << "Pressed m\n";
			pauseMenu();
		}
		// else if(keyEvent.key.keysym.sym==SDLK_3)
		// 	levelChange(3);

	}
	*/

	~MyGame() {
		delete player;
		delete waves;	
	}
};

int main(int argc, char* argv[]) {

	int startGame = mainMenu();
	if(startGame){
		try {
			Config gameConf("game");

			MyGame g(gameConf);

			g.run();
		} catch (Exception e) {
			cerr << e;
		}
	}
    return 0;
}