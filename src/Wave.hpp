#pragma once

#include <vector>
#include <math.h>

#include "Particle.hpp"
#include "MediaManager.hpp"
#include "Animation.hpp"

#define PI 3.14159265

class Wave{
	vector <Particle*> particles;
	Animation a;
	MediaManager *media;
	SDL_Renderer *ren;
	double timeAlive;

	public:
	Wave(SDL_Renderer *newRen, MediaManager *newMedia, int startX, int startY, double waveSpeed=100, double waveDamp=0.8){
		ren = newRen;
		media = newMedia;
		timeAlive = 0.0;

		setAnimation("media/sound1.txt");

		for(int i=0; i < 180; i++) {
			//The accelerations for each sound particle are set at 0 on purpose
			//Waves acceleration should not change!
            particles.push_back(new Particle(ren, &a, startX, startY, waveSpeed, i, 0.0, 0.0, waveDamp));
		 	particles[i]->setBound(0,startY-100,0,startY);
        }
	}

	void setAnimation(string filePath){
		a.read(media, filePath);
	}

	void update(double dt){
		for(unsigned i=0; i<particles.size(); i++){
            particles[i]->update(dt);
        }
		timeAlive += dt;
	}

	double getTimeAlive() { return timeAlive; }

	~Wave(){
		for (auto p:particles) 
		  delete p;
	}
};