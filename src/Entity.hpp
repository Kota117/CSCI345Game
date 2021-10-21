#pragma once

#include <map>

#include "Particle.hpp"
#include "Wave.hpp"
#include "Animation.hpp"

using namespace std;

enum direction{LEFT, RIGHT, STOP};

class Entity:public Particle{
  map<string,Animation *> animations;
  map<string,Mix_Chunk *> sounds;

  Waves *waves;

  int timeMoving, movement, time;
  double speed;
  string entityType;
  bool moving, alive;

  public:
  Entity(SDL_Renderer *newRen, map<string,Animation *> newAnimations, Animation* startingAnimation, Waves *newWaves,map<string,Mix_Chunk *> newSounds, string newEntityType,
		double newx=0.0, double newy=0.0,
		double newv=0.0, int newtheta=0,
		double newax=0.0, double neway=0.0,
		double newdamp=0.0):Particle(newRen, startingAnimation, 
              newx, newy, newv, newtheta, newax, neway, newdamp) {
    dest.w=64;
    dest.h=64;

    timeMoving=0;
    speed=25.0;
    moving=0;
    alive=1;
    time=0;
    
    waves = newWaves;
    sounds = newSounds;
    animations = newAnimations;
    entityType = newEntityType;
  }

  void moveRight() {
    v = speed;
    theta = 0;
    waves->createWave(sounds["footstep"],x+dest.w/2+8,y+dest.h);
    setAnimation(animations["walkRight"]);
  }

  void moveLeft() {
    v = speed;
    theta = 180;
    waves->createWave(sounds["footstep"],x+dest.w/2-8,y+dest.h);
    setAnimation(animations["walkLeft"]);
  }

  void stopMoving() {
    v = 0;
    timeMoving = 0;
    setAnimation(animations["idle"]);
  }

  void clap(){
		waves->createWave(sounds["clap"],x+dest.w/2,y+dest.h/2);
	}

  void killed() {
    // create new sound for entity death
    waves->createWave(sounds["clap"],x+dest.w/2,y+dest.h/2);
    dest.x=0;
    dest.y=0;
    dest.w=0;
    dest.h=0;
    alive=0;
  }

  void move(direction dir, int ms, double dt) {
    if (time == 0) {
      moving=1;
      if (dir==LEFT) moveLeft();
      else if (dir==RIGHT) moveRight();
      else if (dir==STOP) stopMoving();
    }
    time += (int)(1000*dt);
    if (time > ms) {
      stopMoving();
      time=0;
      moving=0;
    }
  }

  void ai(double dt) {
    if (!moving) {
      movement = rand()%3;
      if (movement==0) move(LEFT, 2000, dt);
      else if (movement==1) move(RIGHT, 1000, dt);
      else if (movement==2) move(STOP, 500, dt);
    } else {
      if (movement==0) move(LEFT, 2000, dt);
      else if (movement==1) move(RIGHT, 1000, dt);
      else if (movement==2) move(STOP, 1500, dt);
    }
    
  }

  void update(double dt) {
    if (alive) {
      ai(dt);
      a->update(dt);

      if(timeMoving >= 1000){
        timeMoving%=500;
        if(v<0)
          waves->createWave(sounds["footstep"],x+dest.w/2+8,y+dest.h);
        else
          waves->createWave(sounds["footstep"],x+dest.w/2-8,y+dest.h);
      }
    
      if(v!=0)
        timeMoving += (int)(dt*1000.0);

      vx=v*cos(theta*PI/180); 
      vy=v*sin(theta*PI/180);

      vx+=ax*dt; vy+=ay*dt;
      x+=vx*dt; y+=vy*dt;

      dest.x=(int)x;
      dest.y=(int)y;
		  SDL_RenderCopy(ren, a->getTexture(), a->getFrame(), &dest);
    }
  }

  bool isMoving() { return v!=0; }

  ~Entity() {
    for(auto a:animations) delete a.second;
    for(auto s:sounds) delete s.second;
  }
};