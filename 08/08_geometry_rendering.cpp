#include <SDL2/SDL.h>
#include <iostream>

const int WIDTH = 640, HEIGHT = 480; 
SDL_Window* window = nullptr;
SDL_Renderer* screenRenderer = nullptr;

void init(){
    SDL_Init(SDL_INIT_VIDEO);
    window = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN); 
    screenRenderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
}

void exit(){
    SDL_DestroyRenderer(screenRenderer);
    screenRenderer = nullptr;

    SDL_DestroyWindow(window);
    window = nullptr;
    SDL_Quit();
}

int main(int argc, char* args[]){
    init();

    bool quit = false;
    SDL_Event event;
    while(!quit){
        SDL_SetRenderDrawColor(screenRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_RenderClear(screenRenderer);
        
        SDL_Rect fillRect = {WIDTH/4, HEIGHT/4, WIDTH/2, HEIGHT/2};
        SDL_SetRenderDrawColor(screenRenderer, 0xFF, 0x00, 0x00, 0xFF); 
        SDL_RenderFillRect(screenRenderer, &fillRect);

        SDL_Rect outlineRect = {WIDTH/6, HEIGHT/6, WIDTH * 2/3, HEIGHT * 2/3};
        SDL_SetRenderDrawColor(screenRenderer, 0x00, 0xFF, 0x00, 0xFF); 
        SDL_RenderDrawRect(screenRenderer, &outlineRect);
        
        SDL_SetRenderDrawColor(screenRenderer, 0x00, 0x00, 0xFF, 0xFF);
        SDL_RenderDrawLine(screenRenderer, 0, HEIGHT/2, WIDTH, HEIGHT/2);

        SDL_SetRenderDrawColor(screenRenderer, 0xFF, 0xFF, 0x00, 0xFF);
        for(int y=0; y<HEIGHT; y+=4)
            SDL_RenderDrawPoint(screenRenderer, WIDTH/2, y);

        SDL_RenderPresent(screenRenderer);

        while(SDL_PollEvent(&event)>0){
            if(event.type == SDL_QUIT)
                quit = true;
        }
    }

    exit();
    return 0;
}
