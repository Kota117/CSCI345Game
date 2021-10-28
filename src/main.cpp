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

using namespace std;



class MyGame:public Game {
	Waves *waves;

	
	Player *player;
	vector<Entity *>entities;

	Mix_Chunk *backgroundMusic;

	void spawnEntity(int num=1, string type="") {
		map<string,Animation *> animations;
		map<string,Mix_Chunk *> sounds;

		for (int i=0; i<num; i++) {
			animations["walkRight"] = new Animation();
			animations["walkRight"]->readAnimation(media, type+"walkRight");

			animations["walkLeft"] = new Animation();
			animations["walkLeft"]->readAnimation(media, type+"walkLeft");

			animations["idle"] = new Animation();
			animations["idle"]->readAnimation(media, type+"idle");

			sounds["footstep"] = new Mix_Chunk();
			sounds["footstep"]=media->readSound(type+"footstep");

			sounds["clap"] = new Mix_Chunk();
			sounds["clap"]=media->readSound(type+"clap");

			entities.push_back(new Entity(ren, animations, animations["idle"], waves, sounds, type, 300+i*50, (480/2)-64));
		}
	}

	public:
	MyGame(Config &gameConf):Game(gameConf["name"], stoi(gameConf["screenW"]), stoi(gameConf["screenH"])) {
		waves = new Waves(ren);
		backgroundMusic = media->readSound(gameConf["backgroundMusic"]);

		Config playerConf("player");
		player = new Player(media, ren, waves, playerConf, 100, (480/2)-64);

		spawnEntity(5);
		Mix_PlayChannel(-1,backgroundMusic,-1);
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
				player->walkLeft();
			}
			else if(keyEvent.key.keysym.sym==SDLK_d || keyEvent.key.keysym.sym==SDLK_RIGHT) {
				player->walkRight();
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