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
#include "Object.hpp"
#include "Map.hpp"

using namespace std;

class MyGame:public Game {
	Waves *waves;

	Config *playerConf;
	Player *player;

	map<string, Config *> entityConfs;
	vector<Entity *>entities;

	map<string,Config *> objectConfs;
	vector<Object *>objects;

	Mix_Chunk *backgroundMusic;

	Animation *tvStatic;
	SDL_Rect *staticDest;

	void spawnEntity(int x, string type) {
		entities.push_back(new Entity(media, ren, waves, entityConfs[type], x));
	}

	public:
	MyGame(Config &gameConf):Game(gameConf["name"], stoi(gameConf["screenW"]), stoi(gameConf["screenH"])) {
		waves = new Waves(ren);
		backgroundMusic = media->readSound(gameConf["backgroundMusic"]);

		playerConf = new Config("player");
		player = new Player(media, ren, waves, playerConf, 100);

		entityConfs["basic"] = (new Config("entity"));
		
		for(int i=0; i<1; i++)
			spawnEntity(300+(i*50), "basic");

		objectConfs["floor"] = (new Config("floor"));
		for (int i=0; i<640; i+=16) 
			objects.push_back(new Object(media, ren, waves, objectConfs["floor"],i));

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

	void updateEntities(double dt) {
		for (auto& e:entities) e->update(dt, player->getX());

		vector<int> locations;
		for (int i=0; i<entities.size(); i++) {
			if (entities[i]->collide(player->getDest())) locations.push_back(i-locations.size());
		}
		for (auto i:locations) entities.erase(entities.begin()+i);
	}

	void update(double dt) {
		SDL_RenderClear(ren);

		waves->updateWaves(dt);

		player->update(dt);

		updateEntities(dt);

		for (auto& o:objects) o->update(dt);

		// tvStatic->update(dt);
		// SDL_RenderCopy(ren, tvStatic->getTexture(), tvStatic->getFrame(), staticDest);
		 
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
	}

	~MyGame() {
		delete player;
		delete waves;
		for (int i=0; i<entities.size(); i++) entities.erase(entities.begin());
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