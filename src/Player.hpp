#pragma once

#include <map>

#include "Particle.hpp"
#include "Wave.hpp"
#include "Animation.hpp"

using namespace std;

class Player:public Particle{
	//These are lists of animations and sounds associated with the player
	map<string,Animation *> playerAnimations;
	map<string,Mix_Chunk *> playerSounds;

	Animation *a;
	SDL_Rect dest;

	SDL_Renderer *ren;

	Waves *waves;

	//This member is used to control wave creation timing
	int timeMoving;
	
	double walkSpeed;

	public:
	Player(SDL_Renderer *newRen, map<string,Animation *> newPlayerAnimations, Animation* startingAnimation, Waves *newWaves,
		map<string,Mix_Chunk *> newPlayerSounds,
		double newx=0.0, double newy=0.0,
		double newv=0.0, int newtheta=0,
		double newax=0.0, double neway=0.0,
		double newdamp=0.0):Particle(newx, newy, newv, newtheta, newax, neway, newdamp){

		//The above is constructing a Player object as a particle with some default params
		
		//the placement of these should be better thought out!
		dest.w=64;
		dest.h=64;

		ren=newRen;
		a=startingAnimation;

		timeMoving=0;
		walkSpeed = 50.0;

		waves = newWaves;
		playerSounds = newPlayerSounds;
		playerAnimations = newPlayerAnimations;
	}

	void walkRight(){
		v = walkSpeed;
		theta = 0;
		waves->createWave(playerSounds["footstep"], x+32, y+64);
		setAnimation(playerAnimations["walkRight"]);
	}

	void walkLeft(){
		v = walkSpeed;
		theta = 180;
		waves->createWave(playerSounds["footstep"], x+16, y+64);
		setAnimation(playerAnimations["walkLeft"]);
	}

	void stopMoving(){
		v=0;
		timeMoving=0;
		setAnimation(playerAnimations["idle"]);
	}

	void clap(){
		waves->createWave(playerSounds["clap"], x+32, y+32);
	}

	void update(double dt){
		Particle::update(dt);

		if(timeMoving >= 1000){
			timeMoving%=500;
			if(v<0)
				waves->createWave(playerSounds["footstep"], x, y+64);
			else
				waves->createWave(playerSounds["footstep"], x+32, y+64);
		}
 
		if(v!=0)
			timeMoving += (int)(dt*1000.0);

		
		a->update(dt);
		dest.x = x;
		dest.y = y;

		SDL_RenderCopy(ren, a->getTexture(), a->getFrame(), &dest);
	}

	bool isMoving() { return v!=0; }

	void setAnimation(Animation *newA){ a=newA; }

	SDL_Rect *getDest(){return &dest;}

	~Player(){
		for(auto a:playerAnimations) delete a.second;
		for(auto s:playerSounds) delete s.second;
	}
};