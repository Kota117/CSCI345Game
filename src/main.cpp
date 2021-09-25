#include <iostream>
#include <SDL.h>
#include <vector>
#include <map>
#include <math.h>

#include "Exception.hpp"
#include "MediaManager.hpp"
#include "Game.hpp"
#include "Particle.hpp"

#define PI 3.14159265

using namespace std;

class MyGame:public Game {
	vector <Particle*> particles;
    Animation a;


	public:
	MyGame(int w=640, int h=480):Game("Echos", w, h) {
		a.read(media, "media/sound1.txt");

        for(int i=0; i < 360; i++){
            double vy = sin (i*PI/180) * 100;
			double vx = cos (i*PI/180) * 100;

            particles.push_back(new Particle(ren, &a, w/2, h/2, vx, vy, 0.0, 0.0, 0.9));
		 	particles[i]->setBound(0,0,w-8,h-8);
        }
	}

	void update(double dt) {
		SDL_RenderClear(ren);
        for(unsigned i=0; i<particles.size(); i++){
            particles[i]->update(dt);
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