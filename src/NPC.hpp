#pragma once

#include <map>

#include "Particle.hpp"
#include "Wave.hpp"
#include "Animation.hpp"

using namespace std;

class Npc:public Character{
  map<string,Animation *> animations;
  map<string,Mix_Chunk *> sounds;

  direction currMove;
  int time;

  public:
  Npc(MediaManager *newMedia, SDL_Renderer *newRen, Waves* newWaves, Config *npcConf,
		double newx=0.0, double newy=0.0,
		double newv=0.0, int newtheta=0,
		double newax=0.0, double neway=0.0,
		double newdamp=0.0):Character(newMedia, newRen, newWaves, npcConf, newx, newy, newv, newtheta, newax, neway, newdamp){
    
    currMove=STOP;
    time=0;
  }

  void killed() {
    waves->createWave(sounds["clap"],x+dest.w/2,y+dest.h/2);
  }

  bool collide(SDL_Rect* pDest) {
    SDL_bool collision = SDL_HasIntersection(&dest, pDest);
    if (collision)  {
      killed();
      return true;
    }
    return false;
  }
 
  void ai(double dt, double playerX) {
    if (x>playerX-1 && x<playerX+1) stopMoving();
    else if (x < playerX) moveRight();
    else moveLeft();
  }

  void update(double dt, double playerX) {
    ai(dt, playerX);
    Character::update(dt);
  }

  ~Npc() {}
};