#pragma once

using namespace std;

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