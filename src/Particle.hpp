#pragma once

#include "Animation.hpp"

#define PI 3.14159265

using namespace std;

class Particle {
	SDL_Renderer *ren;
	Animation *a;
	SDL_Rect dest;
	double x, y, vx, vy, ax, ay, v, damp;
	int minx, miny, maxx, maxy, theta;

	public:
	Particle(SDL_Renderer *newRen, Animation *newA,
			 double newx=0.0, double newy=0.0,
			 double newv=0.0, int newtheta=0,
			 double newax=0.0, double neway=0.0,
			 double newdamp=1.0) {
		
		ren=newRen;
		a=newA;
		
		dest.w = 8;
		dest.h = 8;
		
		dest.x = newx;
		dest.y = newy;
		
		v = newv; theta = newtheta;

		x = newx; y = newy;

		vx = newv*cos(newtheta*PI/180); // px/s
		vy = newv*sin(newtheta*PI/180); // px/s

		ax = newax; ay = neway; // px/s/s
		
		damp=newdamp;
		setBound();
	}

	void setBound(int newMinX=0, int newMinY=0, int newMaxX=0, int newMaxY=0) {
		minx=newMinX;
		miny=newMinY;
		maxx=newMaxX;
		maxy=newMaxY;
	}

	void setAnimation(Animation *newA){ a=newA; }
	
	void update(double dt) {

		if (maxx!=minx) {
			if (x<=minx) { 
				if (180<=theta<270)
					theta=540-theta;
				else 
					theta=180-theta;
				theta%=360;
				x=minx;
			}
			if (x>=maxx) { 
				if (270<theta<=360)
					theta=540-theta;
				else 
					theta=180-theta;
				theta%=360;
				x=maxx;
			}
	  	}
	  	if (maxy!=miny) {
			if (y<=miny) {
				theta=360-theta;
				y=miny;
			}
			if (y>=maxy) { 
				theta=360-theta;
				y=maxy;
			}
      	}

		vx=v*cos(theta*PI/180); 
		vy=v*sin(theta*PI/180);

		vx+=ax*dt; vy+=ay*dt;
		x+=vx*dt; y+=vy*dt;

		dest.x=(int)x;
		dest.y=(int)y;

		a->update(dt);
		
		SDL_RenderCopy(ren, a->getTexture(), a->getFrame(), &dest);
	}
};