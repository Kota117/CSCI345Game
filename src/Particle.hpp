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
			 double newdamp=1.0,
			 //The placement of these W and H params needs to be thought out better
			 //Maybe we should add a W/H getter method to the Animation/AnimationFrame class
			 int newW=8, int newH=8) {
		
		ren=newRen;
		a=newA;
		
		dest.w = newW;
		dest.h = newH;
		
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

	void setVX(double newVX) { vx=newVX; }
	double getVX() { return vx; }

	void setX(double newX) { x=newX; }
	double getX() { return x; }
	double getY() { return y; }
	
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

//PLAYER CLASS WIP

// class Player:public Particle{
// 	double walkSpeed = 5;

// 	Animation playerAnim;
// 	MediaManager *media;
// 	Waves *wavs;

// 	public:
// 	Player(MediaManager *media, SDL_Renderer *newRen, Animation *newA,
// 		Waves *newWavs,
// 		double newx=0.0, double newy=0.0,
// 		double newvx=0.0, double newvy=0.0,
// 		double newax=0.0, double neway=0.0,
// 		double newdamp=0.0):Particle(newRen, newA, 
// 							newx, newy, newvx, newvy, newax, neway, newdamp){

// 		//The above is constructing a Player object as a particle with some default params
// 		wavs = newWavs;
// 		playerAnim.read(media, "stand.txt");
// 	}

// 	//Put some registered handlers down here

// 	void setAnimation(string filePath){
// 		playerAnim.read(media, filePath);
// 	}

// 	void walkRight(){
// 		vx = walkSpeed;
// 		setAnimation("media/walkRight.txt");
// 		wavs->createWave(x, y);
// 	}

// 	void update(){
		
// 		Partice:update();
// 	}
	
// 	// double getX() { return x; }
// 	// double getY() { return y; }

// 	// void setVX(double newVX) { vx = newVX; }
// 	// void setVY(double newVY) { vy = newVY; }


// };