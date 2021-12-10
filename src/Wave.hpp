#pragma once

#include <vector>
#include <math.h>
#include <mutex>
#include <SDL_mutex.h>

#include "Particle.hpp"
#include "MediaManager.hpp"
#include "Animation.hpp"

#define PI 3.14159265

class Wave{
	vector <Particle*> particles;
	
	SDL_Renderer *ren;

	double color;
	int size;
	double decayRate;



	public:
	Wave(SDL_Renderer *newRen, int startX, int startY, double waveSpeed=100, double waveDamp=0.8,
		double startColor=255, double newDecayRate=100, int newSize=3){

		
		ren = newRen;
		color = startColor;
		decayRate = newDecayRate;
		size=newSize;

		for(int i=0; i < 360; i++) {
			//The accelerations for each sound particle are set at 0 on purpose
			//Waves acceleration should not change!
            particles.push_back(new Particle(startX, startY, waveSpeed, i, 0.0, 0.0, waveDamp));
		 	particles[i]->setBound();
        }
	}
	//bool waveLock=0;

	Particle *operator[] (int index) {
		return particles[index];
	}

	void drawParticle(int x, int y){
		for(int i=0; i<size; i++)
			for(int j=0; j<size; j++)
				SDL_RenderDrawPoint(ren, x+i, y+j);
	}

	void update(double dt){
		color -= (dt*decayRate);
		
		for(unsigned i=0; i<particles.size(); i++){
            particles[i]->update(dt);
			
        }
	}

	void render(){
		SDL_SetRenderDrawColor(ren, color, color, color, 255);
		for(unsigned i=0; i<particles.size(); i++){
			drawParticle(particles[i]->getX(), particles[i]->getY());
		}
		SDL_SetRenderDrawColor(ren, 0x00, 0x00, 0x00, 0xFF);
	}

	double getColor() { return color; }

	~Wave(){
		for (auto p:particles) 
		  delete p;
	}
};

class Waves{
	SDL_Renderer *ren;
	vector <Wave *> waves;
	SDL_mutex *waveMutex;

	public:
	Waves(SDL_Renderer *newRen){
		ren=newRen;
		waveMutex= SDL_CreateMutex();
	}

	Wave *operator[] (int index) {
		return waves[index];
	}

	void createWave(Mix_Chunk *sound, int startingX, int startingY, double waveSpeed=100, double waveDamp=0.8,
		double startColor=255, double decayRate=100, int size=3){
		
		//we could associate these properties with the actual sounds and have them read in config style. That may be a good choice
		if(SDL_LockMutex(waveMutex)==0){
			waves.push_back(new Wave(ren, startingX, startingY, waveSpeed, waveDamp, startColor, decayRate, size));
			SDL_UnlockMutex(waveMutex);
		}
		Mix_PlayChannel(-1,sound,0);
	}

	void deleteWaves() {
		//SDL_LockMutex(waveMutex);
		while (waves.size()>0){ waves.erase(waves.begin());}
		//SDL_UnlockMutex(waveMutex);
	}

	bool collideSound(Particle *newP) {
		bool hasCollision = false;
		if(SDL_LockMutex(waveMutex)==0){
			for(auto w:waves){
				for(int i=0; i<360; i++) {
					if((*w)[i]->collide(newP)){
						hasCollision = true;
					}
				}
			}
			SDL_UnlockMutex(waveMutex);
		}
		return hasCollision;
	}

/*	void lockWave(Wave *wave){
		wave->waveLock=true;
	}
	void unlockWave(Wave *wave){
		wave->waveLock=false;
	}
*/
	void updateWaves(double dt){
		if(SDL_LockMutex(waveMutex)==0){
			if(waves.size() > 0){
				for(int i=waves.size()-1; i >=0; i--){
					waves[i]->update(dt);

					//Once a wave has become invisible it is deleted
					//This means we are not allowing fully invisible waves to be on screen at all
					if(waves[i]->getColor() < 0.0){
						delete waves[i];
						waves.erase(waves.begin()+i);
					}
				}
			}
			SDL_UnlockMutex(waveMutex);
		}
	}

	void renderWaves(){
		if(SDL_LockMutex(waveMutex)==0){
			for(int i=waves.size()-1; i >=0; i--){
				waves[i]->render();
			}
			SDL_UnlockMutex(waveMutex);
		}
	}
};