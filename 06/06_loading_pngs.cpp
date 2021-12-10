#include <SDL2/SDL_events.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_video.h>
#include <cstdio>
#include <string>
#include <iostream>
#include <vector>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
using namespace std;

const int WIDTH = 640, HEIGHT = 480;

SDL_Window* window = nullptr;
SDL_Surface* screenSurface = nullptr;

bool init(){
    if(SDL_Init(SDL_INIT_VIDEO) < 0){
        printf("%s\n", SDL_GetError());
        return false;
    }
    window = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);    
    if(window == nullptr){
        printf("%s\n", SDL_GetError());
        return false;
    }

    if(!(IMG_INIT_PNG & IMG_Init(IMG_INIT_PNG))){
        printf("%s\n", IMG_GetError());
        return false;
    }

    screenSurface = SDL_GetWindowSurface(window);
    return true;
}

SDL_Surface* loadMedia(string path){
    SDL_Surface* surface = nullptr; surface = IMG_Load(path.c_str());
    
    if(surface == nullptr)
        printf("Unable to load image %s! SDL_image ERROR: %s\n", path.c_str(), IMG_GetError());
    
    return surface;
}

void exit(vector<SDL_Surface*> &surfaces){
    for(int i=0; i < (int)surfaces.size(); i++){
        SDL_FreeSurface(surfaces[i]);
        surfaces[i] = nullptr;
    }
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();
}

int main(int argc, char* args[]){
    init();
    SDL_Surface* media = loadMedia("out.png");
    SDL_Rect rect; rect.x = rect.y = 200;
    rect.w = WIDTH/3; rect.h = HEIGHT/3; 

    SDL_BlitScaled(media, nullptr, screenSurface, &rect);
    SDL_UpdateWindowSurface(window);

    vector<SDL_Surface*> surfaces;
    surfaces.push_back(screenSurface); 
    surfaces.push_back(media);

    SDL_Event event;
    bool quit = false;

    while(!quit){
        while(SDL_PollEvent(&event)){
            if(event.type == SDL_QUIT) quit = true;
        }
    }

    exit(surfaces);
    return 0;
}
