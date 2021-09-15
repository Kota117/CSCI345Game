#include <iostream>
#include <SDL.h>

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

class Game {
    protected:
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
	    // SDL_Delay(100);
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
	SDL_Renderer *ren;
	SDL_Texture *bitmapTex;
	SDL_Rect *src;
	SDL_Rect dest;
	double x, y, vx, vy, ax, ay;
	public:
	Particle(SDL_Renderer *newRen, SDL_Texture *newBitmapTex,
			 SDL_Rect *newSrc,
			 double newx=0.0, double newy=0.0,
			 double newvx=0.0, double newvy=0.0,
			 double newax=0.0, double neway=0.0) {
		src=newSrc;
		ren=newRen;
		bitmapTex=newBitmapTex;
		dest.w = src->w;
		dest.h = src->h;
		dest.x = newx;
		dest.y = newy;
		x = newx;
		y = newy;
		vx = newvx; // px/s
		vy = newvy; // px/s
		ax = newax;
		ay = neway; // px/s/s
	}
	void update(double dt) {
		vx+=ax*dt; vy+=ay*dt;
		x+=vx*dt; y+=vy*dt;
		dest.x=(int)x;
		dest.y=(int)y;
		SDL_RenderCopy(ren, bitmapTex, src, &dest);
	}
};

class MyGame:public Game {
        SDL_Texture *bitmapTex;
		SDL_Rect src;
		Particle *p;
	public:
	MyGame(int w=640, int h=480):Game("An SDL2 window", w, h) {
		SDL_Surface *ob;
		ob = SDL_LoadBMP("media/image.bmp");
    	if (ob == NULL) throw Exception("Could not load media/image.bmp");
		src.w = ob->w;
		src.h = ob->h;
		src.x = 0;
		src.y = 0;
		bitmapTex = SDL_CreateTextureFromSurface(ren, ob);
		if (bitmapTex == NULL) throw Exception("Could not create texture");
		SDL_FreeSurface(ob);
		
		p = new Particle(ren, bitmapTex, &src, 100, 100, 10, 10, 0, -5);
	}
	void update(double dt) {
		
		SDL_RenderClear(ren);
		p->update(dt);
		SDL_RenderPresent(ren);
	}
	~MyGame() {
	    SDL_DestroyTexture(bitmapTex);
	}
};
int main(int argc, char* argv[]) {

    //Had to change to this loop instead of delay to see window on M1

    try {
		MyGame g;
		g.run();
	} catch (Exception e) {
		cerr << e;
	}
    return 0;
}