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
	void read(MediaManager *media,ifstream &in) {
		
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

	public:
	Animation() { 
	  totalTime=0;
	  currentTime=0;
	}

	void read(MediaManager *media,string file) {
		int max;
		ifstream in;

		cout << file << endl;

		in.open(file);

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
		for (auto f:frames) 
		  delete f;
	}
};

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