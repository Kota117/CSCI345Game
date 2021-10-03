#include <iostream>
#include <SDL.h>
#include <SDL_mixer.h>
#include <vector>
#include <map>
#include <math.h>
#include <map>

#include "Exception.hpp"
#include "MediaManager.hpp"
#include "Game.hpp"
#include "Particle.hpp"
#include "Animation.hpp"
#include "Wave.hpp"

using namespace std;

class Player:public Particle{
	double walkSpeed;

	map<string,Animation *> playerAnimations;
	Waves *waves;
	Mix_Chunk *footstepSound;

	bool moving;
	int timeMoving;

	public:
	Player(SDL_Renderer *newRen, map<string,Animation *> newPlayerAnimations, Animation* startingAnimation, Waves *newWaves,
		Mix_Chunk *newFootstepSound,
		double newx=0.0, double newy=0.0,
		double newvx=0.0, double newvy=0.0,
		double newax=0.0, double neway=0.0,
		double newdamp=0.0):Particle(newRen, startingAnimation, 
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
		playerAnimations = newPlayerAnimations;
	}

	//Put some registered handlers down here

	void walkRight(){
		v = walkSpeed;
		theta = 0;
		moving = true;
		setAnimation(playerAnimations["walkRight"]);
	}

	void walkLeft(){
		v = walkSpeed;
		theta = 180;
		moving = true;
		setAnimation(playerAnimations["walkLeft"]);
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

	Player *player;
	map<string,Animation *> playerAnimations;

	Mix_Chunk *sample;


	void initPlayer(){
		playerAnimations["walkRight"] = new Animation();
		playerAnimations["walkRight"]->read(media, "media/walkRight.txt");

		playerAnimations["walkLeft"] = new Animation();
		playerAnimations["walkLeft"]->read(media, "media/walkLeft.txt");

		playerAnimations["idle"] = new Animation();
		playerAnimations["idle"]->read(media, "media/idle.txt");

		sample=media->readSound("media/footstep.wav");

		player = new Player(ren, playerAnimations, playerAnimations["idle"], waves, sample, 320, (480/2)-64);
	}

	public:
	MyGame(int w=640, int h=480):Game("Echos", w, h) {
		totalTime = 0.0;
		waveStartX = 0;
		waves = new Waves(media, ren);

		initPlayer();
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