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

class Tile:public Particle{
  Config *cfg;
  SDL_Renderer *ren;
  MediaManager *media;

  map<string,Animation *> animations;
  map<string, Mix_Chunk *> sounds;

  string tileType;
  
protected:
  Animation *a;
  SDL_Rect dest;
  Waves *waves;
  double x, originalY, w, h;
  SDL_Point center;

public:
  Tile(MediaManager *newMedia, SDL_Renderer *newRen, Config *newCfg, string newType,
  double newx=0.0, double newy=0.0,
  double newv=0.0, int newtheta=0,
  double newax=0.0, double neway=0.0,
  double newdamp=0.0):Particle(newx, newy, newv, newtheta, newax, neway, newdamp) {

    ren=newRen;
    media=newMedia;
    cfg=newCfg;
    
    dest.w = stoi((*cfg)["width"]);
    dest.h = stoi((*cfg)["height"]);
    w=dest.w;
    h=dest.h;
    x=newx;
    originalY=newy;
    center={dest.w/2,dest.h/2};

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
    
    tileType = newType;

    if (tileType == "floor") setFloor();
    else if (tileType == "ceiling") setCeiling();
    else if (tileType == "lWall") setLWall();
    else if (tileType == "rWall") setRWall();
  }

  SDL_Rect *getDest() { return &dest; }
  string getType() { return tileType; }
  
  Animation *getAnimation() { return a; }
  void setAnimation(Animation *newA) { a=newA; }

  void setFloor() { setAnimation(animations["floor"]); y+=dest.h; }
  void setCeiling() { setAnimation(animations["ceiling"]); }
  void setLWall() { setAnimation(animations["lWall"]); }
  void setRWall() { setAnimation(animations["rWall"]); }
  
  void lightUp() { a->setTransparency(255); }

  bool collide(SDL_Rect* pDest) {
    SDL_bool collision = SDL_HasIntersection(&dest, pDest);
    if (collision)  {
      lightUp();
      return true;
    }
    return false;
  }

  void update(double dt) {
    if(a->getTransparency() > 0) a->decTransparency(1);
    a->update(dt);
    dest.x = x;
    dest.y = y;
  }

  void render(){
    SDL_RenderCopy(ren, a->getTexture(), a->getFrame(), &dest);
  }

  double getx(){
    return x;
  }
  double gety(){
    return originalY;
  }
  double geth(){
    return h;
  }
  double getw(){
    return w;
  }
  double centerx(){
    return center.x;
  }
  double centery(){
    return center.y;
  }
  bool inside(int x, int y)
	{
		return (dest.x <= x && x <= dest.x + dest.w &&
				dest.y <= originalY && originalY <= dest.y + dest.h);
	}
  ~Tile() {
    for (auto a:animations) delete a.second;
    for (auto s:sounds) delete s.second;
  }
};