#pragma once

#include <map>
#include <SDL_mixer.h>
#include <SDL.h>

#include "Particle.hpp"
#include "Animation.hpp"
#include "MediaManager.hpp"
#include "Config.hpp"
#include "Wave.hpp"

using namespace std;

class Character:public Particle{
	Config *cfg;
	SDL_Renderer *ren;
	MediaManager *media;

	map<string,Animation *> animations;
	map<string,Mix_Chunk *> sounds;

	double baseSpeed;
    int timeMoving;

	protected:
	Animation *a;
	SDL_Rect dest;

	Waves *waves;


	public:
	Character(MediaManager *newMedia, SDL_Renderer *newRen, Waves *newWaves, Config *newCfg,
		double newx=0.0, double newy=0.0,
		double newv=0.0, int newtheta=0,
		double newax=0.0, double neway=0.0,
		double newdamp=0.0):Particle(newx, newy, newv, newtheta, newax, neway, newdamp){

		ren=newRen;
		media=newMedia;
		cfg=newCfg;
		waves=newWaves;
		timeMoving=0;

		dest.w = stoi((*cfg)["width"]);
		dest.h = stoi((*cfg)["height"]);

		y=newy-dest.h;

		baseSpeed = stod((*cfg)["baseSpeed"]);

		vector<string> newAnimations = cfg->getMany("animations");
		for(auto anim: newAnimations){
			animations[anim] = new Animation();
			animations[anim]->readAnimation(media, anim);
		}

		a=animations[(*cfg)["defaultAnimation"]];
		
		vector<string> newSounds = cfg->getMany("sounds");
		for(auto sound: newSounds){
			sounds[sound] = new Mix_Chunk();
			sounds[sound] = media->readSound(sound);
		}
		
	}

	//Basic Getters
	bool isMoving() { return v!=0; }
	SDL_Rect *getDest(){return &dest;}

	//Basic Setters
	void setAnimation(Animation *newA){ a=newA; }

	void moveRight(){
		v = baseSpeed;
		theta = 0;
		waves->createWave(sounds["footstep"], x+dest.w/2, y+dest.h);
		setAnimation(animations["walkRight"]);
	}

	void moveLeft(){
		v = baseSpeed;
		theta = 180;
		waves->createWave(sounds["footstep"], x+dest.w/4, y+dest.h);
		setAnimation(animations["walkLeft"]);
	}

	void stopMoving(){
		v=0;
		timeMoving=0;
		setAnimation(animations[(*cfg)["defaultAnimation"]]);
	}

	void clap(){ waves->createWave(sounds["clap"], x+dest.w/2, y+dest.h/2); }

	virtual void update(double dt){
		Particle::update(dt);

		if(timeMoving >= 1000){
			timeMoving%=500;
			if(v<0)
				waves->createWave(sounds["footstep"], x, y+dest.h);
			else
				waves->createWave(sounds["footstep"], x+dest.w/2, y+dest.h);
		}
 
		if(v!=0)
			timeMoving += (int)(dt*1000.0);

		a->update(dt);
		dest.x = x;
		dest.y = y;

		SDL_RenderCopy(ren, a->getTexture(), a->getFrame(), &dest);
	}

	~Character(){
		for(auto a:animations) delete a.second;
		for(auto s:sounds) delete s.second;
	}
};