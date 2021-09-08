#include <iostream>
#include <SDL.h>

using namespace std;

int main(int argc, char* argv[]) {

    SDL_Window *window;                    // Declare a pointer

    SDL_Init(SDL_INIT_VIDEO);              // Initialize SDL2

    // Create an application window with the following settings:
    window = SDL_CreateWindow(
        "An SDL2 window",                  // window title
        SDL_WINDOWPOS_UNDEFINED,           // initial x position
        SDL_WINDOWPOS_UNDEFINED,           // initial y position
        640,                               // width, in pixels
        480,                               // height, in pixels
        SDL_WINDOW_OPENGL                  // flags - see below
    );

    // Check that the window was successfully created
    if (window == NULL) {
        // In the case that the window could not be made...
        cerr << "Could not create window: " << SDL_GetError() << endl;
        return 1;
    }

    SDL_Renderer *ren;
    
    ren = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (ren == NULL) {
		cerr << "Could not create renderer" << endl;
		return 1;
	}
	
    SDL_Surface *ob;
    ob = SDL_LoadBMP("media/image.bmp");
    if (ob == NULL) {
		cerr << "Could not load media/image.bmp" << endl;
		return 1;
	}

    SDL_Rect src, dest;
	src.w = ob->w;
	src.h = ob->h;
	dest.w = src.w;
	dest.h = src.h;
	src.x = 0;
	src.y = 0;
	dest.x = 0;
	dest.y = 0;

    SDL_Texture *bitmapTex;
	bitmapTex = SDL_CreateTextureFromSurface(ren, ob);
	if (bitmapTex == NULL) {
		cerr << "Could not create texture" << endl;
		return 1;
	}
	
	SDL_FreeSurface(ob);

    //Had to change to this loop instead of delay to see window on M1
    bool is_running = true;
    SDL_Event event;
    while (is_running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                is_running = false;
            }
        }

    dest.x++;
	dest.y++;
	SDL_RenderClear(ren);
	SDL_RenderCopy(ren, bitmapTex, &src, &dest);
	SDL_RenderPresent(ren);
	SDL_Delay(100);
    }

    // Close and destroy the window
    SDL_DestroyTexture(bitmapTex);
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(window);

    // Clean up
    SDL_Quit();
    return 0;
}