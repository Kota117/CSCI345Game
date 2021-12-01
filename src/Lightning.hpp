#pragma once

#include <map>
#include <SDL_mixer.h>
#include <SDL.h>

#include "Particle.hpp"
#include "Animation.hpp"
#include "MediaManager.hpp"
#include "Config.hpp"

using namespace std;

class Lightning:public Particle{
  Config *cfg;
  SDL_Renderer *ren;
  MediaManager *media;

  map<string,Animation *> animations;
  map<string, Mix_Chunk *> sounds;
  
protected:
  Animation *a;
  SDL_Rect dest;

public:
  Lightning(MediaManager *newMedia, SDL_Renderer *newRen, Config *newCfg,
  double newx=0.0, double newy=0.0,
  double newv=0.0, int newtheta=0,
  double newax=0.0, double neway=0.0,
  double newdamp=0.0):Particle(newx, newy, newv, newtheta, newax, neway, newdamp) {

    ren=newRen;
    media=newMedia;
    cfg=newCfg;
    
    dest.w = stoi((*cfg)["width"]);
    dest.h = stoi((*cfg)["height"]);

    vector<string> newAnimations = cfg->getMany("animations");
		for(auto anim: newAnimations){
			animations[anim] = new Animation(0);
			animations[anim]->readAnimation(media, anim);
		}

		a=animations[(*cfg)["defaultAnimation"]];
		
		vector<string> newSounds = cfg->getMany("sounds");
		for(auto sound: newSounds){
			sounds[sound] = new Mix_Chunk();
			sounds[sound] = media->readSound(sound);
    }

    y = 0;
    a->setTransparency(0);
  }

  SDL_Rect *getDest() { return &dest; }
  
  Animation *getAnimation() { return a; }
  void setAnimation(Animation *newA) { a=newA; }

  void update(double dt, bool flashed=0, int newX=0) {
    if (flashed) {
      x=newX;
      a->setTransparency(255);
    }
    if(a->getTransparency() > 0) a->decTransparency(5);
    a->update(dt);
    dest.x = x;
    dest.y = y;
  }

  void render(){
    SDL_RenderCopy(ren, a->getTexture(), a->getFrame(), &dest);
  }

  ~Lightning() {
    for (auto a:animations) delete a.second;
    for (auto s:sounds) delete s.second;
  }
};