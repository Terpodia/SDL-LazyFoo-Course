#include <iostream>
#include <SDL2/SDL.h>
using namespace std;

const int SCREEN_WIDTH = 640, SCREEN_HEIGHT = 480;
SDL_Window* screen;
SDL_Surface* screenSurface;
SDL_Surface* media;

void init(){
    SDL_Init(SDL_INIT_VIDEO);
    screen = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    screenSurface = SDL_GetWindowSurface(screen);
}

SDL_Surface* loadSurface(string path){
    SDL_Surface* surface = SDL_LoadBMP(path.c_str());
    SDL_Surface* optimizedSurface = SDL_ConvertSurface(surface, screenSurface->format, 0);
    SDL_FreeSurface(surface);
    return optimizedSurface;
}

void exit(){
    SDL_FreeSurface(screenSurface); screenSurface = nullptr;
    SDL_FreeSurface(media); media = nullptr;
    SDL_DestroyWindow(screen); screen = nullptr;
}

int main(int argc, char* args[]){
    init();
    media = loadSurface("preview.bmp"); 

    SDL_Rect stretchRect;
    stretchRect.x = stretchRect.y = 0;
    stretchRect.w = SCREEN_WIDTH; stretchRect.h = SCREEN_HEIGHT;

    SDL_BlitScaled(media, nullptr, screenSurface, &stretchRect);
    SDL_UpdateWindowSurface(screen);
    SDL_Event e;

    bool quit = false;

    while(!quit){
        while(SDL_PollEvent(&e) > 0){
            if(e.type == SDL_QUIT){
                quit = true;
            }            
        }
    }

    exit();

    return 0;
}
