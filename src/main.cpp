#include <iostream>
#include <SDL.h>
#include <vector>
#include <map>
#include <time.h>

#include <math.h>

#define PI 3.14159265

using namespace std;

class Exception {
    string description;
	public:
	Exception(string newDescription) {
		description = newDescription;
	}
    friend ostream & operator << (ostream &out, const Exception &e) {	
		return out << "Error: " << e.description << endl;
	}
};

class MediaManager {
	map<string,SDL_Texture *> images;
	SDL_Renderer *ren;
	public:
	MediaManager(SDL_Renderer *newRen) {
		ren=newRen;
	}
	SDL_Texture *read(string filename) {
		SDL_Texture *bitmapTex;
		if(images.find(filename)==images.end()) {
			SDL_Surface *ob;
			ob = SDL_LoadBMP(filename.c_str());
			if (ob == NULL) throw Exception("Could not load "+filename);
			bitmapTex = SDL_CreateTextureFromSurface(ren,ob);
			if (bitmapTex == NULL) throw Exception("Could not create texture");
			SDL_FreeSurface(ob);
			images[filename]=bitmapTex;
		}
		return images[filename];
	}
	~MediaManager() {
		for(auto i:images) {
	    	SDL_DestroyTexture(i.second);
		}
	}
};

class Game {
    protected:
	MediaManager *media;
	SDL_Window *window;
	SDL_Renderer *ren;
	int ticks; //ms ticks since start
    public:
    Game(string title, int w=640, int h=480){
		SDL_Init(SDL_INIT_VIDEO); 
		window = SDL_CreateWindow(
        	title.c_str(),                     // window title
        	SDL_WINDOWPOS_UNDEFINED,           // initial x position
        	SDL_WINDOWPOS_UNDEFINED,           // initial y position
        	w,                                 // width, in pixels
        	h,                                 // height, in pixels
        	SDL_WINDOW_OPENGL                  // flags - see below
        );
        if (window == NULL) throw Exception("Could not create window: ");
		ren = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
		if (ren == NULL) throw Exception("Could not create renderer");
		media=new MediaManager(ren);
		ticks=SDL_GetTicks();
	}
    void run () {
		int newTicks;
		bool is_running = true;
        SDL_Event event;
        while (is_running) {
            newTicks=SDL_GetTicks();
            while (SDL_PollEvent(&event)) {
                if (event.type == SDL_QUIT) {
                    is_running = false;
                }
            }
			update(double(newTicks-ticks)/1000.0);
			ticks=newTicks;
        }
	}
    virtual void update(double dt/*ms of elapsed time*/)=0;
	~Game(){
    	SDL_DestroyRenderer(ren);
    	SDL_DestroyWindow(window);
    	SDL_Quit();	
	}
};

class Particle {
	SDL_Rect dest;
	double x, y, vx, vy, ax, ay;
	int minx, miny, maxx, maxy;

	public:
	Particle(double newx=0.0, double newy=0.0,
			 double newvx=0.0, double newvy=0.0,
			 double newax=0.0, double neway=0.0) {

		dest.x = newx;
		dest.y = newy;
		x = newx;
		y = newy;
		vx = newvx; // px/s
		vy = newvy; // px/s
		ax = newax;
		ay = neway; // px/s/s
		setBound();
	}

    SDL_Rect getPosition(){
        return dest;
    }
	void setBound(int newMinX=0, int newMinY=0, int newMaxX=0, int newMaxY=0) {
		minx=newMinX;
		miny=newMinY;
		maxx=newMaxX;
		maxy=newMaxY;
	}
	void update(double dt) {
		if (maxx!=minx) {
			if(x<=minx) { vx=-vx; x=minx; }
			if(x>=maxx) { vx=-vx; x=maxx; }
		}
		if (maxy!=miny) {
			if(y<=miny) { vy=-vy; y=miny; }
			if(y>=maxy) { vy=-vy; y=maxy; }
		}
		vx+=ax*dt; vy+=ay*dt;
		x+=vx*dt; y+=vy*dt;
		dest.x=(int)x;
		dest.y=(int)y;
	}
};

class Graphic{
    SDL_Renderer *ren;
	SDL_Texture *bitmapTex;
	SDL_Rect src, dest;

    double drawTime;
	double animationSpeed;

	int drawnFrames;

	SDL_Surface *ob;
    
    public:
    Graphic(SDL_Renderer *newRen, 
            SDL_Rect newSrc,
            SDL_Rect newDest,
			float newAnimationSpeed=20.0) {
        
        ren=newRen;
		
		animationSpeed = newAnimationSpeed;
		drawnFrames = 0;

        src=newSrc;
        dest=newDest;

        dest.h = src.h;
        dest.w = src.w;

	//	ob = SDL_LoadBMP("media/Firework.bmp");
    //	if (ob == NULL) throw Exception("Could not load media/image.bmp");

		SDL_SetColorKey(ob, SDL_TRUE, SDL_MapRGB(ob->format, 0, 0, 0));
		
    //  bitmapTex = SDL_CreateTextureFromSurface(ren, ob);
    //  if (bitmapTex == NULL) throw Exception("Could not create texture");
	//	SDL_FreeSurface(ob);
	}

    void update(SDL_Rect newDest, double dt){
        dest=newDest;
        dest.h = src.h;
        dest.w = src.w;

		//the drawtime is used to animate the sprite
        drawTime += dt;

		if(drawnFrames < 9){
			if(drawTime > (1.0/animationSpeed)){
				src.x = (src.x + 64) % (64*3);
				if(src.x==0)
					src.y = (src.y + 64) % (64*3);
				drawTime = 0;
				drawnFrames++;
			}
		}
		
        SDL_RenderCopy(ren, bitmapTex, &src, &dest); 
    }

    ~Graphic(){
    //    SDL_DestroyTexture(bitmapTex);
    }
};

class MyGame:public Game {
	vector <Particle*> particles;
    vector <Graphic*> g;
    SDL_Rect src, dest;

	public:
	MyGame(int w=640, int h=480):Game("Firework Sim", w, h) {
		srand(time(0));
        src.w = 64;
		src.h = 64;
		src.x = 0;
		src.y = 0;

        for(int i=0; i < 360; i++){
            double vy = sin (i*PI/180) * 100;
			double vx = cos (i*PI/180) * 100;
			
			double speed = rand() % 20;
			SDL_Texture *bitmapTex=media->read("media/Firework.bmp");
            particles.push_back(new Particle(w/2, h/2, vx, vy, 0.0, 0.0));
            g.push_back(new Graphic(ren, src, dest, speed));
		 	particles[i]->setBound(0,0,w,h);
        }
	}

	void update(double dt) {
		SDL_RenderClear(ren);
        for(unsigned i=0; i<particles.size(); i++){
            particles[i]->update(dt);
            g[i]->update(particles[i]->getPosition(), dt);
        }
		SDL_RenderPresent(ren);
	}
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