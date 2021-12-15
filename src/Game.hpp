#pragma once

using namespace std;

class Game {
    protected:
	MediaManager *media;
	SDL_Window *window;
	SDL_Renderer *ren;
	int ticks; //ms ticks since start
	bool is_running;

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
		if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024)==-1) throw Exception("Open Audio Failed");

		media = new MediaManager(ren);
		
		ticks = SDL_GetTicks();
	}

	static int physicsLoop(void *ptr /*type stripped point to the class */){
		int newTicks;

		Game *g = (Game *)ptr;
		g->ticks = SDL_GetTicks();

		while (g->is_running){
		  	newTicks = SDL_GetTicks();
			double dt = double(newTicks-g->ticks)/1000.0;

			g->update(dt);
			SDL_Delay(10);

			g->ticks = newTicks;
		}

		return 0;
	}

	static int renderLoop(void *ptr){
		Game *g = (Game *)ptr;

		while (g->is_running){
		  	g->render();
			SDL_Delay(10);
		}

		return 0;
	}

	void run (){
		int newTicks;
		is_running = true;
		SDL_Event e;
        
		SDL_Thread *physicsThread=SDL_CreateThread(Game::physicsLoop,"Physics",(void *)this);
		SDL_Thread *renderThread=SDL_CreateThread(Game::renderLoop,"Render",(void *) this);
		
		while (is_running){
			while (SDL_PollEvent(&e)){
				if (e.type == SDL_QUIT) 
					is_running = false;
				else if (e.type==SDL_KEYDOWN) handleKeyDown(e);
				else if (e.type==SDL_KEYUP) handleKeyUp(e);
			}
		}

		int retVal;
		SDL_WaitThread(physicsThread,&retVal);
		SDL_WaitThread(renderThread,&retVal);
	}

	virtual void update(double dt /*s of elapsed time*/) = 0;
	virtual void render() = 0;

	virtual void handleKeyUp(SDL_Event key) = 0;
	virtual void handleKeyDown(SDL_Event key) = 0;
    
	~Game(){
		SDL_DestroyRenderer(ren);
		SDL_DestroyWindow(window);
		Mix_CloseAudio();
		SDL_Quit();	
	}
};