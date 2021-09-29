#include <iostream>
#include <SDL.h>
#include <vector>
#include <map>
#include <math.h>

#include "Exception.hpp"
#include "MediaManager.hpp"
#include "Game.hpp"
#include "Particle.hpp"
#include "Animation.hpp"
#include "Wave.hpp"

using namespace std;

class Waves{
	SDL_Renderer *ren;
	MediaManager *media;
	vector <Wave *> waves;

	public:
	Waves(MediaManager *newMedia, SDL_Renderer *newRen){
		media=newMedia;
		ren=newRen;
	}

	void createWave(int startingX, int startingY){
		waves.push_back(new Wave(ren, media, startingX, startingY));
	}

	void updateWaves(double dt){
		if(waves.size() > 0){
			for(int i=0; i < waves.size(); i++){
				waves[i]->update(dt);
				if(waves[i]->getTimeAlive() > 4.0){
					delete waves[i];
					waves.erase(waves.begin()+i);
				}else if(waves[i]->getTimeAlive() > 2.0)
					waves[i]->setAnimation("media/sound3.txt");
				else if(waves[i]->getTimeAlive() > 1.0)
					waves[i]->setAnimation("media/sound2.txt");
				
			}
		}
	}
};

class MyGame:public Game {
	//These two variables are not useful for the game
	//These were put here to test wave mechanics and should be removed when player physics are added
	int totalTime;
	int waveStartX;

	Waves *wavs;

	Animation playerWalkingRight, playerWalkingLeft;
	Particle *player;

	public:
	MyGame(int w=640, int h=480):Game("Echos", w, h) {
		totalTime = 0.0;
		waveStartX = 0;
		wavs = new Waves(media, ren);

		playerWalkingRight.read(media, "media/walkRight.txt");
		playerWalkingLeft.read(media, "media/walkLeft.txt");

		player = new Particle(ren, &playerWalkingRight, 0, (h/2)-64, 50, 0, 0.0, 0.0, 0.0, 64, 64);
	}

	void update(double dt) {
		SDL_RenderClear(ren);

		if(totalTime > 1000){
			waveStartX=64;
			totalTime%=500;

			wavs->createWave(player->getX()+32, player->getY()+64);
		}

		if(player->getX() > (640-64)){
			player->setVX(-(player->getVX()));
			player->setAnimation(&playerWalkingLeft);
		}else if(player->getX() < 0){
			player->setVX(-(player->getVX()));
			player->setAnimation(&playerWalkingRight);
		}
			


		totalTime += (int)(dt*1000.0);

		wavs->updateWaves(dt);

		player->update(dt);
		
		SDL_RenderPresent(ren);
	}

	// Do some event handling
	// On right key down, walk player right
	// On right key up, set velocity 0

	// mirror for left

	~MyGame() {

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