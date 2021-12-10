#include <SDL2/SDL_events.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_video.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
using namespace std;

const int WIDTH = 640, HEIGHT = 480; 

SDL_Window* window;
SDL_Renderer* screenRenderer;
SDL_Texture* texture;

void init(){
    SDL_Init(SDL_INIT_VIDEO); 
    window = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
    screenRenderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_SetRenderDrawColor(screenRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
    IMG_Init(IMG_INIT_PNG);
}

void loadSurface(){
    SDL_Surface *surface = IMG_Load("out.png");
    texture = SDL_CreateTextureFromSurface(screenRenderer, surface);
    SDL_FreeSurface(surface);
}

void exit(){
    SDL_DestroyTexture(texture);
    texture = nullptr;

    SDL_DestroyRenderer(screenRenderer);
    screenRenderer = nullptr;

    SDL_DestroyWindow(window);
    window = nullptr;

    SDL_Quit();
    IMG_Quit();
}

int main(int argv, char* args[]){
    init(); loadSurface();
    SDL_Event event;
    SDL_Rect rect;
    rect.h = HEIGHT/3; rect.w = WIDTH/3;
    rect.x = rect.y = 150;

    bool quit = false;

    while(!quit){
        SDL_RenderClear(screenRenderer);
        SDL_RenderCopy(screenRenderer, texture, nullptr, &rect);
        SDL_RenderPresent(screenRenderer);
        while(SDL_PollEvent(&event) > 0){
            if(event.type == SDL_QUIT)
                quit = true;
        } 
    }

    return 0;
}
