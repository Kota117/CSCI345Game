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

class MyGame:public Game {
	vector <Wave *> waves;

	//These two variables are not useful for the game
	//These were put here to test wave mechanics and should be removed when player physics are added
	int totalTime;
	int waveStartX;

	public:
	MyGame(int w=640, int h=480):Game("Echos", w, h) {
		totalTime = 0.0;
		waveStartX = 0;
	}

	void update(double dt) {
		SDL_RenderClear(ren);

		if(totalTime > 1000){
			waves.push_back(new Wave(ren, media, waveStartX, 480/2));
			waveStartX+=100;
			totalTime%=500;
			if(waveStartX > 640)
				waveStartX %= 640;
		}

		totalTime += (int)(dt*1000.0);

		if(waves.size() > 0){

			for(int i=0; i < waves.size(); i++){
				waves[i]->update(dt);

				if(waves[i]->getTimeAlive() > 3.0){
					delete waves[i];
					waves.erase(waves.begin()+i);
				}else if(waves[i]->getTimeAlive() > 2.0)
					waves[i]->setAnimation("media/sound3.txt");
				else if(waves[i]->getTimeAlive() > 1.0)
					waves[i]->setAnimation("media/sound2.txt");
				
			}
		}
		
		SDL_RenderPresent(ren);
	}

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