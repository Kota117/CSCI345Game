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
	Wave(SDL_Renderer *newRen, MediaManager *newMedia, int startX, int startY){
		ren = newRen;
		media = newMedia;
		timeAlive = 0.0;

		setAnimation("media/sound1.txt");

		int angle = 0;
		for(int i=0; i < 180; i++){
			angle = i+180;
            double vy = sin (angle*PI/180) * 100;
			double vx = cos (angle*PI/180) * 100;

            particles.push_back(new Particle(ren, &a, startX, startY, vx, vy, 0.0, 0.0, 0.5));
		 	particles[i]->setBound(0,startY-100,0,startY+1);
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