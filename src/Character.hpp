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
	bool clapped, inAir, onTile, hasKey, unlocked, hasLeft;

	protected:
	Animation *a;
	SDL_Rect dest;
	Waves *waves;

	public:
	Character(MediaManager *newMedia, SDL_Renderer *newRen, Waves *newWaves, Config *newCfg,
		double newx=0.0, double newy=0.0,
		double newv=0.0, int newtheta=0,
		double newax=0.0, double neway=0.0,
		double newvx=0.0, double newvy=0.0,
		double newdamp=0.0):Particle(newx, newy, newv, newtheta, newax, neway, newdamp, true){
		

		ren = newRen;
		media = newMedia;
		cfg = newCfg;
		waves = newWaves;
		timeMoving = 0;
		inAir = true;
		dir = STOP;
		clapped = false;
		onTile = true;
		hasKey = false;
		unlocked = false;
		hasLeft = false;
		vx = newvx;
		vy = newvy;
		

		dest.w = stoi((*cfg)["width"]) * stoi((*cfg)["scale"]);
		dest.h = stoi((*cfg)["height"]) * stoi((*cfg)["scale"]);

		y = newy-dest.h;

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
	bool isMoving(){ return vx!=0 || vy != 0; }
	SDL_Rect *getDest(){ return &dest; }
	bool isOnTile(){ return onTile; }

	//Basic Setters
	void setClap(bool x){ clapped = x; }
	void setAnimation(Animation *newA){ a = newA; }

	void moveRight(){
		if (dir!=RIGHT){
			dir = RIGHT;
			vx = baseSpeed;
			theta = 0;

			if(onTile){
				waves->createWave(sounds["footstep"], x+dest.w/2, y+dest.h);
			}

			setAnimation(animations["walkRight"]);
		}
	}

	void moveLeft(){
		if (dir!=LEFT){
			dir = LEFT;
			vx = -baseSpeed;
			theta = 180;

			if(onTile){
				waves->createWave(sounds["footstep"], x+dest.w/4, y+dest.h);
			}

			setAnimation(animations["walkLeft"]);
		}
	}

	void stopMoving(){
		if (dir!=STOP){
			dir = STOP;
			vx = 0;
			timeMoving = 0;

			setAnimation(animations[(*cfg)["defaultAnimation"]]);
		}
	}

	void setHasLeft(bool setter){
		hasLeft = setter;
	}
	void setHasKey(bool setter){
		hasKey = setter;
	}
	void collectedKey(){
		hasKey = true;
		Mix_PlayChannel(-1,sounds["key"],0);
	}
	bool leftTheBuilding(){
		return hasLeft;
	}
	void setUnlocked(bool setter){
		unlocked = setter;
	}
	bool unlockedDoor(){
		return unlocked;
	}

	void stopFalling(){
		inAir = false;
		ay = 0;
		timeMoving = 0;

		if (vx>0){
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

	void clap(){
		if (!clapped){
			waves->createWave(sounds["clap"], x+dest.w/2, y+dest.h/2);
			setClap(true);
		}
	}

	void setVy(double newVy){
		vy = newVy;
	}

	void jump(){
		dest.y -= 4;
		vy = jumpSpeed;
		onTile = false;

		if (vx<0)
			setAnimation(animations["jumpLeft"]);
		else
			setAnimation(animations["jumpRight"]);
	}

	void collisions(vector<Tile *> tiles){
		SDL_Rect topBox, bottomBox, leftBox, rightBox;
		topBox.y = dest.y;
		topBox.x = dest.x+5;
		topBox.h = 5;
		topBox.w = dest.w-10;
		bottomBox.x = dest.x+5;
		bottomBox.y = dest.y+dest.h-7;
		bottomBox.h = 10;
		bottomBox.w = dest.w-10;
		leftBox.x = dest.x-2;
		leftBox.y = dest.y+4;
		leftBox.w = 8;
		leftBox.h = dest.h-8;
		rightBox.x = dest.x+dest.w-6;
		rightBox.y = dest.y+4;
		rightBox.w = 8;
		rightBox.h = dest.h-8;

		for(auto &t:tiles){
			if(t->collide(&topBox) && vy < 0){
				if(!(t->getType()=="door")){
					setVy(0);
				}
				else if(!hasLeft && hasKey){
					Mix_PlayChannel(-1,sounds["door"],0);
					unlocked = true;
				}
			}

			else if(t->collide(&leftBox) && vx < 0){
				if(!(t->getType()=="door")){
					x = t->getX()+t->getW()+1;
					vx = 0;
				}
				else if(!hasLeft && hasKey){
					Mix_PlayChannel(-1,sounds["door"],0);
					unlocked = true;
				}
			}
			else if(t->collide(&rightBox) && vx > 0){
				if(!(t->getType()=="door")){
					x = t->getX()-dest.w-1;
					vx = 0;
				}
				else if(!hasLeft && hasKey){
					Mix_PlayChannel(-1,sounds["door"],0);
					unlocked = true;
				}
			}
			else if(t->collide(&bottomBox)){
				if(!(t->getType()=="door")){
					if(vy>0) waves->createWave(sounds["footstep"], x, y+(dest.h-3));
					y = t->getY()-dest.h;
					onTile = true;
					break;
				}
				else if(!hasLeft && hasKey){
					Mix_PlayChannel(-1,sounds["door"],0);
					unlocked = true;
				}
			}
			onTile = false;
		}
		if (!onTile){
			ay = GRAVITY;
			inAir = true;
		}
		else if (onTile){
			ay = 0;
			vy = 0;
			inAir = false;
		}
	}
	

	virtual void update(double dt){
		Particle::update(dt);

		if(dir==LEFT && isOnTile()){
			setAnimation(animations["walkLeft"]);
			if(timeMoving >= 1000){
				timeMoving %= 500;
				waves->createWave(sounds["footstep"], x, y+dest.h);
			}
		}else if(dir==RIGHT && isOnTile()){
			setAnimation(animations["walkRight"]);
			if(timeMoving >= 1000){
				timeMoving %= 500;
				waves->createWave(sounds["footstep"], x+dest.w/2, y+(dest.h-3));
			}
		}else if(isOnTile()){
			setAnimation(animations[(*cfg)["defaultAnimation"]]);
		}


		if(vx!=0) timeMoving += (int)(dt*1000.0);
		
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