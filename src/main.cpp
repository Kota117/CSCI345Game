#include <iostream>
#include <SDL.h>
#include <SDL_mixer.h>
#include <vector>
#include <map>
#include <math.h>

#include "Exception.hpp"
#include "MediaManager.hpp"
#include "Game.hpp"
#include "Particle.hpp"
#include "Animation.hpp"
#include "Wave.hpp"

using namespace std;

class Player:public Particle{
	double walkSpeed;

	Animation playerAnim;
	Waves *waves;
	Mix_Chunk *footstepSound;

	bool moving;
	int timeMoving;

	public:
	Player(SDL_Renderer *newRen, Animation *newA, Waves *newWaves,
		Mix_Chunk *newFootstepSound,
		double newx=0.0, double newy=0.0,
		double newvx=0.0, double newvy=0.0,
		double newax=0.0, double neway=0.0,
		double newdamp=0.0):Particle(newRen, newA, 
							newx, newy, newvx, newvy, newax, neway, newdamp){

		//The above is constructing a Player object as a particle with some default params
		
		//the placement of these should be better thought out!
		dest.w=64;
		dest.h=64;

		moving=false;
		timeMoving=0;
		walkSpeed = 50.0;

		waves = newWaves;
		footstepSound = newFootstepSound;
	}

	//Put some registered handlers down here

	void walkRight(){
		v = walkSpeed;
		moving = true;
		//setAnimation("media/walkRight.txt");
	}

	void walkLeft(){
		v = -walkSpeed;
		moving = true;
	}

	void stopMoving(){
		vx=0;
		timeMoving=0;
	}

	void update(double dt){
		a->update(dt);

		if(timeMoving > 1000){
			timeMoving%=500;
			waves->createWave(footstepSound, x+32, y+64);
		}

		if(moving)
			timeMoving += (int)(dt*1000.0);

		vx=v*cos(theta*PI/180); 
		vy=v*sin(theta*PI/180);

		vx+=ax*dt; vy+=ay*dt;
		x+=vx*dt; y+=vy*dt;

		dest.x=(int)x;
		dest.y=(int)y;
		
		SDL_RenderCopy(ren, a->getTexture(), a->getFrame(), &dest);
	}

};

class MyGame:public Game {
	//These two variables are not useful for the game
	//These were put here to test wave mechanics and should be removed when player physics are added
	int totalTime;
	int waveStartX;

	Waves *waves;

	Animation playerWalkingRight, playerWalkingLeft;
	Player *player;

	Mix_Chunk *sample;

	public:
	MyGame(int w=640, int h=480):Game("Echos", w, h) {
		totalTime = 0.0;
		waveStartX = 0;
		waves = new Waves(media, ren);

		playerWalkingRight.read(media, "media/walkRight.txt");
		playerWalkingLeft.read(media, "media/walkLeft.txt");

		sample=media->readSound("media/footstep.wav");

		//ren, &playerWalkingRight, &waves, sample, 0, (h/2)-64, 50, 0, 0.0, 0.0, 0.0
		player = new Player(ren, &playerWalkingRight, waves, sample, 0, (480/2)-64, 0.0);

		player->walkRight();
	}

	void update(double dt) {
		SDL_RenderClear(ren);

		player->update(dt);

		waves->updateWaves(dt);
		 
		SDL_RenderPresent(ren);
	}

	// Do some event handling
	// On right key down, walk player right
	// On right key up, set velocity 0

	// mirror for left

	~MyGame() {

	}
};

int main(int argc, char* argv[]) {
    try {
		MyGame g;
		g.run();
	} catch (Exception e) {
		cerr << e;
	}
    return 0;
}