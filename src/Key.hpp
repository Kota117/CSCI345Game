#pragma once

#include <map>

#include "Particle.hpp"
#include "Wave.hpp"
#include "Animation.hpp"

using namespace std;

class Key:public Character{
    map<string,Animation *> animations;
    map<string,Mix_Chunk *> sounds;

    direction currMove;
    int time;

    public:
    Key(MediaManager *newMedia, SDL_Renderer *newRen, Waves* newWaves, Config *keyConf,
        double newx=0.0, double newy=0.0,
        double newv=0.0, int newtheta=0,
        double newax=0.0, double neway=0.0,
        double newdamp=0.0):Character(newMedia, newRen, newWaves, keyConf, newx, newy, newv, newtheta, newax, neway, newdamp){
        
        currMove = STOP;
        time = 0;
    }

    bool collide(SDL_Rect* pDest){
        SDL_bool collision = SDL_HasIntersection(&dest, pDest);
        
        if (collision){
            return true;
        }

        return false;
    }

    void update(double dt, double playerX){
        Character::update(dt);
    }

    ~Key(){}
};