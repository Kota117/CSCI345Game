#include <iostream>
#include <SDL.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <vector>
#include <map>
#include <math.h>
#include <string>
#include <SDL_mutex.h>

int mainMenu(){
	SDL_Window *window;                    // Declare a pointer

    SDL_Init(SDL_INIT_VIDEO);              // Initialize SDL2
	TTF_Init();

	int w = 640;
	int h = 480;

    // Create an application window with the following settings:
    window = SDL_CreateWindow(
        "Main Menu",                  // window title
        SDL_WINDOWPOS_UNDEFINED,      // initial x position
        SDL_WINDOWPOS_UNDEFINED,      // initial y position
        w,                            // width, in pixels
        h,                            // height, in pixels
        SDL_WINDOW_OPENGL             // flags - see below
    );

    // Check that the window was successfully created
    if(window == NULL){
        // In the case that the window could not be made...
        printf("Could not create window: %s\n", SDL_GetError());
        return 1;
    }

	SDL_Renderer *ren = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	SDL_Color black = {0, 0, 0, 255};
	SDL_Color white = { 225, 255, 255, 255};
	SDL_Color grey = {120, 120, 120, 255};

	TTF_Font *AovelSansRounded = TTF_OpenFont("media/fonts/aovel-sans-rounded-font/AovelSansRounded-rdDL.ttf", 25);
	
	SDL_Surface *startButtonSurface = TTF_RenderText_Solid(AovelSansRounded, "Start Game", white);
	SDL_Texture *startButtonTexture = SDL_CreateTextureFromSurface(ren, startButtonSurface);

	SDL_Surface *l2ButtonSurface = TTF_RenderText_Solid(AovelSansRounded, "Level Two", white);
	SDL_Texture *l2ButtonTexture = SDL_CreateTextureFromSurface(ren, l2ButtonSurface);

	SDL_Surface *l3ButtonSurface = TTF_RenderText_Solid(AovelSansRounded, "Level Two", white);
	SDL_Texture *l3ButtonTexture = SDL_CreateTextureFromSurface(ren, l3ButtonSurface);

	SDL_Surface *instructionsSurface = TTF_RenderText_Solid(AovelSansRounded, "Find the key and unlock the door to escape.", white);
	SDL_Texture *instructionsTexture = SDL_CreateTextureFromSurface(ren, instructionsSurface);

	int startTextW = 0;
	int startTextH = 0;
	int l2TextW = 0;
	int l2TextH = 0;
	int l3TextW = 0;
	int l3TextH = 0;
	int instructionsTextW = 0;
	int instructionsTextH = 0;

	SDL_QueryTexture(startButtonTexture, NULL, NULL, &startTextW, &startTextH);
	SDL_QueryTexture(l2ButtonTexture, NULL, NULL, &l2TextW, &l2TextH);
	SDL_QueryTexture(l3ButtonTexture, NULL, NULL, &l3TextW, &l3TextH);
	SDL_QueryTexture(instructionsTexture, NULL, NULL, &instructionsTextW, &instructionsTextH);

	SDL_Rect startDestRect = {w/2 - startTextW/2, h/2 - startTextH/2, startTextW, startTextH};
	SDL_Rect l2DestRect = {w/2 + l2TextW, h/2 - l2TextH/2, l2TextW, l2TextH};
	SDL_Rect l3DestRect = {w/2 - 2*l3TextW, h/2 - l3TextH/2, l3TextW, l3TextH};
	SDL_Rect instructionsDestRect = {w/2 - instructionsTextW/2, h/2 + instructionsTextH * 2, instructionsTextW, instructionsTextH};

	bool done = false;

	while (!done){
        SDL_Event e;
		
		int x, y;
		Uint32 buttons;

		SDL_PumpEvents();

		buttons = SDL_GetMouseState(&x, &y);

        if(SDL_PollEvent(&e)){
			if(e.type == SDL_QUIT){ 
				done = true; 
				break;
			} else if(e.type == SDL_MOUSEBUTTONDOWN){
			 	if (x > w/2 - startTextW/2 && y > h/2 - startTextH/2 && x < w/2 + startTextW/2 && y < h/2 + startTextH/2){	
					TTF_CloseFont(AovelSansRounded);
					SDL_DestroyTexture(startButtonTexture);
					SDL_FreeSurface(startButtonSurface);

					SDL_DestroyTexture(l2ButtonTexture);
					SDL_FreeSurface(l2ButtonSurface);

					SDL_DestroyTexture(l3ButtonTexture);
					SDL_FreeSurface(l3ButtonSurface);

					SDL_DestroyTexture(instructionsTexture);
					SDL_FreeSurface(instructionsSurface);

					// Close and destroy the window
					SDL_DestroyWindow(window);

					// Clean up
					TTF_Quit();
					SDL_Quit();
					return 1;
				}
			}
		}

		// make black background
		SDL_SetRenderDrawColor(ren, black.r, black.g, black.b, black.a);
		SDL_RenderClear(ren);
	
		// handle mouseover start button
		if(x > w/2 - startTextW/2 && y > h/2 - startTextH/2 && x < w/2 + startTextW/2 && y < h/2 + startTextH/2){
			// set box color white
			SDL_SetRenderDrawColor(ren, 255,255,255,255);
			startButtonSurface = TTF_RenderText_Solid(AovelSansRounded, "Start Game", black);
			startButtonTexture = SDL_CreateTextureFromSurface(ren, startButtonSurface);
		} else{
			// set box color black
			SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
			startButtonSurface = TTF_RenderText_Solid(AovelSansRounded, "Start Game", white);
			startButtonTexture = SDL_CreateTextureFromSurface(ren, startButtonSurface);
		}

		SDL_RenderFillRect(ren, &startDestRect);
		SDL_RenderCopy(ren, startButtonTexture, NULL, &startDestRect);

		// handle mouseover l2 button
		if(x > w/2 + l2TextW && y > h/2 - l2TextH/2 && x < w/2 + l2TextW*2 && y < h/2 + l2TextH/2){
			// set box color white
			SDL_SetRenderDrawColor(ren, 255,255,255,255);
			l2ButtonSurface = TTF_RenderText_Solid(AovelSansRounded, "Level Two", black);
			l2ButtonTexture = SDL_CreateTextureFromSurface(ren, l2ButtonSurface);
		} else{
			// set box color black
			SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
			l2ButtonSurface = TTF_RenderText_Solid(AovelSansRounded, "Level Two", white);
			l2ButtonTexture = SDL_CreateTextureFromSurface(ren, l2ButtonSurface);
		}

		SDL_RenderFillRect(ren, &l2DestRect);
		SDL_RenderCopy(ren, l2ButtonTexture, NULL, &l2DestRect);

		// handle mouseover l3 button
		if(x > w/2 - 2*l3TextW && y > h/2 - l3TextH/2  &&  x < w/2 - l3TextW && y < h/2 + l3TextH/2){
			// set box color white
			SDL_SetRenderDrawColor(ren, 255,255,255,255);
			l3ButtonSurface = TTF_RenderText_Solid(AovelSansRounded, "Level Three", black);
			l3ButtonTexture = SDL_CreateTextureFromSurface(ren, l3ButtonSurface);
		} else{
			// set box color black
			SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
			l3ButtonSurface = TTF_RenderText_Solid(AovelSansRounded, "Level Three", white);
			l3ButtonTexture = SDL_CreateTextureFromSurface(ren, l3ButtonSurface);
		}

		SDL_RenderFillRect(ren, &l3DestRect);
		SDL_RenderCopy(ren, l3ButtonTexture, NULL, &l3DestRect);

		SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
		SDL_RenderFillRect(ren, &instructionsDestRect);
		SDL_RenderCopy(ren, instructionsTexture, NULL, &instructionsDestRect);

		SDL_RenderPresent(ren);
    }

	TTF_CloseFont(AovelSansRounded);
	
	SDL_DestroyTexture(startButtonTexture);
	SDL_FreeSurface(startButtonSurface);

	SDL_DestroyTexture(l2ButtonTexture);
	SDL_FreeSurface(l2ButtonSurface);

	SDL_DestroyTexture(l3ButtonTexture);
	SDL_FreeSurface(l3ButtonSurface);

	SDL_DestroyTexture(instructionsTexture);
	SDL_FreeSurface(instructionsSurface);

    // Close and destroy the window
    SDL_DestroyWindow(window);

    // Clean up
	TTF_Quit();
    SDL_Quit();
    return 0;
}

int pauseMenu(){
	SDL_Window *window;                    // Declare a pointer

    SDL_Init(SDL_INIT_VIDEO);              // Initialize SDL2
	TTF_Init();

	int w = 640;
	int h = 480;

    // Create an application window with the following settings:
    window = SDL_CreateWindow(
        "Pause Menu",                  // window title
        SDL_WINDOWPOS_UNDEFINED,       // initial x position
        SDL_WINDOWPOS_UNDEFINED,       // initial y position
        w,                             // width, in pixels
        h,                             // height, in pixels
        SDL_WINDOW_OPENGL              // flags - see below
    );

    // Check that the window was successfully created
    if(window == NULL) {
        // In the case that the window could not be made...
        printf("Could not create window: %s\n", SDL_GetError());
        return 1;
    }

	SDL_SetWindowInputFocus(window);

	SDL_Renderer *ren = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	SDL_Color black = {0, 0, 0, 255};
	SDL_Color white = { 225, 255, 255, 255};
	SDL_Color grey = {120, 120, 120, 255};

	TTF_Font *AovelSansRounded = TTF_OpenFont("media/fonts/aovel-sans-rounded-font/AovelSansRounded-rdDL.ttf", 25);
	
	SDL_Surface *startButtonSurface = TTF_RenderText_Solid(AovelSansRounded, "Quit Game", white);
	SDL_Texture *startButtonTexture = SDL_CreateTextureFromSurface(ren, startButtonSurface);

	int startTextW = 0;
	int startTextH = 0;

	SDL_QueryTexture(startButtonTexture, NULL, NULL, &startTextW, &startTextH);

	SDL_Rect startDestRect = {w/2 - startTextW/2, h/2 - startTextH/2, startTextW, startTextH};

	bool done = false;

	while (!done){
        SDL_Event e;
		
		int x, y;
		Uint32 buttons;

		SDL_PumpEvents();

		buttons = SDL_GetMouseState(&x, &y);

        if(SDL_PollEvent(&e)){
			if(e.type == SDL_QUIT){ 
				done=true; 
				break;
			} else if(e.type == SDL_MOUSEBUTTONDOWN){
			 	if(x > w/2 - startTextW/2 && y > h/2 - startTextH/2 && x < w/2 + startTextW/2 && y < h/2 + startTextH/2){	
					TTF_CloseFont(AovelSansRounded);
					SDL_DestroyTexture(startButtonTexture);
					SDL_FreeSurface(startButtonSurface);

					// Close and destroy the window
					SDL_DestroyWindow(window);

					// Clean up
					TTF_Quit();
					SDL_Quit();
					return 1;
				}
			}
		}

		// make black background
		SDL_SetRenderDrawColor(ren, black.r, black.g, black.b, black.a);
		SDL_RenderClear(ren);
	
		// handle mouseover start button
		if(x > w/2 - startTextW/2 && y > h/2 - startTextH/2 && x < w/2 + startTextW/2 && y < h/2 + startTextH/2){
			// set box color white
			SDL_SetRenderDrawColor(ren, 255,255,255,255);
			startButtonSurface = TTF_RenderText_Solid(AovelSansRounded, "Quit Game", black);
			startButtonTexture = SDL_CreateTextureFromSurface(ren, startButtonSurface);
		} else{
			// set box color black
			SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
			startButtonSurface = TTF_RenderText_Solid(AovelSansRounded, "Quit Game", white);
			startButtonTexture = SDL_CreateTextureFromSurface(ren, startButtonSurface);
		}

		SDL_RenderFillRect(ren, &startDestRect);
		SDL_RenderCopy(ren, startButtonTexture, NULL, &startDestRect);

		SDL_RenderPresent(ren);
    }

	TTF_CloseFont(AovelSansRounded);
	
	SDL_DestroyTexture(startButtonTexture);
	SDL_FreeSurface(startButtonSurface);

    // Close and destroy the window
    SDL_DestroyWindow(window);

    // Clean up
	TTF_Quit();
    SDL_Quit();
    return 0;
}