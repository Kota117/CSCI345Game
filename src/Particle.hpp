#pragma once

#include "Animation.hpp"

using namespace std;

class Particle {
	SDL_Renderer *ren;
	Animation *a;
	SDL_Rect dest;
	double x, y, vx, vy, ax, ay, damp;
	int minx, miny, maxx, maxy;

	public:
	Particle(SDL_Renderer *newRen, Animation *newA,
			 double newx=0.0, double newy=0.0,
			 double newvx=0.0, double newvy=0.0,
			 double newax=0.0, double neway=0.0,
			 double newdamp=1.0) {
		
		ren=newRen;
		a=newA;
		
		dest.w = 8;
		dest.h = 8;
		
		dest.x = newx;
		dest.y = newy;
		
		x = newx; y = newy;
		vx = newvx; vy = newvy; // px/s
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
			if(x<=minx) { vx=-damp*vx; x=minx; }
			if(x>=maxx) { vx=-damp*vx; x=maxx; }
		}
		if (maxy!=miny) {
			if(y<=miny) { vy=-damp*vy; y=miny; }
			if(y>=maxy) { vy=-damp*vy; y=maxy; }
		}
		vx+=ax*dt; vy+=ay*dt;
		x+=vx*dt; y+=vy*dt;

		dest.x=(int)x;
		dest.y=(int)y;

		a->update(dt);
		
		SDL_RenderCopy(ren, a->getTexture(), a->getFrame(), &dest);
	}
};