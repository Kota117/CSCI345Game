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
    SDL_Rect src, dest;

	public:
	MyGame(int w=640, int h=480):Game("Echos", w, h) {
		src.x = 0;
		src.y = 0;

        for(int i=0; i < 360; i++){
            double vy = sin (i*PI/180) * 100;
			double vx = cos (i*PI/180) * 100;

			SDL_Texture *tex=media->read("media/sound1.bmp");
		 	SDL_QueryTexture(tex, NULL, NULL, &src.w, &src.h);
            particles.push_back(new Particle(ren,tex,&src, w/2, h/2, vx, vy, 0.0, 0.0, 0.9));
		 	particles[i]->setBound(0,0,w-src.w,h-src.h);
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