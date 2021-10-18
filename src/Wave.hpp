#pragma once

#include <vector>
#include <math.h>

#include "Particle.hpp"
#include "MediaManager.hpp"
#include "Animation.hpp"

#define PI 3.14159265

class Wave{
	vector <SoundParticle*> particles;
	
	SDL_Renderer *ren;
	double timeAlive;

	public:
	Wave(SDL_Renderer *newRen, int startX, int startY, double waveSpeed=100, double waveDamp=0.8){
		timeAlive = 0.0;

		ren = newRen;

		for(int i=0; i < 360; i++) {
			//The accelerations for each sound particle are set at 0 on purpose
			//Waves acceleration should not change!
			//Change this to SoundParticle
            particles.push_back(new SoundParticle(ren, startX, startY, waveSpeed, i, 0.0, 0.0, waveDamp));
		 	particles[i]->setBound(0,startY-100,0,480/2);
        }
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

class Waves{
	SDL_Renderer *ren;
	MediaManager *media;
	vector <Wave *> waves;

	public:
	Waves(SDL_Renderer *newRen){
		ren=newRen;
	}

	void createWave(Mix_Chunk *sound, int startingX, int startingY){
		waves.push_back(new Wave(ren, startingX, startingY));
		Mix_PlayChannel(-1,sound,0);
	}

	void updateWaves(double dt){
		
		if(waves.size() > 0){
			for(int i=0; i < waves.size(); i++){
				waves[i]->update(dt);
				if(waves[i]->getTimeAlive() > 4.0){
					delete waves[i];
					waves.erase(waves.begin()+i);
				}
			}
		}
		
	}
};