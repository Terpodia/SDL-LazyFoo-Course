#include <SDL2/SDL.h>
#include <cstdio>

const int SCREEN_WIDTH = 320, SCREEN_HEIGHT = 240;

SDL_Window* window = nullptr;
SDL_Surface *screenSurface = nullptr; 
SDL_Surface* image = nullptr; 

void init(){
    SDL_Init(SDL_INIT_VIDEO);

    window = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
    SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

    screenSurface = SDL_GetWindowSurface(window);
}

void loadMedia(){
    image = SDL_LoadBMP("preview.bmp");
}

void exit(){
    SDL_FreeSurface(screenSurface);
    screenSurface = nullptr;
    SDL_FreeSurface(image);
    image = nullptr;
    SDL_DestroyWindow(window);
    window = nullptr;
    SDL_Quit();
}

int main(int argc, char* args[]){
    init();
    loadMedia();
    
    SDL_Event e;

    bool quit = false;

    SDL_BlitSurface(image, nullptr, screenSurface, nullptr);
    SDL_UpdateWindowSurface(window);
    while(!quit){
        while(SDL_PollEvent(&e) > 0){
            if(e.type == SDL_QUIT){ quit = true; exit(); }
        }        
    }

    return 0;
}