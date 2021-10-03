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
		SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO); 
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
		
		if (Mix_Init(0)!=0) throw Exception("Mixer Error");
        if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024)==-1) throw Exception("Open Audio Failed");

		media=new MediaManager(ren);
		
		ticks=SDL_GetTicks();
	}

    void run () {
		int newTicks;
		bool is_running = true;
        SDL_Event e;
        
		while (is_running) {
            newTicks=SDL_GetTicks();
            while (SDL_PollEvent(&e)) {
                if (e.type == SDL_QUIT) 
                    is_running = false;
                else if (e.type==SDL_KEYDOWN) handleKeyDown(e);
          		else if (e.type==SDL_KEYUP) handleKeyUp(e);
            }
			update(double(newTicks-ticks)/1000.0);
			ticks=newTicks;
        }
	}

    virtual void update(double dt/*s of elapsed time*/)=0;

	virtual void handleKeyUp(SDL_Event key)=0;
	virtual void handleKeyDown(SDL_Event key)=0;
    
	~Game(){
    	SDL_DestroyRenderer(ren);
    	SDL_DestroyWindow(window);
    	SDL_Quit();	
	}
};