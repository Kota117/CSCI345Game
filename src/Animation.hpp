#pragma once

#include <vector>
#include <fstream>

using namespace std;

class AnimationFrame{
	SDL_Rect frame;
	int millis;
    
	public:
	AnimationFrame(int newMillis, int newX, int newY, int newW, int newH) {
		millis=newMillis;
		frame.x = newX;
		frame.y = newY;
		frame.w = newW;
		frame.h = newH;
	}

	int getMillis() { return millis; }

	SDL_Rect *getFrame() { return &frame; }
};

class Animation{
	SDL_Texture *spriteSheet;
	vector<AnimationFrame*> frames;

	int totalTime;
	int currentTime;

	string sheetName;
	string animationFile;

	public:
	Animation() { 
	  totalTime=0;
	  currentTime=0;
	}

	void read(MediaManager *media,string newAnimationFile) {
		if(newAnimationFile != animationFile){
			animationFile = newAnimationFile;

			int max;
			ifstream in;

			in.open(animationFile);

			in >> max >> sheetName;
			spriteSheet=media->read("media/"+sheetName+".bmp");
			
			int millis,x,y,w,h;
			for (int i=0;i<max;i++) {
				in >> millis >> x >> y >> w >> h;
				AnimationFrame *af=new AnimationFrame(millis, x, y, w, h);
				
				frames.push_back(af);
				totalTime+=af->getMillis(); 
			}
			in.close();
		}	
	}

	void update(double dt) {
		currentTime+=(int)(dt*1000.0);
		//cout << "Total Time " << totalTime << endl;
		currentTime%=totalTime;
	}

	SDL_Rect *getFrame() {
		int checkTime=0;
		int t=0;
		for (t=0;t<frames.size();t++) {
			if (checkTime+frames[t]->getMillis()>currentTime) 
			  break;
			checkTime+=frames[t]->getMillis();
		}
		if (t==frames.size()) t=0;
		return frames[t]->getFrame();
	}

	SDL_Texture *getTexture(){ return spriteSheet; }

	~Animation() {
		//cout << "Destroying Animation" << endl;
		for (auto f:frames) 
		  delete f;
	}
};