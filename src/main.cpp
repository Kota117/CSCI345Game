#include <iostream>
#include <SDL.h>
#include <vector>

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
	}

    SDL_Rect getPosition(){
        return dest;
    }

	void update(double dt) {
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

	SDL_Surface *ob;
    
    public:
    Graphic(SDL_Renderer *newRen, 
            SDL_Rect newSrc,
            SDL_Rect newDest) {
        
        ren=newRen;
		
        src=newSrc;
        dest=newDest;

        dest.h = src.h;
        dest.w = src.w;

		ob = SDL_LoadBMP("media/Soundwave2white.bmp");
    	if (ob == NULL) throw Exception("Could not load media/image.bmp");
		
        bitmapTex = SDL_CreateTextureFromSurface(ren, ob);
        if (bitmapTex == NULL) throw Exception("Could not create texture");
		SDL_FreeSurface(ob);
	}

    void update(SDL_Rect newDest, double dt){
        dest=newDest;
        dest.h = src.h;
        dest.w = src.w;

		//the drawtime is used to animate the sprite
        drawTime += dt;

        if(drawTime > (1.0/20.0)){
            src.x = (src.x + 64) % (64*3);
            if(src.x==0)
                src.y = (src.y + 64) % (64*3);
            drawTime = 0;
        }

        SDL_RenderCopy(ren, bitmapTex, &src, &dest);
            
    }

    ~Graphic(){
        SDL_DestroyTexture(bitmapTex);
    }
};

class MyGame:public Game {
	vector <Particle*> particles;
    vector <Graphic*> g;
    SDL_Rect src, dest;

	public:
	MyGame(int w=640, int h=480):Game("An SDL2 window", w, h) {
        src.w = 64;
		src.h = 64;
		src.x = 0;
		src.y = 0;

        for(int i=0; i < 50; i++){
            double vy = rand() % 100 - 50;
			double vx = rand() % 100 - 50;
            particles.push_back(new Particle(100, 100, vx, vy, 0.0, -20.0));
            g.push_back(new Graphic(ren, src, dest));
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