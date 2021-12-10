#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <bits/stdc++.h>
using namespace std;

const int WIDTH = 640, HEIGHT = 480;

SDL_Window* window;
SDL_Texture* texture;
SDL_Renderer* renderer;

void init(){
    SDL_Init(SDL_INIT_VIDEO);    
    window = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    
    IMG_Init(IMG_INIT_PNG);
}

void load(){
    SDL_Surface* surface = IMG_Load("viewport.png");
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    surface = nullptr;
}

void exit(){
    SDL_DestroyRenderer(renderer);
    renderer = nullptr;
    SDL_DestroyTexture(texture);
    texture = nullptr;
    SDL_DestroyWindow(window);
    window = nullptr;

    SDL_Quit();
    IMG_Quit();
}

int main(int argc, char* args[]){
    init(); load();

    SDL_Event event;
    bool quit = false;

    while(!quit){
        SDL_RenderClear(renderer);

        SDL_Rect r1 = {0, 0, WIDTH/2, HEIGHT/2};
        SDL_RenderSetViewport(renderer, &r1);
        SDL_RenderCopy(renderer, texture, nullptr, nullptr);
        
        SDL_Rect r2 = {WIDTH/2, 0, WIDTH/2, HEIGHT/2};
        SDL_RenderSetViewport(renderer, &r2);
        SDL_RenderCopy(renderer, texture, nullptr, nullptr);
        
        SDL_Rect r3 = {0, HEIGHT/2, WIDTH, HEIGHT/2};
        SDL_RenderSetViewport(renderer, &r3);
        SDL_RenderCopy(renderer, texture, nullptr, nullptr);

        SDL_RenderPresent(renderer);

        while(SDL_PollEvent(&event) > 0){
            if(event.type == SDL_QUIT)
                quit = true;
        }
    }

    return 0;
}
