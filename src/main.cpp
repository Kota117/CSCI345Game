#include <iostream>
#include <SDL.h>
#include <SDL_mixer.h>
#include <vector>
#include <map>
#include <math.h>
#include <map>

#include "Exception.hpp"
#include "MediaManager.hpp"
#include "Game.hpp"
#include "Particle.hpp"
#include "Animation.hpp"
#include "Wave.hpp"
#include "Player.hpp"

using namespace std;

class MyGame:public Game {
	Waves *waves;
	Player *player;

	Mix_Chunk *backgroundMusic;

	void initPlayer(){
		//this list of actions could be setup in a config file
		map<string,Animation *> playerAnimations;
		map<string,Mix_Chunk *> playerSounds;

		playerAnimations["walkRight"] = new Animation();
		playerAnimations["walkRight"]->readAnimation(media, "walkRight");

		playerAnimations["walkLeft"] = new Animation();
		playerAnimations["walkLeft"]->readAnimation(media, "walkLeft");

		playerAnimations["idle"] = new Animation();
		playerAnimations["idle"]->readAnimation(media, "idle");

		playerSounds["footstep"] = new Mix_Chunk();
		playerSounds["footstep"]=media->readSound("footstep");

		playerSounds["clap"] = new Mix_Chunk();
		playerSounds["clap"]=media->readSound("clap");

		player = new Player(ren, playerAnimations, playerAnimations["idle"], waves, playerSounds, 320, (480/2)-64);
	}

	public:
	MyGame(int w=640, int h=480):Game("Echos", w, h) {
		waves = new Waves(media, ren);
		backgroundMusic = media->readSound("backgroundMusic");
		initPlayer();
		Mix_PlayChannel(-1,backgroundMusic,-1);
	}

	void update(double dt) {
		SDL_RenderClear(ren);

		waves->updateWaves(dt);

		player->update(dt);
		 
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
			if(keyEvent.key.keysym.sym==SDLK_a || keyEvent.key.keysym.sym==SDLK_LEFT)
				player->walkLeft();
			else if(keyEvent.key.keysym.sym==SDLK_d || keyEvent.key.keysym.sym==SDLK_RIGHT)
				player->walkRight();
		}
		
		if(keyEvent.key.keysym.sym==SDLK_e)
			player->clap();
	}

	~MyGame() {
		delete player;
		delete waves;
	}
};

int main(int argc, char* argv[]) {
    try {
		MyGame g;
		g.run();
	} catch (Exception e) {
		cerr << e;
	}
    return 0;
}