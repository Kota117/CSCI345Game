#pragma once

#include <map>
#include <SDL_mixer.h>
#include <SDL.h>

#include "Particle.hpp"
#include "Animation.hpp"
#include "MediaManager.hpp"
#include "Config.hpp"
#include "Wave.hpp"
#include "Tile.hpp"

#define GRAVITY 300

using namespace std;

enum direction{LEFT, RIGHT, STOP};

class Character:public Particle{
	Config *cfg;
	SDL_Renderer *ren;
	MediaManager *media;

	map<string,Animation *> animations;
	map<string,Mix_Chunk *> sounds;

	double baseSpeed, jumpSpeed;
	int timeMoving;
	direction dir;
	bool clapped, inAir, onTile;

	protected:
	Animation *a;
	SDL_Rect dest;

	Waves *waves;

	public:
	Character(MediaManager *newMedia, SDL_Renderer *newRen, Waves *newWaves, Config *newCfg,
		double newx=0.0, double newy=0.0,
		double newv=0.0, int newtheta=0,
		double newax=0.0, double neway=0.0,
		double newdamp=0.0):Particle(newx, newy, newv, newtheta, newax, neway, newdamp, true){

		ren=newRen;
		media=newMedia;
		cfg=newCfg;
		waves=newWaves;
		timeMoving=0;
		inAir = true;
		dir=STOP;
		clapped=false;
		onTile=true;

		dest.w = stoi((*cfg)["width"]) * stoi((*cfg)["scale"]);
		dest.h = stoi((*cfg)["height"]) * stoi((*cfg)["scale"]);

		y=newy-dest.h;

		baseSpeed = stod((*cfg)["baseSpeed"]);
		jumpSpeed = stod((*cfg)["jumpSpeed"]);

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
	bool isMoving() { return vx!=0 || vy != 0; }
	SDL_Rect *getDest(){return &dest;}
	bool isInAir() { return inAir;}

	//Basic Setters
	void setClap(bool x) { clapped = x; }
	void setAnimation(Animation *newA){ a=newA; }

	void moveRight(){
		if (dir!=RIGHT) {
			dir=RIGHT;
			vx = baseSpeed;
			theta = 0;
			waves->createWave(sounds["footstep"], x+dest.w/2, y+dest.h);
			setAnimation(animations["walkRight"]);
		}
	}

	void moveLeft(){
		if (dir!=LEFT) {
			dir=LEFT;
			vx = -baseSpeed;
			theta = 180;
			waves->createWave(sounds["footstep"], x+dest.w/4, y+dest.h);
			setAnimation(animations["walkLeft"]);
		}
	}

	void stopMoving(){
		if (dir!=STOP) {
			dir=STOP;
			vx=0;
			timeMoving=0;
			setAnimation(animations[(*cfg)["defaultAnimation"]]);
		}
	}

	void stopFalling(){
		inAir=false;
		vy=0;
		ay=0;
		timeMoving=0;
		if(vx>0){
			waves->createWave(sounds["footstep"], x+dest.w/2, y+(dest.h-3));
			setAnimation(animations["walkRight"]);
		} else if(vx<0){
			waves->createWave(sounds["footstep"], x, y+(dest.h-3));
			setAnimation(animations["walkLeft"]);
		} else{
			waves->createWave(sounds["footstep"], x, y+(dest.h-3));
			setAnimation(animations[(*cfg)["defaultAnimation"]]);
		}
	}

	void hitTile(Tile *t) {
		vx=0;
		if (t->getType() == "lWall") x = t->getX()-dest.w-1;
		else if (t->getType() == "rWall") x = t->getX()+t->getW()+dest.w+1;
		else if (t->getType() == "floor") y = t->getY()-dest.h-1; 
	}

	void clap(){
		if (!clapped) {
			waves->createWave(sounds["clap"], x+dest.w/2, y+dest.h/2);
			setClap(true);
		}
	}

	void jump(){
		if (!inAir){
			inAir = true;
			vy = jumpSpeed;
			ay = GRAVITY;
		
			if (vx<0)
				setAnimation(animations["jumpLeft"]);
			else
				setAnimation(animations["jumpRight"]);
			waves->createWave(sounds["footstep"], x+32, y+32);
		}
	}

	void collisions(vector<Tile *> tiles) {	
		SDL_Rect temp = dest;
		temp.h+=2;
		for (auto &t:tiles) {
      if (t->collide(&dest)){
				hitTile(t);
				if (t->getType() == "floor" && inAir) { stopFalling(); }
      	else if (t->getType() == "ceiling"){ setVY(0); }
      }
		}
		for (auto &t:tiles) {
			if (t->collide(&temp)) {
				onTile=true;
				break;
			}
			onTile=false;
    }
		if (!onTile) {
			ay=GRAVITY;
			inAir=true;
		}
	}

	virtual void update(double dt){
		Particle::update(dt);

		if(timeMoving >= 1000 && !inAir){
			timeMoving%=500;
			if(dir==LEFT)
				waves->createWave(sounds["footstep"], x, y+dest.h);
			else
				waves->createWave(sounds["footstep"], x+dest.w/2, y+(dest.h-3));
		}
 
		if(vx!=0)
			timeMoving += (int)(dt*1000.0);

		a->update(dt);
		dest.x = x;
		dest.y = y;
	}

	virtual void render(){
		SDL_RenderCopy(ren, a->getTexture(), a->getFrame(), &dest);
	}

	~Character(){
		for(auto a:animations) delete a.second;
		for(auto s:sounds) delete s.second;
	}
};