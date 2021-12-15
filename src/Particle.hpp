#pragma once

#include "Animation.hpp"

#define PI 3.14159265

using namespace std;

class Particle {
	protected:
	double x, y, vx, vy, ax, ay, v, damp;
	int minx, miny, maxx, maxy, theta;
	bool isCartesian;        

	public:
	Particle(double newx=0.0, double newy=0.0,
			 double newv=0.0, int newtheta=0,
			 double newax=0.0, double neway=0.0,
			 double newdamp=1.0, bool newIsCartesian=false) {
		
		v = newv; theta = newtheta;


		x = newx; y = newy;

		vx = newv*cos(newtheta*PI/180); // px/s
		vy = newv*sin(newtheta*PI/180); // px/s

		ax = newax; ay = neway; // px/s/s
		
		damp=newdamp;
		isCartesian = newIsCartesian;
		setBound();
	}

	void setBound(int newMinX=-32, int newMinY=-32, int newMaxX=1312, int newMaxY=752) {
		minx=newMinX;
		miny=newMinY;
		maxx=newMaxX;
		maxy=newMaxY;
	}
	
	void setX(double newX) { x=newX; }
	double getX() { return x; }

	void setY(double newY) { y=newY; }
	double getY() { return y; }

	getMaxY(){ return maxy; }

	void setVY(double newVY) { vy=newVY; }

	void updatePolar(double dt){

		vx=v*cos(theta*PI/180); 
		vy=v*sin(theta*PI/180);

		vx+=ax*dt; vy+=ay*dt;
		x+=vx*dt; y+=vy*dt;
	}

	void updateCartesian(double dt){
		vx+=ax*dt; vy+=ay*dt;
		x+=vx*dt; y+=vy*dt;
	}

	virtual SDL_Rect *getDest(){ 
		SDL_Rect *returnVal = new SDL_Rect();
		returnVal->x = x;
		returnVal->y = y;
		returnVal->w = 1;
		returnVal->h = 1;
		return returnVal;
	}
	
	void swap(double &a, double &b) {
		double temp;
		temp = a;
		a = b;
		b = temp;
	}

	void sort(double r[]) {
		for(int j=0; j<3; j++)
			for(int i=0; i<3; i++)
				if(r[i] > r[i+1])
					swap(r[i], r[i+1]);
	}

	bool collide(Particle *newP) {
		SDL_Rect *myBox = getDest();
		SDL_Rect *otherBox = newP->getDest();

		bool hasCollision = SDL_HasIntersection(myBox, otherBox);
		if(hasCollision) {
			double smallest, secondSmallest;

			double r1 = sqrt(pow(otherBox->x-x,2)+pow(otherBox->y-y,2));
			double r2 = sqrt(pow(otherBox->x+otherBox->w-x,2)+pow(otherBox->y-y,2));
			double r3 = sqrt(pow(otherBox->x+otherBox->w-x,2)+pow(otherBox->y+otherBox->h-y,2));
			double r4 = sqrt(pow(otherBox->x-x,2)+pow(otherBox->y+otherBox->h-y,2));

			double r[4]={r1,r2,r3,r4};

			sort(r);

			smallest = r[0];
			secondSmallest = r[1];

			if ((smallest==r2 && secondSmallest==r3) || (smallest==r3 && secondSmallest==r2)) { 
				if (180<=theta && theta<270)
					theta=540-theta;
				else 
					theta=180-theta;
				theta%=360;
				x=otherBox->x+otherBox->w;
			}
			if ((smallest==r1 && secondSmallest==r4) || (smallest==r4 && secondSmallest==r1)) { 
				if (270<theta && theta<=360)
					theta=540-theta;
				else 
					theta=180-theta;
				theta%=360;
				x=otherBox->x;
			}
			if ((smallest==r3 && secondSmallest==r4) || (smallest==r4 && secondSmallest==r3)) {
				theta=360-theta;
				y=otherBox->y+otherBox->h;
			}
			if ((smallest==r1 && secondSmallest==r2) || (smallest==r2 && secondSmallest==r1)) { 
				theta=360-theta;
				y=otherBox->y-myBox->h;
			}
			vx=v*cos(theta*PI/180); 
			vy=v*sin(theta*PI/180);
		}
		delete myBox;
		return hasCollision;
	}

	virtual void update(double dt) {
		if (maxx!=minx) {
			if (x<=minx) { 
				if (180<= theta && theta <270)
					theta=540-theta;
				else 
					theta=180-theta;
				theta%=360;
				x=minx;
			}
			if (x>=maxx) { 
				if (270<theta && theta<=360)
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

		if (isCartesian) updateCartesian(dt);
		else updatePolar(dt);
	}
};