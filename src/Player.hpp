#pragma once

#include <map>

#include "Character.hpp"
#include "Wave.hpp"
#include "Animation.hpp"
#include "Config.hpp"

using namespace std;

//This player class is not very useful right now. It does the exact same thing as the character but this could change
class Player:public Character{
	public:
	Player(MediaManager *newMedia, SDL_Renderer *newRen, Waves *newWaves, Config *playerConf,
		double newx=0.0, double newy=0.0,
		double newv=0.0, int newtheta=0,
		double newax=0.0, double neway=0.0,
		double newdamp=0.0):Character(newMedia, newRen, newWaves, playerConf, newx, newy, newv, newtheta, newax, neway, newdamp){}

	~Player(){}
};